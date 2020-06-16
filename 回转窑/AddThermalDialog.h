#pragma once

// AddThermalDialog 对话框

class AddThermalDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AddThermalDialog)

public:
	AddThermalDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AddThermalDialog();

	// 对话框数据
	enum { IDD = IDD_ADD_THERMAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	DWORD m_ip;
	CString m_port;
	CString m_name;
	CString m_psd;
	CString m_thermalname;
};
