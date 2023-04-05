/**
 * @file crypto.h
 * @author Mathieu
 * @brief File containing all the logic for encryption
 * @version 0.1
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_CRYPTO_H
#define MINESERVER_CRYPTO_H

#include <memory>
#include <openssl/rsa.h>
#include <openssl/evp.h>

namespace crypto
{
    bool init();
    void cleanup();

    std::unique_ptr<std::byte[]> rsaEncrypt(const std::byte *data, size_t len, size_t *outLen);
    std::unique_ptr<std::byte[]> rsaDecrypt(const std::byte *data, size_t len, size_t *outLen);

    class MinecraftHash
    {
    private:
        EVP_MD_CTX *ctx;

    public:
        MinecraftHash();
        ~MinecraftHash();

        void update(const std::string &s);
        std::string finalize();
    };
};

#endif