// IOSetDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "IOSetDialog.h"
#include "afxdialogex.h"
#include "Afxsock.h"
//mysql��������������ͷ�ļ�  
#include "winsock.h"  
//mysqlͷ�ļ���ȻҪ����    
#include "mysql.h"
#include "easylogging++.h"

// CIOSetDialog �Ի���

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


// CIOSetDialog ��Ϣ�������


BOOL CIOSetDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	select_sql="select * from sys_para where para_name='signalequipment'";
	int ress=mysql_query(&mysql,(char*)(LPCSTR)select_sql);
	if(ress==0) //����ѯ�ɹ�
	{
		res = mysql_store_result(&mysql);
		int resnum=mysql_num_rows(res);
		if(resnum==0) //��ѯ���Ϊ��
		{
			AfxMessageBox("���ڻ�û���������ݣ�");
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
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CIOSetDialog::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSocket mysocket;
	if (!mysocket.Create()) //�����׽���
	{
		AfxMessageBox("�׽��ִ���ʧ��!");
		LOG(ERROR)<< "�׽��ִ���ʧ��";
		return;
	}
	UpdateData(TRUE);
	unsigned  char   *pIP; 
	CString  strIP; 
	pIP  =   (unsigned   char*)&m_ip; 
	strIP.Format("%u.%u.%u.%u",*(pIP+3),   *(pIP+2),  *(pIP+1),   *pIP);
	if (!mysocket.Connect(strIP,atoi(m_port))) //���ӷ�����
	{
		AfxMessageBox("�����źŷ�����ʧ��!");
		LOG(ERROR)<< "�����źŷ�����ʧ��";
	}
	else
	{
		AfxMessageBox("�����źŷ������ɹ�!");
		LOG(ERROR)<< "�����źŷ������ɹ�";
	}
}


void CIOSetDialog::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
