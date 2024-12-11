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

#define SERVER_IP "192.168.1.2"  // 服务器的IP地址
#define PORT 8080              // 服务器监听的端口
#define BUFFER_SIZE 1024       // 发送和接收缓冲区大小

int sockfd;
struct sockaddr_in server_addr;
char buffer[BUFFER_SIZE];
char message[BUFFER_SIZE] = "hello world";

int loop(void *arg)
{
    // 发送消息给服务器
    ssize_t sent_bytes = ff_sendto(sockfd, message, strlen(message), 0,
                                    (struct linux_sockaddr *)&server_addr, sizeof(server_addr));
    if (sent_bytes < 0) {
        perror("ff_sendto error");
        return 0;
    }
    printf("Sent message to server: %s\n", message);
    sleep(1);

    // 接收服务器的回显消息
    socklen_t addr_len;
    // ssize_t recv_bytes = ff_recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
    //                                     (struct linux_sockaddr *)&server_addr, &addr_len);
    ssize_t recv_bytes = ff_recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    // if (recv_bytes < 0) {
    //     sleep(1);
    // }

    buffer[recv_bytes] = '\0'; // 确保字符串以null结尾
    printf("Received echo from server: %s\n", buffer);
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
    my_addr.sin_port = htons(8100);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // inet_pton(AF_INET, SERVER_IP, &my_addr.sin_addr);

    // 绑定UDP套接字到指定端口
    int ret = ff_bind(sockfd, (struct linux_sockaddr *)&my_addr, sizeof(my_addr));
    if (ret < 0) {
        printf("ff_bind failed\n");
        close(sockfd);
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported\n");
        close(sockfd);
        exit(1);
    }

    ff_run(loop, NULL);
    return 0;
}
