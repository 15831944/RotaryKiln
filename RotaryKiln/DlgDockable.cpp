// DlgTest.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "DlgDockable.h"
#include "afxdialogex.h"

//可停靠热像仪视图
CDockableDlgThermal::CDockableDlgThermal()
{
}

CDockableDlgThermal::~CDockableDlgThermal()
{
}

BEGIN_MESSAGE_MAP(CDockableDlgThermal, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CDockableDlgThermal::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (!m_thermaldlg.Create(IDD_VIEW_THERMAL, this))
	{
		TRACE0("未能创建窗口/n");
		return -1;
	}

	m_thermaldlg.ShowWindow(SW_SHOW);

	return 0;
}

void CDockableDlgThermal::OnDestroy()
{
	CDockablePane::OnDestroy();
	m_thermaldlg.DestroyWindow();
}

void CDockableDlgThermal::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_thermaldlg.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);
		m_thermaldlg.MoveWindow(rect);
	}
}

//可停靠拼接视图
CDockableDlgSplicing::CDockableDlgSplicing()
{
}

CDockableDlgSplicing::~CDockableDlgSplicing()
{
}

BEGIN_MESSAGE_MAP(CDockableDlgSplicing, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CDockableDlgSplicing::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (!m_splicingdlg.Create(IDD_VIEW_SPLICING, this))
	{
		TRACE0("未能创建窗口/n");
		return -1;
	}

	m_splicingdlg.ShowWindow(SW_SHOW);
	return 0;
}

void CDockableDlgSplicing::OnDestroy()
{
	CDockablePane::OnDestroy();
	m_splicingdlg.DestroyWindow();
}

void CDockableDlgSplicing::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_splicingdlg.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);
		m_splicingdlg.MoveWindow(rect);
	}
}

//可停靠分区域表格视图
CDockableDlgRegionTable::CDockableDlgRegionTable()
{
}

CDockableDlgRegionTable::~CDockableDlgRegionTable()
{
}

BEGIN_MESSAGE_MAP(CDockableDlgRegionTable, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CDockableDlgRegionTable::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (!m_regiontabledlg.Create(IDD_VIEW_REGIONTABLE, this))
	{
		TRACE0("未能创建窗口/n");
		return -1;
	}

	m_regiontabledlg.ShowWindow(SW_SHOW);
	return 0;
}

void CDockableDlgRegionTable::OnDestroy()
{
	CDockablePane::OnDestroy();
	m_regiontabledlg.DestroyWindow();
}

void CDockableDlgRegionTable::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_regiontabledlg.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);
		m_regiontabledlg.MoveWindow(rect);
	}
}

//可停靠横剖面曲线视图
CDockableDlgCurve::CDockableDlgCurve()
{
}

CDockableDlgCurve::~CDockableDlgCurve()
{
}

BEGIN_MESSAGE_MAP(CDockableDlgCurve, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CDockableDlgCurve::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// EXCEPTION Create会抛异常
	if (!m_curvedlg.Create(IDD_VIEW_CURVE, this))
	{
		TRACE0("未能创建窗口/n");
		return -1;
	}

	m_curvedlg.ShowWindow(SW_SHOW);
	return 0;
}

void CDockableDlgCurve::OnDestroy()
{
	CDockablePane::OnDestroy();
	m_curvedlg.DestroyWindow();
}

void CDockableDlgCurve::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_curvedlg.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);
		m_curvedlg.MoveWindow(rect);
	}
}