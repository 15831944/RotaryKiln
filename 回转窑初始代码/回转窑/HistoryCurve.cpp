// HistoryCurve.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "HistoryCurve.h"
#include "afxdialogex.h"



double x,y;
// HistoryCurve �Ի���

IMPLEMENT_DYNAMIC(HistoryCurve, CDialogEx)

HistoryCurve::HistoryCurve(CWnd* pParent /*=NULL*/)
	: CDialogEx(HistoryCurve::IDD, pParent)
{
	
}

HistoryCurve::~HistoryCurve()
{
}

void HistoryCurve::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ChartCtrlHistory, m_ChartCtrl1);
	DDX_Check(pDX, IDC_CHECK1, m_check[0]);
	DDX_Check(pDX, IDC_CHECK2, m_check[1]);
	DDX_Check(pDX, IDC_CHECK3, m_check[2]);
	DDX_Check(pDX, IDC_CHECK4, m_check[3]);
	DDX_Check(pDX, IDC_CHECK5, m_check[4]);
	DDX_Check(pDX, IDC_CHECK6, m_check[5]);
	DDX_Check(pDX, IDC_CHECK7, m_check[6]);
	DDX_Check(pDX, IDC_CHECK8, m_check[7]);
	DDX_Check(pDX, IDC_CHECK9, m_check[8]);
	DDX_Check(pDX, IDC_CHECK10, m_check[9]);
	DDX_Control(pDX, IDC_CHECK1, m_checkcontrol[0]);
	DDX_Control(pDX, IDC_CHECK2, m_checkcontrol[1]);
	DDX_Control(pDX, IDC_CHECK3, m_checkcontrol[2]);
	DDX_Control(pDX, IDC_CHECK4, m_checkcontrol[3]);
	DDX_Control(pDX, IDC_CHECK5, m_checkcontrol[4]);
	DDX_Control(pDX, IDC_CHECK6, m_checkcontrol[5]);
	DDX_Control(pDX, IDC_CHECK7, m_checkcontrol[6]);
	DDX_Control(pDX, IDC_CHECK8, m_checkcontrol[7]);
	DDX_Control(pDX, IDC_CHECK9, m_checkcontrol[8]);
	DDX_Control(pDX, IDC_CHECK10, m_checkcontrol[9]);
}


BEGIN_MESSAGE_MAP(HistoryCurve, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &HistoryCurve::OnBnClickedButton1)
	ON_MESSAGE(MESSAGE_UPDATEPOS, &HistoryCurve::OnMessageUpdatepos)
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK1, &HistoryCurve::OnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK2, &HistoryCurve::OnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK3, &HistoryCurve::OnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK4, &HistoryCurve::OnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK5, &HistoryCurve::OnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK6, &HistoryCurve::OnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK7, &HistoryCurve::OnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK8, &HistoryCurve::OnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK9, &HistoryCurve::OnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK10, &HistoryCurve::OnClickedCheck)
END_MESSAGE_MAP()


// HistoryCurve ��Ϣ�������


class CCustomCursorListener : public CChartCursorListener
{
public:
	void OnCursorMoved(CChartCursor *pCursor, double xValue, double yValue)
	{
		x= xValue;
		y= yValue;
		SendMessage(m_hwnd, MESSAGE_UPDATEPOS, 0, 0);
	}

	void GetHwnd(HWND hwnd)
	{
		m_hwnd = hwnd;
	}
	HWND m_hwnd;
};

BOOL HistoryCurve::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	for(int i=0;i<10;i++)
	{
		m_checkcontrol[i].SetCheck(true);
		m_checkcontrol[i].EnableWindow(FALSE);
		m_checkcontrol[i].ShowWindow(SW_HIDE);
	}
	for(int i=0;i<curve_y->size();i++)
	{
		m_checkcontrol[i].EnableWindow(TRUE);
		m_checkcontrol[i].ShowWindow(SW_SHOW);
	}
	//��ʼ��������
	CChartAxis *pAxis = NULL;
	pAxis = m_ChartCtrl1.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_ChartCtrl1.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	//��ӱ���
	TChartString str1;
	str1 = _T("  ");
	m_ChartCtrl1.GetTitle()->AddString(str1);

	//������
	m_ChartCtrl1.GetTitle()->SetColor(RGB(255, 255, 255));   //���������ɫ
	m_ChartCtrl1.GetLeftAxis()->SetTextColor(RGB(255, 255, 255));  //���������ɫ
	m_ChartCtrl1.GetBottomAxis()->SetTextColor(RGB(255, 255, 255));  //�ײ��������ɫ
	m_ChartCtrl1.SetBorderColor(RGB(255, 255, 255));  //�߿���ɫ��ɫ
	m_ChartCtrl1.SetBackColor(RGB(85, 85, 85));  //������ɫ���ɫ

	

	m_ChartCtrl1.SetZoomEnabled(true);
	m_ChartCtrl1.RemoveAllSeries();//�����
	
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	CCustomCursorListener* m_pCursorListener;
	CChartCrossHairCursor* pCrossHair =
		m_ChartCtrl1.CreateCrossHairCursor();

	HWND hWnd = this->GetSafeHwnd();
	m_pCursorListener = new CCustomCursorListener;
	m_pCursorListener->GetHwnd(hWnd);
	pCrossHair->RegisterListener(m_pCursorListener);


	CChartAxisLabel* pLabel = NULL;

	str1 = _T("�¶ȡ�");
	pAxis = m_ChartCtrl1.GetLeftAxis();
	if(pAxis)
		pLabel = pAxis->GetLabel();
	if(pLabel)
		pLabel->SetText(str1);

	
	str1 = _T("ʱ��");
	pAxis = m_ChartCtrl1.GetBottomAxis();
	if(pAxis)
		pLabel = pAxis->GetLabel();
	if(pLabel)
		pLabel->SetText(str1);
	
	pLineSerie.resize(curve_y->size());
	for(int i=0;i<curve_y->size();i++)
	{
       pLineSerie[i] = m_ChartCtrl1.CreateLineSerie();
	   pLineSerie[i]->SetSeriesOrdering(poNoOrdering);//����Ϊ����
	   //CString ss=(*region_name)[i];
	   pLineSerie[i]->SetName((*region_name)[i].GetBuffer());
	}
	// Configure the legend
	m_ChartCtrl1.GetLegend()->SetVisible(true);
	m_ChartCtrl1.GetLegend()->SetHorizontalMode(true);
	m_ChartCtrl1.GetLegend()->UndockLegend(80,0);
