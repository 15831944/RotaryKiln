
// ��תҤView.h : C��תҤView ��Ľӿ�
//

#pragma once


class C��תҤView : public CView
{
protected: // �������л�����
	C��תҤView();
	DECLARE_DYNCREATE(C��תҤView)

// ����
public:
	C��תҤDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~C��תҤView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ��תҤView.cpp �еĵ��԰汾
inline C��תҤDoc* C��תҤView::GetDocument() const
   { return reinterpret_cast<C��תҤDoc*>(m_pDocument); }
#endif

