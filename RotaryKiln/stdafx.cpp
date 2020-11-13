// stdafx.cpp : 只包括标准包含文件的源文件
// 回转窑.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

AccessConnection accessConnect;

thatboy::logger::FileLogger logger{ "running.log" };

int MachineKey = 0;
int TryCode = 0;