#ifndef _ANIM_HPP_
#define _ANIM_HPP_

#include "GDIBmp.hpp"
#include <vector>
#include "rect.hpp"

class Anim
{
public:
    Anim();
    ~Anim();
    int Update();
    Rect FrameRect();

    enum Animations
    {
        EWalkLeft,
        EWalkRight,
        ETurnLeft,
        ETurnRight,
        EFallingRight,
        EFallingLeft,
        EHitFloorRight,
        EHitFloorLeft,
    };
    void SetCurrent(Animations aAnimation);
    Animations CurrentAnimation();

    class AnimationData
    {
    public:
        AnimationData()
        {
            iFrameCounter = 0;
            iCurFrame = 0;
        }

        int iW; // Width of a frame
        int iH; // Height of a frame
        int iX; // XPos where frames start
        int iY; // YPos where frames start
        int iNumFrames; // How many frames are in this animation
        int iSpeed; // How fast this animation should run
        Animations iAnimation; // Which animation this is
        bool iLoop;
        std::vector< Rect > iFrames;
        int iFrameCounter;
        int iCurFrame;
    public:
        void Create()
        {
            for ( int i=0; i<iNumFrames; i++ )
            {
                Rect frame;
                frame.iX = iX + ( iW * i );
                frame.iY = iY;
                frame.iH = iH;
                frame.iW = iW;
                iFrames.push_back( frame );
            }
        }

        int Update()
        {
            int ret = 0;
            iFrameCounter++;
            if ( iFrameCounter > iSpeed )
            {
                iFrameCounter = 0;
                iCurFrame++;
                if ( iCurFrame >= iFrames.size()  )
                {
                    if ( iLoop )
                    {
                        iCurFrame = 0;
                    }
                    else
                    {
                        iCurFrame = iFrames.size()-1;
                        ret = 1;
                    }
                }
            }
            return ret;
        }

        void Reset()
        {
            iCurFrame = 0;
            iFrameCounter = 0;
        }
    };
    RGDIBmp iSpriteSheet;
private:
    AnimationData* iWalkLeft;
    AnimationData* iWalkRight;
    AnimationData* iTurnLeft;
    AnimationData* iTurnRight;

    // TODO
    AnimationData* iFallRight;
    AnimationData* iFallLeft;

    AnimationData* iHitFloorRight;
    AnimationData* iHitFloorLeft;

    AnimationData* iSplatRight;
    AnimationData* iSplatLeft;

    AnimationData* iCurrent;
};


#endif // _ANIM_HPP_

// End of file
