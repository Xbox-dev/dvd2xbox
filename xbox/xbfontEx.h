//-----------------------------------------------------------------------------
// Interface to the xbfontEx class
//-----------------------------------------------------------------------------
#ifndef XBFONT_H
#define XBFONT_H
#include "XBResource.h"
#include "XBUtil.h"




//-----------------------------------------------------------------------------
// Flags for the CXBFont::DrawText() function
//-----------------------------------------------------------------------------
#define XBFONT_LEFT       0x00000000
#define XBFONT_RIGHT      0x00000001
#define XBFONT_CENTER_X   0x00000002
#define XBFONT_CENTER_Y   0x00000004
#define XBFONT_TRUNCATED  0x00000008




//-----------------------------------------------------------------------------
// Name: struct GLYPH_ATTR
// Desc: Structure to hold information about one glyph (font character image)
//-----------------------------------------------------------------------------
struct GLYPH_ATTR
{
    FLOAT tu1, tv1, tu2, tv2; // Texture coordinates for the image
    SHORT wOffset;            // Pixel offset for glyph start
    SHORT wWidth;             // Pixel width of the glyph
    SHORT wAdvance;           // Pixels to advance after the glyph
};




//-----------------------------------------------------------------------------
// Name: class CXBFont
// Desc: Class to implement texture-based font rendering. A .tga image file of 
//       the pre-rendered font is used to create the texture. A .abc file
//       contains information for spacing the font characters (aka glyphs).
//-----------------------------------------------------------------------------
class CXBFont
{
public:
    // Font vertical dimensions
    FLOAT         m_fFontHeight;
    FLOAT         m_fFontTopPadding;
    FLOAT         m_fFontBottomPadding;
    FLOAT         m_fFontYAdvance;

    FLOAT         m_fXScaleFactor;
    FLOAT         m_fYScaleFactor;
    FLOAT         m_fSlantFactor;

    FLOAT         m_fCursorX;
    FLOAT         m_fCursorY;

    // Translator table for supporting unicode ranges
    WCHAR         m_cMaxGlyph;
    SHORT*        m_TranslatorTable;
    BOOL          m_bTranslatorTableWasAllocated;

    // Glyph data for the font
    DWORD         m_dwNumGlyphs;
    GLYPH_ATTR*   m_Glyphs;

    // Saved state for rendering (if not using a pure device)
    BOOL          m_bSaveState;
    DWORD         m_dwSavedState[16];

    // D3D rendering objects
    CXBPackedResource m_xprResource;
    D3DTexture*       m_pFontTexture;
    static DWORD      m_dwFontVertexShader;
    static DWORD      m_dwFontPixelShader;
    DWORD             m_dwNestedBeginCount;

    // Internal creation calls
    HRESULT     CreateShaders();
    
    // Access functions for debugging purposes
    D3DTexture* GetTexture() const    { return m_pFontTexture; }
    FLOAT       GetFontHeight() const { return m_fFontYAdvance; }

public:
    // Constructor/destructor
    CXBFont();
    ~CXBFont();

    // Functions to create and destroy the internal objects
    HRESULT Create( const CHAR* strFontResourceFileName, DWORD dwResourceOffset=0UL );
    HRESULT Create( D3DTexture* pFontTexture, VOID* pFontData );
    HRESULT Destroy();

    // Returns the dimensions of a text string
    HRESULT GetTextExtent( const WCHAR* strText, FLOAT* pWidth, 
                           FLOAT* pHeight, BOOL bFirstLineOnly=FALSE ) const;
    FLOAT   GetTextWidth( const WCHAR* strText ) const;

    VOID    SetCursorPosition( FLOAT fCursorX, FLOAT fCursorY );
    VOID    SetScaleFactors( FLOAT fXScaleFactor, FLOAT fYScaleFactor );
    VOID    SetSlantFactor( FLOAT fSlantFactor );

    // Function to create a texture containing rendered text
    D3DTexture* CreateTexture( const WCHAR* strText, 
                               D3DCOLOR dwBackgroundColor = 0x00000000,
                               D3DCOLOR dwTextColor = 0xffffffff,
                               D3DFORMAT d3dFormat = D3DFMT_LIN_A8R8G8B8 );

    // Public calls to render text. Callers can simply call DrawText(), but for
    // performance, they should batch multiple calls together, bracketed by 
    // calls to Begin() and End().
    HRESULT Begin();
    HRESULT DrawText( DWORD dwColor, const WCHAR* strText, DWORD dwFlags=0L,
                      FLOAT fMaxPixelWidth = 0.0f );
    HRESULT DrawText( FLOAT sx, FLOAT sy, DWORD dwColor, 
                      const WCHAR* strText, DWORD dwFlags=0L,
                      FLOAT fMaxPixelWidth = 0.0f );
	HRESULT DrawTextVertical( FLOAT sx, FLOAT sy, DWORD dwColor, 
                      const WCHAR* strText, DWORD dwFlags=0L,
                      FLOAT fMaxPixelWidth = 0.0f );
    HRESULT End();
};




//-----------------------------------------------------------------------------
// Defines for special characters. Note this depends on the font used, but all
// samples are coded to use the same basic font
//-----------------------------------------------------------------------------
#define GLYPH_A_BUTTON      L"\400"
#define GLYPH_B_BUTTON      L"\401"
#define GLYPH_X_BUTTON      L"\402"
#define GLYPH_Y_BUTTON      L"\403"
#define GLYPH_WHITE_BUTTON  L"\404"
#define GLYPH_BLACK_BUTTON  L"\405"
#define GLYPH_LEFT_BUTTON   L"\406"
#define GLYPH_RIGHT_BUTTON  L"\407"
#define GLYPH_START1_BUTTON L"\410"
#define GLYPH_START2_BUTTON L"\411"
#define GLYPH_BACK1_BUTTON  L"\412"
#define GLYPH_BACK2_BUTTON  L"\413"

#define GLYPH_LEFT_HAND     L"\414"
#define GLYPH_RIGHT_HAND    L"\415"
#define GLYPH_UP_HAND       L"\416"
#define GLYPH_DOWN_HAND     L"\417"

#define GLYPH_BULLET        L"\500"
#define GLYPH_STAR_1        L"\501"
#define GLYPH_STAR_2        L"\502"
#define GLYPH_STAR_3        L"\503"
#define GLYPH_STAR_4        L"\504"
#define GLYPH_STAR_5        L"\505"

#define GLYPH_BIG_X         L"\506"
#define GLYPH_HAND          L"\507"
#define GLYPH_CHECK_MARK    L"\510"
#define GLYPH_X_MARK        L"\511"
#define GLYPH_SKULL         L"\512"

#define GLYPH_LEFT_TICK     L"\514"
#define GLYPH_RIGHT_TICK    L"\515"
#define GLYPH_UP_TICK       L"\516"
#define GLYPH_DOWN_TICK     L"\517"

#define GLYPH_HOLLOW_CIRCLE L"\600"
#define GLYPH_FILLED_CIRCLE L"\601"

#define GLYPH_LR_ARROW      L"\612"
#define GLYPH_UD_ARROW      L"\613"
#define GLYPH_LEFT_ARROW    L"\614"
#define GLYPH_RIGHT_ARROW   L"\615"
#define GLYPH_UP_ARROW      L"\616"
#define GLYPH_DOWN_ARROW    L"\617"


#endif