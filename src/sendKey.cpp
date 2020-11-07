#include "sendKey.h"

void sendKey(char keyCode, char lastKey)
{
	if (lastKey != keyCode)
	{
		KEYBDINPUT keyInputPress;
		keyInputPress.wVk = keyCode;
		keyInputPress.time = 0;
		keyInputPress.wScan = 0;
		keyInputPress.dwExtraInfo = 0;
		keyInputPress.dwFlags = KEYEVENTF_EXTENDEDKEY | 0;

		KEYBDINPUT keyInputRelease;
		keyInputRelease.wVk = lastKey;
		keyInputRelease.time = 0;
		keyInputRelease.wScan = 0;
		keyInputRelease.dwExtraInfo = 0;
		keyInputRelease.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;

		INPUT input[2];
		input[0].type = INPUT_KEYBOARD;
		input[0].ki = keyInputPress;
		input[1].type = INPUT_KEYBOARD;
		input[1].ki = keyInputRelease;
		SendInput(2, input, sizeof(INPUT));
	}
}