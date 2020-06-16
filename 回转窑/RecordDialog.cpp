// RecordDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "RecordDialog.h"
#include "afxdialogex.h"

// RecordDialog �Ի���

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

// RecordDialog ��Ϣ�������

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ReadData("logs/warn.log");
}

void RecordDialog::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ReadData("logs/error.log");
}