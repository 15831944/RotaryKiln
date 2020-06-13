// AlarmSetDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "AlarmSetDialog.h"
#include "afxdialogex.h"

#include "SQLConnect.hpp"


extern int alarmSetVersion;
// CAlarmSetDialog �Ի���

IMPLEMENT_DYNAMIC(CAlarmSetDialog, CDialogEx)

CAlarmSetDialog::CAlarmSetDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAlarmSetDialog::IDD, pParent)
	, m_alarmTH1(_T(""))
	, m_alarmTH2(_T(""))
	, m_alarmTH3(_T(""))
{

}

CAlarmSetDialog::~CAlarmSetDialog()
{
}

void CAlarmSetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ALARM1, m_alarmTH1);
	DDX_Text(pDX, IDC_EDIT_ALARM2, m_alarmTH2);
	DDX_Text(pDX, IDC_EDIT_ALARM3, m_alarmTH3);
}


BEGIN_MESSAGE_MAP(CAlarmSetDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAlarmSetDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CAlarmSetDialog ��Ϣ�������


void CAlarmSetDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(1);
	v_alarmTH1[RegionIndex]=atof(m_alarmTH1);
	v_alarmTH2[RegionIndex]=atof(m_alarmTH2);
	v_alarmTH3[RegionIndex]=atof(m_alarmTH3);
	alarmSetVersion++;

	CString sql_command;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	sql_command.Format("update region_info set region_alarm1=%f,region_alarm2=%f,region_alarm3=%f where  region_state=1 and region_index=%d",v_alarmTH1[RegionIndex],v_alarmTH2[RegionIndex],v_alarmTH3[RegionIndex],RegionIndex);
	accessConnect.executeSQL(sql_command.GetString());
	CDialogEx::OnOK();
}


BOOL CAlarmSetDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
