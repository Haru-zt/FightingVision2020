#include"ArmorDetector.h"

const Point2f crossPointof(const Point2f& bl,const Point2f& tl,const Point2f& tr,const Point2f& br){
    float a1=tr.y-bl.y;
    float b1=tr.x-bl.x;
    float c1=bl.x*tr.y-tr.x*bl.y;

    float a2=br.y-tl.y;
    float b2=br.x-tl.x;
    float c2=tl.x*br.y-br.x*tl.y;

    float d=a1*b2-a2*b1;

    if(d==0.0)
        return Point2f(FLT_MAX,FLT_MAX);
    else
        return Point2f((b2*c1-b1*c2)/d,(c1*a2-c2*a1)/d);

}

void ArmorBox::setArmorVertices(const LightBar &l_light, const LightBar &r_light)
{
    Size exLSize(int(l_light.LightRect.size.width),int(l_light.LightRect.size.height*2));
    Size exRSize(int(r_light.LightRect.size.width),int(r_light.LightRect.size.height*2));
    RotatedRect exLLight(l_light.center,exLSize,armorAngle);
    RotatedRect exRLight(r_light.center,exRSize,armorAngle);

    Point2f pts_l[4];
    exLLight.points(pts_l);
    Point2f upper_l=pts_l[2];
    Point2f lower_l=pts_l[3];

    Point2f pts_r[4];
    exRLight.points(pts_r);
    Point2f upper_r=pts_r[1];
    Point2f lower_r=pts_r[0];

    armorVertices[0]=upper_l;
    armorVertices[1]=upper_r;
    armorVertices[2]=lower_r;
    armorVertices[3]=lower_l;

}

ArmorBox::ArmorBox(){
    l_index=-1;
    r_index=-1;
    l_Light=LightBar();
    r_Light=LightBar();
    armorNum=0;
    armorVertices.resize(4);
    center=Point2f();
    armorRect=Rect();
    armorImg=Mat();

}

ArmorBox::ArmorBox(const LightBar& l_light,const LightBar& r_light)
{
    this->l_Light=l_light;
    this->r_Light=r_light;

    armorNum=0;
    armorAngle=(l_light.angle+r_light.angle)/2;

    armorVertices.resize(4);
    setArmorVertices(l_light,r_light);

    center=crossPointof(armorVertices[0],armorVertices[1],armorVertices[2],armorVertices[3]);

    armorRect=boundingRect(armorVertices);


}

float ArmorBox::getAngleDiff() const{
    float angle_diff =abs(l_Light.angle-r_Light.angle);
    return angle_diff;

}

float ArmorBox::getDeviatAngle() const{
    float delta_x =r_Light.center.x-l_Light.center.x;
    float delta_y =r_Light.center.y-l_Light.center.y;
    float deviationAngle = abs(atan(delta_y/delta_x))*180/CV_PI;
    return deviationAngle;

}

float ArmorBox::getDislocationX() const{
    float meanLen = (l_Light.length+r_Light.length)/2;
    float xDiff=abs(l_Light.center.x-r_Light.center.x);
    float xDiff_ratio =xDiff/meanLen;
    return xDiff_ratio;
}

float ArmorBox::getDislocationY() const{
    float meanLen=(l_Light.center.y+r_Light.center.y)/2;
    float yDiff=abs(l_Light.center.y-r_Light.center.y);
    float yDiff_ratio=yDiff/meanLen;
    return yDiff_ratio;
}

float ArmorBox::getLengthRation() const{
    float length_diff = abs(l_Light.length-r_Light.length);
    float lengthDiffratio = length_diff/MAX(l_Light.length,r_Light.length);
    return lengthDiffratio;
}

bool ArmorBox::isSuitableArmor() const{
    return
            this->getAngleDiff()    <maxAngle_diff&&
            this->getDeviatAngle()  <maxdeviation_angle&&
            this->getDislocationX() <max_x_diff_ratio&&
            this->getDislocationY() <max_y_diff_ratio&&
            this->getLengthRation() <maxLengthdiff_ratio;
}
