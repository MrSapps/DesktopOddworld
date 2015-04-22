#include "Anim.hpp"
#include "resource.h"

Anim::Anim()
{
    // Load the sprite sheet
    iSpriteSheet.Construct( IDB_SLIG );

    // Default speed
    const int KDefaultSpeed = 1;

    // Walk left
    iWalkLeft = new AnimationData();
    iWalkLeft->iW = 121;
    iWalkLeft->iH = 47;
    iWalkLeft->iX = 0;
    iWalkLeft->iY = 540;
    iWalkLeft->iNumFrames = 9;
    iWalkLeft->iSpeed = KDefaultSpeed;
    iWalkLeft->iAnimation = EWalkLeft;
    iWalkLeft->iLoop = true;
    iWalkLeft->Create();

    // Walk right
    iWalkRight = new AnimationData();
    iWalkRight->iW = 121;
    iWalkRight->iH = 47;
    iWalkRight->iX = 0;
    iWalkRight->iY = 540-47;
    iWalkRight->iNumFrames = 9;
    iWalkRight->iSpeed = KDefaultSpeed;
    iWalkRight->iAnimation = EWalkRight;
    iWalkRight->iLoop = true;
    iWalkRight->Create();

    // Turn left
    iTurnLeft = new AnimationData();
    iTurnLeft->iW = 121;
    iTurnLeft->iH = 47;
    iTurnLeft->iX = 0;
    iTurnLeft->iY = 540-47-47;
    iTurnLeft->iNumFrames = 9;
    iTurnLeft->iSpeed = 2;
    iTurnLeft->iAnimation = ETurnLeft;
    iTurnLeft->iLoop = false;
    iTurnLeft->Create();

    // Turn right
    iTurnRight = new AnimationData();
    iTurnRight->iW = 121;
    iTurnRight->iH = 47;
    iTurnRight->iX = 0;
    iTurnRight->iY = 540-47-47-47;
    iTurnRight->iNumFrames = 9;
    iTurnRight->iSpeed = 2;
    iTurnRight->iAnimation = ETurnRight;
    iTurnRight->iLoop = false;
    iTurnRight->Create();

    // Fall right
    iFallRight = new AnimationData();
    iFallRight->iW = 88;
    iFallRight->iH = 55;
    iFallRight->iX = 0;
    iFallRight->iY = 0;
    iFallRight->iNumFrames = 6;
    iFallRight->iSpeed = 1;
    iFallRight->iAnimation = EFallingRight;
    iFallRight->iLoop = true;
    iFallRight->Create();

    // Fall left
    iFallLeft = new AnimationData();
    iFallLeft->iW = 88;
    iFallLeft->iH = 55;
    iFallLeft->iX = 0;
    iFallLeft->iY = 55;
    iFallLeft->iNumFrames = 6;
    iFallLeft->iSpeed = 1;
    iFallLeft->iAnimation = EFallingLeft;
    iFallLeft->iLoop = true;
    iFallLeft->Create();

    // Hit floor right
    iHitFloorRight = new AnimationData();
    iHitFloorRight->iW = 88;
    iHitFloorRight->iH = 55;
    iHitFloorRight->iX = 0;
    iHitFloorRight->iY = 55+55+47+47;
    iHitFloorRight->iNumFrames = 12;
    iHitFloorRight->iSpeed = 2;
    iHitFloorRight->iAnimation = EHitFloorRight;
    iHitFloorRight->iLoop = false;
    iHitFloorRight->Create();

    // Hit floor left
    iHitFloorLeft = new AnimationData();
    iHitFloorLeft->iW = 88;
    iHitFloorLeft->iH = 55;
    iHitFloorLeft->iX = 0;
    iHitFloorLeft->iY = 55+55+47+47+53;
    iHitFloorLeft->iNumFrames = 12;
    iHitFloorLeft->iSpeed = 2;
    iHitFloorLeft->iAnimation = EHitFloorLeft;
    iHitFloorLeft->iLoop = false;
    iHitFloorLeft->Create();

    iCurrent = iFallRight;
}

Anim::~Anim()
{

}

int Anim::Update()
{
    return iCurrent->Update();
}

Rect Anim::FrameRect()
{
    return iCurrent->iFrames[ iCurrent->iCurFrame ];
}

void Anim::SetCurrent(Animations aAnimation)
{
    iCurrent->Reset();

    switch ( aAnimation )
    {
    case EWalkLeft:
        iCurrent = iWalkLeft;
        break;

    case EWalkRight:
        iCurrent = iWalkRight;
        break;

    case ETurnLeft:
        iCurrent = iTurnLeft;
        break;

    case ETurnRight:
        iCurrent = iTurnRight;
        break;

    case EFallingRight:
        iCurrent = iFallRight;
        break;

    case EFallingLeft:
        iCurrent = iFallLeft;
        break;

    case EHitFloorRight:
        iCurrent = iHitFloorRight;
        break;

    case EHitFloorLeft:
        iCurrent = iHitFloorLeft;
        break;
    }
}

Anim::Animations Anim::CurrentAnimation()
{
    return iCurrent->iAnimation;
}

// End of file
