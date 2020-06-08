//�ļ�����ImageButton.h
#pragma once
class CImageButton : public CButton
{
	DECLARE_DYNAMIC(CImageButton)
public:
	CImageButton();
	virtual ~CImageButton();
	CImageList* m_pImagelist;  //ͼ���б�ָ��
	int m_MyImageIndex; //ͼ������
	BOOL m_bPressed; //��ť�Ƿ񱻰���
	virtual void PreSubclassWindow();
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void SetImageList(CImageList* pImage);
	void SetImageIndex(UINT MyIndex);
protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint MyPoint);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint MyPoint);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint MyPoint);
	DECLARE_MESSAGE_MAP()
private:
	int          m_Style;     //��ť��״��0-������1-��ǰ��2-���£�3-������
	BOOL         b_InRect;            //�������־
	CString      m_strText;           //��ť����
	COLORREF     m_ForeColor;         //�ı���ɫ
	COLORREF     m_BackColor;         //����ɫ
	COLORREF     m_LockForeColor;     //������ť��������ɫ
	CRect        m_ButRect;           //��ť�ߴ�
	CFont*       p_Font;              //����

	void   DrawButton(CDC *pDC);      //�������İ�ť
// �ӿں���
public:
	void SetText(CString str);
	void SetForeColor(COLORREF color);       //�����ı���ɫ
	void SetBkColor(COLORREF color);         //���ñ�����ɫ
	void SetTextFont(int FontHight,LPCTSTR FontName);    //�������� 
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};


