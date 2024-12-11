CXX = g++
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

all: os_image

os_image: $(OBJ)
	$(CXX) -T linker.ld -o mem_test.bin -ffreestanding -O2 -nostdlib $(OBJ) -lgcc
	cp mem_test.bin iso/boot/mem_test.bin
	grub-mkrescue -o mem_test.iso iso

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) mem_test.bin mem_test.iso