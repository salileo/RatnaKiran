#if !defined(AFX_ADDDIALOG_H__E1EAF30B_44F2_46C8_9724_C32E484FA2FB__INCLUDED_)
#define AFX_ADDDIALOG_H__E1EAF30B_44F2_46C8_9724_C32E484FA2FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAddDialog : public CDialog
{
public:
	CAddDialog(DatabaseType type, CProductSet *set, double gold_rate, CWnd* pParent = NULL);
	CAddDialog(DatabaseType type, CProductSet *set, double gold_rate, double conversion, CString currency_name, int desired_serial, CWnd* pParent = NULL);

private:
	void ClearItem();
	void ClearSet();
	BOOL UpdateSetValues(BOOL redraw, int item_index);
	BOOL UpdateItemValues(BOOL redraw);

	BOOL GotoTab(int index, BOOL redraw);
	BOOL UpdateTabs();
	BOOL DrawImage(CString filename);

	DatabaseType m_type;
	CProductSet m_original_set;
	CProductSet *m_set;
	CProductItem *m_item;

	double m_gold_rate;
	double m_conversion;
	CString m_currency_name;
	int m_desired_serial;
	BOOL m_disable_killmessage;

	//{{AFX_DATA(CAddDialog)
	CTabCtrl	m_tabs;
	CStatic	m_image_frame;
	CString	m_set_description;
	CString	m_set_productID;
	int		m_set_piece_count;
	CString	m_item_description;
	CString	m_item_productID;
	CString	m_item_photo;
	int		m_item_piece_count;
	double	m_gross_weight;
	double	m_net_weight;
	int		m_gold_value;
	int		m_diamond_rate;
	double	m_diamond_weight;
	int		m_diamond_value;
	int		m_diamond_marked_rate;
	int		m_diamond_marked_value;
	CString m_diamond_clarity;
	double	m_precious_stone_weight;
	int		m_precious_stone_rate;
	int		m_precious_stone_value;
	int		m_precious_stone_marked_rate;
	int		m_precious_stone_marked_value;
	double	m_pearl_weight;
	int		m_pearl_rate;
	int		m_pearl_value;
	int		m_pearl_marked_rate;
	int		m_pearl_marked_value;
	int		m_labour_charge;
	int		m_marked_labour_charge;
	double	m_costing;
	double	m_marked_costing;
	double	m_other_weight;
	int		m_total_price;
	int		m_total_marked_price;
	BOOL	m_item_sold;
	CString	m_sold_country;
	BOOL	m_use_net_weight;
	int		m_serial_number;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAddDialog)
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAddDialog)
	afx_msg void OnSelchangeItemTabs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingItemTabs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnItemAdd();
	afx_msg void OnItemDelete();
	afx_msg void OnSetDescriptionList();
	afx_msg void OnItemDescriptionList();
	afx_msg void OnItemChangeImage();
	afx_msg void OnItemRemoveImage();
	afx_msg void OnItemSold();
	afx_msg void OnItemUseNetWeight();
	afx_msg void OnDiamondDetails();
	afx_msg void OnStoneDetails();
	afx_msg void OnMarkedValues();
	afx_msg void OnOldSerials();
	afx_msg void OnSetSave();
	afx_msg void OnSetCancel();
	afx_msg void OnKillfocusSetDescription();
	afx_msg void OnKillfocusItemDescription();
	afx_msg void OnKillfocusItemPieceCount();
	afx_msg void OnKillfocusGrossWeight();
	afx_msg void OnKillfocusLabourCharge();
	afx_msg void OnKillfocusOtherWeight();
	afx_msg void OnKillfocusPearlRate();
	afx_msg void OnKillfocusPearlWeight();
	afx_msg void OnKillfocusDiamondClarity();
	afx_msg void OnKillfocusSoldCountry();
	afx_msg void OnKillfocusSerialNumber();
	afx_msg void OnKillfocusCosting();
	afx_msg void OnEditchangeSetDescription();
	afx_msg void OnEditchangeItemDescription();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_ADDDIALOG_H__E1EAF30B_44F2_46C8_9724_C32E484FA2FB__INCLUDED_)
