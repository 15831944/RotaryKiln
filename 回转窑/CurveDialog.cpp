// CurveDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "CurveDialog.h"
#include "afxdialogex.h"
#include "Afxsock.h"

#include "SQLConnect.hpp"
#include "easylogging++.h"

std::vector<int> RegionLeft;
std::vector<int> RegionRight;
std::vector<CString> RegionName;

extern CCurveDialog *p_curvedialog;
extern double *curve_x, *curve_yMax,*curve_yMin;
extern int thermal_heigth,thermal_width;
// CCurveDialog �Ի���

IMPLEMENT_DYNAMIC(CCurveDialog, CDialogEx)

CCurveDialog::CCurveDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCurveDialog::IDD, pParent)
{
	p_curvedialog=this;
}

CCurveDialog::~CCurveDialog()
{
}

void CCurveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCurveDialog, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCurveDialog ��Ϣ�������


BOOL CCurveDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rect,rectChart; 
	GetDlgItem(IDC_ChartCtrl1)->GetWindowRect(&rect);
	ScreenToClient(rect);
	rectChart = rect;
	m_ChartCtrl1.Create(this,rectChart,IDC_ChartCtrl1);
	m_ChartCtrl1.ShowWindow(SW_SHOWNORMAL);

	CChartAxis *pAxis= NULL; 
	pAxis = m_ChartCtrl1.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_ChartCtrl1.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);


	
	TChartString str1;
	/*str1 = _T("�����");
	m_ChartCtrl1.GetTitle()->AddString(str1)*/;


	CChartAxisLabel* pLabel = NULL;

	str1 = _T("�¶ȡ�");
	pAxis = m_ChartCtrl1.GetLeftAxis();
	if(pAxis)
		pLabel = pAxis->GetLabel();
	if(pLabel)
		pLabel->SetText(str1);

	//m_ChartCtrl1.GetLeftAxis()->GetLabel()->SetText(str1);

	str1 = _T("�¶�λ��");
	pAxis = m_ChartCtrl1.GetBottomAxis();
	if(pAxis)
		pLabel = pAxis->GetLabel();
	if(pLabel)
		pLabel->SetText(str1);
	pLineSerieMax = m_ChartCtrl1.CreateLineSerie();
	pLineSerieMax->SetSeriesOrdering(poNoOrdering);//����Ϊ����
	pLineSerieMin = m_ChartCtrl1.CreateLineSerie();
	pLineSerieMin->SetSeriesOrdering(poNoOrdering);//����Ϊ����

	//������



	//new��ʼ������Ҫ�����ݿ�����
	CString select_sql;
	AccessResult res;
	if (SUCCEEDED(accessConnect.select("select * from region_info where region_state=1", res)))//����ѯ�ɹ�
	{
		if(res.empty()) //��ѯ���Ϊ��
		{
			AfxMessageBox("���ڻ�û���������ݣ�");
			LOG(WARNING)<<"û����������";
		}
		else
		{
			for (auto& record : res)
			{
				RegionName.push_back(record["region_name"].c_str());
				RegionLeft.push_back(std::stoi(record["region_left"]));
				RegionRight.push_back(std::stoi(record["region_right"]));
			}
		}
	}

	/*CRect m_crect=m_ChartCtrl1.GetPlottingRect();
	for(int i=0;i<RegionName.size();i++)
	{
		RegionLeft[i]=m_crect.left+RegionLeft[i]*(m_crect.right-m_crect.left)/(curve_x_size-1);
		RegionRight[i]=m_crect.left+RegionRight[i]*(m_crect.right-m_crect.left)/(curve_x_size-1);
	}*/
	//for(int i=0;i<RegionLine.size();i++)
	//{
	//	RegionLine[i]->SetSeriesOrdering(poNoOrdering);//����Ϊ����
	//	RegionLine[i]->ClearSerie();
	//	double x[2]={RegionLeft[i],RegionLeft[i]};
	//	double y[2]={0,50};
	//	RegionLine[i]->AddPoints(x,y,2);
	//	RegionLine[i]->SetName(RegionName[i].GetBuffer());

	//}
	
	SetTimer(0,10,NULL);	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CCurveDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(curve_x)
	{
		
		m_ChartCtrl1.EnableRefresh(false);
		pLineSerieMin->ClearSerie();
		pLineSerieMin->AddPoints(curve_x,curve_yMin,thermal_width);
		pLineSerieMax->ClearSerie();
		pLineSerieMax->AddPoints(curve_x,curve_yMax,thermal_width);
		
		m_ChartCtrl1.EnableRefresh(true);
		
		CDC* m_dc=m_ChartCtrl1.GetDC();
		CRect m_crect=m_ChartCtrl1.GetPlottingRect();
	
		//�����������
		int MaxCenterPos=0,CenterLen=9;
		double CenterSum=0,MaxCenterSum;
		for(int i=0;i<CenterLen;i++)
			CenterSum+=curve_yMax[i];
		MaxCenterSum=CenterSum;
		MaxCenterPos=CenterLen-1;
		for(int i=CenterLen;i<thermal_width;i++)
		{
			CenterSum=CenterSum-curve_yMax[i-CenterLen]+curve_yMax[i];
			if(MaxCenterSum<CenterSum)
			{
				MaxCenterPos=i;
				MaxCenterSum=CenterSum;
			}
		}
		MaxCenterPos=m_crect.left+(MaxCenterPos-CenterLen/2.0)*(m_crect.right-m_crect.left)/(thermal_width-1);
		//�������ļ���end

		CPen pen,gwpen; 
		pen.CreatePen(PS_SOLID,1,RGB(0,255,255)); 
		gwpen.CreatePen(PS_SOLID,1,RGB(255,0,0)); 
		m_dc->SelectObject( &pen );
		static bool cshflag=true;
		if(cshflag)
		{
			if(thermal_width!=0)
			{
				for(int i=0;i<RegionName.size();i++)
				{
					RegionLeft[i]=m_crect.left+RegionLeft[i]*(m_crect.right-m_crect.left)/(thermal_width-1);
					RegionRight[i]=m_crect.left+RegionRight[i]*(m_crect.right-m_crect.left)/(thermal_width-1);
				}
				cshflag=false;
			}
		}
		else
		{
			for(int i=0;i<RegionName.size();i++)
			{
				m_dc->TextOut(RegionLeft[i],0,RegionName[i]);
				m_dc->MoveTo(RegionLeft[i],m_crect.top);   
				m_dc->LineTo(RegionLeft[i],m_crect.bottom);
				m_dc->MoveTo(RegionRight[i],m_crect.top);   
				m_dc->LineTo(RegionRight[i],m_crect.bottom);
			}
			//���������� 
			m_dc->SelectObject( &gwpen );
			m_dc->TextOut(MaxCenterPos,30,"��");
			m_dc->TextOut(MaxCenterPos,50,"��");
			m_dc->MoveTo(MaxCenterPos,m_crect.top);   
			m_dc->LineTo(MaxCenterPos,m_crect.bottom);
			m_dc->MoveTo(MaxCenterPos,m_crect.top);   
			m_dc->LineTo(MaxCenterPos,m_crect.bottom);
		}
		
	}
	CDialogEx::OnTimer(nIDEvent);
}
