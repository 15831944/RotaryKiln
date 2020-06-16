#pragma once

// CCopyDialog 对话框

class CCopyDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCopyDialog)

public:
	CCopyDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCopyDialog();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COPYDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	CString info;
	friend void ShowInfoDialog(CString);
public:
	virtual BOOL OnInitDialog();
};
