#include "floatingwindow.hpp"
#include "hdc.hpp"

std::vector< WndInfo > FloatingWindow::iSysWnds;

FloatingWindow::FloatingWindow( const char* aTitle, const char* aWindowClass, const Rect& aSize )
: Window( aTitle, aWindowClass, aSize ), iSpeed( 2 ), 
  iXState( ENone ), iYState( EFalling ), iWalkingOnHwnd( NULL ), iAllowWalkThroughX( NULL ),
  iDrageedOnto( NULL )
{
    // get the current window size and position
    Rect rect;
    rect.GetWindowRect( iHwnd );

    //iFrames.Construct("C:\\1.bmp");
   // Resize( iFrames.Rect().right, iFrames.Rect().bottom );

    

    if ( !SetLayeredWindowAttributes( iHwnd, RGB(0, 0, 0), 0, LWA_COLORKEY) )
    {
        DWORD gle = GetLastError();
        int a;
        a = 0;
    }

    iAnim = new Anim();
// 87 = invalid param
}

FloatingWindow::~FloatingWindow()
{
    delete iAnim;
}

void FloatingWindow::GetWindows()
{
    // Clear the vector out
    iSysWnds.clear();

    // Get all windows
    EnumWindows( StaticEnumWindowsProc, NULL );

    // Get window rects
    for ( unsigned int i=0; i<iSysWnds.size(); i++ )
    {
        iSysWnds[i].DebugString();
       // OutputDebugString(output.c_str());
    }
    int a;
    a = 0;
}

void FloatingWindow::Tick()
{
    // Always update the animation
    if ( iAnim->Update() == 1 )
    {
        // Animation has finished so we need to set a new one
        switch( iAnim->CurrentAnimation() )
        {         
        case Anim::ETurnLeft:
            iAnim->SetCurrent( Anim::EWalkLeft );
            iXState = EMoveRight;
            break;

        case Anim::ETurnRight:
            iAnim->SetCurrent( Anim::EWalkRight );
            iXState = EMoveLeft;
            break;

        case Anim::EHitFloorRight:
            iAnim->SetCurrent( Anim::EWalkRight );
            iXState = EMoveLeft;
            break;

        case Anim::EHitFloorLeft:
            iAnim->SetCurrent( Anim::EWalkLeft );
            iXState = EMoveRight;
            break;
        }
    }

    // Only update if the window is not being dragged.
    if ( !iDragWindow )
    {
        // TODO: Test for collision, if we're colliding WITHOUT moving, then a window
        // must have moved, thus we should fall through it if we're not "close" to its top

        // Get the window rect
        Rect rect;
        rect.GetWindowRect( iHwnd );

        // Move the rect down
        rect.iY+=iSpeed;      

        // Ensure the new position is within screen bounds!
        KeepRectInWindow( GetDesktopWindow(), rect );

        // Ensure the new position is not passing through the top of a window, since we want to "sit" on them
        KeepOnWindowTops( rect, EDown );

        if ( iXState == EMoveLeft 
            && iAnim->CurrentAnimation()!=Anim::ETurnRight && iAnim->CurrentAnimation()!=Anim::ETurnLeft 
            && iAnim->CurrentAnimation()!=Anim::EHitFloorLeft && iAnim->CurrentAnimation()!=Anim::EHitFloorRight )
        {
            // Move the rect to the left
            rect.iX-=iSpeed;

            // Ensure the new position is within screen bounds!
            KeepRectInWindow( GetDesktopWindow(), rect );

            // Ensure the new position is not passing through the top of a window, since we want to "sit" on them
            KeepOnWindowTops( rect, ELeft );
        }
        else if ( iXState == EMoveRight 
            && iAnim->CurrentAnimation()!=Anim::ETurnRight && iAnim->CurrentAnimation()!=Anim::ETurnLeft 
            && iAnim->CurrentAnimation()!=Anim::EHitFloorLeft && iAnim->CurrentAnimation()!=Anim::EHitFloorRight )
        {
            // Move the rect to the right
            rect.iX+=iSpeed;

            // Ensure the new position is within screen bounds!
            KeepRectInWindow( GetDesktopWindow(), rect );

            // Ensure the new position is not passing through the top of a window, since we want to "sit" on them
            KeepOnWindowTops( rect, ERight );
        }

        // Move window into position
        MoveWindow(iHwnd, rect.iX, rect.iY, rect.iW, rect.iH, TRUE);
        InvalidateRect( iHwnd, NULL, FALSE );
    }
    else
    {
        if ( iXState != EMoveRight )
        {
            iAnim->SetCurrent( Anim::EFallingRight );
            //iXState = EMoveRight;
        }
        else
        {
            iAnim->SetCurrent( Anim::EFallingLeft );
            //iXState = EMoveLeft;
        }
        InvalidateRect( iHwnd, NULL, FALSE );
    }
}

