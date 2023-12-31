/**
 * @file disconnect.cpp
 * @author Lygaen
 * @brief The file containg disconnect packet logic
 * @version 0.1
 * @date 2023-12-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "disconnect.h"

void DisconnectLogin::write(IMCStream *stream)
{
    stream->writeChat(reason);
}

void DisconnectLogin::read(IMCStream *stream)
{
    (void)stream;
    // Does nothing
    throw std::runtime_error("DisconnectLogin read should not be called !");
}

void DisconnectPlay::write(IMCStream *stream)
{
    stream->writeChat(reason);
}

void DisconnectPlay::read(IMCStream *stream)
{
    (void)stream;
    // Does nothing
    throw std::runtime_error("DisconnectPlay read should not be called !");
}
