#ifndef D2XGUISETTINGS
#define D2XGUISETTINGS

#include <xtl.h>
#include <xbApplicationEx.h>
#include <xbfontEx.h>
#include <stdstring.h>
#include <vector>
#include <map>
#include "d2xsettings.h"
#include "d2xinput.h"
#include "d2xgraphics.h"
#include "d2xutils.h" 
#include "d2xmedialib.h"
#include "d2xxbautodetect.h"

#define D2X_CONFIG_FILE		"e:\\TDATA\\0FACFAC0\\settings.d2x"
#define	D2X_SET_STRING		10
#define	D2X_SET_INT			20
#define	D2X_SET_HEX			30

#define D2X_GUI_PROCESS		100
#define D2X_GUI_BACK		110
#define D2X_GUI_MAPDRIVES	120
#define	D2X_GUI_START_NET	130
#define	D2X_GUI_STOP_NET	140
#define	D2X_GUI_START_LCD	150
#define	D2X_GUI_STOP_LCD	160
#define	D2X_GUI_START_FTPD	170
#define	D2X_GUI_STOP_FTPD	180
#define D2X_GUI_SET_LCD		190		
#define D2X_GUI_RESTART_LCD	200
#define D2X_GUI_RESTART		210
#define D2X_GUI_SAVE_SKIN	220
#define D2X_GUI_START_MEDIAD 230
#define D2X_GUI_STOP_MEDIAD	 240
#define D2X_GUI_CALIBRATION	 250
#define D2X_GUI_START_RGB	260
#define D2X_GUI_STOP_RGB	270

#define TEXT_COLOR_MAIN			COLOUR_WHITE
#define HIGHLITE_COLOR_MAIN		COLOUR_WHITE
#define TEXT_COLOR_SUB			COLOUR_WHITE
#define HIGHLITE_COLOR_SUB		COLOUR_WHITE

#define	START_X_MAIN			250
#define START_Y_MAIN			100
#define	START_X_SUB				200
#define START_Y_SUB				100
#define SPACE_X_SUB				200

// stupid: values are linked to showIDs :-(
#define SET_GENERAL			 1
#define	SET_AUDIO			 2
#define SET_LCD				 3
#define SET_NETWORK			 4
#define SET_SKINS			 5
#define SET_GUI				 6
//#define SET_CALIBRATE		 7
#define SET_RESTORE			 7

// workaround for showIDs
static int ref_showid[]={	0, // unused
							1, // main
							2, // general
							3,4,5,6,7};


class D2Xguiset
{
protected:

	struct GUISETITEM
	{
		int	type;
		CStdStringW label;
		bool active;
		int index;
		vector <int> v_int;
		vector <CStdStringW> values;
	};

	struct GUISETMENU
	{
		CStdStringW label;
		bool active;
		int index;
		int elements;
		map <int,GUISETITEM> items; 
	};

	struct SELECTED
	{
		CStdStringW		menulabel;
		int				menuID;
		CStdStringW		itemlabel;
		int				itemID;
		int				value_index;
		CStdStringW		value_label;
		int				items;
		int				showID;
	};


	map<int,GUISETMENU>	SetMenu;
	SELECTED			s_item;
	D2Xinput*			p_input;
	D2Xgraphics			p_graph;
	D2Xutils			p_utils;
	D2Xmedialib*		p_ml;
	D2Xxbautodetect*	p_xbad;


	float				gs_x;
	float				gs_y;
	int					i_vspace;
	unsigned short		start_x;
	unsigned short		start_y;
	unsigned short		v_space;

	void BuildMenu();
	void AnnounceSettings();
	int	 ExecuteSettings();
	void SetItemByIndex(int menuid, int itemid, int index);
	int  GetIndexByItem(int menuid, int itemid);
	int  GetIntValueByItem(int menuid, int itemid);
	void SetStatus(int menuid, int itemid, bool status);
	bool GetStatus(int menuid, int itemid);
	bool AddMenu(int menuID, CStdString label, bool active);
	bool AddInt(int menuID, int itemID, CStdString label, bool active, int default_value, int min, int max, int step);
	bool AddHex(int menuID, int itemID, CStdString label, bool active, int default_value, int min, int max, int step);
	bool AddString(int menuID, int itemID, CStdString label, bool active, int index, CStdString value);

	bool getSkins(vector<CStdString>& v_skins); 

	// browser
	int					cbrowse;

	vector<CStdString>		v_skins;
	int						i_skin;

public:
	D2Xguiset();
	~D2Xguiset();

	
	bool	SaveConfig();
	bool	LoadConfig();
	int		Process(XBGAMEPAD* pad,XBIR_REMOTE* ir);
	//void	ShowGUISettings(CXBFont &fontb, CXBFont &fonts);
	void	ShowGUISettings2(float x,float y,int hspace,int width,int widthpx, int vspace,DWORD fc,DWORD hlfc,const CStdString& font, DWORD dwFlags=0L, bool scroll=false);
	void	CheckingPartitions();
	int		getShowID();
	void	getXY(float* posX, float* posY);
	void	getOrigin(float* posX, float* posY);
	int		getItems(int* vspace=NULL);
	void StartAutoDetect();
	void StopAutoDetect();
};

#endif