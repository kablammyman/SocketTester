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
	if (argc < 6)
	{
		printf("not enough args, SocketTester.exe -ip <ip4> -port<port> -data <what you want to send>");
		exit(0);
	}

	string ip;
	string dataToSend;
	int port = -1;
	NetworkConnection conn;

	int i = 0;
	while (i < argc)
	{
		if (strcmp(argv[i], "-ip") == 0)
		{
			i++;
			ip = argv[i];
		}
		else if (strcmp(argv[i], "-port") == 0)
		{
			i++;
			port = atoi(argv[i]);
		}
		else if (strcmp(argv[i], "-data") == 0)
		{
			i++;
			dataToSend = argv[i];
		}
		i++;
	}
	if (ip.empty() || dataToSend.empty() || port == -1)
	{
		printf("invalid args. SocketTester.exe -ip <ip4> -port<port> -data <what you want to send>");
		exit(0);
	}
	printf("trying to connect to connect to: %s on %d...\n", ip.c_str(), port);

	if (conn.connectToServer(ip, port) == NETWORK_ERROR)
	{
		printf("couldn't connect to: %s on %d\n",ip.c_str(), port);
		exit(-1);
	}
	printf("sending after 1 second: %s\n", dataToSend.c_str());
	Sleep(1000);
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