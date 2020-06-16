#pragma once
//#include "stdafx.h"
#include "ImageOrMat.h"
#include "AlarmSetDialog.h"
#include<fstream>

#define LEL 5

//声明外部变量
extern Mat thermal_src, thermal_srctemp;
extern int thermal_heigth, thermal_width;
extern CWnd* pWnd_show;
extern CDC* pDCDs_memDC;
extern double* curve_x, * curve_yMax, * curve_yMin;

enum ALARMLEVEL
{
	NormalLevel = 0, FirstLevelAlarm = 1, SecondlAlarm = 2, ThirdLevelAlarm = 3
};
typedef struct ALARM_ST
{
	double alarmThreshold[3];
	int alarmlevel;
	ALARM_ST() { memset(alarmThreshold, 0, sizeof(double) * 3); alarmlevel = NormalLevel; }
	ALARM_ST(double alarmThreshold[3]) { memcpy(this->alarmThreshold, alarmThreshold, sizeof(double) * 3); alarmlevel = NormalLevel; }
}ALARM;

// 区域类型
enum AreaType
{
	DefaultRgn = -1, PointRgn = 0, RectRgn = 1, EllipticRgn = 2, LineRgn = 3, PolygonRgn = 4
};
// 区域温度信息
class AreaTempInfo
{
public:
	int Flag;//物体区域的状态0-正常，1-删除，2-隐藏
	CString  Id;//物体区域编号
	CString Name;//物体区域名称
	int Type;   //区域类型
	std::vector<CPoint> Points;// 当前区域边界点
	std::vector<CPoint> Points_Show;// 当前区域边界点显示
	double emissivity;
	Point MinPos;//物体最低温度在物体区域的位置，是相对区域的位置，不是在物体中的位置
	Point MaxPos;//物体最高温度在物体区域的位置，是相对区域的位置，不是在物体中的位置
	double MinTemp;//物体区域最低温度
	double MaxTemp;//物体区域最高温度
	double MeanTemp;//物体区域平均温度
	ALARM AlarmInfo;//物体区域报警信息
	CRgn Region;	// 区域
	CRgn RegionShow;// 显示区域
	CRect ExtRect;	// 外接矩形
	Mat Mask;       // 区域掩模
	double StartTime;//温度跟新时间
	int Width_Show;//显示区域的宽度
	int Heigth_Show;//显示区域的高度
	AreaTempInfo() {}
	AreaTempInfo(const CString& id, const CString& name, const int& type, const CRect& rect, double alarm1, double alarm2, double alarm3)
		: Id(id), Name(name), Type(type)
	{
		Flag = 0;
		emissivity = 1.0;
		AlarmInfo.alarmThreshold[0] = alarm1; AlarmInfo.alarmThreshold[1] = alarm2; AlarmInfo.alarmThreshold[2] = alarm3;
		Points.push_back(CPoint(rect.left, rect.top));
		//Points.push_back(CPoint(rect.right,rect.top));
		Points.push_back(CPoint(rect.right, rect.bottom));
		//Points.push_back(CPoint(rect.left,rect.bottom));
		switch (type)
		{
		case RectRgn:
			Region.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
			break;
		case EllipticRgn:
			Region.CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);
			break;
		}
		ExtRect = rect;
		InitMask();
	}
	AreaTempInfo(const CString& id, const CString& name, const int& type, LONG left, LONG top, LONG right, LONG bottom, double alarm1, double alarm2, double alarm3)
		:Id(id), Name(name), Type(type)
	{
		Flag = 0;
		emissivity = 1.0;
		CRect rect(left, top, right, bottom);
		AlarmInfo.alarmThreshold[0] = alarm1; AlarmInfo.alarmThreshold[1] = alarm2; AlarmInfo.alarmThreshold[2] = alarm3;
		Points.push_back(CPoint(rect.left, rect.top));
		//Points.push_back(CPoint(rect.right,rect.top));
		Points.push_back(CPoint(rect.right, rect.bottom));
		//Points.push_back(CPoint(rect.left,rect.bottom));
		switch (type)
		{
		case RectRgn:
			Region.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
			break;
		case EllipticRgn:
			Region.CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);
			break;
		}
		ExtRect = rect;
		InitMask();
	}
	AreaTempInfo(const CString& id, const CString& name, const int& type, const std::vector<CPoint> points)
		: Id(id), Name(name), Type(type)
	{
		Flag = 0;
		emissivity = 1.0;
		Points.assign(points.begin(), points.end());
		InitRegion();
		if (Region.m_hObject == NULL) return;
		Region.GetRgnBox(&ExtRect);
		InitMask();
	}
	AreaTempInfo(const CString& id, const CString& name, const std::vector<CPoint> points, const Point& minpos, const Point& maxpos, double mintemp, double maxtemp, Mat mask)
		:Id(id), Name(name), MinPos(minpos), MaxPos(maxpos), MinTemp(mintemp), MaxTemp(maxtemp), Mask(mask)
	{
		//copy(points.begin(),points.end(),Points.begin());
		Flag = 0;
		emissivity = 1.0;
		Points.assign(points.begin(), points.end());
		InitRegion();
		InitMask();
	}
	~AreaTempInfo()
	{
		Region.DeleteObject();
	}
	AreaTempInfo(const AreaTempInfo& area)
		:emissivity(area.emissivity), Id(area.Id), Name(area.Name), MinPos(area.MinPos), MaxPos(area.MaxPos), MinTemp(area.MinTemp), MaxTemp(area.MaxTemp), Type(area.Type), Mask(area.Mask)
	{
		Flag = area.Flag;
		Width_Show = area.Width_Show;
		Heigth_Show = area.Heigth_Show;
		AlarmInfo = area.AlarmInfo;
		Points.assign(area.Points.begin(), area.Points.end());
		Points_Show.assign(area.Points_Show.begin(), area.Points_Show.end());
		ExtRect = area.ExtRect;
		//CRgn rgn1;
		Region.CreateRectRgn(0, 0, 0, 0);
		Region.CopyRgn(&area.Region);
		if (area.RegionShow.m_hObject != NULL)
		{
			RegionShow.CreateRectRgn(0, 0, 0, 0);
			RegionShow.CopyRgn(&area.RegionShow);
		}
	}

