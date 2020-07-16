#pragma once
#include "afxcmn.h"


// UserManageDialog 对话框

class UserManageDialog : public CDialogEx
{
	DECLARE_DYNAMIC(UserManageDialog)

public:
	UserManageDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~UserManageDialog();

// 对话框数据
	enum { IDD = IDD_USER_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listctrl;
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32792();
	afx_msg void On32790();
	afx_msg void On32791();
	void initData(void);
	afx_msg void On32793();
	afx_msg void On32794();
};
