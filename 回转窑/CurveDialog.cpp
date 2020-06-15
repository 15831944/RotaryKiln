// CurveDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
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
// CCurveDialog 对话框

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


// CCurveDialog 消息处理程序


BOOL CCurveDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	/*str1 = _T("表标题");
	m_ChartCtrl1.GetTitle()->AddString(str1)*/;


	CChartAxisLabel* pLabel = NULL;

	str1 = _T("温度℃");
	pAxis = m_ChartCtrl1.GetLeftAxis();
	if(pAxis)
		pLabel = pAxis->GetLabel();
	if(pLabel)
		pLabel->SetText(str1);

	//m_ChartCtrl1.GetLeftAxis()->GetLabel()->SetText(str1);

	str1 = _T("温度位置");
	pAxis = m_ChartCtrl1.GetBottomAxis();
	if(pAxis)
		pLabel = pAxis->GetLabel();
	if(pLabel)
		pLabel->SetText(str1);
	pLineSerieMax = m_ChartCtrl1.CreateLineSerie();
	pLineSerieMax->SetSeriesOrdering(poNoOrdering);//设置为无序
	pLineSerieMin = m_ChartCtrl1.CreateLineSerie();
	pLineSerieMin->SetSeriesOrdering(poNoOrdering);//设置为无序

	//区域线



	//new初始化数据要和数据库连接
	CString select_sql;
	AccessResult res;
	if (SUCCEEDED(accessConnect.select("select * from region_info where region_state=1", res)))//检测查询成功
	{
		if(res.empty()) //查询结果为空
		{
			AfxMessageBox("现在还没有区域数据！");
			LOG(WARNING)<<"没有区域数据";
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
	//	RegionLine[i]->SetSeriesOrdering(poNoOrdering);//设置为无序
	//	RegionLine[i]->ClearSerie();
	//	double x[2]={RegionLeft[i],RegionLeft[i]};
	//	double y[2]={0,50};
	//	RegionLine[i]->AddPoints(x,y,2);
	//	RegionLine[i]->SetName(RegionName[i].GetBuffer());

	//}
	
	SetTimer(0,10,NULL);	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCurveDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	
		//计算高温中心
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
		//高温中心计算end

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
			//画高温中心 
			m_dc->SelectObject( &gwpen );
			m_dc->TextOut(MaxCenterPos,30,"中");
			m_dc->TextOut(MaxCenterPos,50,"心");
			m_dc->MoveTo(MaxCenterPos,m_crect.top);   
			m_dc->LineTo(MaxCenterPos,m_crect.bottom);
			m_dc->MoveTo(MaxCenterPos,m_crect.top);   
			m_dc->LineTo(MaxCenterPos,m_crect.bottom);
		}
		
	}
	CDialogEx::OnTimer(nIDEvent);
}