protected:

	void InitRegion()
	{
		switch (Type)
		{
		case RectRgn:
			Region.CreateRectRgn(Points[0].x, Points[0].y, Points[1].x, Points[1].y);
			break;
		case EllipticRgn:
			Region.CreateEllipticRgn(Points[0].x, Points[0].y, Points[1].x, Points[1].y);
			break;
		case PolygonRgn:
			if (Points.size() > 2)
			{
				Region.CreatePolygonRgn(Points.data(), Points.size(), WINDING);
				ExtRect.SetRect(Points[0], Points[0]);
				// 设置最大包围矩形
				for (int i = 0; i < Points.size(); i++)
				{
					if (Points[i].x < ExtRect.left)
						ExtRect.left = Points[i].x;
					else if (Points[i].x > ExtRect.right)
						ExtRect.right = Points[i].x;
					if (Points[i].y < ExtRect.top)
						ExtRect.top = Points[i].y;
					else if (Points[i].y > ExtRect.bottom)
						ExtRect.bottom = Points[i].y;
				}
			}
			break;
		}
	}
	void InitMask()
	{
		if (Type == RectRgn || Type == PointRgn)
		{
			Mask = NULL;
			return;
		}
		// 遍历所属区域
		//Mask.create(ExtRect.bottom-ExtRect.top+1, ExtRect.right-ExtRect.left+1, CV_8UC1);
		Mask = Mat::zeros(ExtRect.bottom - ExtRect.top + 1, ExtRect.right - ExtRect.left + 1, CV_8UC1);
		for (size_t y = ExtRect.top; y < ExtRect.bottom + 1; ++y)
		{
			for (size_t x = ExtRect.left; x < ExtRect.right + 1; ++x)
			{
				if (Region.PtInRegion(x, y))
				{
					Mask.at<uchar>(y - ExtRect.top, x - ExtRect.left) = 1;
				}
			}
		}
	}
	void ComputerMinMaxLoc()
	{
		minMaxLoc((thermal_srctemp)(Range(ExtRect.top, ExtRect.bottom + 1), Range(ExtRect.left, ExtRect.right + 1)), &MinTemp, &MaxTemp, &MinPos, &MaxPos, Mask);
		//minMaxLoc((*thermal_srctemp)(Range(ExtRect.top,ExtRect.bottom+1), Range(ExtRect.left,ExtRect.right+1)), &MinTemp, &MaxTemp, &MinPos, &MaxPos);
		MinTemp = MinTemp;
		MaxTemp = MaxTemp;
		AlarmInfo.alarmlevel = ThirdLevelAlarm;
		if (MaxTemp < AlarmInfo.alarmThreshold[0])
		{
			AlarmInfo.alarmlevel = NormalLevel;
		}
		else
		{
			if (MaxTemp < AlarmInfo.alarmThreshold[1])
			{
				AlarmInfo.alarmlevel = FirstLevelAlarm;
			}
			else
			{
				if (MaxTemp < AlarmInfo.alarmThreshold[2])
				{
					AlarmInfo.alarmlevel = SecondlAlarm;
				}
			}
		}
	}
