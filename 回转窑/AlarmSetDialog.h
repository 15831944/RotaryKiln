#pragma once
#include <vector>

// CAlarmSetDialog �Ի���

class CAlarmSetDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAlarmSetDialog)

public:
	int RegionIndex;
	CString m_alarmTH1;
	CString m_alarmTH2;
	CString m_alarmTH3;
	std::vector<double> v_alarmTH1;
	std::vector<double> v_alarmTH2;
	std::vector<double> v_alarmTH3;
	CAlarmSetDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAlarmSetDialog();

	// �Ի�������
	enum { IDD = IDD_DIALOG_REGIONALARM_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
