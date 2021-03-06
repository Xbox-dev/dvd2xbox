#ifndef D2XFFFTP
#define D2XFFFTP


#include "d2xfile.h"
#include "..\..\lib\libftpc\ftplib.h"
#include <StringUtils.h>
#include <stdstring.h>
#include "..\d2xutils.h"



class D2XfileFTP : public D2Xfile
{
protected:
	
	bool	Connect();
	void	FormPath(char* path, char* ret_path);
	ftplib*	p_ftplib;
	/*static	ftplib*	p_ftplib;*/
	static char	startpwd[128];
	DWORD file_size;
	netbuf *nData;
	char	fileopen[1024];

	int		RmDir(char* path);

public:
	D2XfileFTP();
	virtual ~D2XfileFTP();
	
	virtual int FileOpenWrite(char* filename, int mode=OPEN_MODE_NORMAL, DWORD size=NULL);
	virtual int FileOpenRead(char* filename, int mode=OPEN_MODE_NORMAL);
	virtual int FileWrite(LPCVOID buffer,DWORD dwrite,DWORD *dwrote);
	virtual int FileRead(LPVOID buffer,DWORD dwToRead,DWORD *dwRead);
	virtual int FileClose();
	virtual DWORD GetFileSize(char* filename=NULL);
	virtual int GetDirectory(char* path,VECFILEITEMS *items);
	virtual int CreateDirectory(char* name);

	virtual int DUMMYFileRead(LPVOID buffer,DWORD dwToRead,DWORD *dwRead){return 0;};

	virtual __int64 FileSeek(long offset, int origin){return 0;};
	virtual int DeleteFile(char* filename);
	virtual int DeleteDirectory(char* filename);
	virtual int MoveItem(char* source, char* dest);
	virtual int GetType();
};

#endif