#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CIOSetDialog �Ի���

class CIOSetDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CIOSetDialog)

public:
	CIOSetDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIOSetDialog();

	// �Ի�������
	enum { IDD = IDD_SET_THERMAL_IOPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//CEdit m_port;
	DWORD m_ip;
	//CEdit m_passwd;
	//CEdit m_splicingnumber;
	virtual BOOL OnInitDialog();
	CSpinButtonCtrl m_spin;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CString m_port;
	CString m_passwd;
	CString m_splicingnumber;
};
