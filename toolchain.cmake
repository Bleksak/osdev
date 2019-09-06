INCLUDE(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(i686-elf-gcc GNU)
SET(CMAKE_C_SIZEOF_DATA_PTR 4)

set(CMAKE_C_FLAGS "-D _x86 -x c -Wno-cpp -std=c17 -Wall -Wextra -ffreestanding -nostdlib -march=i686")
set(CMAKE_C_LINK_EXECUTABLE "/home/bleksak/opt/cross/bin/i686-elf-gcc <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_SHARED_LINKER_FLAGS "")

# SET(CMAKE_C_FLAGS "-O2 -g -ffreestanding -fbuiltin -Wall -Wextra -std=gnu99 -msse" CACHE STRING "" FORCE)