//#include <winsock2.h> // ����2 ���� ���
//#include <ws2tcpip.h> // ����2 Ȯ�� ���
#include "game.h"
#include "client.h"
#include <sstream>

#define FPS 144
#define SERVERPORT 9000
#define BUFSIZE    1024

using namespace std;
//char* SERVERIP = (char*)"127.0.0.1";
Game game;

int SDL_main(int argc, char* argv[])
{

	// ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	cout << "Start" << endl;
	
	int startTime, endTime;

	while (!game.done) 
	{
		startTime = clock();
		game.update();

		if (game.curr_state == 0) {
			if (game.connect_server && !game.server_connected) {
				string str(game.Port);
				int i = 0;
				stringstream ssInt(str);
				ssInt >> i;
				
				// connect()
				struct sockaddr_in serveraddr;
				memset(&serveraddr, 0, sizeof(serveraddr));
				serveraddr.sin_family = AF_INET;
				inet_pton(AF_INET, game.IPAdress, &serveraddr.sin_addr);
				serveraddr.sin_port = htons(i);
				retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
				if (retval == SOCKET_ERROR) err_quit("connect()");
				
				game.server_connected = true; //�ѹ��� connect�ϰԲ�
				cout << "������ �����" << endl;
			}
			if (game.find_match) {
				find_match(game.find_match);
				game.find_match = false; //true�ΰ� �ް� false�� �ٲ���
				cout << "find_match ����" << endl;
			}
		}
		else {
			
		}
		
		Sleep(1000 / FPS - game.delayTime);
		endTime = clock();
		game.delayTime = (endTime - startTime) / 1000.f;
	}

	//����
	cout << "End\n" << endl;
	return 0;
}
