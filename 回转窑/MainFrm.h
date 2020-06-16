// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "DlgDockable.h"
#include "ThermalSetDialog.h"

class CMainFrame : public CFrameWndEx
{
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// 特性
public:

	// 操作
public:

	// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

	// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CDockableDlgThermal m_dockabledlgthermal;
	CDockableDlgSplicing m_dockabledlgsplicing;
	CDockableDlgRegionTable m_dockabledlgregiontable;
	CDockableDlgCurve m_dockabledlgcurve;
	CThermalSetDialog* m_theralsetdialog;

	// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnClose();
	afx_msg void ThermalSet();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnChangeUser();
	afx_msg void On32776();
	afx_msg void On32775();
	afx_msg void On32780();
	afx_msg void On32781();
	afx_msg void OnEditPaste();
	afx_msg void On32795();
	afx_msg void OnMenu32796();
	afx_msg void OnPictureSc();
	afx_msg void OnPictureAl();
	afx_msg void OnHelp();
};
