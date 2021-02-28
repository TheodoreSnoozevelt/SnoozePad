#include <windows.h>
#include <xinput.h>

struct GamepadState {
	bool aButtonPressed;
	bool bButtonPressed;
	bool xButtonPressed;
	bool yButtonPressed;
	bool leftShoulderButtonPressed;
	bool rightShoulderButtonPressed;
	bool leftThumbPressed;
	bool rightThumbPressed;
	bool upPressed;
	bool rightPressed;
	bool downPressed;
	bool leftPressed;
	float leftThumbX;
	float leftThumbY;
	float rightThumbX;
	float rightThumbY;
	float leftTrigger;
	float rightTrigger;
};


class Gamepad {
	private:
	XINPUT_STATE state;
	XINPUT_VIBRATION vibration;
	
	
	public:
	Gamepad() {
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	}
	
	bool readGamepadState(GamepadState* st) {
		if (XInputGetState(0, &state) != ERROR_SUCCESS)
			return false;
		st->aButtonPressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);
		st->bButtonPressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);
		st->xButtonPressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0);
		st->yButtonPressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0);
		st->leftPressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0);
		st->rightPressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0);
		st->upPressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0);
		st->downPressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0);
		st->leftShoulderButtonPressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0);
		st->rightShoulderButtonPressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);
		st->leftThumbPressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0);
		st->rightThumbPressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0);
		st->leftThumbX = fmaxf(-1, (float) state.Gamepad.sThumbLX / 32767);
		st->leftThumbY = fmaxf(-1, (float) state.Gamepad.sThumbLY / 32767);
		st->rightThumbX = fmaxf(-1, (float) state.Gamepad.sThumbRX / 32767);
		st->rightThumbY = fmaxf(-1, (float) state.Gamepad.sThumbRY / 32767);
		st->leftTrigger = (float) state.Gamepad.bLeftTrigger / 255;
		st->rightTrigger = (float) state.Gamepad.bRightTrigger / 255;
		return true;
	}
};