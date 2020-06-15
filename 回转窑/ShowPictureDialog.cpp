// ShowPictureDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "ShowPictureDialog.h"
#include "afxdialogex.h"

#include "SQLConnect.hpp"
#include "easylogging++.h"

#define LEL 5
// ShowPictureDialog 对话框


IMPLEMENT_DYNAMIC(ShowPictureDialog, CDialogEx)

ShowPictureDialog::ShowPictureDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(ShowPictureDialog::IDD, pParent)
	, m_region_line(FALSE)
	, m_pj_line(FALSE)
	, m_max_value(FALSE)
	, m_min_value(FALSE)
{

}

ShowPictureDialog::~ShowPictureDialog()
{
}

void ShowPictureDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_region_line);
	DDX_Check(pDX, IDC_CHECK2, m_pj_line);
	DDX_Check(pDX, IDC_CHECK3, m_max_value);
	DDX_Check(pDX, IDC_CHECK4, m_min_value);
}


BEGIN_MESSAGE_MAP(ShowPictureDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ShowPictureDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &ShowPictureDialog::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &ShowPictureDialog::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK3, &ShowPictureDialog::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK4, &ShowPictureDialog::OnBnClickedCheck1)
END_MESSAGE_MAP()


// ShowPictureDialog 消息处理程序
void ShowPictureDialog::ShowPicture()
{
	CRect rect;
	CWnd *pWnd = NULL;
	pWnd =GetDlgItem(IDC_PICTURE_SHOW);//获取控件句柄??
	pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小
	CDC *pDc = pWnd->GetDC();//获取picture的DC
	pDc->SetStretchBltMode(COLORONCOLOR);
	m_image.StretchBlt(pDc->m_hDC, 0,0,rect.Width(),rect.Height(),0,0,m_image.GetWidth(),m_image.GetHeight(), SRCCOPY);//拉伸显示
	UpdateData(true);
	pDc->SetBkMode(TRANSPARENT);
	//画区域线
	if(m_region_line)
	{
		int RL_Show,RR_Show;
		for(int i=0;i<RegionName.size();i++)
		{
			RL_Show=rect.left+RegionLeft[i]*(rect.right-rect.left)/(picture_mat.cols-1);
			RR_Show=rect.left+RegionRight[i]*(rect.right-rect.left)/(picture_mat.cols-1);
			pDc->TextOut(RL_Show,rect.top,RegionName[i]);
			pDc->MoveTo(RL_Show,rect.top);   
			pDc->LineTo(RL_Show,rect.bottom);
			pDc->MoveTo(RR_Show,rect.top);   
			pDc->LineTo(RR_Show,rect.bottom);
		}
	}
	//画拼接线
	if(m_pj_line)
	{
		int RT_Show=0,RT_Show_sum=-1;
		for(int i=0;i<RegionName.size();i++)
		{
			//RT_Show_sum+=SplicingRows[i];
			RT_Show=RT_Show_sum;
			RT_Show=rect.top+RT_Show*(rect.bottom-rect.top)/(picture_mat.rows-1);
			pDc->MoveTo(rect.left ,RT_Show);   
			pDc->LineTo(rect.right,RT_Show);
			RT_Show_sum+=SplicingRows[i];
		}
	}
	//画温度
	if(m_max_value)
	{
		CString str;
		LOGFONT logfont;       //改变输出字体  
		ZeroMemory(&logfont, sizeof(LOGFONT));  
		logfont.lfCharSet = GB2312_CHARSET;  
		logfont.lfHeight = 12;      //设置字体的大小  
		HFONT hFont =CreateFontIndirect(&logfont);  

		SelectObject(pDc->m_hDC,hFont );
		CPen pen; 
		pen.CreatePen(PS_SOLID,1,RGB(0,255,255)); 
		pDc->SelectObject( &pen );
		SetTextColor(pDc->m_hDC, RGB(0,255,255));  
		for(int i=0;i<RegionName.size();i++)
		{
			int x=rect.left+MaxPos[i].x*(rect.right-rect.left)/(picture_mat.cols-1);
			int y=rect.top+MaxPos[i].y*(rect.bottom-rect.top)/(picture_mat.rows-1);
			str.Format("%0.2f℃",Max_local[i]);
			pDc->TextOut(x,y,str);
			pDc->MoveTo(x,y-LEL);   
			pDc->LineTo(x,y+LEL);
			pDc->MoveTo(x-LEL,y);    
			pDc->LineTo(x+LEL,y);  
		}
		pen.DeleteObject() ;	
		DeleteObject(hFont);
	}
	if(m_min_value)
	{
		CString str;
		LOGFONT logfont;       //改变输出字体  
		ZeroMemory(&logfont, sizeof(LOGFONT));  
		logfont.lfCharSet = GB2312_CHARSET;  
		logfont.lfHeight = 12;      //设置字体的大小  
		HFONT hFont =CreateFontIndirect(&logfont);  

		SelectObject(pDc->m_hDC,hFont );
		CPen pen; 
		pen.CreatePen(PS_SOLID,1,RGB(0,255,255)); 
		pDc->SelectObject( &pen );
		SetTextColor(pDc->m_hDC, RGB(0,255,255));  
		for(int i=0;i<RegionName.size();i++)
		{
			int x=rect.left+MinPos[i].x*(rect.right-rect.left)/(picture_mat.cols-1);
			int y=rect.top+MinPos[i].y*(rect.bottom-rect.top)/(picture_mat.rows-1);
			str.Format("%0.1f℃",Min_local[i]);
			pDc->TextOut(x,y,str);
			pDc->MoveTo(x,y-LEL);   
			pDc->LineTo(x,y+LEL);
			pDc->MoveTo(x-LEL,y);    
			pDc->LineTo(x+LEL,y);    
		}
		pen.DeleteObject() ;	
		DeleteObject(hFont);
	}
	pWnd->ReleaseDC(pDc);
}
void ShowMyLUT(cv::Mat & color, cv::Mat & image_gray)
{
	
	//image_gray.convertTo(grayImage, CV_8UC1, alpha,drift);
	Mat grayImage=image_gray;
	{

		MatND  hist;
		const int histSize = 256;
		float range[] = { 0, 255 };
		const float *ranges[] = { range };
		const int channels = 0;
		cv::calcHist(&grayImage, 1, &channels, Mat(), hist, 1, &histSize, ranges);
		float total = image_gray.rows* image_gray.cols;

		float bins[histSize] = { 0 };
		float binsAcc[histSize] = { 0 };
		int redcolor[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,2,4,4,4,6,6,6,8,9,9,11,12,12,13,15,15,15,17,17,17,20,20,20,22,23,23,24,26,26,27,29,29,29,31,31,31,35,36,36,37,38,38,39,41,41,41,44,44,44,46,47,47,48,49,49,50,52,52,53,55,55,55,58,58,58,59,60,60,61,63,63,64,68,68,68,69,69,69,71,72,72,74,75,75,76,77,77,78,82,82,82,85,85,85,86,87,87,88,89,89,90,92,92,92,96,96,96,98,99,99,101,102,102,103,104,104,105,109,109,109,109,109,109,110,111,111,112,114,114,115,117,117,117,119,119,119,120,120,120,122,123,123,124,125,125,125,127,127,127,129,130,130,131,131,131,132,134,134,134,135,135,135,138,138,138,140,141,141,141,142,142,142,143,143,143,146,146,146,148,149,149,151,152,152,152,152,152,152,153,153,153,154,154,154,156,157,157,157,158,158,158,158,158,158,161,161,161,162,162,162,163,163,163,164,166,166,166,166,166,166,167,167,167,168,168,168,169,171,171,171,172,172,172,172,172,172,174,175,175,175,176,176,176,177,177,177,180,180,180,180,180,180,181,181,181,181,182,182,182,185,185,185,186,186,186,186,186,186,187,189,189,189,190,190,190,191,191,191,193,194,194,194,194,194,194,195,195,195,198,198,198,199,199,199,200,200,200,200,200,200,201,203,203,203,204,204,204,205,205,205,206,208,208,208,208,208,208,209,209,209,211,212,212,213,213,213,213,214,214,214,214,214,214,217,217,217,218,218,218,219,220,220,221,222,222,222,222,222,222,223,223,223,225,226,226,226,227,227,227,229,229,229,231,231,231,230,230,230,230,231,231,231,232,232,232,232,232,232,232,232,232,233,233,233,233,234,234,234,233,233,233,233,233,233,234,235,235,235,236,236,236,235,235,235,235,235,235,236,237,237,237,238,238,238,237,237,237,237,237,237,238,238,238,239,240,240,240,239,239,239,240,240,240,240,240,240,240,240,240,240,241,241,241,242,242,242,242,242,242,242,242,242,243,243,243,243,244,244,244,244,244,244,244,244,244,245,245,245,245,246,246,246,246,246,246,246,246,246,247,247,247,247,248,248,248,249,249,249,247,247,247,247,247,247,248,248,248,248,248,248,248,247,247,247,249,249,249,248,248,248,248,248,248,248,248,248,248,247,247,247,247,247,247,248,248,248,248,248,248,248,247,247,247,248,248,248,248,248,248,248,247,247,247,247,247,247,247,247,247,247,247,247,248,248,248,248,247,247,247,247,247,247,248,248,248,248,248,248,248,247,247,247,247,247,247,247,247,247,246,246,246,246,247,247,247,247,247,247,247,247,247,248,248,248,247,247,247,247,248,248,248,247,247,247,246,246,246,247,248,248,248,247,247,247,247,247,247,248,248,248,247,247,247,247,247,247,247,247,247,247,247,247,247,248,248,248,247,247,247,247,248,248,248,248,248,248,248,248,248,248,248,248,248,247,247,247,248,248,248,248,248,248,247,247,247,248,249,249,249,248,248,248,249,249,249,248,248,248,247,247,247,247,247,247,247,246,246,246,246,246,246,246,246,246,245,245,245,245,246,246,246,246,246,246,245,245,245,245,245,245,244,243,243,243,243,243,243,243,243,243,244,244,244,244,243,243,243,243,243,243,242,242,242,242,242,242,242,242,242,242,241,241,241,242,242,242,241,241,241,240,240,240,240,240,240,240,241,241,241,239,239,239,239,239,239,240,240,240,240,240,240,240,238,238,238,239,239,239,239,239,239,239,238,238,238};
		int greencolor[]={13,13,12,11,11,10,10,10,10,10,10,10,8,8,8,8,8,8,7,7,7,7,6,6,6,5,5,5,5,5,5,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,2,2,2,3,3,3,4,4,4,4,5,5,5,4,4,4,4,4,4,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,8,8,8,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,11,11,11,12,12,12,13,13,13,14,15,15,15,15,15,15,17,17,17,18,18,18,19,20,20,20,21,21,21,21,21,21,23,23,23,24,24,24,24,25,25,25,27,27,27,27,27,27,28,29,29,29,30,30,31,32,32,32,33,33,33,33,33,33,34,35,35,35,36,36,36,37,37,37,39,39,39,39,39,39,39,40,40,40,42,42,42,44,44,44,45,45,45,45,45,45,46,47,47,47,48,48,48,49,49,49,50,51,51,51,51,51,51,52,52,52,54,54,54,55,56,56,56,57,57,57,57,57,57,59,59,59,60,60,60,61,61,61,62,63,63,63,63,63,63,64,64,64,65,66,66,66,67,67,68,69,69,69,69,69,69,71,71,71,72,72,72,72,73,73,73,75,75,75,76,76,76,77,77,77,78,79,79,79,80,80,80,83,83,83,83,83,83,84,85,85,86,87,87,87,89,89,89,90,90,90,91,91,91,92,93,93,94,96,96,96,96,96,96,97,98,98,98,99,99,100,102,102,102,102,102,102,104,104,104,105,106,106,107,108,108,108,110,110,110,110,110,110,112,113,113,113,114,114,114,116,116,116,117,117,117,118,119,119,120,121,121,122,123,123,123,123,123,123,125,125,125,126,127,127,128,129,129,129,129,129,129,132,132,132,133,133,133,134,135,135,136,137,137,137,138,138,138,140,140,140,141,141,141,142,143,143,143,146,146,146,147,147,147,148,149,149,150,151,151,151,152,152,152,154,154,154,156,156,156,157,157,157,158,159,159,159,161,161,161,163,163,163,164,165,165,166,167,167,167,167,167,167,170,170,170,171,172,172,173,173,173,174,175,175,175,178,178,178,179,179,179,180,181,181,182,183,183,183,183,183,183,185,185,185,187,188,188,188,189,189,190,191,191,191,193,193,193,195,195,195,196,197,197,197,198,198,198,199,199,199,200,200,200,201,202,202,202,203,203,203,205,205,205,206,206,206,207,207,207,207,207,207,208,209,209,209,211,211,211,211,211,211,212,213,213,213,214,214,214,216,216,216,218,218,218,217,217,217,218,219,219,219,220,220,220,222,222,222,223,223,223,224,224,224,225,226,226,226,227,227,227,227,227,227,228,228,228,228,229,229,229,230,230,230,232,232,232,233,233,233,234,234,234,234,234,234,234,236,236,236,237,237,237,238,238,238,238,238,238,238,239,239,239,240,240,240,241,241,241,241,242,242,242,243,243,243,244,244,244,246,246,246,247,247,247,247,248,248,248,248,248,248,248,248,248,249,249,249,249,250,250,250,250,250,250,250,250,250,251,251,251,251,251,251,251,252,252,252,252,252,252,252,252,252,253,253,253,253,254,254,254};
		int bluecolor[]={0,0,0,0,0,2,3,3,5,10,10,11,18,18,18,25,27,27,31,33,33,37,42,42,43,50,50,50,57,58,58,64,66,66,70,75,75,75,76,76,76,79,79,79,82,83,83,85,86,86,87,89,89,89,92,92,92,95,95,95,97,98,98,100,102,102,103,105,105,105,108,108,108,110,111,111,113,114,114,115,117,117,117,121,121,121,123,124,124,126,127,127,128,130,130,131,133,133,133,137,137,137,139,140,140,141,143,143,144,146,146,146,146,146,146,147,147,147,148,148,148,149,150,150,150,151,151,151,152,152,152,153,153,153,153,153,153,153,154,154,154,155,155,155,157,157,157,158,158,158,158,159,159,159,160,160,160,158,158,158,159,159,159,159,158,158,158,160,160,160,160,160,160,159,159,159,160,160,160,160,159,159,159,159,159,159,160,160,160,159,159,159,159,160,160,160,158,158,158,160,160,160,161,161,161,161,160,160,160,160,160,160,159,159,159,160,160,160,161,161,161,160,157,157,157,157,157,157,156,156,156,154,153,153,153,152,152,152,150,150,150,147,147,147,146,146,146,145,145,145,144,142,142,142,140,140,140,140,140,140,139,139,139,138,136,136,136,135,135,135,133,133,133,131,130,130,130,129,129,129,128,128,128,126,126,126,124,124,124,123,123,123,123,122,122,122,119,119,119,118,118,118,117,116,116,115,113,113,113,112,112,112,112,112,112,110,109,109,108,107,107,107,106,106,106,103,103,103,102,102,102,101,101,101,100,99,99,98,96,96,96,96,96,96,95,95,95,94,92,92,91,90,90,90,89,89,89,87,86,86,85,85,85,85,84,84,84,82,82,82,80,80,80,79,79,79,79,78,78,77,75,75,75,73,73,73,72,72,72,70,69,69,69,68,68,68,67,67,67,67,67,67,64,63,63,63,62,62,62,61,61,61,58,58,58,57,56,56,55,55,55,54,52,52,52,51,51,51,49,49,49,49,49,49,48,46,46,46,45,45,45,43,43,43,41,40,40,40,39,39,39,38,38,38,36,36,36,34,33,33,33,33,33,33,32,32,32,29,29,29,27,27,27,26,26,26,26,25,25,24,22,22,22,20,20,20,19,19,19,18,17,17,17,16,16,16,14,14,14,13,13,13,11,10,10,10,9,9,9,7,7,7,4,4,4,3,3,3,3,2,2,2,2,2,2,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,2,2,2,0,0,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,1,2,2,2,1,1,1,2,2,2,1,1,1,1,2,2,2,0,0,0,1,1,1,1,1,1,1,2,2,2,1,1,1,2,2,2,1,1,1,2,2,2,2,3,3,3,3,3,3,6,7,7,8,8,8,9,11,11,12,14,14,14,15,15,15,18,19,19,20,21,21,21,22,22,22,25,25,25,27,28,28,29,29,29,30,32,32,33,36,36,36,37,37,37,38,39,39,40,42,42,42,43,43,43,46,46,46,48,49,49,50,51,51,52,54,54,54,57,57,57,60,61,61,64,66,66,68,71,71,72,77,77,77,82,82,82,86,87,87,90,92,92,94,97,97,98,103,103,103,107,108,108,111,113,113,115,117,117,118,122,122,122,127,127,127,131,133,133,135,138,138,139,143,143,143,148,148,148,152,153,153,156,159,159,161,164,164,165,169,169,169,173,173,173,178,180,180,183,187,187,189,193,193,193,199,199,199,204,206,206,209,211,211,213,218,218,219,224,224,224,228,229,229,234,237,237,240,244,244,245};

		cv::Mat lut=cv::Mat(1, 256, CV_8UC3);

		vector<float> vectorBins;
		vector<float> maxBins;
		float sumBins = 0.0;
		int countMax = 0;
		float TValue = 0;

		// Find the mapping table
		for (int i = 0; i<histSize; i++)
		{
			float bin_val = hist.at<float>(i); // 第i灰度级上的数
			bins[i] = bin_val / total;

			if (bins[i] > 0)
			{
				vectorBins.push_back(bins[i]);
			}


		}

		// Calculate the Meadin value by 3 sapce
		for (int i = 1; i < vectorBins.size() - 1; i++)
		{
			if (vectorBins[i] < vectorBins[i - 1] && vectorBins[i - 1] < vectorBins[i + 1] || vectorBins[i] > vectorBins[i - 1] && vectorBins[i - 1]  > vectorBins[i + 1])
			{
				vectorBins[i] = vectorBins[i - 1];
			}
			else if (vectorBins[i] < vectorBins[i + 1] && vectorBins[i + 1] < vectorBins[i - 1] || vectorBins[i] > vectorBins[i + 1] && vectorBins[i + 1] > vectorBins[i - 1])
			{
				vectorBins[i] = vectorBins[i + 1];
			}
		}

		// Calculate the max peak value
		for (int i = 1; i < vectorBins.size() - 1; i++)
		{
			if (vectorBins[i] - vectorBins[i - 1] >= 0 && vectorBins[i+1] - vectorBins[i] <= 0)
			{
				maxBins.push_back(vectorBins[i]);
				sumBins += vectorBins[i]; 
				countMax++;
			}
		}

		TValue = sumBins / countMax;


		// Find the mapping table
		for (int i = 0; i<histSize; i++)
		{

			if (bins[i] > TValue)
			{
				bins[i] = TValue;
			}

			if (i>0)
			{
				binsAcc[i] = binsAcc[i - 1] + bins[i];
			}
			else
			{
				binsAcc[0] = bins[0];
			}
		}

		uchar * p;
		p =lut.ptr<uchar>(0);
		for (int i = 0; i<256; i++)  
		{  	
			//static_cast<uchar>(cvRound(binsAcc[i] * 255 / binsAcc[255]));

			int temp=(binsAcc[i] * 255 / binsAcc[255])*821/255.0+0.5;
			p[i * 3] = bluecolor[temp];  
			p[i * 3 + 1] = greencolor[temp];  
			p[i * 3 + 2] = redcolor[temp];  
		}  

		cv::Mat image_gray_3c;
		//单通道的灰度图，转换成R、G、B三通道值均相等的三通道图
		cv::cvtColor(grayImage, image_gray_3c, cv::COLOR_GRAY2RGB);
		LUT(image_gray_3c, lut, color);  
	}
}


void ShowPictureDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	Mat src;
	CFileDialog dlg(TRUE,"*","*.*", 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
		"JPEG文件(*.jpg)|*.jpg|BMP文件(*.bmp)|*.bmp|GIF文件(*.gif)|*.gif||",NULL); 
	if(dlg.DoModal()==IDOK) 
	{ 
		m_path=dlg.GetPathName(); 
		char a[6]={0},b[6]={0},c[6]={0},d[8]={0},e[8]={0}; 
		sscanf(dlg.GetFileTitle(),"%[^_]_%[^_]_%[^_]_%[^_]_%[^_]",a,b,c,d,e);
		min_global=atof(d);
		max_global=atof(e);
		src=imread(m_path.GetBuffer(),1);
		picture_mat=cv::Mat(src.rows,src.cols,CV_32FC1);
		uchar * p;
		float* pf;
		for(int i=0;i<src.rows;i++)
		{
			p =src.ptr<uchar>(i);
			pf = picture_mat.ptr<float>(i);
			for(int j=0,j3=0;j<src.cols;j++)
			{
				j3=j*3;
				pf[j]=min_global+p[j3 + 2]*(max_global-min_global)/255;
			}
		}
		//Mat srcchannel;
		//vector<Mat> channels;
		Mat channels[3];
		split(src,channels);
		ShowMyLUT(src, channels[0]);
		MinPos.clear();MaxPos.clear();Min_local.clear();Max_local.clear();
		Point tempMinPos,tempMaxpos;
		double tempmin,tempmax;
		for(int i=0;i<RegionName.size();i++)
		{
			minMaxLoc(picture_mat(Range::all(), Range(RegionLeft[i],RegionRight[i]+1)), &tempmin, &tempmax, &tempMinPos, &tempMaxpos);
			tempMinPos.x+=RegionLeft[i];
			tempMaxpos.x+=RegionLeft[i];
			MinPos.push_back(tempMinPos);
			MaxPos.push_back(tempMaxpos);
			Min_local.push_back(tempmin/emiss[i]);
			Max_local.push_back(tempmax/emiss[i]);
		}
		//minMaxLoc((*srctemp)(Range(ExtRect.top,ExtRect.bottom+1), Range(ExtRect.left,ExtRect.right+1)), &MinTemp, &MaxTemp, &MinPos, &MaxPos);
		ImageOrMat imagechange;
		imagechange.MatToCImage(src, m_image);
		ShowPicture();
	}
}


