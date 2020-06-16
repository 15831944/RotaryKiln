#pragma once

// CSplicingDialog 对话框

class CSplicingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSplicingDialog)

public:
	CSplicingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSplicingDialog();

	// 对话框数据
	enum { IDD = IDD_VIEW_SPLICING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
