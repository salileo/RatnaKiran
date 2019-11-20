#include "stdafx.h"
#include "RatnaKiran.h"
#include "ExportSerialsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExportSerialsDialog::CExportSerialsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CExportSerialsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportSerialsDialog)
	m_from = 0;
	m_to = -1;
	m_tagname = _T("");
	m_use_old = FALSE;
	m_use_new = TRUE;
	//}}AFX_DATA_INIT
}

void CExportSerialsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportSerialsDialog)
	DDX_Text(pDX, IDC_FROM, m_from);
	DDV_MinMaxInt(pDX, m_from, -1, 2147483647);
	DDX_Text(pDX, IDC_TO, m_to);
	DDV_MinMaxInt(pDX, m_to, -1, 2147483647);
	DDX_Text(pDX, IDC_TAGNAME, m_tagname);
	DDX_Check(pDX, IDC_USE_OLD, m_use_old);
	DDX_Check(pDX, IDC_USE_NEW, m_use_new);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportSerialsDialog, CDialog)
	//{{AFX_MSG_MAP(CExportSerialsDialog)
	ON_BN_CLICKED(IDC_USE_OLD, OnUseOld)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CExportSerialsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CEdit *ctrl = (CEdit *)GetDlgItem(IDC_TAGNAME);
	if(ctrl)
		ctrl->SetReadOnly(!m_use_old);

	return TRUE;
}


void CExportSerialsDialog::OnUseOld() 
{
	if(!UpdateData(TRUE))
		return;

	CEdit *ctrl = (CEdit *)GetDlgItem(IDC_TAGNAME);
	if(ctrl)
		ctrl->SetReadOnly(!m_use_old);
}


void CExportSerialsDialog::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	if((m_from != -1) && (m_to != -1))
	{
		if(m_from > m_to)
		{
			int temp = m_to;
			m_to = m_from;
			m_from = temp;
		}
	}
	else if(m_from == -1)
	{
		if(m_to == -1)
		{
			m_from = 0;
		}
		else
		{
			int temp = m_from;
			m_from = m_to;
			m_to = temp;
		}
	}
	else if(m_to == -1)
	{
		//Nothing to do
	}
		
	UpdateData(FALSE);
	CDialog::OnOK();
}