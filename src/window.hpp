#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <windows.h>
#include "windowclass.hpp"
#include "rect.hpp"

class Window
{
public:
    Window( const char* aTitle, const char* aWindowClass, const Rect& aSize );
    ~Window();
protected:
    void Resize( int aW, int aH );
    virtual void OnPaint() = 0;
    virtual void OnDragEnd() = 0;
    virtual LRESULT CALLBACK WndProc( HWND aHwnd, UINT aMsg, WPARAM aWParam, LPARAM aLParam );
    static LRESULT CALLBACK StaticWndProc( HWND aHwnd, UINT aMsg, WPARAM aWParam, LPARAM aLParam );
protected:
    WindowClass iWindowClass;
    HWND iHwnd;
    bool iDragWindow;
    int iXOff, iYOff;
};

#endif // _WINDOW_HPP_

// End of file