public:
	//显示窗口改变
	void ShowResize(int bmp_width, int bmp_heigth)
	{
		Width_Show = bmp_width;
		Heigth_Show = bmp_heigth;
		//改变显示边界点
		Points_Show.clear();
		for (int i = 0; i < Points.size(); i++)
		{
			Points_Show.push_back(CPoint(Points[i].x * bmp_width / thermal_width, Points[i].y * bmp_heigth / thermal_heigth));
		}
		//改变显示区域
		RegionShow.DeleteObject();
		switch (Type)
		{
		case RectRgn:
			RegionShow.CreateRectRgn(Points_Show[0].x, Points_Show[0].y, Points_Show[1].x, Points_Show[1].y);
			break;
		case EllipticRgn:
			RegionShow.CreateEllipticRgn(Points_Show[0].x, Points_Show[0].y, Points_Show[1].x, Points_Show[1].y);
			break;
		case PolygonRgn:
			RegionShow.CreatePolygonRgn(Points_Show.data(), Points_Show.size(), WINDING);
			break;
		}
	}
	//设置发射率
	void SetEmissivity(double emissivity)
	{
		this->emissivity = emissivity;
	}
	double GetEmissivity()
	{
		return emissivity;
	}
	// 赋值
	AreaTempInfo& operator = (const AreaTempInfo& area)
	{
		emissivity = area.emissivity;
		Id = area.Id;
		Name = area.Name;
		Points = area.Points;
		MaxPos = area.MaxPos;
		MinPos = area.MinPos;
		MaxTemp = area.MaxTemp;
		MinTemp = area.MinTemp;
		Region.CopyRgn(&area.Region);
		AlarmInfo = area.AlarmInfo;
		ExtRect = area.ExtRect;
		Mask = area.Mask;
		return *this;
	}
	// 绘制区域
	void onShowRegion(CDC* pDCDs)
	{
		//pDCDs_show->FrameRgn(&RegionShow, &brush, LineSize,LineSize);//边框绘制
		//pDCDs_show->FrameRgn(&Region, &brush, LineSize,LineSize);//边框绘制
		LOGFONT logfont;       //改变输出字体
		ZeroMemory(&logfont, sizeof(LOGFONT));
		logfont.lfCharSet = GB2312_CHARSET;
		logfont.lfHeight = 12;      //设置字体的大小
		HFONT hFont = CreateFontIndirect(&logfont);

		SelectObject(pDCDs->m_hDC, hFont);
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
		pDCDs->SelectObject(&pen);
		SetTextColor(pDCDs->m_hDC, RGB(0, 255, 255));
		switch (Type)
		{
		case RectRgn:
		{
			pDCDs->TextOut(Points_Show[0].x + 2, Points_Show[0].y + 3, Name);
			CBrush brush(RGB(0, 255, 255));//创建画刷
			pDCDs->FrameRgn(&RegionShow, &brush, 2, 2);//边框绘制
			brush.DeleteObject();
			break;
		}
		case EllipticRgn:
		{
			pDCDs->TextOut((Points_Show[0].x + Points_Show[1].x) / 2, Points_Show[0].y, Name);
			CBrush brush(RGB(0, 255, 255));//创建画刷
			pDCDs->FrameRgn(&RegionShow, &brush, 2, 2);//边框绘制
			brush.DeleteObject();
			break;
		}
		default:
		{
			pDCDs->TextOut(Points_Show[0].x, Points_Show[0].y, Name);
			pDCDs->MoveTo(Points_Show[Points_Show.size() - 1].x, Points_Show[Points_Show.size() - 1].y);
			for (int i = 0; i < Points_Show.size(); i++)
				pDCDs->LineTo(Points_Show[i].x, Points_Show[i].y);
		}
		}
		pen.DeleteObject();
		DeleteObject(hFont);
	}
	void onShowTemperature(Point temperature_pos, float temperature, CDC* pDCDs, int X_offset = 0, int Y_offset = 0)
	{
		CString str;
		LOGFONT logfont;       //改变输出字体
		ZeroMemory(&logfont, sizeof(LOGFONT));
		logfont.lfCharSet = GB2312_CHARSET;
		logfont.lfHeight = 12;      //设置字体的大小
		HFONT hFont = CreateFontIndirect(&logfont);

		SelectObject(pDCDs->m_hDC, hFont);
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
		pDCDs->SelectObject(&pen);
		SetTextColor(pDCDs->m_hDC, RGB(0, 255, 255));
		str.Format("%0.1f℃", temperature);
		pDCDs->TextOut(temperature_pos.x + X_offset, temperature_pos.y + Y_offset, str);
		pDCDs->MoveTo(temperature_pos.x, temperature_pos.y - LEL);
		pDCDs->LineTo(temperature_pos.x, temperature_pos.y + LEL);
		pDCDs->MoveTo(temperature_pos.x - LEL, temperature_pos.y);
		pDCDs->LineTo(temperature_pos.x + LEL, temperature_pos.y);
		pen.DeleteObject();
		DeleteObject(hFont);
	}

	void onShowTemperature()
	{
		Point minLoc, maxLoc;
		minLoc.x = 1.0 * (MinPos.x + ExtRect.left) * Width_Show / thermal_width;
		minLoc.y = 1.0 * (MinPos.y + ExtRect.top) * Heigth_Show / thermal_heigth;
		maxLoc.x = 1.0 * (MaxPos.x + ExtRect.left) * Width_Show / thermal_width;
		maxLoc.y = 1.0 * (MaxPos.y + ExtRect.top) * Heigth_Show / thermal_heigth;
		onShowTemperature(maxLoc, MaxTemp, pDCDs_memDC);
		onShowTemperature(minLoc, MinTemp, pDCDs_memDC);
	}
	// 绘制区域
	void onDraw()
	{
		double time = abs((double)getTickCount() - StartTime) / getTickFrequency();
		if (time >= 2)
		{
			StartTime = (double)getTickCount();
			ComputerMinMaxLoc();
		}
		if (Flag) return;
		CBrush brush(RGB(0, 0, 255));
		onShowRegion(pDCDs_memDC);
		onShowTemperature();
	}
	static void ComputeCurveTemperature(Mat& src)
	{
		static int s = 0;
		int bstep = 3;
		int e;
		e = s + bstep;
		if (thermal_width <= e)
		{
			for (int i = s; i < thermal_width; i++)
			{
				minMaxLoc(src.col(i), curve_yMin + i, curve_yMax + i);
			}
			s = 0;
		}
		else
		{
			for (int i = s; i < e; i++)
			{
				minMaxLoc(src.col(i), curve_yMin + i, curve_yMax + i);
			}
			s = e;
		}
	}
};
//Mat* AreaTempInfo::src=NULL;
//Mat* AreaTempInfo::thermal_srctemp=NULL;
//CWnd * AreaTempInfo::pWnd_show=NULL;
//CDC* AreaTempInfo::pDCDs_show=NULL;

