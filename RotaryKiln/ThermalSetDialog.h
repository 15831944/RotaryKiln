#pragma once
#include "ImageButton.h"
#include "afxwin.h"
#include "IOSetDialog.h"
#include "RegionSetDialog.h"
#include "SplicingSetDialog.h"

// CThermalSetDialog 对话框

class CThermalSetDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CThermalSetDialog)

public:
	CThermalSetDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CThermalSetDialog();

	// 对话框数据
	enum { IDD = IDD_SET_THERMAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CImageButton m_param;
	CImageButton m_io;
	CImageButton m_region;
	CImageButton m_splicing;
	CImageList m_MyImageList;
	CIOSetDialog m_iosetdialog;
	CRegionSetDialog   m_regionsetdialog;
	CSplicingSetDialog m_splicingsetdialog;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSplicing();
	afx_msg void OnBnClickedButtonRegion();
	afx_msg void OnClose();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnBnClickedButtonIo();
};
