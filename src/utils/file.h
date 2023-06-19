/**
 * @file file.h
 * @author Mathieu Cayeux
 * @brief The file containing file (lol) loading logic
 * @version 0.1
 * @date 2023-06-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_FILE_H
#define MINESERVER_FILE_H

#include <string>
#include <vector>

/**
 * @brief File Loader Wrapper
 *
 */
class File
{
private:
    std::vector<char> data;
    std::string path;

public:
    /**
     * @brief Constructs a new File object
     *
     * Constructs an empty file object, really
     * just used for the config or placeholder
     * objects.
     */
    File();
    /**
     * @brief Construct a new File object
     *
     * Internally calls File::load()
     * @param path The Path of the file
     */
    File(std::string path);
    /**
     * @brief Destroy the File object
     *
     */
    ~File();

    /**
     * @brief Loads the data of the file into ram
     *
     * @return true file was loaded correctly
     * @return false file was not loaded correctly
     */
    bool load();

    /**
     * @brief Set the Path of the file
     *
     * You should call File::load() to load
     * the file data afterwards.
     * @param path the path of the targeted file
     */
    void setPath(std::string path);
    /**
     * @brief Get the Path object
     *
     * @warning The path does not assure that the data
     * stored in this object contains the data
     * of the file at the path.
     * @return const std::string& the path of the file
     */
    const std::string &getPath() const;

    /**
     * @brief Get the pointer to the data
     *
     * @return const char* the pointer to the file data
     */
    const char *getPointer() const;
    /**
     * @brief Get the size of the file stored
     *
     * @return int the size of the file stored
     */
    int getSize() const;
};

/**
 * @brief Wrapper around ::File for PNG files
 *
 */
class PNGFile : public File
{
private:
    unsigned int width, height;
    std::string base64String;

public:
    /**
     * @brief Construct a new PNGFile object
     *
     * Constructs an empty file object, really
     * just used for the config or placeholder
     * objects.
     */
    PNGFile();
    /**
     * @brief Construct a new PNGFile object
     *
     * Internally calls File::load(), then
     * does few calculations for loading
     * data used in the server.
     * @param path The Path of the file
     */
    PNGFile(std::string path);
    /**
     * @brief Destroy the PNGFile object
     *
     */
    ~PNGFile();

    /**
     * @brief Get the Width of the file
     *
     * @return unsigned int the width
     */
    unsigned int getWidth() const;
    /**
     * @brief Get the Height of the file
     *
     * @return unsigned int the height
     */
    unsigned int getHeight() const;
    /**
     * @brief Get the Base64 representation of the file
     *
     * @return const std::string& the base64 representation
     */
    const std::string &getBase64String() const;
};

#endif // MINESERVER_FILE_H