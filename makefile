#--------------------------------------Makefile-------------------------------------
CFILES = $(wildcard ./src/*.c)

OFILES = $(CFILES:./src/%.c=./build/%.o)

GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib

all: clean kernel8.img

./build/boot.o: ./linker/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/%.o: ./src/%.c 
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: ./build/boot.o $(OFILES)
	aarch64-none-elf-ld -nostdlib ./build/boot.o $(OFILES) -T ./linker/link.ld -o kernel8.elf
	aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	del *.img *.elf .\build\*.o

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial stdio





