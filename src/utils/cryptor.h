#ifndef __MINESERVER_CRYPTOR_H__
#define __MINESERVER_CRYPTOR_H__

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/engine.h>
#include <string>
#include <tuple>

namespace cryptor
{
    void rsaInit();
    std::tuple<unsigned char *, int> rsaEncrypt(char *data, int len);
    std::tuple<unsigned char *, int> rsaDecrypt(char *data, int len);
}

#endif // __MINESERVER_CRYPTOR_H__