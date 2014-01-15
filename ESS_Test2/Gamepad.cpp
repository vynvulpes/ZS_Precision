#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include "Xinput.h"
#include "Gamepad.h"



using namespace std;

int main()
{
	bool motor = true;
	// Options that I want to implement:
	// 1) Motor strength
	// 2) Ability to toggle motors on/off
	// 3) Overlay to Project64
	cout << "######################################################" << endl;
	cout << "## ZSPrecision alpha_0_0_7 Jhonz                    ##" << endl;
	cout << "## Built for 360 controllers                        ##" << endl;
	cout << "## XInput background Katy Coe. Modified by Jhonz    ##" << endl;
	cout << "## Freeware for all. Enjoy!                         ##" << endl;
	cout << "##	                                            ##" << endl;
	cout << "## ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	cout << "[L3]: Toggle Motors on / off                     ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	cout << "##" << endl;
	cout << "######################################################" << endl;
	cout << "\n";


	Gamepad gamepad;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);

	bool wasConnected = true;

	while (true)
	{
		Sleep(500);

		if (!gamepad.Refresh())
		{
			if (wasConnected)
			{
				wasConnected = false;

				cout << "\rPlease connect an Xbox 360 controller." << endl;
			}
		}
		else
		{
			if (!wasConnected)
			{
				wasConnected = true;

				cout << "Controller connected on port " << gamepad.GetPort() << endl;
			}
			if (gamepad.leftStickX < 0.45 && gamepad.leftStickX > 0.28)
			{
				cout << "\rESS POSITION RIGHT" << flush;
				Sleep(100);
				if(motor == true)
				{
					gamepad.Vibrate(0, 12000);
				} 
			}
			if (gamepad.leftStickX >0.45 || gamepad.leftStickX >= 0 && gamepad.leftStickX < 0.28)
			{
				cout << "\r";
				cout << "                         " << flush;
				gamepad.Vibrate(0, 0);
			}

			if (gamepad.leftStickX > -0.45 && gamepad.leftStickX < -0.28)
			{
				cout << "\rESS POSITION LEFT" << flush;
				Sleep(100);
				if (motor == true)
				{
					gamepad.Vibrate(10000, 0);
				}
			}
			if (gamepad.leftStickX <-0.45 || gamepad.leftStickX <= 0 && gamepad.leftStickX > -0.28)
			{
				cout << "\r";
				cout << "                         " << flush;
				gamepad.Vibrate(0, 0);
			}
			if (gamepad.IsPressed(XINPUT_GAMEPAD_LEFT_THUMB))
			{
				motor = !motor;
				if (motor == true)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					cout << "\rMotor is on" << flush;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
				}
				else{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					cout << "\rMotor is off" << flush;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
				}
			}

		}

	}
}