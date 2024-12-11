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

#define SERVER_IP "192.168.1.2" 
#define PORT 8080         // 服务监听的端口
#define BUFFER_SIZE 1024  // 接收缓冲区大小

int sockfd;
char buffer[BUFFER_SIZE];
struct sockaddr_in client_addr;
socklen_t addr_len = sizeof(client_addr);

int loop(void *arg)
{
    // 接收来自客户端的数据报
    ssize_t n = ff_recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, 
                            (struct linux_sockaddr *)&client_addr, &addr_len);
    // ssize_t n = ff_recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    if (n < 0) {
        // perror("ff_recvfrom error");
        return 0;
    }

    buffer[n] = '\0'; // 确保字符串以null结尾
    printf("Received message from %s:%d: %s\n",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

    // 回显消息给客户端（可选）
    ssize_t send_len = ff_sendto(sockfd, buffer, strlen(buffer), 0, 
                    (struct linux_sockaddr *)&client_addr, addr_len);
    if (send_len < 0) {
        perror("ff_sendto error");
    } else {
        printf("Echo: %s\n", buffer);
    }
    return 0;
}

int main(int argc, char * argv[])
{
    ff_init(argc, argv);

    // 创建UDP套接字
    sockfd = ff_socket(AF_INET, SOCK_DGRAM, 0);
    printf("sockfd:%d\n", sockfd);
    if (sockfd < 0) {
        printf("ff_socket failed\n");
        exit(1);
    }

    /* Set non blocking */
    int on = 1;
    ff_ioctl(sockfd, FIONBIO, &on);

    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // inet_pton(AF_INET, SERVER_IP, &my_addr.sin_addr);

    // 绑定UDP套接字到指定端口
    int ret = ff_bind(sockfd, (struct linux_sockaddr *)&my_addr, sizeof(my_addr));
    if (ret < 0) {
        printf("ff_bind failed\n");
        close(sockfd);
        exit(1);
    }

    printf("UDP Server is listening on port %d\n", PORT);

    ff_run(loop, NULL);
    return 0;
}
