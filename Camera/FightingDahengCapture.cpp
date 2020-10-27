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

#include "FightingDahengCapture.h"

FightingDahengCapture::FightingDahengCapture()
{
	hDevice = NULL;
}

FightingDahengCapture::~FightingDahengCapture()
{
	//发送停采命令
	GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_STOP);
	//注销采集回调
	GXUnregisterCaptureCallback(hDevice);
	GXCloseDevice(hDevice);
	GXCloseLib();
}

BYTE* FightingDahengCapture:: pFrameBuffer=(BYTE*)malloc(1280*1280*3);
CircularQueue<cv::Mat,10> FightingDahengCapture::circular_queue;

DX_BAYER_CONVERT_TYPE FightingDahengCapture::cvtype = RAW2RGB_NEIGHBOUR; //选择插值算法
DX_PIXEL_COLOR_FILTER FightingDahengCapture::nBayerType = BAYERRG;  //选择图像Bayer格式
bool FightingDahengCapture::bFlip = false;
DX_RGB_CHANNEL_ORDER FightingDahengCapture::emChannelOrder = DX_ORDER_BGR;

void GX_STDC FightingDahengCapture::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	if (pFrame->status == GX_FRAME_STATUS_SUCCESS)
	{
		VxInt32 DxStatus = DxRaw8toRGB24Ex((void*)(pFrame->pImgBuf), pFrameBuffer, 1280, 1024,cvtype, nBayerType, bFlip, emChannelOrder);
		cv::Mat matImage(cv::Size(pFrame->nWidth, pFrame->nHeight), CV_8UC3,pFrameBuffer);
		circular_queue.push(matImage.clone());
	}
	return;
}

bool FightingDahengCapture::init()
{
	GX_STATUS status;
	//在起始位置调用GXInitLib()进行初始化，申请资源
	status = GXInitLib();
	if (status != GX_STATUS_SUCCESS)
	{
		printf("Camera init fail");
		return 0;
	}
	
	uint32_t nDeviceNum = 0;
	status = GXUpdateDeviceList(&nDeviceNum, 1000);
	if (status != GX_STATUS_SUCCESS || nDeviceNum <= 0)
	{
		printf("Camera enumrate fail");
		return 0;
	}

	GX_OPEN_PARAM stOpenParam;

	//打开枚举列表中的第一台设备。
	//假设枚举到了3台可用设备，那么用户可设置stOpenParam参数的pszContent字段为1、2、3
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";

	status = GXOpenDevice(&stOpenParam, &hDevice);
	
	if (status != GX_STATUS_SUCCESS)
	{
		printf("Camera open fail");
		return 0;
	}

	//设置参数

//关闭自动白平衡
	GXSetEnum(hDevice, GX_ENUM_BALANCE_WHITE_AUTO, GX_BALANCE_WHITE_AUTO_OFF);
	//设置白平衡系数
	GXSetFloat(hDevice, GX_FLOAT_BALANCE_RATIO, 1.6211);

	//读取当前值
	int64_t nValueW = 0;
	int64_t  nValueH = 0;
	GXGetInt(hDevice, GX_INT_WIDTH, &nValueW);
	GXGetInt(hDevice, GX_INT_WIDTH, &nValueH);

	//设置采集buffer个数
	status = GXSetAcqusitionBufferNumber(hDevice, 10);

	//声明图像格式转换句柄
	//DX_IMAGE_FORMAT_CONVERT_HANDLE handle;
	//创建图像格式转换句柄
	//DxImageFormatConvertCreate(&handle);
	//GX_PIXEL_FORMAT_ENTRY emPixelFormat = GX_PIXEL_FORMAT_BGR8;
	//设置图像输出格式
	//DxImageFormatConvertSetOutputPixelFormat(handle, emPixelFormat);

	//注册图像处理回调函数
	status = GXRegisterCaptureCallback(hDevice, this, OnFrameCallbackFun);

	//发送开采命令
	status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_START);
	//---------------------
	//
	//在这个区间图像会通过OnFrameCallbackFun接口返给用户
	//
	//---------------------
	printf("camera init success\n");
	return true;
}

bool FightingDahengCapture::read(cv::Mat& image)
{
	double start = static_cast<double>(cv::getTickCount());
	while (circular_queue.empty()) {
		double time = ((double)cv::getTickCount() - start) / cv::getTickFrequency();
		if (time > 0.5)
			return false;
	}
	return circular_queue.pop(image);
}
