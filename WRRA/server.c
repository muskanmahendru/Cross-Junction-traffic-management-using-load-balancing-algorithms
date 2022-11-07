#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#define PORT 8889


int main(char const *argv,int argc)
{
int clientfd,valread;
time_t endtime;
FILE *fp;

struct sockaddr_in addr;
char *msg="car1";
char buffer[1024]={0};

clientfd=socket(AF_INET,SOCK_STREAM,0);
if(clientfd<0)
{
printf("\nSocket creation failed.\n");
return -1;
}

addr.sin_family=AF_INET;
addr.sin_port=htons(PORT);

int result=inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);
if(result<=0)
{
printf("Address not supported");
return -1;
}

int connectresult=connect(clientfd,(struct sockaddr *)&addr,sizeof(addr));
if(connectresult<0)
{
printf("\nConnection failed\n");
return -1;
}
while(1)
{
valread=read(clientfd,buffer,1024);
time(&endtime);
printf("\n%s\n",buffer);

if(strcmp(buffer,"exit")==0)
{
break;
}

fp=fopen("et.txt","a");
fputs(ctime(&endtime),fp);
fputs("\n",fp);
fclose(fp);

}


return 0;

}
