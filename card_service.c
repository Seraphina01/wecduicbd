#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include"model.h"
#include<string.h>
#include"global.h"
#include"card_file.h"
#include <direct.h>

Card aCard[50];  //卡信息结构体数组
int nCount = 0;  //卡信息实际个数
lpCardNode cardList = NULL;
#define CARD_FILE_PATH "D:\\AMS\\data\\card.txt"  //要求的文件路径

//初始化链表
int initCardList() {
	lpCardNode head = NULL;
	head = (lpCardNode)malloc(sizeof(CardNode));

	if (head != NULL) {
		head->next = NULL;
		cardList = head;
		return TRUE;
	}
	return FALSE;
}

//清空链表
static void clearCardList() {
    if (cardList == NULL) {
        return;
    }
    lpCardNode cur = cardList->next;
    while (cur != NULL) {
        lpCardNode temp = cur;
        cur = cur->next;
        free(temp);
    }
    cardList->next = NULL;
}
//从文件读取所有卡信息并同步到链表
int getCard() {
    if (cardList == NULL) initCardList(); 
    //清空链表（保证数据同步）
    clearCardList();

    //获取文件中卡的数量
    int cardNum = getCardCount(CARD_FILE_PATH);
    if (cardNum == 0) {
        return FALSE;
    }

    //读取文件中所有卡信息到数组
    Card* pCards = (Card*)malloc(cardNum * sizeof(Card));
    if (pCards == NULL) {
        return FALSE;
    }
    if (readCard(pCards, CARD_FILE_PATH) == FALSE) {
        free(pCards);
        return FALSE;
    }

    //将数组数据插入链表
    lpCardNode pTemp = cardList;
    for (int i = 0; i < cardNum; i++) {
        lpCardNode cur = (lpCardNode)malloc(sizeof(CardNode));
        if (cur == NULL) {
            free(pCards);
            return FALSE;
        }
        cur->data = pCards[i];
        cur->next = NULL;
        pTemp->next = cur;
        pTemp = cur;
    }

    free(pCards);
    return TRUE;
}

int addCard(Card card) {
	lpCardNode cur = NULL;
	lpCardNode pTemp = cardList;

	if (cardList == NULL) {
		if (!initCardList()) {
			return FALSE;
		}
		pTemp = cardList;
	}
    if (pTemp == NULL) {
		return FALSE;
	}
    int mkdirRet = _mkdir("D:\\AMS\\data");
    (void)mkdirRet;

    int fileRet = saveCard(&card, "card.txt");
    if (fileRet == FALSE) {
        return FALSE; // 文件保存失败，返回FALSE
    }
    cur = (lpCardNode)malloc(sizeof(CardNode));
    if (cur != NULL) {
        cur->data = card;
        cur->next = NULL;
        //遍历链表找到最后一个节点
        while (pTemp->next != NULL) {
            pTemp = pTemp->next;
        }
        pTemp->next = cur;
        return TRUE;
    }
	return FALSE;
}

void releaseCardList() {
	lpCardNode cur;
	if (cardList != NULL) {
		while (cardList->next != NULL) {
			cur = cardList->next;
			free(cur);
			cur = NULL;
		}
		//释放头节点
        free(cardList);
        cardList = NULL;
	}
}


Card* queryCards(const char* pName, int* pIndex) {
    //空指针校验（避免访问NULL报错）
    if (pIndex == NULL || pName == NULL) {
        return NULL;
    }
    *pIndex = 0; //初始化匹配数量为0

    //优先查内存链表
    if (cardList != NULL && cardList->next != NULL) {
        lpCardNode pTemp = cardList->next;
        while (pTemp != NULL) {
            if (strstr(pTemp->data.aName, pName) != NULL) {
                (*pIndex)++;
            }
            pTemp = pTemp->next;
        }
        if (*pIndex > 0) {
            Card* pCard = (Card*)malloc(*pIndex * sizeof(Card));
            if (pCard == NULL) {
                *pIndex = 0;
                return NULL;
            }
            pTemp = cardList->next;
            int i = 0;
            while (pTemp != NULL && i < *pIndex) {
                if (pName[0] == '\0' || strstr(pTemp->data.aName, pName) != NULL) {
                    pCard[i] = pTemp->data;
                    i++;
                }
                pTemp = pTemp->next;
            }
            return pCard;
        }
    }
    // ========== 原有逻辑保留 ==========
    //实验要求：先清空链表，从文件同步最新数据到链表
    getCard();

    //链表为空，直接返回NULL（无数据）
    if (cardList == NULL || cardList->next == NULL) {
        return NULL;
    }

    //第一步：遍历链表，统计匹配的卡数量
    lpCardNode pTemp = cardList->next;
    while (pTemp != NULL) {
        //模糊匹配：卡号包含输入的pName
        if (strstr(pTemp->data.aName, pName) != NULL) {
            (*pIndex)++; //匹配数+1
        }
        pTemp = pTemp->next;
    }

    //无匹配结果，返回NULL
    if (*pIndex == 0) {
        return NULL;
    }

    //分配内存，存储匹配的卡信息（动态数组）
    Card* pCard = (Card*)malloc(*pIndex * sizeof(Card));
    if (pCard == NULL) { //内存分配失败
        *pIndex = 0;
        return NULL;
    }

    //遍历链表，填充匹配的卡信息到数组
    pTemp = cardList->next;
    int i = 0;
    while (pTemp != NULL && i < *pIndex) {
        if (pName[0] == '\0' || strstr(pTemp->data.aName, pName) != NULL) {
            pCard[i] = pTemp->data; //复制卡信息到数组
            i++;
        }
        pTemp = pTemp->next;
    }

    //返回匹配结果（数组首地址）
    return pCard;
}