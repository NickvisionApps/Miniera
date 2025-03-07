#ifndef ZIPFILE_H
#define ZIPFILE_H

#include <filesystem>
#include <functional>
#include <vector>
#include <zip.h>

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief A model of a zip file.
     */
    class ZipFile
    {
    public:
        /**
         * @brief Constructs a ZipFile.
         * @param path The path to the zip file
         */
        ZipFile(const std::filesystem::path& path);
        /**
         * @brief Destructs a ZipFile.
         */
        ~ZipFile();
        /**
         * @brief Gets the path to the zip file.
         * @return The path to the zip file
         */
        const std::filesystem::path& getPath() const;
        /**
         * @brief Gets whether or not the ZipFile object is valid.
         * @return True if valid
         * @return False if not valid (error opening zip file)
         */
        bool isValid() const;
        /**
         * @brief Gets a list of entries in the zip file.
         * @return The list of entries
         */
        std::vector<std::filesystem::path> getEntries() const;
        /**
         * @brief Extracts the zip file to a directory.
         * @param dir The directory to extract too
         * @param progress An optional progress function callback
         * @return True if successful
         * @return False if failed
         */
        bool extract(const std::filesystem::path& dir, const std::function<void(double)>& progress = {});
        /**
         * @brief Gets whether or not the ZipFile object is valid.
         * @return True if valid
         * @return False if not valid (error opening zip file)
         */
        operator bool() const;

    private:
        std::filesystem::path m_path;
        zip_t* m_zip;
    };
}

#endif //ZIPFILE_H
