#include "windowclass.hpp"
#include "resource.h"

WindowClass::WindowClass( const char* aClassName, WNDPROC aWindowProc )
{
    WNDCLASSEXA wcex  = { 0 };

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= aWindowProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= GetModuleHandle( NULL );
    ///	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SLIGS));
    wcex.hCursor		= LoadCursor(NULL, MAKEINTRESOURCE(IDI_ICON));
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    //	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SLIGS);
    wcex.lpszClassName	= aClassName;
    //	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    iAtom = RegisterClassExA( &wcex );
}

WindowClass::~WindowClass()
{

}

// End of file
