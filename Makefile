obj-m+=random_number.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) user_process.c -o user
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm user

run:
	-sudo rmmod random_number
	sudo insmod random_number.ko
  
