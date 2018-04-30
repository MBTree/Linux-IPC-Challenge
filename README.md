# Linux-IPC-Challenge
Client and Server pair to format terminal output



## IPC Choice

I chose to use SYSV message queues for 3 reasons

  1. The client and server processes needed to be able to communicate regardless of their process hierarchy.

  2. Message queues make retrieving messages simpler because messages in the queue are discrete objects.

  3. The asynchronous nature of the queue will be beneficial when scaling up.

  ### Strengths and Weaknesses of Message Queues
  
  * Strengths
  	* Allow communication between processes regardless of hierarchy
  	* Messages in the queue are separate objects
	* Message types allow multiple different clients to use one message queue effectively
	* Can be used to communicate asynchronously
	* Structure of each message can be customized to fit the needs of the application
	* Simple structure and easy to use
  * Weaknesses
  	* Messages have an inherent limit on their size (usually 4kB or 8kB)
	* Limit on the size of the whole queue

## Scaling Up

Currently, the server could receive thousands of requets per second from a single client fairly well, depending on the messages. The worst case time to send from the client, receive at the server, format the message, then return to the client, is O(n) where n is the size of the message. However, that scenario only happens when the whole message is one word, meaning the actual time is closer to O(1) if that is an unusual type of message.  

Scaling to receive messages from multiple clients would require sending a unique key, likely the pid of the client, with each message so that the server can then use that key as the message type. This way many different clients can use the same message queue without receiving messages sent to the server from other clients. Scaling up very large might require the server to handle a pool of message queues and assign new clients to new message pools as old ones fill up. 
