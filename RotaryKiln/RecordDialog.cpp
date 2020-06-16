// RecordDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "RecordDialog.h"
#include "afxdialogex.h"

// RecordDialog 对话框

IMPLEMENT_DYNAMIC(RecordDialog, CDialogEx)

RecordDialog::RecordDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(RecordDialog::IDD, pParent)
{
}

RecordDialog::~RecordDialog()
{
}

void RecordDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_edit);
}

BEGIN_MESSAGE_MAP(RecordDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &RecordDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &RecordDialog::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &RecordDialog::OnBnClickedButton6)
END_MESSAGE_MAP()

// RecordDialog 消息处理程序

void RecordDialog::ReadData(CString filename)
{
	CString strText;
	FILE* fp = fopen(filename, "r");
	char ch;
	while ((ch = fgetc(fp)) != EOF)
	{
		if (ch == '\n')
			strText += '\r';
		strText += ch;
	}
	fclose(fp);
	m_edit.SetWindowText(strText);
}

void RecordDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	/*CFileDialog FileDlg(TRUE,
		"",
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"(*.*)||", this);
	if(FileDlg.DoModal()==IDCANCEL)
	{
		return;
	}
	CString filename=FileDlg.GetPathName();*/
	ReadData("logs/info.log");
}

void RecordDialog::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadData("logs/warn.log");
}

void RecordDialog::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadData("logs/error.log");
}