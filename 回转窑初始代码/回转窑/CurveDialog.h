#pragma once
#include "..\ChartClass\ChartCtrl.h"
#include "..\ChartClass\ChartLineSerie.h"
#include "..\ChartClass\ChartAxis.h"
#include "..\ChartClass\ChartTitle.h"
#include "..\ChartClass\ChartAxisLabel.h"

// CCurveDialog �Ի���

class CCurveDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCurveDialog)

public:
	CChartCtrl m_ChartCtrl1;
	CChartLineSerie *pLineSerieMax,*pLineSerieMin;
	//std::vector<CChartLineSerie*> RegionLine;
	//vector<CChartLineSerie> pLineSerieRegion;
	CCurveDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCurveDialog();

// �Ի�������
	enum { IDD = IDD_VIEW_CURVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
