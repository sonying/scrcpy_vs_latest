#include "scrcpy.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libavformat/avformat.h>
//#include <sys/time.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_syswm.h"

#include "config.h"
#include "resource1.h"

#define IDM_VER                     32777
#define IDM_PIXEL_DISP                     32778
#define IDM_CHK_NEW                      32779
#define IDM_TIPS                       32780
#define IDM_REMOVE_BLACK                       32781
#define IDM_SITE                       32782
#define IDM_FULL                  32783
#define IDM_EXIT                        32784
#define IDM_SEP                         32785
 
#define ENGLISH_VER 1

static NOTIFYICONDATA icon;
static SDL_SysWMinfo info;
static SDL_Window *window;

int add_tray_icon()
{
	window = SDL_CreateWindow(L"Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1, 1, SDL_WINDOW_HIDDEN);
	SDL_VERSION(&info.version);

	if (SDL_GetWindowWMInfo(window, &info))
	{
		icon.uCallbackMessage = WM_USER + 1;
		icon.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		icon.hIcon = LoadIcon(info.info.win.hinstance, MAKEINTRESOURCE(IDI_ICON1)); // LoadIcon(NULL, IDI_INFORMATION);
		icon.cbSize = sizeof(icon);
		icon.hWnd = info.info.win.window;
		//strcpy_s(icon.szTip, L"Test tip");

		WCHAR b[128];
		//strcpy(b, L"Blueberry Airplay Receiver");
#if ENGLISH_VER
		wcscpy_s(icon.szTip, 128, L"B: 在iPhone/iPad/Mac 搜索 Blueberry 开始投屏");
#else
		wcscpy_s(icon.szTip, 128,  L"Blueberry cast: Search Blueberry on iPhone/iPad/Mac to start mirroring");
#endif
		//b[128 - 1] = '\0';
		//strcpy(icon.szTip, (char *)b );
		 
		bool success = Shell_NotifyIcon(NIM_ADD, &icon);
	}
}

int del_tray_icon()
{
	Shell_NotifyIcon(NIM_DELETE, &icon);
}

int add_menu(HWND  hWnd)
{
	HMENU hPopMenu;
	int bDisable = FALSE;
	POINT lpClickPoint;

	UINT uFlag = MF_BYPOSITION | MF_STRING;
	GetCursorPos(&lpClickPoint);
	hPopMenu = CreatePopupMenu();

#if ENGLISH_VER
	 
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_SITE, L"Custom Development");
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_CHK_NEW, L"Check for updates");
	
	InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDM_TIPS, L"Instructions "); // Test 2
 
	
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_SEPARATOR, IDM_SEP, L"SEP" );

	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_PIXEL_DISP, L"1:1 Display Ctrl+g");
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_FULL, L"Full screen Ctrl+f" );
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_REMOVE_BLACK, L"Remove black border Ctrl+x");

	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_SEPARATOR, IDM_SEP, L"SEP");
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_EXIT, L"Exit" );
#else
	//InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_VER, L"定制开发");
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_SITE, L"定制开发");
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_CHK_NEW, L"检查更新");

	InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDM_TIPS, L"使用说明"); // Test 2


	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_SEPARATOR, IDM_SEP, L"SEP");

	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_PIXEL_DISP, L"1:1显示 Ctrl+g");
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_FULL, L"全屏 Ctrl+f");
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_REMOVE_BLACK, L"去黑边 Ctrl+x");

	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_SEPARATOR, IDM_SEP, L"SEP");
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_EXIT, L"退出");
#endif

	SetForegroundWindow(hWnd);
	TrackPopupMenu(hPopMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN, lpClickPoint.x, lpClickPoint.y, 0, hWnd, NULL);
	return TRUE;

}

int
handle_tray_event(SDL_Event *event, struct input_manager *im) {
	//MessageBox(NULL, _T("This is a test for menu Test 1"), _T("Test 1"), MB_OK);
	if (event->syswm.msg->msg.win.msg == WM_USER + 1)
	{
		if (LOWORD(event->syswm.msg->msg.win.lParam) == WM_LBUTTONDOWN || LOWORD(event->syswm.msg->msg.win.lParam) == WM_RBUTTONDOWN)
		{
			add_menu(event->syswm.msg->msg.win.hwnd);
			//SDL_ShowWindow(window);
			//SDL_RestoreWindow(window);
		}
	}

	if (event->syswm.msg->msg.win.msg == WM_COMMAND) {
		int wmId, wmEvent;
		wmId = LOWORD(event->syswm.msg->msg.win.wParam);
		wmEvent = HIWORD(event->syswm.msg->msg.win.wParam);
		fprintf(stderr, "wmEvent %d %d \n", wmId, wmEvent);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_CHK_NEW:
			ShellExecute(0, 0, L"http://deeprd.com", 0, 0, SW_SHOW);
			break;
		
		case IDM_SITE:
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			ShellExecute(0, 0, L"http://deeprd.com/#develop", 0, 0, SW_SHOW);
			break;
		case IDM_TIPS:
			ShellExecute(0, 0, L"https://gitee.com/halo-x/Airplay-SDK/blob/master/windows-receiver/README.md", 0, 0, SW_SHOW);
			break;

		
		case IDM_PIXEL_DISP:
			if (im->screen->has_frame)
				screen_resize_to_pixel_perfect(im->screen);
			break;
		case IDM_FULL:
			if (im->screen->has_frame)
				screen_switch_fullscreen(im->screen);
			break;
		case IDM_REMOVE_BLACK:
			//MessageBox(NULL, L"This is a test for menu发发发发 Test 1", L"沃尔沃若", MB_OK);
			if (im->screen->has_frame)
				screen_resize_to_fit(im->screen);
			break;

		case IDM_EXIT:
			//Shell_NotifyIcon(NIM_DELETE, &nidApp);
			//DestroyWindow(hWnd);
			return 1;
			break;

			//return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return 0;
}