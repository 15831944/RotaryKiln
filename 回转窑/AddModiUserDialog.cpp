// AddModiUserDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "AddModiUserDialog.h"
#include "afxdialogex.h"
#include "md5.h"
//mysql必须包含网络相关头文件  
#include "winsock.h"  
//mysql头文件自然要包含    
#include "mysql.h"
#include "easylogging++.h"


#define MD5STR "南京工程学院计算工程学院王杰"


extern CString userNumber;

// AddModiUserDialog 对话框

IMPLEMENT_DYNAMIC(AddModiUserDialog, CDialogEx)

AddModiUserDialog::AddModiUserDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(AddModiUserDialog::IDD, pParent)
	, m_username(_T(""))
	, m_userpwd(_T(""))
	, m_usernumber(_T(""))
	, m_usertypevalue(_T(""))
{
	memset(m_check,false,sizeof(bool)*QXNUM);
}

AddModiUserDialog::~AddModiUserDialog()
{
}

void AddModiUserDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_usertype);
	DDX_Check(pDX, IDC_CHECK1, m_check[0]);
	DDX_Check(pDX, IDC_CHECK2, m_check[1]);
	DDX_Check(pDX, IDC_CHECK3, m_check[2]);
	DDX_Check(pDX, IDC_CHECK4, m_check[3]);
	DDX_Check(pDX, IDC_CHECK5, m_check[4]);
	DDX_Check(pDX, IDC_CHECK6, m_check[5]);
	DDX_Check(pDX, IDC_CHECK7, m_check[6]);
	DDX_Check(pDX, IDC_CHECK8, m_check[7]);
	DDX_Check(pDX, IDC_CHECK9, m_check[8]);
	DDX_Check(pDX, IDC_CHECK10, m_check[9]);
	DDX_Control(pDX, IDC_CHECK1, m_checkcontrol[0]);
	DDX_Control(pDX, IDC_CHECK2, m_checkcontrol[1]);
	DDX_Control(pDX, IDC_CHECK3, m_checkcontrol[2]);
	DDX_Control(pDX, IDC_CHECK4, m_checkcontrol[3]);
	DDX_Control(pDX, IDC_CHECK5, m_checkcontrol[4]);
	DDX_Control(pDX, IDC_CHECK6, m_checkcontrol[5]);
	DDX_Control(pDX, IDC_CHECK7, m_checkcontrol[6]);
	DDX_Control(pDX, IDC_CHECK8, m_checkcontrol[7]);
	DDX_Control(pDX, IDC_CHECK9, m_checkcontrol[8]);
	DDX_Control(pDX, IDC_CHECK10, m_checkcontrol[9]);
	DDX_Text(pDX, IDC_EDIT1, m_usernumber);
	DDX_Text(pDX, IDC_EDIT5, m_userpwd);
	DDX_Text(pDX, IDC_EDIT2, m_username);
	DDX_CBString(pDX, IDC_COMBO1, m_usertypevalue);
	DDX_Control(pDX, IDC_EDIT1, m_usercontrol);
	
}


