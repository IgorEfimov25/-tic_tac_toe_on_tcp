
#define TX_SAFE_MODE           
#include "GlobalStructs.h"
#include "GlobalSettings.h"
#include "TCPConnect.h"
#include <string>


int PageNum = 1; //1 Выбор сервер или клиент 2 экран ожидания 3 игра
int DrawPageNum = 0;
int ClientSettings = 1;
char symvol = 'X';
int turnnow = 1;
char Symvols[3] = "0X";
using namespace std;
string IP;
int Port;
bool IsEnd;
bool ConServ;


Button ScreenButtons[13];
Pole PlayGround[9];

char CheckWinner()
{

	const int winCombos[8][3] = {
		{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, 
		{0, 3, 6}, {1, 4, 7}, {2, 5, 8}, 
		{0, 4, 8}, {2, 4, 6}             
	};

	for (int i = 0; i < 8; i++)
	{
		char a = PlayGround[winCombos[i][0]].Symvol;
		char b = PlayGround[winCombos[i][1]].Symvol;
		char c = PlayGround[winCombos[i][2]].Symvol;

		if (a != '*' && a == b && b == c)
			return a; 
	}
	return '*'; 
}


void FirstPage() {
	ScreenButtons[0].CreateButton(200, 100, 360, 200, "Server", &ClientSettings, 1);

	ScreenButtons[1].CreateButton(400, 100, 560, 200, "Client", &ClientSettings, 2);
	ScreenButtons[2].CreateButton(400, 300, 560, 400, "Start", &PageNum, 2);

	ScreenButtons[3].CreateButton(200, 300, 360, 400, "Exit", &PageNum, 0);
}
void SecondPage() {


	LoadSettings(ClientSettings, IP, Port);
	if (ClientSettings == 1) StartServer(Port);
	if (ClientSettings == 2) ConnectToServer(IP.c_str(), Port);
	ScreenButtons[0].CreateButton(400, 100, 560, 200, "Start", &PageNum, 3);

}
void ThirdPage() 
{

	PlayGround[0].CreatePole(200, 200, 300, 300, 0);
	PlayGround[1].CreatePole(300, 200, 400, 300, 1);
	PlayGround[2].CreatePole(400, 200, 500, 300, 2);
	PlayGround[3].CreatePole(200, 300, 300, 400, 3);
	PlayGround[4].CreatePole(300, 300, 400, 400, 4);
	PlayGround[5].CreatePole(400, 300, 500, 400, 5);
	PlayGround[6].CreatePole(200, 400, 300, 500, 6);
	PlayGround[7].CreatePole(300, 400, 400, 500, 7);
	PlayGround[8].CreatePole(400, 400, 500, 500, 8);
	ScreenButtons[0].CreateButton(600, 100, 700, 200, "Exit", &PageNum, 0);

	for (int i = 0; i < 9; i++)
	{
			PlayGround[i].PaintBorder();
	}
}
int main()
{
	СreateWindow();
	while(!InitNetworking());
	char vaaar[30] = "aafsaads";




	while (true)
	{
		if (PageNum == 0) {
			break;
		}
		if (DrawPageNum != PageNum) {
			Sleep(100);
			for (int i = 0; i < 13; i++)
			{
				ScreenButtons[i].Clear();
			}
			for (int i = 0; i < 9; i++)
			{
				PlayGround[i].ClearPole();
			}
			WindowClear();
			switch (PageNum)
			{
			case(1):
				FirstPage();
				break;
			case(2):
				SecondPage();
				break;
			case(3):
				ThirdPage();
				break;
			}
			for (int i = 0; i < 13; i++)
			{
				ScreenButtons[i].PaintButton();

			}
			DrawPageNum = PageNum;

		}
		int x, y;
		int key = GetMouse(&x, &y);

		for (int i = 0; i < 13; i++) 
		{
			
			
			Button &But = ScreenButtons[i];
			if (!But.IsActive) break;
			if ((x > But.coords[0] && x < But.coords[2]) && (y > But.coords[1] && y < But.coords[3])) 
			{
				//DebugFunc("ada");
				But.OnCursor();
				if (key == 1) {
					But.Click();
					break;
				}
				
			}
			else 
			{
				But.OutCursor();
			}


		}
		if(!IsEnd)
		for (int i = 0; i < 9; i++) 
		{
			
			
			Pole &Pole = PlayGround[i];
			if (!Pole.IsActive) break;
			if ((x > Pole.coords[0] && x < Pole.coords[2]) && (y > Pole.coords[1] && y < Pole.coords[3]))
			{
				Pole.OnCursor(symvol);
				if (key == 1 && ClientSettings-1 == turnnow) {
					int hod = Pole.SwapSymvol(symvol);
					if (hod == 1)
					{
						turnnow = (turnnow + 1) % 2;
						
						char ind = Pole.index;
						char Message[4];
						Message[0] = 'S';
						Message[1] = ind + '0';
						Message[2] = symvol;
						Message[3] = '\n';
						SendData(Message, 4);
						symvol = Symvols[turnnow];
						if (CheckWinner() != '*')
						{
							char Win = CheckWinner();
							char text[30] = "* Win";
							text[0] = Win;
							DrawTexts(10, 40, 26, text);
							IsEnd = true;

						}
					}
				}
				
			}
			else 
			{
				Pole.OutCursor();
			}


		}
		switch (PageNum)
		{
		case(1):
			break;
		case(2):
			if (ClientSettings == 1)
			{
				char buffer[30] = "Redy";
				
				if (!ConServ && !AcceptConnection()) break;
				ConServ = true;
				
				
				SendData(buffer, 30);
				char buffers[30] = "";
				ReceiveData(buffers, 30);

				if (strcmp(buffers, "I too") == 0)
				{
					PageNum = 3;
				}
			}
			if (ClientSettings == 2)
			{
				if (IsConnected())
				{
					char buffer[30];
					ReceiveData(buffer, 30);
					//DebugFunc(buffer);
					if (strcmp(buffer, "Redy") == 0) 
					{
						char buffers[30] = "I too";
						SendData(buffers, 30);
						PageNum = 3;
					}
				

				}

			}
			break;
		case(3):
			if (ClientSettings - 1 != turnnow) 
			{
				char buffer[4];
				if(ReceiveData(buffer, 4) == -1) break;
				
				if (buffer[0] == 'S')
				{
					//DebugFunc(buffer);
					PlayGround[buffer[1] - '0'].SwapSymvol(buffer[2]);
				    turnnow = (turnnow + 1) % 2;
					symvol = Symvols[turnnow];
				}
				if ( CheckWinner() != '*') 
				{
					char Win = CheckWinner();
					char text[30] = "* Win";
					text[0] = Win;
					DrawTexts(10, 40, 26, text);
					IsEnd = true;

				}
				
			}
			break;
		}

	}

	
	PostQuitMessage(0);

}