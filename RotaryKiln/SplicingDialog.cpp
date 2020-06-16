// SplicingDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "SplicingDialog.h"
#include "afxdialogex.h"

extern CSplicingDialog* p_splicingdialog;
extern ATL::CImage m_splicing_image;
// CSplicingDialog 对话框

IMPLEMENT_DYNAMIC(CSplicingDialog, CDialogEx)

CSplicingDialog::CSplicingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSplicingDialog::IDD, pParent)
{
	p_splicingdialog = this;
}

CSplicingDialog::~CSplicingDialog()
{
}

void CSplicingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSplicingDialog, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSplicingDialog 消息处理程序

void CSplicingDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CSplicingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化

	//SetTimer(1,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}