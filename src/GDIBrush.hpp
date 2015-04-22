#ifndef _GDI_BRUSH_HPP
#define _GDI_BRUSH_HPP

#include <windows.h>
#include <assert.h>

class RGDIBrush
{
public:
    RGDIBrush( unsigned char aR, 
               unsigned char aG, unsigned char aB );

    RGDIBrush( unsigned char aR, 
               unsigned char aG, unsigned char aB, int aStyle );

    ~RGDIBrush();
    const HBRUSH Brush() const;
private:
    HBRUSH iBrush;
};

#endif // _GDI_BRUSH_HPP

// End of file
