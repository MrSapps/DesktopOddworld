#include "window.hpp"

Window::Window( const char* aTitle, const char* aWindowClass, const Rect& aSize )
: iWindowClass( aWindowClass, StaticWndProc ), iDragWindow( false )
{
    iHwnd = CreateWindowExA( WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW, aWindowClass, aTitle, 
        WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS , // style
        aSize.iX,  //x
        aSize.iY,  // y
        aSize.iW,  // w
        aSize.iH,  // h
        NULL, // Parent
        NULL, // hMenu
        GetModuleHandle( NULL ), NULL );


    SetWindowLong( iHwnd, GWL_USERDATA, reinterpret_cast < LONG >( this ) );
    ShowWindow( iHwnd, SW_SHOW );
}

Window::~Window()
{
    if ( iHwnd )
    {
        DestroyWindow( iHwnd );
    }
}

void Window::Resize( int aW, int aH )
{
    Rect rect;
    rect.GetWindowRect( this->iHwnd );
    MoveWindow( iHwnd, rect.iX, rect.iY, aW, aH, TRUE );
}

LRESULT CALLBACK Window::WndProc( HWND aHwnd, UINT aMsg, WPARAM aWParam, LPARAM aLParam )
{
    switch ( aMsg )
    {
    case WM_CREATE:
        {
            HDC hdc = GetDC( NULL );

            //BLENDFUNCTION blend = { 0 };
           // blend.AlphaFormat

            UpdateLayeredWindow( aHwnd, hdc, NULL, NULL, NULL, // hdcSrc
                                 NULL, RGB( 0, 0, 0 ), NULL, ULW_COLORKEY );
        }
        break;

    case WM_PAINT:
        {
            OnPaint();
        }
        break;

    case WM_MOVE:
        {
            // TODO: Keep window in GetDesktopWindow bounds?
            int xPos = (int)(short) LOWORD(aLParam);   // horizontal position 
            int yPos = (int)(short) HIWORD(aLParam);   // vertical position 
                        
          //  char buffer[256];
          //  sprintf( buffer, "Moved to %d,%d",  xPos, yPos );
          //  SetWindowText( aHwnd, buffer );
        }
        break;

    // Right button closes the window
        /*
    case WM_RBUTTONDOWN:
        {
            PostQuitMessage(0);
            DestroyWindow( aHwnd );
            iHwnd = NULL;
            return 0;
        }
        break;
*/
    // Left button down is the start of dragging, capture the mouse offset
    case WM_LBUTTONDOWN:
        {
            POINT point;
            GetCursorPos( &point );
            ScreenToClient(aHwnd, &point);
            iXOff = point.x;
            iYOff = point.y;
           // char buffer[256];
            //sprintf( buffer, "Drag off %d,%d",  iXOff, iYOff );
           // SetWindowText( aHwnd, buffer );
            iDragWindow = true;
            SetCapture(aHwnd);
        }
        break;

    // Left button up is the end of dragging
    case WM_LBUTTONUP:
        {
            iXOff = 0;
            iYOff = 0;
            ReleaseCapture();
            if ( iDragWindow )
            {
                OnDragEnd();
            }
            iDragWindow = false;
        }
        break;

    // Mouse move with the button down should drag the window
    case WM_MOUSEMOVE:
        if (iDragWindow == true)
        {
            RECT mainWindowRect;
            POINT pos;
            int windowWidth, windowHeight;

            pos.x = (int)(short) LOWORD(aLParam);
            pos.y = (int)(short) HIWORD(aLParam);

            GetWindowRect(aHwnd,&mainWindowRect);
            windowHeight = mainWindowRect.bottom - mainWindowRect.top;
            windowWidth = mainWindowRect.right - mainWindowRect.left;

            ClientToScreen(aHwnd, &pos);
            //char buffer[256];
            //static int count;
           // count++;
            //sprintf( buffer, "Dragging %d,%d, %d\n",  pos.x-iXOff, pos.y-iYOff, count );
            //OutputDebugString( buffer );
            //SetWindowText( aHwnd, buffer );
            MoveWindow(aHwnd, pos.x-iXOff, pos.y-iYOff, windowWidth, windowHeight, TRUE);
        }
        break;
    }
    return DefWindowProc( aHwnd, aMsg, aWParam, aLParam );
}

LRESULT CALLBACK Window::StaticWndProc( HWND aHwnd, UINT aMsg, WPARAM aWParam, LPARAM aLParam )
{
    Window* thisPtr = reinterpret_cast < Window* > ( GetWindowLong ( aHwnd, GWL_USERDATA ) );
    if ( thisPtr )
    {
        if ( aMsg == WM_DESTROY )
        {
            SetWindowLong( thisPtr->iHwnd, GWL_USERDATA, NULL );
            //delete thisPtr;
        }
        else
        {
            return thisPtr->WndProc( aHwnd, aMsg, aWParam, aLParam );
        }
    }
    return DefWindowProc( aHwnd, aMsg, aWParam, aLParam );
}

// End of file
