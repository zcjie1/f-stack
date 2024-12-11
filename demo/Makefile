TOPDIR=..

ifeq ($(FF_PATH),)
	FF_PATH=${TOPDIR}
endif

ifneq ($(shell pkg-config --exists libdpdk && echo 0),0)
$(error "No installation of DPDK found, maybe you should export environment variable `PKG_CONFIG_PATH`")
endif

PKGCONF ?= pkg-config

CFLAGS += -O0 -g -gdwarf-2 -I../lib $(shell $(PKGCONF) --cflags libdpdk)

LIBS+= $(shell $(PKGCONF) --static --libs libdpdk)
LIBS+= -L${FF_PATH}/lib -Wl,--whole-archive,-lfstack,--no-whole-archive
LIBS+= -Wl,--no-whole-archive -lrt -lm -ldl -lcrypto -pthread -lnuma

TARGET1="server"
TARGET2="client"
all: | build
	cc ${CFLAGS} -o build/${TARGET1} vdev_server.c ${LIBS}
	cc ${CFLAGS} -o build/${TARGET2} vdev_client.c ${LIBS}

build:
	@mkdir -p $@

.PHONY: clean
clean:
	rm -rf *.o build/${TARGET1} build/${TARGET2} build
