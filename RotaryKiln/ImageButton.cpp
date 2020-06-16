//�ļ����� ImageButton.cpp
#include "stdafx.h"
#include "ImageButton.h"
IMPLEMENT_DYNAMIC(CImageButton, CButton)
CImageButton::CImageButton()
{
	m_bPressed = FALSE;
	m_Style = 0;                //��ť��״���
	b_InRect = false;           //�������־
	m_strText = _T("");         //��ť���֣�ʹ��Ĭ�����֣�
	m_ForeColor = RGB(0, 0, 0);             //������ɫ����ɫ��
	m_BackColor = RGB(243, 243, 243);       //����ɫ���Ұ�ɫ��
	m_LockForeColor = GetSysColor(COLOR_GRAYTEXT);     //������ť��������ɫ
	p_Font = NULL;                        //����ָ��
}
CImageButton::~CImageButton()
{
	if (p_Font)     delete p_Font;         //ɾ������
}
BEGIN_MESSAGE_MAP(CImageButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//	ON_WM_DRAWITEM()
	//ON_WM_DRAWITEM()
END_MESSAGE_MAP()
void CImageButton::SetImageIndex(UINT MyIndex)
{
	m_MyImageIndex = MyIndex;
}
void CImageButton::SetImageList(CImageList* pImage)
{
	m_pImagelist = pImage;
}
void CImageButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//CDC MyDC ;
	//MyDC.Attach(lpDrawItemStruct->hDC);
	//if (m_pImagelist)
	//{
	//	UINT MyState = lpDrawItemStruct->itemState; //��ȡ״̬
	//	//��ȡͼ������ͼ���С
	//	IMAGEINFO MyImageInfo;
	//	m_pImagelist->GetImageInfo(m_MyImageIndex,&MyImageInfo);
	//	CSize MyImageSize;
	//	MyImageSize.cx = MyImageInfo.rcImage.right-MyImageInfo.rcImage.left;
	//	MyImageSize.cy = MyImageInfo.rcImage.bottom - MyImageInfo.rcImage.top;
	//	//�ڰ�ť��ֱ���������ʾλͼ
	//	CRect MyRect;
	//	GetClientRect(MyRect);
	//	CPoint MyPoint;
	//	MyPoint.x = 5;
	//	MyPoint.y = (MyRect.Height() - MyImageSize.cy)/2;
	//	m_pImagelist->Draw(&MyDC,m_MyImageIndex,MyPoint,ILD_NORMAL|ILD_TRANSPARENT);
	//	if ((MyState&ODS_SELECTED)||(MyState&ODS_FOCUS))
	//	{//��ť��ѡ�л��߻�ý���ʱ
	//		CRect MyFocusRect(MyRect);
	//		MyFocusRect.DeflateRect(4,4,4,4);
	//		CPen pen(PS_DASHDOTDOT,1,RGB(0,0,0));
	//		CBrush brush;
	//		brush.CreateStockObject(NULL_BRUSH);
	//		MyDC.SelectObject(&brush);
	//		MyDC.SelectObject(&pen);
	//		//���ƽ������
	//		MyDC.DrawFocusRect(MyFocusRect);
	//		//��������Ч��
	//		MyDC.DrawEdge(MyRect,BDR_RAISEDINNER|BDR_RAISEDOUTER,BF_BOTTOMLEFT|BF_TOPRIGHT);
	//		//��ý���ʱ���ƺ�ɫ�߿�
	//		MyDC.Draw3dRect(MyRect,RGB(51,51,51),RGB(0,0,0));
	//	}
	//	else
	//	{//Ĭ�������
	//		CRect MyFocusRect(MyRect);
	//		MyFocusRect.DeflateRect(4,4,4,4);
	//		CPen pen(PS_DOT,1,RGB(192,192,192));
	//		CBrush brush;
	//		brush.CreateStockObject(NULL_BRUSH);
	//		MyDC.SelectObject(&brush);
	//		MyDC.SelectObject(&pen);
	//		MyDC.Rectangle(MyFocusRect);
	//		MyDC.DrawEdge(MyRect,BDR_RAISEDINNER|BDR_RAISEDOUTER,BF_BOTTOMLEFT|BF_TOPRIGHT);
	//	}
	//	if (m_bPressed)
	//	{//�ڰ�ť������ʱ���ư���Ч��
	//		CRect MyFocusRect(MyRect);
	//		MyFocusRect.DeflateRect(4,4,4,4);
	//		MyDC.DrawFocusRect(MyFocusRect);
	//		MyDC.DrawEdge(MyRect,BDR_SUNKENINNER |BDR_SUNKENOUTER ,BF_BOTTOMLEFT|BF_TOPRIGHT);
	//		MyDC.Draw3dRect(MyRect,RGB(51,51,51),RGB(0,0,0));
	//	}
	//	//���ư�ť�ı�
	//	CString MyLabelText;
	//	GetWindowText(MyLabelText);
	//	MyRect.DeflateRect(MyPoint.x+MyImageSize.cx+2,0,0,0);
	//	MyDC.SetBkMode(TRANSPARENT);
	//	MyDC.DrawText(MyLabelText,MyRect,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	//}

	//����
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	m_ButRect = lpDrawItemStruct->rcItem;     //��ȡ��ť�ߴ�

	if (m_strText.IsEmpty())
		GetWindowText(m_strText);           //��ȡ��ť�ı�

	int nSavedDC = pDC->SaveDC();
	VERIFY(pDC);
	DrawButton(pDC);                 //���ư�ť
	pDC->RestoreDC(nSavedDC);
}
void CImageButton::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}
void CImageButton::OnMouseMove(UINT nFlags, CPoint MyPoint)
{
	if (!b_InRect || GetCapture() != this)     //�����밴ť
	{
		b_InRect = true;     //���ý����־
		SetCapture();        //�������
		m_Style = 1;         //���ð�ť״̬
		Invalidate();        //�ػ水ť
	}
	else
	{
		if (!m_ButRect.PtInRect(MyPoint))     //����뿪��ť
		{
			b_InRect = false;    //��������־
			ReleaseCapture();    //�ͷŲ�������
			m_Style = 0;         //���ð�ť״̬
			Invalidate();        //�ػ水ť
		}
	}

	// CButton::onMouseMove(nFlags, MyPoint);
}
void CImageButton::OnLButtonDown(UINT nFlags, CPoint MyPoint)
{
	m_Style = 2;
	Invalidate();         //�ػ水ť
	CButton::OnLButtonDown(nFlags, MyPoint);
}
void CImageButton::OnLButtonUp(UINT nFlags, CPoint MyPoint)
{
	m_Style = 1;
	Invalidate();         //�ػ水ť
	CButton::OnLButtonUp(nFlags, MyPoint);
}

