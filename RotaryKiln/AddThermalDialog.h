#pragma once

// AddThermalDialog �Ի���

class AddThermalDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AddThermalDialog)

public:
	AddThermalDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AddThermalDialog();

	// �Ի�������
	enum { IDD = IDD_ADD_THERMAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	DWORD m_ip;
	CString m_port;
	CString m_name;
	CString m_psd;
	CString m_thermalname;
};