//	m_ChartCtrl1.GetTitle()->AddString(_T("  "));
	m_ChartCtrl1.EnableRefresh(false);
	std::vector<double> curve_x;
	curve_x.resize((*curve_y)[0].size());
	for(int i=0;i<(*curve_y)[0].size();i++)
	{
           curve_x[i]=1.0*i;
	}
	
	for(int i=0;i<curve_y->size();i++)
	{
		pLineSerie[i]->ClearSerie();
		pLineSerie[i]->AddPoints(curve_x.data(),(*curve_y)[i].data(),(*curve_y)[i].size());

	}
	m_ChartCtrl1.EnableRefresh(true);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void HistoryCurve::InitData(std::vector<CString>* Time_x,std::vector<std::vector<double>> *curve_y,std::vector<CString> *region_name)
{
	this->Time_x=Time_x;
	this->curve_y=curve_y;
	this->region_name=region_name;
	////pLineSerieMax = m_ChartCtrl1.CreateLineSerie();
	////pLineSerieMax->SetSeriesOrdering(poNoOrdering);//����Ϊ����
	////pLineSerieMin = m_ChartCtrl1.CreateLineSerie();
	////pLineSerieMin->SetSeriesOrdering(poNoOrdering);//����Ϊ����

}

void HistoryCurve::OnBnClickedButton1()
{
	CCustomCursorListener* m_pCursorListener;
	CChartCrossHairCursor* pCrossHair =
		m_ChartCtrl1.CreateCrossHairCursor();

	HWND hWnd = this->GetSafeHwnd();
	m_pCursorListener = new CCustomCursorListener;
	m_pCursorListener->GetHwnd(hWnd);
	pCrossHair->RegisterListener(m_pCursorListener);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

afx_msg LRESULT HistoryCurve::OnMessageUpdatepos(WPARAM wParam, LPARAM lParam)
{
	CString s;
	s.Format(_T("ʱ�䣺%s, �¶ȣ�%0.1f��"), (*Time_x)[x], y);
	CStatic* pStatic;
	pStatic = (CStatic*)GetDlgItem(IDC_STATIC);
	pStatic->SetWindowText(s);
	return 1;
}

void HistoryCurve::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(m_ChartCtrl1.GetSafeHwnd())//��ֹ�״ε���m_staPicture..m_hWndΪ�ճ���
	{
		CRect rcClient,static_rect;
		GetClientRect(rcClient);
		rcClient.left+=20;
		rcClient.top+=40;
		rcClient.right-=20;
		rcClient.bottom-=50;
		static_rect.left=rcClient.left+20;
		static_rect.top=rcClient.bottom+5;
		static_rect.right=static_rect.top+100;
		static_rect.bottom=static_rect.top+40;
		m_ChartCtrl1.MoveWindow(rcClient);
		Invalidate(true);
		GetDlgItem(IDC_STATIC)->MoveWindow(static_rect);
	}
	// TODO: �ڴ˴������Ϣ����������
}


void HistoryCurve::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnLButtonUp(nFlags, point);
}


void HistoryCurve::OnClickedCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ʼ��������
	
	
	m_ChartCtrl1.EnableRefresh(false);
	
	for(int i=0;i<curve_y->size();i++)
	{	
		if(m_checkcontrol[i].GetCheck()==false)
		   pLineSerie[i]->SetVisible(false);
		else
		   pLineSerie[i]->SetVisible(true);
	}
	m_ChartCtrl1.EnableRefresh(true);
}
