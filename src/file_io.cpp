#include "file_io.h"
#include <fstream>
#include <sstream>
#include <algorithm>

std::string FileIO::readFile(const std::string& path) const {
    std::ifstream file(path);
    
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return buffer.str();
}

FileType FileIO::detectType(const std::string& path) const {
    // Find last dot for extension
    size_t dotPos = path.find_last_of('.');
    
    if (dotPos == std::string::npos) {
        return FileType::TEXT;
    }
    
    std::string ext = path.substr(dotPos);
    
    // Convert to lowercase
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    // Check for code extensions
    if (ext == ".cpp" || ext == ".c" || ext == ".h" || ext == ".hpp" ||
        ext == ".java" || ext == ".py" || ext == ".js" || ext == ".ts" ||
        ext == ".cc" || ext == ".cxx") {
        return FileType::CODE;
    }
    
    return FileType::TEXT;
}

bool FileIO::writeFile(const std::string& path, const std::string& content) const {
    std::ofstream file(path);
    
    if (!file.is_open()) {
        return false;
    }
    
    file << content;
    file.close();
    
    return true;
}

bool FileIO::fileExists(const std::string& path) const {
    std::ifstream file(path);
    return file.good();
}
