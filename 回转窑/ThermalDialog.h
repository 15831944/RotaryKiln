#pragma once
#include "afxwin.h"


// CThermalDialog �Ի���

class CThermalDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CThermalDialog)

public:
	CThermalDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CThermalDialog();

// �Ի�������
	enum { IDD = IDD_VIEW_THERMAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);  // ��������ʾ 
public:
	afx_msg void OnBnClickedToolBar(UINT nID);

public:
	CToolBar m_ToolBar;  
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic m_staPicture;
	int heigth,width; 
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void OnSave(int type);
};
