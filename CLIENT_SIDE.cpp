#include<iostream>
#include<ctime>
#include<cstdlib>
#include<winsock.h>
#define PORT 7809
				using namespace std;
			time_t start, finish;
			int Packet[6];
			float RTT[6], RTT2, AveCRTT;

			int nClientSocket;
			struct sockaddr_in srv;
			int main()
			{
				int nRet = 0;
				WSADATA ws;
				fd_set fr, fw, fe;
				int nMaxfd;
				if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
				{
					cout << "not initilized\n";
					//exit(EXIT_FAILURE);
				}
				else
					cout << "initilized\n";

				int nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (nClientSocket < 0)
				{
					cout << "socket call failed\n";
					WSACleanup();
					exit(EXIT_FAILURE);
				}



				srv.sin_family = AF_INET;
				srv.sin_port = htons(PORT);
				srv.sin_addr.s_addr = inet_addr("192.168.1.79");
				//srv.sin_addr.s_addr = INADDR_ANY;
				memset(&srv.sin_zero, 0, 8);

				nRet = connect(nClientSocket, (struct sockaddr*)&srv, sizeof(srv));
				if (nRet < 0)
				{
					cout << endl << "connection failed\n";
					WSACleanup();
					exit(EXIT_FAILURE);
				}
				else
					cout << endl << "connected to the server\n";
				char buff[256] = { 0, };
				recv(nClientSocket, buff, 256, 0);
				cout << "\n Press any key to see the message received from server\n";
				getchar();
				cout << endl << buff;
				//packet();

				//============================================================================
				int sec = 0;
				int totalsend = 6;
				int packrec = 0;
				int j = 0;
				while (j == 0) {
					for (int i = 0; i < 6; i++)
					{
						cout << "\nSend Packet: " << i + 1 << "\n";
						fgets(buff, 256, stdin);
						getchar();
						send(nClientSocket, buff, 256, 0);
						time(&start);
						int nret = recv(nClientSocket, buff, 256, 0);
						if (nret < 0)
						{
							//packrec--;
							cout << "\nPacket Lost\n";
							break;
						}
						else
						{
							packrec++;
							cout << endl;
							cout << endl << "Message from the Server: " << buff << endl;
							time(&finish);
							RTT[i] = difftime(finish, start);


						}
						cout << "\nRTT[" << i << "]: " << RTT[i] / 1000 << endl;
						//cout << "\nRTT2: " << RTT[2] << endl;
					}
					cout << "\n\n=======================================================================================\n";
					cout << "\n=======================================================================================\n";
					cout << "\n\n\tPACKET LOSS TESTING TOOL\n";
					cout << "\n=======================================================================================\n";
					cout << "\n=======================================================================================\n\n";
					cout << "\nRTT1: " << RTT[1] / 1000 << endl;
					cout << "\nRTT2: " << RTT[2] / 1000 << endl;
					AveCRTT = ((RTT[1] / 1000) + (RTT[2] / 1000)) / 2;
					cout << "\nAverage RTT: " << AveCRTT << endl;
					cout << "\nTotal Packet send: " << totalsend << endl;
					cout << "\nPacket Received: " << packrec << endl;
					int TotalLoss = totalsend - packrec;
					cout << "\nTotal Packet Lost: " << TotalLoss << endl;
					j++;
				}

			}