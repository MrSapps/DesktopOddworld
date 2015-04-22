#ifndef _BITMAP_HPP_
#define _BITMAP_HPP_

#include <windows.h>

class Bitmap
{
public:
    Bitmap( const char* aFileName );
    ~Bitmap();
    HANDLE Handle() { return iBitmap; }
    int W() { return iW; }
    int H() { return iH; }
private:
    HANDLE iBitmap;
    int iW;
    int iH;
};

#endif // _BITMAP_HPP_

// End of file
