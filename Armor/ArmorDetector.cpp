#include "ArmorDetector.h"
#include <vector>
#define Color_Threshold 150
ArmorDetector::ArmorDetector()
{
    ArmorState=LIGHTS_NOT_FOUND;
}

ArmorDetector::~ArmorDetector()
{
}
void ArmorDetector::resetDetctor()
{
    ArmorState=LIGHTS_NOT_FOUND;
    Lights.clear();

}

void ArmorDetector::setEnermyColor(Color enemyColor)
{
    this->enemyColor=enemyColor;
}


void ArmorDetector::setImg(Mat srcImg)
{
    srcImg.copyTo(Img_ori);
    int MatSize=Img_ori.rows*Img_ori.cols;
    Img_bin=Mat::zeros(Img_ori.size(),CV_8UC1);
    uchar* ori_data=(uchar *)Img_ori.data;
    uchar* bin_data=(uchar *)Img_bin.data;
    if(enemyColor==Red)
    {
    for(int i=0;i<MatSize;i++)
        {
        if(*(ori_data+2)-*ori_data>Color_Threshold)
            *bin_data=255;
        ori_data+=3;
        bin_data++;
        }
    }
    else if(enemyColor==Blue)
    {
        for(int i=0;i<MatSize;i++)
        {
        if(*ori_data-*(ori_data+2)>Color_Threshold)
            *bin_data=255;
        ori_data+=3;
        bin_data++;
        }
    }
    Mat kernel =getStructuringElement(MORPH_ELLIPSE,Size(3,3));//element
    dilate(Img_bin,Img_bin,kernel); //make the lightiBar area more smooth

}

void ArmorDetector::findLights()
{
    std::vector<std::vector<Point>> LightContours;
    Mat contourImg;
    Img_bin.copyTo(contourImg);
    findContours(contourImg,LightContours,0,2);//evolution
    RotatedRect LightRect;
    LightBar light;
    for(const auto& lightContour : LightContours){
        if(lightContour.size()<6) continue;
        if(contourArea(lightContour)<minArea) continue;

        LightRect =fitEllipse(lightContour);
        light =LightBar(LightRect);

        if(abs(light.angle)>maxAngle) continue;

        Lights.emplace_back(light);
    }
    if(Lights.size()<2)
    {
        ArmorState = LIGHTS_NOT_FOUND;
        return ;
    }

    sort(Lights.begin(),Lights.end(),
         [](LightBar &a1, LightBar & a2){
        return a1.center.x<a2.center.x;});
    ArmorState=LIGHTS_FOUND;
    return ;
}

void ArmorDetector::run(Mat src)
{
    //load srcImg
    setImg(src);

    resetDetctor();

    findLights();

    if(ArmorState==LIGHTS_FOUND)
    {
        matchArmors();

        if(ArmorState==ARMOR_FOUND){
           ;//
        }
    }

}

bool ArmorDetector::DetectArmor(cv::Mat& src, cv::Point3f& target)
{
	cv::imshow("src", src);
	//target
	return 1;
}

void showArmors(Mat & image, const vector<ArmorBox>& Armors)
{
    Mat armorDisplay;
    image.copyTo(armorDisplay);

    if(!Armors.empty())
    {
        for(auto armor : Armors)
        {
            circle(armorDisplay, armor.center, 2, Scalar(0,255,0),2);
            for(size_t i=0;i<4;i++)
            {
                line(armorDisplay,armor.armorVertices[i], armor.armorVertices[(i+1)%4], Scalar(255,0,255),2,8,0);
            }
        }
    }

    imshow("Armor",armorDisplay);
}

}
