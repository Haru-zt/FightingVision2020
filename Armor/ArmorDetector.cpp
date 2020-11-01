#include "ArmorDetector.h"

ArmorDetector::ArmorDetector()
{
  
}

ArmorDetector::~ArmorDetector()
{
}

void ArmorDetector::ImgTransfm_Bin()
{
    int MatSize=Img_ori.rows*Img_ori.cols;
    Img_bin=Mat::zeros(Img_ori.size(),CV_8UC1);
    uchar* ori_data=(uchar *)Img_ori.data;
    uchar* bin_data=(uchar *)Img_bin.data;
    if(enermyColor==Red)
    {
    for(int i=0;i<MatSize;i++)
    {
        if(*(ori_data+2)-*ori_data>Color_Threshold)
            *bin_data=255;
        ori_data+=3;
        bin_data++;
    }
    }
    else if(enermyColor==Blue)
    {
        for(int i=0;i<MatSize;i++)
    {
        if(*ori_data-*(ori_data+2)>Color_Threshold)
            *bin_data=255;
        ori_data+=3;
        bin_data++;
    }
    }
}

bool ArmorDetector::DetectArmor(cv::Mat& src, cv::Point3f& target)
{
	cv::imshow("src", src);
	//target
	return 1;
}
