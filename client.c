#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#define BUFFSIZE 2048

int main(int argc, char *argv[]) {
   char buf[80]; 
   unsigned long int inaddr;
   int sock, serv_host_port;
   struct sockaddr_in serv_addr;
   char *serv_host_addr;
   struct hostent *hp;
   int sid;
   unsigned int len;
   int received = 0;
   char buffer[256];

   if (argc != 3) {
      fprintf(stderr, "Enter IP address and Port Number", argv[0]);
      fprintf(stderr, "Standard command: \n ./client 127.0.0.1 7890");
       exit(1);
   }

   serv_host_addr = argv[1];
   serv_host_port = atoi(argv[2]);
   
   printf("Trying to connect to server %s at port %d...\n", serv_host_addr, serv_host_port);
   bzero((void *) &serv_addr, sizeof(serv_addr));
   printf("Looking up %s...\n", serv_host_addr);

   if ((hp = gethostbyname(serv_host_addr)) == NULL) {
     perror("host name error");
     exit(1);
   }

   bcopy(hp->h_addr, (char *) &serv_addr.sin_addr, hp->h_length);
   printf("Found it.  Setting port connection to %d...\n", serv_host_port);

   puts("Done. Opening socket...");
   if ((sock  = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
     perror("error in opening socket");
     exit(1);
   }
  
    

        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
        serv_addr.sin_port = htons(atoi(argv[2]));
        if (connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        {
            perror("Failed to connect with server");
        }
        char s[100];


        while(1) { 

        printf("Enter the command you want to execute: >");

        fflush(stdout);
        fgets(s,100,stdin);
        s[strlen(s)-1]='\0'; 
        len = strlen(s);

       
        if (send(sock, s, len, 0) != len) 
        {
            perror("Mismatch in number of sent bytes");
        }
        if(strcmp(s,"exit") == 0) // check if exit is typed
        exit(0);
        fprintf(stdout, "Server Output:\n ");
        while (received < len) 
        {
            int bytes = 0;
           
            if ((bytes = recv(sock, buffer, len, 0)) < 1) 
            {
                perror("Failed to receive bytes from server");
            }
            received += bytes;
            buffer[bytes] = '\0';
          
            fprintf(stdout, buffer);
        }
        int bytes = 0;
       do {
            buffer[bytes] = '\0';
            printf("%s\n", buffer);
        } while((bytes = recv(sock, buffer, BUFFSIZE-1, 0))>=BUFFSIZE-1);
        buffer[bytes] = '\0';
        printf("%s\n", buffer);
        printf("\n");
    }

}
