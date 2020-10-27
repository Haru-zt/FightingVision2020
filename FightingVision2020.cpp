#include "FightingVision2020.h"

int FightingVision2020()
{
    FightingCapture* capture;
    //如果使用大恒相机测试
    //capture = new FightingDahengCapture();
    //如果选择使用视频来测试
    char* filename = "D:\\red-ev-0.MOV";
    capture = new FightingVideoCapture(filename);

    if (capture && !capture->init()) {
        printf("Video source initialization failed.\n");
        delete capture;
        return 1;
    }

    /* ArmorDetector */
    ArmorDetector armor_detector;
    cv::Mat src;
    cv::Point3f target;

    while (true) {
        if (capture->read(src)) {
            if (!(armor_detector.DetectArmor(src, target)))
                target = { 0, 0, -1 };
            ;
            cv::imshow("src", src);
            cv::waitKey(1);
        } else
            break;
    }
    delete capture;
    return 0;
}

int main(int argc, char* argv[])
{
    while (true) {
        FightingVision2020();
        printf("### Error, start again! ###\n\n");
        cv::waitKey(2000);
    }
    return 0;
}
