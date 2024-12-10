.PHONY: dpdk_install lib tool example

COMPILE_COMMANDS = /home/zcj/f-stack/compile_commands.json

all: dpdk_install lib tool example

dpdk_install:
	cd ./dpdk && \
	sudo rm -rf build && \
	meson setup -Denable_kmods=true -Ddisable_libs=flow_classify build && \
	cd build && \
	bear --output $(COMPILE_COMMANDS) --append -- ninja -j64 && \
	sudo meson install && \
	sudo ldconfig

lib:
	cd ./lib && \
	make clean && \
	bear --output $(COMPILE_COMMANDS) --append -- make -j64

tool:
	cd ./tools && \
	make clean && \
	bear --output $(COMPILE_COMMANDS) --append -- make -j64

example:
	cd ./example && \
	make clean && \
	bear --output $(COMPILE_COMMANDS) --append -- make -j64