#ifndef _GDI_BMP_HPP
#define _GDI_BMP_HPP

#include <windows.h>
#include <assert.h>
#include <vector>
#include <string>

struct TRGB
{
    char r,g,b;
};

class RGDIBmp
{
public:
    bool Get2DPixels( std::vector< std::vector< TRGB > >& aDstBuffer ) const;
    bool GetPixels( std::vector< char >& aBuffer ) const;
    bool operator == ( const RGDIBmp& aOtherBmp ) const;
    RGDIBmp();
    RGDIBmp( const HDC aHDC, const int aWidth, const int aHeight );
    ~RGDIBmp();
    RECT Rect() const;
    const HBITMAP HBmp() const;
    void Close();
public:
    void Construct( const char* aPath );
    void Construct( const int aResourceID );
    std::string& File();
private:
    HBITMAP iBmp;
    std::string iFileName;
};

#endif // _GDI_BMP_HPP

// End of file
