// AddThermalDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "AddThermalDialog.h"
#include "afxdialogex.h"
//mysql必须包含网络相关头文件  
#include "winsock.h"  
//mysql头文件自然要包含    
#include "mysql.h"

// AddThermalDialog 对话框

IMPLEMENT_DYNAMIC(AddThermalDialog, CDialogEx)

AddThermalDialog::AddThermalDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(AddThermalDialog::IDD, pParent)
	, m_ip(0)
	, m_port(_T(""))
	, m_name(_T(""))
	, m_psd(_T(""))
	, m_thermalname(_T(""))
{

}

AddThermalDialog::~AddThermalDialog()
{
}

void AddThermalDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_ip);
	DDX_Text(pDX, IDC_EDIT5, m_port);
	DDX_Text(pDX, IDC_EDIT2, m_name);
	DDX_Text(pDX, IDC_EDIT7, m_psd);
	DDX_Text(pDX, IDC_EDIT1, m_thermalname);
}


BEGIN_MESSAGE_MAP(AddThermalDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &AddThermalDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// AddThermalDialog 消息处理程序


void AddThermalDialog::OnBnClickedButton1()
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
	sql_command.Format("update sys_para set para0=\'%s\',para1=\'%s\', para2=\'%s\' ,  para3=\'%s\' , para4=\'%s\' where para_name='thermalequipment'",m_thermalname,strIP,m_port,m_name,m_psd);
	int ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
	mysql_close(&mysql);
	if(ress==0)
	{
		AfxMessageBox("添加成功！");
	}
	else
	{
		AfxMessageBox("添加失败！");
	}
}
