#if !defined(AFX_GENERICTWOEDITDIALOG_H__4AAACD4A_3490_4E7C_9FC8_D88972681D14__INCLUDED_)
#define AFX_GENERICTWOEDITDIALOG_H__4AAACD4A_3490_4E7C_9FC8_D88972681D14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FIRST_READONLY 0
#define SECOND_READONLY 1
#define BOTH_READONLY 2
#define NO_READONLY 3

template<class T1, class T2>
class CGenericTwoEditDialog : public CDialog
{
public:
	CGenericTwoEditDialog(
		T1 start_val_1, T1 lower_limit_1, T1 upper_limit_1, CString title_1, BOOL strict_limit_1, CString validation_failure_1,
		T2 start_val_2, T2 lower_limit_2, T2 upper_limit_2, CString title_2, BOOL strict_limit_2, CString validation_failure_2,
		CString window_title, int properties, CWnd* pParent = NULL);

	//{{AFX_DATA(CGenericTwoEditDialog)
	enum { IDD = IDD_GENERIC_TWO_EDIT_DIALOG };
	CString	m_title_1;
	CString	m_title_2;
	//}}AFX_DATA

	int m_type_1;
	int m_type_2;

	T1 m_val_1;
	T1 m_start_val_1;
	T1 m_lower_limit_1;
	T1 m_upper_limit_1;
	BOOL m_strict_limit_1;
	CString m_validation_failure_1;

	T2 m_val_2;
	T2 m_start_val_2;
	T2 m_lower_limit_2;
	T2 m_upper_limit_2;
	BOOL m_strict_limit_2;
	CString m_validation_failure_2;

	CString m_window_title;
	int m_properties;

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
	T1 start_val_1, T1 lower_limit_1, T1 upper_limit_1, CString title_1, BOOL strict_limit_1, CString validation_failure_1, 
	T2 start_val_2, T2 lower_limit_2, T2 upper_limit_2, CString title_2, BOOL strict_limit_2, CString validation_failure_2, 
	CString window_title, int properties, CWnd* pParent /* = NULL */)
	: CDialog(CGenericTwoEditDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenericTwoEditDialog)
	m_title_1 = title_1;
	m_title_2 = title_2;
	//}}AFX_DATA_INIT

	m_val_1 = start_val_1;
	m_start_val_1 = start_val_1;
	m_lower_limit_1 = lower_limit_1;
	m_upper_limit_1 = upper_limit_1;
	m_strict_limit_1 = strict_limit_1;
	m_validation_failure_1 = validation_failure_1;

	m_val_2 = start_val_2;
	m_start_val_2 = start_val_2;
	m_lower_limit_2 = lower_limit_2;
	m_upper_limit_2 = upper_limit_2;
	m_strict_limit_2 = strict_limit_2;
	m_validation_failure_2 = validation_failure_2;

	m_window_title = window_title;
	m_properties = properties;
		
	DetermineType(1, start_val_1);
	DetermineType(2, start_val_2);
}

template<class T1, class T2>
void CGenericTwoEditDialog<T1, T2>::DetermineType(int i, int obj)
{
	if(i == 1)
		m_type_1 = TYPE_INT;
	else if(i == 2)
		m_type_2 = TYPE_INT;
}	

template<class T1, class T2>
void CGenericTwoEditDialog<T1, T2>::DetermineType(int i, double obj)
{
	if(i == 1)
		m_type_1 = TYPE_DOUBLE;
	else if(i == 2)
		m_type_2 = TYPE_DOUBLE;
}

template<class T1, class T2>
void CGenericTwoEditDialog<T1, T2>::DetermineType(int i, CString obj)
{
	if(i == 1)
		m_type_1 = TYPE_STRING;
	else if(i == 2)
		m_type_2 = TYPE_STRING;
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

	if(m_type_1 == TYPE_INT)
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

	if(m_type_2 == TYPE_INT)
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
	CDialog::OnInitDialog();

	if((m_properties == FIRST_READONLY) ||
		(m_properties == BOTH_READONLY))
	{
		CEdit *ctrl = (CEdit *)GetDlgItem(IDC_ENTRY_1);
		if(ctrl)
			ctrl->SetReadOnly(TRUE);
	}

	if((m_properties == SECOND_READONLY) ||
		(m_properties == BOTH_READONLY))
	{
		CEdit *ctrl = (CEdit *)GetDlgItem(IDC_ENTRY_2);
		if(ctrl)
			ctrl->SetReadOnly(TRUE);
	}

	SetWindowText(m_window_title);
	return TRUE;
}

template<class T1, class T2>
void CGenericTwoEditDialog<T1, T2>::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	if(m_type_1 == TYPE_STRING)
	{
		void *valPtr = &m_val_1;

		if(((CString *)valPtr)->IsEmpty())
		{
			if(!m_validation_failure_1.IsEmpty())
				MessageBox(m_validation_failure_1);
			return;
		}
	}
	else if((m_type_1 == TYPE_DOUBLE) ||
			(m_type_1 == TYPE_INT))
	{
		if(m_lower_limit_1 == m_upper_limit_1)
		{
			if(m_val_1 < m_lower_limit_1)
			{
				if(!m_validation_failure_1.IsEmpty())
					MessageBox(m_validation_failure_1);
				return;
			}

			if(m_strict_limit_1 && (m_val_1 == m_lower_limit_1))
			{
				if(!m_validation_failure_1.IsEmpty())
					MessageBox(m_validation_failure_1);
				return;
			}
		}
		else
		{
			if((m_val_1 < m_lower_limit_1) || (m_val_1 > m_upper_limit_1))
			{
				if(!m_validation_failure_1.IsEmpty())
					MessageBox(m_validation_failure_1);
				return;
			}

			if(m_strict_limit_1 && ((m_val_1 == m_lower_limit_1) || (m_val_1 == m_upper_limit_1)))
			{
				if(!m_validation_failure_1.IsEmpty())
					MessageBox(m_validation_failure_1);
				return;
			}
		}
	}

	if(m_type_2 == TYPE_STRING)
	{
		void *valPtr = &m_val_2;

		if(((CString *)valPtr)->IsEmpty())
		{
			if(!m_validation_failure_2.IsEmpty())
				MessageBox(m_validation_failure_2);
			return;
		}
	}
	else if((m_type_2 == TYPE_DOUBLE) ||
			(m_type_2 == TYPE_INT))
	{
		if(m_lower_limit_2 == m_upper_limit_2)
		{
			if(m_val_2 < m_lower_limit_2)
			{
				if(!m_validation_failure_2.IsEmpty())
					MessageBox(m_validation_failure_2);
				return;
			}

			if(m_strict_limit_2 && (m_val_2 == m_lower_limit_2))
			{
				if(!m_validation_failure_2.IsEmpty())
					MessageBox(m_validation_failure_2);
				return;
			}
		}
		else
		{
			if((m_val_2 < m_lower_limit_2) || (m_val_2 > m_upper_limit_2))
			{
				if(!m_validation_failure_2.IsEmpty())
					MessageBox(m_validation_failure_2);
				return;
			}

			if(m_strict_limit_2 && ((m_val_2 == m_lower_limit_2) || (m_val_2 == m_upper_limit_2)))
			{
				if(!m_validation_failure_2.IsEmpty())
					MessageBox(m_validation_failure_2);
				return;
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
