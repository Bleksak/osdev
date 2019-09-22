INCLUDE(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(clang-9 GNU)

set(CMAKE_C_SIZEOF_DATA_PTR 4)

set(CMAKE_C_FLAGS "-D _x86 -Wpedantic -x c -Wall -Wextra -ffreestanding -nostdlib -nostdinc -msse2 --target=i686-pc-none-elf -march=i686 -ansi -std=c17")
set(CMAKE_C_LINK_FLAGS "-nostdlib -lgcc -T${PROJECT_SOURCE_DIR}/linker.ld --target=i686-pc-none-elf -march=i686")
set(CMAKE_C_LINK_EXECUTABLE "clang-9 <OBJECTS> -o <TARGET> <LINK_LIBRARIES> <CMAKE_C_LINK_FLAGS>")
