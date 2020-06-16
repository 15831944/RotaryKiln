// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "��תҤ.h"

#include "MainFrm.h"
#include "AddThermalDialog.h"
#include "UserManageDialog.h"
#include "HDataDialog.h"
#include "RecordDialog.h"
#include "ShowPictureDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CString userNumber;
extern CString userPermission;
extern ATL::CImage m_image;

// CMainFrame
extern bool HermalShowFlag;
extern bool ReStartFlag;
//extern CWinThread  * pVideoThread;
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_CLOSE()
	ON_COMMAND(ID_32778, &CMainFrame::ThermalSet)
	ON_WM_TIMER()
	ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnChangeUser)
	ON_COMMAND(ID_32776, &CMainFrame::On32776)
	ON_COMMAND(ID_32775, &CMainFrame::On32775)
	ON_COMMAND(ID_32780, &CMainFrame::On32780)
	ON_COMMAND(ID_32781, &CMainFrame::On32781)
	ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnEditPaste)
	ON_COMMAND(ID_32795, &CMainFrame::On32795)
	ON_COMMAND(ID_Menu32796, &CMainFrame::OnMenu32796)
	ON_COMMAND(ID_PICTURE_SC, &CMainFrame::OnPictureSc)
	ON_COMMAND(ID_PICTURE_AL, &CMainFrame::OnPictureAl)
	ON_COMMAND(ID_Menu, &CMainFrame::OnHelp)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	m_theralsetdialog = NULL;
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	//system("net start mysql");
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;      // δ�ܴ���
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// ��ֹ�˵����ڼ���ʱ��ý���
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME : IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// �����û�����Ĺ���������:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	// TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���ù�������ͣ�����ڲ˵��滻
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ���ÿ���(��ס Alt �϶�)�������Զ���
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// �����û�����Ĺ�����ͼ��
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// ���ò˵����Ի�(���ʹ�õ�����)
	// TODO: �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	SetTimer(1, 1000, NULL);

	CMenu* pMenu = GetSystemMenu(false);
	UINT nID = pMenu->GetMenuItemID(pMenu->GetMenuItemCount() - 1);
	pMenu->EnableMenuItem(nID, MF_DISABLED);
	return 0;
}

BOOL CMainFrame::CreateDockingWindows()
{
	// ����������ͣ����ͼ
	if (!m_dockabledlgthermal.Create(_T("��������ͼ"), this, CRect(0, 0, 200, 200), TRUE, IDD_VIEW_THERMAL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�������\n");
		return FALSE; // δ�ܴ���
	}
	m_dockabledlgthermal.SetMinSize(CSize(1000, 800));//���ô�����С��С
	m_dockabledlgthermal.EnableDocking(CBRS_ALIGN_LEFT);
	DockPane(&m_dockabledlgthermal);

	// ����ƴ��ͣ����ͼ
	if (!m_dockabledlgsplicing.Create(_T("ƴ����ͼ"), this, CRect(0, 0, 200, 200), TRUE, IDD_VIEW_SPLICING, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�������\n");
		return FALSE; // δ�ܴ���
	}
	//m_dockabledlgsplicing.SetMinSize(CSize(600,600));//���ô�����С��С
	m_dockabledlgsplicing.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_dockabledlgsplicing);

	// ��������������ͼ
	if (!m_dockabledlgregiontable.Create(_T("����������ͼ"), this, CRect(0, 0, 200, 200), TRUE, IDD_VIEW_REGIONTABLE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�������\n");
		return FALSE; // δ�ܴ���
	}
	//m_dockabledlgregiontable.SetMinSize(CSize(600,600));//���ô�����С��С
	m_dockabledlgregiontable.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_dockabledlgregiontable);
	// ����������������ͼ
	if (!m_dockabledlgcurve.Create(_T("������������ͼ"), this, CRect(0, 0, 200, 200), TRUE, IDD_VIEW_CURVE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�������\n");
		return FALSE; // δ�ܴ���
	}
	m_dockabledlgcurve.SetMinSize(CSize(600, 400));//���ô�����С��С
	m_dockabledlgcurve.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_dockabledlgcurve);

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}
void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	// ���������ͣ����ͼ��Ҫ��ͼ��
	HICON hIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON6), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//HICON hFileViewIcon =LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	//m_wndFileView.SetIcon(hFileViewIcon, FALSE);
	//m_dockabledlgthermal.SetIcon(hFileViewIcon, FALSE);
	m_dockabledlgthermal.SetIcon(hIcon, FALSE);
	hIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_dockabledlgsplicing.SetIcon(hIcon, FALSE);
	hIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON3), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_dockabledlgregiontable.SetIcon(hIcon, FALSE);
	hIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON5), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_dockabledlgcurve.SetIcon(hIcon, FALSE);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

