#include "d2xviewer.h"




D2Xviewer::D2Xviewer()
{

	reset();
}

D2Xviewer::~D2Xviewer()
{
	reset();
}

void D2Xviewer::reset()
{
	c_view.buf.clear();
	c_view.browse = 1;
	c_view.yoffset = 0;
	c_view.xoffset = 0;
	c_view.lines = 0;
	c_view.file.clear();
	c_view.chars = 0;
	c_view.max_chars = 0;
}

void D2Xviewer::init(char* filename,int lines,int chars)
{
	reset();
	c_view.file = string(filename);
	c_view.file_stripped = string(strrchr(filename,'\\'));
	c_view.lines = lines;
	c_view.chars = chars;
	fillBuffer();
}


int D2Xviewer::fillBuffer()
{
	FILE* stream;
	if(c_view.file.empty())
		return VIEW_ERROR;
	stream = fopen(c_view.file.c_str(),"rt");
	if(stream == NULL)
		return VIEW_ERROR;

	char buffer[1024];
	memset(buffer,'0',1024);
	c_view.buf.clear();

	while((fgets(buffer,1024,stream) != NULL))
	{
		c_view.max_chars = __max(strlen(buffer),c_view.max_chars);
		c_view.buf.push_back(string(buffer));
		memset(buffer,'0',1024);
	}
	fclose(stream);
	return VIEW_OK;
}

void D2Xviewer::process(XBGAMEPAD gp)
{
	if((gp.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP)) 
	{
		if((c_view.browse == 1) && (c_view.yoffset > 0))
			c_view.yoffset--;
		if(c_view.browse > 1)
			c_view.browse--;

	}
	if((gp.fY1 > 0.5)) 
	{
		Sleep(100);
		if((c_view.browse == 1) && (c_view.yoffset > 0))
			c_view.yoffset--;
		if(c_view.browse > 1)
			c_view.browse--;
	
	}
	if((gp.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN)) 
	{
		if((c_view.browse == c_view.lines) && (c_view.yoffset < (c_view.buf.size()-c_view.lines)))
			c_view.yoffset++;
		if((c_view.browse < c_view.lines) && (c_view.browse < c_view.buf.size()))
			c_view.browse++;
		
	}
	if(gp.fY1 < -0.5) 
	{
		Sleep(100);
		if((c_view.browse == c_view.lines) && (c_view.yoffset < (c_view.buf.size()-c_view.lines)))
			c_view.yoffset++;
		if((c_view.browse < c_view.lines) && (c_view.browse < c_view.buf.size()))
			c_view.browse++;
	}

	if((gp.wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT)) 
	{
		if(c_view.xoffset > 0)
			c_view.xoffset--;
	}
	if(gp.fX1 < -0.5) 
	{
		Sleep(100);
		if(c_view.xoffset > 0)
			c_view.xoffset--;
	}

	if((gp.wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT)) 
	{
		if(c_view.xoffset < (c_view.max_chars-c_view.chars))
			c_view.xoffset++;
	}
	if(gp.fX1 > 0.5) 
	{
		Sleep(100);
		if(c_view.xoffset < (c_view.max_chars-c_view.chars))
			c_view.xoffset++;
	}
}

void D2Xviewer::show(float x,float y,DWORD fc,DWORD hlfc, CXBFont &font)
{
	int line = 1;
	float tmpy = 0;
	int left = 0;
	int right = 0;
	WCHAR text[128];
	D2Xgraphics	p_graph;
	wsprintfW(text,L"Row (%i/%i)",c_view.browse+c_view.yoffset,c_view.buf.size());
	font.DrawText( x, 30, 0xffff0000, text );
	wsprintfW(text,L"Column (%i/%i)",c_view.xoffset+1,c_view.max_chars-c_view.chars+1);
	font.DrawText( x, 30+font.m_fFontHeight, 0xffff0000, text );
	for(int i=c_view.yoffset;i < (c_view.yoffset+c_view.lines);i++)
	{
		if(i >= c_view.buf.size())
			break;
		left = __min(c_view.xoffset,c_view.buf[i].size());
		right = __min(c_view.chars,c_view.buf[i].size()-left);
		wsprintfW(text,L"%hs",c_view.buf[i].substr(left,right).c_str()); 
		if(line == c_view.browse)
			p_graph.RenderBar(x,y+tmpy,font.m_fFontHeight,530); 

        font.DrawText( x, y+tmpy, fc,text  ); 
		tmpy = line*font.m_fFontHeight;
		line++;
	}
	wsprintfW(text,L"%hs",c_view.file_stripped.c_str()); 
	font.DrawText( x, 435, 0xffff0000, text );
}