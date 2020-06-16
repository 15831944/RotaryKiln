//文件名： ImageButton.cpp
#include "stdafx.h"
#include "ImageButton.h"
IMPLEMENT_DYNAMIC(CImageButton, CButton)
CImageButton::CImageButton()
{
	m_bPressed = FALSE;
	m_Style = 0;                //按钮形状风格
	b_InRect = false;           //鼠标进入标志
	m_strText = _T("");         //按钮文字（使用默认文字）
	m_ForeColor = RGB(0, 0, 0);             //文字颜色（黑色）
	m_BackColor = RGB(243, 243, 243);       //背景色（灰白色）
	m_LockForeColor = GetSysColor(COLOR_GRAYTEXT);     //锁定按钮的文字颜色
	p_Font = NULL;                        //字体指针
}
CImageButton::~CImageButton()
{
	if (p_Font)     delete p_Font;         //删除字体
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
	//	UINT MyState = lpDrawItemStruct->itemState; //获取状态
	//	//获取图像列中图像大小
	//	IMAGEINFO MyImageInfo;
	//	m_pImagelist->GetImageInfo(m_MyImageIndex,&MyImageInfo);
	//	CSize MyImageSize;
	//	MyImageSize.cx = MyImageInfo.rcImage.right-MyImageInfo.rcImage.left;
	//	MyImageSize.cy = MyImageInfo.rcImage.bottom - MyImageInfo.rcImage.top;
	//	//在按钮垂直方向居中显示位图
	//	CRect MyRect;
	//	GetClientRect(MyRect);
	//	CPoint MyPoint;
	//	MyPoint.x = 5;
	//	MyPoint.y = (MyRect.Height() - MyImageSize.cy)/2;
	//	m_pImagelist->Draw(&MyDC,m_MyImageIndex,MyPoint,ILD_NORMAL|ILD_TRANSPARENT);
	//	if ((MyState&ODS_SELECTED)||(MyState&ODS_FOCUS))
	//	{//按钮被选中或者获得焦点时
	//		CRect MyFocusRect(MyRect);
	//		MyFocusRect.DeflateRect(4,4,4,4);
	//		CPen pen(PS_DASHDOTDOT,1,RGB(0,0,0));
	//		CBrush brush;
	//		brush.CreateStockObject(NULL_BRUSH);
	//		MyDC.SelectObject(&brush);
	//		MyDC.SelectObject(&pen);
	//		//绘制焦点矩形
	//		MyDC.DrawFocusRect(MyFocusRect);
	//		//绘制立体效果
	//		MyDC.DrawEdge(MyRect,BDR_RAISEDINNER|BDR_RAISEDOUTER,BF_BOTTOMLEFT|BF_TOPRIGHT);
	//		//获得焦点时绘制黑色边框
	//		MyDC.Draw3dRect(MyRect,RGB(51,51,51),RGB(0,0,0));
	//	}
	//	else
	//	{//默认情况下
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
	//	{//在按钮被按下时绘制按下效果
	//		CRect MyFocusRect(MyRect);
	//		MyFocusRect.DeflateRect(4,4,4,4);
	//		MyDC.DrawFocusRect(MyFocusRect);
	//		MyDC.DrawEdge(MyRect,BDR_SUNKENINNER |BDR_SUNKENOUTER ,BF_BOTTOMLEFT|BF_TOPRIGHT);
	//		MyDC.Draw3dRect(MyRect,RGB(51,51,51),RGB(0,0,0));
	//	}
	//	//绘制按钮文本
	//	CString MyLabelText;
	//	GetWindowText(MyLabelText);
	//	MyRect.DeflateRect(MyPoint.x+MyImageSize.cx+2,0,0,0);
	//	MyDC.SetBkMode(TRANSPARENT);
	//	MyDC.DrawText(MyLabelText,MyRect,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	//}

	//字体
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	m_ButRect = lpDrawItemStruct->rcItem;     //获取按钮尺寸

	if (m_strText.IsEmpty())
		GetWindowText(m_strText);           //获取按钮文本

	int nSavedDC = pDC->SaveDC();
	VERIFY(pDC);
	DrawButton(pDC);                 //绘制按钮
	pDC->RestoreDC(nSavedDC);
}
void CImageButton::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}
void CImageButton::OnMouseMove(UINT nFlags, CPoint MyPoint)
{
	if (!b_InRect || GetCapture() != this)     //鼠标进入按钮
	{
		b_InRect = true;     //设置进入标志
		SetCapture();        //捕获鼠标
		m_Style = 1;         //设置按钮状态
		Invalidate();        //重绘按钮
	}
	else
	{
		if (!m_ButRect.PtInRect(MyPoint))     //鼠标离开按钮
		{
			b_InRect = false;    //清除进入标志
			ReleaseCapture();    //释放捕获的鼠标
			m_Style = 0;         //设置按钮状态
			Invalidate();        //重绘按钮
		}
	}

	// CButton::onMouseMove(nFlags, MyPoint);
}
void CImageButton::OnLButtonDown(UINT nFlags, CPoint MyPoint)
{
	m_Style = 2;
	Invalidate();         //重绘按钮
	CButton::OnLButtonDown(nFlags, MyPoint);
}
void CImageButton::OnLButtonUp(UINT nFlags, CPoint MyPoint)
{
	m_Style = 1;
	Invalidate();         //重绘按钮
	CButton::OnLButtonUp(nFlags, MyPoint);
}

