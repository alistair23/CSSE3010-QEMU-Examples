# put your *.o targets here, make should handle the rest!

STDPERPH_PATH=$(NP2_ROOT)/src/Libraries/STM32F4xx_StdPeriph_Driver
NP2_PATH=$(NP2_ROOT)/src/netduinoplus2
VCP_PATH=$(NP2_ROOT)/src/netduinoplus2/vcp
USBCORE_PATH=$(NP2_ROOT)/src/Libraries/STM32_USB/STM32_USB_Device_Library/Core
USBCDC_PATH=$(NP2_ROOT)/src/Libraries/STM32_USB/STM32_USB_Device_Library/Class/cdc
USBOTG_PATH=$(NP2_ROOT)/src/Libraries/STM32_USB/STM32_USB_OTG_Driver

PROJ_NAME=main

SRCS = $(PROJ_NAME).c 
SRCS += $(STDPERPH_PATH)/src/*.c
SRCS += $(NP2_PATH)/src/*.c
SRCS += $(VCP_PATH)/src/*.c $(USBCORE_PATH)/src/*.c $(USBCDC_PATH)/src/usbd_cdc_core.c 
SRCS += $(USBOTG_PATH)/src/usb_dcd.c $(USBOTG_PATH)/src/usb_dcd_int.c $(USBOTG_PATH)/src/usb_core.c
# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

###################################################

CROSS_COMPILE=arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
OBJCOPY=$(CROSS_COMPILE)objcopy
OBJDUMP=$(CROSS_COMPILE)objdump

CFLAGS  = -g -O0 -Wall -T$(NP2_PATH)/stm32_flash.ld -Wmaybe-uninitialized -DNO_FPU
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m3 -mthumb-interwork -DUSE_STDPERIPH_DRIVER -DUSE_STM324xG_EVAL
#CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

###################################################

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

CFLAGS += -I$(STDPERPH_PATH)/inc -I$(NP2_ROOT)/src/Libraries/CMSIS/Include -I$(NP2_PATH)/inc 
CFLAGS += -I$(VCP_PATH)/inc -I$(USBCORE_PATH)/inc -I$(USBCDC_PATH)/inc -I$(USBOTG_PATH)/inc
CFLAGS += -DENABLE_VCP #Enable USB VCP for debug_printf
#CFLAGS += -DENABLE_DEBUG_UART	#Enable UART4 for debug printf

SRCS += $(NP2_PATH)/startup_stm32f4xx.s # add startup file to build

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: proj

lib:
	$(MAKE) -C lib

proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -Llib #-lstm32f4
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(OBJDUMP) -S $(PROJ_NAME).elf > $(PROJ_NAME).dump

prog:
	sudo dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000 -D $(PROJ_NAME).bin

clean:
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).dump