BOOL CALLBACK FloatingWindow::StaticEnumWindowsProc( HWND aHwnd, LPARAM aLParam )
{
    // Visible and not minimized and not maxed (maxed windows can't be used for collision testing!!)
    // actually they can because the task bar is maxed although it dosent fill the screen!
    WndInfo info( aHwnd );
    if ( IsWindowVisible( aHwnd ) && !IsIconic( aHwnd ) /*&& !IsZoomed( aHwnd )*/ && info.iClassName != "test" )
    {
        
        // If YPos is zero then we can't "walk" across it, so don't bother adding it
        if ( info.iRect.iY > 0 )
        {
            iSysWnds.push_back( info );
        }
    }
    else
    {
        if ( info.iClassName.length() )
        {
            int a;
            a = 0;
        }
    }
    return TRUE;
}

void FloatingWindow::KeepRectInWindow( HWND aWindow, Rect& aRect )
{
    // Get the rect
    Rect winRect;
    winRect.GetWindowRect( aWindow );

    //char tmp[512];
    //sprintf( tmp, "Slig X(%d) W(%d) Desktop x w = %d,%d\n",  aRect.iX, aRect.iW, winRect.iX, winRect.iW );
    //OutputDebugString( tmp );

    // Keep X in screen bounds
    if ( aRect.iX < winRect.iX )
    {
        aRect.iX = winRect.iX;
        iXState = EMoveRight;
        iAnim->SetCurrent( Anim::ETurnLeft );
    }
    else if  ( (aRect.iX+aRect.iW) > (winRect.iW) ) 
    {
        aRect.iX = winRect.iW - aRect.iW;
        iXState = EMoveLeft;
        iAnim->SetCurrent( Anim::ETurnRight );
    }

    // Keep Y in screen bounds
    if ( aRect.iY < winRect.iY )
    {
        aRect.iY = winRect.iY;
    }
    else if ( (aRect.iY+aRect.iH) > (winRect.iH) )
    {
        aRect.iY = winRect.iH - aRect.iH;
    }
}

