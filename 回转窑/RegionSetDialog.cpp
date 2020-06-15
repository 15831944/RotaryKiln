// RegionSetDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "RegionSetDialog.h"
#include "afxdialogex.h"

#define MD5STR "南京工程学院计算工程学院王杰"

//mysql必须包含网络相关头文件  
#include "winsock.h"  
//mysql头文件自然要包含    
#include "mysql.h"


extern ATL::CImage m_image;
extern int m_imageVersion;
extern int m_imageVersion_old;
bool ImageCopy(const CImage &srcImage, CImage &destImage);

// CRegionSetDialog 对话框

IMPLEMENT_DYNAMIC(CRegionSetDialog, CDialogEx)

CRegionSetDialog::CRegionSetDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegionSetDialog::IDD, pParent)
	, m_left(_T(""))
	, m_right(_T(""))
	, m_top(_T(""))
	, m_bottom(_T(""))
	, m_regionname(_T(""))
	, m_emiss(_T(""))
{

}

CRegionSetDialog::~CRegionSetDialog()
{

}

void CRegionSetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_left);
	DDX_Text(pDX, IDC_EDIT2, m_right);
	DDX_Text(pDX, IDC_EDIT3, m_top);
	DDX_Text(pDX, IDC_EDIT4, m_bottom);
	DDX_Control(pDX, IDC_LIST_REGIONINFO, m_listctrl);
	DDX_Text(pDX, IDC_EDIT_REGIONNAME, m_regionname);
	DDX_Text(pDX, IDC_EDIT6, m_emiss);
}


BEGIN_MESSAGE_MAP(CRegionSetDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CRegionSetDialog::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LEFT, &CRegionSetDialog::OnDeltaposSpinLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RIGHT, &CRegionSetDialog::OnDeltaposSpinRight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TOP, &CRegionSetDialog::OnDeltaposSpinTop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BOTTOM, &CRegionSetDialog::OnDeltaposSpinBottom)
	ON_BN_CLICKED(IDC_BUTTON2, &CRegionSetDialog::OnBnClickedButton2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_REGIONINFO, &CRegionSetDialog::OnItemchangedListRegioninfo)
	ON_BN_CLICKED(IDC_BUTTON3, &CRegionSetDialog::OnBnClickedButton3)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, &CRegionSetDialog::OnBnClickedButton4)
END_MESSAGE_MAP()


// CRegionSetDialog 消息处理程序


BOOL CRegionSetDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	int cx = m_image.GetWidth();
	int cy = m_image.GetHeight();
	CSpinButtonCtrl* pSpin;
	pSpin=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN_LEFT);
	pSpin->SetRange32(0,cx-1);
	pSpin->SetBase(10);
	pSpin=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN_RIGHT);
	pSpin->SetRange32(0,cx-1);
	pSpin->SetBase(10);
	pSpin=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN_TOP);
	pSpin->SetRange32(0,cy-1);
	pSpin->SetBase(10);
	pSpin=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN_BOTTOM);
	pSpin->SetRange32(0,cy-1);
	pSpin->SetBase(10);

	
	//初始化
	
	m_listctrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	CRect rect;
	// 获取控件的位置和大小   
	m_listctrl.GetClientRect(&rect);   
	// 为列表视图控件添加全行选中和栅格风格   
	//m_listctrl->SetExtendedStyle(m_listctrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  
	// 为列表视图控件添加三列   
	int nIndex = 0;
	m_listctrl.InsertColumn(nIndex++, "索引", LVCFMT_CENTER, rect.Width()/7-20);   
	m_listctrl.InsertColumn(nIndex++, "名称", LVCFMT_CENTER, rect.Width()/7+22);   
	m_listctrl.InsertColumn(nIndex++, "左边", LVCFMT_CENTER, rect.Width()/7); 
	m_listctrl.InsertColumn(nIndex++, "右边", LVCFMT_CENTER, rect.Width()/7);
	m_listctrl.InsertColumn(nIndex++, "上边", LVCFMT_CENTER, rect.Width()/7);
	m_listctrl.InsertColumn(nIndex++, "下边", LVCFMT_CENTER, rect.Width()/7);
	m_listctrl.InsertColumn(nIndex++, "发射率", LVCFMT_CENTER, rect.Width()/7);
	m_left=m_right=m_top=m_bottom="0";

	MYSQL mysql; //数据库连接句柄
	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_init(&mysql);
	//设置数据库编码格式
	//	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//密码加字符串""
	if(!mysql_real_connect(&mysql,"localhost","root","123456","mydb",3306,NULL,0))
	{//mydb为你所创建的数据库，3306为端口号，root是用户名,123456是密码
		AfxMessageBox("数据库连接失败");
		CString e=mysql_error(&mysql);//需要将项目属性中字符集修改为“使用多字节字符集”或“未设置”  
		MessageBox(e);  
		//return;
	}
	
	CString select_sql;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	select_sql="select * from region_info where region_state=1";
	int ress=mysql_query(&mysql,(char*)(LPCSTR)select_sql);
	if(ress==0) //检测查询成功
	{
		res = mysql_store_result(&mysql);
		int resnum=mysql_num_rows(res);
		if(resnum==0) //查询结果为空
		{
			AfxMessageBox("现在还没有区域数据！");
		}
		else
		{
			//while((row=mysql_fetch_row(res)))
			for(int i=0;i<resnum;i++)
			{
				row=mysql_fetch_row(res);
				m_listctrl.InsertItem(i,row[0]);//增加一行
				m_listctrl.SetItemText(i,1,row[1]);//在第一行上设置第二列的内容
				m_listctrl.SetItemText(i,2,row[2]);
				m_listctrl.SetItemText(i,3,row[3]);
				m_listctrl.SetItemText(i,4,row[4]);
				m_listctrl.SetItemText(i,5,row[5]);
				m_listctrl.SetItemText(i,6,row[6]);
			}		
		}

	}
	mysql_close(&mysql);
	UpdateData(FALSE);
	SetTimer(1,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CRegionSetDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//CImage m_image_cpy;
	//ImageCopy(m_image,m_image_cpy);
	//CRect rect,srect;
	//CWnd *pWnd = NULL;
	//pWnd =GetDlgItem(IDC_PIC_REGION_SET);//获取控件句柄??
	//pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小
	//CDC *pDc = pWnd->GetDC();//获取picture的DC
	//pDc->SetStretchBltMode(COLORONCOLOR);
	//m_image.StretchBlt(pDc->m_hDC, 0,0,rect.Width(),rect.Height(),0,0,m_image.GetWidth(),m_image.GetHeight(), SRCCOPY);//拉伸显示
	//pWnd->ReleaseDC(pDc);
	m_imageVersion++;
}


void CRegionSetDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(1);
	CDialogEx::OnClose();
}


void CRegionSetDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	static bool fistflag=true;
	if(fistflag)
	{
		OnBnClickedButton1();
		fistflag=false;
	}
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CRegionSetDialog::OnDeltaposSpinLeft(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CSpinButtonCtrl* pSpin;
	UpdateData(TRUE);
	pSpin=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN_RIGHT);
	if(atoi(m_right)<=pNMUpDown->iPos+pNMUpDown->iDelta)
		pSpin->SetPos(pNMUpDown->iPos+pNMUpDown->iDelta+1);
	*pResult = 0;
}


void CRegionSetDialog::OnDeltaposSpinRight(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CSpinButtonCtrl* pSpin;
	UpdateData(TRUE);
	pSpin=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN_LEFT);
	if(atoi(m_left)>=pNMUpDown->iPos+pNMUpDown->iDelta)
		pSpin->SetPos(pNMUpDown->iPos+pNMUpDown->iDelta-1);
	*pResult = 0;
}


void CRegionSetDialog::OnDeltaposSpinTop(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CSpinButtonCtrl* pSpin;
	UpdateData(TRUE);
	pSpin=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN_BOTTOM);
	if(pNMUpDown->iPos+pNMUpDown->iDelta>=atoi(m_bottom))
		pSpin->SetPos(pNMUpDown->iPos+pNMUpDown->iDelta+1);
	*pResult = 0;
}


void CRegionSetDialog::OnDeltaposSpinBottom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CSpinButtonCtrl* pSpin;
	UpdateData(TRUE);
	pSpin=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN_TOP);
	if(atoi(m_top)>=pNMUpDown->iPos+pNMUpDown->iDelta)
		pSpin->SetPos(pNMUpDown->iPos+pNMUpDown->iDelta-1);
	*pResult = 0;
}


void CRegionSetDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_listctrl.GetItemCount();
	UpdateData(TRUE);
	CString indexstr;
	indexstr.Format("%d",index);
	m_listctrl.InsertItem(index,indexstr);//增加一行
	m_listctrl.SetItemText(index,1,m_regionname);//在第一行上设置第二列的内容
	m_listctrl.SetItemText(index,2,m_left);
	m_listctrl.SetItemText(index,3,m_right);
	m_listctrl.SetItemText(index,4,m_top);
	m_listctrl.SetItemText(index,5,m_bottom);
	CString stremiss="0."+m_emiss;
	m_listctrl.SetItemText(index,6,stremiss);
}


