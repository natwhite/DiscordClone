Welcome to Discord Clone! 
The basis for a discord application look-alike using a C# front-end and a C++ backend interfacting through a DLL.
All the neccessary files can be found in the Debug directory.

Usage instructions:
Run Debug/startUp.bat which will open a server and client
All of the clients connect to the server (specified in the batch file) if able
Clients can send a messages by typing in the lower textbox and pushing the send button
all messages will be relayed to all other clients connected to the server

Files : 
server.exe  is very similar to the networking assignment, but has been tweeked to rebroadcast messages to all connected clients.
Csharp-GUI.exe is a C# form that uses the compiled ClientDLL.dll to connect to the specified server
ClientDLL.dll is a compiled c++ libary that uses boosts networking capabilities to connect to the specified server