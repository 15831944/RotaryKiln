#pragma once

// CSplicingDialog �Ի���

class CSplicingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSplicingDialog)

public:
	CSplicingDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSplicingDialog();

	// �Ի�������
	enum { IDD = IDD_VIEW_SPLICING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
