#pragma once
#include "afxwin.h"


// RecordDialog �Ի���

class RecordDialog : public CDialogEx
{
	DECLARE_DYNAMIC(RecordDialog)

public:
	RecordDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~RecordDialog();
	void ReadData(CString filename);
// �Ի�������
	enum { IDD = IDD_DIALOG_RECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
