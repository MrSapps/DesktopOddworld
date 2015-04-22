#ifndef _FLOATING_WINDOW_HPP_
#define _FLOATING_WINDOW_HPP_

#include <windows.h>
#include "window.hpp"
#include "GDIBmp.hpp"
#include "Anim.hpp"

class WndInfo
{
public:
    WndInfo(HWND aHwnd)
        : iHwnd( aHwnd )
    {
        iRect.GetWindowRect( iHwnd );
        Title();
        Class();
    }

    std::string Title()
    {
        char buffer[1024] = { 0 };
        int chars = GetWindowText( iHwnd, buffer, sizeof(buffer) );
        iTitle = buffer;
        return iTitle;
    }

    std::string Class()
    {
        char buffer[1024] = { 0 };
        int chars = GetClassName( iHwnd, buffer, sizeof(buffer) );
        iClassName = buffer;
        return iClassName;
    }

    std::string DebugString()
    {
        std::string output;
        output = "Class name(";
        output += Class();
        output += ") Window title(";
        output += Title();
        output += ")\n";
        return output;
    }

    HWND iHwnd;
    Rect iRect;
    std::string iTitle;
    std::string iClassName;
};

class FloatingWindow : public Window
{
public:
    enum Direction
    {
        EDown,
        ELeft,
        ERight,
    };
    enum TXState
    {
        EMoveLeft,
        EMoveRight,
        ENone,
    };

    enum TYState
    {
        EFalling,
        EHitFloor,
        ENormal,
    };
    FloatingWindow( const char* aTitle, const char* aWindowClass, const Rect& aSize );
    ~FloatingWindow();
    static void GetWindows();
    void Tick();
protected:
    static BOOL CALLBACK StaticEnumWindowsProc( HWND aHwnd, LPARAM aLParam );
    void KeepRectInWindow( HWND aWindow, Rect& aRect );
    void KeepOnWindowTops( Rect& aRect, Direction aDirection );
    bool IsWindowInFront( HWND aWindow, HWND aMightBeInFrontWindow );
    bool IsWindowFullyInWindow( HWND aFirst, HWND aSecond );
    virtual void OnPaint();
    virtual void OnDragEnd();
private:
    int iSpeed;
    static std::vector< WndInfo > iSysWnds;
    TXState iXState;
    TYState iYState;
    HWND iWalkingOnHwnd;
    HWND iAllowWalkThroughX;
    HWND iDrageedOnto;
    RGDIBmp iFrames;
    Anim* iAnim;
};

#endif // _FLOATING_WINDOW_HPP_

// End of file
