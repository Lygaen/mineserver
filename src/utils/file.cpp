/**
 * @file file.cpp
 * @author Lygaen
 * @brief The file containing the logic for system file handling
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "file.h"
#include <fstream>
#include <net/stream.h>
#include <openssl/evp.h>
#include <utils/logger.h>

File::File() : path("")
{
}

File::File(std::string path) : path(path)
{
    load();
}

File::~File()
{
}

bool File::load()
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    data.resize(size);
    file.read(data.data(), size);

    return file.good();
}

void File::setPath(std::string path)
{
    this->path = path;
}

const std::string &File::getPath() const
{
    return path;
}

const char *File::getPointer() const
{
    return data.data();
}

int File::getSize() const
{
    return data.size();
}

PNGFile::PNGFile() : File()
{
}

PNGFile::PNGFile(std::string path) : File(path), width(0), height(0)
{
    MemoryStream m;
    m.write(reinterpret_cast<std::byte *>(const_cast<char *>(getPointer())), 1, 15 + 8);

    char buff[3];
    m.read(reinterpret_cast<std::byte *>(buff), 0, 3);

    if (buff[0] != 'P' || buff[1] != 'N' || buff[2] != 'G')
        return;

    // Crafty way to just skip 12 bytes
    m.readLong();
    m.readInt();

    int32_t temp = m.readInt();
    width = *reinterpret_cast<unsigned int *>(&temp);
    temp = m.readInt();
    height = *reinterpret_cast<unsigned int *>(&temp);

    int encodedLen = ((getSize() + 2) / 3) * 4;
    char *encoded = new char[encodedLen + 1];
    EVP_EncodeBlock(reinterpret_cast<unsigned char *>(encoded), reinterpret_cast<unsigned char *>(const_cast<char *>(getPointer())), getSize());
    encoded[encodedLen] = '\0';
    base64String = std::string(encoded, encodedLen + 1);
    delete[] encoded;
}

PNGFile::~PNGFile()
{
}

unsigned int PNGFile::getWidth() const
{
    return width;
}

unsigned int PNGFile::getHeight() const
{
    return height;
}

const std::string &PNGFile::getBase64String() const
{
    return base64String;
}
