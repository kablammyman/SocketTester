// SocketTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>

#include "NetworkConnection.h"

using namespace std;

#define DEFAULT_BUFLEN 512

int main(int argc, char * argv[])
{
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	bool done = false;
	int socketIndex = 0;
	
	string ip = "127.0.0.1";
	//string dataToSend = "-hash,C:\\Users\\Victor\\Desktop\\Imagens\\rad1E0A3.jpg";
	string dataToSend = "-ADDIMG|C:\\Users\\Victor\\Desktop\\Imagens\\rad1E0A3.jpg";
	int port = 2345;//port for createImageHash
	NetworkConnection conn;

	if (conn.connectToServer(ip, port) == NETWORK_ERROR)
	{
		printf("couldn't connect to: %s on %d\n",ip.c_str(), port);
		exit(-1);
	}
	
	
	conn.sendData(socketIndex, dataToSend.c_str());

	do {
		if (conn.hasRecivedData(socketIndex))
		{
			int iResult = conn.getData(socketIndex, recvbuf, DEFAULT_BUFLEN);
			if (iResult > 0)
			{
				recvbuf[iResult] = '\0';
				printf("%s -> %d bytes.\n", recvbuf, iResult);

			}
			else if (iResult == 0)//client disconnected
				printf("server disconnected\n");
			else
				printf("recv failed: %d\n", WSAGetLastError());

			done = true;
		}
	} while (!done);

	conn.shutdown();
	return 0;
}