// CCopyDialog.cpp: 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "CCopyDialog.h"
#include "afxdialogex.h"


// CCopyDialog 对话框

IMPLEMENT_DYNAMIC(CCopyDialog, CDialogEx)

CCopyDialog::CCopyDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COPYDIALOG, pParent)
{

}

CCopyDialog::~CCopyDialog()
{
}

void CCopyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCopyDialog, CDialogEx)
END_MESSAGE_MAP()


// CCopyDialog 消息处理程序

void ShowInfoDialog(CString info)
{
	CCopyDialog dlg;
	dlg.info = info;
	dlg.DoModal();
}


BOOL CCopyDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDlgItemText(IDC_INFO, info);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
