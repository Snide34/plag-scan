#include "file_io.h"
#include <gtest/gtest.h>
#include <fstream>

TEST(FileIOTest, DetectCodeType) {
    FileIO io;
    
    EXPECT_EQ(io.detectType("test.cpp"), FileType::CODE);
    EXPECT_EQ(io.detectType("test.c"), FileType::CODE);
    EXPECT_EQ(io.detectType("test.h"), FileType::CODE);
    EXPECT_EQ(io.detectType("test.java"), FileType::CODE);
    EXPECT_EQ(io.detectType("test.py"), FileType::CODE);
    EXPECT_EQ(io.detectType("test.js"), FileType::CODE);
    EXPECT_EQ(io.detectType("test.ts"), FileType::CODE);
}

TEST(FileIOTest, DetectTextType) {
    FileIO io;
    
    EXPECT_EQ(io.detectType("test.txt"), FileType::TEXT);
    EXPECT_EQ(io.detectType("test.md"), FileType::TEXT);
    EXPECT_EQ(io.detectType("test.doc"), FileType::TEXT);
    EXPECT_EQ(io.detectType("noextension"), FileType::TEXT);
}

TEST(FileIOTest, WriteAndReadFile) {
    FileIO io;
    std::string testPath = "test_temp_file.txt";
    std::string content = "Test content for file IO";
    
    ASSERT_TRUE(io.writeFile(testPath, content));
    
    std::string readContent = io.readFile(testPath);
    EXPECT_EQ(readContent, content);
    
    // Cleanup
    std::remove(testPath.c_str());
}

TEST(FileIOTest, FileExists) {
    FileIO io;
    
    // Create temp file
    std::string testPath = "test_exists.txt";
    std::ofstream file(testPath);
    file << "test";
    file.close();
    
    EXPECT_TRUE(io.fileExists(testPath));
    EXPECT_FALSE(io.fileExists("nonexistent_file_xyz.txt"));
    
    // Cleanup
    std::remove(testPath.c_str());
}

TEST(FileIOTest, ReadNonexistentFile) {
    FileIO io;
    
    EXPECT_THROW(io.readFile("nonexistent_file.txt"), std::runtime_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
