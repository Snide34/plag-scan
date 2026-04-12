#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>

enum class FileType {
    TEXT,
    CODE
};

class FileIO {
public:
    std::string readFile(const std::string& path) const;
    FileType detectType(const std::string& path) const;
    bool writeFile(const std::string& path, const std::string& content) const;
    bool fileExists(const std::string& path) const;
};

#endif // FILE_IO_H
