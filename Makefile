QEMU := qemu-system-riscv64
QEMUFLAGS := -nographic -smp 1 -M virt -bios none -m 256
QEMUDBGFLAGS := -nographic -smp 1 -M virt -bios none -m 256
# TARGET := bin/hello.bin
KERNEL_BIN := bin/hello.bin
KERNEL_ELF := bin/hello.elf

# A := @

run:
	$(A) $(QEMU) $(QEMUFLAGS) -kernel $(KERNEL_BIN)

dbg: build
	$(A) $(QEMU) $(QEMUDBGFLAGS) -kernel $(KERNEL_ELF) -s -S
#    $(QEMU) -machine virt -nographic -bios $(BOOTLOADER) -device loader,file=$(KERNEL_BIN),addr=$(KERNEL_ENTRY_PA) -drive file=$(FS_IMG),if=none,format=raw,id=x0 -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0 -s -S

gdb:
	riscv64-unknown-elf-gdb $(KERNEL_ELF) -x ./gdbinit

detect:
	qemu-system-riscv64 -machine virt -machine dumpdtb=riscv64-virt.dtb
	dtc -I dtb -O dts -o riscv64-virt.dts riscv64-virt.dtb