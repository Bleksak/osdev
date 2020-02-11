mkdir -p build_bash/outfiles

string=`find "src" -name "*.c"`
asmstring=`find "src" -name "*.asm"`

readarray -t a <<<"$string"; declare a;
readarray -t b <<<"$asmstring"; declare b;

clear

ccnt=${#a[@]}
asmcnt=${#b[@]}

libcc_path=`i686-elf-gcc --print-libgcc-file-name`

for ((i=0;i<ccnt;i++))
do
    echo "Compiling ${a[i]}"
    mkdir -p build_bash/outfiles/$(dirname "${a[i]}")
    i686-elf-gcc -c -g -O3 -I `pwd`/src --freestanding -Wall -Wextra ${a[i]} -o build_bash/outfiles/${a[i]}.o -std=c17
    a[i]="build_bash/outfiles/${a[i]}.o"
done

for ((i=0;i<asmcnt;i++))
do
    echo "Assembling ${b[i]}"
    mkdir -p build_bash/outfiles/$(dirname ${b[i]})
    nasm -f elf32 ${b[i]} -o build_bash/outfiles/${b[i]}.o
    b[i]="build_bash/outfiles/${b[i]}.o"
done

echo "Linking kernel.elf"
i686-elf-gcc -nostdlib ${a[@]} ${b[@]} $libcc_path -o build_bash/kernel.elf -T linker.ld
