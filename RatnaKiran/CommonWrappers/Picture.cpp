#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SPicture::SPicture()
{
	m_pPicture = NULL;
}

SPicture::~SPicture()
{
	UnLoad();
}

BOOL SPicture::Load(CString sResourceType, CString sResource)
{
	BOOL bResult = FALSE;

	if(sResourceType.IsEmpty() || sResource.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return bResult;
	}

	if(m_pPicture != NULL) //already exists
		UnLoad();

	BYTE* pBuff = NULL;
	int nSize = 0;

	if(GetResource(sResource.GetBuffer(0), sResourceType.GetBuffer(0), pBuff, nSize))
	{
		if(nSize > 0)
		{
			SALIL_NEW(pBuff, BYTE[nSize]);

			if(GetResource(sResource.GetBuffer(0), sResourceType.GetBuffer(0), pBuff, nSize))
			{
				if(LoadFromBuffer(pBuff, nSize))
					bResult = TRUE;
				else
				{
					CString errstr = "Could not load the image resource '" + sResource + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
			{
				CString errstr = "Could not find the image resource '" + sResource + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}

			delete [] pBuff;
		}
	}
	else
	{
		CString errstr = "Could not find the image resource '" + sResource + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	return bResult;
}

BOOL SPicture::Load(CString sFileName)
{
	BOOL bResult = FALSE;

	if(sFileName.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return bResult;
	}

	if(m_pPicture != NULL) //already exists
		UnLoad();

	CFile cFile;
	CFileException e;
	if(!cFile.Open(sFileName, CFile::modeRead | CFile::typeBinary, &e))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.", e.m_cause);
		errstr = "Could not open image file '" + sFileName + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return bResult;
	}

	BYTE* pBuff = NULL;
	DWORD size = (DWORD)cFile.GetLength();
	SALIL_NEW(pBuff, BYTE[size]);
	DWORD readcount = 0;

	try
	{
		readcount = cFile.Read(pBuff, size);
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",readerror->m_cause);
		errstr = "Could not read from file '" + sFileName + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		delete [] pBuff;
		cFile.Close();
		return bResult;
	}

	if(readcount > 0)
	{
		if(LoadFromBuffer(pBuff, size))
			bResult = TRUE;
		else
		{
			CString errstr = "Could not load image resource in file '" + sFileName + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	}
	else
	{
		CString errstr = "Could not read data from image file '" + sFileName + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	delete [] pBuff;
	cFile.Close();
	return bResult;
}

void SPicture::UnLoad()
{
	if(m_pPicture != NULL)
	{
		m_pPicture->Release();
		m_pPicture = NULL;
	}
}

BOOL SPicture::Draw(CDC* pDC)
{
	if((m_pPicture != NULL) && (pDC != NULL))
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		int nWidth	= MulDiv(hmWidth,	pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
		int nHeight	= MulDiv(hmHeight,	pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);
		
		return Draw(pDC, 0, 0, nWidth, nHeight);
	}
	else
		SHOW_INTERNAL_ERROR;

	return FALSE;
}

BOOL SPicture::Draw(CDC* pDC, CPoint Pos)
{
	if((m_pPicture != NULL) && (pDC != NULL))
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		int nWidth	= MulDiv(hmWidth,	pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
		int nHeight	= MulDiv(hmHeight,	pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);
		
		return Draw(pDC, Pos.x, Pos.y, nWidth, nHeight);
	}
	else
		SHOW_INTERNAL_ERROR;

	return FALSE;
}

BOOL SPicture::Draw(CDC* pDC, CPoint Pos, CSize Size)
{
	if((m_pPicture != NULL) && (pDC != NULL))
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		int nWidth	= Size.cx;
		int nHeight	= Size.cy;
		
		return Draw(pDC, Pos.x, Pos.y, nWidth, nHeight);
	}
	else
		SHOW_INTERNAL_ERROR;

	return FALSE;
}

BOOL SPicture::Draw(CDC* pDC, CSize Size)
{
	if((m_pPicture != NULL) && (pDC != NULL))
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		int nWidth	= Size.cx;
		int nHeight	= Size.cy;
		
		return Draw(pDC, 0, 0, nWidth, nHeight);
	}
	else
		SHOW_INTERNAL_ERROR;

	return FALSE;
}

BOOL SPicture::Draw(CDC* pDC, double nSizeRatio)
{
	if((m_pPicture != NULL) && (pDC != NULL))
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		int nWidth	= int(MulDiv(hmWidth,	pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH) * nSizeRatio);
		int nHeight	= int(MulDiv(hmHeight,	pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH) * nSizeRatio);
		
		return Draw(pDC, 0, 0, nWidth, nHeight);
	}
	else
		SHOW_INTERNAL_ERROR;

	return FALSE;
}

BOOL SPicture::Draw(CDC* pDC, CPoint Pos, double nSizeRatio)
{
	if((m_pPicture != NULL) && (pDC != NULL))
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		int nWidth	= int(MulDiv(hmWidth,	pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH) * nSizeRatio);
		int nHeight	= int(MulDiv(hmHeight,	pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH) * nSizeRatio);
		
		return Draw(pDC, Pos.x, Pos.y, nWidth, nHeight);
	}
	else
		SHOW_INTERNAL_ERROR;

	return FALSE;
}

