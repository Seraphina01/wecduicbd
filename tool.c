#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include"tool.h"
#include<string.h>
#include<stdlib.h>

//将time_t类型转换为字符串，字符串格式为“年-月-日 时：分”
void timeToString(time_t t, char* pBuf) {
	struct tm pTimeInfo;
	localtime_s(&pTimeInfo,&t);
	strftime(pBuf, 20, "%Y-%m-%d %H:%M", &pTimeInfo);
}
//字符串转time_t（解析"年-月-日 时：分"格式）
time_t stringToTime(char* pTime) {
    struct tm tm = {0};
    long year, mon, mday, hour, min;
    int ret = sscanf(pTime, "%ld-%ld-%ld %ld:%ld", 
           &year, &mon, &mday, &hour, &min);
    if (ret != 5) {
        return time(NULL);
    }
    //转换为tm结构体（tm_year从1900开始，tm_mon从0开始）
    tm.tm_year = (int)(year - 1900);
    tm.tm_mon = (int)(mon - 1);
    tm.tm_mday = (int)mday;
    tm.tm_hour = (int)hour;
    tm.tm_min = (int)min;
    tm.tm_sec = 0;
    return mktime(&tm);
}