#if !defined(AFX_PICTURE_H__6098A4C3_D6D5_4711_BC7B_1595F459B480__INCLUDED_)
#define AFX_PICTURE_H__6098A4C3_D6D5_4711_BC7B_1595F459B480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SPicture  
{
public:
	SPicture();
	virtual ~SPicture();

	BOOL Load(CString sResourceType, CString sResource);
	BOOL Load(CString sFileName);
	BOOL LoadFromBuffer(BYTE* pBuff, int nSize);
	void UnLoad();
	BOOL Draw(CDC* pDC);
	BOOL Draw(CDC* pDC, CPoint Pos);
	BOOL Draw(CDC* pDC, CSize Size);
	BOOL Draw(CDC* pDC, CPoint Pos, CSize Size);
	BOOL Draw(CDC* pDC, double nSizeRatio);
	BOOL Draw(CDC* pDC, CPoint Pos, double nSizeRatio);
	BOOL Draw(CDC* pDC, int x, int y, int cx, int cy);
	CSize GetSize(CDC* pDC);
private:
	static BOOL GetResource(LPSTR lpName, LPSTR lpType, void* pResource, int& nBufSize);
	IPicture* m_pPicture;

	enum
	{
		HIMETRIC_INCH = 2540
	};
};

#endif // !defined(AFX_PICTURE_H__6098A4C3_D6D5_4711_BC7B_1595F459B480__INCLUDED_)
