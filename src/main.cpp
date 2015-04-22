#include "main.hpp"
#include <stdexcept>
#include "windowclass.hpp"
#include "window.hpp"
#include "rect.hpp"
#include "floatingwindow.hpp"
#include "resource.h"


std::vector< FloatingWindow* > g_windows;
const int KTimerId = 0xFE1;
const int KTrayMsgId = (WM_APP + 100);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int       nCmdShow)
{

    // Register the main window class
    WindowClass wndClass( KWindowClass, WndProc );

    // Create all of the windows
    CreateWindows();

    // Create the "sink" window that handles the timer to update all other windows
    HWND hWnd = CreateWindowA( KWindowClass, KWindowTitle, 
         WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, // style
        CW_USEDEFAULT,  //x
        0,              // y
        1, // w
        1,  // h
        NULL, // Parent
        NULL, // hMenu
        hInstance, NULL);
    DWORD gle = GetLastError();

    // Update the new window
    UpdateWindow(hWnd);

    // Add the tray icon
    AddTrayIcon(hWnd);

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  
    }

    for ( unsigned int i=0; i<g_windows.size(); i++ )
    {
        delete g_windows[i];
    }

    return (int) msg.wParam;
}

void CreateWindows()
{
    for (unsigned int i = 0; i < 1; i++)
    {
        Rect rect;
        rect.iX = i * 145;
        rect.iY = 0;
        rect.iW = 121;
        rect.iH = 55;

        g_windows.push_back(new FloatingWindow("test", "test", rect));
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        {
            SetTimer ( hWnd, KTimerId, 1000 / 60, NULL ); // 1000 = 1 second
        }
        break;

    case WM_TIMER:
        {
            // Enum current windows to know the bounds, this is done once and shared between all window instances
            FloatingWindow::GetWindows(); 
            for ( unsigned int i=0; i<g_windows.size(); i++ )
            {
                // Update the windows pos, collision and animation
                g_windows[i]->Tick();
            }
        }
        break;

    case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;

    case KTrayMsgId:
    {
        UINT uID;
        UINT uMouseMsg;
        uID = (UINT)wParam;
        uMouseMsg = (UINT)lParam;
        switch (uID)
        {
            case KTrayMsgId:
            {
                if (uMouseMsg == WM_LBUTTONDOWN)
                {
                    PostQuitMessage(0);
                    return 1;
                }
                else if (uMouseMsg == WM_RBUTTONDOWN)
                {
                    CreateWindows();
                }
            }
            break;
        }

    }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


void AddTrayIcon( HWND aHwnd )
{
    NOTIFYICONDATA tnd = { 0 };
    tnd.cbSize = sizeof(NOTIFYICONDATA);
    tnd.hWnd = aHwnd;
    tnd.uID = KTrayMsgId;
    tnd.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
    tnd.uCallbackMessage = KTrayMsgId;
    tnd.hIcon = LoadIcon( GetModuleHandle( NULL ) ,MAKEINTRESOURCE( IDI_ICON ) );
    strcpy( tnd.szTip,"Sligs, left click to exit, right click to add more!" );
    Shell_NotifyIcon( NIM_ADD,&tnd );
}

// End of file
