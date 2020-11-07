#ifndef MOUSEINPUT_H
#define MOUSEINPUT_H

struct mouse_struct
{
	int x;
	int y;
	bool click = false;
};

extern mouse_struct mouse;

void mouseCallbackFunction(int mouseEvent, int x, int y, int flags, void* userdata);

#endif // !MOUSEINPUT_H
