// AddThermalDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "AddThermalDialog.h"
#include "afxdialogex.h"
//mysql��������������ͷ�ļ�  
#include "winsock.h"  
//mysqlͷ�ļ���ȻҪ����    
#include "mysql.h"

// AddThermalDialog �Ի���

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


// AddThermalDialog ��Ϣ�������


void AddThermalDialog::OnBnClickedButton1()
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
	sql_command.Format("update sys_para set para0=\'%s\',para1=\'%s\', para2=\'%s\' ,  para3=\'%s\' , para4=\'%s\' where para_name='thermalequipment'",m_thermalname,strIP,m_port,m_name,m_psd);
	int ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
	mysql_close(&mysql);
	if(ress==0)
	{
		AfxMessageBox("��ӳɹ���");
	}
	else
	{
		AfxMessageBox("���ʧ�ܣ�");
	}
}
