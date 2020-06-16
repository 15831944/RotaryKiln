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
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUserDrop();
	afx_msg void OnUserAppend();
	afx_msg void OnUserModify();
	void initData(void);
	afx_msg void OnUserDisable();
	afx_msg void OnUserEnable();
};
