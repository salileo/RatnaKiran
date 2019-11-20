#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int g_progress_count = 0;

CProgressDialog::CProgressDialog(CString work, DWORD length, CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDialog)
	//}}AFX_DATA_INIT

	m_max = length;
	m_created = FALSE;

	if(!work.IsEmpty())
		m_work = work + ".";

	m_scale = 0.0;
}


CProgressDialog::~CProgressDialog()
{
	if(m_created)
		FinishDialog();
}


void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDialog)
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_progress);
	DDX_Text(pDX, IDC_PROGRESS_WORK, m_work);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDialog, CDialog)
	//{{AFX_MSG_MAP(CProgressDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CProgressDialog::StartDialog()
{
	if(m_created)
	{
		ShowWindow(SW_SHOW);
	}
	else if(!Create(IDD_PROGRESS_DIALOG))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not create progress dialog.");
		return FALSE;
	}

#if 0
	CString title;
	if(!m_work.IsEmpty())
		title = m_work + ". ";

	title = title + "\nPlease Wait ...";
	SetWindowText(title);
#endif

	m_created = TRUE;
	CenterWindow();

	CRect rect;
	GetWindowRect(rect);

	int x = rect.left;
	int y = rect.top + (g_progress_count * rect.Height());

	SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	if(!SetupDialog(m_max, TRUE))
		SHOW_INTERNAL_ERROR_REASON("Could not set initial values for progress dialog.");

	ShowWindow(SW_SHOW);
	g_progress_count++;
	return TRUE;
}


BOOL CProgressDialog::SetupDialog(DWORD length, BOOL reset)
{
	if((!m_created) || (length < 0))
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	int old_pos = 0;
	if(!reset)
	{
		old_pos = m_progress.GetPos();
		length = length + m_max;
	}

	m_max = length;
	DWORD i = min(m_max, SHRT_MAX);
	m_scale = (double)i / (double)m_max;

	m_progress.SetRange(0,(short)i);
	m_progress.SetStep(1);

	if(reset)
		m_progress.SetPos(0);
	else
	{
		if(!ProgressForward(old_pos))
			SHOW_INTERNAL_ERROR_REASON("Could not update the progress dialog properly.");
	}

	return TRUE;
}


BOOL CProgressDialog::FinishDialog()
{
	if(!m_created)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(DestroyWindow())
		m_created = FALSE;
	else
		ShowWindow(SW_HIDE);

	g_progress_count--;
	return TRUE;
}


BOOL CProgressDialog::ProgressForward(DWORD step_size)
{
	if(!m_created)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	int current = m_progress.GetPos();
	int forward = (int)((double)step_size * m_scale);

	while((DWORD)(current + forward) > m_max)
		forward = forward - m_max;

	m_progress.SetPos(current + forward);

	if(!RedrawWindow(NULL, NULL, RDW_UPDATENOW))
		SHOW_INTERNAL_ERROR_REASON("Could not redraw the progress dialog");

	return TRUE;
}
