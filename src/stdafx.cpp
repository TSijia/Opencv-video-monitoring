// stdafx.cpp : 只包括标准包含文件的源文件
// video_server.pch 将作为预编译标头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

vector<CommandAndVar> commandvar = {
{ "threadID",               "-1"     },
{ "threadOP",               "none"   },
{ "CameraLabel",            "-1"     },
{ "filePath",               "luren1.avi" },//"luren1.avi" },
{ "areaID",                 "111111" },
{ "edgeNUM",                "4"      },
{ "method",                 "knn"    }
};

map<int, vector<CommandAndVar>> ParameterTable = {
{ 0, commandvar },
{ 1, commandvar },
{ 2, commandvar },
{ 3, commandvar },
{ 4, commandvar },
{ 5, commandvar },
};
map<int, int> threadoperator = {
{ 0, 0 },
{ 1, 0 },
{ 2, 0 },
{ 3, 0 },
{ 4, 0 },
{ 5, 0 },
};

// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用
