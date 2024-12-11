CXX = g++
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -nostdlib -lgcc
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o) 

all: efi_image

efi_image: $(OBJ)
	$(CXX) -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib $(OBJ) -lgcc
	mkdir -p EFI/BOOT
	cp kernel.bin EFI/BOOT/kernel.bin
	cp grub.cfg EFI/BOOT/grub.cfg
	grub-mkstandalone --format=x86_64-efi --output=EFI/BOOT/BOOTX64.EFI --modules="part_gpt part_msdos fat ext2 normal multiboot configfile" --locales="" --fonts="" "boot/grub/grub.cfg=grub.cfg"
	dd if=/dev/zero of=efi_image.img bs=1M count=64
	mkfs.vfat efi_image.img
	mmd -i efi_image.img ::/EFI ::/EFI/BOOT
	mcopy -i efi_image.img EFI/BOOT/BOOTX64.EFI ::/EFI/BOOT/
	mcopy -i efi_image.img EFI/BOOT/kernel.bin ::/EFI/BOOT/
	mcopy -f efi_image.img EFI/BOOT/grub.cfg ::/EFI/BOOT/

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) kernel.bin EFI efi_image.img