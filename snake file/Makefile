obj-m   := fpga_dot_driver.o

KDIR :=/work/achro-em/kernel
PWD :=$(shell pwd)


all: driver app

driver:
#	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
app:
#	gcc -static -o fpga_test_dot fpga_test_dot.c
	arm-linux-gnueabihf-gcc -static -o snake snake.c -lpthread

install_nfs:
	cp -a fpga_dot_driver.ko /nfsroot
	cp -a snake /nfsroot
	cp -a fpga_text_lcd_driver.ko /nfsroot
	cp -a fpga_push_switch_driver.ko /nfsroot
	cp -a fpga_interface_driver.ko /nfsroot
install_scp:
	scp fpga_interface_driver.ko fpga_push_switch_driver.ko fpga_text_lcd_driver.ko fpga_dot_driver.ko snake pi@192.168.0.xxx:/home/pi/Modules

clean:
#	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -rf fpga_dot_driver.ko
	rm -rf *.mod.*
	rm -rf *.o
	rm -rf fpga_test_dot
	rm -rf Module.symvers
	rm -rf modules.order
	rm -rf .dot*
	rm -rf .tmp*
