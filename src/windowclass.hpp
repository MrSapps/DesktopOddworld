#ifndef _WINDOW_CLASS_HPP_
#define _WINDOW_CLASS_HPP_

#include <windows.h>
#include <vector>

class WindowClass
{
public:
    WindowClass( const char* aClassName, WNDPROC aWindowProc );
    ~WindowClass();
private:
    ATOM iAtom;
    static std::vector< WindowClass* > iCurrentClasses;
};

#endif // _WINDOW_CLASS_HPP_

// End of file
