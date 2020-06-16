// IOSetDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "IOSetDialog.h"
#include "afxdialogex.h"
#include "Afxsock.h"

#include "SQLConnect.hpp"
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

	AccessResult res;

	if (SUCCEEDED(accessConnect.select("select * from sys_para where para_name='signalequipment'", res))) //����ѯ�ɹ�
	{
		if (res.empty()) //��ѯ���Ϊ��
		{
			AfxMessageBox("���ڻ�û���������ݣ�");
		}
		else
		{
			for (auto& record : res)
			{
				m_ip = ntohl(inet_addr(record["para0"].c_str()));
				m_port = record["para1"].c_str();
				m_passwd = record["para2"].c_str();
				m_splicingnumber = record["para3"].c_str();
			}
		}
	}

	m_spin.SetRange32(0, 20);
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
		LOG(ERROR) << "�׽��ִ���ʧ��";
		return;
	}
	UpdateData(TRUE);
	unsigned  char* pIP;
	CString  strIP;
	pIP = (unsigned   char*)&m_ip;
	strIP.Format("%u.%u.%u.%u", *(pIP + 3), *(pIP + 2), *(pIP + 1), *pIP);
	if (!mysocket.Connect(strIP, atoi(m_port))) //���ӷ�����
	{
		AfxMessageBox("�����źŷ�����ʧ��!");
		LOG(ERROR) << "�����źŷ�����ʧ��";
	}
	else
	{
		AfxMessageBox("�����źŷ������ɹ�!");
		LOG(ERROR) << "�����źŷ������ɹ�";
	}
}

void CIOSetDialog::OnBnClickedButton2()
{
	CString sql_command;
	//select_sql_by_user.Format("execute user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	//select_sql="execute * from sys_para where para_name='signalequipment'";
	UpdateData(TRUE);
	unsigned  char* pIP;
	CString  strIP;
	pIP = (unsigned   char*)&m_ip;
	strIP.Format("%u.%u.%u.%u", *(pIP + 3), *(pIP + 2), *(pIP + 1), *pIP);
	sql_command.Format("update sys_para set para0=\'%s\', para1=\'%s\' ,  para2=\'%s\' , para3=\'%s\' where para_name='signalequipment'", strIP, m_port, m_passwd, m_splicingnumber);
	accessConnect.execute(sql_command.GetString());
}