set(CMAKE_SYSTEM Generic)
set(CMAKE_SYSTEM_NAME Generic)

INCLUDE(CMakeForceCompiler)

# set(CMAKE_C_COMPILER clang)

CMAKE_FORCE_C_COMPILER(clang Clang)

set(CMAKE_C_SIZEOF_DATA_PTR 4)

# set(CMAKE_C_LINK_EXECUTABLE "i686-elf-gcc <OBJECTS> -o <TARGET> <LINK_LIBRARIES> <CMAKE_C_LINK_FLAGS>")

set(CMAKE_BUILD_TYPE Release)

execute_process(COMMAND i686-elf-gcc "-print-libgcc-file-name" OUTPUT_VARIABLE FILE_LIBGCC)

set(CMAKE_C_FLAGS "-D _x86 -Wpedantic -x c -Wall -Wextra -ffreestanding -ansi -std=c17 --target=i686-pc-none-elf -march=i686")
set(CMAKE_C_LINK_FLAGS "-T${PROJECT_SOURCE_DIR}/linker.ld -flto --target=i686-pc-none-elf -nostdlib ${FILE_LIBGCC}")
set(CMAKE_C_LINK_EXECUTABLE "clang <OBJECTS> -o <TARGET> <LINK_LIBRARIES> <CMAKE_C_LINK_FLAGS>")
