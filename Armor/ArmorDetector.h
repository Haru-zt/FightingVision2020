#pragma once
//take exam by JLU2020
#include "opencv2/opencv.hpp"
#include<vector>
#define minArea 50
#define maxAngle 45
#define maxAngle_diff 6
#define maxLengthdiff_ratio 0.5
#define maxdeviation_angle 50
#define max_y_diff_ratio 0.5
#define max_x_diff_ratio 0.5
using namespace cv;
using namespace std;
enum State
{
    LIGHTS_NOT_FOUND,
    LIGHTS_FOUND,
    ARMOR_NOT_FOUND,
    ARMOR_FOUND
};

enum Color
{
    Red=0,
    Blue=1
};

class LightBar
{
public:
    LightBar();
    LightBar(const RotatedRect& Light);
    ~LightBar();
public:
    RotatedRect LightRect;
    float length;
    Point2f center;
    float angle;
};

class ArmorBox
{
public:
    ArmorBox();
    ArmorBox(const LightBar& l_Light,const LightBar& r_Light);
    float getAngleDiff() const;
    float getDeviatAngle() const;
    float getDislocationX() const;
    float getDislocationY() const;
    float getLengthRation() const;
    bool isSuitableArmor() const;
    void setArmorVertices(const LightBar& l_light, const LightBar& r_light);
    void mathcArmors();

public:
    LightBar l_Light,r_Light;
    int l_index,r_index;
    int armorNum;
    std::vector<Point2f> armorVertices;
    Point2f center;
    Rect armorRect;
    float armorAngle;
    Mat armorImg;
};

class ArmorDetector {
public:
    ArmorDetector();
    ~ArmorDetector();
    void setEnermyColor(Color enermyColor);
    void setImg(Mat srcImg);
    bool DetectArmor(cv::Mat& src, cv::Point3f& target);
    void resetDetctor();
    void setEnermyColor();
    void run(Mat src);
    void findLights();
    void matchArmors();

public:
    Mat Img_ori; //original img from cameral
    Mat Img_bin; //binary img
    Color enemyColor;
    State ArmorState;
    vector<LightBar> Lights;
    vector<ArmorBox> Armors;
};

void showArmors(Mat & image, const vector<ArmorBox>& Armors);
