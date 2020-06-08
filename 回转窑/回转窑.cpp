
// ��תҤ.cpp : ����Ӧ�ó��������Ϊ��
//
#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "��תҤ.h"
#include "MainFrm.h"
#include "Resource.h"
#include "��תҤDoc.h"
#include "��תҤView.h"
//#include "ImageOrMat.h"
#include "ImageProcessing.hpp"
#include "../thermalcamera/YoseenSDK/ThermalCamera.hpp"
#include "Afxsock.h"
#include "easylogging++.h"

// ע��
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

//ȫ�ֱ���
Mat thermal_src,thermal_srctemp;
int thermal_heigth,thermal_width;
CWnd * pWnd_show;
//CDC* pDCDs_show;
CDC* pDCDs_memDC;
double *curve_x, *curve_yMax,*curve_yMin;
ATL::CImage m_image;
ATL::CImage m_splicing_image;
AreaVessel areavessel;//Ĭ����������
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
std::vector<CPoint> nowRegionPoints;// ��ǰ����߽��


// C��תҤApp

BEGIN_MESSAGE_MAP(C��תҤApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &C��תҤApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// C��תҤApp ����

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
			AfxMessageBox("д���¶�����ʧ��!");
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

C��תҤApp::C��תҤApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("��תҤ.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� C��תҤApp ����
C��תҤApp theApp;



//��ȡ���������ݲ���ʾ�̺߳���
 //CWinThread  * pVideoThread; 
int ConnectSign(CSocket& mysocket)
{
	int index_sum;

	//��ȡ�źŵ�ַ���˿�

	CString IP,Port;
	SQLResult res;
	accessConnect.executeSQL("select * from sys_para where para_name='signalequipment'", res);
	if (!res.empty() && !res.begin()->second.empty())
	{
		IP = res["para0"].front().c_str();
		Port = res["para1"].front().c_str();
	}
	if (!mysocket.Create()) //�����׽���
	{
		AfxMessageBox("�׽��ִ���ʧ��!");
		LOG(ERROR)<< "�׽��ִ���ʧ��";
		return 0;
	}
	if (!mysocket.Connect(IP, atoi(Port))) //���ӷ�����
	{
		AfxMessageBox("���ӷ�����ʧ��!");
		LOG(ERROR)<< "���ӷ�����ʧ��";
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
   SendBuf[6] = 255;//��Ԫ��ʶ��
   SendBuf[7] = 0x02;//m_cmdword;//������
   SendBuf[11] = 0x02;
   while(SignFlag)
   {
	   ////��ȡ�ź�  
	   mysocket.Send(SendBuf,12); 
	   ReceiveBuf[9]=0;
	   //���մ���������
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
	////�����ź��豸
	//�����źŴ����߳�
	AfxBeginThread((AFX_THREADPROC)ThreadSignProc, (LPVOID)&signindex,THREAD_PRIORITY_IDLE);

	//�����ź��豸END
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
		AfxMessageBox("����������ʧ�ܣ�");
		LOG(ERROR)<< "����������ʧ��";
		HermalShowFlag=false;
		return previewHandle;
	}
	else
		LOG(INFO) << "���������ӳɹ�";
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
		//��ʼ��������������ͼ��������Ϣ
		curve_x=new double[thermal_width];
		for(int i=0;i<thermal_width;i++) curve_x[i]=double(i);
		curve_yMax=new double[thermal_width];
		curve_yMin=new double[thermal_width];
		memset(curve_yMax,0,sizeof(double)*thermal_width);
		memset(curve_yMin,0,sizeof(double)*thermal_width);
		////��ʼ��ƴ����ͼ����
		//for (int i=0; i<piecesplicing.size();i++)
		//{
		///*	piecesplicing[i].top_in_all=splicing_y_size;
		//	piecesplicing[i].bottom_in_all=splicing_y_size+piecesplicing[i].heigth;
		//	splicing_y_size+=piecesplicing[i].heigth;*/
		//	splicing_mat.push_back(Mat(piecesplicing[i].heigth,curve_x_size,CV_8SC3,Scalar(255,255,255)));
		//}
		CRect rect;
		pWnd_show = alldialog_p->p_thermaldialog->GetDlgItem(IDC_THERMAL_SHOW);//��ȡ�ؼ����
		CDC* pDCDs_show=pWnd_show->GetDC();
		while (HermalShowFlag)
		{  
			//long t1=GetTickCount();//����ο�ʼǰȡ��ϵͳ����ʱ��(ms)        
			
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
			
			////����ƴ��ͼ�����
			////��ȡ�ź�
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
						//����ƴ��ͼ��
						CTime tm = CTime::GetCurrentTime(); 
						CString strpicturename,strPath;
						strPath.Format(_T("picture\\%d��\\%d��\\%d��"),tm.GetYear(),tm.GetMonth(),tm.GetDay());
						if (!PathFileExists(strPath))
						{
							//������
							system("md "+strPath);
						}
						strpicturename.Format(_T("picture\\%d��\\%d��\\%d��\\%d_%d_%d_%.2f_%.2f.jpg"),tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetHour(),tm.GetMinute(),tm.GetSecond(),min,max);
						m_splicing_image.Save(strpicturename);
						
									
						//ת��ƴ��ͼ��
						imagechange.MatToCImage(mat_for_show, m_splicing_image);
						//CWnd *pWndpj = alldialog_p->p_splicingdialog->GetDlgItem(IDC_SPLICING_SHOW);//��ȡ�ؼ����??
						//pWndpj->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С
						//CDC *pDcpj = pWndpj->GetDC();//��ȡpicture��DC
						//pDcpj->SetStretchBltMode(COLORONCOLOR);
						//m_splicing_image.StretchBlt(pDcpj->m_hDC, 0,0,rect.Width(),rect.Height(),0,0,m_splicing_image.GetWidth(),m_splicing_image.GetHeight(), SRCCOPY);//������ʾ
						//pWndpj->ReleaseDC(pDcpj);
						//��ʾ����
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
			//��ʾƴ��ͼ��
			//if(Notfirstsignflag)
			if(!m_splicing_image.IsNull())
			{
				CWnd *pWndpj = alldialog_p->p_splicingdialog->GetDlgItem(IDC_SPLICING_SHOW);//��ȡ�ؼ����??
				pWndpj->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С
				CDC *pDcpj = pWndpj->GetDC();//��ȡpicture��DC
				pDcpj->SetStretchBltMode(COLORONCOLOR);
				m_splicing_image.StretchBlt(pDcpj->m_hDC, 0,0,rect.Width(),rect.Height(),0,0,m_splicing_image.GetWidth(),m_splicing_image.GetHeight(), SRCCOPY);//������ʾ
				pWndpj->ReleaseDC(pDcpj);
			}

					
			//CWnd *pWnd = NULL;
			//CRect rect;
			//pWnd_show = alldialog_p->p_thermaldialog->GetDlgItem(IDC_THERMAL_SHOW);//��ȡ�ؼ����
			pWnd_show->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С
			//���±�������
			areavessel.UpdateAlarmInfo(alarmSetVersion,alldialog_p->p_regiontabledialog->m_alarmsetdialog);
			//��ʾĬ��������¶�
			areavessel.onDraw();
			//�����Ǳ�������
			for(int i=0;i<areavessel.defaultAreaNum;i++)
			{
				if(areavessel.areaVector[i].AlarmInfo.alarmlevel!=NormalLevel)
				{
					alldialog_p->p_thermaldialog->OnSave(1);
				}
			}
			//���¶�д�����ݿ�
			 WriteTemp(areavessel);
			//��ʾ�Զ�������
			if(nowAreaType==PointRgn)
			{//������ʱ��ʾ��ǰ���λ�õ��¶�
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
					pWnd_show->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С
					float temp=thermal_srctemp.at<float>(tempArea.MaxPos.y*thermal_heigth/rect.Height(),tempArea.MaxPos.x*thermal_width/rect.Width());	
					tempArea.onShowTemperature(tempArea.MaxPos,temp,pDCDs_show,5,-10);//�¶�û���趨
				}		
			}
			if(oldAreaVersion+1==nowAreaVersion)
			{
				pWnd_show->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С
				std::vector<CPoint> nowRegionPointsT;// ��ǰ����߽��ת��
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
				   pWnd_show->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С
				  tempArea.ShowResize(rect.Width(),rect.Height());
				  areavessel.AddArea(tempArea);
				  areavessel.SaveRegion();
				  oldAreaVersion+=2;
			}
			//�޸�RegionTable������Ϣ
			//��ȫ�˳�������Ҫ�ŵ��޸�RegionTable����֮ǰ�����������˳�
			//if (HermalShowCloseFlag)
			//	break;
			areavessel.UpdateRegionTable(alldialog_p->p_regiontabledialog->m_listctrl);
			

			 //���º�����������ͼ��������Ϣ
			 Mat testInput;
			 thermal_srctemp.convertTo(testInput,CV_32F);
			 AreaTempInfo::ComputeCurveTemperature(testInput);//û���ò��м���,�ֲ�����
		
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
		WinExec(strPath, SW_SHOW);//����strPath·���µ�ִ���ļ�
	}
	 ::SendMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);//���
	return 0;
}
// C��תҤApp ��ʼ��
BOOL C��תҤApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(C��תҤDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(C��תҤView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// �����ݿ�
	if (S_OK != accessConnect.openDatabase("RotaryKiln.accdb"))
	{
		AfxMessageBox(("���ݿ�����ʧ��:" + accessConnect.getLastError()).c_str());
		LOG(ERROR) << "login:" + accessConnect.getLastError();
	}

	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);	

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->SetWindowText("��תҤ");  
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	//��ʼ����־
	easyloggingpp::Configurations confFromFile("dailyrecord.conf");
	easyloggingpp::Loggers::reconfigureAllLoggers(confFromFile); // Re-configures all the loggers to current configuration file
	easyloggingpp::Configurations defaultConf;
	defaultConf.setToDefault();

	//��½�Ի���
	LOG(WARNING)<<"û����������";
	CLogon mylogon;
	int nRet = mylogon.DoModal(); 
	if ( nRet != LOGON_TRUE)
		return FALSE; 

	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;//Ҫ�����MainFrm.hͷ�ļ�
	pFrame->m_wndStatusBar.SetPaneText(0,"�û�����"+userNumber);

	int index_sum = 0;
	SQLResult res;
	if (accessConnect.executeSQL("select * from region_info where region_state=1", res) != S_OK)
	{
		AfxMessageBox("ִ��SQL������");
		LOG(WARNING) << "ִ��SQL������";
	}
	if (res.empty() || res.begin()->second.empty())
	{
		AfxMessageBox("���ڻ�û���������ݣ�");
		LOG(WARNING) << "û����������";
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

	//���ļ���ȡ�Զ�������
	areavessel.ReadRegion();
	if (S_OK != accessConnect.executeSQL("select * from sys_para where para_name='signalequipment'", res)
		|| res.empty() || res.begin()->second.empty())
	{
		AfxMessageBox("���ڻ�û������ƴ�������������ã�");
		LOG(WARNING) << "û������ƴ����";
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

	//��ȡ�����ǵ�ַ���û���������
	accessConnect.executeSQL("select * from sys_para where para_name='thermalequipment'", res);
	if (!res.empty() && !res.begin()->second.empty())
	{
		strcpy(loginInfo.CameraAddr, res["para1"].front().c_str());
	}
	

	//��ȡ���������ݲ���ʾ
	//HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadVideoProc, (LPVOID)this, 0, NULL);
	//CloseHandle(hThread);
	alldialog_p.p_thermaldialog=p_thermaldialog;
	alldialog_p.p_splicingdialog=p_splicingdialog;
	alldialog_p.p_regiontabledialog=p_regiontabledialog;
	alldialog_p.p_curvedialog=p_curvedialog;
	//

	//��ʼ������������ͼ
	CListCtrl *m_listctrl=&p_regiontabledialog->m_listctrl; 
	// ���б���ͼ�ؼ��в����б���������б������ı� 
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
		//lvitem.pszText = "����";
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
	
	
	//������һ���߳�ThreadProc,������ȼ�THREAD_PRIORITY_IDLE����κεȼ�����Ϊ1    
	AfxBeginThread((AFX_THREADPROC)ThreadVideoProc, (LPVOID)&alldialog_p,THREAD_PRIORITY_IDLE);

	//ע��
	std::string strKey="2551b7a69845407894c8aa642189d8a1";//���룺�й������Ͼ�����ѧԺ

	auto key = VerifyMachineDialog();
	if (key == -1)
	{
		MessageBox(nullptr, "ע�����ʽ����", "��֤ʧ��", MB_OK);
		return FALSE;
	}
	else if (key == 0)
	{
		MessageBox(nullptr, "ע�������", "��֤ʧ��", MB_OK);
		return FALSE;
	}


	//typedef void (*lpCall)(char*);
	////����dll���  
	//HINSTANCE hDll;  
	////װ��dll  
	//hDll = LoadLibrary(_T("dialog.dll"));  
	//if(NULL==hDll)
	//{  
	//	AfxMessageBox(_T("DLL����ʧ�ܣ�"));  
	//}  
	////����ָ��DLL�е�����⺯����ַ  
	//lpCall Check_Register=(lpCall)GetProcAddress(hDll,"Check_Register");  
	//lpCall GetRegisterKey=(lpCall)GetProcAddress(hDll,"GetRegisterKey");  
	//if(NULL==Check_Register)  
	//{  
	//	AfxMessageBox(_T("DLL�ڲ���������ʧ�ܣ�"));  
	//}  
	////����DLL�Ի���  
	//HINSTANCE exe_hInstance = GetModuleHandle(NULL);
	//AfxSetResourceHandle(hDll); //�л�״̬
	//Check_Register((char*)strKey.c_str());
	//AfxSetResourceHandle(exe_hInstance); //�ָ�״̬
	//FreeLibrary(hDll);


	return TRUE;
}

int C��תҤApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);
	return CWinAppEx::ExitInstance();
}

// C��תҤApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void C��תҤApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// C��תҤApp �Զ������/���淽��

void C��תҤApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void C��תҤApp::LoadCustomState()
{
}

void C��תҤApp::SaveCustomState()
{
}

// C��תҤApp ��Ϣ�������



