// AddThermalDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "AddThermalDialog.h"
#include "afxdialogex.h"

#include "SQLConnect.hpp"

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
	CString sql_command;
	UpdateData(TRUE);
	unsigned  char* pIP;
	CString strIP;
	pIP = (unsigned char*)&m_ip;
	strIP.Format("%u.%u.%u.%u", *(pIP + 3), *(pIP + 2), *(pIP + 1), *pIP);
	sql_command.Format("update sys_para set para0=\'%s\',para1=\'%s\', para2=\'%s\' ,  para3=\'%s\' , para4=\'%s\' where para_name='thermalequipment'", m_thermalname, strIP, m_port, m_name, m_psd);

	if (SUCCEEDED(accessConnect.execute(sql_command.GetString())))
		AfxMessageBox("添加成功！");
	else
		AfxMessageBox("添加失败！");
}
