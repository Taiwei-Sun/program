#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
//#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>

//unsigned long int htonl(unsigned long int hostlong);
//unsigned short int htons(unsigned short int hostshort);

int main()
{
 int server_sockfd, client_sockfd;
 int server_len, client_len;
 //struct sockaddr_un server_address;
 //struct sockaddr_un client_address;
 struct sockaddr_in server_address;
 struct sockaddr_in client_address;
 
 /*unlink("server_socket");
 server_sockfd=socket(AF_UNIX, SOCK_STREAM, 0);*/
 server_sockfd=socket(AF_INET, SOCK_STREAM, 0);
 
 /*server_address.sun_family=AF_UNIX;
 strcpy(server_address.suni_path, "server_socket");*/
 server_address.sin_family=AF_INET;
 server_address.sin_addr.s_addr=inet_addr("127.0.0.1");
 //address.sin_addr.s_addr=htonl(INADDR_ANY);
 server_address.sin_port=htons(9734);

 server_len=sizeof(server_address);
 bind(server_sockfd, (struct sockaddr*)&server_address, server_len);

 listen(server_sockfd, 5);
 while(1)
 {
  char ch; 

  printf("Server waitting\n");

  client_len=sizeof(client_address);
  client_sockfd=accept(server_sockfd, (struct sockaddr*)&client_address, &client_len);


  read(client_sockfd, &ch, 1);
  ch++;
  write(client_sockfd, &ch, 1);
  close(client_sockfd);


 }



}


