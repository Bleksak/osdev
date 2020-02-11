INCLUDE(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(i686-elf-gcc GNU)

set(CMAKE_C_SIZEOF_DATA_PTR 4)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_C_FLAGS "-D _x86 -Wpedantic -x c -Wno-cpp -std=c17 -Wall -Wextra -ffreestanding -march=i686 -ansi")
set(CMAKE_C_LINK_FLAGS "-nostdlib -lgcc -T ${PROJECT_SOURCE_DIR}/linker.ld")
set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <OBJECTS> -o <TARGET> <LINK_LIBRARIES> <CMAKE_C_LINK_FLAGS>")