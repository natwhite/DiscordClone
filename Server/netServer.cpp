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
typedef boost::shared_ptr<list<socket_ptr>> clientList_ptr;
typedef boost::shared_ptr<list<pair<socket_ptr, int>>> ClientIndex_ptr;

// Create the service, acceptor, mutex, and clientList requied for the program to function
io_service service;
tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), 8001));
boost::mutex mtx;
clientList_ptr clientList(new list <socket_ptr>);
ClientIndex_ptr ClientIndex(new list<pair<socket_ptr, int>>);

// constant for use as a buffer size
const int bufferSize = 1024;
int clientNumber = 0;

// enum to store sleep lengths 
enum sleepLength
{
	sml = 100,
	lng = 200
};

// Function prototypes
void disconnectClient(pair<socket_ptr, int> client);
void acceptorLoop();
void requestLoop();
void relayMessages(string* message);

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

		cout << "New client joined.";

		pair<socket_ptr, int> nextClient = pair<socket_ptr, int>(clientSock, clientNumber++);

		// lock the mutex to ensure this is the only thread changing the lient list
		mtx.lock();
		ClientIndex->emplace_back(nextClient);
		mtx.unlock();

		cout << ClientIndex->size() << " total clients" << endl;
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
		if (!ClientIndex->empty())
		{
			// lock the mutex to insure that no other threads interfere 
			mtx.lock();
			// check each client in the client list
			for (auto& client : *ClientIndex)
			{
				if (client.first->available())
				{
					char readBuffer[bufferSize] = { 0 };

					// read the message from the client
					int bytesRead = client.first->read_some(buffer(readBuffer, bufferSize));

					string_ptr message(new string(readBuffer, bytesRead));

					// if the message was "exit", disconnect the client from the server
					if (message->find("exit") != string::npos)
					{
						disconnectClient(client);
						break;
					}

					string* taggedMessage = (new string(to_string(client.second) + " : " + *message));

					relayMessages(taggedMessage);

					cout << *taggedMessage << endl;
				}
			}

			mtx.unlock();
		}

		boost::this_thread::sleep(boost::posix_time::millisec((int)sleepLength::lng));
	}
}

void relayMessages(string* message) {
	for (auto& client : *ClientIndex)
		client.first->write_some(buffer(*message, bufferSize));
}

// This function disconnects the client from the server and updates the client list
void disconnectClient(pair<socket_ptr, int> client)
{
	// store the position of the client on the client list
	auto position = find(ClientIndex->begin(), ClientIndex->end(), client);

	// shutdown and close the client socket
	client.first->shutdown(tcp::socket::shutdown_both);
	client.first->close();

	// remove the client from the client list
	ClientIndex->erase(position);

	// display that a client disconnected and the new number of connected clients on the server
	cout << "Client Disconnected! " << clientList->size() << " total clients" << endl;
}