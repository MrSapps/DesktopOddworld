#include "GDIBmp.hpp"
#include "hdc.hpp"

RGDIBmp::RGDIBmp()
: iBmp( 0 )
{

}

bool RGDIBmp::Get2DPixels( std::vector< std::vector< TRGB > >& aDstBuffer ) const
{
    bool result = true;

    // Create a device context
    HWND hwnd = NULL;
    RHDC hdc( hwnd );

    // Get the bitmap W/h
    DWORD w = Rect().right;
    DWORD h = Rect().bottom;

    // Get the bitmap size
    BITMAPINFO info = { 0 };

    // Set the structure size
    info.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    info.bmiHeader.biCompression = BI_RGB;
    info.bmiHeader.biWidth = w;
    info.bmiHeader.biHeight = h;
    info.bmiHeader.biBitCount = 24;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biSizeImage = w*h*3; // 3 = RGB

    // Create a 2d array
    aDstBuffer.resize( w );
    for ( unsigned int i=0; i<aDstBuffer.size(); ++i )
    {
        aDstBuffer[i].resize( h );
    }

    // Read the bitmap pixels
    std::vector< TRGB > m;
    m.resize( w*h );
    int retVal = GetDIBits( hdc.Hdc() , iBmp, 0,h, &m[0], &info, DIB_RGB_COLORS );
    if ( retVal != h )
    {
        result = false;
    }
    else
    {
        /*
        for ( unsigned int i=0; i<m.size(); ++i )
        {
            TRGB t = m[i];
            m[i].r = t.b;
            m[i].r = t.r;
            m[i].r = t.r;
        }
        */
        int c = w*h-1;

        for ( unsigned int y=0; y<h; ++y )
        {
            for ( unsigned int x=0; x<w; ++x )
            {
                aDstBuffer[ x ] [ y ] = m [ c ]; c--;
            }
        } 


        // Need to flip all X's
        std::vector< std::vector< TRGB >  > buf;
        buf.resize( w );
        for ( unsigned int i=0; i<buf.size(); ++i )
        {
            buf[i].resize( h );
        }

        for ( unsigned int y=0; y<h; ++y )
        {
            for ( unsigned int x=w; x>0; x-- )
            {
                buf [ (w)-x ] [y] = aDstBuffer[ x-1 ] [ y ];
            }
        } 
        aDstBuffer = buf;
    }

    return result;
}

bool RGDIBmp::GetPixels( std::vector< char >& aBuffer ) const
{


    // Create a device context
    HWND hwnd = NULL;
    RHDC hdc( hwnd );

    // Get the bitmap size
    BITMAPINFO info = { 0 };
    // Set the structure size
    info.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    info.bmiHeader.biCompression = BI_RGB;
    info.bmiHeader.biWidth = 16;
    info.bmiHeader.biHeight = 16;
    info.bmiHeader.biBitCount = 24;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biSizeImage = 16*16*3;

    std::vector< char > t;
    t.resize( 16*16*3 );
    aBuffer.resize( 16*16*3 );

    // Grab the info
    int retVal = GetDIBits( hdc.Hdc() , iBmp, 0,16, &t[0], &info, DIB_RGB_COLORS );
    
    for ( unsigned int i=0; i<t.size(); ++i )
    {
        aBuffer[i] = t[ t.size() - i -1 ];
    }

    /*
    // If we got the info ok
    bool GotPixels = false;
    if ( retVal )
    {
        // Calc how many bytes we need
        const int size = sizeof( BITMAPINFOHEADER ) + 
                         sizeof( RGBQUAD ) * info.bmiHeader.biClrUsed;

        // Allocate a new bitmap info of that size
        BITMAPINFO* infoPtr = ( BITMAPINFO* )new BYTE[ size ]; 
    
        // Set the header
        infoPtr->bmiHeader = info.bmiHeader;
        // Read the pixels as RGB
        infoPtr->bmiHeader.biCompression = BI_RGB;

        // Resize the output buffer
        assert( info.bmiHeader.biSizeImage );
        int datalen = info.bmiHeader.biSizeImage / 4;
        aBuffer.resize( datalen );

        // Get the pixels
        int readScanLines = GetDIBits( hdc.Hdc() , iBmp, 0, 
                                       info.bmiHeader.biHeight, &aBuffer[0], 
                                       infoPtr, DIB_RGB_COLORS );

        // Cleanup
        delete[] ( BYTE* )infoPtr;    

        if ( readScanLines == info.bmiHeader.biHeight )
        {
            GotPixels = true;
        }
    }
    return GotPixels;
    */
    return 1;
}

bool RGDIBmp::operator == ( const RGDIBmp& aOtherBmp ) const
{
    return false; // ARGH
/*
    // Get our pixesl
    std::vector< RGBQUAD > usPixels;
    bool gotUs = GetPixels( usPixels );
    assert( gotUs == true );

    // Get their pixels
    std::vector< RGBQUAD > themPixels;
    bool gotThem = aOtherBmp.GetPixels( themPixels );
    assert( gotThem == true );

    if ( usPixels.size() != themPixels.size() )
    {
        return false;
    }

    return ( memcmp( &usPixels[0], 
                     &themPixels[0], 
                     usPixels.size() * sizeof( RGBQUAD ) ) == 0 );
                     */
}

std::string& RGDIBmp::File()
{
    return iFileName;
}

void RGDIBmp::Construct( const char* aPath )
{
    iFileName = aPath;
    iBmp = ( HBITMAP )LoadImage( GetModuleHandle( NULL ), aPath, 
        IMAGE_BITMAP, 
        0, // Use actuall size
        0, // Use actuall size
        LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
    assert( iBmp );
}

void RGDIBmp::Construct( const int aResourceID )
{
    iBmp = LoadBitmap( GetModuleHandle( NULL ), MAKEINTRESOURCE( aResourceID ) );
    assert( iBmp );
}

RGDIBmp::RGDIBmp( const HDC aHDC, const int aWidth, const int aHeight )
  : iBmp( NULL )
{
    iBmp = CreateCompatibleBitmap( aHDC, aWidth, aHeight );
    assert( iBmp );
}

RGDIBmp::~RGDIBmp()
{
    Close();
}

const HBITMAP RGDIBmp::HBmp() const
{
//    assert( iBmp );
    return iBmp;
}

RECT RGDIBmp::Rect() const
{
    BITMAP tempBMP;
    GetObject( iBmp, sizeof( BITMAP ), &tempBMP );
    RECT bmpRect;
    bmpRect.left = 0;
    bmpRect.right = tempBMP.bmWidth;
    bmpRect.top = 0;
    bmpRect.bottom = tempBMP.bmHeight;
    return bmpRect;
}

void RGDIBmp::Close()
{
    DeleteObject( iBmp );
    iBmp = NULL;
}

// End of file
