/**
 * @file chatmessage.h
 * @author Lygaen
 * @brief The file containing minecraft chat message implementation
 * @version 1.0
 * @date 2023-04-08
 *
 * @copyright Copyright (c) 2023
 * @todo Chat message is missing hover event
 */

#ifndef MINESERVER_CHATMESSAGE_H
#define MINESERVER_CHATMESSAGE_H

#include <string>
#include <unordered_map>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <plugins/luaheaders.h>

/**
 * @brief Minecraft Chat Message implementation
 *
 * Following great <a href="https://wiki.vg/Text_formatting">Wiki.vg article</a>
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
     * @brief The text excluding extras
     *
     * Does not parse for additional string components,
     * only text.
     */
    std::string text{};

    /**
     * @brief Click Event
     *
     * On message click event
     */
    class ClickEvent
    {
    public:
        /**
         * @brief Type of action to perform
         *
         */
        enum ActionType
        {
            /**
             * @brief No action
             *
             * Click event will not be saved to JSON.
             */
            NONE,
            /**
             * @brief Open URL
             *
             */
            OPEN_URL,
            /**
             * @brief Run command
             *
             */
            RUN_COMMAND,
            /**
             * @brief Suggest command to user
             *
             */
            SUGGEST_COMMAND,
            /**
             * @brief Change page (book only)
             *
             */
            CHANGE_PAGE,
            /**
             * @brief Copy text to clipboard
             *
             */
            COPY_TO_CLIPBOARD
        };

        /**
         * @brief Action to perform
         *
         */
        ActionType action;
        /**
         * @brief Value of action
         *
         */
        std::string value;

        /**
         * @brief Construct a new Click Event object
         *
         * @param action the action type
         * @param value the value of action
         */
        ClickEvent(ActionType action, std::string value);
        /**
         * @brief Construct a new Click Event object
         *
         * @param changePage see ActionType::CHANGE_PAGE
         */
        ClickEvent(std::uint32_t changePage);
        /**
         * @brief Construct a new Click Event object
         *
         */
        ClickEvent();
        /**
         * @brief Destroy the Click Event object
         *
         */
        ~ClickEvent() = default;

        /**
         * @brief Load click event from JSON
         *
         * @param document the document to load from
         */
        void load(const rapidjson::Value &document);
        /**
         * @brief Save click event to JSON
         *
         * @param document the document to save to
         * @param alloc the document allocator
         */
        void save(rapidjson::Value &document, rapidjson::Document::AllocatorType &alloc) const;

        /**
         * @brief Loads Click Event to lua
         *
         * @param state the state of lua
         * @param namespaceName the namespace name
         */
        static void loadLua(lua_State *state, const char *namespaceName);
    };

    /**
     * @brief Click event for message
     *
     */
    ClickEvent clickEvent;

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
    ChatMessage(const std::string &msg);
    /**
     * @brief Construct a new Chat Message object
     *
     * @param msg the text of the chat message
     */
    ChatMessage(const std::string &&msg);
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

    /**
     * @brief Loads the ChatMessage class to a Lua one
     *
     * @param state the state to load to
     * @param namespaceName the namespace to load to
     */
    static void loadLua(lua_State *state, const char *namespaceName);
};

#ifndef DOXYGEN_IGNORE_THIS
template <>
struct luabridge::Stack<ChatMessage::ClickEvent::ActionType>
    : luabridge::Enum<ChatMessage::ClickEvent::ActionType,
                      ChatMessage::ClickEvent::ActionType::NONE,
                      ChatMessage::ClickEvent::ActionType::OPEN_URL,
                      ChatMessage::ClickEvent::ActionType::RUN_COMMAND,
                      ChatMessage::ClickEvent::ActionType::SUGGEST_COMMAND,
                      ChatMessage::ClickEvent::ActionType::CHANGE_PAGE,
                      ChatMessage::ClickEvent::ActionType::COPY_TO_CLIPBOARD>
{
};
#endif

#endif // MINESERVER_CHATMESSAGE_H