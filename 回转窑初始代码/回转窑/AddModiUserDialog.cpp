// AddModiUserDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "AddModiUserDialog.h"
#include "afxdialogex.h"
#include "md5.h"
//mysql��������������ͷ�ļ�  
#include "winsock.h"  
//mysqlͷ�ļ���ȻҪ����    
#include "mysql.h"
#include "easylogging++.h"


#define MD5STR "�Ͼ�����ѧԺ���㹤��ѧԺ����"


extern CString userNumber;

// AddModiUserDialog �Ի���

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


// AddModiUserDialog ��Ϣ�������


BOOL AddModiUserDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_usertype.InsertString(0,"����Ա");
	m_usertype.InsertString(1,"����Ա");
	m_usertype.InsertString(2,"��ͨ��Ա");
	if(usernumber=="")
	{
		SetWindowText(_T("����û�"));
		SetDlgItemText(IDC_BUTTON1, _T("���"));
	}
	else
	{
		//��ȡ���ݿ�
		MYSQL mysql; //���ݿ����Ӿ��
		MYSQL_RES *res;
		MYSQL_ROW row;
		mysql_init(&mysql);
		//�������ݿ�����ʽ
		//	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
		//������ַ���""
		if(!mysql_real_connect(&mysql,"localhost","root","123456","mydb",3306,NULL,0))
		{//mydbΪ�������������ݿ⣬3306Ϊ�˿ںţ�root���û���,123456������
			AfxMessageBox("���ݿ�����ʧ��");
			CString e=mysql_error(&mysql);//��Ҫ����Ŀ�������ַ����޸�Ϊ��ʹ�ö��ֽ��ַ�������δ���á�  
			MessageBox(e);  
			return FALSE;
		}
		CString select_sql_by_user;
		//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
		select_sql_by_user.Format("select * from user_info where user_number= \'%s\'",usernumber);
		int ress=mysql_query(&mysql,(char*)(LPCSTR)select_sql_by_user);
		if(ress==0) //����ѯ�ɹ�
		{
			res = mysql_store_result(&mysql);
			if(mysql_num_rows(res)==0) //��ѯ���Ϊ��
			{
				AfxMessageBox("�û�������");
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
					AfxMessageBox("����У���������û����ݿ��ܱ��۸�!");
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
						AfxMessageBox("����Ϊ�ս����޸����룡");
					}
				}
			}

		}
		mysql_close(&mysql);
	}
	//��ȡ���ݿ�
	MYSQL mysql; //���ݿ����Ӿ��
	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_init(&mysql);
	//�������ݿ�����ʽ
	//	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//������ַ���""
	if(!mysql_real_connect(&mysql,"localhost","root","123456","mydb",3306,NULL,0))
	{//mydbΪ�������������ݿ⣬3306Ϊ�˿ںţ�root���û���,123456������
		AfxMessageBox("���ݿ�����ʧ��");
		CString e=mysql_error(&mysql);//��Ҫ����Ŀ�������ַ����޸�Ϊ��ʹ�ö��ֽ��ַ�������δ���á�  
		MessageBox(e);  
		return FALSE;
	}
	CString select_sql_by_user;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	select_sql_by_user.Format("select * from user_info where user_number= \'%s\'",userNumber);
	int ress=mysql_query(&mysql,(char*)(LPCSTR)select_sql_by_user);
	if(ress==0) //����ѯ�ɹ�
	{
		res = mysql_store_result(&mysql);
		if(mysql_num_rows(res)==0) //��ѯ���Ϊ��
		{
			AfxMessageBox("�û�������");
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
	// �쳣: OCX ����ҳӦ���� FALSE
}


void AddModiUserDialog::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�û�Ȩ��
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
	//д�����ݿ�
	MYSQL mysql; //���ݿ����Ӿ��
	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_init(&mysql);
	//�������ݿ�����ʽ
	//	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//������ַ���""
	if(!mysql_real_connect(&mysql,"localhost","root","123456","mydb",3306,NULL,0))
	{//mydbΪ�������������ݿ⣬3306Ϊ�˿ںţ�root���û���,123456������
		AfxMessageBox("���ݿ�����ʧ��");
		CString e=mysql_error(&mysql);//��Ҫ����Ŀ�������ַ����޸�Ϊ��ʹ�ö��ֽ��ַ�������δ���á�  
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
			AfxMessageBox("�û�������Ϊ��!");
			return;
		}
		sql_command.Format("insert into user_info (user_permission,user_passwd,user_type,user_name,user_number,add_user_number) values(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')",strqx,user_passwd,m_usertypevalue,m_username,m_usernumber,userNumber);
		ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
		if(ress)
		{
			AfxMessageBox("����ʧ�ܣ������û�����ͻ!");
			LOG(ERROR)<< "����û�ʧ��";
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
			AfxMessageBox("����ʧ��!");
			LOG(ERROR)<< "�޸��û�ʧ��";
			return;
		}
		
	}
	sql_command.Format("select * from user_info where user_number= \'%s\'",m_usernumber);
	ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
	if(ress)
	{
		AfxMessageBox("����ʧ��!");
		return;
	}
	CString strveri="";
	if(ress==0) //����ѯ�ɹ�
	{
		res = mysql_store_result(&mysql);
		if(mysql_num_rows(res)==0) //��ѯ���Ϊ��
		{
			AfxMessageBox("�û�������");
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
		AfxMessageBox("����ʧ��!");
		return;
	}
	mysql_close(&mysql);
	AfxMessageBox("�����ɹ�!");
	LOG(INFO)<< "�û������ɹ�";
}
