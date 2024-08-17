#/bin/bash
set -e

LINKER_SCRIPT="STR711-ROM.ld"

TOOLCHAIN="/home/tulip/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-"
TOOLCHAIN_GCC=$TOOLCHAIN"gcc"
TOOLCHAIN_GPP=$TOOLCHAIN"g++"
TOOLCHAIN_AS=$TOOLCHAIN"as"
TOOLCHAIN_SIZE=$TOOLCHAIN"size"
TOOLCHAIN_OBJDUMP=$TOOLCHAIN"objdump"
TOOLCHAIN_OBJCOPY=$TOOLCHAIN"objcopy"
echo $TOOLCHAIN_GCC

# -c => Compile or assemble the source files, but do not link. 
# -fdata-sections
#-ffunction-sections => Place each function or data item into its own section in the output file if the target supports arbitrary sections. The name of the function or the name of the data item determines the section’s name in the output file. 
# -mthumb => Select between generating code that executes in ARM and Thumb states
GCC_FLAGS="-c -ffunction-sections -fdata-sections -mcpu=arm7tdmi -g -mfloat-abi=soft"
AS_FLAGS="-c -mcpu=arm7tdmi -g"
INLCUDES="-I StdPeriphDriver/inc/ -I CMSIS/Include/"

file1="$TOOLCHAIN_GCC Main.c $INLCUDES -o build/main.o $GCC_FLAGS"
echo $file1
$file1

file2="$TOOLCHAIN_AS vector.S $INLCUDES -o build/vector.o $AS_FLAGS"
echo $file2
$file2

file3="$TOOLCHAIN_AS startup.S $INLCUDES -o build/startup.o $AS_FLAGS"
echo $file3
$file3

filename=71x_it
file="$TOOLCHAIN_GCC $filename.c $INLCUDES -o build/$filename.o $GCC_FLAGS"
$file

filename=gpio
file="$TOOLCHAIN_GCC $filename.c $INLCUDES -o build/$filename.o $GCC_FLAGS"
$file

filename=71x_rccu
file="$TOOLCHAIN_GCC $filename.c $INLCUDES -o build/$filename.o $GCC_FLAGS"
$file

linker="$TOOLCHAIN_GCC build/*.o -nostartfiles -T$LINKER_SCRIPT -Wl,--gc-sections -mcpu=arm7tdmi -o output.elf  -Xlinker -Map=output.map"
echo $linker
$linker

$TOOLCHAIN_OBJDUMP -S --disassemble -marm output.elf > output.asm
$TOOLCHAIN_OBJCOPY -O ihex output.elf output.hex
$TOOLCHAIN_OBJCOPY -O binary output.elf output.bin

$TOOLCHAIN_SIZE "output.elf"
