#pragma once

#include "opencv2/opencv.hpp"

class ArmorDetector {
public:
    ArmorDetector();
    ~ArmorDetector();
    
    bool DetectArmor(cv::Mat& src, cv::Point3f& target);

private:
   
};
