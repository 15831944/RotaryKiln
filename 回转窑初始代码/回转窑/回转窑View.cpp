
// 回转窑View.cpp : C回转窑View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "回转窑.h"
#endif

#include "回转窑Doc.h"
#include "回转窑View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// C回转窑View

IMPLEMENT_DYNCREATE(C回转窑View, CView)

BEGIN_MESSAGE_MAP(C回转窑View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &C回转窑View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND_RANGE(IDC_TOOLBAR_DEFAULT,IDC_TOOLBAR_SAVE,NULL)
END_MESSAGE_MAP()

// C回转窑View 构造/析构

C回转窑View::C回转窑View()
{
	// TODO: 在此处添加构造代码

}

C回转窑View::~C回转窑View()
{
}

BOOL C回转窑View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// C回转窑View 绘制

void C回转窑View::OnDraw(CDC* /*pDC*/)
{
	C回转窑Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 在此处为本机数据添加绘制代码
}


// C回转窑View 打印


void C回转窑View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL C回转窑View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void C回转窑View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void C回转窑View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void C回转窑View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void C回转窑View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// C回转窑View 诊断

#ifdef _DEBUG
void C回转窑View::AssertValid() const
{
	CView::AssertValid();
}

void C回转窑View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

C回转窑Doc* C回转窑View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C回转窑Doc)));
	return (C回转窑Doc*)m_pDocument;
}
#endif //_DEBUG


// C回转窑View 消息处理程序


