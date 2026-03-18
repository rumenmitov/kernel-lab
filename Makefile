# Directories
KDIR     ?= /lib/modules/$(shell uname -r)/build
SRC      := $(CURDIR)/src
BUILD    := $(CURDIR)/build


# Programs / Scripts
SUDO       := /usr/bin/env sudo
FORMAT     := /usr/bin/env clang-format
CHECKPATCH := $(KDIR)/scripts/checkpatch.pl


all: build dev


mkbuild:
	@rm -rf $(BUILD)
	@mkdir -p $(BUILD)
	@touch $(BUILD)/Makefile
	@cp -r $(SRC) $(BUILD)/src


build: mkbuild
	@$(MAKE) -C $(KDIR) M=$(BUILD) src=$(CURDIR) modules


dev: mkbuild
	@$(MAKE) -C $(KDIR) M=$(BUILD) src=$(CURDIR) compile_commands.json
	@ln -sf $(BUILD)/compile_commands.json .


lint:
	@$(FORMAT) -i $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*.h)
	@$(CHECKPATCH) --no-tree -f $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*.h)


install: build
	@$(SUDO) insmod build/hello.ko


uninstall:
	@$(SUDO) rmmod hello


clean:
	@$(MAKE) -C $(KDIR) M=$(BUILD) src=$(CURDIR) clean
	@rm -f compile_commands.json
	@rm -rf $(BUILD)
