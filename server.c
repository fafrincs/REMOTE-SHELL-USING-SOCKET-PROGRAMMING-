#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

//#define MAXPENDING 5
#define BUFFSIZE 2048
#define MAX 2048

void dp (int sock);
void setup(char inputBuffer[], char *args[], int *background);

int main(int argc, char *argv[]) {

   int bytes,pid;
   int sock, serv_host_port, clilen, newsock;
   struct sockaddr_in cli_addr, serv_addr;

   if (argc != 2) {
      
      fprintf(stderr, "Please Enter Server Port Number. \n", argv[0]);
      fprintf(stderr, " standard command :  ./server 7890 \n ");
      //fprintf(stderr, "\t<port>\t- port to listen on\n");
      exit(1);
   } 

   serv_host_port = atoi(argv[1]);

   printf("Listen activating.\n");

   printf("Trying to create socket at port %d...\n", serv_host_port);   

   /* Create socket from which to read */
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("can't open stream socket");
      exit(1);
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(serv_host_port);
   
   if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("can't bind to local address");
      exit(1);
   }

   printf("Socket created! Accepting connections.\n\n");


  // listen(sock, 5);

   /*signal(SIGCHLD, SIG_IGN);
    while(1) {
    
   clilen = sizeof(cli_addr);
   newsock = accept(sock, (struct sockaddr *) &cli_addr, &clilen);
   if (newsock < 0) {
     perror("accepting connection");
     exit(1);
   }
*/
   printf("Connection established with client.\n\n");

   listen(sock, 5);


   clilen = sizeof(cli_addr);
   
   while (1) {
      newsock = accept(sock, (struct sockaddr *) &cli_addr, &clilen);
        
      if (newsock < 0) {
         perror("ERROR on accept");
         exit(1);
      }
      
      /* Create child process */
      pid = fork();
        
      if (pid < 0) {
         perror("ERROR on fork");
         exit(1);
      }
      
      if (pid == 0) {
         
         dp(newsock);
         exit(0);

      }
      else {
         close(newsock);
      }
        
   } 
}

    
void setup(char inputBuffer[], char *args[], int *background)
{
    const char s[4] = " \t\n";
    char *token;
    token = strtok(inputBuffer, s);
    int i = 0;
    while( token != NULL)
    {
        args[i] = token;
        i++;
        token = strtok(NULL,s);
    }
    args[i] = NULL;
}

void dp (int sock) {

char buffer[BUFFSIZE];
int received = -1;
char data[MAX];
memset(data,0,MAX);

while(1) { 
    data[0] = '\0';
    if((received = recv(sock, buffer,BUFFSIZE,0))<0){

        perror("ERROR on Received");
    }

    buffer[received] = '\0';

    strcat (data,  buffer);
    if (strcmp(data, "exit")==0) 
        exit(0);

    puts (data);
        char *args[100];
        setup(data,args,0);
        int pipefd[2],lenght;

        if(pipe(pipefd))
            perror("ERROR on created");

        pid_t pid = fork();
        char path[MAX];

        if(pid==0)
        {
            close(1); 
            dup2(pipefd[1],1); 
            close(pipefd[0]); 
            close(pipefd[1]); 
            execvp(args[0],args); 
        }
        else
            if(pid>0)
            {
                close(pipefd[1]);
                memset(path,0,MAX);
                while(lenght=read(pipefd[0],path,MAX-1)){
                  
                    if(send(sock,path,strlen(path),0) != strlen(path) ){
                        perror("ERROR");
                    }
                    fflush(NULL);
                    
                memset(path,0,MAX);
                }
                close(pipefd[0]);
              
            }
            else
            {
                printf("Error !\n");
            }
    }    
}






