// ThermalDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "ThermalDialog.h"
#include "afxdialogex.h"
#include <Vector>
#include "ImageProcessing.hpp"

extern AreaVessel areavessel;//Ĭ����������
extern CThermalDialog* p_thermaldialog;
CString strToolBar;
CRect WindowRectThermalShow;
// ��������
//enum AreaType
//{
//	DefaultRgn=-1,PointRgn=0,RectRgn=1, EllipticRgn=2,LineRgn=3,PolygonRgn=4
//};

extern AreaType nowAreaType;
extern std::vector<CPoint> nowRegionPoints;// ��ǰ����߽��
extern int nowAreaVersion;
extern int oldAreaVersion;

// CThermalDialog �Ի���

IMPLEMENT_DYNAMIC(CThermalDialog, CDialogEx)

CThermalDialog::CThermalDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CThermalDialog::IDD, pParent)
{
	p_thermaldialog = this;
	heigth = 100;
	width = 100;
}

CThermalDialog::~CThermalDialog()
{
}

void CThermalDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THERMAL_SHOW, m_staPicture);
}

BEGIN_MESSAGE_MAP(CThermalDialog, CDialogEx)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xffff, OnToolTipText)
	ON_COMMAND_RANGE(IDC_TOOLBAR_DEFAULT, IDC_TOOLBAR_SAVE, CThermalDialog::OnBnClickedToolBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CThermalDialog ��Ϣ�������

BOOL CThermalDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR_THERMAL))
	{
		TRACE0("δ�ܴ���������\n");
		AfxMessageBox(_T("δ�ܴ���������\n"));
		return FALSE;      // δ�ܴ���
	}
	m_ToolBar.ShowWindow(SW_SHOW);
	//�ؼ�����λ
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	GetDlgItem(IDC_THERMAL_SHOW)->GetWindowRect(&WindowRectThermalShow);
	ScreenToClient(&WindowRectThermalShow);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CThermalDialog::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;
	UINT  uID = pNMHDR->idFrom;     // �൱��ԭWM_COMMAND���ݷ�ʽ��wParam��low-order��, ��wParam�зŵ����ǿؼ���ID��

	if (uID == NULL)
		return   FALSE;
	strToolBar.LoadString(uID);
	pTTT->lpszText = strToolBar.GetBuffer();
	//pTTT->lpszText="��ã�";
	return TRUE;
}

//�������ϰ�ť����Ϣ��Ӧ
void CThermalDialog::OnBnClickedToolBar(UINT nID)
{
	if (nID == IDC_TOOLBAR_SAVE)
	{
		OnSave(0);
		return;
	}
	nowAreaType = (AreaType)(nID - IDC_TOOLBAR_DEFAULT - 1);

	if (oldAreaVersion + 1 == nowAreaVersion)
	{
		nowRegionPoints.clear();
		nowAreaVersion--;
	}
}

void CThermalDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GetDlgItem(IDC_THERMAL_SHOW)->GetWindowRect(&WindowRectThermalShow);
	ScreenToClient(&WindowRectThermalShow);
	if (WindowRectThermalShow.PtInRect(point))
	{  //DefaultRgn=-1,PointRgn=0,RectRgn=1, EllipticRgn=2,LineRgn=3,PolygonRgn=4
		CPoint ppoint;
		ppoint.x = point.x - WindowRectThermalShow.left;
		ppoint.y = point.y - WindowRectThermalShow.top;
		switch (nowAreaType)
		{
		case DefaultRgn:
			break;
		case PointRgn:
			break;
		case RectRgn:
		case EllipticRgn:
			if (oldAreaVersion == nowAreaVersion)
			{
				nowRegionPoints.clear();
				nowRegionPoints.push_back(ppoint);
				nowRegionPoints.push_back(ppoint);
				nowAreaVersion++;
			}
			break;
		case LineRgn:
			if (oldAreaVersion == nowAreaVersion)
			{
				nowRegionPoints.clear();
				nowRegionPoints.push_back(ppoint);
				nowAreaVersion++;
			}
			else if (oldAreaVersion + 1 == nowAreaVersion)
			{
				if (abs(nowRegionPoints[nowRegionPoints.size() - 1].x - ppoint.x) + abs(nowRegionPoints[nowRegionPoints.size() - 1].y - ppoint.y) <= 2)
				{
					nowAreaVersion++;
				}
				else
					nowRegionPoints.push_back(ppoint);
			}
			break;
		case PolygonRgn:
			if (oldAreaVersion == nowAreaVersion)
			{
				nowRegionPoints.clear();
				nowRegionPoints.push_back(ppoint);
				nowAreaVersion++;
			}
			else if (oldAreaVersion + 1 == nowAreaVersion)
			{
				if (abs(nowRegionPoints[nowRegionPoints.size() - 1].x - ppoint.x) + abs(nowRegionPoints[nowRegionPoints.size() - 1].y - ppoint.y) <= 2)
				{
					nowAreaVersion++;
				}
				else
					nowRegionPoints.push_back(ppoint);
			}
			break;
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CThermalDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GetDlgItem(IDC_THERMAL_SHOW)->GetWindowRect(&WindowRectThermalShow);
	ScreenToClient(&WindowRectThermalShow);
	if (WindowRectThermalShow.PtInRect(point))
	{   //DefaultRgn=-1,PointRgn=0,RectRgn=1, EllipticRgn=2,LineRgn=3,PolygonRgn=4
		CPoint ppoint;
		ppoint.x = point.x - WindowRectThermalShow.left;
		ppoint.y = point.y - WindowRectThermalShow.top;
		switch (nowAreaType)
		{
		case DefaultRgn:
			break;
		case PointRgn:
			break;
		case RectRgn:
		case EllipticRgn:
			if (oldAreaVersion + 1 == nowAreaVersion)
			{
				nowRegionPoints[1] = ppoint;
				nowAreaVersion++;
			}
			break;
		case LineRgn:
			break;
		case PolygonRgn:
			break;
		}
	}
	else if (oldAreaVersion + 1 == nowAreaVersion)
	{
		nowAreaVersion--;
		nowRegionPoints.clear();
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CThermalDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (WindowRectThermalShow.PtInRect(point) && nFlags == 1 && oldAreaVersion + 1 == nowAreaVersion)
	{   //DefaultRgn=-1,PointRgn=0,RectRgn=1, EllipticRgn=2,LineRgn=3,PolygonRgn=4
		CPoint ppoint;
		ppoint.x = point.x - WindowRectThermalShow.left;
		ppoint.y = point.y - WindowRectThermalShow.top;
		if (nowAreaType == RectRgn || nowAreaType == EllipticRgn)
		{
			nowRegionPoints[1] = ppoint;
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CThermalDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GetDlgItem(IDC_THERMAL_SHOW)->GetWindowRect(&WindowRectThermalShow);
	ScreenToClient(&WindowRectThermalShow);
	if (WindowRectThermalShow.PtInRect(point))
	{  //DefaultRgn=-1,PointRgn=0,RectRgn=1, EllipticRgn=2,LineRgn=3,PolygonRgn=4
		CPoint ppoint;
		ppoint.x = point.x - WindowRectThermalShow.left;
		ppoint.y = point.y - WindowRectThermalShow.top;
		switch (nowAreaType)
		{
		case LineRgn:
			if (oldAreaVersion + 1 == nowAreaVersion)
			{
				nowAreaVersion++;
				nowRegionPoints.push_back(ppoint);
			}
			break;
		case PolygonRgn:
			if (oldAreaVersion + 1 == nowAreaVersion)
			{
				nowAreaVersion++;
				nowRegionPoints.push_back(ppoint);
			}
			break;
		}
	}
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CThermalDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_staPicture.GetSafeHwnd())//��ֹ�״ε���m_staPicture..m_hWndΪ�ճ���
	{
		CRect rcClient;
		GetClientRect(rcClient);
		rcClient.left += 20;
		rcClient.top += 40;
		rcClient.right -= 20;
		rcClient.bottom -= 20;
		if (1.0 * (rcClient.bottom - rcClient.top) / (rcClient.right - rcClient.left) > 1.0 * heigth / width)
		{
			int ajh = (rcClient.right - rcClient.left) * heigth / width;
			ajh = 0.5 * (rcClient.bottom - rcClient.top - ajh);
			rcClient.top += ajh;
			rcClient.bottom -= ajh;
		}
		else
		{
			int ajw = (rcClient.bottom - rcClient.top) * (width) / heigth;
			ajw = 0.5 * (rcClient.right - rcClient.left - ajw);
			rcClient.left += ajw;
			rcClient.right -= ajw;
		}
		m_staPicture.MoveWindow(rcClient);
		GetDlgItem(IDC_THERMAL_SHOW)->GetWindowRect(&WindowRectThermalShow);
		ScreenToClient(&WindowRectThermalShow);
		Invalidate(true);
	}
	// TODO: �ڴ˴������Ϣ����������
}

void CThermalDialog::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GetDlgItem(IDC_THERMAL_SHOW)->GetWindowRect(&WindowRectThermalShow);
	ScreenToClient(&WindowRectThermalShow);
	if (WindowRectThermalShow.PtInRect(point))
	{  //DefaultRgn=-1,PointRgn=0,RectRgn=1, EllipticRgn=2,LineRgn=3,PolygonRgn=4
		CPoint ppoint;
		ppoint.x = point.x - WindowRectThermalShow.left;
		ppoint.y = point.y - WindowRectThermalShow.top;
		//vector<AreaTempInfo> areaVector=areavessel.areaVector;
		//areavessel.defaultAreaNum
		vector<AreaTempInfo>::iterator it;
		for (it = areavessel.areaVector.begin() + areavessel.defaultAreaNum; it != areavessel.areaVector.end(); it++)
		{
			if (it->RegionShow.PtInRegion(ppoint) && it->Flag == 0)
			{
				if (AfxMessageBox("��Ҫɾ������:" + it->Id, 1, 0) == IDOK)
				{
					it->Flag = 1;
				}
				areavessel.SaveRegion();
				return;
			}
		}
		for (it = areavessel.areaVector.begin(); it != areavessel.areaVector.begin() + areavessel.defaultAreaNum; it++)
		{
			if (it->RegionShow.PtInRegion(ppoint))
			{
				if (it->Flag == 0)
				{
					if (AfxMessageBox("��Ҫ��������:" + it->Name, 1, 0) == IDOK)
					{
						it->Flag = 2;
					}
					return;
				}
				else
				{
					if (AfxMessageBox("��Ҫ��ʾ����:" + it->Name, 1, 0) == IDOK)
					{
						it->Flag = 0;
					}
					return;
				}
			}
		}
	}
	else
	{
		CDialogEx::OnRButtonUp(nFlags, point);
	}
}

void CThermalDialog::OnSave(int type)
{
	// TODO: �ڴ���������������
	CWnd* bmpShow = GetDlgItem(IDC_THERMAL_SHOW);
	CDC* pdc = bmpShow->GetDC();
	//CImage  imag;
	ATL::CImage imag;    //��ʶ���Է�����ȷ
	CRect rect;

	GetClientRect(&rect);        //��ȡ������С
	bmpShow->GetWindowRect(&rect);
	imag.Create(rect.Width(), rect.Height(), 32);
	::BitBlt(imag.GetDC(), 0, 0, rect.Width(), rect.Height(), pdc->m_hDC, 0, 0, SRCCOPY);
	CTime tm = CTime::GetCurrentTime();
	static CTime lastTime = 0;
	if (type == 1)
	{
		if (tm - lastTime > CTimeSpan(0, 0, 5, 0))
			lastTime = tm;
		else// С��5���Ӳ���ͼ
			return;
	}

	CString strpicturename, strPath;
	if (0 == type)
		strPath = "������ץͼ";
	else
		strPath = "�����Ǳ���";

	if (!PathFileExists(strPath))
	{
		//������
		system("md " + strPath);
	}
	strpicturename.Format(_T("\\%d%02d%02d%02d%02d%02d.jpg"), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
	strpicturename = strPath + strpicturename;
	HRESULT hResult = imag.Save(strpicturename); //����ͼƬ
	ReleaseDC(pdc);
	imag.ReleaseDC();
}