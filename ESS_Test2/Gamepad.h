/* XInput support */
/* (c) Katy Coe (sole proprietor) 2013 */
/* www.djkaty.com */

#include <Windows.h>
#include <Xinput.h>
#include <algorithm>
#include <boost/assign.hpp>
#include <string>
#include <map>

using namespace boost::assign;
using std::string;

#pragma comment(lib, "Xinput9_1_0.lib")

// The gamepad support class
class Gamepad
{
public:
	// =========================================================================
	// Public usable sub-types
	// =========================================================================

	// Names for each axis of movement for each analog item
	enum AnalogButtons {
		LeftStickLeft,
		LeftStickRight,
		LeftStickUp,
		LeftStickDown,
		RightStickLeft,
		RightStickRight,
		RightStickUp,
		RightStickDown,
		LeftTrigger,
		RightTrigger,
		EndOfButtons
	};

	// Analog keyboard mappings have a percentage threshold and a target keyboard key
	struct AnalogMapping {
		float threshold;
		int key;
	};

private:
	// The port on which the controller is connected (0-3)
	int cId;

	// The last retrieved state of the controller
	XINPUT_STATE state;

	// The window to send keyboard events to (or NULL for none)
	HWND targetWindow;

	// The X-axis analog stick deadzone (0-1)
	float deadzoneX;

	// The Y-axis analog stick deadzone (0-1)
	float deadzoneY;

	// Mapping of controller buttons to keys
	std::map<WORD, int> keyMap;

	// Mapping of analog controller items to keys
	std::map<AnalogButtons, AnalogMapping> analogMap;

	// Repeat rate of generated key events from controller buttons
	std::map<WORD, unsigned int> repeatMs;

	// Repeat rate of generated key events from analog controller items
	std::map<AnalogButtons, unsigned int> analogRepeatMs;

	// The GetTickCount() of when each button was last pressed
	std::map<WORD, DWORD> lastPress;

	// The GetTickCount() of when each analog item was pressed (passed the threshold)
	std::map<AnalogButtons, DWORD> analogLastPress;

	// Configure the controller button names. Internal use only.
	void setButtons()
	{
		std::map<WORD, string> bn = map_list_of
			(XINPUT_GAMEPAD_A, "A")
			(XINPUT_GAMEPAD_B, "B")
			(XINPUT_GAMEPAD_X, "X")
			(XINPUT_GAMEPAD_Y, "Y")
			(XINPUT_GAMEPAD_DPAD_LEFT, "Left")
			(XINPUT_GAMEPAD_DPAD_RIGHT, "Right")
			(XINPUT_GAMEPAD_DPAD_UP, "Up")
			(XINPUT_GAMEPAD_DPAD_DOWN, "Down")
			(XINPUT_GAMEPAD_LEFT_SHOULDER, "LB")
			(XINPUT_GAMEPAD_RIGHT_SHOULDER, "RB")
			(XINPUT_GAMEPAD_BACK, "Back")
			(XINPUT_GAMEPAD_START, "Start")
			(XINPUT_GAMEPAD_LEFT_THUMB, "LS")
			(XINPUT_GAMEPAD_RIGHT_THUMB, "RS");

		Buttons.insert(bn.begin(), bn.end());
	}

	// The previous state of the controller
	XINPUT_STATE previous;

	// The previous state of the analog sticks and triggers
	float prevLeftStickX;
	float prevLeftStickY;
	float prevRightStickX;
	float prevRightStickY;
	float prevLeftTrigger;
	float prevRightTrigger;

	// Internal use only
	void sendKeysOnThreshold(AnalogButtons, float, float, float, int);

public:
	// =========================================================================
	// Public methods and properties
	// =========================================================================

	// Enable gamepad support
	Gamepad() : deadzoneX(0.05f), deadzoneY(0.02f), targetWindow(NULL) { setButtons(); SetRepeatIntervalMsAll(0); }

	// Enable gamepad support supplying default X and Y-axis deadzones
	Gamepad(float dzX, float dzY) : deadzoneX(dzX), deadzoneY(dzY), targetWindow(NULL) { setButtons(); SetRepeatIntervalMsAll(0); }

	// A map of XINPUT_GAMEPAD_* button IDs to string button names
	std::map<WORD, string> Buttons;

	// The current state of the analog sticks and triggers (movement amount from 0-1)
	float leftStickX;
	float leftStickY;
	float rightStickX;
	float rightStickY;
	float leftTrigger;
	float rightTrigger;

	// Get the port on which the active controller is plugged in (1-4)
	int  GetPort();

	// Get the current state of the controller (not normally needed)
	XINPUT_GAMEPAD *GetState();

	// Try to establish a connection with the controller (returns true if succeeded)
	bool CheckConnection();

	// Refresh the state of the controller. Call once per frame (calls CheckConnection())
	bool Refresh();

	// Returns true if the specified XINPUT_GAMEPAD_* button is pressed
	bool IsPressed(WORD);

	// Set the X and Y-axis analog stick deadzones
	void SetDeadzone(float, float);

	// Set the target window to receive key events and enable them
	void SetWindow(HWND);

	// Add a key translation mapping from XINPUT_GAMEPAD_* to a virtual key code (VK_*)
	void AddKeyMapping(WORD, int);

	// Remove a key translation mapping from XINPUT_GAMEPAD_* button
	void RemoveKeyMappingByButton(WORD);

	// Remove a key translation mapping from a virtual key code (VK_*)
	void RemoveKeyMapping(int);

	// Add a key translation mapping from an analog item moved more than the specified threshold to a virtual key code (VK_*)
	void AddAnalogKeyMapping(AnalogButtons, float, int);

	// Remove a key translation mapping from an analog item
	void RemoveAnalogKeyMapping(AnalogButtons);

	// Remove all digital and analog key translation mappings
	void ClearMappings();

	// Set the global keyboard repeat interval for all buttons and analog items on the controller - overwrites any previous settings
	void SetRepeatIntervalMsAll(unsigned int);

	// Set the keyboard repeat interval for the specified XINPUT_GAMEPAD_* button in milliseconds
	void SetRepeatIntervalMs(WORD, unsigned int);

	// Set the keyboard repeat interval for the specified analog item in milliseconds
	void SetAnalogRepeatIntervalMs(AnalogButtons, unsigned int);

	void Vibrate(int leftVal = 0, int rightVal = 0);
};