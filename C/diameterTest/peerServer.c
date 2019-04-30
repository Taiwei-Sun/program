#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define SERVER_IP "0.0.0.0"


int main()
{

 int server_sockfd, client_sockfd;
 int server_len, client_len;
 struct sockaddr_in server_address;
 struct sockaddr_in client_address;




 
 server_sockfd=socket(AF_INET, SOCK_STREAM, 0);
 
 server_address.sin_family=AF_INET;
 server_address.sin_addr.s_addr=inet_addr(SERVER_IP);
 server_address.sin_port=htons(3868);

 server_len=sizeof(server_address);
 bind(server_sockfd, (struct sockaddr*)&server_address, server_len);

 listen(server_sockfd, 5);
 while(1)
 {
  unsigned char buffer[BUFFER_SIZE];
  int in_len=0;
  int i;

  printf("Server waitting...\n");
  client_len=0;
  client_len=sizeof(client_address);
  client_sockfd=accept(server_sockfd, (struct sockaddr*)&client_address, &client_len);

  printf("client connected...\n");
  in_len=read(client_sockfd, buffer, BUFFER_SIZE);
  printf("incoming length=%d\n",in_len);
 
  printf("Start--hex\n");
  for(i=0;i<in_len;i++)
   printf("%02x",buffer[i]);

  printf("\n---char\n");
  for(i=0;i<in_len;i++)
   if(buffer[i]>31&&buffer[i]<127)
    printf("%c",buffer[i]);
   else printf("X");
  printf("\nEnd\n");
 

  //write(client_sockfd, &ch, 1);
  close(client_sockfd);


 }



}


