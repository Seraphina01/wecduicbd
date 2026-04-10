#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"card_file.h"
#include"tool.h"
#include"model.h"
#include"global.h"
#include <direct.h>  //用于创建文件夹

//辅助函数，创建文件夹
static void createDir(const char* dirPath) {
    int ret = _mkdir(dirPath);
    (void)ret;//静默使用，消除未使用变量警告
}

int saveCard(const Card* pCard, const char* pPath) {
    //入参合法性校验
    if (pCard == NULL || pPath == NULL || strlen(pPath) == 0) {
        return FALSE;
    }
    if (strlen(pCard->aName) > 18 || strlen(pCard->aPwd) > 8) {
        printf("卡号/密码长度超限！\n");
        return FALSE;
    }
    //打开文件："a"=追加模式，不存在则创建
    FILE* fp = fopen(pPath, "a");
    if (fp == NULL) {
        return FALSE;//文件打开失败，返回FALSE
    }
     //time_t转字符串（年-月-日 时：分）
    char tStartStr[20] = {0};
    char tEndStr[20] = {0};
    char tLastTimeStr[20] = {0};
    timeToString(pCard->tStart, tStartStr);
    timeToString(pCard->tEnd, tEndStr);
    timeToString(pCard->tLastTime, tLastTimeStr);

    fprintf(fp, "%s##%s##%d##%s##%s##%.1f##%s##%d##%.1f##%d\n",
            pCard->aName,        // 卡号
            pCard->aPwd,         // 密码
            pCard->nStatus,      // 状态
            tStartStr,           // 开卡时间（字符串）
            tEndStr,             // 截止时间（字符串）
            (float)pCard->fTotalUse,    // 累积金额
            tLastTimeStr,        // 最后使用时间（字符串）
            pCard->nUseCount,    // 使用次数
            (float)pCard->fBalance,     // 当前余额
            pCard->nDel);        // 删除标识

    fclose(fp);
    return TRUE;
}

//读取文件中所有卡信息到结构体数组
int readCard(Card* pCard, const char* pPath) {
    if (pCard == NULL || pPath == NULL || strlen(pPath) == 0) {
        return FALSE;
    }

    FILE* fp = fopen(pPath, "r");
    if (fp == NULL) {
        return FALSE;
    }

    char line[512] = {0};
    int index = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        //解析单行字符串为Card结构体
        pCard[index] = parseCard(line);
        index++;
        memset(line, 0, sizeof(line));
    }

    fclose(fp);
    return TRUE;
}

//解析##分隔的字符串为Card结构体
Card parseCard(char* pBuf) {
    Card card = {0};
    if (pBuf == NULL) {
        return card;
    }

    //按##分割字符串
    char* token = strtok(pBuf, "##");
    int fieldIndex = 0;
    while (token != NULL) {
        switch (fieldIndex) {
            case 0: strcpy(card.aName, token); break;        //卡号
            case 1: strcpy(card.aPwd, token); break;         //密码
            case 2: card.nStatus = atoi(token); break;       //状态
            case 3: card.tStart = stringToTime(token); break;//开卡时间（转time_t）
            case 4: card.tEnd = stringToTime(token); break;  //截止时间（转time_t）
            case 5: card.fTotalUse = (float)atof(token); break;     //累积金额
            case 6: card.tLastTime = stringToTime(token); break;//最后使用时间
            case 7: card.nUseCount = atoi(token); break;     //使用次数
            case 8: card.fBalance = (float)atof(token); break;      //当前余额
            case 9: card.nDel = atoi(token); break;          //删除标识
            default: break;
        }
        token = strtok(NULL, "##");
        fieldIndex++;
    }
    return card;
}

//获取文件中卡信息的行数
int getCardCount(const char* pPath) {
    if (pPath == NULL || strlen(pPath) == 0) {
        return 0;
    }

    FILE* fp = fopen(pPath, "r");
    if (fp == NULL) {
        return 0;
    }

    int count = 0;
    char line[512] = {0};
    while (fgets(line, sizeof(line), fp) != NULL) {
        count++;
        memset(line, 0, sizeof(line));
    }
    fclose(fp);
    return count;
}