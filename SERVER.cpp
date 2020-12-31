#include<iostream>
#include<cstdlib>
#include<winsock.h>

#define PORT 7809
using namespace std;
fd_set fr, fw, fe;
int nMaxfd;
int nSocket;
int nArrClient[5];
void ProcessnewMessage(int nClientSocket)
{
	//cout << "\nProcessing the new message for client socket: " << nClientSocket;
	char buff[257 + 1] = { 0, };
	int nRet = recv(nClientSocket, buff, 257, 0);
	cout << "\nProcessing the new message for client socket: " << nClientSocket;
	//time(&start);
	if (nRet < 0)
	{
		cout << "\nClosing the connection for Client\n";
		//time(&finish);
		closesocket(nClientSocket);
		for (int nIndex = 0; nIndex < 5; nIndex++)
		{
			if (nArrClient[nIndex] == nClientSocket)
			{
				nArrClient[nIndex] = 0;
				break;
			}
		}

	}
	else
	{
		if (((rand() % 2) + 1) == 2)
		{

			cout << "\nThe message from the client is: " << buff;
			send(nClientSocket, "Processed your request", 23, 0);
			//time(&finish);
			cout << "\n=========================================================================\n";

		}
		else
		{
			for (int nIndex = 0; nIndex < 5; nIndex++)
			{
				if (nArrClient[nIndex] == nClientSocket)
				{
					Beep(4000, 300);
					Beep(4000, 300);
					Beep(4000, 300);
					cout << "\nThe message from the client is: " << buff;
					//send(nClientSocket, "Processed your request", 23, 0);
					nArrClient[nIndex] = 0;
					//break;
				}
			}
		}

	}
}
void processnewrequest()
{
	if (FD_ISSET(nSocket, &fr))
	{
		int nLen = sizeof(struct sockaddr);
		int nClientSocket = accept(nSocket, NULL, &nLen);
		if (nClientSocket > 0)
		{
			//put it into fd set;
			int nIndex;
			for (nIndex = 0; nIndex < 5; nIndex++)
			{
				if (nIndex == 0)
				{
					nArrClient[nIndex] = nClientSocket;
					send(nClientSocket, "Got the connection done successfull", 36, 0);
					break;
				}
			}
			if (nIndex == 5)
			{
				cout << "\nNo space for a new connection\n";
			}
		}
	}
	else
	{
		for (int nIndex = 0; nIndex < 5; nIndex++)
		{
			if (FD_ISSET(nArrClient[nIndex], &fr))
			{
				//Got the new message from the client
				ProcessnewMessage(nArrClient[nIndex]);

			}
		}
	}
}

int main()
{
	int nRet = 0;
	struct sockaddr_in srv;
	WSADATA ws;

	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0);

	nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0)
	{
		cout << "socket not opened\n";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		cout << "socket opened\n" << nSocket << endl;

	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY;
	memset(&(srv.sin_zero), 0, 8);

	int nOptval = 0;
	int nOptlen = sizeof(nOptval);
	nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOptval, nOptlen);
	if (!nRet)
	{
		cout << endl << "setsockopt Call successful\n";
	}
	else
	{
		cout << endl << "setsockopt Call Faild\n";
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
	if (nRet < 0)
	{
		cout << "Fail to bind to local port\n";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "Successfully bind to local port\n";
	}
	nRet = listen(nSocket, 5);
	if (nRet < 0)
	{
		cout << "Fail to start listen to local port\n";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		cout << "started listening to the local port\n";

	nMaxfd = nSocket;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;


	while (1) {
		FD_ZERO(&fr);
		FD_ZERO(&fw);
		FD_ZERO(&fe);

		FD_SET(nSocket, &fr);
		FD_SET(nSocket, &fe);

		for (int nIndex = 0; nIndex < 5; nIndex++)
		{
			if (nArrClient[nIndex] != 0)
			{
				FD_SET(nArrClient[nIndex], &fr);
				FD_SET(nArrClient[nIndex], &fe);

			}
		}
		nRet = select(nMaxfd + 1, &fr, &fw, &fe, &tv);
		if (nRet > 0)
		{

			processnewrequest();

		}

	}


}