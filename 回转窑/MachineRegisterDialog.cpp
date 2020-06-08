// MachineRegisterDialog.cpp: 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "MachineRegisterDialog.h"
#include "afxdialogex.h"
#include "MachineRegistrationClient.h"


// MachineRegisterDialog 对话框

IMPLEMENT_DYNAMIC(MachineRegisterDialog, CDialogEx)

MachineRegisterDialog::MachineRegisterDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGDIALOG, pParent)
{
}

MachineRegisterDialog::~MachineRegisterDialog()
{
}

void MachineRegisterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MachineRegisterDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &MachineRegisterDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// MachineRegisterDialog 消息处理程序


BOOL MachineRegisterDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 7D347-1E848-A43E7-AAD9A
	SetDlgItemText(IDC_MACHINEID, QueryMachineId());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void MachineRegisterDialog::OnBnClickedOk()
{
	const char envirent[] = "huizhuanyao";
	CString regCode;
	GetDlgItemText(IDC_REGCODE, regCode);
	regKeyValue = VerifyMachine(envirent, regCode.GetString());
	CDialogEx::OnOK();
}

int VerifyMachineDialog()
{
	MachineRegisterDialog dlg;
	dlg.DoModal();
	return dlg.regKeyValue;
}
