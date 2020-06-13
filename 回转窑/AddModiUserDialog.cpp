// AddModiUserDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "AddModiUserDialog.h"
#include "afxdialogex.h"
#include "md5.h"

#include "SQLConnect.hpp"
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
		CString select_sql_by_user;
		//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
		select_sql_by_user.Format("select * from user_info where user_number= \'%s\'",usernumber);
		SQLResult res;
		if(accessConnect.executeSQL(select_sql_by_user.GetString(), res) ==S_OK) //����ѯ�ɹ�
		{
			if(res.empty()|| res.begin()->second.empty()) //��ѯ���Ϊ��
				AfxMessageBox("�û�������");
			else
			{
				m_usernumber = usernumber;
				m_usertypevalue = res["user_type"].front().c_str();
				m_username = res["user_name"].front().c_str();
				CString strqx = res["user_permission"].front().c_str();
				for (int i = 0; i < QXNUM; i++)
				{
					if (strqx.Find(65 + i) > -1)
						m_check[i] = true;
				}
				m_usercontrol.SetReadOnly(TRUE);
				UpdateData(FALSE);
				AfxMessageBox("����Ϊ�ս����޸����룡");
			}

		}
	}
	SQLResult res;
	CString select_sql_by_user;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	select_sql_by_user.Format("select * from user_info where user_number= \'%s\'",userNumber);
	if(accessConnect.executeSQL(select_sql_by_user.GetString(), res) ==S_OK) //����ѯ�ɹ�
	{
		if(res.empty() || res.begin()->second.empty()) //��ѯ���Ϊ��
		{
			AfxMessageBox("�û�������");
		}
		else
		{
			CString strqx= res["user_permission"].front().c_str();
			for(int i=0;i<QXNUM;i++)
			{
				if(strqx.Find (65+i)==-1)
					m_checkcontrol[i].EnableWindow(FALSE);
			}
			if(m_check[3])
				m_checkcontrol[3].EnableWindow(FALSE);
			UpdateData(FALSE);		
		}

	}
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
	CString user_passwd = m_usernumber + m_userpwd + MD5STR + std::to_string(MachineKey).c_str();
	md5.update(user_passwd.GetBuffer());
	user_passwd=md5.toString().c_str();

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
		
		if (accessConnect.executeSQL(sql_command.GetString()) != S_OK)
		{
			AfxMessageBox(("����ʧ��: " + accessConnect.getLastError()).c_str());
			LOG(ERROR) << "����û�ʧ�� " << accessConnect.getLastError();
			return;
		}
	}
	else
	{
		if(m_userpwd=="")
			sql_command.Format("update user_info set user_permission=\'%s\',user_type=\'%s\',user_name=\'%s\' where  user_number=\'%s\'",strqx,m_usertypevalue,m_username,m_usernumber);
		else
			sql_command.Format("update user_info set user_permission=\'%s\',user_passwd=\'%s\',user_type=\'%s\',user_name=\'%s\' where  user_number=\'%s\'",strqx,user_passwd,m_usertypevalue,m_username,m_usernumber);
		if(accessConnect.executeSQL(sql_command.GetString()) != S_OK)
		{
			AfxMessageBox(("����ʧ��: " + accessConnect.getLastError()).c_str());
			LOG(ERROR) << "�޸��û�ʧ��" << accessConnect.getLastError();
			return;
		}
		
	}
	AfxMessageBox("�����ɹ�!");
	LOG(INFO)<< "�û������ɹ�";
}
