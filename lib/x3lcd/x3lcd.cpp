
//#include "../stdafx.h"
#include "x3lcd.h"
#include "../../dvd2xbox/d2xsettings.h"
//#include "../../utils/log.h"
#include <xtl.h>
#include "conio.h"
//#include "../../util.h"


char X3LcdAnimIndex=0;

//*************************************************************************************************************
static void outb(unsigned short port, unsigned char data)
{
	__asm
	{
		nop
			mov dx, port
			nop
			mov al, data
			nop
			out dx,al
			nop
			nop
	}
}

//*************************************************************************************************************
static unsigned char inb(unsigned short port)
{
	unsigned char data;
	__asm
	{
		mov dx, port
			in al, dx
			mov data,al
	}
	return data;
}

//*************************************************************************************************************
CX3LCD::CX3LCD()
{
	m_iActualpos=0;
	m_iRows    = 4;
	m_iColumns = 20;				
	m_iRow1adr = 0x00;
	m_iRow2adr = 0x40;
	m_iRow3adr = 0x14;
	m_iRow4adr = 0x54;
	m_iBackLight=32;
}

//*************************************************************************************************************
CX3LCD::~CX3LCD()
{
}

//*************************************************************************************************************
void CX3LCD::Initialize()
{
	StopThread();
	if (!g_d2xSettings.m_bLCDUsed) 
	{
    /*CLog::Log(LOGINFO, "lcd not used");*/
    return;
	}
	Create();
}

void CX3LCD::SetBackLight(int iLight)
{
	m_iBackLight=iLight;
}
void CX3LCD::SetContrast(int iContrast) { }
//*************************************************************************************************************
void CX3LCD::Stop()
{
	//if (!g_d2xSettings.m_bLCDUsed) 
		//return;
	StopThread();
}

//*************************************************************************************************************
void CX3LCD::SetLine(int iLine, const CStdString& strLine)
{
	if (!g_d2xSettings.m_bLCDUsed) 
		return;
	if (iLine < 0 || iLine >= (int)m_iRows) 
		return;

	CStdString strLineLong=strLine;
	strLineLong.Trim();
	StringToLCDCharSet(strLineLong);

	while (strLineLong.size() < m_iColumns) 
		strLineLong+=" ";
	if (strLineLong != m_strLine[iLine])
	{
		m_bUpdate[iLine] = true;
		m_strLine[iLine] = strLineLong;
		m_event.Set();
	}
}


//************************************************************************************************************************
// wait_us: delay routine
// Input: (wait in ~us)
//************************************************************************************************************************
void CX3LCD::wait_us(unsigned int value) 
{
	// 1 us = 1000msec
	int iValue = value / 30;
	if (iValue>10) 
		iValue = 10;
	if (iValue) 
		Sleep(iValue);
}


//************************************************************************************************************************
// DisplayOut: writes command or datas to display
// Input: (Value to write, token as CMD = Command / DAT = DATAs / INI for switching to 4 bit mode)
//************************************************************************************************************************
void CX3LCD::DisplayOut(unsigned char data, unsigned char command) 
{
	unsigned char cmd = 0;
	static DWORD	dwTick=0;

	if ((GetTickCount()-dwTick) < 3)
	{
		Sleep(1);
	}
	dwTick=GetTickCount();

	if (command & DAT)
	{
		cmd |= DISPCON_RS;
	}

	//outbut higher nibble
	
	outb(DISP_O_DAT, data & 0xF0);			// set Data high nibble
	outb(DISP_O_CMD, cmd);					// set RS if needed
	outb(DISP_O_CMD, DISPCON_E | cmd);		// set E
	outb(DISP_O_CMD, cmd);					// reset E

	if ((command & INI) == 0) 
	{							
		// if it's not the init command, do second nibble
		//outbut lower nibble
		outb(DISP_O_DAT, (data << 4) & 0xF0);		// set Data low nibble
		outb(DISP_O_CMD, cmd);				// set RS if needed
		outb(DISP_O_CMD, DISPCON_E | cmd);	// set E
		outb(DISP_O_CMD, cmd);				// reset E
		if ((data & 0xFC) == 0) 
		{						                          
			// if command was a Clear Display
			// or Cursor Home, wait at least 1.5 ms more
			m_iActualpos = 0;
			Sleep(2);							                                   
		}
		if ((command & DAT) == 0x02)
			m_iActualpos++;
	}
	else 
	{										
		m_iActualpos = 0;
		//wait_us(2500);		// wait 2.5 msec	
	}

	wait_us(30);
}