BEGIN_MESSAGE_MAP(AddModiUserDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &AddModiUserDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// AddModiUserDialog 消息处理程序


BOOL AddModiUserDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_usertype.InsertString(0,"管理员");
	m_usertype.InsertString(1,"操作员");
	m_usertype.InsertString(2,"普通人员");
	if(usernumber=="")
	{
		SetWindowText(_T("添加用户"));
		SetDlgItemText(IDC_BUTTON1, _T("添加"));
	}
	else
	{
		//读取数据库
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
			return FALSE;
		}
		CString select_sql_by_user;
		//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
		select_sql_by_user.Format("select * from user_info where user_number= \'%s\'",usernumber);
		int ress=mysql_query(&mysql,(char*)(LPCSTR)select_sql_by_user);
		if(ress==0) //检测查询成功
		{
			res = mysql_store_result(&mysql);
			if(mysql_num_rows(res)==0) //查询结果为空
			{
				AfxMessageBox("用户不存在");
			}
			else
			{
				MD5 md5;
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
					EndDialog(0);
				}
				else
				{
					{					
						m_usernumber=usernumber;
						m_usertypevalue=row[2];
						m_username=row[3];
						CString strqx=row[6];
						for(int i=0;i<QXNUM;i++)
						{
                           if(strqx.Find (65+i)>-1)
							   m_check[i]=true;
						}
						m_usercontrol.SetReadOnly(TRUE);
						mysql_free_result(res);
						UpdateData(FALSE);
						AfxMessageBox("密码为空将不修改密码！");
					}
				}
			}

		}
		mysql_close(&mysql);
	}
	//读取数据库
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
		return FALSE;
	}
	CString select_sql_by_user;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	select_sql_by_user.Format("select * from user_info where user_number= \'%s\'",userNumber);
	int ress=mysql_query(&mysql,(char*)(LPCSTR)select_sql_by_user);
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
			CString strqx=row[6];
			for(int i=0;i<QXNUM;i++)
			{
				if(strqx.Find (65+i)==-1)
					m_checkcontrol[i].EnableWindow(FALSE);
			}
			if(m_check[3])
				m_checkcontrol[3].EnableWindow(FALSE);
			mysql_free_result(res);
			UpdateData(FALSE);		
		}

	}
	mysql_close(&mysql);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void AddModiUserDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取用户权限
	UpdateData(TRUE);
	CString strqx="";
	for(int i=0;i<QXNUM;i++)
	{
		if(!m_check[i]) continue;
		CString strcheck;
		strcheck.Format("%c",65+i);
		strqx=strqx+strcheck;
	}
	MD5 md5;
	CString user_passwd=m_userpwd+MD5STR;
	md5.update(user_passwd.GetBuffer());
	user_passwd=md5.toString().c_str();
	//写入数据库
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
		return;
	}

	CString sql_command;
	int ress;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	if(usernumber=="")
	{
		//sql_command.Format("insert into region_info (region_index,region_name,region_left,region_right,region_top,region_bottom,region_emissivity)  values(%s,\'%s\',%s,%s,%s,%s,%s)",m_listctrl.GetItemText(i, 0),m_listctrl.GetItemText(i, 1),m_listctrl.GetItemText(i, 2),m_listctrl.GetItemText(i, 3),m_listctrl.GetItemText(i,4),m_listctrl.GetItemText(i,5),m_listctrl.GetItemText(i,6));
		//sql_command.Format("insert into user_info (user_permission,user_passwd,user_type,user_name,add_datetime) values(\'%s\',\'%s\',\'%s\',\'%s\',current_timestamp)",strqx,user_passwd,m_usertypevalue,m_username);
		if(m_usernumber=="")
		{
			AfxMessageBox("用户名不能为空!");
			return;
		}
		sql_command.Format("insert into user_info (user_permission,user_passwd,user_type,user_name,user_number,add_user_number) values(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')",strqx,user_passwd,m_usertypevalue,m_username,m_usernumber,userNumber);
		ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
		if(ress)
		{
			AfxMessageBox("操作失败，可能用户名冲突!");
			LOG(ERROR)<< "添加用户失败";
		    return;
		}
	}
	else
	{
		if(m_userpwd=="")
			sql_command.Format("update user_info set user_permission=\'%s\',user_type=\'%s\',user_name=\'%s\' where  user_number=\'%s\'",strqx,m_usertypevalue,m_username,m_usernumber);
		else
			sql_command.Format("update user_info set user_permission=\'%s\',user_passwd=\'%s\',user_type=\'%s\',user_name=\'%s\' where  user_number=\'%s\'",strqx,user_passwd,m_usertypevalue,m_username,m_usernumber);
		ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
		if(ress)
		{
			AfxMessageBox("操作失败!");
			LOG(ERROR)<< "修改用户失败";
			return;
		}
		
	}
	sql_command.Format("select * from user_info where user_number= \'%s\'",m_usernumber);
	ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
	if(ress)
	{
		AfxMessageBox("操作失败!");
		return;
	}
	CString strveri="";
	if(ress==0) //检测查询成功
	{
		res = mysql_store_result(&mysql);
		if(mysql_num_rows(res)==0) //查询结果为空
		{
			AfxMessageBox("用户不存在");
			return;
		}
		else
		{

			row=mysql_fetch_row(res);
			for(int i=0;i<11;i++)
				strveri+=row[i];
			md5.reset();
			md5.update(strveri.GetBuffer());
			strveri=md5.toString().c_str();
		}
		mysql_free_result(res);
	}

	sql_command.Format("update user_info set verification=\'%s\' where  user_number=\'%s\'",strveri,m_usernumber);
	ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
	if(ress)
	{
		AfxMessageBox("操作失败!");
		return;
	}
	mysql_close(&mysql);
	AfxMessageBox("操作成功!");
	LOG(INFO)<< "用户操作成功";
}
