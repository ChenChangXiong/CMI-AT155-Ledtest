

FILE := module_test
obj-m += $(FILE).o  

KDIR  := /home/chencx/aplex/kernel3.2.0/

all:
	@rm   $(FILE).c 
	@cp  /mnt/share/work_demo/modules/$(FILE).c  ./
	@echo  "---copy finish---"
	@chmod 666  $(FILE).c

	make -C $(KDIR)  M=$(PWD)  modules		

	@rm  *.order  *.sym*  *.mod*  .*.cmd  *.o .tmp*  -rf
	@echo  "======copy to windows======"
	@cp  *.ko  /mnt/share/work_demo/modules/
#	@clear
#   @ls
clean:
	@rm  *.order  *.sym*  *.mod*  .*.cmd  *.o .tmp*  -rf

chmod:
	@chmod  666  $(FILE).c

bsp:
	@cp ./*.ko    /mnt/share/
