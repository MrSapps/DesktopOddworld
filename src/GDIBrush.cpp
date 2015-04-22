#include "GDIBrush.hpp"

RGDIBrush::RGDIBrush( unsigned char aR, 
                      unsigned char aG, unsigned char aB )
{
    iBrush = CreateSolidBrush( RGB( aR, aG, aB ) );
    assert( iBrush );
}

RGDIBrush::RGDIBrush( unsigned char aR, 
               unsigned char aG, unsigned char aB, int aStyle )
{
    iBrush = CreateHatchBrush( aStyle, RGB( aR, aG, aB ) );
}

RGDIBrush::~RGDIBrush()
{
    DeleteObject( iBrush );
}

const HBRUSH RGDIBrush::Brush() const
{
    assert( iBrush );
    return iBrush;
}

// End of file