BOOL SPicture::Draw(CDC* pDC, int x, int y, int cx, int cy)
{
	if((m_pPicture != NULL) && (pDC != NULL))
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		HRESULT result = m_pPicture->Render(pDC->m_hDC, x, y, cx, cy, 0, hmHeight, hmWidth, -hmHeight, NULL);
		if(result == S_OK)
			return TRUE;
		else
		{
			CString errstr;
			errstr.Format("%l", result);
			errstr = "Could not render image onscreen. ERROR : #" + errstr + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	}
	else
		SHOW_INTERNAL_ERROR;

	return FALSE;
}

BOOL SPicture::LoadFromBuffer(BYTE* pBuff, int nSize)
{
	BOOL bResult = FALSE;
	if(!pBuff || (nSize < 0))
	{
		SHOW_INTERNAL_ERROR;
		return bResult;
	}

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
	if(!hGlobal)
	{
		SHOW_INTERNAL_ERROR_LAST("Could not allocation memory for image.");
		return bResult;
	}

	void* pData = GlobalLock(hGlobal);
	if(!pData)
	{
		SHOW_INTERNAL_ERROR_LAST("Could not lock memory access for image.");
		GlobalFree(hGlobal);
		return bResult;
	}

	memcpy(pData, pBuff, nSize);
	
	if(!GlobalUnlock(hGlobal))
	{
		if(GetLastError() != NO_ERROR)
			SHOW_INTERNAL_ERROR_LAST("Could not unlock memory access for image.");
	}

	IStream* pStream = NULL;
	HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
	if(hr == S_OK)
	{
		hr = OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&m_pPicture);
		if(hr == S_OK)
			bResult = TRUE;
		else
		{
			CString errstr;
			errstr.Format("%l", hr);
			errstr = "Could not load image content from stream. ERROR : #" + errstr + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	
		pStream->Release();
	}
	else
	{
		CString errstr;
		errstr.Format("%l", hr);
		errstr = "Could not create image data stream. ERROR : #" + errstr + ".";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	return bResult;
}

BOOL SPicture::GetResource(LPSTR lpName, LPSTR lpType, void* pResource, int& nBufSize)
{ 
	HRSRC hResInfo;
	HANDLE hRes;
	HMODULE hInst = NULL; 
	LPSTR lpRes = NULL; 
	int nLen = 0;
	BOOL bResult = FALSE;

	// Find the resource
	hResInfo = FindResource(hInst, lpName, lpType);
	if(hResInfo == NULL) 
	{
		CString errstr = lpName;
		errstr = "Could not find the resouce '" + errstr + "'.";
		SHOW_INTERNAL_ERROR_LAST(errstr);
		return FALSE;
	}

	// Load the resource
	hRes = LoadResource(hInst, hResInfo);
	if(hRes == NULL) 
	{
		CString errstr = lpName;
		errstr = "Could not load the resouce '" + errstr + "' into memory.";
		SHOW_INTERNAL_ERROR_LAST(errstr);
		return FALSE;
	}

	// Lock the resource
	lpRes = (char *)LockResource(hRes);
	if (lpRes != NULL)
	{ 
		if(pResource == NULL)
		{
			nBufSize = SizeofResource(hInst, hResInfo);
			bResult = TRUE;
		}
		else
		{
			if (nBufSize >= (int)SizeofResource(hInst, hResInfo))
			{
				memcpy(pResource, lpRes, nBufSize);
				bResult = TRUE;
			}
			else
			{
				CString errstr = lpName;
				errstr = "Not enough buffer space to copy the resouce '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}
		} 

		if(!UnlockResource(hRes))
		{
			CString errstr = lpName;
			errstr = "Could not unlock the resouce '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	}
	else
	{
		CString errstr = lpName;
		errstr = "Could not lock the resouce '" + errstr + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	// Free the resource
	if(!FreeResource(hRes))
	{
		CString errstr = lpName;
		errstr = "Could not free the resouce '" + errstr + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	return bResult;
}

CSize SPicture::GetSize(CDC* pDC)
{
	CSize rcResult = CSize(0,0);
	
	if((m_pPicture != NULL) && (pDC != NULL))
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		rcResult.cx	= MulDiv(hmWidth,	pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
		rcResult.cy	= MulDiv(hmHeight,	pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);
	}
	else
		SHOW_INTERNAL_ERROR;

	return rcResult;
}
