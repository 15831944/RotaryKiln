// ��תҤView.cpp : C��תҤView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "��תҤ.h"
#endif

#include "��תҤDoc.h"
#include "��תҤView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// C��תҤView

IMPLEMENT_DYNCREATE(C��תҤView, CView)

BEGIN_MESSAGE_MAP(C��תҤView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &C��תҤView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND_RANGE(IDC_TOOLBAR_DEFAULT, IDC_TOOLBAR_SAVE, NULL)
END_MESSAGE_MAP()

// C��תҤView ����/����

C��תҤView::C��תҤView()
{
	// TODO: �ڴ˴���ӹ������
}

C��תҤView::~C��תҤView()
{
}

BOOL C��תҤView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// C��תҤView ����

void C��תҤView::OnDraw(CDC* /*pDC*/)
{
	C��תҤDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

// C��תҤView ��ӡ

void C��תҤView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL C��תҤView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void C��תҤView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void C��תҤView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void C��תҤView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void C��תҤView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// C��תҤView ���

#ifdef _DEBUG
void C��תҤView::AssertValid() const
{
	CView::AssertValid();
}

void C��תҤView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

C��תҤDoc* C��תҤView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C��תҤDoc)));
	return (C��תҤDoc*)m_pDocument;
}
#endif //_DEBUG

// C��תҤView ��Ϣ�������