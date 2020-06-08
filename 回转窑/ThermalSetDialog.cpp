// ThermalSetDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "ThermalSetDialog.h"
#include "afxdialogex.h"

extern ATL::CImage m_image;
// CThermalSetDialog �Ի���

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


// CThermalSetDialog ��Ϣ�������


BOOL CThermalSetDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//����ͼ���б�
	m_MyImageList.Create(32,32,ILC_COLOR24|ILC_MASK,0,0);
	//for(int i=0;i<5;i++)
	//{//��ϵͳ�������ͼ��
	//	m_MyImageList.Add(::ExtractIcon(::AfxGetInstanceHandle(), _T("Shell32.dll"),i));
	//}	
	//��ImageList���ͼƬ
	m_MyImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SET1));
	m_MyImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SET2));
	m_MyImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SET3));
	m_MyImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SET4));
	m_param.SetImageList(&m_MyImageList);
	m_param.SetImageIndex(0);//���ð�ť��ʾ��ͼ������
	m_param.SetBkColor(RGB(176,196,222));
	m_param.SetForeColor(RGB(28,28,28));
	m_param.SetTextFont(120,_T("����"));
	m_io.SetImageList(&m_MyImageList);
	m_io.SetImageIndex(1);//���ð�ť��ʾ��ͼ������
	m_io.SetBkColor(RGB(176,196,222));
	m_io.SetForeColor(RGB(28,28,28));
	m_io.SetTextFont(120,_T("����"));
	m_region.SetImageList(&m_MyImageList);
	m_region.SetImageIndex(2);//���ð�ť��ʾ��ͼ������
	m_region.SetBkColor(RGB(176,196,222));
	m_region.SetForeColor(RGB(28,28,28));
	m_region.SetTextFont(120,_T("����"));
	m_splicing.SetImageList(&m_MyImageList);
	m_splicing.SetImageIndex(3);//���ð�ť��ʾ��ͼ������
	m_splicing.SetBkColor(RGB(176,196,222));
	m_splicing.SetForeColor(RGB(28,28,28));
	m_splicing.SetTextFont(120,_T("����"));
	GetDlgItem(IDC_GROUP_CONFIGURE)->ShowWindow(SW_SHOW);
	m_iosetdialog.Create(IDD_SET_THERMAL_IOPARAM,GetDlgItem(IDC_GROUP_CONFIGURE));
	m_iosetdialog.ShowWindow(SW_SHOW);
	m_regionsetdialog.Create(IDD_SET_THERMAL_REGION,GetDlgItem(IDC_GROUP_CONFIGURE));
	m_regionsetdialog.ShowWindow(SW_HIDE);
	m_splicingsetdialog.Create(IDD_SET_THERMAL_SPLICING,GetDlgItem(IDC_GROUP_CONFIGURE));
	m_splicingsetdialog.ShowWindow(SW_HIDE);
	//����ҳ����GroupBox�е�λ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CThermalSetDialog::OnBnClickedButtonIo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_iosetdialog.ShowWindow(SW_SHOW);
	m_regionsetdialog.ShowWindow(SW_HIDE);
	m_splicingsetdialog.ShowWindow(SW_HIDE);
	SetDlgItemText(IDC_GROUP_CONFIGURE,"����������IO����");
}

void CThermalSetDialog::OnBnClickedButtonSplicing()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_iosetdialog.ShowWindow(SW_HIDE);
	m_regionsetdialog.ShowWindow(SW_HIDE);
	m_splicingsetdialog.ShowWindow(SW_SHOW);
	m_splicingsetdialog.OnInitDialog();
	m_splicingsetdialog.OnBnClickedButton1();
   SetDlgItemText(IDC_GROUP_CONFIGURE,"����������ƴ�Ӳ���");
}


void CThermalSetDialog::OnBnClickedButtonRegion()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_iosetdialog.ShowWindow(SW_HIDE);
	m_regionsetdialog.ShowWindow(SW_SHOW);
	m_splicingsetdialog.ShowWindow(SW_HIDE);
	m_regionsetdialog.OnBnClickedButton1();
	SetDlgItemTextA(IDC_GROUP_CONFIGURE,"�����������������");
}


void CThermalSetDialog::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnClose();
	DestroyWindow();
	delete this;
}


BOOL CThermalSetDialog::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	//HBRUSH hBrush = CreateSolidBrush(RGB( 0 , 0 , 255 ) );    //���ر���������˴�Ϊ��ɫRGB
	//cs.lpszClass = AfxRegisterWndClass(NULL , NULL , hBrush , NULL );
	return CDialogEx::PreCreateWindow(cs);
}



