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


   //Create the message queue
   key = ftok(".", 'q');
   if( (qid = msgget(key, IPC_CREAT | 0666)) == -1) {
      perror("Failed to create message queue");
      exit(EXIT_FAILURE);
   }

   //Server waits indefinately until it receives a message
   while(1) {
      qbuf.mtype = RCV_TYPE;
      msgrcv(qid, &qbuf, MAX_MSG_SIZE, RCV_TYPE, 0);

      //Format the message according to requirements, then prepare to send back
      std::string message = std::string(qbuf.mtext);
      message = formatMessage(message);
      strncpy(qbuf.mtext, message.c_str(), MAX_MSG_SIZE);
      qbuf.mtype = SND_TYPE;

      //Send formatted message back to client
      if((msgsnd(qid, &qbuf, strlen(qbuf.mtext)+1, 0)) == -1) {
         perror("Message Send Failed");
         break;
      }
      
   }

   //Delete Message queue if there's an error
   msgctl(qid, IPC_RMID, 0);
   
   return EXIT_SUCCESS;

}

std::string formatMessage(std::string message) {

   unsigned int lastNewLine = 0;
   for(unsigned int i = 79; i < message.length(); i--){
      //Check if the 80th character on the line is not part of a word
      //   if the 80th character is part of word, walk back before the word begins
      if(message[i] == ' '){
         message.replace(i,1, "\n");
         lastNewLine = i;
         i+=80;
      } else if (i == lastNewLine){ //If the word is longer than 80 characters
         i+=80;
         lastNewLine = i;
         message.insert(i, "\n");
         i+=80;
      }
   }
   //Add a newline at the end to make output look clean
   message.append("\n");

   return message; 
}
