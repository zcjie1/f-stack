.PHONY: dpdk_install lib tool example

all: dpdk_install lib tool example

dpdk_install:
	cd ./dpdk && \
	sudo rm -rf build && \
	meson setup -Denable_kmods=true -Ddisable_libs=flow_classify build && \
	cd build && \
	bear --output /home/zcj/f-stack/dpdk/build/compile_commands.json --append -- ninja -j64 && \
	sudo meson install && \
	sudo ldconfig

lib:
	cd ./lib && \
	make clean && \
	bear --output /home/zcj/f-stack/dpdk/build/compile_commands.json --append -- make -j64

tool:
	cd ./tools && \
	make clean && \
	bear --output /home/zcj/f-stack/dpdk/build/compile_commands.json --append -- make -j64

example:
	cd ./example && \
	make clean && \
	bear --output /home/zcj/f-stack/dpdk/build/compile_commands.json --append -- make -j64