void FloatingWindow::KeepOnWindowTops( Rect& aRect, Direction direction )
{
    // TODO: Dont allow window to be pushed outside of GetDesktopWindow() area** LEAVE FOR NOW

    // TOOD: Ensure window is always on top, use 255,0,255 for testing instead of white
    // TODO: GDI Double buffering rendering of bitmaps for slig frames -- Get a test Anim up and running
    // TODO: Walking, turning, falling , and hit floor slig frames
    // TODO: System tray icon for exit/about/options
    // TODO: Slig sounds, random game speak?

    // If this window is set
    if ( iAllowWalkThroughX )
    {
        // Check if we are in its rect, if not then unset it
        if ( !IsWindowFullyInWindow( iHwnd, iAllowWalkThroughX ) )
        {
            iAllowWalkThroughX = NULL;
        }
        // Is iHwnd in iAllowWalkThroughX?
    }

    bool yhit = false;
    for ( unsigned int i=0; i<iSysWnds.size(); i++ )
    {
        if ( iSysWnds[i].iHwnd != iHwnd )
        {
            // Is the bottom of the slig lower than the top of any window?
            if ( direction == EDown )
            {
                // Don't allow passing through the tops of windows!

                // Check in the bottom of the slig is going through the top of the window
                if ( aRect.iY+aRect.iH >= iSysWnds[i].iRect.iY && aRect.iY < iSysWnds[i].iRect.iY+iSysWnds[i].iRect.iH  )
                {
                    // We're breaking the Y barrier, check if we're within X range
                    if ( aRect.iX+aRect.iW > iSysWnds[i].iRect.iX && 
                         aRect.iX < iSysWnds[i].iRect.iX+iSysWnds[i].iRect.iW )
                    {
                        if ( !IsWindowInFront( iWalkingOnHwnd, iSysWnds[i].iHwnd ) && iDrageedOnto != iSysWnds[i].iHwnd )
                        {
                            // If we're inside a window because we're walking on a window that has a lower Z order
                            // then do not fix up Y
                            if ( iAllowWalkThroughX != iSysWnds[i].iHwnd && iDrageedOnto != iSysWnds[i].iHwnd )
                            {
                                // Yes so ensure the slig stays on top of the window
                                // Note this can cause "jumping" if a user suddenly drags a window
                                // right on top of the slig it will appear to be on top of it instantly..
                                aRect.iY = iSysWnds[i].iRect.iY-aRect.iH;

                                // If we've just hit the floor
                                if ( iYState == EHitFloor )
                                {
                                    // Then goto normal/walking state
                                    iYState = ENormal;

                                    // TODO Store the window that we're walking on!!!!!!!!1
                                    // This allows the X collision to know if the window is behind the slig
                                    // in which case to ignore the X collision.
                                    iWalkingOnHwnd = iSysWnds[i].iHwnd;

                                    // Start to move left or right!
                                    if ( iXState = EMoveRight )
                                    {
                                        iAnim->SetCurrent( Anim::EHitFloorRight );
                                        iXState = EMoveLeft;
                                    }
                                    else
                                    {
                                        iAnim->SetCurrent( Anim::EHitFloorLeft );
                                        iXState = EMoveRight;
                                    }
                                }

                                // If we've just been falling
                                if ( iYState == EFalling )
                                {
                                    iYState = EHitFloor;
                                }

                                yhit = true;
                                break;
                            }
                        }
                    }
                }
            }

            if ( direction == ERight )
            {
                // Don't allow passing through the sides of windows!

                // Check if slig right is going to hit the X of any window
                if ( aRect.iX+aRect.iW >= iSysWnds[i].iRect.iX && aRect.iX < iSysWnds[i].iRect.iX+iSysWnds[i].iRect.iW )
                {
                    // We're going to break the X barrier, check if we're within Y range
                    if ( aRect.iY+aRect.iH > iSysWnds[i].iRect.iY && 
                         aRect.iY < iSysWnds[i].iRect.iY + iSysWnds[i].iRect.iH )
                    {
                        if ( !IsWindowInFront( iWalkingOnHwnd, iSysWnds[i].iHwnd ) )
                        {
                            if ( iDrageedOnto != iSysWnds[i].iHwnd )
                            {
                                // We're in Y so limit X
                                aRect.iX = iSysWnds[i].iRect.iX -aRect.iW;

                                // And now we need to move left!
                                iAnim->SetCurrent( Anim::ETurnRight );

                                iXState = EMoveLeft;
                            }
                        }
                        // TODO: break?
                    }
                }
            }
           
           if ( direction == ELeft )
           {
                // Don't allow passing through the sides of windows!

                // Check if slig left is going to hit the X of any window
               if ( aRect.iX <= iSysWnds[i].iRect.iX+iSysWnds[i].iRect.iW && aRect.iX+aRect.iW > iSysWnds[i].iRect.iX )
                {
                    // We're going to break the X barrier, check if we're within Y range
                    if ( aRect.iY+aRect.iH > iSysWnds[i].iRect.iY && 
                         aRect.iY < iSysWnds[i].iRect.iY + iSysWnds[i].iRect.iH )
                    {
                        // We're in Y so limit X if this window is INFRONT of iWalkingOnHwnd
                        if ( !IsWindowInFront( iWalkingOnHwnd, iSysWnds[i].iHwnd ) )
                        {
                            if ( iDrageedOnto != iSysWnds[i].iHwnd )
                            {
                                aRect.iX = iSysWnds[i].iRect.iX+iSysWnds[i].iRect.iW;

                                // And now we need to move right!
                                iAnim->SetCurrent( Anim::ETurnLeft );
                                iXState = EMoveRight;
                            }
                            // TODO: break?
                        }
                    }
                }
           }
        }
    }
    if ( !yhit && direction == EDown )
    {
        iWalkingOnHwnd = NULL;

        // We must be falling!
        iYState = EFalling;

        if ( iXState = EMoveRight )
        {
            iAnim->SetCurrent( Anim::EFallingRight );
            iXState = EMoveRight;
        }
        else
        {
            iAnim->SetCurrent( Anim::EFallingLeft );
            iXState = EMoveLeft;
        }

        // Stop walking!
        iXState = ENone;
    }
}

