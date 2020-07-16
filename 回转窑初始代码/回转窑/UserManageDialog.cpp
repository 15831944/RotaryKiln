// UserManageDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "UserManageDialog.h"
#include "afxdialogex.h"
#include "AddModiUserDialog.h"
#include "md5.h"
//mysql��������������ͷ�ļ�  
#include "winsock.h"  
//mysqlͷ�ļ���ȻҪ����    
#include "mysql.h"
#include "easylogging++.h"


#define MD5STR "�Ͼ�����ѧԺ���㹤��ѧԺ����"

// UserManageDialog �Ի���
CString strusername;
extern CString userPermission;

IMPLEMENT_DYNAMIC(UserManageDialog, CDialogEx)

UserManageDialog::UserManageDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(UserManageDialog::IDD, pParent)
{

}

UserManageDialog::~UserManageDialog()
{
}

void UserManageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listctrl);
}


BEGIN_MESSAGE_MAP(UserManageDialog, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &UserManageDialog::OnRclickList1)
	ON_COMMAND(ID_32792, &UserManageDialog::On32792)
	ON_COMMAND(ID_32790, &UserManageDialog::On32790)
	ON_COMMAND(ID_32791, &UserManageDialog::On32791)
	ON_COMMAND(ID_32793, &UserManageDialog::On32793)
	ON_COMMAND(ID_32794, &UserManageDialog::On32794)
END_MESSAGE_MAP()


// UserManageDialog ��Ϣ�������


BOOL UserManageDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��

	m_listctrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	CRect rect;
	// ��ȡ�ؼ���λ�úʹ�С   
	m_listctrl.GetClientRect(&rect);   
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	//m_listctrl->SetExtendedStyle(m_listctrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  
	// Ϊ�б���ͼ�ؼ��������   
	int nIndex = 0;
	m_listctrl.InsertColumn(nIndex++, "����", LVCFMT_CENTER, rect.Width()/5-40);   
	m_listctrl.InsertColumn(nIndex++, "�û���", LVCFMT_CENTER, rect.Width()/5+10); 
	m_listctrl.InsertColumn(nIndex++, "����", LVCFMT_CENTER, rect.Width()/5+10); 
	m_listctrl.InsertColumn(nIndex++, "����", LVCFMT_CENTER, rect.Width()/5+10);  
	m_listctrl.InsertColumn(nIndex++, "״̬", LVCFMT_CENTER, rect.Width()/5+10);  
	
	initData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void UserManageDialog::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	CMenu menu;
	//����̲߳���
	VERIFY(menu.LoadMenu(IDR_MENU_USER));			//�����Ƕ����MENU���ļ�����
	CMenu* popup = menu.GetSubMenu(0);
	ASSERT(popup != NULL);
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	strusername="";
	if (pNMListView->iItem != -1)
	{
		//��������д�����Ҫ��Ϊ�˺���Ĳ���Ϊ׼����
		//��ȡ�б���ͼ�ؼ��е�һ����ѡ�����λ��  
		POSITION m_pstion = m_listctrl.GetFirstSelectedItemPosition();
	    strusername= m_listctrl.GetItemText(m_listctrl.GetNextSelectedItem(m_pstion),1);
		//�ú�����ȡ��posָ�����б����������Ȼ��pos����Ϊ��һ��λ�õ�POSITIONֵ
		//m_alarmsetdialog.RegionIndex= m_listctrl.GetNextSelectedItem(m_pstion);
       }
}

