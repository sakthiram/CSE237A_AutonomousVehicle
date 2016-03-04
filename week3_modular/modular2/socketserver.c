#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include "create.h"
#include <string.h>

void body_socket(SharedVariable* sv) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n,i;
   
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 9225;
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */
   listen(sockfd,5); 
  clilen = sizeof(cli_addr);
   while(1){	
   printf("Inside While\n");
   /* Accept actual connection from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	printf("accept done\n");	
   if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
   }
   
   /* If connection is established then start communicating */
   bzero(buffer,256);
   n = read( newsockfd,buffer,255 );
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   for(i = 0; i<10 ; i++ ){	
   sv->directions[i] = buffer[i];
   }
   switch(sv->directions[0]){
   	case 'B': sv->turn = 2;
		     break;
	   case 'C': sv->turn = 1;	
		     break;
  	 case 'D': sv->turn = 0;
		     break;
  	 default:  printf("Cannot find destination \n");
		     break;	  
   }
   printf("Directions is %s, turn is %d",sv->directions,sv->turn);			  
   sv->manual_stop = 0;
   sv->init_start = 1;	
  close(newsockfd);
}  				     
}
