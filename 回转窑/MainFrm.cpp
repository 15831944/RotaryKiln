// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "回转窑.h"

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
	ID_SEPARATOR,           // 状态行指示器
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
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
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME : IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 允许用户定义的工具栏操作:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
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

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
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
	// 创建热像仪停靠视图
	if (!m_dockabledlgthermal.Create(_T("热像仪视图"), this, CRect(0, 0, 200, 200), TRUE, IDD_VIEW_THERMAL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建窗口\n");
		return FALSE; // 未能创建
	}
	m_dockabledlgthermal.SetMinSize(CSize(1000, 800));//设置窗口最小大小
	m_dockabledlgthermal.EnableDocking(CBRS_ALIGN_LEFT);
	DockPane(&m_dockabledlgthermal);

	// 创建拼接停靠视图
	if (!m_dockabledlgsplicing.Create(_T("拼接视图"), this, CRect(0, 0, 200, 200), TRUE, IDD_VIEW_SPLICING, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建窗口\n");
		return FALSE; // 未能创建
	}
	//m_dockabledlgsplicing.SetMinSize(CSize(600,600));//设置窗口最小大小
	m_dockabledlgsplicing.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_dockabledlgsplicing);

	// 创建分区域表格视图
	if (!m_dockabledlgregiontable.Create(_T("分区域表格视图"), this, CRect(0, 0, 200, 200), TRUE, IDD_VIEW_REGIONTABLE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建窗口\n");
		return FALSE; // 未能创建
	}
	//m_dockabledlgregiontable.SetMinSize(CSize(600,600));//设置窗口最小大小
	m_dockabledlgregiontable.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_dockabledlgregiontable);
	// 创建横剖面曲线视图
	if (!m_dockabledlgcurve.Create(_T("横剖面曲线视图"), this, CRect(0, 0, 200, 200), TRUE, IDD_VIEW_CURVE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建窗口\n");
		return FALSE; // 未能创建
	}
	m_dockabledlgcurve.SetMinSize(CSize(600, 400));//设置窗口最小大小
	m_dockabledlgcurve.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_dockabledlgcurve);

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}
void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	// 添加热像仪停靠视图需要的图标
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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

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

// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
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
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// 为所有用户工具栏启用自定义按钮
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HermalShowFlag = false;
	Sleep(1000);
	//CloseHandle(pVideoThread);
	CFrameWndEx::OnClose();
}

//设置拼接信息
void CMainFrame::ThermalSet()
{
	// TODO: 在此添加命令处理程序代码
	if (-1 == userPermission.Find('B'))
	{
		AfxMessageBox("你的权限不够，请切换用户！");
		return;
	}
	if (m_image.IsNull())
	{
		AfxMessageBox("你还没有连接热像仪！");
		return;
	}
	m_theralsetdialog = new CThermalSetDialog;
	m_theralsetdialog->Create(IDD_SET_THERMAL, this);
	m_theralsetdialog->ShowWindow(SW_SHOW);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;//要求包含MainFrm.h头文件
	pFrame->m_wndStatusBar.SetPaneText(0, "用户名：" + userNumber);
	CFrameWndEx::OnTimer(nIDEvent);
}

void CMainFrame::OnChangeUser()
{
	// TODO: 在此添加命令处理程序代码
	CLogon mylogon;
	int nRet = mylogon.DoModal();
	//if ( nRet == LOGON_FALSE)

	return;
}

//设置热像仪信息
void CMainFrame::On32776()
{
	// TODO: 在此添加命令处理程序代码
	//设置热像仪信息
	if (-1 == userPermission.Find('A'))
	{
		AfxMessageBox("你的权限不够，请切换用户！");
		return;
	}
	AddThermalDialog addthermaldialog;
	addthermaldialog.DoModal();
	return;
}

//用户管理界面
void CMainFrame::On32775()
{
	// TODO: 在此添加命令处理程序代码
	if (-1 == userPermission.Find('D'))
	{
		AfxMessageBox("你的权限不够，请切换用户！");
		return;
	}
	UserManageDialog usermangedialog;
	usermangedialog.DoModal();
	return;
}

//查询历史数据
void CMainFrame::On32780()
{
	// TODO: 在此添加命令处理程序代码
	if (-1 == userPermission.Find('H'))
	{
		AfxMessageBox("你的权限不够，请切换用户！");
		return;
	}
	HDataDialog hdatadialog;
	hdatadialog.DoModal();
	return;
}

//查询日志
void CMainFrame::On32781()
{
	// TODO: 在此添加命令处理程序代码
	if (-1 == userPermission.Find('I'))
	{
		AfxMessageBox("你的权限不够，请切换用户！");
		return;
	}
	RecordDialog recorddialog;
	recorddialog.DoModal();
	return;
}

void CMainFrame::OnEditPaste()
{
	// TODO: 在此添加命令处理程序代码
	if (-1 == userPermission.Find('J'))
	{
		AfxMessageBox("你的权限不够，请切换用户！");
		return;
	}
	ShowPictureDialog showpicturedialog;
	showpicturedialog.DoModal();
	return;
}

void CMainFrame::On32795()
{
	// TODO: 在此添加命令处理程序代码
	if (!HermalShowFlag)
	{
		::SendMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);//最常用
	}
	HermalShowFlag = false;
}

void CMainFrame::OnMenu32796()
{
	// TODO: 在此添加命令处理程序代码
	if (!HermalShowFlag)
	{
		char buf[256];
		::GetModuleFileName(NULL, buf, sizeof(buf));
		CString strPath = buf;
		WinExec(strPath, SW_SHOW);//启动strPath路径下的执行文件
		::SendMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);//最常用
	}
	HermalShowFlag = false;
	ReStartFlag = true;
}

void CMainFrame::OnPictureSc()
{
	// TODO: 在此添加命令处理程序代码
	CString strPath;
	strPath = "热像仪抓图";
	if (!PathFileExists(strPath))
	{
		//不存在
		system("md " + strPath);
	}
	ShellExecute(NULL, "open", strPath, NULL, NULL, SW_SHOWNORMAL);
}

void CMainFrame::OnPictureAl()
{
	// TODO: 在此添加命令处理程序代码
	CString strPath;
	strPath = "热像仪报警";
	if (!PathFileExists(strPath))
	{
		//不存在
		system("md " + strPath);
	}
	ShellExecute(NULL, "open", strPath, NULL, NULL, SW_SHOWNORMAL);
}

void CMainFrame::OnHelp()
{
	// TODO: 在此添加命令处理程序代码
	CString strPathFile;
	strPathFile = "回转窑.chm";
	if (!PathFileExists(strPathFile))
	{
		//不存在
		AfxMessageBox("帮助文档没有安装");
	}
	ShellExecute(NULL, "open", strPathFile, NULL, NULL, SW_SHOWNORMAL);
}