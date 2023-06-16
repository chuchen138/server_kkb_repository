/*************************************************************************
	> File Name: 2.test.c
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Mon 22 May 2023 09:01:16 PM CST
 ************************************************************************/

#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
char buff[25];

int main(){
    FILE* fp = fopen("text","a+");
    if(fp == NULL){
        perror("fopen");
        return -1;
    }
    int ret = 0;
    while (fgets(buff,sizeof(buff),fp) != NULL)
    {
        printf("%s",buff);
    }
    /*
    while(!feof(fp)){
        memset(buff,0,sizeof(buff));
        fread(buff,1,sizeof(buff),fp);
        printf("[%s]\n",buff);
    }*/
    char c[]="iuabsgduj";
    //printf("buff = %d\n",sizeof(buff));
    //fwrite(c,1,sizeof(c),fp);
    fputs("iudbasfvui isdvb",fp);
    //fwrite(c,1,sizeof(c),fp);
    //fwrite(c,1,sizeof(c),fp);
    fclose(fp);
    
    return 0;
}