void CRegionSetDialog::OnItemchangedListRegioninfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if((pNMLV->uChanged&LVIF_STATE)&&(pNMLV->uNewState&LVIS_SELECTED)&&!(pNMLV->uOldState&LVIS_FOCUSED))
	{
		//首先得到点击的位置
		int nId;
		POSITION pos=m_listctrl.GetFirstSelectedItemPosition();
		if(pos==NULL)
		{
			return;
		}
		nId=(int)m_listctrl.GetNextSelectedItem(pos);
		m_regionname=m_listctrl.GetItemText(nId,1);
		m_left=m_listctrl.GetItemText(nId,2);
		m_right=m_listctrl.GetItemText(nId,3);
		m_top=m_listctrl.GetItemText(nId,4);
		m_bottom=m_listctrl.GetItemText(nId,5);
		UpdateData(FALSE);
	}
	*pResult = 0;
}


void CRegionSetDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	   m_listctrl.DeleteItem(m_listctrl.GetItemCount()-1);
}



void CRegionSetDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int left=0,right=0,top=0,bottom=0;
	static int old_m_imageVersion=-1;
	if(m_imageVersion_old!=m_imageVersion) return;
	if(old_m_imageVersion!=m_imageVersion||left!=atoi(m_left)||right!=atoi(m_right)||top!=atoi(m_top)||bottom!=atoi(m_bottom))
	{
		left=atoi(m_left);right=atoi(m_right);top=atoi(m_top);bottom=atoi(m_bottom);old_m_imageVersion=m_imageVersion;
	   CWnd *pWnd = NULL;
	   CRect rect;
	   pWnd = GetDlgItem(IDC_PIC_REGION_SET);//获取控件句柄??
	   pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小
	   CDC* pDc=pWnd->GetDC();
	   pDc->SetStretchBltMode(COLORONCOLOR);
	   m_image.StretchBlt(pDc->m_hDC, 0,0,rect.Width(),rect.Height(),0,0,m_image.GetWidth(),m_image.GetHeight(), SRCCOPY);//拉伸显示
	   CPen pen; 
	   pen.CreatePen(PS_SOLID,1,RGB(0,255,255)); 
	   pDc->SelectObject( &pen );
	   int temp;
	   temp=left*rect.Width()/m_image.GetWidth();
	   pDc->MoveTo(temp,0);   
	   pDc->LineTo(temp,rect.Height()-1);
	   temp=right*rect.Width()/m_image.GetWidth();
	   pDc->MoveTo(temp,0);   
	   pDc->LineTo(temp,rect.Height()-1);
	   temp=top*rect.Height()/m_image.GetHeight();
	   pDc->MoveTo(0,temp); 
	   pDc->LineTo(rect.Width(),temp);
	   temp=bottom*rect.Height()/m_image.GetHeight();
	   pDc->MoveTo(0,temp); 
	   pDc->LineTo(rect.Width(),temp);
	   pen.DeleteObject() ;
	   ReleaseDC(pDc);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CRegionSetDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	return;
	CDialogEx::OnOK();
}


void CRegionSetDialog::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	MYSQL mysql; //数据库连接句柄
	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_init(&mysql);
	//设置数据库编码格式
	//	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//密码加字符串""
	if(!mysql_real_connect(&mysql,"localhost","root","123456","mydb",3306,NULL,0))
	{//mydb为你所创建的数据库，3306为端口号，root是用户名,123456是密码
		AfxMessageBox("数据库连接失败");
		CString e=mysql_error(&mysql);//需要将项目属性中字符集修改为“使用多字节字符集”或“未设置”  
		MessageBox(e);  
		return;
	}
	
	CString sql_command;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	sql_command="update region_info set region_state=0 where region_state=1";
	int ress;
	ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
	for(int i=0;i<m_listctrl.GetItemCount();i++)
	{
		sql_command.Format("insert into region_info (region_index,region_name,region_left,region_right,region_top,region_bottom,region_emissivity)  values(%s,\'%s\',%s,%s,%s,%s,%s)",m_listctrl.GetItemText(i, 0),m_listctrl.GetItemText(i, 1),m_listctrl.GetItemText(i, 2),m_listctrl.GetItemText(i, 3),m_listctrl.GetItemText(i,4),m_listctrl.GetItemText(i,5),m_listctrl.GetItemText(i,6));
		ress=mysql_query(&mysql,(char*)(LPCSTR)sql_command);
		if(ress)
		{
			AfxMessageBox("保存失败！");
			return;
		}
    }
	mysql_close(&mysql);
	AfxMessageBox("保存成功！");
}
