c_files_and_whatnot := main mn_gfx_implementation_gx

default : build/out.dol

build :
	mkdir $@

build/out.dol: build/out.elf build
	elf2dol build/out.elf build/out.dol 

build/out.elf: ${c_files_and_whatnot:%=build/%.o}
	powerpc-eabi-gcc build/main.o build/mn_gfx_implementation_gx.o -g -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -Wl,-Map,build/out.elf.map  -L/opt/devkitpro/libogc/lib/wii -lwiiuse -lbte -logc -lm -o build/out.elf

build/%.o : src/%.c build
	powerpc-eabi-gcc -MMD -MP -MF build/${@:build/%.o=%.d} -g -O2 -Wall -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -Ibuild -I/opt/devkitpro/libogc/include -c $< -o $@

#build/${c_files_and_whatnot}.o: src/${c_files_and_whatnot}.c
#	powerpc-eabi-gcc -MMD -MP -MF build/main.d  -g -O2 -Wall -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -Ibuild -I/opt/devkitpro/libogc/include -c src/misc/main.c -o build/main.o

src/${c_files_and_whatnot}.c:
	echo "no"

clean:
	rm -rf build

# Variables that are only valid in a build command:
#  $@ is the target being built
#  $< is the *first* dependency of the target being built