//ɾ���û�
void UserManageDialog::On32792()
{
	// TODO: �ڴ���������������
	if(strusername=="")
	{
		AfxMessageBox("��û��ѡ���û���");
		return;
	}
	if(-1==userPermission.Find('F'))
	{
		AfxMessageBox("���Ȩ�޲��������л��û���");
		return;
	}
	if(2==AfxMessageBox(_T("��ȷ��Ҫɾ���û���"), MB_OKCANCEL | MB_ICONQUESTION, 0))
		return;
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
	sql_command.Format("delete  from user_info where  user_number=\'%s\'",strusername);
	int ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
	if(ress)
	{
		AfxMessageBox("����ʧ��!");
		return;
	}
	mysql_close(&mysql);
	AfxMessageBox("�����ɹ�!");
	LOG(INFO) << "ɾ���û�:"+strusername;
	initData();
	return;
}
//�����û�
void UserManageDialog::On32793()
{
	// TODO: �ڴ���������������
	if(strusername=="")
	{
		AfxMessageBox("��û��ѡ���û���");
		return;
	}
	if(-1==userPermission.Find('E'))
	{
		AfxMessageBox("���Ȩ�޲��������л��û���");
		return;
	}
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
	sql_command.Format("update user_info set user_state=0 where  user_number=\'%s\'",strusername);
	int ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
	if(ress)
	{
		AfxMessageBox("����ʧ��!");
		return;
	}
	sql_command.Format("select * from user_info where user_number= \'%s\'",strusername);
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
			MD5 md5;
			md5.reset();
			md5.update(strveri.GetBuffer());
			strveri=md5.toString().c_str();
		}
		mysql_free_result(res);
	}

	sql_command.Format("update user_info set verification=\'%s\' where  user_number=\'%s\'",strveri,strusername);
	ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
	if(ress)
	{
		AfxMessageBox("����ʧ��!");
		return;
	}

	mysql_close(&mysql);
	AfxMessageBox("�����ɹ�!");
	LOG(INFO) << "�����û�:"+strusername;
	initData();
	return;
}
//�����û�
void UserManageDialog::On32794()
{
	// TODO: �ڴ���������������
	if(strusername=="")
	{
		AfxMessageBox("��û��ѡ���û���");
		return;
	}
	if(-1==userPermission.Find('G'))
	{
		AfxMessageBox("���Ȩ�޲��������л��û���");
		return;
	}
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
	sql_command.Format("update user_info set user_state=1 where  user_number=\'%s\'",strusername);
	int ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
	if(ress)
	{
		AfxMessageBox("����ʧ��!");
		return;
	}
	sql_command.Format("select * from user_info where user_number= \'%s\'",strusername);
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
			MD5 md5;
			md5.reset();
			md5.update(strveri.GetBuffer());
			strveri=md5.toString().c_str();
		}
		mysql_free_result(res);
	}

	sql_command.Format("update user_info set verification=\'%s\' where  user_number=\'%s\'",strveri,strusername);
	ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
	if(ress)
	{
		AfxMessageBox("����ʧ��!");
		return;
	}

	mysql_close(&mysql);
	AfxMessageBox("�����ɹ�!");
	LOG(INFO) << "�����û�:"+strusername;
	initData();
	return;
}

//����û�
void UserManageDialog::On32790()
{
	// TODO: �ڴ���������������
	AddModiUserDialog adduserdialog;
	adduserdialog.usernumber="";
	adduserdialog.DoModal();
	initData();
	return;
}

//�޸��û�
void UserManageDialog::On32791()
{
	// TODO: �ڴ���������������
	if(strusername=="")
	{
		AfxMessageBox("��û��ѡ���û���");
		return;
	}
	AddModiUserDialog modiuserdialog;
	modiuserdialog.usernumber=strusername;
	modiuserdialog.DoModal();
	initData();
	return;
}



void UserManageDialog::initData(void)
{
	m_listctrl.DeleteAllItems();
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
		//return;
	}

	CString select_sql;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	//select_sql="select * from user_info where user_state=1";
	select_sql="select * from user_info";
	int ress=mysql_query(&mysql,(char*)(LPCSTR)select_sql);
	if(ress==0) //����ѯ�ɹ�
	{
		res = mysql_store_result(&mysql);
		int resnum=mysql_num_rows(res);
		if(resnum==0) //��ѯ���Ϊ��
		{
			AfxMessageBox("���ڻ�û���û����ݣ�");
		}
		else
		{
			//while((row=mysql_fetch_row(res)))
			CString strindex;
			for(int i=0;i<resnum;i++)
			{
				row=mysql_fetch_row(res);
				strindex.Format("%d",i);
				m_listctrl.InsertItem(i,strindex);//����һ��
				m_listctrl.SetItemText(i,1,row[0]);//�ڵ�һ�������õڶ��е�����
				m_listctrl.SetItemText(i,2,row[2]);
				m_listctrl.SetItemText(i,3,row[3]);
				CString str=row[11];
				if(str=="1")
					m_listctrl.SetItemText(i,4,"����");
				else
					m_listctrl.SetItemText(i,4,"����");
			}		
		}

	}
	mysql_close(&mysql);
	UpdateData(FALSE);
}


