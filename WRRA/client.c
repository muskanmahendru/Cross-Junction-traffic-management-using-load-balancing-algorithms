#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#define PORT 8889



int main(char const *argv,int argc)
{
FILE *fp;
time_t starttime,endtime;
int clientfd,valread;
struct sockaddr_in addr;
char msg[1024];
char buffer[1024]={0};
char wtime[1024];

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

/*
valread=read(clientfd,buffer,1024);
printf("\n%s\n",buffer);
*/

while(1)
{

//printf("\nstart time is %s\n",ctime(&starttime));

printf("\nConnected to Server.Enter the text.--->\n");
scanf("%s",&msg);


if(strcmp(msg,"exit")==0)
{
break;
}
time(&starttime);
send(clientfd,msg,1024,0);
//printf("\nend time is %s\n",ctime(&endtime));
//printf("The time taken to send a message is %f",difftime(endtime,starttime));
//sprintf(wtime, "%f",starttime);

fp=fopen("st.txt","a");
fputs(ctime(&starttime),fp);
fputs("\n",fp);
fclose(fp);



//printf("\n%s\n",wtime);

}



return 0;

}
