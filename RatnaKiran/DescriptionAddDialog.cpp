#include "stdafx.h"
#include "RatnaKiran.h"
#include "DatabaseGlobals.h"
#include "DescriptionAddDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDescriptionAddDialog::CDescriptionAddDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDescriptionAddDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDescriptionAddDialog)
	m_name = _T("");
	m_prefix = _T("");
	m_number_of_pieces = 0;
	//}}AFX_DATA_INIT
}


void CDescriptionAddDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDescriptionAddDialog)
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_PREFIX, m_prefix);
	DDX_Text(pDX, IDC_NUMBER_OF_PIECES, m_number_of_pieces);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDescriptionAddDialog, CDialog)
	//{{AFX_MSG_MAP(CDescriptionAddDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDescriptionAddDialog::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	if(!CSettings::AddCategory(m_name, m_prefix, m_number_of_pieces))
	{
		CString errstr = "Could not add category '" + m_name + "' to the settings.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return;
	}
	
	CDialog::OnOK();
}
