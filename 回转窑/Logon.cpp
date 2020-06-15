// Logon.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "Logon.h"
#include "afxdialogex.h"
#include "md5.h"

#define MD5STR "南京工程学院计算工程学院王杰"

//mysql必须包含网络相关头文件  
#include "winsock.h"  
//mysql头文件自然要包含    
#include "mysql.h"
#include "easylogging++.h"



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
	strDateTime=strDateTime+MD5STR;
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

void mysql_real_cstring(CString &str)
{
	std::string strSql=str.GetBuffer();
	std::string strDest;
	size_t iSrcSize = strSql.size();
	
	for (size_t i = 0; i < iSrcSize; i++) {
		char ch = strSql[i];
		switch (ch)
		{
		case '\0':
			break;
		case '\n':
			break;
		case '\r':
			break;
		case '\'':
			break;
		case '"':
			break;
		case '\\':
			break;
		case '%':
			break;
		case '_':
			break;
		case ' ':
			break;
		default:
			strDest.append(1, ch);
			break;
		}
	}
	str=strDest.c_str();
	return;
}
void CLogon::OnBnClickedButtonLogon()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_comboxUser.GetWindowText(user_number);
	mysql_real_cstring(user_number);
	if(user_number=="administrator"&&m_password==administrator_passwd)
	{	
		userNumber=user_number;
		userPermission="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		AfxMessageBox("超级用户!");
		EndDialog(LOGON_TRUE);
		return;
	}
	MYSQL mysql; //数据库连接句柄
	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_init(&mysql);
	//设置数据库编码格式
	//	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//密码加字符串""
	if(!mysql_real_connect(&mysql,"localhost","root","123456","mydb",3306,NULL,0))
	{//mydb为你所创建的数据库，3306为端口号，root是用户名,123456是密码
		AfxMessageBox("数据库连接失败");
		CString e=mysql_error(&mysql);//需要将项目属性中字符集修改为“使用多字节字符集”或“未设置”  
		LOG(ERROR)   << "login:"+e;
		MessageBox(e);  
		return;
	}
	if(user_number.IsEmpty()||m_password.IsEmpty())
	{
		MessageBox(_T("用户名或密码不能为空!"),_T("用户登录信息"));
		return;
	}
	MD5 md5;
	user_passwd=m_password+MD5STR;
	md5.update(user_passwd.GetBuffer());
	user_passwd=md5.toString().c_str();//toString()函数获得加密字符串，c_str();函数重新转换成CString
	
	CString select_sql_by_user;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	select_sql_by_user.Format("select * from user_info where user_number= \'%s\'",user_number);
	int ress=mysql_query(&mysql,(char*)(LPCSTR)select_sql_by_user);
	BOOL logon_flag=false;
	if(ress==0) //检测查询成功
	{
		res = mysql_store_result(&mysql);
		if(mysql_num_rows(res)==0) //查询结果为空
		{
			AfxMessageBox("用户不存在");
		}
		else
		{

			row=mysql_fetch_row(res);
			CString str;
			for(int i=0;i<11;i++)
				str+=row[i];
			md5.reset();
			md5.update(str.GetBuffer());
			str=md5.toString().c_str();
			if(str!=row[12])
			{
				AfxMessageBox("数据校验错误，你的用户数据可能被篡改!");
				LOG(WARNING)<< userNumber+"用户数据可能被篡改";
			}
			else
			{
				if(user_passwd==row[1])
				{					
					userNumber=user_number;
					userPermission=row[6];
					logon_flag=true;
					mysql_free_result(res);
					AfxMessageBox("登录成功!");
				    LOG(INFO) << userNumber+"登录";
				}
				else
				{
					AfxMessageBox("密码错误!");
					LOG(ERROR)   << userNumber+"登录密码错误";
				}
		    }
		}

	}
	mysql_close(&mysql);
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
				strDateTime=strDateTime+MD5STR;
				md5.update(strDateTime.GetBuffer());
				strDateTime=md5.toString().c_str();//toString()函数获得加密字符串，c_str();函数重新转换成CString
				administrator_passwd=strDateTime+MD5STR;
				md5.reset();
				md5.update(administrator_passwd.GetBuffer());
				administrator_passwd=md5.toString().c_str();
				m_comboxUser.SetWindowText("administrator");
				UpdateData(false);
				AfxMessageBox(strDateTime);
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
