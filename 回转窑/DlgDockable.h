#pragma once

// CDlgTest 对话框
#include "resource.h"
#include "ThermalDialog.h"
#include "SplicingDialog.h"
#include "RegionTableDialog.h"
#include "CurveDialog.h"


//热像仪视图可停靠类
class CDockableDlgThermal : public CDockablePane
{
public:
	CDockableDlgThermal();
	virtual ~CDockableDlgThermal();

private:
	CThermalDialog m_thermaldlg;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};

//拼接视图可停靠类
class CDockableDlgSplicing: public CDockablePane
{
public:
	CDockableDlgSplicing();
	virtual ~CDockableDlgSplicing();

private:
	CSplicingDialog m_splicingdlg;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};

//分区域表格视图可停靠类
class CDockableDlgRegionTable: public CDockablePane
{
public:
	CDockableDlgRegionTable();
	virtual ~CDockableDlgRegionTable();

private:
	CRegionTableDialog m_regiontabledlg;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};

//横剖面曲线视图可停靠类
class CDockableDlgCurve: public CDockablePane
{
public:
	CDockableDlgCurve();
	virtual ~CDockableDlgCurve();

private:
	CCurveDialog m_curvedlg;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};
