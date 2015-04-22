#include "rect.hpp"

void Rect::GetWindowRect( HWND aHwnd )
{
    // Get the window rect
    RECT winRect;
    ::GetWindowRect( aHwnd, &winRect );

    // Calc the window w/h
    iH = winRect.bottom - winRect.top;
    iW = winRect.right - winRect.left;

    // Get the x/y pos
    iX = winRect.left;
    iY = winRect.top;
}

// End of file
