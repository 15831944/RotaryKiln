#pragma once
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include "atlimage.h"
using namespace cv;
using namespace std;

class ImageOrMat
{
public:
	ImageOrMat();
	~ImageOrMat();
public:
	///*MatToCImage 
	// *简介： 
	// *   OpenCV的Mat转ATL/MFC的CImage，仅支持单通道灰度或三通道彩色 
	// *参数： 
	// *   mat：OpenCV的Mat 
	// *   cimage：ATL/MFC的CImage 
	//*/  
	void MatToCImage(Mat& mat, ATL::CImage& cimage);

	///*CImageToMat 
	//*简介： 
	//*   ATL/MFC的CImage转OpenCV的Mat，仅支持单通道灰度或三通道彩色 
	//*参数： 
	//*   cimage：ATL/MFC的CImage 
	//*   mat：OpenCV的Mat 
	//*/  
	void CImageToMat(ATL::CImage& cimage, Mat& mat);
};
