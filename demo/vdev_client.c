#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "ff_config.h"
#include "ff_api.h"

int loop(void *arg)
{
    while(1) {
        printf("client hello\n");
        sleep(5);
    }

    return 0;
}

int main(int argc, char * argv[])
{
    ff_init(argc, argv);

    ff_run(loop, NULL);
    return 0;
}
