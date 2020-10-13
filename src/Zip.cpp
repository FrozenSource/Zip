#include <Zip.hpp>

unzFile unzOpenBuffer(const void *buffer, int size)
{
    zlib_filefunc_def filefunc32 = {0};
    ourmemory_t *punzmem = (ourmemory_t *)malloc(sizeof(ourmemory_t));
    punzmem->size = size;
    punzmem->base = (char *)malloc(punzmem->size);
    memcpy(punzmem->base, buffer, punzmem->size);
    punzmem->grow = 0;
    punzmem->cur_offset = 0;
    punzmem->limit = 0;
    fill_memory_filefunc(&filefunc32, punzmem);
    return unzOpen2(NULL, &filefunc32);
}

std::vector<std::filesystem::path> GetZipEntries(const void *buffer, int size)
{
    std::vector<std::filesystem::path> results;
    unzFile zip = unzOpenBuffer(buffer, size);
    if (zip)
    {
        unz_global_info info;
        int rv = unzGetGlobalInfo(zip, &info);

        if (UNZ_OK == unzGoToFirstFile(zip))
        {
            do
            {
                char szFilename[BUFSIZ];
                if (UNZ_OK == unzGetCurrentFileInfo(zip, NULL, szFilename, sizeof(szFilename), NULL, 0, NULL, 0))
                    results.push_back(std::filesystem::path(szFilename));
            } while (UNZ_OK == unzGoToNextFile(zip));
        }
    }
    return results;
}

std::vector<std::filesystem::path> GetZipFilenames(const void *buffer, int size)
{
    auto oEntries = GetZipEntries(buffer, size);
    for (size_t uiIndex = 0; uiIndex < oEntries.size(); uiIndex++)
    {
        string sEntry = oEntries[uiIndex].string();
        if (sEntry.at(sEntry.size() - 1) == '/' || sEntry.at(sEntry.size() - 1) == '\\')
        {
            oEntries.erase(oEntries.begin() + uiIndex);
            uiIndex--;
        }
    }
    return oEntries;
}

bool ExtractZipFileContents(const void *buffer, int size, const char *szFilename, std::string &contents)
{
    bool result = false;
    unzFile zip = unzOpenBuffer(buffer, size);
    if (zip)
    {
        if (UNZ_OK == unzLocateFile(zip, szFilename, 0))
        {
            if (UNZ_OK == unzOpenCurrentFile(zip))
            {
                char _buffer[BUFSIZ];
                size_t bytes;
                while (0 < (bytes = unzReadCurrentFile(zip, _buffer, sizeof(_buffer))))
                {
                    contents.append(_buffer, bytes);
                }
                unzCloseCurrentFile(zip);
                result = (bytes == 0);
            }
        }
        unzClose(zip);
    }
    return result;
}