#include "crypto.h"
#include <vector>
#include <algorithm>
#include <openssl/rand.h>

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
    if (!ctx || !EVP_PKEY_encrypt_init(ctx))
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
    if (!ctx || !EVP_PKEY_decrypt_init(ctx))
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
