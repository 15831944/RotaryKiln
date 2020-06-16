// HDataDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "回转窑.h"
#include "HDataDialog.h"
#include "afxdialogex.h"

#include "SQLConnect.hpp"
#include "HistoryCurve.h"

// HDataDialog 对话框

IMPLEMENT_DYNAMIC(HDataDialog, CDialogEx)

HDataDialog::HDataDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(HDataDialog::IDD, pParent)
	, m_startime(COleDateTime::GetCurrentTime())
	, m_endtime(COleDateTime::GetCurrentTime())
{
}

HDataDialog::~HDataDialog()
{
}

void HDataDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listctrl);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_startime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_endtime);
}

BEGIN_MESSAGE_MAP(HDataDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &HDataDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &HDataDialog::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON2, &HDataDialog::OnBnClickedButton2)
END_MESSAGE_MAP()

// HDataDialog 消息处理程序

BOOL HDataDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listctrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	CRect rect;
	// 获取控件的位置和大小
	m_listctrl.GetClientRect(&rect);
	// 为列表视图控件添加全行选中和栅格风格
	//m_listctrl->SetExtendedStyle(m_listctrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 为列表视图控件添加三列
	int nIndex = 0;
	m_listctrl.InsertColumn(nIndex++, "索引", LVCFMT_CENTER, 80);
	m_listctrl.InsertColumn(nIndex++, "时间", LVCFMT_CENTER, 150);

	AccessResult res;
	if (SUCCEEDED(accessConnect.select("select * from region_info where region_state=1 order by region_index", res))) //检测查询成功
	{
		if (res.empty()) //查询结果为空
		{
			AfxMessageBox("现在还没有区域数据！");
		}
		else
		{
			list_title = "索引\t时间";
			for (size_t i = 0; i < res.size(); i++)
			{
				m_listctrl.InsertColumn(nIndex++, res[i]["region_name"].c_str(), LVCFMT_CENTER, 100);
				region_name.push_back(res[i]["region_name"].c_str());
				list_title += "\t";
				list_title += res[i]["region_name"].c_str();
			}
			list_title += "\n";
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void HDataDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int line_index = -1, list_line_index = -1;

	m_listctrl.SetRedraw(FALSE);
	//m_listctrl.SetExtendedStyle(m_listctrl.GetExtendedStyle()|LVS_EX_DOUBLEBUFFER);
	//m_listctrl.LockWindowUpdate();
	CString select_sql;
	select_sql.Format("select * from region_temperature where line_time>=\'%s\' and line_time<=\'%s\' order by line_index", m_startime.Format("%Y-%m-%d"), m_endtime.Format("%Y-%m-%d"));
	//select_sql.Format("execute * from region_temperature where line_time>=\'%s\' and line_time<=\'%s\'",m_startime.Format("%Y-%m-%d"),m_endtime.Format("%Y-%m-%d"));

	AccessResult res;
	if (SUCCEEDED(accessConnect.select(select_sql.GetString(), res))) //检测查询成功
	{
		if (res.empty()) //查询结果为空
		{
			AfxMessageBox("现在还没有历史数据！");
		}
		else
		{
			m_listctrl.DeleteAllItems();
			for (auto& record : res)
			{
				if (line_index != stoi(record["line_index"]))
				{
					line_index = stoi(record["line_index"]);
					m_listctrl.InsertItem(list_line_index, std::to_string(++list_line_index).c_str());//增加一行
					m_listctrl.SetItemText(list_line_index, 1, record["line_time"].c_str());//在第一行上设置第二列的内容
					m_listctrl.SetItemText(list_line_index, stoi(record["region_index"]) + 2, record["region_temp"].c_str());//在第一行上设置第二列的内容
				}
				else
				{
					m_listctrl.SetItemText(list_line_index, stoi(record["region_index"]) + 2, record["region_temp"].c_str());//在第一行上设置第二列的内容
				}
			}
		}
	}
	m_listctrl.SetRedraw(TRUE);
	m_listctrl.Update(0); // 更新索引为0的行
	//m_listctrl.UnlockWindowUpdate();
}

void HDataDialog::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//导出数据
	CTime time = CTime::GetCurrentTime();
	CString strRptTime = time.Format("%Y%m%d_%H%M%S");

	CFileDialog dlg(FALSE, ".xls", strRptTime, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, "(*.xls)|*.xls||", this); dlg.m_ofn.lpstrTitle = _T("导出文件");
	if (dlg.DoModal() == IDOK)
	{
		// 实现存盘
		CString buff;
		CString fileName = dlg.GetPathName();
		CFile file(fileName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive);
		//file.Write("操作\t表号\t读取类型\t图片\n",24);
		file.Write(list_title, list_title.GetLength());
		//CString ss=m_listctrl.GetItemText(-1,0);
		int rows = m_listctrl.GetItemCount();//行数
		int cols = m_listctrl.GetHeaderCtrl()->GetItemCount();//列数
		for (int i = 0; i < rows; i++)
		{
			buff = m_listctrl.GetItemText(i, 0);
			for (int j = 1; j < cols; j++)
			{
				buff = buff + "\t" + m_listctrl.GetItemText(i, j);
			}
			buff = buff + "\n";
			file.Write(buff, buff.GetLength());
		}

		file.Close();
		AfxMessageBox("保存成功！");
	}
}

void HDataDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());//如果不添加，DoModal会返回-1，并且无法显示界面
	//CDlgTemplate dlgTemplate();
	HistoryCurve myhistoryvurve;
	std::vector<CString> Time_x;
	std::vector<std::vector<double>> curve_y;
	int rows = m_listctrl.GetItemCount();//行数
	int cols = m_listctrl.GetHeaderCtrl()->GetItemCount();//列数
	if (rows < 2)
	{
		AfxMessageBox("历史数据太少！");
		return;
	}
	for (int i = 0; i < rows; i++)
	{
		Time_x.push_back(m_listctrl.GetItemText(i, 1));
	}
	curve_y.resize(cols - 2);
	for (int j = 2; j < cols; j++)
	{
		for (int i = 0; i < rows; i++)
		{
			curve_y[j - 2].push_back(atof(m_listctrl.GetItemText(i, j)));
		}
	}
	myhistoryvurve.InitData(&Time_x, &curve_y, &region_name);
	INT_PTR nResult = myhistoryvurve.DoModal();

	//INT_PTR nResult = dlgTemplate.DoModal();
	return;
}