#include "./common/head.h"

char name[20],passwd[20];
#define conf "./wechat.conf"
int sockfd;

void logout(int signum){
    struct wechat_msg msg;
    msg.type = WECHAT_FIN;
    send(sockfd,&msg,sizeof(msg),0);
    DBG(NONE"bye\n"NONE);
    exit(0);
}

int main(int argc, char *argv[]){
    int opt, mode = 1 ,server_port = 0, sex = 1;
    char server_ip[20] = {0};
    while((opt = getopt(argc, argv, "h:p:s:n:P:m:")) != -1){
        switch(opt){
            case 'h':
            strcpy(server_ip,optarg);
            break;
            case 'p':
            server_port = atoi(optarg);
            break;
            case 's':
            sex = atoi(optarg);
            break;
            case 'n':
            strcpy(name, optarg);
            break;
            case 'P':
            strcpy(passwd, optarg);
            break;
            case 'm':
            mode = atoi(optarg);
            break;
            default:
            fprintf(stderr, "Usage : %s -p port -P passwd -n name -h hostt -s sex.\n", argv[0]);
            exit(1);
        }
    }

    if(!strlen(name)) strcpy(name, get_conf_value(conf,"NAME"));
    if(!strlen(passwd)) strcpy(passwd, get_conf_value(conf,"PASSWD"));
    if(!strlen(server_ip)) strcpy(server_ip, get_conf_value(conf,"HOST"));
    if(sex < 0) sex = atoi(get_conf_value(conf,"SEX"));
    if(!server_port) server_port =atoi(get_conf_value(conf,"PORT"));

    DBG(BLUE"<D>"NONE" : name = %s\n",name);
    DBG(BLUE"<D>"NONE" : passwd = %s\n",passwd);
    DBG(BLUE"<D>"NONE" : server_ip = %s\n",server_ip);
    DBG(BLUE"<D>"NONE" : server_port = %d\n", server_port);
    DBG(BLUE"<D>"NONE" : sex = %s\n", sex == 1?"男":"女");

    if((sockfd = socket_connect(server_ip, server_port)) < 0){
        perror("sockfd");
        exit(1);
    }

    struct wechat_msg signupMsg;
    bzero(&signupMsg, sizeof(struct wechat_msg));
    if(!mode){
        DBG(BLUE"<D>"NONE" : Program running in signup mode.\n");
        signupMsg.type = WECHAT_SIGNUP;
    }else{
        signupMsg.type = WECHAT_SIGNIN;
        DBG(BLUE"<Mode>"NONE" : program is running in signin mode.\n");
    }
    strcpy(signupMsg.name, name);
    strcpy(signupMsg.passwd, passwd);
    signupMsg.sex = sex;
    send(sockfd, (void*)&signupMsg, sizeof(signupMsg), 0);
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    if(select(sockfd+1, &rfds, NULL, NULL, &tv) <= 0){
        fprintf(stderr, RED"<System>"NONE" : connect is time out.\n");
        exit(1);
    }

    int ret = recv(sockfd, (void*)&signupMsg, sizeof(struct wechat_msg), 0);
    if(ret <= 0){
        fprintf(stderr, RED"<System>"NONE" : Server return a success for signup.\n");
        exit(1);
    }
    if(signupMsg.type & WECHAT_ACK){
        DBG(GREEN"<Mode>"NONE" : recv success mode = %d.\n",signupMsg.type);
        if(!mode){
            DBG(YELLOW"Please run again.\n"NONE);
            exit(0);
        }
    }else{
        DBG(RED"<Failure>"NONE" : Server return a failure for signup.\n");
        exit(1);
    }
    //struct wechat_msg msg;

    pthread_t tid;
    pthread_create(&tid,NULL,client_work,(void*)&sockfd);

    signal(SIGINT, logout);
    while(1){
        DBG(BLUE"Please Input:\n");
        memset(signupMsg.msg, 0, sizeof(signupMsg.msg));
        scanf("%[^\n]s", signupMsg.msg);
        getchar();
        signupMsg.type = WECHAT_WALL;
        if(signupMsg.msg[0] == '@'){
            signupMsg.type = WECHAT_MSG;
            strncpy(signupMsg.to, signupMsg.msg+1,strchr(signupMsg.msg,' ')-signupMsg.msg-1);
        }
        if(!strlen(signupMsg.msg)) continue;
        send(sockfd, (void *)&signupMsg, sizeof(signupMsg), 0);
        // system("clear");
    }


    return 0;
}
