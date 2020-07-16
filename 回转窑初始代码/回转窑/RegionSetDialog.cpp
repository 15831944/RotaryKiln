// RegionSetDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "RegionSetDialog.h"
#include "afxdialogex.h"

#define MD5STR "�Ͼ�����ѧԺ���㹤��ѧԺ����"

//mysql��������������ͷ�ļ�  
#include "winsock.h"  
//mysqlͷ�ļ���ȻҪ����    
#include "mysql.h"


extern ATL::CImage m_image;
extern int m_imageVersion;
extern int m_imageVersion_old;
bool ImageCopy(const CImage &srcImage, CImage &destImage);

// CRegionSetDialog �Ի���

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


// CRegionSetDialog ��Ϣ�������


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

	
	//��ʼ��
	
	m_listctrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	CRect rect;
	// ��ȡ�ؼ���λ�úʹ�С   
	m_listctrl.GetClientRect(&rect);   
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	//m_listctrl->SetExtendedStyle(m_listctrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  
	// Ϊ�б���ͼ�ؼ��������   
	int nIndex = 0;
	m_listctrl.InsertColumn(nIndex++, "����", LVCFMT_CENTER, rect.Width()/7-20);   
	m_listctrl.InsertColumn(nIndex++, "����", LVCFMT_CENTER, rect.Width()/7+22);   
	m_listctrl.InsertColumn(nIndex++, "���", LVCFMT_CENTER, rect.Width()/7); 
	m_listctrl.InsertColumn(nIndex++, "�ұ�", LVCFMT_CENTER, rect.Width()/7);
	m_listctrl.InsertColumn(nIndex++, "�ϱ�", LVCFMT_CENTER, rect.Width()/7);
	m_listctrl.InsertColumn(nIndex++, "�±�", LVCFMT_CENTER, rect.Width()/7);
	m_listctrl.InsertColumn(nIndex++, "������", LVCFMT_CENTER, rect.Width()/7);
	m_left=m_right=m_top=m_bottom="0";

	MYSQL mysql; //���ݿ����Ӿ��
	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_init(&mysql);
	//�������ݿ�����ʽ
	//	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//������ַ���""
	if(!mysql_real_connect(&mysql,"localhost","root","123456","mydb",3306,NULL,0))
	{//mydbΪ�������������ݿ⣬3306Ϊ�˿ںţ�root���û���,123456������
		AfxMessageBox("���ݿ�����ʧ��");
		CString e=mysql_error(&mysql);//��Ҫ����Ŀ�������ַ����޸�Ϊ��ʹ�ö��ֽ��ַ�������δ���á�  
		MessageBox(e);  
		//return;
	}
	
	CString select_sql;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	select_sql="select * from region_info where region_state=1";
	int ress=mysql_query(&mysql,(char*)(LPCSTR)select_sql);
	if(ress==0) //����ѯ�ɹ�
	{
		res = mysql_store_result(&mysql);
		int resnum=mysql_num_rows(res);
		if(resnum==0) //��ѯ���Ϊ��
		{
			AfxMessageBox("���ڻ�û���������ݣ�");
		}
		else
		{
			//while((row=mysql_fetch_row(res)))
			for(int i=0;i<resnum;i++)
			{
				row=mysql_fetch_row(res);
				m_listctrl.InsertItem(i,row[0]);//����һ��
				m_listctrl.SetItemText(i,1,row[1]);//�ڵ�һ�������õڶ��е�����
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
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CRegionSetDialog::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CImage m_image_cpy;
	//ImageCopy(m_image,m_image_cpy);
	//CRect rect,srect;
	//CWnd *pWnd = NULL;
	//pWnd =GetDlgItem(IDC_PIC_REGION_SET);//��ȡ�ؼ����??
	//pWnd->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С
	//CDC *pDc = pWnd->GetDC();//��ȡpicture��DC
	//pDc->SetStretchBltMode(COLORONCOLOR);
	//m_image.StretchBlt(pDc->m_hDC, 0,0,rect.Width(),rect.Height(),0,0,m_image.GetWidth(),m_image.GetHeight(), SRCCOPY);//������ʾ
	//pWnd->ReleaseDC(pDc);
	m_imageVersion++;
}


void CRegionSetDialog::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


void CRegionSetDialog::OnDeltaposSpinLeft(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSpinButtonCtrl* pSpin;
	UpdateData(TRUE);
	pSpin=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN_TOP);
	if(atoi(m_top)>=pNMUpDown->iPos+pNMUpDown->iDelta)
		pSpin->SetPos(pNMUpDown->iPos+pNMUpDown->iDelta-1);
	*pResult = 0;
}


void CRegionSetDialog::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index=m_listctrl.GetItemCount();
	UpdateData(TRUE);
	CString indexstr;
	indexstr.Format("%d",index);
	m_listctrl.InsertItem(index,indexstr);//����һ��
	m_listctrl.SetItemText(index,1,m_regionname);//�ڵ�һ�������õڶ��е�����
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if((pNMLV->uChanged&LVIF_STATE)&&(pNMLV->uNewState&LVIS_SELECTED)&&!(pNMLV->uOldState&LVIS_FOCUSED))
	{
		//���ȵõ������λ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	   m_listctrl.DeleteItem(m_listctrl.GetItemCount()-1);
}



void CRegionSetDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	static int left=0,right=0,top=0,bottom=0;
	static int old_m_imageVersion=-1;
	if(m_imageVersion_old!=m_imageVersion) return;
	if(old_m_imageVersion!=m_imageVersion||left!=atoi(m_left)||right!=atoi(m_right)||top!=atoi(m_top)||bottom!=atoi(m_bottom))
	{
		left=atoi(m_left);right=atoi(m_right);top=atoi(m_top);bottom=atoi(m_bottom);old_m_imageVersion=m_imageVersion;
	   CWnd *pWnd = NULL;
	   CRect rect;
	   pWnd = GetDlgItem(IDC_PIC_REGION_SET);//��ȡ�ؼ����??
	   pWnd->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С
	   CDC* pDc=pWnd->GetDC();
	   pDc->SetStretchBltMode(COLORONCOLOR);
	   m_image.StretchBlt(pDc->m_hDC, 0,0,rect.Width(),rect.Height(),0,0,m_image.GetWidth(),m_image.GetHeight(), SRCCOPY);//������ʾ
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
	// TODO: �ڴ����ר�ô����/����û���
	return;
	CDialogEx::OnOK();
}


void CRegionSetDialog::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MYSQL mysql; //���ݿ����Ӿ��
	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_init(&mysql);
	//�������ݿ�����ʽ
	//	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//������ַ���""
	if(!mysql_real_connect(&mysql,"localhost","root","123456","mydb",3306,NULL,0))
	{//mydbΪ�������������ݿ⣬3306Ϊ�˿ںţ�root���û���,123456������
		AfxMessageBox("���ݿ�����ʧ��");
		CString e=mysql_error(&mysql);//��Ҫ����Ŀ�������ַ����޸�Ϊ��ʹ�ö��ֽ��ַ�������δ���á�  
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
			AfxMessageBox("����ʧ�ܣ�");
			return;
		}
    }
	mysql_close(&mysql);
	AfxMessageBox("����ɹ���");
}
