#pragma once
#include <vector>

// CAlarmSetDialog 对话框

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
	CAlarmSetDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAlarmSetDialog();

	// 对话框数据
	enum { IDD = IDD_DIALOG_REGIONALARM_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
