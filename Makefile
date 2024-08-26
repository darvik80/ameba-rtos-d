MAKE := make
CC := gcc

.PHONY: all clean flash

DIR_MAIN := project/realtek_amebaD_va0_example
DIR_COMPONENT := component

SRC := $(wildcard **/*.c */*/*.c */*/*/*.c */*/*/*/*.c */*/*/*/*/*.c */*/*/*/*/*/*.c */*/*/*/*/*/*/*.c)
INC := $(shell find $(DIR_COMPONENT) -type d)
INC += $(shell find $(DIR_MAIN) -type d)
INC_DIR := $(foreach d, $(INC), -I$d)

stub:
	$(CC) -c $(SRC) -I$(INC_DIR)

all:
	$(MAKE) -C project/realtek_amebaD_va0_example/GCC-RELEASE/project_lp
	$(MAKE) -C project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp

flash:
	mkdir -p image
	cp project/realtek_amebaD_va0_example/GCC-RELEASE/project_lp/asdk/image/km0_boot_all.bin image
	cp project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/image/km4_boot_all.bin image
	cp project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/image/km0_km4_image2.bin image
ifeq ($(COMPILEOS),$(DARWIN_OS))
	cp -r tools/macos/bsp image
	cp -r tools/macos/imgtool_flashloader_amebad.bin image
	cp -r tools/macos/upload_image_tool_macos image
	cd image && ./upload_image_tool_macos ../image /dev/cu.usbserial-1130 "{board}" Enable Disable 1500000
endif

clean:
	$(MAKE) -C project/realtek_amebaD_va0_example/GCC-RELEASE/project_lp clean
	$(MAKE) -C project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp clean

