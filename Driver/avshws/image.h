/**************************************************************************

    AVStream Simulated Hardware Sample

    Copyright (c) 2001, Microsoft Corporation.

    File:

        image.h

    Abstract:

        The image synthesis and overlay header.  These objects provide image
        synthesis (pixel, color-bar, etc...) onto RGB24 and UYVY buffers as
        well as software string overlay into these buffers.

    History:

        created 1/16/2001

**************************************************************************/

/**************************************************************************

    Constants

**************************************************************************/

//
// COLOR:
//
// Pixel color for placement onto the synthesis buffer.
//


//
// POSITION_CENTER:
//
// Only useful for text overlay.  This can be substituted for LocX or LocY
// in order to center the text screen on the synthesis buffer.
//
#define POSITION_CENTER ((ULONG)-1)

/*************************************************

    CImageSynthesizer

    This class synthesizes images in various formats for output from the
    capture filter.  It is capable of performing various text overlays onto
    the image surface.

*************************************************/

class CImageSynthesizer {

protected:

    //
    // The width and height the synthesizer is set to. 
    //
    ULONG m_Width;
    ULONG m_Height;

    //
    // The synthesis buffer.  All scan conversion happens in the synthesis
    // buffer.  This must be set with SetBuffer() before any scan conversion
    // routines are called.
    //
    PUCHAR m_SynthesisBuffer;

    //
    // The default cursor.  This is a pointer into the synthesis buffer where
    // a non specific PutPixel will be placed. 
    //
    PUCHAR m_Cursor;

public:

    //
    // PutPixel():
    //
    // Place a pixel at the specified image cursor and move right
    // by one pixel.  No bounds checking...  wrap around occurs.
    //
    virtual void
    PutPixel (
        PUCHAR *ImageLocation,
        UCHAR color[2]
        ) = 0;

    //
    // PutPixel():
    //
    // Place a pixel at the default image cursor and move right
    // by one pixel.  No bounds checking...  wrap around occurs.
    //
    // If the derived class doesn't provide an implementation, provide
    // one.
    //
    virtual void
    PutPixel (
        UCHAR color[2]
    )
    {
        PutPixel (&m_Cursor, color);
    }

    virtual long
    GetBytesPerPixel() = 0;
        

    //
    // GetImageLocation():
    //
    // Get the location into the image buffer for a specific X/Y location.
    // This also sets the synthesizer's default cursor to the position
    // LocX, LocY.
    //
    virtual PUCHAR
    GetImageLocation (  
        ULONG LocX,
        ULONG LocY
        ) = 0;

    //
    // SetImageSize():
    //
    // Set the image size of the synthesis buffer.
    //
    void
    SetImageSize (
        ULONG Width,
        ULONG Height
        )
    {
        m_Width = Width;
        m_Height = Height;
    }

    //
    // SetBuffer():
    //
    // Set the buffer the synthesizer generates images to.
    //
    void
    SetBuffer (
        PUCHAR SynthesisBuffer
        )
    {
        m_SynthesisBuffer = SynthesisBuffer;
    }

    //
    // SynthesizeBars():
    //
    // Synthesize EIA-189-A standard color bars.
    //
    void
    SynthesizeBars (
        );

    void DrawFrame();

    //
    // OverlayText():
    //
    // Overlay a text string onto the image.
    //
    void
    OverlayText (
        _In_ ULONG LocX,
        _In_ ULONG LocY,
        _In_ ULONG Scaling,
        _In_ LPSTR Text
     
        );

    //
    // DEFAULT CONSTRUCTOR
    //
    CImageSynthesizer (
        ) :
        m_Width (0),
        m_Height (0),
        m_SynthesisBuffer (NULL)
    {
    }

    //
    // CONSTRUCTOR:
    //
    CImageSynthesizer (
        ULONG Width,
        ULONG Height
        ) :
        m_Width (Width),
        m_Height (Height),
        m_SynthesisBuffer (NULL)
    {
    }

    //
    // DESTRUCTOR:
    //
    virtual
    ~CImageSynthesizer (
        )
    {
    }


	void
		CopyBuffer(PVOID data, ULONG dataLength);
};

/*************************************************

    CRGB24Synthesizer

    Image synthesizer for RGB24 format.

*************************************************/

class CRGB24Synthesizer : public CImageSynthesizer {

private:


    BOOLEAN m_FlipVertical;

public:

    //
    // PutPixel():
    //
    // Place a pixel at a specific cursor location.  *ImageLocation must
    // reside within the synthesis buffer.
    //
    virtual void
        PutPixel(
            PUCHAR* ImageLocation,
            UCHAR color[3]
        )
    {
            *(*ImageLocation)++ = color[0];
            *(*ImageLocation)++ = color[1];
            *(*ImageLocation)++ = color[2];
        
    }

