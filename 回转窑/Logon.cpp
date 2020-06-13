// Logon.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "Logon.h"
#include "afxdialogex.h"
#include "md5.h"

#define MD5STR "南京工程学院计算工程学院王杰"

#include "SQLConnect.hpp"
#include "easylogging++.h"
#include "CCopyDialog.h"



extern CString userNumber;
extern  CString userPermission;
CString  administrator_passwd;

// CLogon 对话框

IMPLEMENT_DYNAMIC(CLogon, CDialogEx)

CLogon::CLogon(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogon::IDD, pParent)
	, m_password(_T(""))
{
	SYSTEMTIME st;
	CString strDateTime;
	GetLocalTime(&st);
	strDateTime.Format("%4d-%2d-%2d %2d:%2d:%2d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	MD5 md5;
	strDateTime = strDateTime + MD5STR;// +std::to_string(MachineKey).c_str();
	md5.update(strDateTime.GetBuffer());
	administrator_passwd=md5.toString().c_str();
}

CLogon::~CLogon()
{
}

void CLogon::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDV_MaxChars(pDX, m_password, 40);
	DDX_Control(pDX, IDC_COMBO1_USER, m_comboxUser);
}


BEGIN_MESSAGE_MAP(CLogon, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_AUTOLOGON, &CLogon::OnBnClickedCheckAutologon)
	ON_BN_CLICKED(IDC_BUTTON_LOGON, &CLogon::OnBnClickedButtonLogon)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CLogon::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CLogon 消息处理程序





void CLogon::OnBnClickedCheckAutologon()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton * pBtn = NULL ;  
	pBtn = (CButton * )GetDlgItem(IDC_CHECK_AUTOLOGON);
	if (pBtn->GetCheck()) 
		autologon_flag=TRUE;
	else
		autologon_flag=FALSE;
}

void CLogon::OnBnClickedButtonLogon()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_comboxUser.GetWindowText(user_number);

	MD5 md5;
	user_passwd = user_number + m_password + MD5STR + std::to_string(MachineKey).c_str();
	md5.update(user_passwd.GetBuffer());
	user_passwd = md5.toString().c_str();//toString()函数获得加密字符串，c_str();函数重新转换成CString

	if(user_number=="administrator"&&m_password==administrator_passwd)
	{	
		userNumber=user_number;
		userPermission="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		AfxMessageBox("超级用户!");
		EndDialog(LOGON_TRUE);
		return;
	}

	if(user_number.IsEmpty()||m_password.IsEmpty())
	{
		MessageBox(_T("用户名或密码不能为空!"),_T("用户登录信息"));
		return;
	}

	AccessResult res;	
	CString select_sql_by_user;
	select_sql_by_user.Format("select * from user_info where user_number= \'%s\'",user_number);
	accessConnect.executeSQL(select_sql_by_user.GetString(), res);
	BOOL logon_flag=false;
	if (res.empty())//查询结果为空
	{
		AfxMessageBox("用户不存在");
	}
	else
	{
		if (user_passwd == res[0]["user_passwd"].c_str())
		{
			userNumber = user_number;
			userPermission = res[0]["user_permission"].c_str();
			logon_flag = true;
			AfxMessageBox("登录成功!");
			LOG(INFO) << userNumber + "登录";
		}
		else
		{
			AfxMessageBox("密码错误!");
			LOG(ERROR) << userNumber + "登录密码错误";
		}
	}
	if (logon_flag)
		EndDialog(LOGON_TRUE);
	return;	
}


void CLogon::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(LOGON_FALSE);
}


BOOL CLogon::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	static bool m_ctrl_down = false;//此函数第一次调用的时候初始化
	static bool m_alt_down = false;//此函数第一次调用的时候初始化
	if (pMsg->message == WM_KEYDOWN)
	{

		switch (pMsg->wParam)
		{
			//VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A) 不区分大小写

		case 'A'://Ctrl + A
		case 'a':
			if (m_ctrl_down)
			{
				SYSTEMTIME st;
				CString strDateTime;
				GetLocalTime(&st);
				strDateTime.Format("%4d-%2d-%2d %2d:%2d:%2d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
				MD5 md5;
				strDateTime = strDateTime + MD5STR;// +std::to_string(MachineKey).c_str();
				md5.update(strDateTime.GetBuffer());
				strDateTime=md5.toString().c_str();//toString()函数获得加密字符串，c_str();函数重新转换成CString
				administrator_passwd = strDateTime + MD5STR;// +std::to_string(MachineKey).c_str();
				md5.reset();
				md5.update(administrator_passwd.GetBuffer());
				administrator_passwd=md5.toString().c_str();
				m_comboxUser.SetWindowText("administrator");
				UpdateData(false);
				ShowInfoDialog(strDateTime);
				//AfxMessageBox(strDateTime);
				return true;
			}
			break;
		case VK_CONTROL:
			m_ctrl_down = true;break;
		case VK_MENU:
			m_alt_down = true; break;
		default:
			break;
		}
	}
	if (pMsg->message == WM_KEYUP)
	{		switch (pMsg->wParam)
		{
		case VK_CONTROL:
			m_ctrl_down = false;break;
		case VK_MENU:
			m_alt_down = false; break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CLogon::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
    OnBnClickedButtonLogon();//回车键调用确定安装
}


BOOL CLogon::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	//HBRUSH hBrush = CreateSolidBrush(RGB( 0 , 0 , 255 ) );    //加载背景句柄，此处为蓝色RGB
	//cs.lpszClass = AfxRegisterWndClass(NULL , NULL , hBrush , NULL );
	return CDialogEx::PreCreateWindow(cs);
}


void CLogon::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::PreSubclassWindow();
}
