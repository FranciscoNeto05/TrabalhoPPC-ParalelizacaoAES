cmake_minimum_required(VERSION 3.15)

project(AES_Encryptor)

set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g3 -fopenmp -I/opt/homebrew/opt/openssl@3/include")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto")

add_executable(aes_encryptor index.cpp)

target_include_directories(aes_encryptor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(aes_encryptor ssl crypto)

add_custom_target(run
    COMMAND aes_encryptor
    DEPENDS aes_encryptor
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)