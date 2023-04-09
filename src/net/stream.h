/**
 * @file stream.h
 * @author Mathieu Cayeux
 * @brief The file containing data for all of the streams
 * @version 1.0
 * @date 2023-03-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_STREAM_H
#define MINESERVER_STREAM_H

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <utils/network.h>
#include <net/types/chatmessage.h>

/**
 * @brief Stream interface
 *
 * The interface that all the streams should
 * implement, and it has the necessary functions
 * for IO using the Minecraft protocol.
 */
class IStream
{
public:
    virtual ~IStream() = default;

    /**
     * @brief Reads data from the sub-class
     *
     * Standard-compliant way of reading from a stream.
     * Will write @p len bytes of data at maximum
     * to the @p buffer starting at @p offset
     * @param buffer the buffer to write to
     * @param offset the offset to start writing at
     * @param len the maximum length to write
     */
    virtual void read(std::byte *buffer, std::size_t offset, std::size_t len) = 0;
    /**
     * @brief Writes data to the sub-class
     *
     * Standard-compliant way of writing to a stream.
     * Will read a maximum of @p len bytes from @p buffer
     * starting at @p offset
     * @param buffer the buffer to read from
     * @param offset the offset to start reading from
     * @param len the maximum read length
     */
    virtual void write(std::byte *buffer, std::size_t offset, std::size_t len) = 0;
    /**
     * @brief Flushes the stream
     *
     * Empties the buffer, sending it correctly, resets any offset / data.
     */
    virtual void flush() = 0;

    /**
     * @brief Reads a Boolean
     *
     * Reads a boolean from the stream in a Minecrafty way.
     */
    bool readBoolean();
    /**
     * @brief Writes a Boolean
     *
     * Writes a boolean to the stream in a Minecraft way.
     * @param b the boolean to write
     */
    void writeBoolean(bool b);

    /**
     * @brief Reads a Byte
     *
     * Reads a byte from the stream in a Minecrafty way.
     */
    std::int8_t readByte();
    /**
     * @brief Writes a Byte
     *
     * Writes a byte to the stream in a Minecraft way.
     * @param b the byte to write
     */
    void writeByte(std::int8_t b);

    /**
     * @brief Reads an Unsigned Byte
     *
     * Reads an unsigned byte from the stream in a Minecrafty way.
     */
    std::uint8_t readUnsignedByte();
    /**
     * @brief Writes an Unsigned Byte
     *
     * Writes an unsigned byte to the stream in a Minecraft way.
     * @param b the unsigned byte to write
     */
    void writeUnsignedByte(std::uint8_t b);

    /**
     * @brief Reads a Short
     *
     * Reads a short from the stream in a Minecrafty way.
     */
    std::int16_t readShort();
    /**
     * @brief Writes a Short
     *
     * Writes a short to the stream in a Minecraft way.
     * @param s the short to write
     */
    void writeShort(std::int16_t s);

    /**
     * @brief Reads an Unsigned Short
     *
     * Reads an unsigned short from the stream in a Minecrafty way.
     */
    std::uint16_t readUnsignedShort();
    /**
     * @brief Writes an Unsigned short
     *
     * Writes a unsigned short to the stream in a Minecraft way.
     * @param s the unsigned short to write
     */
    void writeUnsignedShort(std::uint16_t s);

    /**
     * @brief Reads an Integer
     *
     * Reads an integer from the stream in a Minecrafty way.
     */
    std::int32_t readInt();
    /**
     * @brief Writes a Integer
     *
     * Writes an integer to the stream in a Minecraft way.
     * @param i the integer to write
     */
    void writeInt(std::int32_t i);

    /**
     * @brief Reads a Long
     *
     * Reads a long from the stream in a Minecrafty way.
     */
    std::int64_t readLong();
    /**
     * @brief Writes a Long
     *
     * Writes a long to the stream in a Minecraft way.
     * @param l the long to write
     */
    void writeLong(std::int64_t l);

    /**
     * @brief Reads a Float
     *
     * Reads a float from the stream in a Minecrafty way.
     */
    float readFloat();
    /**
     * @brief Writes a Float
     *
     * Writes a float to the stream in a Minecraft way.
     * @param f the float to write
     */
    void writeFloat(float f);

