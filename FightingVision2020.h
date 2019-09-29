﻿#pragma once

#include <iostream>

#include <opencv2/opencv.hpp>

#include "Camera/FightingCapture.h"
#include "Camera/FightingVideoCapture.h"
#include "Camera/FightingCameraCapture.h"

#include "SerialPort/SerialPort.h"

enum class State { ENERGY_STATE, ARMOR_STATE };