/*************************************************************************
	> File Name: common.c
	> Author: suyelu 
	> Mail: suyelu@126.com
	> Created Time: Sat 03 Jul 2021 07:28:26 PM CST
 ************************************************************************/

#include "head.h"
int make_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) return -1;
    flags |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}
int make_block(int fd) {
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) return -1;
    flags &= ~O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}

int socket_create(int port) {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family  = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)); 
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0) {
        return -1;
    }
    if (listen(sockfd, 8) < 0) {
        return -1;
    }
    return sockfd;
}

int socket_create_udp(int port) {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family  = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)); 
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0) {
        return -1;
    }
    //if (listen(sockfd, 8) < 0) {
    //    return -1;
    //}
    return sockfd;
}
int socket_connect(const char *ip, int port) {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        return -1;
    }
    return sockfd;
}
int socket_udp(){
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }
    return sockfd;
}

int recv_file_from_socket(int sockfd, char *name, char *dir) {
    char path[1024] = {0};
    sprintf(path, "%s/%s", dir, name);
    int fd = open(path, O_CREAT | O_WRONLY, 0644);
    if (fd < 0) {
        close(fd);
        return -1;
    }
    while (1) {
        char buff[512] = {0};
        int rsize = recv(sockfd, buff, sizeof(buff), 0);
        if (rsize <= 0) {
            close(fd);
            return rsize;
        }
        int nwrite = write(fd, buff, rsize);
        if (nwrite != rsize) {
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}

int send_file_to_socket(int confd, char *name){
    int fd;
    if ((fd = open(name, O_RDONLY)) < 0) {
        return -1;
    }
    while (1) {
        char buff[512] = {0};
        int rsize = read(fd, buff, sizeof(buff));
        if (rsize <= 0) {
            close(fd);
            close(confd);
            DBG(YELLOW"<Debug>"NONE" : After sent!\n");
            return rsize;
        }
        send(confd, buff, rsize, 0);
    }
}

char *get_conf_value(const char *filename, const char *key) {
    bzero(conf_ans, sizeof(conf_ans));
    FILE *fp;
    char *line = NULL, *sub = NULL;
    ssize_t len = 0, nread = 0;
    if (filename == NULL || key == NULL) {
        return NULL;
    }
    if ((fp = fopen(filename, "r")) == NULL) {
        return NULL;
    }
 
    while ((nread = getline(&line, &len, fp)) != -1) {
        if ((sub = strstr(line, key)) == NULL) continue;
        if (line[strlen(key)] == '=' && sub == line) {
            strcpy(conf_ans, line + strlen(key) + 1);
            if (conf_ans[strlen(conf_ans) - 1] == '\n') 
                conf_ans[strlen(conf_ans) - 1] = '\0';
        }
    }
    free(line);
    fclose(fp);
    return conf_ans;
}




int socket_connect_timeout(const char *ip, int port, int timeout) {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);
    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    struct timeval tv;
    tv.tv_usec = 0;
    tv.tv_sec = timeout;

    make_nonblock(sockfd);
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
    }
    
    if (select(sockfd + 1, NULL, &wfds, NULL, &tv) <= 0) {
        DBG(RED"Error : connection timeout!\n"NONE);
        exit(1);
    }
    int error;
    int len = sizeof(error);
    if ((getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len)) < 0) {
        perror("getsockopt");
        exit(1);
    }
    if (error != 0) {
        DBG(RED"Error ： connection error!\n");
        return -1;
    }
    make_block(sockfd);
    DBG(GREEN"Success : connection  succcessfull！\n"NONE);
    return sockfd;
}