    /**
     * @brief Reads a Double
     *
     * Reads a double from the stream in a Minecrafty way.
     */
    double readDouble();
    /**
     * @brief Writes a Double
     *
     * Writes a double to the stream in a Minecraft way.
     * @param d the double to write
     */
    void writeDouble(double d);

    /**
     * @brief Reads a String
     *
     * Reads a string from the stream in a Minecrafty way.
     */
    std::string readString();
    /**
     * @brief Writes a String
     *
     * Writes a string to the stream in a Minecraft way.
     * @param s the string to write
     */
    void writeString(const std::string &s);

    /**
     * @brief Reads a Chat Message
     *
     * Reads a chat message from the stream in a Minecrafty way.
     * @todo Do something to ensure the return value is moved
     */
    ChatMessage readChat();
    /**
     * @brief Writes a Chat Message
     *
     * Writes a chat message to the stream in a Minecraft way.
     * @param s the string to write
     */
    void writeChat(const ChatMessage &c);

    /**
     * @brief Reads a Variable Integer
     *
     * Reads a variable integer from the stream in a Minecrafty way,
     * following google's standard on Variable Integers.
     */
    std::int32_t readVarInt();
    /**
     * @brief Writes a Variable Integer
     *
     * Writes a variable integer to the stream in a Minecraft way,
     * following google's standard on Variable Integers.
     * @param i the variable integer to write
     */
    void writeVarInt(std::int32_t i);

    /**
     * @brief Reads a Variable Long
     *
     * Reads a variable long from the stream in a Minecrafty way,
     * following google's standard on Variable Longs.
     */
    std::int64_t readVarLong();
    /**
     * @brief Writes a Variable Long
     *
     * Writes a variable long to the stream in a Minecraft way,
     * following google's standard on Variable Longs.
     * @param l the variable long to write
     */
    void writeVarLong(std::int64_t l);
};

/**
 * @brief A stream from Memory
 *
 * A simple wrapper, mainly used for
 * buffering to use a stream on memory,
 * with variable size using a vector.
 * Constructed like a passthrough stream :
 * you read and write to the same buffer.
 */
class MemoryStream : public IStream
{
private:
    std::vector<std::byte> data;
    std::uint32_t readIndex{};

public:
    /**
     * @brief Construct a new Memory Stream object
     */
    MemoryStream() = default;
    /**
     * @brief Destroy the Memory Stream object
     *
     */
    ~MemoryStream() = default;

    /**
     * @brief Reads from the stream
     *
     * Reads from the memory buffer of the stream.
     * @param buffer the buffer to write to
     * @param offset the offset to start writing at
     * @param len the maximum length to write
     */
    void read(std::byte *buffer, std::size_t offset, std::size_t len) override;
    /**
     * @brief Writes to the stream
     *
     * Writes to the memory of the stream, appending it.
     * @param buffer the buffer to read from
     * @param offset the offset to start reading from
     * @param len the maximum read length
     */
    void write(std::byte *buffer, std::size_t offset, std::size_t len) override;
    /**
     * @brief Flushes the stream
     *
     * Clears the buffer of the stream.
     */
    void flush() override;
};

/**
 * @brief Stream from a net client socket
 *
 * Stream for IO on a Network client socket
 */
class NetSocketStream : public IStream
{
private:
    ClientSocket socket;

public:
    /**
     * @brief Construct a new Net Socket Stream object
     *
     * @param socket the socket to IO on
     */
    NetSocketStream(ClientSocket socket);
    /**
     * @brief Destroy the Net Socket Stream object
     *
     */
    ~NetSocketStream();

    /**
     * @brief Reads from the network socket
     *
     * Reads from the network socket, writing it
     * to @p buffer
     * @param buffer the buffer to write to
     * @param offset the offset to start writing at
     * @param len the maximum length to write
     */
    void read(std::byte *buffer, std::size_t offset, std::size_t len) override;
    /**
     * @brief Writes to the network socket
     *
     * Writes to the network socket
     * @param buffer the buffer to read from
     * @param offset the offset to start reading from
     * @param len the maximum read length
     */
    void write(std::byte *buffer, std::size_t offset, std::size_t len) override;
    /**
     * @brief Flushes the stream
     * @deprecated Not implemented, useless, should not be used
     */
    void flush() override;
};

#endif // MINESERVER_STREAM_H