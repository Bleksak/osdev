INCLUDE(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(i686-elf-gcc GNU)
SET(CMAKE_C_SIZEOF_DATA_PTR 4)

set(CMAKE_C_LINK_FLAGS "-nostdlib -T ${PROJECT_SOURCE_DIR}/linker.ld -lgcc")

set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <OBJECTS> -o <TARGET> <LINK_LIBRARIES> <CMAKE_C_LINK_FLAGS>")

set(CMAKE_C_FLAGS "-D -Wpedantic _x86 -x c -Wno-cpp -std=c17 -Wall -Wextra -ffreestanding -nostdlib -nostdinc -march=i686 -msse2")
