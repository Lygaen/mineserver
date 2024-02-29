/**
 * @file encryptionexchange.cpp
 * @author Lygaen
 * @brief The file containing encryption request / response logic
 * @version 0.1
 * @date 2023-12-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "encryptionexchange.h"
#include <utils/crypto.h>

void EncryptionRequest::write(IMCStream *stream)
{
    stream->writeString("");

    int len;
    auto publicKey = crypto::getPublicRSAKey(&len);
    stream->writeVarInt(len);
    stream->write(publicKey.get(), 0, len);

    stream->writeVarInt(verifyTokenLength);
    stream->write(verifyToken, 0, verifyTokenLength);
}

void EncryptionRequest::read(IMCStream *stream)
{
    (void)stream;
    // Does nothing
    throw std::runtime_error("EncryptionRequest read should not be called !");
}

void EncryptionResponse::write(IMCStream *stream)
{
    (void)stream;
    // Does nothing
    throw std::runtime_error("EncryptionRequest write should not be called !");
}

void EncryptionResponse::read(IMCStream *stream)
{
    {
        int len = stream->readVarInt();
        std::byte buff[len];
        stream->read(buff, 0, len);

        sharedSecret = crypto::rsaDecrypt(buff, len, &sharedSecretLength);
    }
    {
        int len = stream->readVarInt();
        std::byte buff[len];
        stream->read(buff, 0, len);

        verifyToken = crypto::rsaDecrypt(buff, len, &verifyTokenLength);
    }
}
