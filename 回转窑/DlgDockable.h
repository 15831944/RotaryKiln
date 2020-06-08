#pragma once

// CDlgTest �Ի���
#include "resource.h"
#include "ThermalDialog.h"
#include "SplicingDialog.h"
#include "RegionTableDialog.h"
#include "CurveDialog.h"


//��������ͼ��ͣ����
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

//ƴ����ͼ��ͣ����
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

//����������ͼ��ͣ����
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

//������������ͼ��ͣ����
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
