obj-m := myfs.o
myfs-objs := super.o

all:
	make -C /lib/modules/5.5.5/build M=$(PWD) modules

clean:
	make -C /lib/modules/5.5.5/build M=$(PWD) clean