//************************************************************************************************************************
//DisplayBuildCustomChars: load customized characters to character ram of display, resets cursor to pos 0
//************************************************************************************************************************
void CX3LCD::DisplayBuildCustomChars() 
{
	int I;

	static char Bar0[] ={0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
	static char Bar1[] ={0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18};
	static char Bar2[] ={0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c};
	static char REW[8][8]=
	{
		{0x00, 0x05, 0x0a, 0x14, 0x0a, 0x05, 0x00, 0x00},
		{0x00, 0x0a, 0x14, 0x08, 0x14, 0x0a, 0x00, 0x00},
		{0x00, 0x14, 0x08, 0x10, 0x08, 0x14, 0x00, 0x00},
		{0x00, 0x08, 0x10, 0x00, 0x10, 0x08, 0x00, 0x00},
		{0x00, 0x10, 0x00, 0x01, 0x00, 0x10, 0x00, 0x00},
		{0x00, 0x00, 0x01, 0x02, 0x01, 0x00, 0x00, 0x00},
		{0x00, 0x01, 0x02, 0x05, 0x02, 0x01, 0x00, 0x00},
		{0x00, 0x02, 0x05, 0x0a, 0x05, 0x02, 0x00, 0x00}

	};
	static char FF[8][8]=
	{
		{0x00, 0x14, 0x0a, 0x05, 0x0a, 0x14, 0x00, 0x00},
		{0x00, 0x0a, 0x05, 0x02, 0x05, 0x0a, 0x00, 0x00},
		{0x00, 0x05, 0x02, 0x01, 0x02, 0x05, 0x00, 0x00},
		{0x00, 0x02, 0x01, 0x00, 0x01, 0x02, 0x00, 0x00},
		{0x00, 0x01, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00},
		{0x00, 0x00, 0x10, 0x08, 0x10, 0x00, 0x00, 0x00},
		{0x00, 0x10, 0x08, 0x14, 0x08, 0x10, 0x00, 0x00},
		{0x00, 0x08, 0x14, 0x0a, 0x14, 0x08, 0x00, 0x00}
	};
	static char Play[] ={0x00, 0x04, 0x06, 0x07, 0x07, 0x06, 0x04, 0x00};
	static char Stop[] ={0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00};
	static char Pause[]={0x00, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x00};
	static char Backslash[]={0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00};
	static char Underscore[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F};

	DisplayOut(DISP_CGRAM_SET, CMD);
	for(I=0;I<8;I++) DisplayOut(Bar0[I], DAT);  			// Bar0
	for(I=0;I<8;I++) DisplayOut(Bar1[I], DAT);  			// Bar1
	for(I=0;I<8;I++) DisplayOut(Bar2[I], DAT);  			// Bar2
	for(I=0;I<8;I++) DisplayOut(REW[X3LcdAnimIndex][I], DAT);   	// REW
	for(I=0;I<8;I++) DisplayOut(FF[X3LcdAnimIndex][I], DAT);    	// FF
	for(I=0;I<8;I++) DisplayOut(Play[I], DAT);  			// Play
	for(I=0;I<8;I++) DisplayOut(Stop[I], DAT);  			// Stop
	for(I=0;I<8;I++) DisplayOut(Pause[I], DAT); 			// Pause

	// didn't find it on my LCD :-)
	//for(I=0;I<8;I++) DisplayOut(Underscore[I], DAT);		// Underscore
	//for(I=0;I<8;I++) DisplayOut(Backslash[I], DAT);			// Backslash
	DisplayOut(DISP_DDRAM_SET, CMD);
	X3LcdAnimIndex=(X3LcdAnimIndex+1) & 0x7;
}


//************************************************************************************************************************
// DisplaySetPos: sets cursor position
// Input: (row position, line number from 0 to 3)
//************************************************************************************************************************
void CX3LCD::DisplaySetPos(unsigned char pos, unsigned char line) 
{

	unsigned int cursorpointer;

	cursorpointer = pos % m_iColumns;

	if (line == 0) {
		cursorpointer += m_iRow1adr;
	}
	if (line == 1) {
		cursorpointer += m_iRow2adr;
	}
	if (line == 2) {
		cursorpointer += m_iRow3adr;
	}
	if (line == 3) {
		cursorpointer += m_iRow4adr;
	}
	DisplayOut(DISP_DDRAM_SET | cursorpointer, CMD);
	m_iActualpos = cursorpointer;
}

//************************************************************************************************************************
// DisplayWriteFixText: write a fixed text to actual cursor position
// Input: ("fixed text like")
//************************************************************************************************************************
void CX3LCD::DisplayWriteFixtext(const char *textstring)
{
	unsigned char  c;
	while (c = *textstring++) {
		DisplayOut(c,DAT);
	}
}


//************************************************************************************************************************
// DisplayWriteString: write a string to acutal cursor position 
// Input: (pointer to a 0x00 terminated string)
//************************************************************************************************************************

void CX3LCD::DisplayWriteString(char *pointer) 
{
	/* display a normal 0x00 terminated string on the LCD display */
	unsigned char c;
	do {
		c = *pointer;
		if (c == 0x00) 
			break;

		DisplayOut(c, DAT);
		*pointer++;
	} while(1);

}		


//************************************************************************************************************************
// DisplayClearChars:  clears a number of chars in a line and resets cursor position to it's startposition
// Input: (Startposition of clear in row, row number, number of chars to clear)
//************************************************************************************************************************
void CX3LCD::DisplayClearChars(unsigned char startpos , unsigned char line, unsigned char lenght) 
{
	int i;

	DisplaySetPos(startpos,line);
	for (i=0;i<lenght; i++){
		DisplayOut(0x20,DAT);
	}
	DisplaySetPos(startpos,line);

}


//************************************************************************************************************************
// DisplayProgressBar: shows a grafic bar staring at actual cursor position
// Input: (percent of bar to display, lenght of whole bar in chars when 100 %)
//************************************************************************************************************************
void CX3LCD::DisplayProgressBar(unsigned char percent, unsigned char charcnt) 
{

	unsigned int barcnt100;

	barcnt100 = charcnt * 5 * percent / 100;

	int i;	
	for (i=1; i<=charcnt; i++) {
		if (i<=(int)(barcnt100 / 5)){
			DisplayOut(4,DAT);
		}
		else 
		{
			if ( (i == (barcnt100 /5)+1) && (barcnt100 % 5 != 0) ){
				DisplayOut((barcnt100 % 5)-1,DAT);
			}
			else{
				DisplayOut(0x20,DAT);
			}
		}
	}
}
//************************************************************************************************************************
//Set brightness level 
//************************************************************************************************************************
void CX3LCD::DisplaySetBacklight(unsigned char level) 
{
	outb(DISP_O_LIGHT, (int)(2.55*(double)level) );
}
//************************************************************************************************************************
void CX3LCD::DisplayInit()
{
	//initialize GP/IO
	outb(DISP_O_DAT, 0);
	outb(DISP_O_CMD, 0);
	outb(DISP_O_DIR_DAT, 0xFF);
	outb(DISP_O_DIR_CMD, 0x07);

	DisplayOut(DISP_FUNCTION_SET | DISP_DL_FLAG, INI);	// 8-Bit Datalength if display is already initialized to 4 bit
	Sleep(5);
	DisplayOut(DISP_FUNCTION_SET | DISP_DL_FLAG, INI);	// 8-Bit Datalength if display is already initialized to 4 bit
	Sleep(5);
	DisplayOut(DISP_FUNCTION_SET | DISP_DL_FLAG, INI);	// 8-Bit Datalength if display is already initialized to 4 bit
	Sleep(5);
	DisplayOut(DISP_FUNCTION_SET, INI);													// 4-Bit Datalength
	Sleep(5);
	DisplayOut(DISP_FUNCTION_SET | DISP_N_FLAG ,CMD);	// 4-Bit Datalength, 2 Lines, Font 5x7, clear RE Flag
	Sleep(5);
	DisplayOut(DISP_CONTROL | DISP_D_FLAG ,CMD);		// Display on
	Sleep(5);
	DisplayOut(DISP_CLEAR, CMD);				// Display clear
	Sleep(5);
	DisplayOut(DISP_ENTRY_MODE_SET | DISP_ID_FLAG,CMD);	// Cursor autoincrement
	Sleep(5);

	DisplayBuildCustomChars();				
	DisplaySetPos(0,0);
}


//************************************************************************************************************************
void CX3LCD::Process()
{
	int iOldLight=-1;  

	m_iColumns = g_d2xSettings.m_iLCDColumns;
	m_iRows    = g_d2xSettings.m_iLCDRows;
	m_iRow1adr = g_d2xSettings.m_iLCDAdress[0];
	m_iRow2adr = g_d2xSettings.m_iLCDAdress[1];
	m_iRow3adr = g_d2xSettings.m_iLCDAdress[2];
	m_iRow4adr = g_d2xSettings.m_iLCDAdress[3];
	m_iBackLight= g_d2xSettings.m_iLCDBackLight;
	if (m_iRows >= MAX_ROWS) 
		m_iRows = MAX_ROWS - 1;

	DisplayInit();
	while (!m_bStop)
	{
		Sleep(SCROLL_SPEED_IN_MSEC);  
		if (m_iBackLight != iOldLight)
		{
			// backlight setting changed
			iOldLight=m_iBackLight;
			DisplaySetBacklight(m_iBackLight);
		}
		DisplayBuildCustomChars();
		for (int iLine = 0; iLine < (int)m_iRows; ++iLine)
		{
			if (m_bUpdate[iLine])
			{
				CStdString strTmp = m_strLine[iLine];
				if (strTmp.size() > m_iColumns)
				{
					strTmp=m_strLine[iLine].Left(m_iColumns);
				}
				m_iPos[iLine]=0;
				DisplaySetPos(0, iLine);
				DisplayWriteFixtext(strTmp.c_str());
				m_bUpdate[iLine] = false;
				m_dwSleep[iLine] = GetTickCount();
			}
			else if ( g_d2xSettings.enableLCDScrolling && ((GetTickCount()-m_dwSleep[iLine]) > 1000))
			{
				int iSize = m_strLine[iLine].size();
				if (iSize > (int)m_iColumns)
				{
					//scroll line
					CStdString strRow = m_strLine[iLine]+"   -   ";
					int iSize = strRow.size();
					m_iPos[iLine]++;
					if (m_iPos[iLine] >= iSize) 
						m_iPos[iLine] = 0;
					int iPos = m_iPos[iLine];
					CStdString strLine = "";
					for (int iCol = 0; iCol < (int)m_iColumns; ++iCol)
					{
						strLine += strRow.GetAt(iPos);
						iPos++;
						if (iPos >= iSize) 
							iPos = 0;
					}
					DisplaySetPos(0, iLine);
					DisplayWriteFixtext(strLine.c_str());
				}
			}
		}
	}
	for (int i = 0; i < (int)m_iRows; i++)
	{
		DisplayClearChars(0, i, m_iColumns);
	} 
	DisplayOut(DISP_CONTROL, CMD);		                  // Display off
}