// SplicingDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "SplicingDialog.h"
#include "afxdialogex.h"

extern CSplicingDialog* p_splicingdialog;
extern ATL::CImage m_splicing_image;
// CSplicingDialog �Ի���

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

// CSplicingDialog ��Ϣ�������

void CSplicingDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CSplicingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//SetTimer(1,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}