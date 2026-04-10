#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"menu.h"
#include<stdlib.h>
#include"model.h"
#include<time.h>
#include"card_service.h"
#include<string.h>
#include"tool.h"
#include"global.h"

void copy(char aInput[], char aOutput[], int nSize);
int getsize(const char* pInfo);

void outputMenu() {
	printf("\n");
	printf("----------菜单----------\n");
	printf("1.添加卡\n");
	printf("2.查询卡\n");
	printf("3.上机\n");
	printf("4.下机\n");
	printf("5.充值\n");
	printf("6.退费\n");
	printf("7.查询统计\n");
	printf("8.注销卡\n");
	printf("0.退出\n");
	printf("请选择菜单项编号（0~8）：");
}

void add() {
	Card card;
	char aName[32] = { '\0' };
	char aPwd[20] = { '\0' };
	int nNameSize = 0;
	int nPwdSize = 0;
	printf("----------添加卡----------\n");
	printf("请输入卡号（0~18）：");
	if (scanf("%s", aName) != 1) { // 检查字符串输入
		printf("输入错误！\n");
		while (getchar() != '\n');
		return;
	}

	//判断卡号长度
	nNameSize = getsize(aName);
	if (nNameSize > 18) {
		printf("输入的卡号超过规定长度！\n");
		return;
	}
	//将输入的卡号保存到卡结构体中
	strcpy(card.aName, aName);

	printf("请输入密码（0~8）：");
	if (scanf("%s", aPwd) != 1) { // 检查字符串输入
		printf("输入错误！\n");
		while (getchar() != '\n');
		return;
	}

	//判断密码长度
	nPwdSize = getsize(aPwd);
	if (nPwdSize > 8) {
		printf("输入的密码超过规定长度！\n");
		return;
	}
	//将输入的密码保存到卡结构体中
	strcpy(card.aPwd, aPwd);

	printf("请输入开卡金额（RMB）：");
	if (scanf(" %f", &card.fBalance) != 1) { // 检查浮点输入
		printf("金额必须是数字！\n");
		while (getchar() != '\n');
		return;
	}

	card.fTotalUse = card.fBalance;   //新增卡时默认累计金额和余额就是开卡金额
	card.nDel = 0;     //删除标识
	card.nStatus = 0;    //卡状态
	card.nUseCount = 0;     //使用次数

	card.tStart = card.tEnd = card.tLastTime = time(NULL);
	struct tm* startTime = localtime(&card.tStart), * endTime = localtime(&card.tEnd);
	endTime->tm_year = startTime->tm_year + 1;
	card.tEnd = mktime(endTime);//修改截止时间

    int ret = addCard(card);
    if (ret == TRUE) {
        printf("添加卡信息成功！\n"); // 实验要求的成功提示
	}
	else {
		printf("添加卡信息失败！\n"); // 实验要求的失败提示
		return;
	}

	printf("\n");
	printf("-----添加的卡信息如下-----\n");
	printf("卡号\t密码\t卡状态\t余额\n");
	printf("%s\t%s\t%d\t%.1f\n", card.aName, card.aPwd, card.nStatus, card.fBalance);
}
	
int getsize(const char* pInfo) {
	int nSize = 0;
	while (*(pInfo + nSize) != '\0') {
		nSize++;
	}
	return nSize;
}

void copy(char aInput[], char aOutput[], int nSize) {
	for (int i = 0; i < nSize; i++) {
		aOutput[i] = '\0';
	}
	int i = 0;
	while (i < nSize - 1) {
		if (aInput[i] == '\0') {
			break;
		}
		aOutput[i] = aInput[i];
		i++;
	}
	aOutput[nSize - 1] = '\0';
}

void query() {
	printf("----------查询卡----------\n");
	char aName[18] = { 0 };
	char aTime[20] = { 0 };
	int nIndex = 0;
	int i = 0;
	Card* pCard = NULL;
	printf("请输入查询的卡号：");
	(void)scanf("%s", aName);
		pCard = queryCards(aName, &nIndex);
	
	if (pCard == NULL || nIndex == 0) {
		printf("没有该卡的信息！\n");
		return;
	} 
	printf("-----查询到 %d 张匹配的卡-----\n", nIndex);
	printf("%-10s\t%-6s\t%-8s\t%-12s\t%-12s\t%-20s\n", "卡号", "状态", "余额", "累计使用", "使用次数", "上次使用时间");
		//显示 
    for (i = 0; i < nIndex; i++) {
		 timeToString(pCard[i].tLastTime, aTime);
		printf("%-10s\t%-6d\t%-8.1f\t%-12.1f\t%-12d\t%s\n", pCard[i].aName, pCard[i].nStatus, pCard[i].fBalance, pCard[i].fTotalUse, pCard[i].nUseCount, aTime);
	   
	}
	free(pCard);
}

void exitApp() {
	releaseCardList();
}