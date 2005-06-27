#include "D2Xmedialib.h"


D2Xmedialib::D2Xmedialib()
{
	p_Font = D2Xfont::Instance();
	p_tex  = D2Xtexture::Instance();
}

D2Xmedialib::~D2Xmedialib()
{

}

int D2Xmedialib::LoadMedia(CStdString& strSkindir)
{

	if(strSkindir[strSkindir.size()-1] != '\\')
		strSkindir.append("\\");

	if(LoadFonts(strSkindir) == 0)
		return 0;

	if(LoadBitmaps(strSkindir) == 0)
		return 0;
	
	return 1;
}

int D2Xmedialib::LoadFonts(CStdString& strSkindir)
{
	TiXmlElement*		itemElement;
	TiXmlNode*			fontNode;
	CStdString			strValue;

	strValue = strSkindir;
	strValue.append("fonts.xml");

	TiXmlDocument xmldoc( strValue );
	bool loadOkay = xmldoc.LoadFile();
	if ( !loadOkay )
		return 0;

	itemElement = xmldoc.RootElement();
	if( !itemElement )
		return 0;

	strValue = itemElement->Value();
	if (strValue != CStdString("fonts"))
		return 0;

	for( fontNode = itemElement->FirstChild( "font" );
	fontNode;
	fontNode = fontNode->NextSibling( "font" ) )
	{
		const TiXmlNode *pNode = fontNode->FirstChild("name");
		if (pNode)
		{
			CStdString strFontName = pNode->FirstChild()->Value();
			const TiXmlNode *pNode = fontNode->FirstChild("filename");
			if (pNode)
			{
				CStdString strFontFileName = strSkindir;
				strFontFileName.append("media\\");
				strFontFileName.append( pNode->FirstChild()->Value() );
				if (strstr(strFontFileName, ".xpr") != NULL)
				{
					p_Font->LoadFont(strFontFileName,strFontName);
				}
			}
		}
	}

	
	return 1;
}

void D2Xmedialib::DrawText( const CStdString name, FLOAT fX, FLOAT fY, DWORD dwColor, const CStdStringW& strText)
{
	p_Font->DrawText(  name, fX, fY, dwColor,  strText);
}

// Bitmaps

int D2Xmedialib::LoadBitmaps(CStdString& strSkindir)
{
	TiXmlElement*		itemElement;
	TiXmlNode*			bmpNode;
	CStdString			strValue;

	strValue = strSkindir;
	strValue.append("textures.xml");

	TiXmlDocument xmldoc( strValue );
	bool loadOkay = xmldoc.LoadFile();
	if ( !loadOkay )
		return 0;

	itemElement = xmldoc.RootElement();
	if( !itemElement )
		return 0;

	strValue = itemElement->Value();
	if (strValue != CStdString("textures"))
		return 0;

	for( bmpNode = itemElement->FirstChild( "texture" );
	bmpNode;
	bmpNode = bmpNode->NextSibling( "texture" ) )
	{
		const TiXmlNode *pNode = bmpNode->FirstChild("name");
		if (pNode)
		{
			CStdString strBMPName = pNode->FirstChild()->Value();
			const TiXmlNode *pNode = bmpNode->FirstChild("filename");
			if (pNode)
			{
				CStdString strBMPFileName = strSkindir;
				strBMPFileName.append("media\\");
				strBMPFileName.append( pNode->FirstChild()->Value() );

				const TiXmlNode *pNode = bmpNode->FirstChild("colorkey");
				
				DWORD ck = 0;
				if(pNode)
				{
					CStdString colorkey = pNode->FirstChild()->Value();
					sscanf( colorkey.c_str(),"%X",&ck);
				}

				
				p_tex->LoadTexture2(strBMPFileName,strBMPName, ck);
		
			}
		}
	}

	
	return 1;
}

void D2Xmedialib::RenderTexture2(CStdString name, FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	p_tex->RenderTexture2(name,  x,  y,  width,  height);
}