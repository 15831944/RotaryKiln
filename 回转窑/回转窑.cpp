
// 回转窑.cpp : 定义应用程序的类行为。
//
#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "回转窑.h"
#include "MainFrm.h"
#include "Resource.h"
#include "回转窑Doc.h"
#include "回转窑View.h"
//#include "ImageOrMat.h"
#include "ImageProcessing.hpp"
#include "../thermalcamera/YoseenSDK/ThermalCamera.hpp"
#include "Afxsock.h"
#include "easylogging++.h"

// 注册
#include "MachineRegisterDialog.h"


_INITIALIZE_EASYLOGGINGPP
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString userNumber;
CString userPermission;
YoseenLoginInfo loginInfo;
typedef struct 
{
	CThermalDialog *p_thermaldialog;
	CSplicingDialog *p_splicingdialog;
	CRegionTableDialog *p_regiontabledialog;
	CCurveDialog *p_curvedialog;
}ALL_DIALOG_P;
ALL_DIALOG_P alldialog_p;
CThermalDialog *p_thermaldialog;
CSplicingDialog *p_splicingdialog;
CRegionTableDialog *p_regiontabledialog;
CCurveDialog *p_curvedialog;

//全局变量
Mat thermal_src,thermal_srctemp;
int thermal_heigth,thermal_width;
CWnd * pWnd_show;
//CDC* pDCDs_show;
CDC* pDCDs_memDC;
double *curve_x, *curve_yMax,*curve_yMin;
ATL::CImage m_image;
ATL::CImage m_splicing_image;
AreaVessel areavessel;//默认区域容器
Vector<PIECESPLICING> piecesplicing;
int alarmSetVersion=0;
int m_imageVersion=0;
int m_imageVersion_old=-1;
int nowAreaVersion=0;
int oldAreaVersion=0;
bool HermalShowFlag = true;
bool SignFlag = true;
bool ReStartFlag=false;
int signindex=-1;
AreaType nowAreaType=DefaultRgn;
std::vector<CPoint> nowRegionPoints;// 当前区域边界点


// C回转窑App

BEGIN_MESSAGE_MAP(C回转窑App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &C回转窑App::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// C回转窑App 构造

int GetLineIndex()
{
	SQLResult res;
	if (S_OK == accessConnect.executeSQL("select max(line_index) as max_index from region_temperature", res)
		&& !res.empty() && !res.begin()->second.empty())
		return stoi(res["max_index"].front());
	else
		return 0;
	return -1;
}

bool findFirstFileExists(LPCTSTR lpPath, DWORD dwFilter)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(lpPath, &fd);
	BOOL bFilter = (FALSE == dwFilter) ? TRUE : fd.dwFileAttributes & dwFilter;
	BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;
	FindClose(hFind);
	return RetValue;
}

bool WriteTemp(AreaVessel& areavessel)
{
	static int LineIndex=GetLineIndex();
	/*static int count=0;
	if(count++<50) return true;
	count=0;*/
	static double StartTime;
	double time = abs((double)getTickCount() - StartTime)/getTickFrequency();
	if(time<30)
	   return true;
	StartTime=(double)getTickCount();
	
	if(-1==LineIndex) return false;
	LineIndex++;

	CString sql_command;
	int ress;

	for(int i=0;i<areavessel.defaultAreaNum;i++)
	{
		sql_command.Format("insert into region_temperature (line_index,region_index,region_temp) values(%d,%d,%0.2f)",LineIndex,atoi(areavessel.areaVector[i].Id),areavessel.areaVector[i].MaxTemp);
		
		if (!accessConnect.executeSQL(sql_command.GetString()))
		{
			AfxMessageBox("写入温度数据失败!");
			return false;
		}
	}
	if (findFirstFileExists(_T("opcdata"), FALSE)!=TRUE)
	{
		CTime tm = CTime::GetCurrentTime(); 
		CString sendData="%"+tm.Format("%Y%m%d%H%M%S")+"%";
		CString tempstr;
		for(int i=0;i<areavessel.areaVector.size();i++)
		{ 
			tempstr.Format("%0.2f%%",areavessel.areaVector[i].MaxTemp);
            sendData=sendData+tempstr;
		}
		ofstream outfile("opcdata"); 
		outfile<<sendData<<endl;
		outfile.close();
	}
	return true;
}

