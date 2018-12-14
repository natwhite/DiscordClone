#pragma once
// Includes for c++ standard libraries
#include <iostream>
#include <queue>
#include <string>
#include <cstdlib>
#include <stdexcept>

// Includes for boost libraries
#include <boost\thread.hpp>
#include <boost\bind.hpp>
#include <boost\asio.hpp>
#include <boost\asio\ip\tcp.hpp>
#include <boost\algorithm\string.hpp>

// namespaces to make the code easier to read
using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

// typedefs to make the code easier to read
typedef boost::shared_ptr<tcp::socket> socket_ptr;
typedef boost::shared_ptr<string> string_ptr;
typedef boost::shared_ptr<queue<string_ptr>> messageQueue_ptr;

namespace DiscordCopy {
	extern "C" { __declspec(dllexport) double Add(double a, double b); }
	extern "C" { __declspec(dllexport) int initializeConnection(const char* connection); }
	extern "C" { __declspec(dllexport) void sendMessage(const char* message); }
	extern "C" { __declspec(dllexport) void displayLoop(socket_ptr sock); }
	extern "C" { __declspec(dllexport) void inboundLoop(socket_ptr sock); }
	extern "C" { __declspec(dllexport) void writeLoop(socket_ptr sock); }
}
