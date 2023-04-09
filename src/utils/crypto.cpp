#include "crypto.h"
#include <vector>
#include <algorithm>
#include <openssl/rand.h>
#include <openssl/x509.h>
#include <cstring>

#define KEY_LENGTH 1024

static EVP_PKEY *keypair;

bool crypto::init()
{
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (!ctx || !EVP_PKEY_keygen_init(ctx))
    {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, KEY_LENGTH);
    EVP_PKEY_keygen(ctx, &keypair);

    EVP_PKEY_CTX_free(ctx);
    return true;
}

void crypto::cleanup()
{
    EVP_PKEY_free(keypair);
}

std::unique_ptr<std::byte[]> crypto::rsaEncrypt(const std::byte *data, size_t len, size_t *outLen)
{
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(keypair, nullptr);
    *outLen = ULONG_MAX;
    if (!ctx || !EVP_PKEY_encrypt_init(ctx) || !EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING))
    {
        EVP_PKEY_CTX_free(ctx);
        return std::unique_ptr<std::byte[]>();
    }

    if (!EVP_PKEY_encrypt(ctx, nullptr, outLen, (const unsigned char *)data, len))
    {
        EVP_PKEY_CTX_free(ctx);
        return std::unique_ptr<std::byte[]>();
    }

    std::byte *out = new std::byte[*outLen];
    if (!EVP_PKEY_encrypt(ctx, (unsigned char *)out, outLen, (const unsigned char *)data, len))
    {
        EVP_PKEY_CTX_free(ctx);
        return std::unique_ptr<std::byte[]>();
    }

    return std::unique_ptr<std::byte[]>(out);
}

std::unique_ptr<std::byte[]> crypto::rsaDecrypt(const std::byte *data, size_t len, size_t *outLen)
{
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(keypair, nullptr);
    if (!ctx || !EVP_PKEY_decrypt_init(ctx) || !EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING))
    {
        EVP_PKEY_CTX_free(ctx);
        *outLen = 0;
        return std::unique_ptr<std::byte[]>();
    }

    if (!EVP_PKEY_decrypt(ctx, nullptr, outLen, (const unsigned char *)data, len))
    {
        EVP_PKEY_CTX_free(ctx);
        return std::unique_ptr<std::byte[]>();
    }

    std::byte *out = new std::byte[*outLen];
    if (!EVP_PKEY_decrypt(ctx, (unsigned char *)out, outLen, (const unsigned char *)data, len))
    {
        EVP_PKEY_CTX_free(ctx);
        return std::unique_ptr<std::byte[]>();
    }

    return std::unique_ptr<std::byte[]>(out);
}

std::unique_ptr<std::byte[]> crypto::getPublicRSAKey()
{
    X509_PUBKEY *pubkey = X509_PUBKEY_new();
    X509_PUBKEY_set(&pubkey, keypair);

    unsigned char *der;
    int len = i2d_X509_PUBKEY(pubkey, &der);
    if (len <= 0)
    {
        X509_PUBKEY_free(pubkey);
        return std::unique_ptr<std::byte[]>();
    }

    std::byte *out = new std::byte[len];

    // Ye, ye, not efficient but whatever, it's
    // not like it is called 200 times per seconds
    std::memcpy(out, der, len);

    OPENSSL_free(der);
    X509_PUBKEY_free(pubkey);

    return std::unique_ptr<std::byte[]>(out);
}

std::unique_ptr<std::byte[]> crypto::randomSecure(size_t len)
{
    std::byte *data = new std::byte[len];
    if (RAND_bytes((unsigned char *)data, len))
        return std::unique_ptr<std::byte[]>(data);
    return std::unique_ptr<std::byte[]>();
}

crypto::MinecraftHash::MinecraftHash()
{
    ctx = EVP_MD_CTX_new();
    EVP_DigestInit(ctx, EVP_sha1());
}

crypto::MinecraftHash::~MinecraftHash()
{
    EVP_MD_CTX_free(ctx);
}

void crypto::MinecraftHash::update(const std::string &s)
{
    EVP_DigestUpdate(ctx, s.c_str(), s.size());
}

