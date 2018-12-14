/*
Server for Networking assignment
*/

// includes for c++ standard libraries
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <cstdlib>

// Includes for boost libraries
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/asio/ip/tcp.hpp>

// namespaces to make the code easier to read
using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

// typedefs to make the code easier to read
typedef boost::shared_ptr<tcp::socket> socket_ptr;
typedef boost::shared_ptr<string> string_ptr;
typedef map<socket_ptr, string_ptr> clientMap;
typedef boost::shared_ptr<clientMap> clientMap_ptr;
typedef boost::shared_ptr<list<socket_ptr>> clientList_ptr;

// Create the service, acceptor, mutex, and clientList requied for the program to function
io_service service;
tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), 8001));
boost::mutex mtx;
clientList_ptr clientList(new list <socket_ptr>);

// constant for use as a buffer size
const int bufferSize = 1024;

// enum to store sleep lengths 
enum sleepLength
{
	sml = 100,
	lng = 200
};

// Function prototypes
bool clientSentExit(string_ptr);
void disconnectClient(socket_ptr);
void acceptorLoop();
void requestLoop();

// This function creates the threads that will communicate with the client
int main()
{
	boost::thread_group threads;

	threads.create_thread(boost::bind(acceptorLoop));
	boost::this_thread::sleep(boost::posix_time::millisec((int)sleepLength::sml));

	threads.create_thread(boost::bind(requestLoop));
	boost::this_thread::sleep(boost::posix_time::millisec((int)sleepLength::sml));

	threads.join_all();

	puts("Press any key to exit...");
	getc(stdin);
	return EXIT_SUCCESS;
}

// This function accepts client connections and keeps track of the number of 
// clients that are connected
void acceptorLoop()
{
	cout << "Waiting for clients..." << endl;

	// loop forever
	for (;;)
	{
		socket_ptr clientSock(new tcp::socket(service));

		acceptor.accept(*clientSock);

		cout << "New client joined. ";

		// lock the mutex to ensure this is the only thread changing the lient list
		mtx.lock();
		clientList->emplace_back(clientSock);
		mtx.unlock();

		cout << clientList->size() << " total clients" << endl;
	}
}

// This function recieves the message from the client and returns it back to the client
void requestLoop()
{
	// loop forever
	for (;;)
	{
		// if there are clients connected, scan the client list until a message is received, 
		//then return that message to the client
		if (!clientList->empty())
		{
			// lock the mutex to insure that no other threads interfere 
			mtx.lock();
			// check each client in the client list
			for (auto& clientSock : *clientList)
			{
				if (clientSock->available())
				{
					char readBuffer[bufferSize] = { 0 };

					// read the message from the client
					int bytesRead = clientSock->read_some(buffer(readBuffer, bufferSize));

					string_ptr message(new string(readBuffer, bytesRead));

					// if the message was "exit", disconnect the client from the server
					if (clientSentExit(message))
					{
						disconnectClient(clientSock);
						break;
					}

					// send the message back to the client with "Server: " appended to the front to let the
					// client user know that this message came from the server
					*message = "Server: " + *message;

					clientSock->write_some(buffer(*message, bufferSize));

					cout << "MessageLog: " << *message << endl;
				}
			}

			mtx.unlock();
		}

		boost::this_thread::sleep(boost::posix_time::millisec((int)sleepLength::lng));
	}
}

// This function checks if the client message is "exit"
bool clientSentExit(string_ptr message)
{
	if (message->find("exit") != string::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// This function disconnects the client from the server and updates the client list
void disconnectClient(socket_ptr clientSock)
{
	// store the position of the client on the client list
	auto position = find(clientList->begin(), clientList->end(), clientSock);

	// shutdown and close the client socket
	clientSock->shutdown(tcp::socket::shutdown_both);
	clientSock->close();

	// remove the client from the client list
	clientList->erase(position);

	// display that a client disconnected and the new number of connected clients on the server
	cout << "Client Disconnected! " << clientList->size() << " total clients" << endl;
}