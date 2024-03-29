#if !defined(AFX_PICTURE_H__6098A4C3_D6D5_4711_BC7B_1595F459B480__INCLUDED_)
#define AFX_PICTURE_H__6098A4C3_D6D5_4711_BC7B_1595F459B480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRecordPicture  
{
public:
	CRecordPicture();
	virtual ~CRecordPicture();

	bool			Load(CString sResourceType, CString sResource);
	bool			Load(CString sFileName);
	bool			LoadFromBuffer(BYTE* pBuff, int nSize);
	void			UnLoad();
	bool			Draw(CDC* pDC);
	bool			Draw(CDC* pDC, CPoint Pos);
	bool			Draw(CDC* pDC, CSize Size);
	bool			Draw(CDC* pDC, CPoint Pos, CSize Size);
	bool			Draw(CDC* pDC, double nSizeRatio);
	bool			Draw(CDC* pDC, CPoint Pos, double nSizeRatio);
	bool			Draw(CDC* pDC, int x, int y, int cx, int cy);
	CSize			GetSize(CDC* pDC);
private:
	static	bool	GetResource(LPSTR lpName, LPSTR lpType, void* pResource, int& nBufSize);
	IPicture*		m_pPicture;
	enum
	{
		HIMETRIC_INCH	= 2540
	};
};

#endif // !defined(AFX_PICTURE_H__6098A4C3_D6D5_4711_BC7B_1595F459B480__INCLUDED_)
