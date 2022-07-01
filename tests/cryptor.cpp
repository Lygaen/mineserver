#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <utils/cryptor.h>
#include <string>

TEST_CASE("RSA Encrypt/Decrypt")
{
    cryptor::rsaInit();

    std::string data = "Hello World ! This is some soon to be encrypted string !";
    auto encry = cryptor::rsaEncrypt(data.data(), data.size() + 1);
    auto encryptedData = std::string((char *)std::get<0>(encry));
    CHECK(encryptedData != data);

    auto decry = cryptor::rsaDecrypt((char *)std::get<0>(encry), std::get<1>(encry));
    auto decryptedData = std::string((char *)std::get<0>(decry));
    CHECK(decryptedData == data);
}