std::string crypto::MinecraftHash::finalize()
{
    unsigned char data[EVP_MAX_MD_SIZE];
    unsigned int size;
    if (!EVP_DigestFinal(ctx, data, &size))
        return "";

    std::string result;
    BIGNUM *bn = BN_bin2bn(data, size, nullptr);

    if (BN_is_bit_set(bn, 159))
    {
        result += '-';

        auto tmp = std::vector<unsigned char>(BN_num_bytes(bn));
        BN_bn2bin(bn, tmp.data());
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), [](unsigned char b)
                       { return ~b; });
        BN_bin2bn(tmp.data(), tmp.size(), bn);

        BN_add_word(bn, 1);
    }

    char *hex = BN_bn2hex(bn);

    auto view = std::string_view(hex);
    while (view.size() && view[0] == '0')
        view = view.substr(1);

    result.append(view.begin(), view.end());
    OPENSSL_free(hex);
    BN_free(bn);

    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });

    EVP_DigestInit(ctx, EVP_sha1()); // so it is reusable
    return result;
}

crypto::AES128CFB8Cipher::AES128CFB8Cipher(CipherState state, const std::byte *key, const std::byte *iv) : state(state)
{
    ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    if (state == crypto::CipherState::ENCRYPT)
        EVP_EncryptInit(ctx, EVP_aes_128_cfb8(), (const unsigned char *)key, (const unsigned char *)iv);
    else if (state == crypto::CipherState::DECRYPT)
        EVP_DecryptInit(ctx, EVP_aes_128_cfb8(), (const unsigned char *)key, (const unsigned char *)iv);
}

crypto::AES128CFB8Cipher::~AES128CFB8Cipher()
{
    EVP_CIPHER_CTX_free(ctx);
}

int crypto::AES128CFB8Cipher::update(const std::byte *data, size_t len, std::byte *out)
{
    int outLen;
    if (state == crypto::CipherState::ENCRYPT)
        EVP_EncryptUpdate(ctx, (unsigned char *)out, &outLen, (const unsigned char *)data, len);
    else if (state == crypto::CipherState::DECRYPT)
        EVP_DecryptUpdate(ctx, (unsigned char *)out, &outLen, (const unsigned char *)data, len);

    return outLen;
}

int crypto::AES128CFB8Cipher::finalize(std::byte *out)
{
    int outLen;
    if (state == crypto::CipherState::ENCRYPT)
        EVP_EncryptFinal_ex(ctx, (unsigned char *)out, &outLen);
    else if (state == crypto::CipherState::DECRYPT)
        EVP_DecryptFinal_ex(ctx, (unsigned char *)out, &outLen);
    return outLen;
}

size_t crypto::AES128CFB8Cipher::calculateBufferSize(size_t len)
{
    int bs = EVP_CIPHER_CTX_block_size(ctx);
    if (state == crypto::CipherState::ENCRYPT)
        return len + bs - (len % bs);
    else if (state == crypto::CipherState::DECRYPT)
        return len;

    return 0;
}

crypto::ZLibCompressor::ZLibCompressor(int level) : compressionLevel(level)
{
}

std::unique_ptr<std::byte[]> crypto::ZLibCompressor::deflate(const std::byte *data, size_t len, int *outLen)
{
    z_stream zs = {0};
    *outLen = 0;

    if (deflateInit(&zs, compressionLevel) != Z_OK)
        return std::unique_ptr<std::byte[]>();

    zs.next_in = (Bytef *)data;
    zs.avail_in = len;

    std::byte *out = new std::byte[len];

    zs.next_out = (Bytef *)out;
    zs.avail_out = len;

    if (::deflate(&zs, Z_FINISH) != Z_STREAM_END)
    {
        delete[] out;
        return std::unique_ptr<std::byte[]>();
    }

    deflateEnd(&zs);
    *outLen = zs.total_out;

    return std::unique_ptr<std::byte[]>(out);
}

std::unique_ptr<std::byte[]> crypto::ZLibCompressor::inflate(const std::byte *data, size_t len, int *outLen)
{
    z_stream zs = {0};

    if (inflateInit(&zs) != Z_OK)
    {
        *outLen = 0;
        return std::unique_ptr<std::byte[]>();
    }

    zs.next_in = (Bytef *)data;
    zs.avail_in = len;

    std::byte *out = new std::byte[*outLen];

    zs.next_out = (Bytef *)out;
    zs.avail_out = *outLen;

    if (::inflate(&zs, Z_FINISH) != Z_STREAM_END)
    {
        *outLen = 0;
        delete[] out;
        return std::unique_ptr<std::byte[]>();
    }

    inflateEnd(&zs);
    *outLen = zs.total_out;

    return std::unique_ptr<std::byte[]>(out);
}
