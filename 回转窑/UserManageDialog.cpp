// UserManageDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "UserManageDialog.h"
#include "afxdialogex.h"
#include "AddModiUserDialog.h"
#include "md5.h"

#include "SQLConnect.hpp"
#include "easylogging++.h"

#define MD5STR "�Ͼ�����ѧԺ���㹤��ѧԺ����"

// UserManageDialog �Ի���
CString strusername;
extern CString userPermission;

IMPLEMENT_DYNAMIC(UserManageDialog, CDialogEx)

UserManageDialog::UserManageDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(UserManageDialog::IDD, pParent)
{
}

UserManageDialog::~UserManageDialog()
{
}

void UserManageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listctrl);
}

BEGIN_MESSAGE_MAP(UserManageDialog, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &UserManageDialog::OnRclickList1)
	ON_COMMAND(ID_USER_DROP, &UserManageDialog::OnUserDrop)
	ON_COMMAND(ID_USER_APPEND, &UserManageDialog::OnUserAppend)
	ON_COMMAND(ID_USER_MODIFY, &UserManageDialog::OnUserModify)
	ON_COMMAND(ID_USER_DISBLE, &UserManageDialog::OnUserDisable)
	ON_COMMAND(ID_USER_ENABLE, &UserManageDialog::OnUserEnable)
END_MESSAGE_MAP()

// UserManageDialog ��Ϣ�������

BOOL UserManageDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��

	m_listctrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	CRect rect;
	// ��ȡ�ؼ���λ�úʹ�С
	m_listctrl.GetClientRect(&rect);
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����
	//m_listctrl->SetExtendedStyle(m_listctrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// Ϊ�б���ͼ�ؼ��������
	int nIndex = 0;
	m_listctrl.InsertColumn(nIndex++, "����", LVCFMT_CENTER, rect.Width() / 5 - 40);
	m_listctrl.InsertColumn(nIndex++, "�û���", LVCFMT_CENTER, rect.Width() / 5 + 10);
	m_listctrl.InsertColumn(nIndex++, "����", LVCFMT_CENTER, rect.Width() / 5 + 10);
	m_listctrl.InsertColumn(nIndex++, "����", LVCFMT_CENTER, rect.Width() / 5 + 10);
	m_listctrl.InsertColumn(nIndex++, "״̬", LVCFMT_CENTER, rect.Width() / 5 + 10);

	initData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void UserManageDialog::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	CMenu menu;
	//����̲߳���
	VERIFY(menu.LoadMenu(IDR_MENU_USER));			//�����Ƕ����MENU���ļ�����
	CMenu* popup = menu.GetSubMenu(0);
	ASSERT(popup != NULL);
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	strusername = "";
	if (pNMListView->iItem != -1)
	{
		//��������д�����Ҫ��Ϊ�˺���Ĳ���Ϊ׼����
		//��ȡ�б���ͼ�ؼ��е�һ����ѡ�����λ��
		POSITION m_pstion = m_listctrl.GetFirstSelectedItemPosition();
		strusername = m_listctrl.GetItemText(m_listctrl.GetNextSelectedItem(m_pstion), 1);
		//�ú�����ȡ��posָ�����б����������Ȼ��pos����Ϊ��һ��λ�õ�POSITIONֵ
		//m_alarmsetdialog.RegionIndex= m_listctrl.GetNextSelectedItem(m_pstion);
	}
}

