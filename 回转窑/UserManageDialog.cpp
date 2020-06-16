// UserManageDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "UserManageDialog.h"
#include "afxdialogex.h"
#include "AddModiUserDialog.h"
#include "md5.h"

#include "SQLConnect.hpp"
#include "easylogging++.h"

#define MD5STR "南京工程学院计算工程学院王杰"

// UserManageDialog 对话框
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

// UserManageDialog 消息处理程序

BOOL UserManageDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	//初始化

	m_listctrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	CRect rect;
	// 获取控件的位置和大小
	m_listctrl.GetClientRect(&rect);
	// 为列表视图控件添加全行选中和栅格风格
	//m_listctrl->SetExtendedStyle(m_listctrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 为列表视图控件添加三列
	int nIndex = 0;
	m_listctrl.InsertColumn(nIndex++, "索引", LVCFMT_CENTER, rect.Width() / 5 - 40);
	m_listctrl.InsertColumn(nIndex++, "用户名", LVCFMT_CENTER, rect.Width() / 5 + 10);
	m_listctrl.InsertColumn(nIndex++, "类型", LVCFMT_CENTER, rect.Width() / 5 + 10);
	m_listctrl.InsertColumn(nIndex++, "姓名", LVCFMT_CENTER, rect.Width() / 5 + 10);
	m_listctrl.InsertColumn(nIndex++, "状态", LVCFMT_CENTER, rect.Width() / 5 + 10);

	initData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void UserManageDialog::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	CMenu menu;
	//添加线程操作
	VERIFY(menu.LoadMenu(IDR_MENU_USER));			//这里是定义的MENU的文件名称
	CMenu* popup = menu.GetSubMenu(0);
	ASSERT(popup != NULL);
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	strusername = "";
	if (pNMListView->iItem != -1)
	{
		//下面的两行代码主要是为了后面的操作为准备的
		//获取列表视图控件中第一个被选择项的位置
		POSITION m_pstion = m_listctrl.GetFirstSelectedItemPosition();
		strusername = m_listctrl.GetItemText(m_listctrl.GetNextSelectedItem(m_pstion), 1);
		//该函数获取由pos指定的列表项的索引，然后将pos设置为下一个位置的POSITION值
		//m_alarmsetdialog.RegionIndex= m_listctrl.GetNextSelectedItem(m_pstion);
	}
}

//删除用户
void UserManageDialog::OnUserDrop()
{
	// TODO: 在此添加命令处理程序代码
	if (strusername == "")
	{
		AfxMessageBox("你没有选择用户！");
		return;
	}
	if (-1 == userPermission.Find('F'))
	{
		AfxMessageBox("你的权限不够，请切换用户！");
		return;
	}
	if (2 == AfxMessageBox(_T("您确定要删除用户吗？"), MB_OKCANCEL | MB_ICONQUESTION, 0))
		return;

	CString sql_command;
	sql_command.Format("delete  from user_info where  user_number=\'%s\'", strusername.GetString());
	if (FAILED(accessConnect.execute(sql_command.GetString())))
	{
		AfxMessageBox(("操作失败: " + accessConnect.getLastError()).c_str());
		return;
	}
	AfxMessageBox("操作成功!");
	LOG(INFO) << "删除用户:" + strusername;
	initData();
	return;
}
//禁用用户
void UserManageDialog::OnUserDisable()
{
	// TODO: 在此添加命令处理程序代码
	if (strusername == "")
	{
		AfxMessageBox("你没有选择用户！");
		return;
	}
	if (-1 == userPermission.Find('E'))
	{
		AfxMessageBox("你的权限不够，请切换用户！");
		return;
	}

	CString sql_command;
	sql_command.Format("update user_info set user_state=0 where  user_number=\'%s\'", strusername.GetString());
	if (FAILED(accessConnect.execute(sql_command.GetString())))
	{
		AfxMessageBox(("操作失败: " + accessConnect.getLastError()).c_str());
		return;
	}
	AccessResult res;
	sql_command.Format("select * from user_info where user_number= \'%s\'", strusername.GetString());
	if (FAILED(accessConnect.select(sql_command.GetString(), res)))
	{
		AfxMessageBox(("操作失败: " + accessConnect.getLastError()).c_str());
		return;
	}
	CString strveri = "";
	if (res.empty()) //查询结果为空
	{
		AfxMessageBox("用户不存在");
		return;
	}
	AfxMessageBox("操作成功!");
	LOG(INFO) << "禁用用户:" + strusername;
	initData();
	return;
}
//激活用户
void UserManageDialog::OnUserEnable()
{
	// TODO: 在此添加命令处理程序代码
	if (strusername == "")
	{
		AfxMessageBox("你没有选择用户！");
		return;
	}
	if (-1 == userPermission.Find('G'))
	{
		AfxMessageBox("你的权限不够，请切换用户！");
		return;
	}

	CString sql_command;
	sql_command.Format("update user_info set user_state=1 where  user_number=\'%s\'", strusername.GetString());
	if (FAILED(accessConnect.execute(sql_command.GetString())))
	{
		AfxMessageBox(("操作失败: " + accessConnect.getLastError()).c_str());
		return;
	}

	AccessResult res;
	sql_command.Format("select * from user_info where user_number= \'%s\'", strusername.GetString());
	if (FAILED(accessConnect.select(sql_command.GetString(), res)))
	{
		AfxMessageBox(("操作失败: " + accessConnect.getLastError()).c_str());
		return;
	}
	if (res.empty()) //查询结果为空
	{
		AfxMessageBox("用户不存在");
		return;
	}
	AfxMessageBox("操作成功!");
	LOG(INFO) << "激活用户:" + strusername;
	initData();
	return;
}

//添加用户
void UserManageDialog::OnUserAppend()
{
	// TODO: 在此添加命令处理程序代码
	AddModiUserDialog adduserdialog;
	adduserdialog.usernumber = "";
	adduserdialog.DoModal();
	initData();
	return;
}

//修改用户
void UserManageDialog::OnUserModify()
{
	// TODO: 在此添加命令处理程序代码
	if (strusername == "")
	{
		AfxMessageBox("你没有选择用户！");
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
	if (SUCCEEDED(accessConnect.select(select_sql.GetString(), res))) //检测查询成功
	{
		if (res.empty()) //查询结果为空
		{
			AfxMessageBox("现在还没有用户数据！");
		}
		else
		{
			m_listctrl.DeleteAllItems();
			for (size_t i = 0; i < res.size(); i++)
			{
				m_listctrl.InsertItem(i, std::to_string(i).c_str());//增加一行
				m_listctrl.SetItemText(i, 1, res[i]["user_number"].c_str());//在第一行上设置第二列的内容
				m_listctrl.SetItemText(i, 2, res[i]["user_type"].c_str());
				m_listctrl.SetItemText(i, 3, res[i]["user_name"].c_str());
				if (res[i]["user_state"] == "1")
					m_listctrl.SetItemText(i, 4, "正常");
				else
					m_listctrl.SetItemText(i, 4, "禁用");
			}
		}
	}
	UpdateData(FALSE);
}