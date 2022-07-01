#include "cryptor.h"

namespace cryptor
{
    static unsigned char *publicKeyChar;
    static unsigned char *privateKeyChar;

    void rsaInit()
    {
        EVP_PKEY_CTX *keyCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
        EVP_PKEY_keygen_init(keyCtx);
        EVP_PKEY_CTX_set_rsa_keygen_bits(keyCtx, 1024);

        EVP_PKEY *key = nullptr;
        EVP_PKEY_keygen(keyCtx, &key);
        EVP_PKEY_CTX_free(keyCtx);

        BIO *privateBIO = BIO_new(BIO_s_mem());
        PEM_write_bio_PrivateKey(privateBIO, key, nullptr, nullptr, 0, 0, nullptr);
        int privateKeyLen = BIO_pending(privateBIO);
        privateKeyChar = (unsigned char *)malloc(privateKeyLen);
        BIO_read(privateBIO, privateKeyChar, privateKeyLen);

        BIO *publicBIO = BIO_new(BIO_s_mem());
        PEM_write_bio_PUBKEY(publicBIO, key);
        int publicKeyLen = BIO_pending(publicBIO);
        publicKeyChar = (unsigned char *)malloc(publicKeyLen);
        BIO_read(publicBIO, publicKeyChar, publicKeyLen);
    }

    std::tuple<unsigned char *, int> rsaEncrypt(char *data, int len)
    {
        BIO *rsaPublicBIO = BIO_new_mem_buf(publicKeyChar, -1);
        RSA *rsaPublicKey = nullptr;
        PEM_read_bio_RSA_PUBKEY(rsaPublicBIO, &rsaPublicKey, nullptr, nullptr);

        EVP_PKEY *publicKey = EVP_PKEY_new();
        EVP_PKEY_assign_RSA(publicKey, rsaPublicKey);

        unsigned char *ptr = (unsigned char *)malloc(RSA_size(rsaPublicKey));
        int encryptLen = RSA_public_encrypt(len, (unsigned char *)data, ptr, rsaPublicKey, RSA_PKCS1_PADDING);

        return {ptr, encryptLen};
    }

    std::tuple<unsigned char *, int> rsaDecrypt(char *data, int len)
    {
        BIO *rsaPrivateBIO = BIO_new_mem_buf(privateKeyChar, -1);
        RSA *rsaPrivateKey = nullptr;
        PEM_read_bio_RSAPrivateKey(rsaPrivateBIO, &rsaPrivateKey, nullptr, nullptr);
        EVP_PKEY *privateKey = EVP_PKEY_new();
        EVP_PKEY_assign_RSA(privateKey, rsaPrivateKey);

        unsigned char *decrypt = (unsigned char *)malloc(len);
        int decryptLen = RSA_private_decrypt(len, (unsigned char *)data, decrypt,
                                             rsaPrivateKey, RSA_PKCS1_PADDING);

        return {decrypt, decryptLen};
    }
}