bool FloatingWindow::IsWindowInFront( HWND aWindow, HWND aMightBeInFrontWindow )
{
    // Is aMightBeInFrontWindow in front of aWindow?
    while ( aWindow = GetNextWindow( aWindow, GW_HWNDNEXT ) )
    {
        if ( aWindow == aMightBeInFrontWindow )
        {
            iAllowWalkThroughX = aMightBeInFrontWindow;
            return true;
        }
    }
//    iAllowWalkThroughX = NULL;
    return false;
}

bool FloatingWindow::IsWindowFullyInWindow( HWND aFirst, HWND aSecond )
{
    // Get the rect
    Rect first;
    first.GetWindowRect( aFirst );

    Rect second;
    second.GetWindowRect( aSecond );

    // Is the left and right fully within the X of the window?
    if ( second.iX <= first.iX+first.iW &&  second.iX+second.iH >= first.iX )
    {
        // Is the top and bottom fully within the Y of the window?
        if ( second.iY + second.iH >= first.iY && second.iY <= first.iY + first.iH )
        {
            return true;
        }
    }

    return false;
}

void FloatingWindow::OnPaint()
{
    PAINTSTRUCT ps;

    Rect winRect;
    winRect.GetWindowRect( iHwnd );

    // Device context
    HDC gdiContext = BeginPaint(iHwnd, &ps);

    // Create a mem DC
    RHDC dc( gdiContext ); // CreateCompatibleDC

    // Get the current frame area to copy
    Rect frameRect = iAnim->FrameRect();

    // Select the bmp into the new mem context   
    RGDIBmp bmp( gdiContext, winRect.iW, winRect.iH); // Back buffer
    dc.SelectObject( bmp.HBmp() );

    // TODO: Clear the back buffer to avoid courrpted gfx
    RECT clearRect = { 0 };
    clearRect.top = 0;
    clearRect.left = 0;
    clearRect.bottom = winRect.iH;
    clearRect.right = winRect.iW;
    dc.ClearRect( clearRect, 0,0,0 );

    RHDC tsHdc( gdiContext );

    // Select the sprite sheet
    tsHdc.SelectObject( iAnim->iSpriteSheet.HBmp() );

    int yoff = abs( winRect.iH - frameRect.iH );

    // Into back buffer
    BitBlt( dc.Hdc(), 0, yoff, // Always in the top left of the target rect
        frameRect.iW, frameRect.iH, // Always the frame size
        
        tsHdc.Hdc() , 
        frameRect.iX, frameRect.iY,  // Always the location of the frame in the sprite sheet
        SRCCOPY );

   // StretchBlt( gdiContext, 
    //    0,0 ,
     //   121 , 47, // Use all of the window
      //  dc.Hdc(),
       // 0,0,
        // frameRect.iW, frameRect.iH , SRCCOPY); // All of the frame

    // Back buffer to screen
    BitBlt( gdiContext, 0,0, // Always the top left of the window
       winRect.iW, winRect.iH,
        dc.Hdc(), 0,0 , SRCCOPY );

    POINT pos;
//    pos.x = x;
 //   pos.y = y;

    SIZE size;
  //  size.cx = w;
   // size.cy = h;
/*
    if ( !UpdateLayeredWindow( iHwnd, gdiContext, &pos, &size, dc.Hdc(), // hdcSrc
                                 NULL, RGB( 0, 0, 0 ), NULL, ULW_COLORKEY ) )
    {
        DWORD gle = GetLastError();
        int a;
        a =0;
    }
*/
    EndPaint(iHwnd, &ps);
}

void FloatingWindow::OnDragEnd()
{
    // If we where dragged fully off the window we where ignoring collision with
    // then stop ignoring collision with it
    if ( iAllowWalkThroughX )
    {
        if ( !IsWindowFullyInWindow( iHwnd, iAllowWalkThroughX ) )
        {
            iAllowWalkThroughX = NULL;
        }
    }
/*
    // If we've been dragged fully inside a window then we need to ignore collision with it
    iDrageedOnto = NULL;
    for ( unsigned int i=0; i<iSysWnds.size(); i++ )
    {
        if ( iSysWnds[i].iHwnd != iHwnd )
        {
            if ( IsWindowFullyInWindow( iHwnd, iSysWnds[i].iHwnd ) )
            {
                iDrageedOnto = iSysWnds[i].iHwnd;
            }
        }
    }
*/
}

// End of file
