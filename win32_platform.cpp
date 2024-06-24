#include <windows.h>
#include "platform_common.cpp"

bool running = true;

struct Render_State {

	int height, width;
	void* memory;
	BITMAPINFO bitmapinfo;
};

Render_State render_state;

#include "renderer.cpp"
#include "game.cpp"

LRESULT CALLBACK window_callback(
	_In_ HWND hwnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	LRESULT result = 0;
	switch (uMsg)
	{
		case WM_CLOSE:
		case WM_DESTROY: {
			running = false;
		} break;
		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
			render_state.height = rect.bottom - rect.top;
			render_state.width = rect.right - rect.left;

			int buffer_size = render_state.width * render_state.height * sizeof(unsigned int);
			if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
			render_state.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			render_state.bitmapinfo.bmiHeader.biSize = sizeof(render_state.bitmapinfo.bmiHeader);
			render_state.bitmapinfo.bmiHeader.biWidth = render_state.width;
			render_state.bitmapinfo.bmiHeader.biHeight = render_state.height;
			render_state.bitmapinfo.bmiHeader.biPlanes = 1;
			render_state.bitmapinfo.bmiHeader.biBitCount = 32;
			render_state.bitmapinfo.bmiHeader.biCompression = BI_RGB;

		} break;
		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
			
	}
	return result;
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	//window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Game Window Class";
	window_class.lpfnWndProc = window_callback;

	//registering class
	RegisterClass(&window_class);

	//window creation :)
	HWND window = CreateWindow(window_class.lpszClassName, L"Ponging creation", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);

	HDC hdc = GetDC(window);

	Input input = {};

	float delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}


	while (running) {
		MSG message;

		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.buttons[i].changed = false;
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			switch (message.message) {
			case WM_KEYUP:
			case WM_KEYDOWN: {
				unsigned int vk_code = (unsigned int)message.wParam;
				bool is_down = ((message.lParam & (1 << 31)) == 0);

				switch (vk_code)
				{
					case VK_UP: {
						input.buttons[BUTTON_UP].is_down = is_down;
						input.buttons[BUTTON_UP].changed = true;
					} break;
					case VK_DOWN: {
						input.buttons[BUTTON_DOWN].is_down = is_down;
						input.buttons[BUTTON_DOWN].changed = true;
					} break;
					case VK_LEFT: {
						input.buttons[BUTTON_LEFT].is_down = is_down;
						input.buttons[BUTTON_LEFT].changed = true;
					} break;
					case VK_RIGHT: {
						input.buttons[BUTTON_RIGHT].is_down = is_down;
						input.buttons[BUTTON_RIGHT].changed = true;
					} break;
					case VK_SHIFT: {
						input.buttons[BUTTON_SHIFT].is_down = is_down;
						input.buttons[BUTTON_SHIFT].changed = true;
					} break;
					case VK_RETURN: {
						input.buttons[BUTTON_ENTER].is_down = is_down;
						input.buttons[BUTTON_ENTER].changed = true;
					} break;
					case 0x57: { 
						input.buttons[BUTTON_W].is_down = is_down;
						input.buttons[BUTTON_W].changed = true;
					} break;
					case 0x53: { 
						input.buttons[BUTTON_S].is_down = is_down;
						input.buttons[BUTTON_S].changed = true;
					} break;
				}
			}
				break;
				default: {
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
		}

		//simulation part
		simulate_game(&input, delta_time);


		//rendering
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		frame_begin_time = frame_end_time;

	}
	
}