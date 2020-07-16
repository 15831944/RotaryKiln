// RegionTableDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "RegionTableDialog.h"
#include "afxdialogex.h"
#include "TextSpeaker.h"

extern CRegionTableDialog *p_regiontabledialog;
gds::CTextSpeaker m_TextSpeak;

extern CString userPermission;
// CRegionTableDialog �Ի���

IMPLEMENT_DYNAMIC(CRegionTableDialog, CDialogEx)

CRegionTableDialog::CRegionTableDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegionTableDialog::IDD, pParent)
{
	p_regiontabledialog=this;
}

CRegionTableDialog::~CRegionTableDialog()
{
}

void CRegionTableDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_REGIONTABLE, m_listctrl);
}


BEGIN_MESSAGE_MAP(CRegionTableDialog, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_REGIONTABLE, &CRegionTableDialog::OnNMRClickListRegiontable)
	ON_COMMAND(ID_32773, &CRegionTableDialog::AlarmSetFUN)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_32774, &CRegionTableDialog::OnUpdate32774)
END_MESSAGE_MAP()


// CRegionTableDialog ��Ϣ�������


BOOL CRegionTableDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	   
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//������չ��ʽ�����е�LVS_EX_SUBITEMIMAGESû�еĻ�û���ڷǵ�һ�����ͼƬ
	m_listctrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	//����ImageList������ǰ3�������ֱ�Ϊx��y��λͼ��ɫ
	//���ͼ���������ɫ���Կ������Ÿ��µ�3��
	//m_IconList.Create(120, 128, ILC_COLORDDB | ILC_MASK, 30, 30);
	m_IconList.Create(32, 32, ILC_COLORDDB | ILC_MASK, 0, 4);
	//��ImageList���ͼƬ
	m_IconList.Add(AfxGetApp()->LoadIcon(IDI_ICON0));
	m_IconList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_IconList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_IconList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	//��ImageList���õ�ListCtrl��
	m_listctrl.SetImageList(&m_IconList, LVSIL_SMALL);
	
	CRect rect;
	// ��ȡ�ؼ���λ�úʹ�С   
	m_listctrl.GetClientRect(&rect);   
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	//m_listctrl->SetExtendedStyle(m_listctrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  
	// Ϊ�б���ͼ�ؼ��������   
	int nIndex = 0;
	m_listctrl.InsertColumn(nIndex++, "����", LVCFMT_CENTER, rect.Width()/4);   
	m_listctrl.InsertColumn(nIndex++, "�����", LVCFMT_CENTER, rect.Width()/4);   
	m_listctrl.InsertColumn(nIndex++, "ƽ����", LVCFMT_CENTER, rect.Width()/4); 
	m_listctrl.InsertColumn(nIndex++, "��������", LVCFMT_CENTER, rect.Width()/4); 
	//�����һ�о���
	LVCOLUMN lvc;lvc.mask=LVCF_FMT;
	m_listctrl.GetColumn(0,&lvc);
	lvc.fmt&=~LVCFMT_JUSTIFYMASK;
	lvc.fmt|=LVCFMT_CENTER;
	m_listctrl.SetColumn(0,&lvc);
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


	m_alarmsetdialog.Create(IDD_DIALOG_REGIONALARM_SET,this);
	
	SetTimer(1,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CRegionTableDialog::OnNMRClickListRegiontable(NMHDR *pNMHDR, LRESULT *pResult)
{
	//IDR_MENU_REGIONTABLE_LISTCTLRM
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 1;//0��Ϣ�������ݣ�1ֹͣ����

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//����̲߳���
		VERIFY(menu.LoadMenu(IDR_MENU_REGIONTABLE_LISTCTLRM));			//�����Ƕ����MENU���ļ�����
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		if(!alarmflag)
		    popup->CheckMenuItem(ID_32774, MF_BYCOMMAND | MF_CHECKED);//ͨ������ID��ѡ��menuSkin3
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		
		//��������д�����Ҫ��Ϊ�˺���Ĳ���Ϊ׼����
		//��ȡ�б���ͼ�ؼ��е�һ����ѡ�����λ��  
		POSITION m_pstion = m_listctrl.GetFirstSelectedItemPosition();
		//�ú�����ȡ��posָ�����б����������Ȼ��pos����Ϊ��һ��λ�õ�POSITIONֵ
		m_alarmsetdialog.RegionIndex= m_listctrl.GetNextSelectedItem(m_pstion);
	}


}


void CRegionTableDialog::AlarmSetFUN()
{
	// TODO: �ڴ���������������
	//CAlarmSetDialog *alarmsetdlg = new CAlarmSetDialog;
	//alarmsetdlg->Create(IDD_DIALOG_REGIONALARM_SET,this);
	//alarmsetdlg->ShowWindow(SW_SHOW)
	if(-1==userPermission.Find('C'))
	{
		AfxMessageBox("���Ȩ�޲��������л��û���");
		return;
	}
	CString str;
	str.Format("%0.1f",m_alarmsetdialog.v_alarmTH1[m_alarmsetdialog.RegionIndex]);
	m_alarmsetdialog.m_alarmTH1=str;
	str.Format("%0.1f",m_alarmsetdialog.v_alarmTH2[m_alarmsetdialog.RegionIndex]);
	m_alarmsetdialog.m_alarmTH2=str;
	str.Format("%0.1f",m_alarmsetdialog.v_alarmTH3[m_alarmsetdialog.RegionIndex]);
	m_alarmsetdialog.m_alarmTH3=str;
	//m_alarmsetdialog.Create(IDD_DIALOG_REGIONALARM_SET,this);
	m_alarmsetdialog.UpdateData(0);
	m_alarmsetdialog.ShowWindow(SW_SHOW);
}


void CRegionTableDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	if(1==nIDEvent)
	{	
		if(alarmflag&&m_TextSpeak.GetFreeFlag())
		{	
			int itemNum=m_listctrl.GetItemCount();
			CString str="";
			for (int i=0; i<itemNum;i++)
			{
				LV_ITEM lvitem1 = {0};;
				lvitem1.iItem = i;
				lvitem1.iSubItem = 0;
				lvitem1.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
				m_listctrl.GetItem(&lvitem1);
				switch(lvitem1.iImage)
				{
				case 0:
					break;
				case 1:
					str=str+m_listctrl.GetItemText(i, 3)+",����¶�"+m_listctrl.GetItemText(i, 1)+"���϶ȣ�һ������;";
					break;
				case 2:
					str=str+m_listctrl.GetItemText(i, 3)+",����¶�"+m_listctrl.GetItemText(i, 1)+"���϶ȣ���������;";
					break;
				case 3:
					str=str+m_listctrl.GetItemText(i, 3)+",����¶�"+m_listctrl.GetItemText(i, 1)+"���϶ȣ���������;";
					break;

				}
			}
			if(!str.IsEmpty())
			   m_TextSpeak.Speak(str);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CRegionTableDialog::OnUpdate32774(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	
		alarmflag=!alarmflag;
	//	pCmdUI->SetCheck(alarmflag);
		
}
