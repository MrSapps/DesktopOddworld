#ifndef _RECT_HPP_
#define _RECT_HPP_

#include <windows.h>

class Rect
{
public:
    int iW,iH,iX,iY;
    void GetWindowRect( HWND aHwnd );
private:
};

#endif // _RECT_HPP_

// End of file
