#include"ArmorDetector.h"

LightBar::LightBar(const RotatedRect &LightRect){
    this->LightRect=LightRect;
    length=MAX(LightRect.size.height,LightRect.size.width);
    center=LightRect.center;
    if(LightRect.angle>90)
        angle=LightRect.angle-180;
    else
        angle=LightRect.angle;
}

LightBar::LightBar(){
    LightRect=RotatedRect();
    length=0;
    angle=0;
    center=Point2f();
}
