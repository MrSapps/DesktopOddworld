#include "bitmap.hpp"
#include <sstream>

Bitmap::Bitmap( const char* aFileName )
{
    //iBitmap = LoadBitmapA( GetModuleHandle( NULL ), aFileName );
    iBitmap = LoadImageA( NULL, aFileName, IMAGE_BITMAP, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE ); 
    if ( !iBitmap )
    {
        std::stringstream ss;
        ss << "Error loading bitmap " << aFileName << " Error " << GetLastError();
        throw std::runtime_error( ss.str().c_str() );
    }
    BITMAP bm;
    GetObject( iBitmap, sizeof(BITMAP), (LPSTR)&bm);

    iW = bm.bmWidth;
    iH = bm.bmHeight;
}

Bitmap::~Bitmap()
{
    DeleteObject( iBitmap );
}

// End of file
