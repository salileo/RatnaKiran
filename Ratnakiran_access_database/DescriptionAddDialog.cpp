#include "stdafx.h"
#include "RatnaKiran.h"
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


BOOL CDescriptionAddDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!MakeGlobalsValid())
	{
		UnHandledError(__FILE__, __LINE__);
		CDialog::OnCancel();
		return FALSE;
	}
	
	return TRUE;
}

void CDescriptionAddDialog::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	if(!MakeGlobalsValid())
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	if(m_name.IsEmpty())
	{
		MessageBox("Cannot add an empty entry.", "Error");
		return;
	}

	if(!gDesc.AddDescription(m_name, m_prefix, m_number_of_pieces))
		return;

	CDialog::OnOK();
}

