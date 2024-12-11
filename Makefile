CXX = g++
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -nostdlib -lgcc
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

all: os_image

os_image: $(OBJ)
	$(CXX) -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib $(OBJ) -lgcc
	mkdir -p iso/boot
	cp kernel.bin iso/boot/kernel.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o mem_test.iso iso

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) kernel.bin mem_test.iso