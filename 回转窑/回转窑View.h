
// 回转窑View.h : C回转窑View 类的接口
//

#pragma once


class C回转窑View : public CView
{
protected: // 仅从序列化创建
	C回转窑View();
	DECLARE_DYNCREATE(C回转窑View)

// 特性
public:
	C回转窑Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~C回转窑View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // 回转窑View.cpp 中的调试版本
inline C回转窑Doc* C回转窑View::GetDocument() const
   { return reinterpret_cast<C回转窑Doc*>(m_pDocument); }
#endif

