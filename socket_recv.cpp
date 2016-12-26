#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <iostream>
#include <cstdio>
#include <unistd.h>
using std::string;
int main(int arc, char **argv)
{
    int server_sockfd = -1;
    socklen_t server_len = 0;
    socklen_t client_len = 0;
    char buffer[512];
    ssize_t result = 0;
 
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
 
    // 创建数据报套接字
    server_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
 
    // 设置监听的端口、IP
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(1235);
    server_len = sizeof(server_addr);
 
    // 绑定（命名）套接字
    bind(server_sockfd, (struct sockaddr *)&server_addr, server_len);
 
    // 忽略子进程停止 或 退出 的信息
    signal(SIGCHLD, SIG_IGN);
 
    while (1)
    {
        // 接收数据，用 client_addr 来储存数据来源程序的IP端口
        result = recvfrom(server_sockfd, buffer, sizeof(buffer), 0,
                          (struct sockaddr *)&client_addr, &client_len);

        if (result==-1)
        {
            std::cout<<"continue"<<std::endl;
            continue;
        }
        if (fork() == 0)
        {
            
            if (strcmp(buffer, "reload_config"))
            {
                std::cout<<"same"<<std::endl;
            }
            int len=strlen(buffer);
            buffer[len]='\0';
            std::cout<<buffer<<std::endl;
            // 注意，一定要关闭子进程，否则程序运行会不正常
            exit(0);
        }
        break;
    }
    
    // 关闭套接字
    close(server_sockfd);
}