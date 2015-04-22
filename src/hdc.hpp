#ifndef _HDC_HPP
#define _HDC_HPP

#include <windows.h>
#include <assert.h>

class RHDC
{
public:
    RHDC( const HDC aHDC );
    RHDC( const HWND aHwnd );
    ~RHDC();
    const HDC Hdc() const;
    void SelectObject( HGDIOBJ aObject );
    void ClearRect( const RECT aRect, unsigned char aR, 
                    unsigned char aG, unsigned char aB );
private:
    enum THDCType
    {
        EDeleteObject,
        EReleaseDc,
    };
    HDC iHDC;
    THDCType iType;
    HWND iHwnd;
    HGDIOBJ iPrevObj;
};

#endif // _HDC_HPP

// End of file
