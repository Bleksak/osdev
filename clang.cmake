set(CMAKE_SYSTEM Generic)
set(CMAKE_SYSTEM_NAME Generic)

INCLUDE(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(clang GNU)

# set(CMAKE_C_COMPILER clang)
set(CMAKE_C_LINKER_PREFERENCE clang)

SET(CMAKE_C_SIZEOF_DATA_PTR 4)

set(CMAKE_C_FLAGS "-D _x86 -x c -std=c17 -Wall -Wextra -ffreestanding -nostdlib -nostdinc -msse2 --target=i686-pc-none-elf -march=i686")
set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
