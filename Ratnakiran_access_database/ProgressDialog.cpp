#include "stdafx.h"
#include "RatnaKiran.h"
#include "ProgressDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int g_progress_count = 0;

CProgressDialog::CProgressDialog(CString work, int length, CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDialog)
	//}}AFX_DATA_INIT

	m_max = length;
	m_created = FALSE;
	m_work = work;
}


CProgressDialog::~CProgressDialog()
{
	FinishDialog();
}


void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDialog)
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDialog, CDialog)
	//{{AFX_MSG_MAP(CProgressDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CProgressDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_progress.SetRange(0,m_max);
	m_progress.SetStep(1);
	m_progress.SetPos(0);

	return TRUE;
}


void CProgressDialog::StartDialog()
{
	if(Create(IDD_PROGRESS_DIALOG) == 0)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	CString title;
	if(!m_work.IsEmpty())
		title = title + m_work + ". ";

	title = title + "Please Wait ...";
	SetWindowText(title);
	m_created = TRUE;
	CenterWindow();
	//PeekAndPump();

	CRect rect;
	GetWindowRect(rect);

	int x = rect.left;
	int y = rect.top + (g_progress_count * rect.Height());

	SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	ShowWindow(SW_SHOW);

	g_progress_count++;
}


void CProgressDialog::FinishDialog()
{
	if(!m_created)
		return;

	if(!DestroyWindow())
		ShowWindow(SW_HIDE);

	m_created = FALSE;
	g_progress_count--;
}


void CProgressDialog::ProgressForward()
{
	if(!m_created)
		return;

	m_progress.StepIt();
}