BOOL ShowPictureDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	int index_sum;

	AccessResult res;
	if (SUCCEEDED(accessConnect.select("select * from region_info where region_state=1 order by region_index", res))) //检测查询成功
	{
		if (res.empty()) //查询结果为空
		{
			AfxMessageBox("现在还没有区域数据！");
			LOG(WARNING) << "没有区域数据";
		}
		else
		{
			for (auto& record : res)
			{
				RegionName.push_back(record["region_name"].c_str());
				RegionLeft.push_back(stoi(record["region_left"]));
				RegionRight.push_back(stoi(record["region_right"]));
				emiss.push_back(stof(record["region_emissivity"]));
			}
		}
	}

	if( SUCCEEDED( accessConnect.select("select * from sys_para where para_name='signalequipment'", res) )) //检测查询成功
	{
		if(res.empty()) //查询结果为空
		{
			AfxMessageBox("现在还没有设置拼接数，请先设置！");
			LOG(WARNING)<<"没有设置拼接数";
		}
		else
		{
			index_sum = stoi(res[0]["para3"]);
		}
	}
	CString select_sql;
	select_sql.Format("select * from sys_para where para_name='splicingregion' and para_index<%d order by para_index", index_sum);
	if (SUCCEEDED(accessConnect.select(select_sql.GetString(), res))) //检测查询成功
	{
		for (auto& record : res)
			SplicingRows.push_back(stoi(record["para1"]) - stoi(record["para0"]));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void ShowPictureDialog::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowPicture();
}
