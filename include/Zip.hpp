#pragma once
#include <vector>
#include <filesystem>
#include <minizip/unzip.h>
#include <minizip/ioapi_mem.h>

unzFile unzOpenBuffer(const void *buffer, int size);

std::vector<std::filesystem::path> GetZipEntries(const void *buffer, int size);

std::vector<std::filesystem::path> GetZipFilenames(const void *buffer, int size);

bool ExtractZipFileContents(const void *buffer, int size, const char *szFilename, std::string &contents);