#ifndef WINDOWSUTILS
#define WINDOWSUTILS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <sstream>


bool InitMainWindow(int width, int height, HINSTANCE hInstance, const std::string& appName, WNDPROC mssgHandler);
int Run(void(*pUpdate)(float), void(*pRender)(float));
void OnResize_internal();
LRESULT DefaultMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE GetAppInst();
HWND GetMainWnd();
void GetClientExtents(int& width, int& height);



#endif
