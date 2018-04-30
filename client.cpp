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

   key = ftok(".", 'q');
   //Create the message queue
   if( (qid = msgget(key, 0)) == -1) {
      perror("Failed to get message queue");
      exit(EXIT_FAILURE);
   }

   
   std::string message = std::string(argv[1]);

   strncpy(qbuf.mtext, message.c_str(), MAX_MSG_SIZE-10);
   qbuf.mtext[MAX_MSG_SIZE - 1] = '\0';
   qbuf.mtype = SND_TYPE;

   if((msgsnd(qid, &qbuf, strlen(qbuf.mtext)+1, 0)) == -1) {
      perror("Message Send Failed");
      exit(EXIT_FAILURE);
   }

   qbuf.mtype = RCV_TYPE;
   if((msgrcv(qid, &qbuf, MAX_MSG_SIZE+10, RCV_TYPE, 0)) == -1) {
      perror("Did not receive formatted message");
      exit(EXIT_FAILURE);
   }

   std::cout << std::string(qbuf.mtext); 

   

   //std::string test = "typing is propobaly the hardest thing absolutely charming and bragadocious willingly superfluous and meaningles to the point of excess";
   //test = formatMessage(test);
   //std::cout << test;

   return EXIT_SUCCESS;

}

