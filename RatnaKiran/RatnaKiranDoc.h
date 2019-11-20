#if !defined(AFX_RATNAKIRANDOC_H__368DDB83_68F0_4952_82B1_F544F0D7CE65__INCLUDED_)
#define AFX_RATNAKIRANDOC_H__368DDB83_68F0_4952_82B1_F544F0D7CE65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRatnaKiranDoc : public CDocument
{
protected:
	DECLARE_DYNCREATE(CRatnaKiranDoc)

public:
	CRatnaKiranDoc();
	virtual ~CRatnaKiranDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CList<CProductVoucher *,CProductVoucher *> m_stock_vouchers;
	CList<CProductVoucher *,CProductVoucher *> m_export_vouchers;
	CList<CProductVoucher *,CProductVoucher *> m_kundan_vouchers;
	CProductVoucher *m_selected_voucher;

	BOOL ReadVouchers();
	BOOL WriteVouchers();

	BOOL AddVoucher(CProductVoucher *voucher);
	BOOL DeleteVoucher(CString voucher_name);
	CProductVoucher *GetVoucher(CString voucher_name);
	CProductSet *GetSet(CString productID, CProductVoucher **voucher);
	BOOL SetCurrentVoucher(CString name);

	BOOL IsIgnored(CString name);
	void AddToIgnoreEntry(CString name);
	void DeleteFromIgnoreEntry(CString name);

private:
	CList<CString, CString> m_ignore_list;
	void ClearVouchers();
	BOOL ReadFile(CString filename);
	BOOL WriteFile(CProductVoucher *cur_voucher);

	//{{AFX_VIRTUAL(CRatnaKiranDoc)
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CRatnaKiranDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_RATNAKIRANDOC_H__368DDB83_68F0_4952_82B1_F544F0D7CE65__INCLUDED_)