//void CImageButton::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
//}

//void CImageButton::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
//}

void CImageButton::DrawButton(CDC* pDC)
{
	//����״̬
	if (m_Style == 3) m_Style = 0;
	if (GetStyle() & WS_DISABLED)
		m_Style = 3;     //��ֹ״̬
	//����״̬�����߿���ɫ��������ɫ
	COLORREF bColor, fColor;     //bColorΪ�߿���ɫ��fColorΪ������ɫ
	switch (m_Style)
	{
	case 0: bColor = RGB(192, 192, 192); fColor = m_ForeColor; break;   //������ť
	case 1: bColor = RGB(255, 255, 255); fColor = m_ForeColor; break;   //������ʱ��ť
	case 2: bColor = RGB(192, 192, 192); fColor = m_ForeColor; break;   //���µİ�ť
	case 3: bColor = m_BackColor; fColor = m_LockForeColor; break;    //�����İ�ť
	}
	//���ư�ť����
	CBrush Brush;

	Brush.CreateSolidBrush(m_BackColor);     //����ˢ
	pDC->SelectObject(&Brush);
	CPen Pen;
	Pen.CreatePen(PS_SOLID, 1, bColor);
	pDC->SelectObject(&Pen);
	pDC->RoundRect(&m_ButRect, CPoint(5, 5));    //��Բ�Ǿ���
	//���ư�ť����ʱ�ı߿�
	if (m_Style != 2)
	{
		CRect Rect;
		Rect.SetRect(m_ButRect.left + 2, m_ButRect.top + 1, m_ButRect.right, m_ButRect.bottom);
		pDC->DrawEdge(&Rect, BDR_RAISEDINNER, BF_RECT);     //���߿�
	}
	//����ͼ��
	CSize MyImageSize;
	CRect MyRect = m_ButRect;
	if (m_pImagelist)
	{
		IMAGEINFO MyImageInfo;
		m_pImagelist->GetImageInfo(m_MyImageIndex, &MyImageInfo);
		CSize MyImageSize;
		MyImageSize.cx = MyImageInfo.rcImage.right - MyImageInfo.rcImage.left;
		MyImageSize.cy = MyImageInfo.rcImage.bottom - MyImageInfo.rcImage.top;
		//�ڰ�ť��ֱ���������ʾλͼ
		CPoint MyPoint;
		MyPoint.x = 5;
		MyPoint.y = (m_ButRect.Height() - MyImageSize.cy) / 2;
		m_pImagelist->Draw(pDC, m_MyImageIndex, MyPoint, ILD_NORMAL | ILD_TRANSPARENT);
		MyRect.left = MyPoint.x + MyImageSize.cx + 2;
	}
	//���ư�ť����
	pDC->SetTextColor(fColor);         //������
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(m_strText, &MyRect, DT_SINGLELINE | DT_LEFT
		| DT_VCENTER | DT_END_ELLIPSIS);
	//����ӵ�н��㰴ť�����߿�
	if (GetFocus() == this)
	{
		CRect Rect;
		Rect.SetRect(m_ButRect.left + 3, m_ButRect.top + 2, m_ButRect.right - 3, m_ButRect.bottom - 2);
		pDC->DrawFocusRect(&Rect);     //��ӵ�н�������߿�
	}
}

//���ð�ť�ı�
void CImageButton::SetText(CString str)
{
	m_strText = _T("");
	SetWindowText(str);
}

//�����ı���ɫ
void CImageButton::SetForeColor(COLORREF color)
{
	m_ForeColor = color;
	Invalidate();
}

//���ñ�����ɫ
void CImageButton::SetBkColor(COLORREF color)
{
	m_BackColor = color;
	Invalidate();
}

//��������(����߶ȡ�������)
void CImageButton::SetTextFont(int FontHight, LPCTSTR FontName)
{
	if (p_Font)     delete p_Font;     //ɾ��������
	p_Font = new CFont;
	p_Font->CreatePointFont(FontHight, FontName);     //����������
	SetFont(p_Font);                 //��������
}