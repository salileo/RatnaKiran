#ifndef _common_h_
#define _common_h_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcview.h>
#include <afxtempl.h>

#include <math.h>
#include "Picture.h"
#include "ProgressDialog.h"

#define BEGIN_WAIT \
{\
	AfxGetApp()->BeginWaitCursor();\
}


#define END_WAIT \
{\
	AfxGetApp()->EndWaitCursor();\
}

extern BOOL gShowErrors;

#define SHOW_ERROR(x) \
{\
	CWnd *_error_frame = AfxGetMainWnd();\
	CString _error_reason = x;\
	if(_error_reason.IsEmpty())\
	{\
		_error_reason = "Some error occured";\
	}\
	HWND _error_window = NULL;\
	if(_error_frame != NULL)\
	{\
		_error_window = _error_frame->m_hWnd;\
	}\
	::MessageBox(_error_window, _error_reason, "Error", MB_OK | MB_ICONERROR);\
}

#define SHOW_ERROR_YESNO(x,y) \
{\
	CWnd *_error_frame = AfxGetMainWnd();\
	CString _error_reason = x;\
	if(_error_reason.IsEmpty())\
	{\
		_error_reason = "Some error occured";\
	}\
	HWND _error_window = NULL;\
	if(_error_frame != NULL)\
	{\
		_error_window = _error_frame->m_hWnd;\
	}\
	y = ::MessageBox(_error_window, _error_reason, "Error", MB_YESNO | MB_ICONERROR);\
}

#define SHOW_WARNING(x) \
{\
	CWnd *_error_frame = AfxGetMainWnd();\
	CString _error_reason = x;\
	if(_error_reason.IsEmpty())\
	{\
		_error_reason = "Some error occured";\
	}\
	HWND _error_window = NULL;\
	if(_error_frame != NULL)\
	{\
		_error_window = _error_frame->m_hWnd;\
	}\
	::MessageBox(_error_window, _error_reason, "Warning", MB_OK | MB_ICONWARNING);\
}

#define SHOW_WARNING_YESNO(x,y) \
{\
	CWnd *_error_frame = AfxGetMainWnd();\
	CString _error_reason = x;\
	if(_error_reason.IsEmpty())\
	{\
		_error_reason = "Some error occured";\
	}\
	HWND _error_window = NULL;\
	if(_error_frame != NULL)\
	{\
		_error_window = _error_frame->m_hWnd;\
	}\
	y = ::MessageBox(_error_window, _error_reason, "Warning", MB_YESNO | MB_ICONWARNING);\
}

#define SHOW_INFO(x) \
{\
	CWnd *_error_frame = AfxGetMainWnd();\
	CString _error_reason = x;\
	if(_error_reason.IsEmpty())\
	{\
		_error_reason = "Some error occured";\
	}\
	HWND _error_window = NULL;\
	if(_error_frame != NULL)\
	{\
		_error_window = _error_frame->m_hWnd;\
	}\
	::MessageBox(_error_window, _error_reason, "", MB_OK | MB_ICONINFORMATION);\
}

#define SHOW_INFO_YESNO(x,y) \
{\
	CWnd *_error_frame = AfxGetMainWnd();\
	CString _error_reason = x;\
	if(_error_reason.IsEmpty())\
	{\
		_error_reason = "Some error occured";\
	}\
	HWND _error_window = NULL;\
	if(_error_frame != NULL)\
	{\
		_error_window = _error_frame->m_hWnd;\
	}\
	y = ::MessageBox(_error_window, _error_reason, "", MB_YESNO | MB_ICONINFORMATION);\
}

#define SHOW_INTERNAL_ERROR_REASON(x) \
{\
	CString _error_message_internal;\
	CString _error_reason = x;\
	_error_message_internal.Format("An internal error occured at the following location. Please contact the creator for assistance immediately. \n%s::%d", __FILE__, __LINE__);\
	if(!_error_reason.IsEmpty())\
	{\
		_error_message_internal = _error_message_internal + "\n\nReason :\n" + _error_reason;\
	}\
	SHOW_ERROR(_error_message_internal);\
}

#define SHOW_INTERNAL_ERROR \
{\
	SHOW_INTERNAL_ERROR_REASON("");\
}

#define SHOW_INTERNAL_ERROR_LAST(x) \
{\
	LPVOID _error_lpMsgBuf;\
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &_error_lpMsgBuf, 0, NULL);\
	CString _error_last = (LPCTSTR)_error_lpMsgBuf;\
	_error_last = x + CString("...\nERROR : ") + _error_last;\
	SHOW_INTERNAL_ERROR_REASON(_error_last);\
}


#define SALIL_NEW(var_ptr,type) \
{\
	try\
	{\
		var_ptr = new type;\
		if(!var_ptr)\
		{\
			SHOW_INTERNAL_ERROR_REASON("Out of Memory");\
			exit(1);\
		}\
	}\
	catch(CMemoryException *memerror)\
	{\
		memerror = NULL;\
		SHOW_INTERNAL_ERROR_REASON("Out of Memory");\
		exit(1);\
	}\
}

#define GET_SET_BOOL(x) \
BOOL Get##x()\
{\
	return m_##x;\
}\
void Set##x(BOOL val)\
{\
	m_##x = val;\
}

#define GET_SET_INT(x) \
int Get##x()\
{\
	return m_##x;\
}\
void Set##x(int val)\
{\
	m_##x = val;\
}

#define GET_SET_DOUBLE(x) \
double Get##x()\
{\
	return m_##x;\
}\
void Set##x(double val)\
{\
	m_##x = val;\
}

#define GET_SET_CHAR(x) \
char *Get##x()\
{\
	return m_##x;\
}\
void Set##x(char *val)\
{\
	if(val)\
		strcpy(m_##x, val);\
	else\
		strcpy(m_##x, "");\
}

#define ENCODE_DATA 1

#if ENCODE_DATA

#define ENCODE(var_ptr,type) \
{\
	int array_size = sizeof(type) / sizeof(char);\
	char *data = (char *)(var_ptr);\
	int index = 0;\
	while(index < array_size)\
	{\
		char temp = ~data[index];\
		data[index] = temp;\
		index++;\
	}\
}

#define DECODE(var_ptr,type) \
{\
	int array_size = sizeof(type) / sizeof(char);\
	char *data = (char *)(var_ptr);\
	int index = 0;\
	while(index < array_size)\
	{\
		char temp = ~data[index];\
		data[index] = temp;\
		index++;\
	}\
}

#else //ENCODE_DATA

#define ENCODE(a, b)
#define DECODE(a, b)

#endif //ENCODE_DATA

CString GetBaseName(CString file);
CString GetDirName(CString file);
BOOL MoveDatabaseFile(CString from, CString to);
BOOL IsFileExisting(CString filename);
CString GetFileName(char *filtername, char *filtervalue, char *defaultextension, BOOL forOpen);
BOOL IsNumber(CString str);
BOOL IsGreaterString(CString str1, CString str2);
int GetCharacterNumber(char ch);
int FindMenuItem(CMenu* menu, CString menustring);
double RoundOff(double val, int decimal_places);

#endif