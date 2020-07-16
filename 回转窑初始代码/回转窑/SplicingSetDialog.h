#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSplicingSetDialog �Ի���

class CSplicingSetDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSplicingSetDialog)

public:
	CSplicingSetDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSplicingSetDialog();

// �Ի�������
	enum { IDD = IDD_SET_THERMAL_SPLICING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_top;
	CString m_bottom;
	CSpinButtonCtrl m_spinleft;
	CSpinButtonCtrl m_spinright;
	afx_msg void OnBnClickedButtonSave();
	virtual BOOL OnInitDialog();
	CListCtrl m_listctrl;
	afx_msg void OnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_all_flag;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
};
