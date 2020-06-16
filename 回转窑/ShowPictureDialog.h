#pragma once
#include <vector>
#include "ImageOrMat.h"

// ShowPictureDialog 对话框

class ShowPictureDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ShowPictureDialog)

public:
	ShowPictureDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ShowPictureDialog();

	// 对话框数据
	enum { IDD = IDD_DIALOG_HPICTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

public:
	CString m_path;
	BOOL m_region_line;
	BOOL m_pj_line;
	BOOL m_max_value;
	BOOL m_min_value;
	std::vector<int> RegionLeft;
	std::vector<int> RegionRight;
	std::vector<CString> RegionName;
	std::vector<float> emiss;
	std::vector<Point> MinPos;
	std::vector<Point> MaxPos;
	std::vector<float> Min_local;
	std::vector<float> Max_local;
	std::vector<int> SplicingRows;
	ATL::CImage m_image;
	Mat picture_mat;
	float min_global, max_global;
	virtual BOOL OnInitDialog();
	void ShowPicture();
	afx_msg void OnBnClickedCheck1();
};
