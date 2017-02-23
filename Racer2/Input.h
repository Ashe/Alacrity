#ifndef INPUT_H
#define INPUT_H

#include <Xinput.h>
#include <string>
#include <WinUser.h>


#include "D3D.h"
#include "SimpleMath.h"

/*ideally we'd create a complete set of our key codes
completely independently of windows, but as I'm lazy
I'll just add a few extra codes to the VK_ list windows
provides in "winuser.h", to make it more consistent.
*/
#define VK_NUMPAD_ENTER 0xF0		//these three are very naughty
#define VK_LALT			0xF1		//they are actually some oem
#define VK_RALT			0xF2		//keys ATTN/FINISH/COPY - but lets pinch them :)


#define VK_0	0x30
#define VK_1	0x31
#define VK_2	0x32
#define VK_3	0x33
#define VK_4	0x34
#define VK_5	0x35
#define VK_6	0x36
#define VK_7	0x37
#define VK_8	0x38
#define VK_9	0x39

#define VK_A	0x41
#define VK_B	0x42
#define VK_C	0x43
#define VK_D	0x44
#define VK_E	0x45
#define VK_F	0x46
#define VK_G	0x47
#define VK_H	0x48
#define VK_I	0x49
#define VK_J	0x4a
#define VK_K	0x4b
#define VK_L	0x4c
#define VK_M	0x4d
#define VK_N	0x4e
#define VK_O	0x4f
#define VK_P	0x50
#define VK_Q	0x51
#define VK_R	0x52
#define VK_S	0x53
#define VK_T	0x54
#define VK_U	0x55
#define VK_V	0x56
#define VK_W	0x57
#define VK_X	0x58
#define VK_Y	0x59
#define VK_Z	0x5a


class MouseAndKeys
{
public:
	MouseAndKeys()
	{
		Reset();

	}
	void Reset()
	{
		ZeroMemory(mInBuffer, sizeof(mInBuffer));
		ZeroMemory(mKeyBuffer, sizeof(mKeyBuffer));
		mButtons[0] = mButtons[1] = mButtons[2] = false;
		mMouseScreen = mMouseMove = DirectX::SimpleMath::Vector2(0,0);
	}
	//start up the mouse system, call once only
	void Initialise(HWND hwnd, bool showMouse = true, bool confineMouse = false);
	//accessors
	void GetPressedKeyNames(std::wstring& mssg);
	bool IsPressed(unsigned short vkeyCode) const;
	void GetMouseNames(std::wstring& mssg);
	typedef enum { LBUTTON = 0, MBUTTON = 1, RBUTTON = 2 } ButtonT;
	bool GetMouseButton(ButtonT b) const {
		return mButtons[b];
	}
	DirectX::SimpleMath::Vector2 GetMousePos( bool absolute) const { 
		if (absolute)
			return mMouseScreen; 
		return mMouseMove;
	}
	//call this after game has finished using the mouse, probably at end of render
	void PostProcess();
	//convert a click on the screen into a ray in the world
	void MousePosToWorldRay(const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		const DirectX::SimpleMath::Vector3& camPos,
		DirectX::SimpleMath::Ray& ray);
	//for consistency we can reset mouse input after task switch (window<->fullscreen)
	void OnLost();
	void OnReset();

	//add to your windows message pump
	//rawInput comes from lparam after a WM_INPUT message
	void MessageEvent(HRAWINPUT rawInput);

private:
	//main window handle
	HWND mHwnd;
	enum { RAWBUFF_SIZE = 512, KEYBUFF_SIZE = 255, KMASK_IS_DOWN = 1, MAX_BUTTONS = 3 };
	//raw input buffer
	BYTE mInBuffer[RAWBUFF_SIZE];
	unsigned short mKeyBuffer[KEYBUFF_SIZE];  //virtual key codes, but only the standard ones (first 256)
	//support for three mouse buttons
	bool mButtons[MAX_BUTTONS];
	//track mouse position changes (2D)
	DirectX::SimpleMath::Vector2 mMouseScreen, mMouseMove;
	//if the mouse is confined, track the size of the window
	bool mConfineMouse;
	RECT mNewClip;           // new area for ClipCursor
	RECT mOldClip;        // previous area for ClipCursor

	void ProcessKeys(RAWINPUT* raw);
	void ProcessMouse(RAWINPUT* raw);
	void GetMousePosAbsolute(DirectX::SimpleMath::Vector2& pos);
};



/*
Simple input handler
Only supports the first controller detected, but easy to add support for more.
*/
class Gamepad
{
public:

	struct State
	{
		int port = -1;
		float leftStickX=0;
		float leftStickY=0;
		float rightStickX=0;
		float rightStickY=0;
		float leftTrigger=0;
		float rightTrigger=0;
		float deadzoneX=0.1f;
		float deadzoneY=0.1f;
		XINPUT_STATE state;
	};

	void SetDeadZone(int idx, float x, float y) {
		assert(idx >= 0 && idx < XUSER_MAX_COUNT);
		mPads[idx].deadzoneX = x;
		mPads[idx].deadzoneY = y;
	}
	const State& GetState(int idx) const {
		assert(idx >= 0 && idx < XUSER_MAX_COUNT);
		return mPads[idx];
	}	bool Refresh();
	void Initialise();
	bool IsConnected(int idx) {
		assert(idx >= 0 && idx < XUSER_MAX_COUNT);
		return mPads[idx].port != -1;
	}
	void Update();

	/*
	XINPUT_GAMEPAD_DPAD_UP 
	XINPUT_GAMEPAD_DPAD_DOWN
	XINPUT_GAMEPAD_DPAD_LEFT
	XINPUT_GAMEPAD_DPAD_RIGHT
	XINPUT_GAMEPAD_START
	XINPUT_GAMEPAD_BACK
	XINPUT_GAMEPAD_LEFT_THUMB
	XINPUT_GAMEPAD_RIGHT_THUMB
	XINPUT_GAMEPAD_LEFT_SHOULDER
	XINPUT_GAMEPAD_RIGHT_SHOULDER
	XINPUT_GAMEPAD_A 
	XINPUT_GAMEPAD_B 
	XINPUT_GAMEPAD_X 
	XINPUT_GAMEPAD_Y 
	*/
	bool IsPressed(int idx, WORD);

	void GetAnalogueNames(int idx, std::wstring& mssg);
	void GetDigitalNames(int idx, std::wstring& mssg);
private:
	State mPads[XUSER_MAX_COUNT];

};


#endif