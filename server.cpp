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
#define RCV_TYPE 1234
#define SND_TYPE 4321

struct mymsgbuf {
   long mtype;
   char mtext[MAX_MSG_SIZE];

};

std::string formatMessage(std::string message);

int main(int argc, char **argv) {

   key_t key;
   int qid;
   struct mymsgbuf qbuf; 


   key = ftok(".", 'q');
   //Create the message queue
   if( (qid = msgget(key, IPC_CREAT | 0666)) == -1) {
      perror("Failed to create message queue");
      exit(EXIT_FAILURE);
   }

   while(1) {
      qbuf.mtype = RCV_TYPE;
      msgrcv(qid, &qbuf, MAX_MSG_SIZE, RCV_TYPE, 0);

      std::string message = std::string(qbuf.mtext);
      message = formatMessage(message);
      strncpy(qbuf.mtext, message.c_str(), MAX_MSG_SIZE);
      qbuf.mtype = SND_TYPE;

      if((msgsnd(qid, &qbuf, strlen(qbuf.mtext)+1, 0)) == -1) {
         perror("Message Send Failed");
         break;
      }
   }

   msgctl(qid, IPC_RMID, 0);

   //std::string test = "typing is propobaly the hardest thing absolutely charming and bragadocious willingly superfluous and meaningles to the point of excess";
   //test = formatMessage(test);
   //std::cout << test;

   return EXIT_SUCCESS;

}

std::string formatMessage(std::string message) {

   unsigned int lastNewLine = 0;
   for(unsigned int i = 79; i < message.length(); i--){
      if(message[i] == ' '){
         message.replace(i,1, "\n");
         lastNewLine = i;
         i+=80;
      } else if (i == lastNewLine){
         i+=80;
         lastNewLine = i;
         message.insert(i, "\n");
         i+=80;
      }
   }
   message.append("\n");

   return message; 
}
