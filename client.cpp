#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <iostream>
#include <cstring>
#include <string>

#define MAX_MSG_SIZE 510
#define RCV_TYPE 4321
#define SND_TYPE 1234

struct mymsgbuf {
   long mtype;
   char mtext[MAX_MSG_SIZE];

};

int main(int argc, char **argv) {

   key_t key;
   int qid;
   struct mymsgbuf qbuf; 

   if(argc != 2){
      std::cout << "client requires exactly 1 string as an argument" << std::endl;
   
   }

   //Connect to the message queue
   key = ftok(".", 'q');
   if( (qid = msgget(key, 0)) == -1) {
      perror("Failed to get message queue");
      exit(EXIT_FAILURE);
   }

   //Setup message buffer to be sent
   std::string message = std::string(argv[1]);
   strncpy(qbuf.mtext, message.c_str(), MAX_MSG_SIZE-10);
   qbuf.mtext[MAX_MSG_SIZE - 1] = '\0';
   qbuf.mtype = SND_TYPE;

   //Send raw message to server
   if((msgsnd(qid, &qbuf, strlen(qbuf.mtext)+1, 0)) == -1) {
      perror("Message Send Failed");
      exit(EXIT_FAILURE);
   }

   //Receive formatted message from server
   qbuf.mtype = RCV_TYPE;
   if((msgrcv(qid, &qbuf, MAX_MSG_SIZE+10, RCV_TYPE, 0)) == -1) {
      perror("Did not receive formatted message");
      exit(EXIT_FAILURE);
   }

   //output to STDOUT
   std::cout << std::string(qbuf.mtext); 

   return EXIT_SUCCESS;

}

