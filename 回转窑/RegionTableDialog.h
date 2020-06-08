#pragma once
#include "afxcmn.h"
#include "AlarmSetDialog.h"


// CRegionTableDialog �Ի���

class CRegionTableDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CRegionTableDialog)

public:
	CRegionTableDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegionTableDialog();

// �Ի�������
	enum { IDD = IDD_VIEW_REGIONTABLE };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listctrl;
	CImageList m_IconList;
	CAlarmSetDialog m_alarmsetdialog;
	bool alarmflag;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickListRegiontable(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void AlarmSetFUN();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnUpdate32774(CCmdUI *pCmdUI);
};
