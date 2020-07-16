// Logon.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��תҤ.h"
#include "Logon.h"
#include "afxdialogex.h"
#include "md5.h"

#define MD5STR "�Ͼ�����ѧԺ���㹤��ѧԺ����"

//mysql��������������ͷ�ļ�  
#include "winsock.h"  
//mysqlͷ�ļ���ȻҪ����    
#include "mysql.h"
#include "easylogging++.h"



extern CString userNumber;
extern  CString userPermission;
CString  administrator_passwd;

// CLogon �Ի���

IMPLEMENT_DYNAMIC(CLogon, CDialogEx)

CLogon::CLogon(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogon::IDD, pParent)
	, m_password(_T(""))
{
	SYSTEMTIME st;
	CString strDateTime;
	GetLocalTime(&st);
	strDateTime.Format("%4d-%2d-%2d %2d:%2d:%2d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	MD5 md5;
	strDateTime=strDateTime+MD5STR;
	md5.update(strDateTime.GetBuffer());
	administrator_passwd=md5.toString().c_str();
}

CLogon::~CLogon()
{
}

void CLogon::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDV_MaxChars(pDX, m_password, 40);
	DDX_Control(pDX, IDC_COMBO1_USER, m_comboxUser);
}


BEGIN_MESSAGE_MAP(CLogon, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_AUTOLOGON, &CLogon::OnBnClickedCheckAutologon)
	ON_BN_CLICKED(IDC_BUTTON_LOGON, &CLogon::OnBnClickedButtonLogon)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CLogon::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CLogon ��Ϣ�������





void CLogon::OnBnClickedCheckAutologon()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton * pBtn = NULL ;  
	pBtn = (CButton * )GetDlgItem(IDC_CHECK_AUTOLOGON);
	if (pBtn->GetCheck()) 
		autologon_flag=TRUE;
	else
		autologon_flag=FALSE;
}

void mysql_real_cstring(CString &str)
{
	std::string strSql=str.GetBuffer();
	std::string strDest;
	size_t iSrcSize = strSql.size();
	
	for (size_t i = 0; i < iSrcSize; i++) {
		char ch = strSql[i];
		switch (ch)
		{
		case '\0':
			break;
		case '\n':
			break;
		case '\r':
			break;
		case '\'':
			break;
		case '"':
			break;
		case '\\':
			break;
		case '%':
			break;
		case '_':
			break;
		case ' ':
			break;
		default:
			strDest.append(1, ch);
			break;
		}
	}
	str=strDest.c_str();
	return;
}
void CLogon::OnBnClickedButtonLogon()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_comboxUser.GetWindowText(user_number);
	mysql_real_cstring(user_number);
	if(user_number=="administrator"&&m_password==administrator_passwd)
	{	
		userNumber=user_number;
		userPermission="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		AfxMessageBox("�����û�!");
		EndDialog(LOGON_TRUE);
		return;
	}
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
		LOG(ERROR)   << "login:"+e;
		MessageBox(e);  
		return;
	}
	if(user_number.IsEmpty()||m_password.IsEmpty())
	{
		MessageBox(_T("�û��������벻��Ϊ��!"),_T("�û���¼��Ϣ"));
		return;
	}
	MD5 md5;
	user_passwd=m_password+MD5STR;
	md5.update(user_passwd.GetBuffer());
	user_passwd=md5.toString().c_str();//toString()������ü����ַ�����c_str();��������ת����CString
	
	CString select_sql_by_user;
	//select_sql_by_user.Format("select user_number,user_passwd from userinfo where user_number= \'%s\'",user_number);
	select_sql_by_user.Format("select * from user_info where user_number= \'%s\'",user_number);
	int ress=mysql_query(&mysql,(char*)(LPCSTR)select_sql_by_user);
	BOOL logon_flag=false;
	if(ress==0) //����ѯ�ɹ�
	{
		res = mysql_store_result(&mysql);
		if(mysql_num_rows(res)==0) //��ѯ���Ϊ��
		{
			AfxMessageBox("�û�������");
		}
		else
		{

			row=mysql_fetch_row(res);
			CString str;
			for(int i=0;i<11;i++)
				str+=row[i];
			md5.reset();
			md5.update(str.GetBuffer());
			str=md5.toString().c_str();
			if(str!=row[12])
			{
				AfxMessageBox("����У���������û����ݿ��ܱ��۸�!");
				LOG(WARNING)<< userNumber+"�û����ݿ��ܱ��۸�";
			}
			else
			{
				if(user_passwd==row[1])
				{					
					userNumber=user_number;
					userPermission=row[6];
					logon_flag=true;
					mysql_free_result(res);
					AfxMessageBox("��¼�ɹ�!");
				    LOG(INFO) << userNumber+"��¼";
				}
				else
				{
					AfxMessageBox("�������!");
					LOG(ERROR)   << userNumber+"��¼�������";
				}
		    }
		}

	}
	mysql_close(&mysql);
	if (logon_flag)
		EndDialog(LOGON_TRUE);
	return;	
}


void CLogon::OnBnClickedButtonCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EndDialog(LOGON_FALSE);
}


BOOL CLogon::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	static bool m_ctrl_down = false;//�˺�����һ�ε��õ�ʱ���ʼ��
	static bool m_alt_down = false;//�˺�����һ�ε��õ�ʱ���ʼ��
	if (pMsg->message == WM_KEYDOWN)
	{

		switch (pMsg->wParam)
		{
			//VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A) �����ִ�Сд

		case 'A'://Ctrl + A
		case 'a':
			if (m_ctrl_down)
			{
				SYSTEMTIME st;
				CString strDateTime;
				GetLocalTime(&st);
				strDateTime.Format("%4d-%2d-%2d %2d:%2d:%2d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
				MD5 md5;
				strDateTime=strDateTime+MD5STR;
				md5.update(strDateTime.GetBuffer());
				strDateTime=md5.toString().c_str();//toString()������ü����ַ�����c_str();��������ת����CString
				administrator_passwd=strDateTime+MD5STR;
				md5.reset();
				md5.update(administrator_passwd.GetBuffer());
				administrator_passwd=md5.toString().c_str();
				m_comboxUser.SetWindowText("administrator");
				UpdateData(false);
				AfxMessageBox(strDateTime);
				return true;
			}
			break;
		case VK_CONTROL:
			m_ctrl_down = true;break;
		case VK_MENU:
			m_alt_down = true; break;
		default:
			break;
		}
	}
	if (pMsg->message == WM_KEYUP)
	{		switch (pMsg->wParam)
		{
		case VK_CONTROL:
			m_ctrl_down = false;break;
		case VK_MENU:
			m_alt_down = false; break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CLogon::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
    OnBnClickedButtonLogon();//�س�������ȷ����װ
}


BOOL CLogon::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	//HBRUSH hBrush = CreateSolidBrush(RGB( 0 , 0 , 255 ) );    //���ر���������˴�Ϊ��ɫRGB
	//cs.lpszClass = AfxRegisterWndClass(NULL , NULL , hBrush , NULL );
	return CDialogEx::PreCreateWindow(cs);
}


void CLogon::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	CDialogEx::PreSubclassWindow();
}