    //
    // PutPixel():
    //
    // Place a pixel at the default cursor location.  The cursor location
    // must be set via GetImageLocation(x, y).
    // 
    virtual void
    PutPixel (
        UCHAR color[3]
        )
    {
            *m_Cursor++ = color[0];
            *m_Cursor++ = color[1];
            *m_Cursor++ = color[2];
        
    }

    virtual long
    GetBytesPerPixel () 
    {
        return 3;
    }

    virtual PUCHAR
    GetImageLocation (
        ULONG LocX,
        ULONG LocY
        )
    {
        if (m_FlipVertical) {
            return (m_Cursor = 
                (m_SynthesisBuffer + 3 * 
                    (LocX + (m_Height - 1 - LocY) * m_Width))
                );
        } else {
            return (m_Cursor =
                (m_SynthesisBuffer + 3 * (LocX + LocY * m_Width))
                );
        }
    }

    //
    // DEFAULT CONSTRUCTOR:
    //
    CRGB24Synthesizer (
        BOOLEAN FlipVertical
        ) :
        m_FlipVertical (FlipVertical)
    {
    }

    //
    // CONSTRUCTOR:
    //
    CRGB24Synthesizer (
        BOOLEAN FlipVertical,
        ULONG Width,
        ULONG Height
        ) :
        CImageSynthesizer (Width, Height),
        m_FlipVertical (FlipVertical)
    {
    }

    //
    // DESTRUCTOR:
    //
    virtual
    ~CRGB24Synthesizer (
        )
    {
    }

};

/*************************************************

    CYUVSynthesizer

    Image synthesizer for YUV format.

*************************************************/

class CYUVSynthesizer : public CImageSynthesizer {

private:

    
    BOOLEAN m_Parity;

public:

    //
    // PutPixel():
    //
    // Place a pixel at a specific cursor location.  *ImageLocation must
    // reside within the synthesis buffer.
    //
    virtual void
    PutPixel (
        PUCHAR *ImageLocation,
        UCHAR color[3]
        )
    {

        BOOLEAN Parity = (((*ImageLocation - m_SynthesisBuffer) & 0x2) != 0);

        

        UCHAR Y = (color[0] / 4) +  (color[1] / 4) + (color[2] / 10) + 16;
        UCHAR U = (color[0]*(-1)/7) - (2* color[1] /7) +( 3 * color[2] /7) + 128;
        UCHAR V = (3* color[0] /7) - (5 * color[1] /14) - (color[2] / 13) + 128;

        UCHAR tranformColor[3] = { Y,U,V };

#if DBG
        //
        // Check that the current pixel points to a valid start pixel
        // in the UYVY buffer.
        //
        BOOLEAN Odd = (((*ImageLocation - m_SynthesisBuffer) & 0x1) != 0);
        NT_ASSERT ((m_Parity && Odd) || (!m_Parity && !Odd));
#endif // DBG

            
            if (Parity) {
                *(*ImageLocation)++ = tranformColor[2];
            } else {
                *(*ImageLocation)++ = tranformColor[1];
                *(*ImageLocation)++ = tranformColor[0];
                *(*ImageLocation)++ = tranformColor[1];
            }
       
    }

    //
    // PutPixel():
    //
    // Place a pixel at the default cursor location.  The cursor location
    // must be set via GetImageLocation(x, y).
    //
    virtual void
    PutPixel (
        UCHAR color[3]
        )

    {
        UCHAR Y = (color[0] / 4) + (color[1] / 4) + (color[2] / 10) + 16;
        UCHAR U = (color[0] * (-1) / 7) - (2 * color[1] / 7) + (3 * color[2] / 7) + 128;
        UCHAR V = (3 * color[0] / 7) - (5 * color[1] / 14) - (color[2] / 13) + 128;

        UCHAR tranformColor[3] = { Y,U,V };


        
            if (m_Parity) {
                *m_Cursor++ = tranformColor[2];
            } else {
                *m_Cursor++ = tranformColor[1];
                *m_Cursor++ = tranformColor[0];
                *m_Cursor++ = tranformColor[1];
            }
        
        m_Parity = !m_Parity;

    }

    virtual long
    GetBytesPerPixel () 
    {
        return 2;
    }

    virtual PUCHAR
    GetImageLocation (
        ULONG LocX,
        ULONG LocY
        )
    {
    
        m_Cursor = m_SynthesisBuffer + ((LocX + LocY * m_Width) << 1);
        if (m_Parity = ((LocX & 1) != 0)) 
            m_Cursor++;

        return m_Cursor;
    }

    //
    // DEFAULT CONSTRUCTOR:
    //
    CYUVSynthesizer (
        ) 
    {
    }

    //
    // CONSTRUCTOR:
    //
    CYUVSynthesizer (
        ULONG Width,
        ULONG Height
        ) :
        CImageSynthesizer (Width, Height)
    {
    }

    //
    // DESTRUCTOR:
    //
    virtual
    ~CYUVSynthesizer (
        )
    {
    }

};

