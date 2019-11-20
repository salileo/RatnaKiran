#if !defined(AFX_GENERICTWOEDITDIALOG_H__4AAACD4A_3490_4E7C_9FC8_D88972681D14__INCLUDED_)
#define AFX_GENERICTWOEDITDIALOG_H__4AAACD4A_3490_4E7C_9FC8_D88972681D14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Common.h"

enum GenericTwoEditProperties
{
	Simple_Prop = 1,
	ReadOnly_Prop = 2,
	Password_Prop = 4
};

template<class T1, class T2>
class CGenericTwoEditDialog : public CDialog
{
public:
	CGenericTwoEditDialog(
		T1 start_val_1, CString title_1, T2 start_val_2, CString title_2,
		CString window_title, CWnd* pParent = NULL);

	void SetLimits1(T1 lower_limit_1, T1 upper_limit_1, BOOL strict_limit_1, CString validation_failure_1);
	void SetLimits2(T2 lower_limit_2, T2 upper_limit_2, BOOL strict_limit_2, CString validation_failure_2);

	void SetProperties(GenericTwoEditProperties prop1, GenericTwoEditProperties prop2);

	//{{AFX_DATA(CGenericTwoEditDialog)
	enum { IDD = IDD_GENERIC_TWO_EDIT_DIALOG };
	CString	m_title_1;
	CString	m_title_2;
	//}}AFX_DATA

	enum Dialog_Types
	{
		Type_Int,
		Type_Double,
		Type_String
	};

	Dialog_Types m_type_1;
	Dialog_Types m_type_2;

	T1 m_val_1;
	T1 m_start_val_1;
	BOOL m_limits_exist_1;
	T1 m_lower_limit_1;
	T1 m_upper_limit_1;
	BOOL m_strict_limit_1;
	CString m_validation_failure_1;
	int m_property_1;

	T2 m_val_2;
	T2 m_start_val_2;
	BOOL m_limits_exist_2;
	T2 m_lower_limit_2;
	T2 m_upper_limit_2;
	BOOL m_strict_limit_2;
	CString m_validation_failure_2;
	int m_property_2;

	CString m_window_title;

	//{{AFX_VIRTUAL(CGenericTwoEditDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void DetermineType(int i, int obj);
	void DetermineType(int i, double obj);
	void DetermineType(int i, CString obj);

protected:
	//{{AFX_MSG(CGenericTwoEditDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

template<class T1, class T2>
CGenericTwoEditDialog<T1, T2>::CGenericTwoEditDialog(
	T1 start_val_1, CString title_1, T2 start_val_2, CString title_2, 
	CString window_title, CWnd* pParent /* = NULL */)
	: CDialog(CGenericTwoEditDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenericTwoEditDialog)
	m_title_1 = title_1;
	m_title_2 = title_2;
	//}}AFX_DATA_INIT

	m_val_1 = start_val_1;
	m_start_val_1 = start_val_1;
	m_limits_exist_1 = FALSE;
	m_property_1 = Simple_Prop;

	m_val_2 = start_val_2;
	m_start_val_2 = start_val_2;
	m_limits_exist_2 = FALSE;
	m_property_2 = Simple_Prop;

	m_window_title = window_title;
		
	DetermineType(1, start_val_1);
	DetermineType(2, start_val_2);
}

template<class T1, class T2>
void CGenericTwoEditDialog<T1, T2>::SetLimits1(T1 lower_limit_1, T1 upper_limit_1, BOOL strict_limit_1, CString validation_failure_1)
{
	m_limits_exist_1 = TRUE;
	m_lower_limit_1 = lower_limit_1;
	m_upper_limit_1 = upper_limit_1;
	m_strict_limit_1 = strict_limit_1;
	m_validation_failure_1 = validation_failure_1;
}

template<class T1, class T2>
void CGenericTwoEditDialog<T1, T2>::SetLimits2(T2 lower_limit_2, T2 upper_limit_2, BOOL strict_limit_2, CString validation_failure_2)
{
	m_limits_exist_2 = TRUE;
	m_lower_limit_2 = lower_limit_2;
	m_upper_limit_2 = upper_limit_2;
	m_strict_limit_2 = strict_limit_2;
	m_validation_failure_2 = validation_failure_2;
}

