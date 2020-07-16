#pragma once
#include "afxcmn.h"
#include "atlcomtime.h"
#include<vector>

// HDataDialog 对话框

class HDataDialog : public CDialogEx
{
	DECLARE_DYNAMIC(HDataDialog)

public:
	HDataDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~HDataDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_HISDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listctrl;
	CString list_title;
	std::vector<CString> region_name;
	afx_msg void OnBnClickedButton1();
	COleDateTime m_startime;
	COleDateTime m_endtime;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton2();
};
