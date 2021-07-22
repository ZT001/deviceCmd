/*
 * @Description  : 设备指令集
 * @Author       : zwt
 * @Date         : 2020-05-14 13:41:58
 * @LastEditTime : 2020-12-11 08:47:22
 * @LastEditors  : zwt
 */

#ifndef _RC_CMD_
#define _RC_CMD_

#include <stdint.h>

#define MAX_CMD_COUNT 100
#define MAX_CMD_LENGTH 20
#define MAX_CMD_STR_LENGTH 128
#define MAX_PARAM_COUNT 10
typedef enum
{
	pNone = 0x00,
	// pCHAR			=0x01,
	pBYTE = 0x02,
	pINT16 = 0x03,
	pUINT16 = 0x04,
	pINT32 = 0x05,
	pUINT32 = 0x06,
	pFLOAT = 0x07,
	pSTRING = 0x08,
	pIP = 0x09,
	pMAC = 0x0A,
	pAI = 0x0B,
	pDI = 0x0C,
	pDO = 0x0D,
	pMETER = 0x0E,
} eValueType;

typedef enum
{
	ctParam = 0x01, /*参数配置指令，读写操作*/
	ctSystem = 0x02 /*系统指令，执行系统相关操作，
								 收到此类参数直接通过事件通知调用程序自行处理*/
} eCMDType;

typedef struct
{
	char cmd[MAX_CMD_LENGTH]; /*cmd string*/
	uint8_t *paramPointer;	  /*参数指针*/
	uint32_t Size;			  /*参数数量( 单位为sizeof(ValueType),STRING类型表示字符串长度),
                               size>1认为此参数为数组参数，指令支持数组模式标记[1]*/
	eCMDType cmdType;	  /*指令类型，参数，系统*/
	eValueType ValueType; /*参数值类型*/
} sBindingInfo;

/**
 * @description: 执行传入的cmd
 * @param      : cmd，传入的指令
 *               paramPointer,绑定数据地址，固定数据类型见设备命令表
 * @return     :
 */
void CMD_Add(char *cmd, uint8_t *pointer, uint32_t size, eCMDType cmdType,
			 eValueType valueType);

/**
 * @description: 获取cmd数量
 * @param      :
 * @return     : cmd数量
 */
uint32_t CMD_GetCount(void);

/**
 * @description: 获取cmd字符串
 * @param      : id,cmd序号
                                                                 cmdstr，传入的指令
 * @return     :
 */
void CMD_GetCmdInfo(int id, sBindingInfo *bi);

/**
 * @description: 执行传入的cmd
 * @param      : cmd，传入的指令
 *               echo,回传字符串
                                                                 eventHandle,事件通知，通知主程序执行系统指令
 * @return     :
 */
void CMD_Excute(char *cmd, char *echo, void(eventHandle(char *cmd)));

#endif
