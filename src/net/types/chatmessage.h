/**
 * @file chatmessage.h
 * @author Mathieu Cayeux
 * @brief The file containing minecraft chat message implementation
 * @version 1.0
 * @date 2023-04-08
 *
 * @copyright Copyright (c) 2023
 * @todo Chat message is very incomplete
 */

#ifndef MINESERVER_CHATMESSAGE_H
#define MINESERVER_CHATMESSAGE_H

#include <string>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

/**
 * @brief Minecraft Chat Message implementation
 *
 * Following great <a href="https://wiki.vg/Chat">Wiki.vg article</a>
 * on the implementation of Minecraft's chat system.
 */
class ChatMessage
{
public:
    /**
     * @brief Boldness for #text
     *
     */
    bool bold{};
    /**
     * @brief Italicness for #text
     *
     */
    bool italic{};
    /**
     * @brief Underline or not #text
     *
     */
    bool underlined{};
    /**
     * @brief Strikethrough or not #text
     *
     */
    bool strikethrough{};
    /**
     * @brief Obfuscate or not #text
     *
     */
    bool obfuscated{};
    /**
     * @brief Color of #text
     *
     * Should be one of the normal colors, but can also be a format code
     * (however, the fields relating to styles should be used instead for that purpose).
     * If not present (or set to reset), then the default color for the text will be used,
     * which varies by the situation (in some cases, it is white; in others, it is black;
     * in still others, it is a shade of gray that isn't normally used on text).
     */
    std::string color{};
    /**
     * @brief Text to insert
     *
     * Only used for messages in chat. When shift is held,
     * clicking the component inserts the given text into
     * the chat box at the cursor (potentially replacing selected text).
     * Has no effect on other locations at this time.
     */
    std::string insertion{};

    /**
     * @brief The (limited) text
     *
     * Does not parse for additional string components,
     * only text.
     *
     * @todo Add additional string component
     */
    std::string text{};

private:
    ChatMessage *next = nullptr;

public:
    /**
     * @brief Construct a new Chat Message object
     *
     */
    ChatMessage();
    /**
     * @brief Construct a new Chat Message object
     *
     * @param msg the text of the chat message
     */
    ChatMessage(std::string msg);
    /**
     * @brief Destroy the Chat Message object
     *
     */
    ~ChatMessage();

    /**
     * @brief Adds an extra
     *
     * Adds additional data to the chat message with
     * another ::ChatMessage
     * @param cm the chat message to add
     */
    void addExtra(ChatMessage *cm);

    /**
     * @brief Loads data from a document
     *
     * @param document The document to load from
     * @todo Contains an ugly of copying strings
     */
    void load(const rapidjson::Value &document);
    /**
     * @brief Saves data from a document
     *
     * @param document The document to save to
     * @param alloc The allocator of the document
     */
    void save(rapidjson::Value &document, rapidjson::Document::AllocatorType &alloc) const;

    /**
     * @brief Equality operator between two chat messages
     *
     * @param lhs the left-handside variable
     * @param rhs the righ-handside variable
     * @return true the two variables are equal
     * @return false the two variables are not equal
     */
    friend bool operator==(const ChatMessage &lhs, const ChatMessage &rhs);
};

#endif // MINESERVER_CHATMESSAGE_H