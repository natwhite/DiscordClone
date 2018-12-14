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

	// This function listens on the socket for messages from the server and updates the message queue
	// when a message is received from the server
	void inboundLoop(socket_ptr sock)
	{
		int bytesRead = 0;
		char readBuffer[1024] = { 0 };

		// loop forever
		for (;;)
		{
			if (sock->available())
			{
				// read the message from the server
				bytesRead = sock->read_some(buffer(readBuffer, INPUTSIZE));
				string_ptr message(new string(readBuffer, bytesRead));

				// add message to the message queue
				messageQueue->push(message);
			}

			// sleep to avoid thread conflicts
			boost::this_thread::sleep(boost::posix_time::millisec(1000));
		}
	}

	static socket_ptr localSock;
	static string inputMessage;

	void sendMessage(const char* message) {
		inputMessage = string(message) + '\n';
		localSock->write_some(buffer(inputMessage, INPUTSIZE));
	}

	// This function takes input from the user and sends the given string to the server
	void writeLoop(socket_ptr sock)
	{
		//char inputBuffer[INPUTSIZE] = { 0 };

		// loop forever
		for (;;)
		{
			// get input from the user
			//cin.getline(inputBuffer, INPUTSIZE);
			//inputMessage = (string)inputBuffer + '\n';

			// if the user types a message, send it to the server
			if (!inputMessage.empty())
			{
				sock->write_some(buffer(inputMessage, INPUTSIZE));
			}

			// if the user types "exit", exit the program
			if (inputMessage.find("exit") != string::npos)
			{
				exit(1);
			}

			// reset the input string and buffer to prepare for the next message
			//inputMessage.clear();
			//memset(inputBuffer, 0, INPUTSIZE);
		}
	}

	// This function displays messages from the message queue
	void displayLoop(socket_ptr sock)
	{
		// loop forever
		for (;;)
		{
			// if a there is a message in the queue, print it to the screen
			if (!messageQueue->empty())
			{
				cout << "\n" + *(messageQueue->front()) + "\n: ";

				// remove message from the queue once it has been printed
				messageQueue->pop();
			}

			// pause the thread to avoid thread conflicts
			boost::this_thread::sleep(boost::posix_time::millisec(1000));
		}
	}

	int initializeConnection(const char* connection) {
		string ipAddress = string(connection);
		tcp::endpoint ep(ip::address::from_string(ipAddress), 8001); // connect to a server on the localhost at port 8001
																	   // constant to be used as the input size

		try {
			boost::thread_group threads;
			socket_ptr sock(new tcp::socket(service));
			localSock = sock;

			// connect to the server
			sock->connect(ep);
			//cout << "Welcome to client!\nTypeMessages for the server!\nType \"exit\" to quit\n: ";

			// create threads
			threads.create_thread(boost::bind(displayLoop, sock));
			threads.create_thread(boost::bind(inboundLoop, sock));
			//sendMessage(connection);
			//threads.create_thread(boost::bind(writeLoop, sock));

			//threads.join_all();
		}
		catch (const std::exception& e)
		{
			cerr << "Error: " << e.what() << endl;
		}

		//puts("Press any key to continue...");
		//getc(stdin);
		////return EXIT_SUCCESS;
		return 1;
	}
}
