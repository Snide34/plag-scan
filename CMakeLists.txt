cmake_minimum_required(VERSION 3.14)
project(PatternPlagiarismDetector VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project source files
set(SOURCES
    src/preprocessor.cpp
    src/pattern_matcher.cpp
    src/kmp.cpp
    src/rabin_karp.cpp
    src/lcs.cpp
    src/suffix_array.cpp
    src/similarity_engine.cpp
    src/scorer.cpp
    src/report.cpp
    src/file_io.cpp
)

set(HEADERS
    include/preprocessor.h
    include/pattern_matcher.h
    include/similarity_engine.h
    include/scorer.h
    include/file_io.h
)

# Create library
add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional: GoogleTest integration
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    
    add_executable(preprocessor_tests tests/preprocessor_test.cpp)
    target_link_libraries(preprocessor_tests 
        ${PROJECT_NAME}
        GTest::GTest 
        GTest::Main
    )
    
    add_executable(pattern_tests tests/test_pattern.cpp)
    target_link_libraries(pattern_tests 
        ${PROJECT_NAME}
        GTest::GTest 
        GTest::Main
    )
    
    add_executable(similarity_tests tests/test_similarity.cpp)
    target_link_libraries(similarity_tests 
        ${PROJECT_NAME}
        GTest::GTest 
        GTest::Main
    )
    
    add_executable(scorer_tests tests/test_scorer.cpp)
    target_link_libraries(scorer_tests 
        ${PROJECT_NAME}
        GTest::GTest 
        GTest::Main
    )
    
    add_executable(file_io_tests tests/test_file_io.cpp)
    target_link_libraries(file_io_tests 
        ${PROJECT_NAME}
        GTest::GTest 
        GTest::Main
    )
    
    add_executable(integration_tests tests/integration_test.cpp)
    target_link_libraries(integration_tests 
        ${PROJECT_NAME}
        GTest::GTest 
        GTest::Main
    )
    
    add_test(NAME PreprocessorTests COMMAND preprocessor_tests)
    add_test(NAME PatternTests COMMAND pattern_tests)
    add_test(NAME SimilarityTests COMMAND similarity_tests)
    add_test(NAME ScorerTests COMMAND scorer_tests)
    add_test(NAME FileIOTests COMMAND file_io_tests)
    add_test(NAME IntegrationTests COMMAND integration_tests)
endif()

# Main executable
add_executable(plagiarism_detector src/main.cpp)
target_link_libraries(plagiarism_detector ${PROJECT_NAME})
