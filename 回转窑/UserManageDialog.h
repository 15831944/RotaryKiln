#pragma once
#include "afxcmn.h"

// UserManageDialog �Ի���

class UserManageDialog : public CDialogEx
{
	DECLARE_DYNAMIC(UserManageDialog)

public:
	UserManageDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~UserManageDialog();

	// �Ի�������
	enum { IDD = IDD_USER_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
