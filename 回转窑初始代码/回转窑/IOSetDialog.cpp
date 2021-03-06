// IOSetDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "IOSetDialog.h"
#include "afxdialogex.h"
#include "Afxsock.h"
//mysql必须包含网络相关头文件  
#include "winsock.h"  
//mysql头文件自然要包含    
#include "mysql.h"
#include "easylogging++.h"

// CIOSetDialog 对话框

IMPLEMENT_DYNAMIC(CIOSetDialog, CDialogEx)

CIOSetDialog::CIOSetDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIOSetDialog::IDD, pParent)
	, m_ip(0)
	, m_port(_T(""))
	, m_passwd(_T(""))
	, m_splicingnumber(_T(""))
{

}

CIOSetDialog::~CIOSetDialog()
{
}

void CIOSetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_ip);
	DDX_Control(pDX, IDC_SPIN1, m_spin);
	DDX_Text(pDX, IDC_EDIT_IOPORT, m_port);
	DDX_Text(pDX, IDC_EDIT_IOPASSWORD, m_passwd);
	DDX_Text(pDX, IDC_EDIT_SPLICINGNUMBER, m_splicingnumber);
}


BEGIN_MESSAGE_MAP(CIOSetDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CIOSetDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CIOSetDialog::OnBnClickedButton2)
END_MESSAGE_MAP()


// CIOSetDialog 消息处理程序


BOOL CIOSetDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
		MessageBox(e);  
		//return;
	}

	CString select_sql;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	select_sql="select * from sys_para where para_name='signalequipment'";
	int ress=mysql_query(&mysql,(char*)(LPCSTR)select_sql);
	if(ress==0) //检测查询成功
	{
		res = mysql_store_result(&mysql);
		int resnum=mysql_num_rows(res);
		if(resnum==0) //查询结果为空
		{
			AfxMessageBox("现在还没有区域数据！");
		}
		else
		{
			//while((row=mysql_fetch_row(res)))
			for(int i=0;i<resnum;i++)
			{
				row=mysql_fetch_row(res);
				m_ip= ntohl(inet_addr(row[2]));
				m_port=row[3];
				m_passwd=row[4];
				m_splicingnumber=row[5];
			}		
		}

	}
	mysql_close(&mysql);
	


    m_spin.SetRange32(0,20);
	m_spin.SetBase(atoi(m_splicingnumber));
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CIOSetDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CSocket mysocket;
	if (!mysocket.Create()) //创建套接字
	{
		AfxMessageBox("套接字创建失败!");
		LOG(ERROR)<< "套接字创建失败";
		return;
	}
	UpdateData(TRUE);
	unsigned  char   *pIP; 
	CString  strIP; 
	pIP  =   (unsigned   char*)&m_ip; 
	strIP.Format("%u.%u.%u.%u",*(pIP+3),   *(pIP+2),  *(pIP+1),   *pIP);
	if (!mysocket.Connect(strIP,atoi(m_port))) //连接服务器
	{
		AfxMessageBox("连接信号服务器失败!");
		LOG(ERROR)<< "连接信号服务器失败";
	}
	else
	{
		AfxMessageBox("连接信号服务器成功!");
		LOG(ERROR)<< "连接信号服务器成功";
	}
}


void CIOSetDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
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
		MessageBox(e);  
		//return;
	}

	CString sql_command;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	//select_sql="select * from sys_para where para_name='signalequipment'";
    UpdateData(TRUE);
	unsigned  char   *pIP; 
	CString  strIP; 
	pIP  =   (unsigned   char*)&m_ip; 
	strIP.Format("%u.%u.%u.%u",*(pIP+3),   *(pIP+2),  *(pIP+1),   *pIP);
	sql_command.Format("update sys_para set para0=\'%s\', para1=\'%s\' ,  para2=\'%s\' , para3=\'%s\' where para_name='signalequipment'",strIP,m_port,m_passwd,m_splicingnumber);
	int ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
	mysql_close(&mysql);
}
