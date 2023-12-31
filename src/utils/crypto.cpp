/**
 * @file crypto.cpp
 * @author Lygaen
 * @brief The file containing the cryptography logic
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "crypto.h"
#include <vector>
#include <algorithm>
#include <openssl/rand.h>
#include <openssl/x509.h>
#include <openssl/md5.h>
#include <cstring>

static EVP_PKEY *keypair;

bool crypto::init()
{
    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_ciphers();
    OpenSSL_add_all_digests();

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (!ctx || !EVP_PKEY_keygen_init(ctx))
    {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, RSA_KEY_LENGTH);
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

std::unique_ptr<std::byte[]> crypto::getPublicRSAKey(int *outLen)
{
    unsigned char *buff = nullptr;
    int len = i2d_PUBKEY(keypair, &buff);

    std::byte *out = new std::byte[len];
    std::memcpy(out, buff, len);

    *outLen = len;
    OPENSSL_free(buff);
    return std::unique_ptr<std::byte[]>(out);
}

std::unique_ptr<std::byte[]> crypto::randomSecure(size_t len)
{
    std::byte *data = new std::byte[len];
    if (RAND_bytes((unsigned char *)data, len))
        return std::unique_ptr<std::byte[]>(data);
    delete[] data;
    return std::unique_ptr<std::byte[]>();
}

std::string crypto::md5Digest(const std::string &data)
{
    EVP_MD_CTX *context = EVP_MD_CTX_new();
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;
    std::string output;

    EVP_DigestInit(context, EVP_md5());
    EVP_DigestUpdate(context, data.c_str(), data.length());
    EVP_DigestFinal_ex(context, md_value, &md_len);
    EVP_MD_CTX_free(context);

    output.resize(md_len * 2);
    for (unsigned int i = 0; i < md_len; ++i)
        std::sprintf(&output[i * 2], "%02x", md_value[i]);
    return output;
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
    int outLen = 0;
    if (state == crypto::CipherState::ENCRYPT)
        EVP_EncryptUpdate(ctx, (unsigned char *)out, &outLen, (const unsigned char *)data, len);
    else if (state == crypto::CipherState::DECRYPT)
        EVP_DecryptUpdate(ctx, (unsigned char *)out, &outLen, (const unsigned char *)data, len);

    return outLen;
}

int crypto::AES128CFB8Cipher::finalize(std::byte *out)
{
    int outLen = 0;
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
    z_stream zs = z_stream();
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
    z_stream zs = z_stream();

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
