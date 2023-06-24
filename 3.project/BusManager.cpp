#include<iostream>
using namespace std;
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<cstring>

int main(){

    int shmid = shmget((key_t)9999,10240*12,IPC_CREAT|0640);
    if(shmid < 0){
        return -1;
    }

    char *shmp = (char *)shmat(shmid, NULL, 0);
    if(shmp < 0) return -1;
    memset(shmp,0,10240*12);
    int m;
    scanf("%d",&m);

    shmdt(shmp);
    return 0;
}
