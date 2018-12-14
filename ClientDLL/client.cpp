// DllTest7.cpp : Defines the exported functions for the DLL application.
#include "client.h"

// Create the service, message queue, and endpoint needed for the client to function
io_service service;
messageQueue_ptr messageQueue(new queue<string_ptr>);
const int INPUTSIZE = 256;

// pointer to a copy of the prompt
string_ptr promptCpy;

namespace DiscordCopy {

	double Add(double a, double b) {
		return a + b;
	}

	static socket_ptr localSock;
	static string inputMessage;

	// This function listens on the socket for messages from the server and updates the message queue
	// when a message is received from the server
	void inboundLoop()
	{
		int bytesRead = 0;
		char readBuffer[1024] = { 0 };

		// loop forever
		for (;;)
		{
			if (localSock->available())
			{
				// read the message from the server
				bytesRead = localSock->read_some(buffer(readBuffer, INPUTSIZE));
				string_ptr message(new string(readBuffer, bytesRead));

				// add message to the message queue
				messageQueue->push(message);
			}

			// sleep to avoid thread conflicts
			boost::this_thread::sleep(boost::posix_time::millisec(200));
		}
	}

	void sendMessage(const char* message) {
		inputMessage = string(message) + '\n';
		localSock->write_some(buffer(inputMessage, INPUTSIZE));
	}

	void loadMessage(char* strBldr, int size) {

		// if a there is a message in the queue, print it to the screen
		if (!messageQueue->empty())
		{
			const char* nextMsg = (*(messageQueue->front())).c_str();

			for (size_t i = 0; i < size; i++) {
				strBldr[i] = nextMsg[i];
			}

			// remove message from the queue once it has been printed
			messageQueue->pop();
		}
		
		//// pause the thread to avoid thread conflicts
		//boost::this_thread::sleep(boost::posix_time::millisec(1000));
	}

	void initializeConnection(const char* connection) {
		string ipAddress = string(connection);
		tcp::endpoint ep(ip::address::from_string(ipAddress), 8001); // connect to a server on the localhost at port 8001
																	   // constant to be used as the input size

		try {
			boost::thread_group threads;
			socket_ptr sock(new tcp::socket(service));
			localSock = sock;

			// connect to the server
			sock->connect(ep);

			// create threads
			//threads.create_thread(boost::bind(displayLoop, sock));
			threads.create_thread(boost::bind(inboundLoop));
			//threads.create_thread(boost::bind(writeLoop, sock));

		}
		catch (const std::exception& e)
		{
			cerr << "Error: " << e.what() << endl;
		}
	}
}