#pragma once
#include "afxcmn.h"

// CRegionSetDialog 对话框

class CRegionSetDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CRegionSetDialog)

public:
	CRegionSetDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegionSetDialog();

	// 对话框数据
	enum { IDD = IDD_SET_THERMAL_REGION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnDeltaposSpinLeft(NMHDR* pNMHDR, LRESULT* pResult);
	CString m_left;
	CString m_right;
	afx_msg void OnDeltaposSpinRight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinTop(NMHDR* pNMHDR, LRESULT* pResult);
	CString m_top;
	CString m_bottom;
	afx_msg void OnDeltaposSpinBottom(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl m_listctrl;
	CString m_regionname;
	afx_msg void OnItemchangedListRegioninfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
	CString m_emiss;
	afx_msg void OnBnClickedButton4();
};
