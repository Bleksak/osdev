set(CMAKE_SYSTEM Generic)
set(CMAKE_SYSTEM_NAME Generic)

INCLUDE(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(clang Clang)

set(CMAKE_C_SIZEOF_DATA_PTR 4)

set(CMAKE_C_FLAGS "-D _x86 -Wpedantic -x c -Wall -Wextra -ffreestanding -nostdlib -nostdinc -msse2 -ansi -std=c17")
set(CMAKE_C_LINK_FLAGS "-nostdlib -lgcc -T${PROJECT_SOURCE_DIR}/linker.ld")
set(CMAKE_C_LINK_EXECUTABLE "i686-elf-gcc <OBJECTS> -o <TARGET> <LINK_LIBRARIES> <CMAKE_C_LINK_FLAGS>")
