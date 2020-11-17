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

BYTE* FightingDahengCapture:: pFrameBuffer=(BYTE*)malloc(640*480*3);//待修改
CircularQueue<cv::Mat,10> FightingDahengCapture::circular_queue;
DX_BAYER_CONVERT_TYPE FightingDahengCapture::cvtype = RAW2RGB_NEIGHBOUR; //选择插值算法
DX_PIXEL_COLOR_FILTER FightingDahengCapture::nBayerType = BAYERRG;  //选择图像Bayer格式
bool FightingDahengCapture::bFlip = false;
DX_RGB_CHANNEL_ORDER FightingDahengCapture::emChannelOrder = DX_ORDER_BGR;

void GX_STDC FightingDahengCapture::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	if (pFrame->status == GX_FRAME_STATUS_SUCCESS)
	{
		VxInt32 DxStatus = DxRaw8toRGB24Ex((void*)(pFrame->pImgBuf), pFrameBuffer, 640, 480,cvtype, nBayerType, bFlip, emChannelOrder);
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
		printf("Camera init fail\n");
		return 0;
	}
	
	uint32_t nDeviceNum = 0;
	status = GXUpdateDeviceList(&nDeviceNum, 1000);
	if (status != GX_STATUS_SUCCESS || nDeviceNum <= 0)
	{
		printf("Camera enumrate fail\n");
		return 0;
	}

	GX_DEVICE_BASE_INFO* pBaseinfo = new GX_DEVICE_BASE_INFO[nDeviceNum];

	size_t nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);
	//获取所有设备的基础信息
	GXGetAllDeviceBaseInfo(pBaseinfo, &nSize);
	
	//fx,fy参数加载（待修改）
	if (strcmp((pBaseinfo->szUserID), "hero") == 0)
	{
		double fx = 1279.07754353772;
		double fy = 1279.73275299414;
	}
	else if (strcmp((pBaseinfo->szUserID), "sentry1") == 0)
	{
		double fx = 1279.07754353772;
		double fy = 1279.73275299414;
	}
	else if (strcmp((pBaseinfo->szUserID), "sentry2") == 0)
	{
		double fx = 1279.07754353772;
		double fy = 1279.73275299414;
	}
	else if (strcmp((pBaseinfo->szUserID), "infantry1") == 0)
	{
		double fx = 1300.7;
		double fy = 1301.5;
	}
	else if (strcmp((pBaseinfo->szUserID), "infantry2") == 0)
	{
		double fx = 1289.1;
		double fy = 1288.7;
	}
	else if (strcmp((pBaseinfo->szUserID), "infantry3") == 0)
	{	
		double fx = 1283.9;
		double fy = 1286.2;
	}
	else 
	{
		double fx = 1279.07754353772;
		double fy = 1279.73275299414;
		printf("camera user ID error\n");
	}
	delete[]pBaseinfo;

	GX_OPEN_PARAM stOpenParam;

	//打开枚举列表中的第一台设备。
	//假设枚举到了3台可用设备，那么用户可设置stOpenParam参数的pszContent字段为1、2、3
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";

	status = GXOpenDevice(&stOpenParam, &hDevice);
	
	if (status != GX_STATUS_SUCCESS)
	{
		printf("Camera open fail\n");
		return 0;
	}

	//设置参数
	
	//设置图像分辨率
	int64_t nOffsetX = (1280- 640)/2;
	int64_t nOffsetY = (1024-480)/2;
	GXSetEnum(hDevice, GX_ENUM_RREGION_SELECTOR, GX_REGION_SELECTOR_REGION0);
	GXSetInt(hDevice, GX_INT_WIDTH, 640);
	GXSetInt(hDevice, GX_INT_HEIGHT, 480);
	GXSetInt(hDevice, GX_INT_OFFSET_X, nOffsetX);
	GXSetInt(hDevice, GX_INT_OFFSET_Y, nOffsetY);

	//设置帧率
	GXSetFloat(hDevice, GX_FLOAT_ACQUISITION_FRAME_RATE, 210);

	//设置自动白平衡光照环境
	GXSetEnum(hDevice, GX_ENUM_AWB_LAMP_HOUSE,2);
	//GX_AWB_LAMP_HOUSE_ADAPTIVE = 0,                      ///< 自适应光源
	//	GX_AWB_LAMP_HOUSE_D65 = 1,                      ///< 指定色温6500k
	//	GX_AWB_LAMP_HOUSE_FLUORESCENCE = 2,                      ///< 指定荧光灯
	//	GX_AWB_LAMP_HOUSE_INCANDESCENT = 3,                      ///< 指定白炽灯
	//	GX_AWB_LAMP_HOUSE_D75 = 4,                      ///< 指定色温7500k
	//	GX_AWB_LAMP_HOUSE_D50 = 5,                      ///< 指定色温5000k
	//	GX_AWB_LAMP_HOUSE_U30 = 6,                      ///< 指定色温3000k
	
	//打开自动白平衡
	GXSetEnum(hDevice, GX_ENUM_BALANCE_WHITE_AUTO, 1);
	//GX_BALANCE_WHITE_AUTO_CONTINUOUS=1

	//设置白平衡系数
	//GXSetFloat(hDevice, GX_FLOAT_BALANCE_RATIO, 1.6211);
	
	//设置曝光时间
	GXSetFloat(hDevice, GX_FLOAT_EXPOSURE_TIME, 4500);

	//开启自动坏点校正
	GXSetEnum(hDevice, GX_ENUM_DEAD_PIXEL_CORRECT, GX_DEAD_PIXEL_CORRECT_ON);

	//开启平场校正
	GXSetEnum(hDevice, GX_ENUM_FLAT_FIELD_CORRECTION,GX_ENUM_FLAT_FIELD_CORRECTION_ON);

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
