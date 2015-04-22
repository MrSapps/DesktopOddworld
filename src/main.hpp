#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <windows.h>
#include "bitmap.hpp"

const static char* KWindowClass = "Sligs";
const static char* KWindowTitle = "Sligs";

// Prototypes
ATOM				MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void CreateWindows();
void AddTrayIcon( HWND aHwnd );

#endif // _MAIN_HPP_

// End of file
