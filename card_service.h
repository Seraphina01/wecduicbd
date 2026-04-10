#ifndef CARD_SERVICE_H
#define CARD_SERVICE_H
#include"model.h"

int addCard(Card card);
int initCardList();
Card* queryCards(const char* pName, int* pIndex);//查询卡（先同步文件到链表，再模糊查询）
void releaseCardList();
int getCard();//从文件同步卡信息到链表

#endif 