//ɾ���û�
void UserManageDialog::OnUserDrop()
{
	// TODO: �ڴ���������������
	if (strusername == "")
	{
		AfxMessageBox("��û��ѡ���û���");
		return;
	}
	if (-1 == userPermission.Find('F'))
	{
		AfxMessageBox("���Ȩ�޲��������л��û���");
		return;
	}
	if (2 == AfxMessageBox(_T("��ȷ��Ҫɾ���û���"), MB_OKCANCEL | MB_ICONQUESTION, 0))
		return;

	CString sql_command;
	sql_command.Format("delete  from user_info where  user_number=\'%s\'", strusername.GetString());
	if (FAILED(accessConnect.execute(sql_command.GetString())))
	{
		AfxMessageBox(("����ʧ��: " + accessConnect.getLastError()).c_str());
		return;
	}
	AfxMessageBox("�����ɹ�!");
	LOG(INFO) << "ɾ���û�:" + strusername;
	initData();
	return;
}
//�����û�
void UserManageDialog::OnUserDisable()
{
	// TODO: �ڴ���������������
	if (strusername == "")
	{
		AfxMessageBox("��û��ѡ���û���");
		return;
	}
	if (-1 == userPermission.Find('E'))
	{
		AfxMessageBox("���Ȩ�޲��������л��û���");
		return;
	}

	CString sql_command;
	sql_command.Format("update user_info set user_state=0 where  user_number=\'%s\'", strusername.GetString());
	if (FAILED(accessConnect.execute(sql_command.GetString())))
	{
		AfxMessageBox(("����ʧ��: " + accessConnect.getLastError()).c_str());
		return;
	}
	AccessResult res;
	sql_command.Format("select * from user_info where user_number= \'%s\'", strusername.GetString());
	if (FAILED(accessConnect.select(sql_command.GetString(), res)))
	{
		AfxMessageBox(("����ʧ��: " + accessConnect.getLastError()).c_str());
		return;
	}
	CString strveri = "";
	if (res.empty()) //��ѯ���Ϊ��
	{
		AfxMessageBox("�û�������");
		return;
	}
	AfxMessageBox("�����ɹ�!");
	LOG(INFO) << "�����û�:" + strusername;
	initData();
	return;
}
//�����û�
void UserManageDialog::OnUserEnable()
{
	// TODO: �ڴ���������������
	if (strusername == "")
	{
		AfxMessageBox("��û��ѡ���û���");
		return;
	}
	if (-1 == userPermission.Find('G'))
	{
		AfxMessageBox("���Ȩ�޲��������л��û���");
		return;
	}

	CString sql_command;
	sql_command.Format("update user_info set user_state=1 where  user_number=\'%s\'", strusername.GetString());
	if (FAILED(accessConnect.execute(sql_command.GetString())))
	{
		AfxMessageBox(("����ʧ��: " + accessConnect.getLastError()).c_str());
		return;
	}

	AccessResult res;
	sql_command.Format("select * from user_info where user_number= \'%s\'", strusername.GetString());
	if (FAILED(accessConnect.select(sql_command.GetString(), res)))
	{
		AfxMessageBox(("����ʧ��: " + accessConnect.getLastError()).c_str());
		return;
	}
	if (res.empty()) //��ѯ���Ϊ��
	{
		AfxMessageBox("�û�������");
		return;
	}
	AfxMessageBox("�����ɹ�!");
	LOG(INFO) << "�����û�:" + strusername;
	initData();
	return;
}

//����û�
void UserManageDialog::OnUserAppend()
{
	// TODO: �ڴ���������������
	AddModiUserDialog adduserdialog;
	adduserdialog.usernumber = "";
	adduserdialog.DoModal();
	initData();
	return;
}

//�޸��û�
void UserManageDialog::OnUserModify()
{
	// TODO: �ڴ���������������
	if (strusername == "")
	{
		AfxMessageBox("��û��ѡ���û���");
		return;
	}
	AddModiUserDialog modiuserdialog;
	modiuserdialog.usernumber = strusername;
	modiuserdialog.DoModal();
	initData();
	return;
}

void UserManageDialog::initData(void)
{
	CString select_sql;
	AccessResult res;
	select_sql = "select * from user_info";
	if (SUCCEEDED(accessConnect.select(select_sql.GetString(), res))) //����ѯ�ɹ�
	{
		if (res.empty()) //��ѯ���Ϊ��
		{
			AfxMessageBox("���ڻ�û���û����ݣ�");
		}
		else
		{
			m_listctrl.DeleteAllItems();
			for (size_t i = 0; i < res.size(); i++)
			{
				m_listctrl.InsertItem(i, std::to_string(i).c_str());//����һ��
				m_listctrl.SetItemText(i, 1, res[i]["user_number"].c_str());//�ڵ�һ�������õڶ��е�����
				m_listctrl.SetItemText(i, 2, res[i]["user_type"].c_str());
				m_listctrl.SetItemText(i, 3, res[i]["user_name"].c_str());
				if (res[i]["user_state"] == "1")
					m_listctrl.SetItemText(i, 4, "����");
				else
					m_listctrl.SetItemText(i, 4, "����");
			}
		}
	}
	UpdateData(FALSE);
}