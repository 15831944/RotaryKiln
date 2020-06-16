#pragma once
#include "afxwin.h"
#include "Global.h"

// CLogon 对话框

class CLogon : public CDialogEx
{
	DECLARE_DYNAMIC(CLogon)

public:
	CLogon(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogon();

	// 对话框数据
	enum { IDD = IDD_LOGON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString user_number;
	CString user_passwd;
	int usertype;
	CString m_password;
	CComboBox m_comboxUser;
	BOOL autologon_flag;
	afx_msg void OnBnClickedCheckAutologon();
	afx_msg void OnBnClickedButtonLogon();
	afx_msg void OnBnClickedButtonCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
};
