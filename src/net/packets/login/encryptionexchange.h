/**
 * @file encryptionexchange.h
 * @author Lygaen
 * @brief The file containing encryption request / response logic
 * @version 0.1
 * @date 2023-12-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_ENCRYPTIONEXCHANGE_H
#define MINESERVER_ENCRYPTIONEXCHANGE_H

#include <net/packet.h>
#include <plugins/luaheaders.h>

/**
 * @brief Encryption Request Packet
 *
 * Used in protocol encryption scheme,
 * for the server to ask to the client.
 *
 * It is not exposed to lua for security
 * reasons. A plugin should not handle
 * encryption and stuff.
 */
class EncryptionRequest : public IPacket
{
private:
    void write(IMCStream *stream) override;

public:
    /**
     * @brief Construct a new Encryption Request object
     *
     * @param verifyToken the verify token pointer
     * @param verifyTokenLength the length of the verify token
     */
    EncryptionRequest(std::byte *verifyToken, int verifyTokenLength) : IPacket(0x01), verifyToken(verifyToken), verifyTokenLength(verifyTokenLength) {}
    /**
     * @brief Destroy the Encryption Request object
     *
     */
    ~EncryptionRequest() override = default;

    /**
     * @brief The pointer to the verify token
     *
     */
    std::byte *verifyToken;
    /**
     * @brief The length of the verify token
     *
     */
    int verifyTokenLength;

    /**
    @deprecated should not be used, useless
    */
    void read(IMCStream *stream) override;
};

/**
 * @brief Encryption Response Packet
 *
 * Used to verify the verify token as well
 * as encrypting the channel with the shared secret.
 *
 * It is not exposed to lua for security
 * reasons. A plugin should not handle
 * encryption and stuff.
 */
class EncryptionResponse : public IPacket
{
private:
    /**
    @deprecated should not be used, useless
    */
    void write(IMCStream *stream) override;

public:
    /**
     * @brief Construct a new Encryption Response object
     *
     */
    EncryptionResponse() : IPacket(0x01) {}
    /**
     * @brief Destroy the Encryption Response object
     *
     */
    ~EncryptionResponse() override = default;

    /**
     * @brief The length of the shared secret
     *
     */
    size_t sharedSecretLength{};
    /**
     * @brief The shared secret between the two clients
     *
     * Already decrypted when reading it from the stream.
     */
    std::unique_ptr<std::byte[]> sharedSecret;
    /**
     * @brief The length of the verify token
     *
     */
    size_t verifyTokenLength{};
    /**
     * @brief The verify token
     *
     * Already decrypted as well when reading it from the stream.
     */
    std::unique_ptr<std::byte[]> verifyToken;

    /**
     * @brief Reads the packet from the stream
     *
     * @param stream the stream to read from
     */
    void read(IMCStream *stream) override;
};

#endif // MINESERVER_ENCRYPTIONEXCHANGE_H