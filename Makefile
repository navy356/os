kernel_source_files := $(shell find src/impl/kernel -name *.c)
kernel_object_files := $(patsubst src/impl/kernel/%.c, build/kernel/%.o, $(kernel_source_files))

x86_64_c_source_files := $(shell find src/impl/x86_64 -name *.c)
x86_64_c_object_files := $(patsubst src/impl/x86_64/%.c, build/x86_64/%.o, $(x86_64_c_source_files))

x86_64_asm_source_files := $(shell find src/impl/x86_64 -name *.asm)
x86_64_asm_object_files := $(patsubst src/impl/x86_64/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

x86_64_object_files := $(x86_64_c_object_files) $(x86_64_asm_object_files)

module_source_files := $(shell find modules/ -name *.asm)
module_object_files := $(patsubst modules/%.asm, targets/x86_64/iso/modules/%.o, $(module_source_files))

$(kernel_object_files): build/kernel/%.o : src/impl/kernel/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc -c -I src/intf -ffreestanding $(patsubst build/kernel/%.o, src/impl/kernel/%.c, $@) -o $@

$(x86_64_c_object_files): build/x86_64/%.o : src/impl/x86_64/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc -c -I src/intf -ffreestanding $(patsubst build/x86_64/%.o, src/impl/x86_64/%.c, $@) -o $@

$(x86_64_asm_object_files): build/x86_64/%.o : src/impl/x86_64/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst build/x86_64/%.o, src/impl/x86_64/%.asm, $@) -o $@ -i src/impl/x86_64/boot/

$(module_object_files): targets/x86_64/iso/modules/%.o : modules/%.asm
	nasm -f bin $(patsubst targets/x86_64/iso/modules/%.o,modules/%.asm, $@) -o $@

.PHONY: build
build: $(kernel_object_files) $(x86_64_object_files)
	mkdir -p dist/x86_64 && \
	x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld $(kernel_object_files) $(x86_64_object_files) && \
	cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso

run: dist/x86_64/kernel.iso
	qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso

modules: $(module_object_files)
	echo $(module_object_files)

debug: dist/x86_64/kernel.iso
	bochs -f bochsrc.txt -q

clean:
	rm -f $(kernel_object_files)
	rm -f $(x86_64_c_object_files)
	rm -f $(x86_64_asm_object_files)
	rm -f dist/x86_64/kernel.iso
	rm -f dist/x86_64/kernel.bin
	rm -f $(module_object_files)