C回转窑App::C回转窑App()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("回转窑.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 C回转窑App 对象
C回转窑App theApp;



//获取热像仪数据并显示线程函数
 //CWinThread  * pVideoThread; 
int ConnectSign(CSocket& mysocket)
{
	int index_sum;

	//获取信号地址、端口

	CString IP,Port;
	SQLResult res;
	accessConnect.executeSQL("select * from sys_para where para_name='signalequipment'", res);
	if (!res.empty() && !res.begin()->second.empty())
	{
		IP = res["para0"].front().c_str();
		Port = res["para1"].front().c_str();
	}
	if (!mysocket.Create()) //创建套接字
	{
		AfxMessageBox("套接字创建失败!");
		LOG(ERROR)<< "套接字创建失败";
		return 0;
	}
	if (!mysocket.Connect(IP, atoi(Port))) //连接服务器
	{
		AfxMessageBox("连接服务器失败!");
		LOG(ERROR)<< "连接服务器失败";
		return 0;
	}
	return 1;
}
DWORD WINAPI   ThreadSignProc(LPVOID lpParameter)
{
	CSocket mysocket;
	if(!ConnectSign(mysocket))
	{
		SignFlag=false;
		return 0;
	}
  // CSocket* mysocket=(CSocket*)lpParameter;
   unsigned char SendBuf[12] = {0};
   unsigned char ReceiveBuf[200];
   SendBuf[5] = 0x06;
   SendBuf[6] = 255;//单元标识符
   SendBuf[7] = 0x02;//m_cmdword;//命令字
   SendBuf[11] = 0x02;
   while(SignFlag)
   {
	   ////获取信号  
	   mysocket.Send(SendBuf,12); 
	   ReceiveBuf[9]=0;
	   //接收传来的数据
	   int receive_data_num =  mysocket.Receive(ReceiveBuf,200);
	   static bool signflag=false;
	   if(signindex==-1)
	   {
			if(ReceiveBuf[9]==1)
			{
	   			signindex=0;
	   			signflag=true;
			}
	   }
	   else
	   {

	   		switch (ReceiveBuf[9])
	   		{
	   			case 0:
	   				signflag=false;
	   				break;
	   			case 1:
	   				{
	   					if(signflag) 
	   						break;
	   					else
	   						signflag=true;
	   					signindex=0;
	   					break;
	   				}
	   			case 2:
	   				if(signflag) 
	   					break;
	   				else
	   					signflag=true;
	   				signindex++;
	   				break;
	   		}
	   }
	   Sleep(5);
   }
   SignFlag=true;
   return 0;
}
DWORD WINAPI   ThreadVideoProc(LPVOID lpParameter)
{
	//HWND hwnd = (HWND)lpParameter;
	////连接信号设备
	//创建信号处理线程
	AfxBeginThread((AFX_THREADPROC)ThreadSignProc, (LPVOID)&signindex,THREAD_PRIORITY_IDLE);

	//连接信号设备END
	ALL_DIALOG_P *alldialog_p=(ALL_DIALOG_P*)lpParameter;
	ImageOrMat imagechange;
	//Mat src,srcTemp;
	//vector<Mat> splicing_mat;
	Mat all_splicing_mat;
	//ATL::CImage m_image;
	//VideoCapture video;
	//YoseenLoginInfo loginInfo;
	//strcpy(loginInfo.CameraAddr,"192.168.1.104");
	CameraBasicInfo cameraBasicInfo;
	s32 userHandle;
	//strcpy(loginInfo.CameraAddr,"192.168.1.103");
	s32 previewHandle=Camera_login(loginInfo,cameraBasicInfo,userHandle);
	if(previewHandle<0)
	{
		AfxMessageBox("连接热像仪失败！");
		LOG(ERROR)<< "连接热像仪失败";
		HermalShowFlag=false;
		return previewHandle;
	}
	else
		LOG(INFO) << "热像仪连接成功";
	 GetCamera_Info(userHandle,thermal_heigth,thermal_width);
	 p_thermaldialog->heigth=thermal_heigth;
	 p_thermaldialog->width=thermal_width;
	 p_thermaldialog->OnSize(1,0,0);
	 SRCMat CustomMat;
	 CustomMat.tempMat=cv::Mat(thermal_heigth,thermal_width,CV_32FC1);
	 CustomMat.temMatRGB=cv::Mat(thermal_heigth,thermal_width,CV_8UC3);
	 CustomMat.emissivityMat=Mat::ones(thermal_heigth,thermal_width,CV_32FC1);
	 vector<AreaTempInfo> areaVector=areavessel.areaVector;
	 for (int i=0; i<areaVector.size();i++)
	 {
		//CustomMat.emissivityMat(Range(areaVector[i].Points[0].y, areaVector[i].Points[2].y), Range(areaVector[i].Points[0].x, areaVector[i].Points[1].x)) = areaVector[i].emissivity;
		CustomMat.emissivityMat(Range(areaVector[i].Points[0].y, areaVector[i].Points[1].y), Range(areaVector[i].Points[0].x, areaVector[i].Points[1].x)) = 1.0;
	 }
	 Camera_Preview(&CustomMat,NULL);

	 Sleep(1000);
	
	{
		//初始化横剖面曲线视图的数据信息
		curve_x=new double[thermal_width];
		for(int i=0;i<thermal_width;i++) curve_x[i]=double(i);
		curve_yMax=new double[thermal_width];
		curve_yMin=new double[thermal_width];
		memset(curve_yMax,0,sizeof(double)*thermal_width);
		memset(curve_yMin,0,sizeof(double)*thermal_width);
		////初始化拼接视图矩阵
		//for (int i=0; i<piecesplicing.size();i++)
		//{
		///*	piecesplicing[i].top_in_all=splicing_y_size;
		//	piecesplicing[i].bottom_in_all=splicing_y_size+piecesplicing[i].heigth;
		//	splicing_y_size+=piecesplicing[i].heigth;*/
		//	splicing_mat.push_back(Mat(piecesplicing[i].heigth,curve_x_size,CV_8SC3,Scalar(255,255,255)));
		//}
		CRect rect;
		pWnd_show = alldialog_p->p_thermaldialog->GetDlgItem(IDC_THERMAL_SHOW);//获取控件句柄
		CDC* pDCDs_show=pWnd_show->GetDC();
		while (HermalShowFlag)
		{  
			//long t1=GetTickCount();//程序段开始前取得系统运行时间(ms)        
			
			//video >> src;
			
			//
			Sleep(20);
			CustomMat.temMatRGB.copyTo(thermal_src);
			CustomMat.tempMat.copyTo(thermal_srctemp);
			//imshow("aa",CustomMat);
			//waitKey(0);
			//src=CustomMat;
			if(m_imageVersion_old!=m_imageVersion)
			{
			    imagechange.MatToCImage(thermal_src, m_image);
				m_imageVersion_old=m_imageVersion;
			}
			
			////计算拼接图像矩阵
			////获取信号
			static bool Notfirstsignflag=false;
			static int oldsignindex=signindex;
			if(oldsignindex!=signindex)
			{
				oldsignindex=signindex;
				if(signindex==0)
				{
					if(Notfirstsignflag)
					{
						Mat mat_for_save=cv::Mat(all_splicing_mat.rows,all_splicing_mat.cols,CV_8UC3);
						Mat mat_for_show=cv::Mat(all_splicing_mat.rows,all_splicing_mat.cols,CV_8UC3);
						uchar *p_show,*p_save;
						float *pf;
						int j3;
						double max,min;
						minMaxIdx(all_splicing_mat,&min,&max);
						cv::Mat grayImage=cv::Mat(all_splicing_mat.rows,all_splicing_mat.cols,CV_8UC1);
						double alpha = 255.0 / (max - min);
						double drift=-alpha * min;
						all_splicing_mat.convertTo(grayImage, CV_8UC1, alpha,drift);
						MyLUT(mat_for_show,grayImage);
						cv::cvtColor(grayImage, mat_for_save, cv::COLOR_GRAY2RGB);
						/*for(int i=0;i<all_splicing_mat.rows;i++)
						{
							p_show =mat_for_show.ptr<uchar>(i);
							p_save=mat_for_save.ptr<uchar>(i);
							pf = all_splicing_mat.ptr<float>(i);
							for(int j=0;j<all_splicing_mat.cols;j++)
							{
								j3=j*3;
								uchar temp=((pf[j]-min)*255/(max-min));
								p_save[j3 + 0]=p_save[j3 + 1]=p_save[j3 + 2]=temp;
								p_show[j3 + 0]=(255-temp);
								p_show[j3 + 1]=abs(127-temp);
								p_show[j3 + 2]=temp;		
							}
						}*/
						imagechange.MatToCImage(mat_for_save, m_splicing_image);
						//保存拼接图像
						CTime tm = CTime::GetCurrentTime(); 
						CString strpicturename,strPath;
						strPath.Format(_T("picture\\%d年\\%d月\\%d日"),tm.GetYear(),tm.GetMonth(),tm.GetDay());
						if (!PathFileExists(strPath))
						{
							//不存在
							system("md "+strPath);
						}
						strpicturename.Format(_T("picture\\%d年\\%d月\\%d日\\%d_%d_%d_%.2f_%.2f.jpg"),tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetHour(),tm.GetMinute(),tm.GetSecond(),min,max);
						m_splicing_image.Save(strpicturename);
						
									
						//转换拼接图像
						imagechange.MatToCImage(mat_for_show, m_splicing_image);
						//CWnd *pWndpj = alldialog_p->p_splicingdialog->GetDlgItem(IDC_SPLICING_SHOW);//获取控件句柄??
						//pWndpj->GetClientRect(&rect);//获取句柄指向控件区域的大小
						//CDC *pDcpj = pWndpj->GetDC();//获取picture的DC
						//pDcpj->SetStretchBltMode(COLORONCOLOR);
						//m_splicing_image.StretchBlt(pDcpj->m_hDC, 0,0,rect.Width(),rect.Height(),0,0,m_splicing_image.GetWidth(),m_splicing_image.GetHeight(), SRCCOPY);//拉伸显示
						//pWndpj->ReleaseDC(pDcpj);
						//显示结束
						all_splicing_mat=thermal_srctemp(Range(piecesplicing[0].top,piecesplicing[0].bottom),Range(0,thermal_width));
					}
					else
					{
						 Notfirstsignflag=true;
					}
					all_splicing_mat=thermal_srctemp(Range(piecesplicing[0].top,piecesplicing[0].bottom),Range(0,thermal_width));

				}
				else
				{
					if(signindex<piecesplicing.size())
					{
					   cv::vconcat(all_splicing_mat,thermal_srctemp(Range(piecesplicing[signindex].top,piecesplicing[signindex].bottom),Range::all()), all_splicing_mat);
					}
				}
			}
			//显示拼接图像
			//if(Notfirstsignflag)
			if(!m_splicing_image.IsNull())
			{
				CWnd *pWndpj = alldialog_p->p_splicingdialog->GetDlgItem(IDC_SPLICING_SHOW);//获取控件句柄??
				pWndpj->GetClientRect(&rect);//获取句柄指向控件区域的大小
				CDC *pDcpj = pWndpj->GetDC();//获取picture的DC
				pDcpj->SetStretchBltMode(COLORONCOLOR);
				m_splicing_image.StretchBlt(pDcpj->m_hDC, 0,0,rect.Width(),rect.Height(),0,0,m_splicing_image.GetWidth(),m_splicing_image.GetHeight(), SRCCOPY);//拉伸显示
				pWndpj->ReleaseDC(pDcpj);
			}

					
			//CWnd *pWnd = NULL;
			//CRect rect;
			//pWnd_show = alldialog_p->p_thermaldialog->GetDlgItem(IDC_THERMAL_SHOW);//获取控件句柄
			pWnd_show->GetClientRect(&rect);//获取句柄指向控件区域的大小
			//跟新报警设置
			areavessel.UpdateAlarmInfo(alarmSetVersion,alldialog_p->p_regiontabledialog->m_alarmsetdialog);
			//显示默认区域和温度
			areavessel.onDraw();
			//热像仪报警保存
			for(int i=0;i<areavessel.defaultAreaNum;i++)
			{
				if(areavessel.areaVector[i].AlarmInfo.alarmlevel!=NormalLevel)
				{
					alldialog_p->p_thermaldialog->OnSave(1);
				}
			}
			//把温度写入数据库
			 WriteTemp(areavessel);
			//显示自定义区域
			if(nowAreaType==PointRgn)
			{//点区域时显示当前鼠标位置的温度
				CRect WindowRectThermalShow;
				pWnd_show->GetWindowRect(&WindowRectThermalShow);
				AreaTempInfo tempArea;
				CPoint  point;
				GetCursorPos(&point);
				if (WindowRectThermalShow.PtInRect(point))
				{
					tempArea.MaxPos.x=(point.x-WindowRectThermalShow.left);
					tempArea.MaxPos.y=(point.y-WindowRectThermalShow.top);
					//tempArea.pDCDs=pWnd->GetDC();
					CDC * pDCDs_show=pWnd_show->GetDC();
					pDCDs_show->SetBkMode(TRANSPARENT);
					pWnd_show->GetClientRect(&rect);//获取句柄指向控件区域的大小
					float temp=thermal_srctemp.at<float>(tempArea.MaxPos.y*thermal_heigth/rect.Height(),tempArea.MaxPos.x*thermal_width/rect.Width());	
					tempArea.onShowTemperature(tempArea.MaxPos,temp,pDCDs_show,5,-10);//温度没有设定
				}		
			}
			if(oldAreaVersion+1==nowAreaVersion)
			{
				pWnd_show->GetClientRect(&rect);//获取句柄指向控件区域的大小
				std::vector<CPoint> nowRegionPointsT;// 当前区域边界点转换
				for(int i=0;i<nowRegionPoints.size();i++)
				{
					nowRegionPointsT.push_back(CPoint(1.0*nowRegionPoints[i].x*thermal_width/rect.Width(),1.0*nowRegionPoints[i].y*thermal_heigth/rect.Height()));
				}
			    AreaTempInfo tempArea("ls","ls",nowAreaType,nowRegionPointsT);
				
				//tempArea.pDCDs=pWnd->GetDC();
				tempArea.ShowResize(rect.Width(),rect.Height());
				tempArea.onShowRegion(pWnd_show->GetDC());
			}
			else if(oldAreaVersion+2==nowAreaVersion)
			{
				  CString str;
				  static int userareaindex=0;
				  userareaindex++;
				  str.Format("N%d",userareaindex);
				  for(int i=0;i<nowRegionPoints.size();i++)
				  {
					  nowRegionPoints[i].x=nowRegionPoints[i].x*thermal_width/rect.Width();
					  nowRegionPoints[i].y=nowRegionPoints[i].y*thermal_heigth/rect.Height();
				  }
				  AreaTempInfo tempArea(str,str,nowAreaType,nowRegionPoints);
				   pWnd_show->GetClientRect(&rect);//获取句柄指向控件区域的大小
				  tempArea.ShowResize(rect.Width(),rect.Height());
				  areavessel.AddArea(tempArea);
				  areavessel.SaveRegion();
				  oldAreaVersion+=2;
			}
			//修改RegionTable窗体信息
			//安全退出，必须要放到修改RegionTable窗体之前否则不能正常退出
			//if (HermalShowCloseFlag)
			//	break;
			areavessel.UpdateRegionTable(alldialog_p->p_regiontabledialog->m_listctrl);
			

			 //更新横剖面曲线视图的数据信息
			 Mat testInput;
			 thermal_srctemp.convertTo(testInput,CV_32F);
			 AreaTempInfo::ComputeCurveTemperature(testInput);//没有用并行计算,分部计算
		
		}
	    alldialog_p->p_curvedialog->ShowWindow(SW_HIDE);
		SignFlag=!SignFlag;
		delete[] curve_x;
		delete[] curve_yMax;
		delete[] curve_yMin;
		Camera_exit(userHandle,previewHandle);
	}
	for(int i=0;i<10&&!SignFlag;i++)
	{
		Sleep(10);
	}
	if(ReStartFlag)
	{
		char buf[256];
		::GetModuleFileName(NULL,buf,sizeof(buf));
		CString strPath = buf;
		WinExec(strPath, SW_SHOW);//启动strPath路径下的执行文件
	}
	 ::SendMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);//最常用
	return 0;
}
// C回转窑App 初始化
BOOL C回转窑App::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(C回转窑Doc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(C回转窑View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 打开数据库
	if (S_OK != accessConnect.openDatabase("RotaryKiln.accdb"))
	{
		AfxMessageBox(("数据库连接失败:" + accessConnect.getLastError()).c_str());
		LOG(ERROR) << "login:" + accessConnect.getLastError();
	}

	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);	

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->SetWindowText("回转窑");  
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	//初始化日志
	easyloggingpp::Configurations confFromFile("dailyrecord.conf");
	easyloggingpp::Loggers::reconfigureAllLoggers(confFromFile); // Re-configures all the loggers to current configuration file
	easyloggingpp::Configurations defaultConf;
	defaultConf.setToDefault();

	//登陆对话框
	LOG(WARNING)<<"没有区域数据";
	CLogon mylogon;
	int nRet = mylogon.DoModal(); 
	if ( nRet != LOGON_TRUE)
		return FALSE; 

	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;//要求包含MainFrm.h头文件
	pFrame->m_wndStatusBar.SetPaneText(0,"用户名："+userNumber);

	int index_sum = 0;
	SQLResult res;
	if (accessConnect.executeSQL("select * from region_info where region_state=1", res) != S_OK)
	{
		AfxMessageBox("执行SQL语句出错！");
		LOG(WARNING) << "执行SQL语句出错";
	}
	if (res.empty() || res.begin()->second.empty())
	{
		AfxMessageBox("现在还没有区域数据！");
		LOG(WARNING) << "没有区域数据";
	}
	else
	{
		int resnum = res.begin()->second.size();
		for (int i = 0; i < resnum; i++)
		{
			AreaTempInfo tempareateminfo{
				res["region_index"].front().c_str(),
				res["region_name"].front().c_str(),
				RectRgn,
				stoi(res["region_left"].front()),
				stoi(res["region_top"].front()),
				stoi(res["region_right"].front()),
				stoi(res["region_bottom"].front()),
				stof(res["region_alarm1"].front()),
				stof(res["region_alarm2"].front()),
				stof(res["region_alarm3"].front())
			};
			tempareateminfo.SetEmissivity(stof(res["region_emissivity"].front()));
			tempareateminfo.MaxTemp = 0.0;
			areavessel.AddArea(tempareateminfo);
			areavessel.defaultAreaNum++;
		}
	}

	//从文件读取自定义区域
	areavessel.ReadRegion();
	if (S_OK != accessConnect.executeSQL("select * from sys_para where para_name='signalequipment'", res)
		|| res.empty() || res.begin()->second.empty())
	{
		AfxMessageBox("现在还没有设置拼接数，请先设置！");
		LOG(WARNING) << "没有设置拼接数";
	}
	else
	{
		index_sum = stoi(res["para3"].front());
	}
	accessConnect.executeSQL("select * from sys_para where para_name='splicingregion' and para_index<" 
		+ std::to_string(index_sum) + " order by para_index", res);

	if (!res.empty() && !res.begin()->second.empty())
	{
		int resnum = res.begin()->second.size();
		for (int index = 0; index < resnum; index++)
		{
			piecesplicing.push_back(PIECESPLICING(index, stoi(res["para0"].front()), stoi(res["para1"].front())));
		}
	}

	//获取热像仪地址、用户名、密码
	accessConnect.executeSQL("select * from sys_para where para_name='thermalequipment'", res);
	if (!res.empty() && !res.begin()->second.empty())
	{
		strcpy(loginInfo.CameraAddr, res["para1"].front().c_str());
	}
	

	//获取热像仪数据并显示
	//HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadVideoProc, (LPVOID)this, 0, NULL);
	//CloseHandle(hThread);
	alldialog_p.p_thermaldialog=p_thermaldialog;
	alldialog_p.p_splicingdialog=p_splicingdialog;
	alldialog_p.p_regiontabledialog=p_regiontabledialog;
	alldialog_p.p_curvedialog=p_curvedialog;
	//

	//初始化分区域表格视图
	CListCtrl *m_listctrl=&p_regiontabledialog->m_listctrl; 
	// 在列表视图控件中插入列表项，并设置列表子项文本 
	CString str;
	LV_ITEM lvitem;
	vector<AreaTempInfo> areaVector=areavessel.areaVector;
	for (int i=0; i<areavessel.defaultAreaNum;i++)
	{
		//
		p_regiontabledialog->m_alarmsetdialog.v_alarmTH1.push_back(areaVector[i].AlarmInfo.alarmThreshold[0]);
		p_regiontabledialog->m_alarmsetdialog.v_alarmTH2.push_back(areaVector[i].AlarmInfo.alarmThreshold[1]);
		p_regiontabledialog->m_alarmsetdialog.v_alarmTH3.push_back(areaVector[i].AlarmInfo.alarmThreshold[2]);
		//m_listctrl->InsertItem(i,kiln[i].objectname);
		memset((char *)&lvitem, '\0', sizeof(LV_ITEM));
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvitem.iItem = i; lvitem.iSubItem =0; lvitem.iImage = 0;
		//lvitem.pszText = "文字";
		m_listctrl->InsertItem(&lvitem);
		//m_listctrl->InsertItem(i,kiln[i].objectname); 
		//m_listctrl->InsertItem(&lvitem);
		//m_listctrl->SetItemText(i,0,kiln[i].objectname); 
		//areaVector[i].MaxTemp=0.0;
		str.Format("%0.1f", areaVector[i].MaxTemp);
		m_listctrl->SetItemText(i,1,str);   
		m_listctrl->SetItemText(i,2, "--"); 
		m_listctrl->SetItemText(i,3,areaVector[i].Name); 
	}
	
	
	//创建第一个线程ThreadProc,相对优先级THREAD_PRIORITY_IDLE面对任何等级调整为1    
	AfxBeginThread((AFX_THREADPROC)ThreadVideoProc, (LPVOID)&alldialog_p,THREAD_PRIORITY_IDLE);

	//注册
	std::string strKey="2551b7a69845407894c8aa642189d8a1";//明码：中国江苏南京工程学院

	auto key = VerifyMachineDialog();
	if (key == -1)
	{
		MessageBox(nullptr, "注册码格式错误", "验证失败", MB_OK);
		return FALSE;
	}
	else if (key == 0)
	{
		MessageBox(nullptr, "注册码错误", "验证失败", MB_OK);
		return FALSE;
	}


	//typedef void (*lpCall)(char*);
	////创建dll句柄  
	//HINSTANCE hDll;  
	////装载dll  
	//hDll = LoadLibrary(_T("dialog.dll"));  
	//if(NULL==hDll)
	//{  
	//	AfxMessageBox(_T("DLL加载失败！"));  
	//}  
	////检索指定DLL中的输出库函数地址  
	//lpCall Check_Register=(lpCall)GetProcAddress(hDll,"Check_Register");  
	//lpCall GetRegisterKey=(lpCall)GetProcAddress(hDll,"GetRegisterKey");  
	//if(NULL==Check_Register)  
	//{  
	//	AfxMessageBox(_T("DLL内部函数调用失败！"));  
	//}  
	////创建DLL对话框  
	//HINSTANCE exe_hInstance = GetModuleHandle(NULL);
	//AfxSetResourceHandle(hDll); //切换状态
	//Check_Register((char*)strKey.c_str());
	//AfxSetResourceHandle(exe_hInstance); //恢复状态
	//FreeLibrary(hDll);


	return TRUE;
}

int C回转窑App::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);
	return CWinAppEx::ExitInstance();
}

// C回转窑App 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void C回转窑App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// C回转窑App 自定义加载/保存方法

void C回转窑App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void C回转窑App::LoadCustomState()
{
}

void C回转窑App::SaveCustomState()
{
}

// C回转窑App 消息处理程序



