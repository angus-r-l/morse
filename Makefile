PROJ_NAME = main

SRCS = $(PROJ_NAME).c 
MYLIB = $(NUC_SRC)/mylib
RTOS_PATH = $(NUC_SRC)/Third_Party/FreeRTOS/Source

CFLAGS = -I$(MYLIB)
CFLAGS += -DENABLE_VCP
CFLAGS += -Wall -Wpedantic

################################################################################

LIBSRCS += $(RTOS_PATH)/portable/MemMang/heap_3.c

################################################################################

ROOT = $(shell pwd)

OBJS = $(SRCS:.c=.o)

################################################################################

.PHONY: $(shell mkdir -p obj) lib proj all depend
all: proj

# Include RTOS makefile
include $(RTOS_PATH)/Makefile.rtos

# Include NucleoF401 makefile
include $(NUC_SRC)/Makefile.nuc

lib:
	$(MAKE) -C lib

proj:	$(PROJ_NAME).elf

$(PROJ_NAME).elf:	$(STM32OBJ) $(OBJS)
	$(CC) $(LDFLAGS) -o $@ -Llib $(OBJS) $(STM32OBJ) $(LD_SYS_LIBS)
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

%.o:
	$(CC) $(CFLAGS) -c $< -o $@

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	mkdir -p obj
	$(CC) $(CFLAGS) -MM $^>>./.depend;

flash:
	st-flash write $(PROJ_NAME).bin 0x8000000 # Flash Nucleo

clean:
	rm -f *.o
	rm -f $(OBJDIR)/*.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(LIBSTM32)
	rm -rf .depend
	rmdir $(OBJDIR)

include .depend
