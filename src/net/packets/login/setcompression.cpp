/**
 * @file setcompression.cpp
 * @author Lygaen
 * @brief The file containing set compression packet logic
 * @version 0.1
 * @date 2023-12-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "setcompression.h"

void SetCompression::write(IMCStream *stream)
{
    stream->writeVarInt(threshold);
}

void SetCompression::read(IMCStream *stream)
{
    (void)stream;
    // Does nothing
    throw std::runtime_error("SetCompression read should not be called !");
}