//区域容器
class AreaVessel
{
public:
	vector<AreaTempInfo> areaVector;
	int defaultAreaNum;
	CRect rectShow;
	LV_ITEM lvitem;
	LONG alarmSetVersion;
public:
	AreaVessel()
	{
		alarmSetVersion = 0;
		memset((char*)&lvitem, '\0', sizeof(LV_ITEM));
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvitem.iSubItem = 0;
		defaultAreaNum = 0;
	}
	void AddArea(AreaTempInfo& areatempinfo)
	{
		areaVector.push_back(areatempinfo);
	}
	void ShowResize(int bmp_width, int bmp_heigth)
	{
		for (int i = 0; i < areaVector.size(); i++)
		{
			areaVector[i].ShowResize(bmp_width, bmp_heigth);
		}
	}
	void onDraw()
	{
		CRect rect;
		CDC* pDc = pWnd_show->GetDC();//获取picture的DC
		pWnd_show->GetClientRect(&rect);//获取句柄指向控件区域的大小
		//for(int i=0;i<areaVector.size();i++)
		//{
		//	areaVector[i].src=&src;
		//	//areaVector[i].thermal_srctemp=&thermal_srctemp;
		//}
		static CRect rect_old;
		static int bmp_width = (rect.Width() + 3) / 4 * 4, bmp_heigth = 1.0 * bmp_width * thermal_heigth / thermal_width + 0.5;
		if (rect_old != rect)
		{
			rect_old = rect;
			bmp_width = (rect.Width() + 3) / 4 * 4;
			bmp_heigth = bmp_heigth = 1.0 * bmp_width * thermal_heigth / thermal_width + 0.5;
			ShowResize(bmp_width, bmp_heigth);
			pWnd_show->GetParent()->Invalidate(TRUE);
		}
		//矩阵的宽度必须是4的倍数
		resize(thermal_src, thermal_src, Size(bmp_width, bmp_heigth), 0, 0, INTER_CUBIC);//INTER_CUBIC  INTER_LINEAR
		BITMAPINFO* m_pBMI;
		CBitmap bmp;
		int biBitCount = 24;//图像类型
		//int lineByte=(_src.cols * biBitCount+31)/32*4;
		m_pBMI = new BITMAPINFO;//生成彩色图像的信息头
		m_pBMI->bmiHeader.biBitCount = 24;
		m_pBMI->bmiHeader.biClrImportant = 0;
		m_pBMI->bmiHeader.biClrUsed = 0;
		m_pBMI->bmiHeader.biCompression = BI_RGB;
		m_pBMI->bmiHeader.biHeight = -bmp_heigth;
		m_pBMI->bmiHeader.biWidth = bmp_width;
		m_pBMI->bmiHeader.biPlanes = 1;
		m_pBMI->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		m_pBMI->bmiHeader.biXPelsPerMeter = 0;
		m_pBMI->bmiHeader.biYPelsPerMeter = 0;
		//	m_pBMI->bmiHeader.biSizeImage=lineByte*_src.rows;

		CDC memDC;
		pDCDs_memDC = &memDC;
		BOOL flg = memDC.CreateCompatibleDC(pDc);
		if (flg)
		{
			memDC.SetBkMode(TRANSPARENT);
			bmp.CreateCompatibleBitmap(pDc, rect.Width(), rect.Height());
			memDC.SelectObject(&bmp);
			SetDIBits(pDc->m_hDC, bmp, 0, abs(m_pBMI->bmiHeader.biHeight), thermal_src.data, m_pBMI, DIB_RGB_COLORS);
		}
		/*AreaTempInfo::pDCDs_show=&memDC;
		AreaTempInfo::pWnd_show=pWnd_show;*/

		for (int i = 0; i < areaVector.size(); i++)
		{
			//areaVector[i].src=&src;
			//areaVector[i].thermal_srctemp=&thermal_srctemp;
			//areaVector[i].pDCDs_show=&memDC;
			//areaVector[i].pWnd_show=pWnd_show;
			areaVector[i].onDraw();
		}
		//SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE);
		//pDc->StretchBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,thermal_width,thermal_heigth,SRCCOPY);
		//pDc->StretchBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,src.cols,src.rows,SRCCOPY);
		//pDc->BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);
		pDc->BitBlt(0, 0, bmp_width, bmp_heigth, &memDC, 0, 0, SRCCOPY);
		memDC.DeleteDC();
		memDC.DeleteDC();
		pWnd_show->ReleaseDC(pDc);
		bmp.DeleteObject();
		delete m_pBMI;
	}
	void UpdateRegionTable(CListCtrl& m_listctrl)
	{
		CString str;
		for (int i = 0; i < defaultAreaNum; i++)
		{
			str.Format("%0.1f", areaVector[i].MaxTemp);
			m_listctrl.SetItemText(i, 1, str);
			lvitem.iItem = i;
			lvitem.iImage = areaVector[i].AlarmInfo.alarmlevel;
			//lvitem.pszText = "文字";
			m_listctrl.SetItem(&lvitem);
			/*LV_ITEM lvitem1 = {0};;
			lvitem1.iItem = i;
			lvitem1.iSubItem = 0;
			lvitem1.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
			alldialog_p->p_regiontabledialog->m_listctrl.GetItem(&lvitem1);
			int a=lvitem1.iImage;
			CString str;
			str.Format("%d",a);
			AfxMessageBox(str);*/
		}
	}
	void UpdateAlarmInfo(LONG alarmSetVersion, const CAlarmSetDialog& m_alarmsetdialog)
	{
		if (this->alarmSetVersion == alarmSetVersion)
			return;
		this->alarmSetVersion = alarmSetVersion;
		for (int i = 0; i < defaultAreaNum; i++)
		{
			areaVector[i].AlarmInfo.alarmThreshold[0] = m_alarmsetdialog.v_alarmTH1[i];
			areaVector[i].AlarmInfo.alarmThreshold[1] = m_alarmsetdialog.v_alarmTH2[i];
			areaVector[i].AlarmInfo.alarmThreshold[2] = m_alarmsetdialog.v_alarmTH3[i];
		}
	}
	void SaveRegion()
	{
		ofstream outfile("region.dat", ios::binary);
		if (!outfile)
		{
			AfxMessageBox("打开自定义区域文件失败！");
			return;
		}
		int RegionNum = 0;
		for (int i = defaultAreaNum; i < areaVector.size(); i++)
		{
			if (areaVector[i].Flag == 0)
				RegionNum++;
		}
		outfile.write((char*)&RegionNum, sizeof(RegionNum));
		for (int i = defaultAreaNum; i < areaVector.size(); i++)
		{
			if (areaVector[i].Flag != 0) continue;
			int PointNum = areaVector[i].Points.size();
			outfile.write((char*)&PointNum, sizeof(PointNum));
			int Type = areaVector[i].Type;
			outfile.write((char*)&Type, sizeof(Type));
			for (int j = 0; j < PointNum; j++)
			{
				CPoint tempPoints;
				tempPoints = areaVector[i].Points[j];
				outfile.write((char*)&tempPoints, sizeof(tempPoints));
			}
		}
		outfile.close();
	}
	void ReadRegion()
	{
		ifstream infile("region.dat", ios::binary);
		if (!infile)
		{
			AfxMessageBox("打开自定义区域文件失败！");
			return;
		}
		int RegionNum;
		infile.read((char*)&RegionNum, sizeof(RegionNum));
		for (int i = 0; i < RegionNum; i++)
		{
			int PointNum;
			infile.read((char*)&PointNum, sizeof(PointNum));
			int RegionType;
			infile.read((char*)&RegionType, sizeof(RegionType));
			vector<CPoint> inPoints;
			for (int j = 0; j < PointNum; j++)
			{
				CPoint tempPoints;
				infile.read((char*)&tempPoints, sizeof(tempPoints));
				inPoints.push_back(tempPoints);
			}
			CString str;
			str.Format("D%d", i);
			AreaTempInfo tempArea(str, str, RegionType, inPoints);
			//CRect rect;
			//pWnd_show->GetClientRect(&rect);//获取句柄指向控件区域的大小
			//tempArea.ShowResize(rect.Width(),rect.Height());
			AddArea(tempArea);
		}
		infile.close();
	}
};