template<class T1, class T2>
void CGenericTwoEditDialog<T1, T2>::SetProperties(GenericTwoEditProperties prop1, GenericTwoEditProperties prop2)
{
	m_property_1 = prop1;
	m_property_2 = prop2;
}

template<class T1, class T2>
void CGenericTwoEditDialog<T1, T2>::DetermineType(int i, int obj)
{
	if(i == 1)
		m_type_1 = Type_Int;
	else if(i == 2)
		m_type_2 = Type_Int;
}	

template<class T1, class T2>
void CGenericTwoEditDialog<T1, T2>::DetermineType(int i, double obj)
{
	if(i == 1)
		m_type_1 = Type_Double;
	else if(i == 2)
		m_type_2 = Type_Double;
}

template<class T1, class T2>
void CGenericTwoEditDialog<T1, T2>::DetermineType(int i, CString obj)
{
	if(i == 1)
		m_type_1 = Type_String;
	else if(i == 2)
		m_type_2 = Type_String;
}

template<class T1, class T2>
void CGenericTwoEditDialog<T1, T2>::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenericTwoEditDialog)
	DDX_Text(pDX, IDC_TITLE_1, m_title_1);
	DDX_Text(pDX, IDC_TITLE_2, m_title_2);
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_ENTRY_1, m_val_1);
	DDX_Text(pDX, IDC_ENTRY_2, m_val_2);

	if(m_limits_exist_1 && (m_type_1 == Type_Int))
	{
		void *valPtr = &m_val_1;
		void *lowerPtr = &m_lower_limit_1;
		void *upperPtr = &m_upper_limit_1;

		if(m_strict_limit_1)
		{
			if(m_lower_limit_1 == m_upper_limit_1)
				DDV_MinMaxInt(pDX, *((int *)valPtr), *((int *)lowerPtr) + 1, 2147483647);
			else
				DDV_MinMaxInt(pDX, *((int *)valPtr), *((int *)lowerPtr) + 1, *((int *)upperPtr) + 1);
		}
		else
		{
			if(m_lower_limit_1 == m_upper_limit_1)
				DDV_MinMaxInt(pDX, *((int *)valPtr), *((int *)lowerPtr), 2147483647);
			else
				DDV_MinMaxInt(pDX, *((int *)valPtr), *((int *)lowerPtr), *((int *)upperPtr));
		}
	}

	if(m_limits_exist_2 && (m_type_2 == Type_Int))
	{
		void *valPtr = &m_val_2;
		void *lowerPtr = &m_lower_limit_2;
		void *upperPtr = &m_upper_limit_2;

		if(m_strict_limit_2)
		{
			if(m_lower_limit_2 == m_upper_limit_2)
				DDV_MinMaxInt(pDX, *((int *)valPtr), *((int *)lowerPtr) + 1, 2147483647);
			else
				DDV_MinMaxInt(pDX, *((int *)valPtr), *((int *)lowerPtr) + 1, *((int *)upperPtr) + 1);
		}
		else
		{
			if(m_lower_limit_2 == m_upper_limit_2)
				DDV_MinMaxInt(pDX, *((int *)valPtr), *((int *)lowerPtr), 2147483647);
			else
				DDV_MinMaxInt(pDX, *((int *)valPtr), *((int *)lowerPtr), *((int *)upperPtr));
		}
	}
}


