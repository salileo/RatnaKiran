#if !defined(AFX_ADDDIALOGEXPORT_H__AC01DCBB_64BA_4ACB_ABDB_B0C0C2966AB0__INCLUDED_)
#define AFX_ADDDIALOGEXPORT_H__AC01DCBB_64BA_4ACB_ABDB_B0C0C2966AB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAddDialogExport : public CDialog
{
public:
	CAddDialogExport(int serial, CWnd* pParent = NULL);

	CTableSet *m_set;
	CTableItem *m_item;

	void ClearItem();
	void UpdateSetValues(BOOL redraw);
	void UpdateItemValues(BOOL redraw);
	void UpdateTabs();

	void DrawImage(CString filename);
	CPoint ScaleImage(CSize original, CSize window, CSize *final);
	int ShowMessage(CString mesg, CString caption = "", int type = MB_OK);
	
private:
	int m_desired_serial;
	BOOL m_disable_killmessage;

	//{{AFX_DATA(CAddDialogExport)
	enum { IDD = IDD_ADD_DIALOG_EXPORT };
	CTabCtrl	m_tabs;
	CStatic	m_image_frame;
	CString	m_item_description;
	CString	m_item_productID;
	CString	m_item_photo;
	int		m_item_piece_count;
	double	m_gross_weight;
	double	m_net_weight;
	int		m_gold_value;
	double	m_diamond_weight;
	int		m_diamond_rate;
	int		m_diamond_value;
	double	m_stone_weight;
	int		m_stone_rate;
	int		m_stone_value;
	double	m_pearl_weight;
	int		m_pearl_rate;
	int		m_pearl_value;
	int		m_labour_charge;
	double	m_other_weight;
	int		m_total_price;
	BOOL	m_item_sold;
	CString	m_sold_country;
	int		m_serial_number;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAddDialogExport)
	protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAddDialogExport)
	afx_msg void OnSelchangeItemTabs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingItemTabs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnItemAdd();
	afx_msg void OnItemDelete();
	afx_msg void OnItemDescriptionList();
	afx_msg void OnItemChangeImage();
	afx_msg void OnItemRemoveImage();
	afx_msg void OnItemDumpImage();
	afx_msg void OnItemSold();
	afx_msg void OnDiamondDetails();
	afx_msg void OnStoneDetails();
	afx_msg void OnSetSave();
	afx_msg void OnSetCancel();
	afx_msg void OnKillfocusItemDescription();
	afx_msg void OnKillfocusItemPieceCount();
	afx_msg void OnKillfocusGrossWeight();
	afx_msg void OnKillfocusLabourCharge();
	afx_msg void OnKillfocusOtherWeight();
	afx_msg void OnKillfocusPearlRate();
	afx_msg void OnKillfocusPearlWeight();
	afx_msg void OnKillfocusSoldCountry();
	afx_msg void OnKillfocusSerialNumber();
	afx_msg void OnOldSerials();
	afx_msg void OnEditchangeItemDescription();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_ADDDIALOGEXPORT_H__AC01DCBB_64BA_4ACB_ABDB_B0C0C2966AB0__INCLUDED_)
