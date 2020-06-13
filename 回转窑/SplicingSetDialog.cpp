// SplicingSetDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "SplicingSetDialog.h"
#include "afxdialogex.h"

#include "SQLConnect.hpp"


extern ATL::CImage m_image;
extern int m_imageVersion;
extern int m_imageVersion_old;
// CSplicingSetDialog �Ի���

bool ImageCopy(const CImage &srcImage, CImage &destImage);

IMPLEMENT_DYNAMIC(CSplicingSetDialog, CDialogEx)

CSplicingSetDialog::CSplicingSetDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSplicingSetDialog::IDD, pParent)
	, m_top(_T(""))
	, m_all_flag(FALSE)
{

}

CSplicingSetDialog::~CSplicingSetDialog()
{
}

void CSplicingSetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_top);
	DDX_Text(pDX, IDC_EDIT5, m_bottom);
	DDX_Control(pDX, IDC_SPIN1, m_spinleft);
	DDX_Control(pDX, IDC_SPIN2, m_spinright);
	DDX_Control(pDX, IDC_LIST1, m_listctrl);
	DDX_Check(pDX, IDC_CHECK1, m_all_flag);
}


BEGIN_MESSAGE_MAP(CSplicingSetDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSplicingSetDialog::OnBnClickedButtonSave)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CSplicingSetDialog::OnColumnclickList1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CSplicingSetDialog::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CSplicingSetDialog::OnDeltaposSpin2)
	ON_BN_CLICKED(IDC_BUTTON1, &CSplicingSetDialog::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSplicingSetDialog ��Ϣ�������


void CSplicingSetDialog::OnBnClickedButtonSave()
{
	CString sql_command;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	//select_sql="select * from sys_para where para_name='signalequipment'";
	UpdateData(TRUE);
	
	for(int i=0;i<m_listctrl.GetItemCount();i++)
	{
		sql_command.Format("update sys_para set para0=\'%s\', para1=\'%s\' where para_name='splicingregion'",m_listctrl.GetItemText(i,1),m_listctrl.GetItemText(i,2));
		accessConnect.executeSQL(sql_command.GetString());
	}
}


BOOL CSplicingSetDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��Spin control
	//int cx = m_image.GetWidth();
	int cy = m_image.GetHeight();
	m_spinleft.SetRange32(0,cy-1);
	m_spinleft.SetBase(10);
	m_spinright.SetRange32(0,cy-1);
	m_spinright.SetBase(10);
	m_top="0";m_bottom="1";
	//��ʼ��listctl
	m_listctrl.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_CHECKBOXES| LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	CRect rect;
	// ��ȡ�ؼ���λ�úʹ�С   
	m_listctrl.GetClientRect(&rect);   
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	//m_listctrl->SetExtendedStyle(m_listctrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  
	// Ϊ�б���ͼ�ؼ�������� 
	m_listctrl.DeleteAllItems();
	int nIndex = 0;
	m_listctrl.InsertColumn(nIndex++, "      (ȫѡ)", LVCFMT_CENTER, rect.Width()/4);
	m_listctrl.InsertColumn(nIndex++, "�ϱ�", LVCFMT_CENTER, rect.Width()/4);
	m_listctrl.InsertColumn(nIndex++, "�±�", LVCFMT_CENTER, rect.Width()/4);
	m_listctrl.InsertColumn(nIndex++, "����", LVCFMT_CENTER, rect.Width()/4);   
	////������
	//���HeadCtrl
	CHeaderCtrl *headerCtrl = m_listctrl.GetHeaderCtrl();
	//��ȡ��ǰ˳��
	int cnt = headerCtrl->GetItemCount();
	LPINT order = (LPINT)malloc(cnt * sizeof(int));
	headerCtrl->GetOrderArray(order, cnt);
	//����˳��
	int temp;
	temp = order[0];
	order[0] = order[3];
	order[3] = temp;
	//������˳���ͷ�
	headerCtrl->SetOrderArray(cnt, order);
	free(order);

	int index_sum;
	
	SQLResult res;
	
	if(accessConnect.executeSQL("select * from sys_para where para_name='signalequipment'", res)== S_OK) //����ѯ�ɹ�
	{
		if(res.empty() || res.begin()->second.empty()) //��ѯ���Ϊ��
		{
			AfxMessageBox("���ڻ�û������ƴ�������������ã�");
		}
		else
		{
			index_sum=stoi(res["para3"].front());	
		}

	}
	CString select_sql;
	select_sql.Format("select * from sys_para where para_name='splicingregion' and para_index<%d",index_sum);
	if (accessConnect.executeSQL(select_sql.GetString(), res) == S_OK)//����ѯ�ɹ�
	{
		if (!res.empty() && !res.begin()->second.empty())
		{
			int resnum = res.begin()->second.size();
			CString indexstr;
			for (int index = 0; index < resnum; index++)
			{
				indexstr.Format("%d", index);
				m_listctrl.InsertItem(index, "");//����һ��
				m_listctrl.SetItemText(index, 1, res["para0"][index].c_str());//�ڵ�һ�������õڶ��е�����
				m_listctrl.SetItemText(index, 2, res["para1"][index].c_str());
				m_listctrl.SetItemText(index, 3, indexstr);
			}
		}
	}
	//int index_sum=10;
	//int index;
	//UpdateData(FALSE);
	//CString indexstr;
	//for(index=0;index<index_sum;index++)
	//{
	//	indexstr.Format("%d",index);
	//	m_listctrl.InsertItem(index,"");//����һ��
	//	m_listctrl.SetItemText(index,1,"0");//�ڵ�һ�������õڶ��е�����
	//	m_listctrl.SetItemText(index,2,"0");
	//	m_listctrl.SetItemText(index,3,indexstr);
	//}	
	UpdateData(FALSE);

	SetTimer(1,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSplicingSetDialog::OnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(pNMLV->iSubItem==0)
	{
		m_all_flag=!m_all_flag;
		for (int i=0;i<m_listctrl.GetItemCount();i++)
		{
			m_listctrl.SetCheck(i,m_all_flag);
		}
	}
	UpdateData(FALSE);
	m_imageVersion++;
	Invalidate();
	*pResult = 0;
}





void CSplicingSetDialog::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CSpinButtonCtrl* pSpin;
	UpdateData(TRUE);
	//pSpin=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN2);

	if(pNMUpDown->iPos+pNMUpDown->iDelta>=atoi(m_bottom))
	{
		m_spinright.SetPos(pNMUpDown->iPos+pNMUpDown->iDelta+1);
		m_bottom.Format("%d",pNMUpDown->iPos+pNMUpDown->iDelta+1);
	}
	
	m_top.Format("%d",pNMUpDown->iPos+pNMUpDown->iDelta);
	
	for (int i=0;i<m_listctrl.GetItemCount();i++)
	{
		if(m_listctrl.GetCheck(i))
		{
			m_listctrl.SetItemText(i,1,m_top);//�ڵ�һ�������õڶ��е�����
			m_listctrl.SetItemText(i,2,m_bottom);
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CSplicingSetDialog::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CSpinButtonCtrl* pSpin;
	UpdateData(TRUE);
	//pSpin=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN_TOP);
	if(atoi(m_top)>=pNMUpDown->iPos+pNMUpDown->iDelta)
	{
		m_spinleft.SetPos(pNMUpDown->iPos+pNMUpDown->iDelta-1);
		m_top.Format("%d",pNMUpDown->iPos+pNMUpDown->iDelta-1);
	}
	m_bottom.Format("%d",pNMUpDown->iPos+pNMUpDown->iDelta);
	for (int i=0;i<m_listctrl.GetItemCount();i++)
	{
		if(m_listctrl.GetCheck(i))
		{
			m_listctrl.SetItemText(i,1,m_top);//�ڵ�һ�������õڶ��е�����
			m_listctrl.SetItemText(i,2,m_bottom);
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CSplicingSetDialog::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CImage m_image_cpy;
	//ImageCopy(m_image,m_image_cpy);
	//CRect rect,srect;
	//int cx = m_image.GetWidth();
	//int cy = m_image.GetHeight();
	//CWnd *pWnd = NULL;
	//pWnd =GetDlgItem(IDC_PIC_SPLICING_SET);//��ȡ�ؼ����??
	//pWnd->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С
	//CDC *pDc = pWnd->GetDC();//��ȡpicture��DC
	//pDc->SetStretchBltMode(COLORONCOLOR);
	//m_image.StretchBlt(pDc->m_hDC, 0,0,rect.Width(),rect.Height(),0,0,cx,cy, SRCCOPY);//������ʾ
	//pWnd->ReleaseDC(pDc);
	m_imageVersion++;
}


void CSplicingSetDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	static bool fistflag=true;
	if(fistflag)
	{
		OnBnClickedButton1();
		fistflag=false;
	}
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}
//bool ImageCopy(const CImage &srcImage, CImage &destImage)
//{
//	int i;//ѭ������
//	if (srcImage.IsNull())
//		return FALSE;
//	//Դͼ�����
//	BYTE* srcPtr = (BYTE*)srcImage.GetBits();
//	int srcBitsCount = srcImage.GetBPP();
//	int srcWidth = srcImage.GetWidth();
//	int srcHeight = srcImage.GetHeight();
//	int srcPitch = srcImage.GetPitch();
//	//����ԭ��ͼ��
//	if (!destImage.IsNull())
//	{
//		destImage.Destroy();
//	}
//	//����CImage����ͼ�񲢷����ڴ�
//	if (srcBitsCount == 32)   //֧��alphaͨ��
//	{
//		destImage.Create(srcWidth, srcHeight, srcBitsCount, 1);
//	}
//	else
//	{
//		destImage.Create(srcWidth, srcHeight, srcBitsCount, 0);
//	}
//	//���ص�ɫ��
//	if (srcBitsCount <= 8 && srcImage.IsIndexed())//��Ҫ��ɫ��
//	{
//		RGBQUAD pal[256];
//		int nColors = srcImage.GetMaxColorTableEntries();
//		if (nColors>0)
//		{
//			srcImage.GetColorTable(0, nColors, pal);
//			destImage.SetColorTable(0, nColors, pal);//���Ƶ�ɫ�����
//		}
//	}
//	//Ŀ��ͼ�����
//	BYTE *destPtr = (BYTE*)destImage.GetBits();
//	int destPitch = destImage.GetPitch();
//	//����ͼ������
//	for (i = 0; i<srcHeight; i++)
//	{
//		memcpy(destPtr + i*destPitch, srcPtr + i*srcPitch, abs(srcPitch));
//	}
//
//	return TRUE;
//}


void CSplicingSetDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	static int top=0,bottom=0;
	static int old_m_imageVersion=-1;
	if(m_imageVersion_old!=m_imageVersion) return;
	if(old_m_imageVersion!=m_imageVersion||top!=atoi(m_top)||bottom!=atoi(m_bottom))
	{
		top=atoi(m_top);bottom=atoi(m_bottom);old_m_imageVersion=m_imageVersion;
		CWnd *pWnd = NULL;
		CRect rect;
		pWnd = GetDlgItem(IDC_PIC_SPLICING_SET);//��ȡ�ؼ����??
		pWnd->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С
		CDC* pDc=pWnd->GetDC();
		pDc->SetStretchBltMode(COLORONCOLOR);
		m_image.StretchBlt(pDc->m_hDC, 0,0,rect.Width(),rect.Height(),0,0,m_image.GetWidth(),m_image.GetHeight(), SRCCOPY);//������ʾ
		CPen pen; 
		pen.CreatePen(PS_SOLID,1,RGB(0,255,255)); 
		pDc->SelectObject( &pen );
		int temp;
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

void CSplicingSetDialog::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(1);
	CDialogEx::OnClose();
}
