#ifndef D2XACL
#define D2XACL

#include "d2xlogger.h"
#include "d2xutils.h"
#include "d2xfilecopy.h"
#include "d2xsettings.h"
#include <iosupport.h>
#include <string.h>
#include <list>
#include <XKEEPROM.h>
#include "d2xfilefactory.h"


#define ACL_UNKNOWN			0
#define ACL_HEXREPLACE		1
#define ACL_COPYFILES		2
#define ACL_DELFILES		3
#define ACL_SETMEDIA		4
#define ACL_MOVEFILES		5
#define ACL_FILENAMEREPLACE	6
#define ACL_APPLYPPF		7
#define ACL_SETREGION		8

#define ACL_PREPROCESS		100
#define ACL_POSTPROCESS		200

#define ACL_PATTERNS		  6
#define ACL_PATTERN_LENGTH 1024

class D2Xacl
{
protected:

	D2Xlogger		p_log;
	D2Xutils		p_util;
	char			m_destination[1024];	
	char			m_pattern[ACL_PATTERNS][ACL_PATTERN_LENGTH];
	char			m_currentmask[1024];
	ULONG			m_titleID;
	int				m_acltype;
	typedef vector <string>::iterator iXBElist;
	typedef map <string,string>::iterator iRENlist;
	

	void			reset();
	void			resetPattern();
	void			FillVars(char* pattern);
	void			HexReplace(const char* pattern, bool cache);
	void			FileNameReplace(const char* file,bool cache);
	bool			processSection(char* pattern);
	bool			PreprocessSection(char* pattern);
	bool			processFiles(char *path, bool rec);
	void			DelItem(char* item);
	int				ApplyPPF(char* file,char* ppf);

	
	
public:
	D2Xacl();
	~D2Xacl();

	bool processACL(char* dest,int state);
	
};

#endif