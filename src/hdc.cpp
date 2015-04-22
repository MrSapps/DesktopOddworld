#include "hdc.hpp"
#include "gdibrush.hpp"

RHDC::RHDC( const HDC aHDC )
: iHDC( NULL ), iPrevObj( NULL ), iType( EDeleteObject ), iHwnd( NULL )
{
    iHDC = CreateCompatibleDC( aHDC );
    assert( iHDC );
}

RHDC::RHDC( const HWND aHwnd )
: iHDC( NULL ), iPrevObj( NULL ), iType( EReleaseDc ), iHwnd( aHwnd )
{
    iHDC = GetDC( aHwnd );
    assert( iHDC );
}

RHDC::~RHDC()
{
    ::SelectObject( iHDC, iPrevObj );
    if ( iType == EDeleteObject )
    {
        DeleteDC( iHDC );
    }
    else
    {
        ReleaseDC( iHwnd, iHDC );
    }
}

const HDC RHDC::Hdc() const
{
    return iHDC;
}

void RHDC::SelectObject( HGDIOBJ aObject )
{
    HGDIOBJ prevObj = ::SelectObject( iHDC, aObject );
    if ( !iPrevObj )
    {
        iPrevObj = prevObj;
    }
}

void RHDC::ClearRect( const RECT aRect, unsigned char aR, 
                      unsigned char aG, unsigned char aB )
{
    assert( iHDC );
    RGDIBrush brush( aR, aB, aB );
    SelectObject( brush.Brush() );
    FillRect( iHDC, &aRect, brush.Brush() );
}

// End of file