template<class T1, class T2>
BOOL CGenericTwoEditDialog<T1, T2>::OnInitDialog() 
{
	if(!CDialog::OnInitDialog())
	{
		SHOW_INTERNAL_ERROR_REASON("Error initializing the two edit dialog.");
		return FALSE;
	}

	CEdit *ctrl = (CEdit *)GetDlgItem(IDC_ENTRY_1);
	if(ctrl)
	{
		if(m_property_1 & ReadOnly_Prop)
			ctrl->SetReadOnly(TRUE);

		if(m_property_1 & Password_Prop)
			ctrl->SetPasswordChar(1);
	}
	else
		SHOW_INTERNAL_ERROR_REASON("Could not find first edit control.");

	ctrl = (CEdit *)GetDlgItem(IDC_ENTRY_2);
	if(ctrl)
	{
		if(m_property_2 & ReadOnly_Prop)
			ctrl->SetReadOnly(TRUE);

		if(m_property_2 & Password_Prop)
			ctrl->SetPasswordChar(1);
	}
	else
		SHOW_INTERNAL_ERROR_REASON("Could not find second edit control.");

	SetWindowText(m_window_title);
	return TRUE;
}

template<class T1, class T2>
void CGenericTwoEditDialog<T1, T2>::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	if(m_limits_exist_1)
	{
		if(m_type_1 == Type_String)
		{
			void *valPtr = &m_val_1;

			if(((CString *)valPtr)->IsEmpty())
			{
				if(!m_validation_failure_1.IsEmpty())
					SHOW_INFO(m_validation_failure_1);
				return;
			}
		}
		else if((m_type_1 == Type_Double) ||
				(m_type_1 == Type_Int))
		{
			if(m_lower_limit_1 == m_upper_limit_1)
			{
				if(m_val_1 < m_lower_limit_1)
				{
					if(!m_validation_failure_1.IsEmpty())
						SHOW_INFO(m_validation_failure_1);
					return;
				}

				if(m_strict_limit_1 && (m_val_1 == m_lower_limit_1))
				{
					if(!m_validation_failure_1.IsEmpty())
						SHOW_INFO(m_validation_failure_1);
					return;
				}
			}
			else
			{
				if((m_val_1 < m_lower_limit_1) || (m_val_1 > m_upper_limit_1))
				{
					if(!m_validation_failure_1.IsEmpty())
						SHOW_INFO(m_validation_failure_1);
					return;
				}

				if(m_strict_limit_1 && ((m_val_1 == m_lower_limit_1) || (m_val_1 == m_upper_limit_1)))
				{
					if(!m_validation_failure_1.IsEmpty())
						SHOW_INFO(m_validation_failure_1);
					return;
				}
			}
		}
	}

	if(m_limits_exist_2)
	{
		if(m_type_2 == Type_String)
		{
			void *valPtr = &m_val_2;

			if(((CString *)valPtr)->IsEmpty())
			{
				if(!m_validation_failure_2.IsEmpty())
					SHOW_INFO(m_validation_failure_2);
				return;
			}
		}
		else if((m_type_2 == Type_Double) ||
				(m_type_2 == Type_Int))
		{
			if(m_lower_limit_2 == m_upper_limit_2)
			{
				if(m_val_2 < m_lower_limit_2)
				{
					if(!m_validation_failure_2.IsEmpty())
						SHOW_INFO(m_validation_failure_2);
					return;
				}

				if(m_strict_limit_2 && (m_val_2 == m_lower_limit_2))
				{
					if(!m_validation_failure_2.IsEmpty())
						SHOW_INFO(m_validation_failure_2);
					return;
				}
			}
			else
			{
				if((m_val_2 < m_lower_limit_2) || (m_val_2 > m_upper_limit_2))
				{
					if(!m_validation_failure_2.IsEmpty())
						SHOW_INFO(m_validation_failure_2);
					return;
				}

				if(m_strict_limit_2 && ((m_val_2 == m_lower_limit_2) || (m_val_2 == m_upper_limit_2)))
				{
					if(!m_validation_failure_2.IsEmpty())
						SHOW_INFO(m_validation_failure_2);
					return;
				}
			}
		}
	}

	CDialog::OnOK();
}


template<class T1, class T2>
void CGenericTwoEditDialog<T1, T2>::OnCancel() 
{
	m_val_1 = m_start_val_1;
	m_val_2 = m_start_val_2;

	UpdateData(FALSE);
	CDialog::OnCancel();
}

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_GENERICTWOEDITDIALOG_H__4AAACD4A_3490_4E7C_9FC8_D88972681D14__INCLUDED_)