//void CImageButton::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
//}

//void CImageButton::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
//}

void CImageButton::DrawButton(CDC* pDC)
{
	//调整状态
	if (m_Style == 3) m_Style = 0;
	if (GetStyle() & WS_DISABLED)
		m_Style = 3;     //禁止状态
	//根据状态调整边框颜色和文字颜色
	COLORREF bColor, fColor;     //bColor为边框颜色，fColor为文字颜色
	switch (m_Style)
	{
	case 0: bColor = RGB(192, 192, 192); fColor = m_ForeColor; break;   //正常按钮
	case 1: bColor = RGB(255, 255, 255); fColor = m_ForeColor; break;   //鼠标进入时按钮
	case 2: bColor = RGB(192, 192, 192); fColor = m_ForeColor; break;   //按下的按钮
	case 3: bColor = m_BackColor; fColor = m_LockForeColor; break;    //锁定的按钮
	}
	//绘制按钮背景
	CBrush Brush;

	Brush.CreateSolidBrush(m_BackColor);     //背景刷
	pDC->SelectObject(&Brush);
	CPen Pen;
	Pen.CreatePen(PS_SOLID, 1, bColor);
	pDC->SelectObject(&Pen);
	pDC->RoundRect(&m_ButRect, CPoint(5, 5));    //画圆角矩形
	//绘制按钮按下时的边框
	if (m_Style != 2)
	{
		CRect Rect;
		Rect.SetRect(m_ButRect.left + 2, m_ButRect.top + 1, m_ButRect.right, m_ButRect.bottom);
		pDC->DrawEdge(&Rect, BDR_RAISEDINNER, BF_RECT);     //画边框
	}
	//绘制图像
	CSize MyImageSize;
	CRect MyRect = m_ButRect;
	if (m_pImagelist)
	{
		IMAGEINFO MyImageInfo;
		m_pImagelist->GetImageInfo(m_MyImageIndex, &MyImageInfo);
		CSize MyImageSize;
		MyImageSize.cx = MyImageInfo.rcImage.right - MyImageInfo.rcImage.left;
		MyImageSize.cy = MyImageInfo.rcImage.bottom - MyImageInfo.rcImage.top;
		//在按钮垂直方向居中显示位图
		CPoint MyPoint;
		MyPoint.x = 5;
		MyPoint.y = (m_ButRect.Height() - MyImageSize.cy) / 2;
		m_pImagelist->Draw(pDC, m_MyImageIndex, MyPoint, ILD_NORMAL | ILD_TRANSPARENT);
		MyRect.left = MyPoint.x + MyImageSize.cx + 2;
	}
	//绘制按钮文字
	pDC->SetTextColor(fColor);         //画文字
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(m_strText, &MyRect, DT_SINGLELINE | DT_LEFT
		| DT_VCENTER | DT_END_ELLIPSIS);
	//绘制拥有焦点按钮的虚线框
	if (GetFocus() == this)
	{
		CRect Rect;
		Rect.SetRect(m_ButRect.left + 3, m_ButRect.top + 2, m_ButRect.right - 3, m_ButRect.bottom - 2);
		pDC->DrawFocusRect(&Rect);     //画拥有焦点的虚线框
	}
}

//设置按钮文本
void CImageButton::SetText(CString str)
{
	m_strText = _T("");
	SetWindowText(str);
}

//设置文本颜色
void CImageButton::SetForeColor(COLORREF color)
{
	m_ForeColor = color;
	Invalidate();
}

//设置背景颜色
void CImageButton::SetBkColor(COLORREF color)
{
	m_BackColor = color;
	Invalidate();
}

//设置字体(字体高度、字体名)
void CImageButton::SetTextFont(int FontHight, LPCTSTR FontName)
{
	if (p_Font)     delete p_Font;     //删除旧字体
	p_Font = new CFont;
	p_Font->CreatePointFont(FontHight, FontName);     //创建新字体
	SetFont(p_Font);                 //设置字体
}