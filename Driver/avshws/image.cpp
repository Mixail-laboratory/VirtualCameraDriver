/**************************************************************************

    AVStream Simulated Hardware Sample

    Copyright (c) 2001, Microsoft Corporation.

    File:

        image.cpp

    Abstract:

        The image synthesis and overlay code.  These objects provide image
        synthesis (pixel, color-bar, etc...) onto RGB24 and UYVY buffers as
        well as software string overlay into these buffers.

	This entire file, data and all, must be in locked segments.

    History:

        created 1/16/2001

**************************************************************************/

#include "avshws.h"

/**************************************************************************

    Constants

**************************************************************************/



//
// Standard definition of EIA-189-A color bars.  The actual color definitions
// are either in CRGB24Synthesizer or CYUVSynthesizer.
//

/**************************************************************************

    LOCKED CODE

**************************************************************************/

#ifdef ALLOC_PRAGMA
#pragma code_seg()
#endif // ALLOC_PRAGMA


void
CImageSynthesizer::
SynthesizeBars (PVOID data
    )

/*++

Routine Description:

    Synthesize EIA-189-A standard color bars onto the Image.  The image
    in question is the current synthesis buffer.

Arguments:

    None

Return Value:

    None

--*/

{
   
    PUCHAR dataPtr = (PUCHAR)data;

    //
    // Set the default cursor...
    //
    GetImageLocation(0, 0);

    //
    // Synthesize a single line.
    //

    
        PUCHAR ImageStart = m_Cursor;
        for (ULONG x = 0; x < m_Width * 4; x+=4) {
            UCHAR pixel[3] = { 146, 211, 16 };
            PutPixel(pixel);
        }
        PUCHAR ImageEnd = m_Cursor;
        
    
        for (ULONG line = 0; line < m_Height; line++) {
            GetImageLocation(0, line);
            RtlCopyMemory(
                m_Cursor,
                ImageStart,
                ImageEnd - ImageStart
            );
        }
   

    
 
}

void CImageSynthesizer::DrawFrame()
{

}

/*************************************************/


void 
CImageSynthesizer::
OverlayText (
    _In_ ULONG LocX,
    _In_ ULONG LocY,
    _In_ ULONG Scaling,
    _In_ LPSTR Text

    )

/*++

Routine Description:

    Overlay text onto the synthesized image.  Clip to fit the image
    if the overlay does not fit.  The image buffer used is the set
    synthesis buffer.

Arguments:

    LocX -
        The X location on the image to begin the overlay.  This MUST
        be inside the image.  POSITION_CENTER may be used to indicate
        horizontal centering.

    LocY -
        The Y location on the image to begin the overlay.  This MUST
        be inside the image.  POSITION_CENTER may be used to indicate
        vertical centering.

    Scaling -
        Normally, the overlay is done in 8x8 font.  A scaling of
        2 indicates 16x16, 3 indicates 24x24 and so forth.

    Text -
        A character string containing the information to overlay

    BgColor -
        The background color of the overlay window.  For transparency,
        indicate TRANSPARENT here.

    FgColor -
        The foreground color for the text overlay.

Return Value:

    None

--*/

{

    NT_ASSERT ((LocX <= m_Width || LocX == POSITION_CENTER) &&
            (LocY <= m_Height || LocY == POSITION_CENTER));

    ULONG StrLen = 0;
    CHAR* CurChar;

    //
    // Determine the character length of the string.
    //
    for (CurChar = Text; CurChar && *CurChar; CurChar++)
        StrLen++;

    //
    // Determine the physical size of the string plus border.  There is
    // a definable NO_CHARACTER_SEPARATION.  If this is defined, there will
    // be no added space between font characters.  Otherwise, one empty pixel
    // column is added between characters.
    //
    #ifndef NO_CHARACTER_SEPARATION
        ULONG LenX = (StrLen * (Scaling << 3)) + 1 + StrLen;
    #else // NO_CHARACTER_SEPARATION
        ULONG LenX = (StrLen * (Scaling << 3)) + 2;
    #endif // NO_CHARACTER_SEPARATION

    ULONG LenY = 2 + (Scaling << 3);

    //
    // Adjust for center overlays.
    //
    // NOTE: If the overlay doesn't fit into the synthesis buffer, this
    // merely left aligns the overlay and clips off the right side.
    //
    if (LocX == POSITION_CENTER) {
        if (LenX >= m_Width) {
            LocX = 0;
        } else {
            LocX = (m_Width >> 1) - (LenX >> 1);
        }
    }

    if (LocY == POSITION_CENTER) {
        if (LenY >= m_Height) {
            LocY = 0;
        } else {
            LocY = (m_Height >> 1) - (LenY >> 1);
        }
    }

    //
    // Determine the amount of space available on the synthesis buffer.
    // We will clip anything that finds itself outside the synthesis buffer.
    //
    ULONG SpaceX = m_Width - LocX;
    ULONG SpaceY = m_Height - LocY;

    //
    // Set the default cursor position.
    //
    GetImageLocation (LocX, LocY);

    //
    // Overlay a background color row.
    //
   
    LocY++;
    if (SpaceY) SpaceY--;

    //
    // Loop across each row of the image.
    //
    for (ULONG row = 0; row < 8 && SpaceY; row++) {
        //
        // Generate a line.
        //
        GetImageLocation (LocX, LocY++);

        PUCHAR ImageStart = m_Cursor;

        ULONG CurSpaceX = SpaceX;
       

        //
        // Generate the row'th row of the overlay.
        //
        CurChar = Text;
        
        //
        // If there is no separation character defined, account for the
        // border.
        // 
        #ifdef NO_CHARACTER_SEPARATION
            if (CurSpaceX) {
                PutPixel (BgColor);
                CurSpaceX--;
            }
        #endif // NO_CHARACTER_SEPARATION
            

        PUCHAR ImageEnd = m_Cursor;
        //
        // Copy the line downward scale times.
        //
        for (ULONG scale = 1; scale < Scaling && SpaceY; scale++) {
            GetImageLocation (LocX, LocY++);
            RtlCopyMemory (m_Cursor, ImageStart, ImageEnd - ImageStart);
            SpaceY--;
        }

    }

    //
    // Add the bottom section of the overlay.
    //
    GetImageLocation (LocX, LocY);
    

}


void CImageSynthesizer::CopyBuffer(PVOID data, ULONG dataLength)
{

}