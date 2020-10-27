/*
 *  Copyright (C) 2020 赵桐
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "FightingCapture.h"
#include <opencv2/opencv.hpp>
#include "CircularQueue.h"

#include "Include/Daheng_include/GxIAPI.h"
#include "Include/Daheng_include/DxImageProc.h"

#ifdef Windows
#include <Windows.h>
#endif

class FightingDahengCapture : public FightingCapture
{
public:
    FightingDahengCapture();
    ~FightingDahengCapture();

    bool init() final;

    bool read(cv::Mat& image) final;
   
    static void GX_STDC OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);

private:
    GX_DEV_HANDLE hDevice;
    static BYTE* pFrameBuffer;

    static DX_BAYER_CONVERT_TYPE cvtype; //选择插值算法
    static DX_PIXEL_COLOR_FILTER nBayerType;  //选择图像Bayer格式
    static bool bFlip;
    static DX_RGB_CHANNEL_ORDER emChannelOrder;

    static CircularQueue<cv::Mat, 10> circular_queue;
};