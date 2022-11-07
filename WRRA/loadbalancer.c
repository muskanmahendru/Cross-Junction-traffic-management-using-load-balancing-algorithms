#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include<time.h>
#define PORT 8889


//Server 1 weight is 3.
// Server 2 weight is 2.

int main(int argc, char const *argv[]) 
{ 
time_t starttime,endtime;
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};
    char *hello = "Hello from server"; 
int i,s=0,c=0,s1w=3,s2w=2;
int clientarray[5]={0,0,0,0,0};
int serverarray[2]={0,0};
int new_server[2],new_client[5];
char buffer1[1024]="exit";
int j=0;;
float Totaltime[5];
float sum;
float suma=0;
int waitingtime[5];
       

printf("\n Loadbalancer started working.[ Weighted Round Robin Algorithm] ----->\n");

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

while(s<2)
{

for(i=0;i<2;i++)
{

if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
{ 
        perror("accept"); 
        exit(EXIT_FAILURE); 
} 

if(serverarray[i]==0)
{
serverarray[i]=new_socket;

printf("New server connection , socket fd is %d , ip is : %s , port : %d  \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs (address.sin_port));
}
s=s+1;
}


}


while(c<4)
{

for(i=0;i<5;i++)
{

if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
{ 
        perror("accept"); 
        exit(EXIT_FAILURE); 
} 

if(clientarray[i]==0)
{
clientarray[i]=new_socket;

printf("New client connection , socket fd is %d , ip is : %s , port : %d  \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs (address.sin_port));
time(&starttime);
}

c=c+1;
}


}



while(1)
{
for(i=0;i<5;i++)
{
valread=read(clientarray[i],buffer,1024);
if(valread==0)
{  getpeername(clientarray[i] , (struct sockaddr*)&address ,  
                        (socklen_t*)&addrlen);   
                    printf("Client disconnected , ip %s , port %d \n" ,  
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));   

                    close(clientarray[i]);   
                    clientarray[i] = 0; 
}


if(valread>0)
{
if(s1w>0)
{
printf("\nClient of ip %s is sent to Server1\n",inet_ntoa(address.sin_addr));
send(serverarray[0],buffer,1024,0);
s1w=s1w-1;
time(&endtime);
//printf("\n%f\n",difftime(endtime,starttime));
Totaltime[j]=difftime(endtime,starttime);
}
else if(s1w==0 && s2w>0)
{

printf("\nClient of ip %s is sent to Server2\n",inet_ntoa(address.sin_addr));
send(serverarray[1],buffer,1024,0);
s2w=s2w-1;

time(&endtime);
//printf("\n%f\n",difftime(endtime,starttime));
Totaltime[j]=difftime(endtime,starttime);
}

}


else if(clientarray[0]==0&&clientarray[1]==0&&clientarray[2]==0&&clientarray[3]==0&&clientarray[4]==0)
{
send(serverarray[0],buffer1,1024,0);

send(serverarray[1],buffer1,1024,0);
/*for(int i=0;i<5;i++)
{
printf("\n%f\n",Totaltime[i]);
}
*/
for(int i=0;i<5;i++)
{
printf("\nThe total time taken by Client %d is %f",i+1,Totaltime[i]);//total time taken
//calculation of waiting time
if(i==0)
{
printf("\nThe waiting time of Client %d is %f\n",i+1,0.0);
waitingtime[0]=0;
}
if(i>0)
{

for(int j=0;j<i;j++)
{
sum=sum+Totaltime[j];
}
printf("\nThe waiting time of Client %d is %f\n",i+1,sum);
waitingtime[i]=sum;
sum=0;
}
suma=suma+waitingtime[i];
}

printf("\nThe average waiting time is %f\n",suma/5);

printf("\nAll Connection are terminated\n");
exit(0);
    
}
j=j+1;



}

}

 return 0; 
} 
