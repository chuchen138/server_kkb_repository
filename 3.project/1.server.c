#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serv_sock < 0){
        perror("socket");
        return -1;
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8086);
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if(listen(serv_sock, 20)<0){
        perror("listen");
        return -1;
    }
    struct sockaddr_in clnt_addr;

    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_sock < 0){
        perror("accept");
        return -1;
    }

    char str[] = "hello! I'm sunxiangguo！";
    write(clnt_sock, str, sizeof(str));

    close(clnt_sock);
    close(serv_sock);
    
    return 0;
}
