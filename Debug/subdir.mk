################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../.asm \
../bootblock.asm \
../cat.asm \
../echo.asm \
../entryother.asm \
../forktest.asm \
../grep.asm \
../init.asm \
../initcode.asm \
../kernel.asm \
../kill.asm \
../ln.asm \
../ls.asm \
../mkdir.asm \
../rm.asm \
../sh.asm \
../stressfs.asm \
../usertests.asm \
../wc.asm \
../zombie.asm 

C_SRCS += \
../bio.c \
../bitmap.c \
../bootmain.c \
../cat.c \
../console.c \
../context.c \
../desktop.c \
../drawingAPI.c \
../echo.c \
../exec.c \
../file.c \
../forktest.c \
../fs.c \
../grep.c \
../ide.c \
../init.c \
../ioapic.c \
../kalloc.c \
../kbd.c \
../kill.c \
../lapic.c \
../ln.c \
../log.c \
../ls.c \
../main.c \
../memide.c \
../message.c \
../mkdir.c \
../mkfs.c \
../mouse.c \
../mp.c \
../picirq.c \
../pipe.c \
../printf.c \
../proc.c \
../rm.c \
../sh.c \
../shell.c \
../spinlock.c \
../stressfs.c \
../string.c \
../syscall.c \
../sysfile.c \
../sysproc.c \
../syswindow.c \
../text.c \
../timer.c \
../trap.c \
../uart.c \
../ulib.c \
../umalloc.c \
../usertests.c \
../vegamode.c \
../vesamode.c \
../vm.c \
../wc.c \
../window.c \
../zombie.c 

O_SRCS += \
../bio.o \
../bitmap.o \
../bootasm.o \
../bootblock.o \
../bootblockother.o \
../bootmain.o \
../cat.o \
../console.o \
../context.o \
../desktop.o \
../drawingAPI.o \
../echo.o \
../entry.o \
../entryother.o \
../exec.o \
../file.o \
../forktest.o \
../fs.o \
../grep.o \
../ide.o \
../init.o \
../initcode.o \
../ioapic.o \
../kalloc.o \
../kbd.o \
../kill.o \
../lapic.o \
../ln.o \
../log.o \
../ls.o \
../main.o \
../message.o \
../mkdir.o \
../mouse.o \
../mp.o \
../picirq.o \
../pipe.o \
../printf.o \
../proc.o \
../rm.o \
../sh.o \
../shell.o \
../spinlock.o \
../stressfs.o \
../string.o \
../swtch.o \
../syscall.o \
../sysfile.o \
../sysproc.o \
../syswindow.o \
../timer.o \
../trap.o \
../trapasm.o \
../uart.o \
../ulib.o \
../umalloc.o \
../usertests.o \
../usys.o \
../vectors.o \
../vesamode.o \
../vm.o \
../wc.o \
../window.o \
../zombie.o 

S_UPPER_SRCS += \
../bootasm.S \
../entry.S \
../entryother.S \
../initcode.S \
../swtch.S \
../trapasm.S \
../usys.S \
../vectors.S 

OBJS += \
./xv6.o \
./bio.o \
./bitmap.o \
./bootasm.o \
./bootblock.o \
./bootmain.o \
./cat.o \
./console.o \
./context.o \
./desktop.o \
./drawingAPI.o \
./echo.o \
./entry.o \
./entryother.o \
./exec.o \
./file.o \
./forktest.o \
./fs.o \
./grep.o \
./ide.o \
./init.o \
./initcode.o \
./ioapic.o \
./kalloc.o \
./kbd.o \
./kernel.o \
./kill.o \
./lapic.o \
./ln.o \
./log.o \
./ls.o \
./main.o \
./memide.o \
./message.o \
./mkdir.o \
./mkfs.o \
./mouse.o \
./mp.o \
./picirq.o \
./pipe.o \
./printf.o \
./proc.o \
./rm.o \
./sh.o \
./shell.o \
./spinlock.o \
./stressfs.o \
./string.o \
./swtch.o \
./syscall.o \
./sysfile.o \
./sysproc.o \
./syswindow.o \
./text.o \
./timer.o \
./trap.o \
./trapasm.o \
./uart.o \
./ulib.o \
./umalloc.o \
./usertests.o \
./usys.o \
./vectors.o \
./vegamode.o \
./vesamode.o \
./vm.o \
./wc.o \
./window.o \
./zombie.o 

C_DEPS += \
./bio.d \
./bitmap.d \
./bootmain.d \
./cat.d \
./console.d \
./context.d \
./desktop.d \
./drawingAPI.d \
./echo.d \
./exec.d \
./file.d \
./forktest.d \
./fs.d \
./grep.d \
./ide.d \
./init.d \
./ioapic.d \
./kalloc.d \
./kbd.d \
./kill.d \
./lapic.d \
./ln.d \
./log.d \
./ls.d \
./main.d \
./memide.d \
./message.d \
./mkdir.d \
./mkfs.d \
./mouse.d \
./mp.d \
./picirq.d \
./pipe.d \
./printf.d \
./proc.d \
./rm.d \
./sh.d \
./shell.d \
./spinlock.d \
./stressfs.d \
./string.d \
./syscall.d \
./sysfile.d \
./sysproc.d \
./syswindow.d \
./text.d \
./timer.d \
./trap.d \
./uart.d \
./ulib.d \
./umalloc.d \
./usertests.d \
./vegamode.d \
./vesamode.d \
./vm.d \
./wc.d \
./window.d \
./zombie.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


