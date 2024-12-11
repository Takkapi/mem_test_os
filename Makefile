CXX = g++
AS = nasm
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -nostdlib -lgcc
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o) multiboot_header.o

all: os_image

os_image: $(OBJ)
	$(CXX) -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib $(OBJ) -lgcc
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o mem_test.iso iso

multiboot_header.o: multiboot_header.S
	$(AS) multiboot_header.S -o multiboot_header.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) kernel.bin mem_test.iso