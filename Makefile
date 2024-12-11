.PHONY: dpdk_install lib tool example demo

COMPILE_COMMANDS = /home/zcj/f-stack/compile_commands.json

all: dpdk_install lib tool example demo
fstack: lib tool example demo

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

demo:
	cd ./demo && \
	make clean && \
	bear --output $(COMPILE_COMMANDS) --append -- make

demo_server:
	sudo rm -rf /tmp/vhost0
	sudo ./demo/build/server --conf ./demo/server.ini --proc-type=primary --proc-id=0

demo_client:
	
	sudo ./demo/build/client --conf ./demo/client.ini --proc-type=primary --proc-id=0