// CMainFrame ��Ϣ�������

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp, LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp, lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// ���ཫִ�������Ĺ���

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// Ϊ�����û������������Զ��尴ť
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	HermalShowFlag = false;
	Sleep(1000);
	//CloseHandle(pVideoThread);
	CFrameWndEx::OnClose();
}

//����ƴ����Ϣ
void CMainFrame::ThermalSet()
{
	// TODO: �ڴ���������������
	if (-1 == userPermission.Find('B'))
	{
		AfxMessageBox("���Ȩ�޲��������л��û���");
		return;
	}
	if (m_image.IsNull())
	{
		AfxMessageBox("�㻹û�����������ǣ�");
		return;
	}
	m_theralsetdialog = new CThermalSetDialog;
	m_theralsetdialog->Create(IDD_SET_THERMAL, this);
	m_theralsetdialog->ShowWindow(SW_SHOW);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;//Ҫ�����MainFrm.hͷ�ļ�
	pFrame->m_wndStatusBar.SetPaneText(0, "�û�����" + userNumber);
	CFrameWndEx::OnTimer(nIDEvent);
}

void CMainFrame::OnChangeUser()
{
	// TODO: �ڴ���������������
	CLogon mylogon;
	int nRet = mylogon.DoModal();
	//if ( nRet == LOGON_FALSE)

	return;
}

//������������Ϣ
void CMainFrame::On32776()
{
	// TODO: �ڴ���������������
	//������������Ϣ
	if (-1 == userPermission.Find('A'))
	{
		AfxMessageBox("���Ȩ�޲��������л��û���");
		return;
	}
	AddThermalDialog addthermaldialog;
	addthermaldialog.DoModal();
	return;
}

//�û��������
void CMainFrame::On32775()
{
	// TODO: �ڴ���������������
	if (-1 == userPermission.Find('D'))
	{
		AfxMessageBox("���Ȩ�޲��������л��û���");
		return;
	}
	UserManageDialog usermangedialog;
	usermangedialog.DoModal();
	return;
}

//��ѯ��ʷ����
void CMainFrame::On32780()
{
	// TODO: �ڴ���������������
	if (-1 == userPermission.Find('H'))
	{
		AfxMessageBox("���Ȩ�޲��������л��û���");
		return;
	}
	HDataDialog hdatadialog;
	hdatadialog.DoModal();
	return;
}

//��ѯ��־
void CMainFrame::On32781()
{
	// TODO: �ڴ���������������
	if (-1 == userPermission.Find('I'))
	{
		AfxMessageBox("���Ȩ�޲��������л��û���");
		return;
	}
	RecordDialog recorddialog;
	recorddialog.DoModal();
	return;
}

void CMainFrame::OnEditPaste()
{
	// TODO: �ڴ���������������
	if (-1 == userPermission.Find('J'))
	{
		AfxMessageBox("���Ȩ�޲��������л��û���");
		return;
	}
	ShowPictureDialog showpicturedialog;
	showpicturedialog.DoModal();
	return;
}

void CMainFrame::On32795()
{
	// TODO: �ڴ���������������
	if (!HermalShowFlag)
	{
		::SendMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);//���
	}
	HermalShowFlag = false;
}

void CMainFrame::OnMenu32796()
{
	// TODO: �ڴ���������������
	if (!HermalShowFlag)
	{
		char buf[256];
		::GetModuleFileName(NULL, buf, sizeof(buf));
		CString strPath = buf;
		WinExec(strPath, SW_SHOW);//����strPath·���µ�ִ���ļ�
		::SendMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);//���
	}
	HermalShowFlag = false;
	ReStartFlag = true;
}

void CMainFrame::OnPictureSc()
{
	// TODO: �ڴ���������������
	CString strPath;
	strPath = "������ץͼ";
	if (!PathFileExists(strPath))
	{
		//������
		system("md " + strPath);
	}
	ShellExecute(NULL, "open", strPath, NULL, NULL, SW_SHOWNORMAL);
}

void CMainFrame::OnPictureAl()
{
	// TODO: �ڴ���������������
	CString strPath;
	strPath = "�����Ǳ���";
	if (!PathFileExists(strPath))
	{
		//������
		system("md " + strPath);
	}
	ShellExecute(NULL, "open", strPath, NULL, NULL, SW_SHOWNORMAL);
}

void CMainFrame::OnHelp()
{
	// TODO: �ڴ���������������
	CString strPathFile;
	strPathFile = "��תҤ.chm";
	if (!PathFileExists(strPathFile))
	{
		//������
		AfxMessageBox("�����ĵ�û�а�װ");
	}
	ShellExecute(NULL, "open", strPathFile, NULL, NULL, SW_SHOWNORMAL);
}