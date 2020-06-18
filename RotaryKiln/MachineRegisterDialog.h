#pragma once

// MachineRegisterDialog 对话框

class MachineRegisterDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MachineRegisterDialog)

public:
	MachineRegisterDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MachineRegisterDialog();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	friend int VerifyMachineDialog();
protected:

	int regKeyValue = -1;
public:
	afx_msg void OnBnClickedBtntry();
};
