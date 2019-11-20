#if !defined(AFX_RATNAKIRANDOC_H__D779214B_4E08_4D9C_BB1A_9BAB4256A913__INCLUDED_)
#define AFX_RATNAKIRANDOC_H__D779214B_4E08_4D9C_BB1A_9BAB4256A913__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TableVoucher.h"

class CRatnakiranDoc : public CDocument
{
protected: // create from serialization only
	CRatnakiranDoc();
	DECLARE_DYNCREATE(CRatnakiranDoc)

public:
	CTableVoucher	m_vouchers;

	//{{AFX_VIRTUAL(CRatnakiranDoc)
	public:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

	BOOL GetVoucherList(CString type, CList<CString, CString> *list);
	BOOL CreateNewVoucher(CString type, CString ID);
	BOOL DeleteVoucher(CString ID);

public:
	virtual ~CRatnakiranDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(CRatnakiranDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_RATNAKIRANDOC_H__D779214B_4E08_4D9C_BB1A_9BAB4256A913__INCLUDED_)
