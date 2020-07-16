// RegionTableDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "RegionTableDialog.h"
#include "afxdialogex.h"
#include "TextSpeaker.h"

extern CRegionTableDialog *p_regiontabledialog;
gds::CTextSpeaker m_TextSpeak;

extern CString userPermission;
// CRegionTableDialog 对话框

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


// CRegionTableDialog 消息处理程序


BOOL CRegionTableDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	   
	// TODO:  在此添加额外的初始化
	//设置扩展样式，其中的LVS_EX_SUBITEMIMAGES没有的话没法在非第一列添加图片
	m_listctrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	//创建ImageList，其中前3个参数分别为x，y，位图颜色
	//如果图像出来的颜色不对可以试着改下第3项
	//m_IconList.Create(120, 128, ILC_COLORDDB | ILC_MASK, 30, 30);
	m_IconList.Create(32, 32, ILC_COLORDDB | ILC_MASK, 0, 4);
	//向ImageList添加图片
	m_IconList.Add(AfxGetApp()->LoadIcon(IDI_ICON0));
	m_IconList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_IconList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_IconList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	//将ImageList设置到ListCtrl中
	m_listctrl.SetImageList(&m_IconList, LVSIL_SMALL);
	
	CRect rect;
	// 获取控件的位置和大小   
	m_listctrl.GetClientRect(&rect);   
	// 为列表视图控件添加全行选中和栅格风格   
	//m_listctrl->SetExtendedStyle(m_listctrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  
	// 为列表视图控件添加三列   
	int nIndex = 0;
	m_listctrl.InsertColumn(nIndex++, "报警", LVCFMT_CENTER, rect.Width()/4);   
	m_listctrl.InsertColumn(nIndex++, "最高温", LVCFMT_CENTER, rect.Width()/4);   
	m_listctrl.InsertColumn(nIndex++, "平均温", LVCFMT_CENTER, rect.Width()/4); 
	m_listctrl.InsertColumn(nIndex++, "区域名称", LVCFMT_CENTER, rect.Width()/4); 
	//解决第一列居中
	LVCOLUMN lvc;lvc.mask=LVCF_FMT;
	m_listctrl.GetColumn(0,&lvc);
	lvc.fmt&=~LVCFMT_JUSTIFYMASK;
	lvc.fmt|=LVCFMT_CENTER;
	m_listctrl.SetColumn(0,&lvc);
	//获得HeadCtrl
	CHeaderCtrl *headerCtrl = m_listctrl.GetHeaderCtrl();
	//获取当前顺序
	int cnt = headerCtrl->GetItemCount();
	LPINT order = (LPINT)malloc(cnt * sizeof(int));
	headerCtrl->GetOrderArray(order, cnt);
	//交换顺序
	int temp;
	temp = order[0];
	order[0] = order[3];
	order[3] = temp;
	//设置新顺序并释放
	headerCtrl->SetOrderArray(cnt, order);
	free(order);


	m_alarmsetdialog.Create(IDD_DIALOG_REGIONALARM_SET,this);
	
	SetTimer(1,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CRegionTableDialog::OnNMRClickListRegiontable(NMHDR *pNMHDR, LRESULT *pResult)
{
	//IDR_MENU_REGIONTABLE_LISTCTLRM
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 1;//0消息继续传递，1停止传递

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//添加线程操作
		VERIFY(menu.LoadMenu(IDR_MENU_REGIONTABLE_LISTCTLRM));			//这里是定义的MENU的文件名称
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		if(!alarmflag)
		    popup->CheckMenuItem(ID_32774, MF_BYCOMMAND | MF_CHECKED);//通过命令ID，选中menuSkin3
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		
		//下面的两行代码主要是为了后面的操作为准备的
		//获取列表视图控件中第一个被选择项的位置  
		POSITION m_pstion = m_listctrl.GetFirstSelectedItemPosition();
		//该函数获取由pos指定的列表项的索引，然后将pos设置为下一个位置的POSITION值
		m_alarmsetdialog.RegionIndex= m_listctrl.GetNextSelectedItem(m_pstion);
	}


}


void CRegionTableDialog::AlarmSetFUN()
{
	// TODO: 在此添加命令处理程序代码
	//CAlarmSetDialog *alarmsetdlg = new CAlarmSetDialog;
	//alarmsetdlg->Create(IDD_DIALOG_REGIONALARM_SET,this);
	//alarmsetdlg->ShowWindow(SW_SHOW)
	if(-1==userPermission.Find('C'))
	{
		AfxMessageBox("你的权限不够，请切换用户！");
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
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
					str=str+m_listctrl.GetItemText(i, 3)+",最高温度"+m_listctrl.GetItemText(i, 1)+"摄氏度，一级报警;";
					break;
				case 2:
					str=str+m_listctrl.GetItemText(i, 3)+",最高温度"+m_listctrl.GetItemText(i, 1)+"摄氏度，二级报警;";
					break;
				case 3:
					str=str+m_listctrl.GetItemText(i, 3)+",最高温度"+m_listctrl.GetItemText(i, 1)+"摄氏度，三级报警;";
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
	// TODO: 在此添加命令更新用户界面处理程序代码
	
		alarmflag=!alarmflag;
	//	pCmdUI->SetCheck(alarmflag);
		
}
