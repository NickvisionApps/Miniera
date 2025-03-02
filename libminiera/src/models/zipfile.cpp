#include "models/zipfile.h"
#include <cstring>
#include <fstream>
#include <vector>

namespace Nickvision::Miniera::Shared::Models
{
    ZipFile::ZipFile(const std::filesystem::path& path)
        : m_path{ path },
        m_zip{ zip_open(path.string().c_str(), 0, nullptr) }
    {

    }

    ZipFile::~ZipFile()
    {
        if(m_zip)
        {
            zip_close(m_zip);
        }
    }

    const std::filesystem::path& ZipFile::getPath() const
    {
        return m_path;
    }

    bool ZipFile::isValid() const
    {
        return m_zip;
    }

    std::vector<std::filesystem::path> ZipFile::getEntries() const
    {
        std::vector<std::filesystem::path> entries;
        if(m_zip)
        {
            for(zip_int64_t i = 0; i < zip_get_num_entries(m_zip, 0); i++)
            {
                struct zip_stat stat;
                zip_stat_index(m_zip, i, 0, &stat);
                entries.push_back(stat.name);
            }
        }
        return entries;
    }

    bool ZipFile::extract(const std::filesystem::path& dir)
    {
        if(!m_zip)
        {
            return false;
        }
        if(!std::filesystem::exists(dir))
        {
            std::filesystem::create_directories(dir);
        }
        for(zip_int64_t i = 0; i < zip_get_num_entries(m_zip, 0); i++)
        {
            struct zip_stat stat;
            zip_stat_index(m_zip, i, 0, &stat);
            std::filesystem::path filePath{ dir / stat.name };
            std::filesystem::create_directories(filePath);
            if(stat.name[strlen(stat.name) - 1] == '/')
            {
                continue;
            }
            zip_file_t* file{ zip_fopen_index(m_zip, i, 0) };
            if(!file)
            {
                return false;
            }
            std::vector<char> buffer(stat.size);
            zip_fread(file, buffer.data(), stat.size);
            zip_fclose(file);
            std::ofstream out{ filePath, std::ios::binary };
            out.write(buffer.data(), buffer.size());
        }
        return true;
    }

    ZipFile::operator bool() const
    {
        return isValid();
    }
}
