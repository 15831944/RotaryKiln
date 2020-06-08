#pragma once
#include "..\ChartClass\ChartCtrl.h"
#include "..\ChartClass\ChartLineSerie.h"
#include "..\ChartClass\ChartAxis.h"
#include "..\ChartClass\ChartTitle.h"
#include "..\ChartClass\ChartAxisLabel.h"

// CCurveDialog 对话框

class CCurveDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCurveDialog)

public:
	CChartCtrl m_ChartCtrl1;
	CChartLineSerie *pLineSerieMax,*pLineSerieMin;
	//std::vector<CChartLineSerie*> RegionLine;
	//vector<CChartLineSerie> pLineSerieRegion;
	CCurveDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCurveDialog();

// 对话框数据
	enum { IDD = IDD_VIEW_CURVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
