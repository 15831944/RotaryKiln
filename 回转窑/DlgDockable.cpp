// DlgTest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "DlgDockable.h"
#include "afxdialogex.h"

//��ͣ����������ͼ
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
		TRACE0("δ�ܴ�������/n");
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

//��ͣ��ƴ����ͼ
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
		TRACE0("δ�ܴ�������/n");
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

//��ͣ������������ͼ
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
		TRACE0("δ�ܴ�������/n");
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

//��ͣ��������������ͼ
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

	// EXCEPTION Create�����쳣
	if (!m_curvedlg.Create(IDD_VIEW_CURVE, this))
	{
		TRACE0("δ�ܴ�������/n");
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