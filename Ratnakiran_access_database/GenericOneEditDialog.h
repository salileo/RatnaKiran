#if !defined(AFX_GENERICONEEDITDIALOG_H__630F0ED1_D57B_4BF3_A0FA_204969AE1547__INCLUDED_)
#define AFX_GENERICONEEDITDIALOG_H__630F0ED1_D57B_4BF3_A0FA_204969AE1547__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class TYPE>
class CGenericOneEditDialog : public CDialog
{
public:
	CGenericOneEditDialog(TYPE start_val, TYPE lower_limit, TYPE upper_limit,
						  BOOL strict_limit, CString validation_failure,
						  CString title, CString window_title, CWnd* pParent = NULL);

	//{{AFX_DATA(CGenericOneEditDialog)
	enum { IDD = IDD_GENERIC_ONE_EDIT_DIALOG };
	CString	m_title;
	//}}AFX_DATA

	Dialog_Types m_type;

	TYPE m_val;
	TYPE m_start_val;
	TYPE m_lower_limit;
	TYPE m_upper_limit;
	BOOL m_strict_limit;

	CString m_window_title;
	CString m_validation_failure;

	//{{AFX_VIRTUAL(CGenericOneEditDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	void DetermineType(int obj);
	void DetermineType(double obj);
	void DetermineType(CString obj);

protected:
	//{{AFX_MSG(CGenericOneEditDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

template<class T>
CGenericOneEditDialog<T>::CGenericOneEditDialog(T start_val, T lower_limit, T upper_limit, 
												CString title, BOOL strict_limit, CString validation_failure, 
												CString window_title, CWnd* pParent /*=NULL*/)
	: CDialog(CGenericOneEditDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenericOneEditDialog)
	m_title = title;
	//}}AFX_DATA_INIT

	m_val = start_val;
	m_start_val = start_val;
	m_lower_limit = lower_limit;
	m_upper_limit = upper_limit;
	m_strict_limit = strict_limit;

	m_window_title = window_title;
	m_validation_failure = validation_failure;

	DetermineType(start_val);
}

template<class T>
void CGenericOneEditDialog<T>::DetermineType(int obj)
{
	m_type = TYPE_INT;
}

template<class T>
void CGenericOneEditDialog<T>::DetermineType(double obj)
{
	m_type = TYPE_DOUBLE;
}

template<class T>
void CGenericOneEditDialog<T>::DetermineType(CString obj)
{
	m_type = TYPE_STRING;
}

template<class T>
void CGenericOneEditDialog<T>::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenericOneEditDialog)
	DDX_Text(pDX, IDC_TITLE, m_title);
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_ENTRY, m_val);
	if(m_type == TYPE_INT)
	{
		void *valPtr = &m_val;
		void *lowerPtr = &m_lower_limit;
		void *upperPtr = &m_upper_limit;

		if(m_strict_limit)
		{
			if(m_lower_limit == m_upper_limit)
				DDV_MinMaxInt(pDX, *((int *)valPtr), *((int *)lowerPtr) + 1, 2147483647);
			else
				DDV_MinMaxInt(pDX, *((int *)valPtr), *((int *)lowerPtr) + 1, *((int *)upperPtr) + 1);
		}
		else
		{
			if(m_lower_limit == m_upper_limit)
				DDV_MinMaxInt(pDX, *((int *)valPtr), *((int *)lowerPtr), 2147483647);
			else
				DDV_MinMaxInt(pDX, *((int *)valPtr), *((int *)lowerPtr), *((int *)upperPtr));
		}
	}
}

template<class T>
BOOL CGenericOneEditDialog<T>::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText(m_window_title);
	return TRUE;
}

template<class T>
void CGenericOneEditDialog<T>::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	if(m_type == TYPE_STRING)
	{
		void *valPtr = &m_val;

		if(((CString *)valPtr)->IsEmpty())
		{
			if(!m_validation_failure.IsEmpty())
				MessageBox(m_validation_failure);
			return;
		}
	}
	else if((m_type == TYPE_DOUBLE) ||
			(m_type == TYPE_INT))
	{
		if(m_lower_limit == m_upper_limit)
		{
			if(m_val < m_lower_limit)
			{
				if(!m_validation_failure.IsEmpty())
					MessageBox(m_validation_failure);
				return;
			}

			if(m_strict_limit && (m_val == m_lower_limit))
			{
				if(!m_validation_failure.IsEmpty())
					MessageBox(m_validation_failure);
				return;
			}
		}
		else
		{
			if((m_val < m_lower_limit) || (m_val > m_upper_limit))
			{
				if(!m_validation_failure.IsEmpty())
					MessageBox(m_validation_failure);
				return;
			}

			if(m_strict_limit && ((m_val == m_lower_limit) || (m_val == m_upper_limit)))
			{
				if(!m_validation_failure.IsEmpty())
					MessageBox(m_validation_failure);
				return;
			}
		}
	}

	CDialog::OnOK();
}

template<class T>
void CGenericOneEditDialog<T>::OnCancel() 
{
	m_val = m_start_val;
	UpdateData(FALSE);
	CDialog::OnCancel();
}

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_GENERICONEEDITDIALOG_H__630F0ED1_D57B_4BF3_A0FA_204969AE1547__INCLUDED_)
