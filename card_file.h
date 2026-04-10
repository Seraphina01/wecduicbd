#ifndef _CARD_FILE_
#define _CARD_FILE_
#include "model.h"

//保存单条卡信息到文件（追加模式，##分隔）
int saveCard(const Card* pCard, const char* pPath);

//获取文件中卡信息的总行数
int getCardCount(const char* pPath);

//读取文件中所有卡信息到结构体数组
int readCard(Card* pCard, const char* pPath);

//解析单行字符串（##分隔）为Card结构体
Card parseCard(char* pBuf);

#endif
