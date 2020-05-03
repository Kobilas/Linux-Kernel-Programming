ifneq($(KERNELRELEASE),)

include Kbuild

else

KDIR ?= /lib/modules/5.5.5/build

all:
	$(MAKE) -C $(KDIR) M=$$PWD modules
install:
	$(MAKE) INSTALL_MOD_DIR=packet_counter -C $(KDIR) M=$$PWD modules_install
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

endif
