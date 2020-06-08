#pragma once
#include "..\ChartClass\ChartCtrl.h"
#include "..\ChartClass\ChartLineSerie.h"
#include "..\ChartClass\ChartAxis.h"
#include "..\ChartClass\ChartTitle.h"
#include "..\ChartClass\ChartAxisLabel.h"
#include<vector>


#define MESSAGE_UPDATEPOS WM_USER+1001
// HistoryCurve 对话框

class HistoryCurve : public CDialogEx
{
	DECLARE_DYNAMIC(HistoryCurve)

public:
	CChartCtrl m_ChartCtrl1;
	BOOL m_check[10];
	CButton m_checkcontrol[10];
	std::vector<CChartLineSerie *>pLineSerie;
	std::vector<CString>* Time_x;
	std::vector<std::vector<double>> *curve_y;
	std::vector<CString> *region_name;
	HistoryCurve(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~HistoryCurve();

// 对话框数据
	enum { IDD = IDD_HISTORY_CURVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void InitData(std::vector<CString>* Time_x,std::vector<std::vector<double>> *curve_y,std::vector<CString> *region_name);
	afx_msg void OnBnClickedButton1();
protected:
	afx_msg LRESULT OnMessageUpdatepos(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClickedCheck();
};
