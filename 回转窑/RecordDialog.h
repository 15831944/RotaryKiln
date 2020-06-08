#pragma once
#include "afxwin.h"


// RecordDialog 对话框

class RecordDialog : public CDialogEx
{
	DECLARE_DYNAMIC(RecordDialog)

public:
	RecordDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RecordDialog();
	void ReadData(CString filename);
// 对话框数据
	enum { IDD = IDD_DIALOG_RECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
