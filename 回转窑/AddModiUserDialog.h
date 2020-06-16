#pragma once
#include "afxwin.h"

#define QXNUM 10

// AddModiUserDialog 对话框

class AddModiUserDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AddModiUserDialog)

public:
	AddModiUserDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AddModiUserDialog();

	// 对话框数据
	enum { IDD = IDD_ADDMODIFY_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_usertype;
	CString usernumber;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	BOOL m_check[QXNUM];
	CString m_username;
	CString m_userpwd;
	CString m_usernumber;
	CString m_usertypevalue;
	CEdit m_usercontrol;
	CButton m_checkcontrol[QXNUM];
};