//拼接块结构体
typedef struct PIECE_SPLICING
{
	int index;
	int top;
	int bottom;
	int top_in_all;
	int bottom_in_all;
	int heigth;
	PIECE_SPLICING() :index(0), top(0), heigth(0) {};
	PIECE_SPLICING(int index, int top, int bottom)
	{
		this->index = index; this->top = top; this->bottom = bottom;
		heigth = bottom - top + 1;
	};
} PIECESPLICING;

//计算横剖面曲线视图的温度数据
//单核计算
//void ComputeCurveTemperature(Mat &src)
//{
//	static int s=0;
//	int bstep=1;
//	int e;
//	e=s+bstep;
//	if(curve_x_size<=e)
//	{
//		for(int i=s;i<curve_x_size;i++)
//		{
//			minMaxLoc(src.col(i), curve_yMin+i, curve_yMax+i);
//		}
//		s=0;
//	}
//	else
//	{
//		for(int i=s;i<e;i++)
//		{
//			minMaxLoc(src.col(i), curve_yMin+i, curve_yMax+i);
//		}
//		s=e;
//	}
//
//}
//用opencv parallel_for_并行计算
//class parallelCurveTemperatureBody : public ParallelLoopBody//参考官方给出的answer，构造一个并行的循环体类
//{
//public:
//	parallelCurveTemperatureBody(Mat& _src)//class constructor
//	{
//		src = &_src;
//	}
//	void operator()(const Range& range) const//重载操作符（）
//	{
//		Mat& srcMat = *src;
//		int stepSrc = srcMat.cols*srcMat.channels();//获取每一行的元素总个数（相当于cols*channels，等同于step1)
//		for (int colIdx = range.start; colIdx < range.end; ++colIdx)
//		{
//			float* pData = (float*)srcMat.col(colIdx).data;
//			curve_yMax[colIdx]=pData[0];
//			curve_yMin[colIdx]=pData[0];
//			float temp;
//			for (int i = 1; i < srcMat.rows; ++i)
//			{
//				temp= pData[i*stepSrc];
//				if(temp>curve_yMax[colIdx])
//				{
//					curve_yMax[colIdx]=temp;
//				}
//				else if(temp<curve_yMin[colIdx])
//				{
//					curve_yMin[colIdx]=temp;
//				}
//			}
//		}
//	}
//
//private:
//	Mat* src;
//};

//void parallelCurveTemperatureWithParallel_for_(Mat _src)//'parallel_for_' loop
//{
//	int totalCols = _src.cols;
//	parallel_for_(Range(0, totalCols), parallelCurveTemperatureBody(_src));
//
//}