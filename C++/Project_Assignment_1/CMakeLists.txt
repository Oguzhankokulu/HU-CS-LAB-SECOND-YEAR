cmake_minimum_required(VERSION 3.10)

# Set project name
project(ClearVision VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable debugging flags
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Add source files
set(SOURCES
    src/main.cpp 
    src/SecretImage.cpp 
    src/GrayscaleImage.cpp 
    src/Filter.cpp
    src/Crypto.cpp
)

# Add header files (for clarity, though not strictly necessary for CMake)
set(HEADERS
    src/SecretImage.h
    src/GrayscaleImage.h
    src/Filter.h
    src/stb_image.h
    src/stb_image_write.h
    src/Crypto.h
)

# Add the executable
add_executable(clearvision ${SOURCES} ${HEADERS})

# Include directories (for headers)
target_include_directories(clearvision PRIVATE ${CMAKE_SOURCE_DIR})

# Set build type to Debug
set(CMAKE_BUILD_TYPE Debug)
