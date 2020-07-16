// ThermalSetDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "ThermalSetDialog.h"
#include "afxdialogex.h"

extern ATL::CImage m_image;
// CThermalSetDialog 对话框

IMPLEMENT_DYNAMIC(CThermalSetDialog, CDialogEx)

CThermalSetDialog::CThermalSetDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CThermalSetDialog::IDD, pParent)
{

}

CThermalSetDialog::~CThermalSetDialog()
{
}

void CThermalSetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PARAM, m_param);
	DDX_Control(pDX, IDC_BUTTON_IO, m_io);
	DDX_Control(pDX, IDC_BUTTON_REGION, m_region);
	DDX_Control(pDX, IDC_BUTTON_SPLICING, m_splicing);
}


BEGIN_MESSAGE_MAP(CThermalSetDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SPLICING, &CThermalSetDialog::OnBnClickedButtonSplicing)
	ON_BN_CLICKED(IDC_BUTTON_REGION, &CThermalSetDialog::OnBnClickedButtonRegion)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_IO, &CThermalSetDialog::OnBnClickedButtonIo)
END_MESSAGE_MAP()


// CThermalSetDialog 消息处理程序


BOOL CThermalSetDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//创建图像列表
	m_MyImageList.Create(32,32,ILC_COLOR24|ILC_MASK,0,0);
	//for(int i=0;i<5;i++)
	//{//从系统库中添加图标
	//	m_MyImageList.Add(::ExtractIcon(::AfxGetInstanceHandle(), _T("Shell32.dll"),i));
	//}	
	//向ImageList添加图片
	m_MyImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SET1));
	m_MyImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SET2));
	m_MyImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SET3));
	m_MyImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SET4));
	m_param.SetImageList(&m_MyImageList);
	m_param.SetImageIndex(0);//设置按钮显示的图像索引
	m_param.SetBkColor(RGB(176,196,222));
	m_param.SetForeColor(RGB(28,28,28));
	m_param.SetTextFont(120,_T("仿宋"));
	m_io.SetImageList(&m_MyImageList);
	m_io.SetImageIndex(1);//设置按钮显示的图像索引
	m_io.SetBkColor(RGB(176,196,222));
	m_io.SetForeColor(RGB(28,28,28));
	m_io.SetTextFont(120,_T("仿宋"));
	m_region.SetImageList(&m_MyImageList);
	m_region.SetImageIndex(2);//设置按钮显示的图像索引
	m_region.SetBkColor(RGB(176,196,222));
	m_region.SetForeColor(RGB(28,28,28));
	m_region.SetTextFont(120,_T("仿宋"));
	m_splicing.SetImageList(&m_MyImageList);
	m_splicing.SetImageIndex(3);//设置按钮显示的图像索引
	m_splicing.SetBkColor(RGB(176,196,222));
	m_splicing.SetForeColor(RGB(28,28,28));
	m_splicing.SetTextFont(120,_T("仿宋"));
	GetDlgItem(IDC_GROUP_CONFIGURE)->ShowWindow(SW_SHOW);
	m_iosetdialog.Create(IDD_SET_THERMAL_IOPARAM,GetDlgItem(IDC_GROUP_CONFIGURE));
	m_iosetdialog.ShowWindow(SW_SHOW);
	m_regionsetdialog.Create(IDD_SET_THERMAL_REGION,GetDlgItem(IDC_GROUP_CONFIGURE));
	m_regionsetdialog.ShowWindow(SW_HIDE);
	m_splicingsetdialog.Create(IDD_SET_THERMAL_SPLICING,GetDlgItem(IDC_GROUP_CONFIGURE));
	m_splicingsetdialog.ShowWindow(SW_HIDE);
	//设置页面在GroupBox中的位置
	CRect rect;
	GetDlgItem(IDC_GROUP_CONFIGURE)->GetClientRect(rect);
	rect.top+=30;
	rect.bottom-=4;
	rect.left+=10;
	rect.right-=4;
	m_iosetdialog.MoveWindow(rect);
	m_regionsetdialog.MoveWindow(rect);
	m_splicingsetdialog.MoveWindow(rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CThermalSetDialog::OnBnClickedButtonIo()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iosetdialog.ShowWindow(SW_SHOW);
	m_regionsetdialog.ShowWindow(SW_HIDE);
	m_splicingsetdialog.ShowWindow(SW_HIDE);
	SetDlgItemText(IDC_GROUP_CONFIGURE,"配置热像仪IO参数");
}

void CThermalSetDialog::OnBnClickedButtonSplicing()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iosetdialog.ShowWindow(SW_HIDE);
	m_regionsetdialog.ShowWindow(SW_HIDE);
	m_splicingsetdialog.ShowWindow(SW_SHOW);
	m_splicingsetdialog.OnInitDialog();
	m_splicingsetdialog.OnBnClickedButton1();
   SetDlgItemText(IDC_GROUP_CONFIGURE,"配置热像仪拼接参数");
}


void CThermalSetDialog::OnBnClickedButtonRegion()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iosetdialog.ShowWindow(SW_HIDE);
	m_regionsetdialog.ShowWindow(SW_SHOW);
	m_splicingsetdialog.ShowWindow(SW_HIDE);
	m_regionsetdialog.OnBnClickedButton1();
	SetDlgItemTextA(IDC_GROUP_CONFIGURE,"配置热像仪区域参数");
}


void CThermalSetDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnClose();
	DestroyWindow();
	delete this;
}


BOOL CThermalSetDialog::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	//HBRUSH hBrush = CreateSolidBrush(RGB( 0 , 0 , 255 ) );    //加载背景句柄，此处为蓝色RGB
	//cs.lpszClass = AfxRegisterWndClass(NULL , NULL , hBrush , NULL );
	return CDialogEx::PreCreateWindow(cs);
}



