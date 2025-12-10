default := all

all : wii_homebrew

wii_homebrew : build/out.dol

c_compiler_powerpc := /opt/devkitpro/devkitPPC/bin/powerpc-eabi-gcc
cpp_compiler_powerpc := /opt/devkitpro/devkitPPC/bin/powerpc-eabi-g++
elf2dol := /opt/devkitpro/tools/bin/elf2dol

c_files_and_whatnot := main mn_gfx_implementation_gx dot

build :
	mkdir $@

# other thing
build/out.dol: build/out.elf build
	${elf2dol} build/out.elf build/out.dol

# link
build/out.elf: ${c_files_and_whatnot:%=build/%.o}
	${cpp_compiler_powerpc} ${c_files_and_whatnot:%=build/%.o} -g -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -Wl,-Map,build/out.elf.map  -L/opt/devkitpro/libogc/lib/wii -lfat -lwiiuse -lbte -logc -lm -o build/out.elf

# compile c
build/%.o : src/%.c build
	${c_compiler_powerpc} -MMD -MP -MF build/${@:build/%.o=%.d} -g -O2 -Wall -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -Ibuild -I/opt/devkitpro/libogc/include -c $< -o $@

# compile c++
build/%.o : src/%.cpp build
	${cpp_compiler_powerpc} -MMD -MP -MF build/${@:build/%.o=%.d} -g -O2 -Wall -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -Ibuild -I/opt/devkitpro/libogc/include -c $< -o $@

# build/${c_files_and_whatnot}.o: src/${c_files_and_whatnot}.c
#	powerpc-eabi-gcc -MMD -MP -MF build/main.d  -g -O2 -Wall -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -Ibuild -I/opt/devkitpro/libogc/include -c src/misc/main.c -o build/main.o


src/${c_files_and_whatnot}.c:
	echo "no"

clean:
	rm -rf build

# Variables that are only valid in a build command:
#  $@ is the target being built
#  $< is the *first* dependency of the target being built
