/*
 * @Description  :
 * @Author       : zwt
 * @Date         : 2020-05-14 13:41:22
 * @LastEditTime : 2020-12-10 17:56:58
 * @LastEditors  : zwt
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rc_cmd.h"

static sBindingInfo _Bi[MAX_CMD_COUNT] = {0};
static uint32_t _BiCount = 0;


/*****************************************************************
 * private function
 * **************************************************************/
/**
 * @description: 查找指令，找到返回指令下标，找不到返回-1
 * @param      : cmd，传入的指令
 *               paramPointer,绑定数据地址，固定数据类型见设备命令表
 * @return     :
 */
static int CMD_GetBandingIndex(char *cmd)
{
  for (int i = 0; i < _BiCount; i++)
  {
    /*命令长度不相等则比较下一个*/
    if (strlen(cmd) != strlen(_Bi[i].cmd))
      continue;
    /*比较命令部分*/
    if (strncasecmp(_Bi[i].cmd, cmd, strlen(_Bi[i].cmd)) == 0)
      return i;
  }

  return -1; /*未找到指令*/
}

/**
 * @description: 
 * @param      : 
 * @return     : 
 */
static void CMD_BYTE(uint8_t biIndex, uint8_t index, uint8_t isQuery, char *param[], uint8_t paramCount, char *echo)
{
  const int TYPE_SIZE = 1;
  /*指令响应处理*/
  if (isQuery)
  {
    sprintf(echo, "%d\r\n", *((uint8_t *)(_Bi[biIndex].paramPointer + index * TYPE_SIZE)));
  }
  else
  {
    uint8_t aa = (uint8_t)atoi(param[0]);
    memcpy((uint8_t *)_Bi[biIndex].paramPointer + index * TYPE_SIZE, &aa, _Bi[biIndex].Size);
  }
}
/**
 * @description: 
 * @param      : 
 * @return     : 
 */
static void CMD_INT16(uint8_t biIndex, uint8_t index, uint8_t isQuery, char *param[], uint8_t paramCount, char *echo)
{
  const int TYPE_SIZE = 2;
  /*指令响应处理*/
  if (isQuery)
  {
    sprintf(echo, "%d\r\n", *((int16_t *)(_Bi[biIndex].paramPointer)));
  }
  else
  {
    int16_t aa = (int16_t)atoi(param[0]);
    memcpy((uint8_t *)_Bi[biIndex].paramPointer + index * TYPE_SIZE, &aa, TYPE_SIZE);
  }
}
/**
 * @description: 
 * @param      : 
 * @return     : 
 */
static void CMD_UINT16(uint8_t biIndex, uint8_t index, uint8_t isQuery, char *param[], uint8_t paramCount, char *echo)
{
  const int TYPE_SIZE = 2;
  /*指令响应处理*/
  if (isQuery)
  {
    sprintf(echo, "%d\r\n", *((uint16_t *)(_Bi[biIndex].paramPointer)));
  }
  else
  {
    uint16_t aa = (uint16_t)atoi(param[0]);
    memcpy((uint8_t *)_Bi[biIndex].paramPointer + index * TYPE_SIZE, &aa, TYPE_SIZE);
  }
}
/**
 * @description: 
 * @param      : 
 * @return     : 
 */
static void CMD_INT32(uint8_t biIndex, uint8_t index, uint8_t isQuery, char *param[], uint8_t paramCount, char *echo)
{
  const int TYPE_SIZE = 4;
  /*指令响应处理*/
  if (isQuery)
  {
    sprintf(echo, "%d\r\n", *((int32_t *)(_Bi[biIndex].paramPointer)));
  }
  else
  {
    int32_t aa = (int32_t)atoi(param[0]);
    memcpy((uint8_t *)_Bi[biIndex].paramPointer + index * TYPE_SIZE, &aa, TYPE_SIZE);
  }
}
/**
 * @description: 
 * @param      : 
 * @return     : 
 */
static void CMD_UINT32(uint8_t biIndex, uint8_t index, uint8_t isQuery, char *param[], uint8_t paramCount, char *echo)
{
  const int TYPE_SIZE = 4;
  /*指令响应处理*/
  if (isQuery)
  {
    sprintf(echo, "%d\r\n", *((uint32_t *)(_Bi[biIndex].paramPointer)));
  }
  else
  {
    uint32_t aa = (uint32_t)atoi(param[0]);
    memcpy((uint8_t *)_Bi[biIndex].paramPointer + index * TYPE_SIZE, &aa, TYPE_SIZE);
  }
}
/**
 * @description: 
 * @param      : 
 * @return     : 
 */
static void CMD_FLOAT(uint8_t biIndex, uint8_t index, uint8_t isQuery, char *param[], uint8_t paramCount, char *echo)
{
  const int TYPE_SIZE = 4;
  /*指令响应处理*/
  if (isQuery)
  {
    sprintf(echo, "%f\r\n", *((float *)(_Bi[biIndex].paramPointer)));
  }
  else
  {
    float aa = (float)atof(param[0]);
    memcpy((uint8_t *)_Bi[biIndex].paramPointer + index * TYPE_SIZE, &aa, TYPE_SIZE);
  }
}
/**
 * @description: 
 * @param      : 
 * @return     : 
 */
static void CMD_STRING(uint8_t biIndex, uint8_t index, uint8_t isQuery, char *param[], uint8_t paramCount, char *echo)
{
	uint32_t strl=0;
  /*指令响应处理*/
  if (isQuery)
  {
		strl=strlen((char*)_Bi[biIndex].paramPointer);
		if(strl>_Bi[biIndex].Size)
			return;
    sprintf(echo, "%s\r\n", _Bi[biIndex].paramPointer);
  }
  else
  {
    if (_Bi[biIndex].Size >= strlen(param[0]))
      strcpy((char *)_Bi[biIndex].paramPointer, param[0]);
    else
      sprintf(echo, "max length %d\r\n", _Bi[biIndex].Size);
  }
}
/**
 * @description: 
 * @param      : 
 * @return     : 
 */
static void CMD_IP(uint8_t biIndex, uint8_t index, uint8_t isQuery, char *param[], uint8_t paramCount, char *echo)
{
  /*指令响应处理*/
  if (isQuery)
  {
    sprintf(echo, "%d.%d.%d.%d\r\n", _Bi[biIndex].paramPointer[0], _Bi[biIndex].paramPointer[1], _Bi[biIndex].paramPointer[2], _Bi[biIndex].paramPointer[3]);
  }
  else
  {
    char *str;
    if (strlen((char *)_Bi[biIndex].paramPointer) == strlen(param[0]))
      strcpy((char *)_Bi[biIndex].paramPointer, param[0]);
    str = strtok(param[0], ".");
    ((uint8_t *)_Bi[biIndex].paramPointer)[0] = atoi(str);
    str = strtok(NULL, ".");
    ((uint8_t *)_Bi[biIndex].paramPointer)[1] = atoi(str);
    str = strtok(NULL, ".");
    ((uint8_t *)_Bi[biIndex].paramPointer)[2] = atoi(str);
    str = strtok(NULL, ".");
    ((uint8_t *)_Bi[biIndex].paramPointer)[3] = atoi(str);
  }
}
/**
 * @description: 
 * @param      : 
 * @return     : 
 */
static void CMD_MAC(uint8_t biIndex, uint8_t index, uint8_t isQuery, char *param[], uint8_t paramCount, char *echo)
{
  /*指令响应处理*/
  if (isQuery)
  {
    sprintf(echo, "%d.%d.%d.%d.%d.%d\r\n", _Bi[biIndex].paramPointer[0],
            _Bi[biIndex].paramPointer[1], _Bi[biIndex].paramPointer[2], _Bi[biIndex].paramPointer[3], _Bi[biIndex].paramPointer[4], _Bi[biIndex].paramPointer[5]);
  }
  else
  {
    char *str;
    if (strlen((char *)_Bi[biIndex].paramPointer) == strlen(param[0]))
      strcpy((char *)_Bi[biIndex].paramPointer, param[0]);
    str = strtok(param[0], ".");
    ((uint8_t *)_Bi[biIndex].paramPointer)[0] = atoi(str);
    str = strtok(NULL, ".");
    ((uint8_t *)_Bi[biIndex].paramPointer)[1] = atoi(str);
    str = strtok(NULL, ".");
    ((uint8_t *)_Bi[biIndex].paramPointer)[2] = atoi(str);
    str = strtok(NULL, ".");
    ((uint8_t *)_Bi[biIndex].paramPointer)[3] = atoi(str);
    str = strtok(NULL, ".");
    ((uint8_t *)_Bi[biIndex].paramPointer)[4] = atoi(str);
    str = strtok(NULL, ".");
    ((uint8_t *)_Bi[biIndex].paramPointer)[5] = atoi(str);
  }
}
/**
 * @description: 
 * @param      : 
 * @return     : 
 */
static void CMD_AI(uint8_t biIndex, uint8_t index, uint8_t isQuery, char *param[], uint8_t paramCount, char *echo)
{
  const int TYPE_SIZE = 18;
  uint16_t unit;
  float f1, f2, f3, f4;
  /*ai channel info*/
  //typedef struct
  //{
  //	uint16_t	RangeUnit;
  //	float			RangeLow;
  //	float			RangeUpp;
  //	float     AlarmLow;
  //	float 		AlarmUpp;
  //}scfgAI_chn_info;
  /*指令响应处理*/
  if (isQuery)
  {
    memcpy(&unit, (_Bi[biIndex].paramPointer + index * TYPE_SIZE), sizeof(uint16_t));
    memcpy(&f1, (_Bi[biIndex].paramPointer + 2 + index * TYPE_SIZE), sizeof(float));
    memcpy(&f2, (_Bi[biIndex].paramPointer + 6 + index * TYPE_SIZE), sizeof(float));
    memcpy(&f3, (_Bi[biIndex].paramPointer + 10 + index * TYPE_SIZE), sizeof(float));
    memcpy(&f4, (_Bi[biIndex].paramPointer + 14 + index * TYPE_SIZE), sizeof(float));
    sprintf(echo, "%d,%.3f,%.3f,%.3f,%.3f\r\n", unit, f1, f2, f3, f4);
  }
  else
  {
    if (paramCount != 5)
    {
      sprintf(echo, "error param count\r\n");
    }
    unit = (uint16_t)atoi(param[0]);
    f1 = (float)atof(param[1]);
    f2 = (float)atof(param[2]);
    f3 = (float)atof(param[3]);
    f4 = (float)atof(param[4]);
    memcpy((_Bi[biIndex].paramPointer + index * TYPE_SIZE), &unit, sizeof(uint16_t));
    memcpy((_Bi[biIndex].paramPointer + 2 + index * TYPE_SIZE), &f1, sizeof(float));
    memcpy((_Bi[biIndex].paramPointer + 6 + index * TYPE_SIZE), &f2, sizeof(float));
    memcpy((_Bi[biIndex].paramPointer + 10 + index * TYPE_SIZE), &f3, sizeof(float));
    memcpy((_Bi[biIndex].paramPointer + 14 + index * TYPE_SIZE), &f4, sizeof(float));
  }
}
/**
 * @description: 
 * @param      : 
 * @return     : 
 */
static void CMD_DI(uint8_t biIndex, uint8_t index, uint8_t isQuery, char *param[], uint8_t paramCount, char *echo)
{
  const int TYPE_SIZE = 6;
  //typedef struct
  //{
  //	uint16_t DIInputType;//输入电压类型，220v输入，48v输入
  //	uint16_t Enable;//bit0~bit15,   0-未启用，1-启用
  //	uint16_t Effictive;//bit0~bit15,    0-0有效,1-1有效
  //}scfgDI_chn_info;
  /*指令响应处理*/
  if (isQuery)
  {
    sprintf(echo, "0x%02x,0x%02x,0x%02x\r\n", *((uint16_t *)(_Bi[biIndex].paramPointer + index * TYPE_SIZE)),
            *((uint16_t *)(_Bi[biIndex].paramPointer + 2 + index * TYPE_SIZE)),
            *((uint16_t *)(_Bi[biIndex].paramPointer + 4 + index * TYPE_SIZE)));
  }
  else
  {
    char *str;
    str = strtok(param[0], ",");
    *((uint16_t *)(_Bi[biIndex].paramPointer + index * TYPE_SIZE)) = (uint16_t)atoi(str);
    str = strtok(NULL, ",");
    *((uint16_t *)(_Bi[biIndex].paramPointer + 2 + index * TYPE_SIZE)) = (uint16_t)atof(str);
    str = strtok(NULL, ".");
    *((uint16_t *)(_Bi[biIndex].paramPointer + 4 + index * TYPE_SIZE)) = (uint16_t)atof(str);
  }
}
/**
 * @description: 
 * @param      : 
 * @return     : 
 */
static void CMD_DO(uint8_t biIndex, uint8_t index, uint8_t isQuery, char *param[], uint8_t paramCount, char *echo)
{
  const int TYPE_SIZE = 2;
  //typedef struct
  //{
  //	uint16_t		NC_NO;//bit0~bit15,   0-常开常闭
  //}scfgDO_chn_info;
  /*指令响应处理*/
  if (isQuery)
  {
    sprintf(echo, "0x%02x\r\n", *((uint16_t *)(_Bi[biIndex].paramPointer + index * TYPE_SIZE)));
  }
  else
  {
    *((uint16_t *)(_Bi[biIndex].paramPointer + index * TYPE_SIZE)) = (uint16_t)atoi(param[0]);
  }
}
/**
 * @description: 
 * @param      : 
 * @return     : 
 */
static void CMD_METER(uint8_t biIndex, uint8_t index, uint8_t isQuery, char *param[], uint8_t paramCount, char *echo)
{
  const int TYPE_SIZE = 20;
  //typedef struct
  //{
  //	uint8_t MAC[6];
  //}sMeterParam;
  /*指令响应处理*/
  if (isQuery)
  {
    sprintf(echo, "%02X:%02X:%02X:%02X:%02X:%02X\r\n", 
		_Bi[biIndex].paramPointer[0 + index * TYPE_SIZE],
		_Bi[biIndex].paramPointer[1 + index * TYPE_SIZE],
		_Bi[biIndex].paramPointer[2 + index * TYPE_SIZE],
		_Bi[biIndex].paramPointer[3 + index * TYPE_SIZE],
		_Bi[biIndex].paramPointer[4 + index * TYPE_SIZE],
		_Bi[biIndex].paramPointer[5 + index * TYPE_SIZE]);
  }
  else
  {
    char *str;
		unsigned int aa;
    str = strtok(param[0], ":");
		sscanf(str,"%02x",&aa);
		memcpy((_Bi[biIndex].paramPointer +0 + index * TYPE_SIZE),&aa,sizeof(uint8_t));
    str = strtok(NULL, ":");
		sscanf(str,"%02x",&aa);
		memcpy((_Bi[biIndex].paramPointer +1 + index * TYPE_SIZE),&aa,sizeof(uint8_t));
		str = strtok(NULL, ":");
		sscanf(str,"%02x",&aa);
		memcpy((_Bi[biIndex].paramPointer +2 + index * TYPE_SIZE),&aa,sizeof(uint8_t));
		str = strtok(NULL, ":");
		sscanf(str,"%02x",&aa);
		memcpy((_Bi[biIndex].paramPointer +3 + index * TYPE_SIZE),&aa,sizeof(uint8_t));
		str = strtok(NULL, ":");
		sscanf(str,"%02x",&aa);
		memcpy((_Bi[biIndex].paramPointer +4 + index * TYPE_SIZE),&aa,sizeof(uint8_t));
		str = strtok(NULL, ":");
		sscanf(str,"%02x",&aa);
		memcpy((_Bi[biIndex].paramPointer +5 + index * TYPE_SIZE),&aa,sizeof(uint8_t));
  }
}
/*****************************************************************
 * public function
 * **************************************************************/
/**
 * @description: 执行传入的cmd
 * @param      : cmd，传入的指令
 *               paramPointer,绑定数据地址，固定数据类型见设备命令表
 *							 size,数组类型长度[1-size],篇STRING类型为字符串最大长度
 *							 cmdtype，指令类型，
 *							 值类型，参数的数据类型，byte,int...
 * @return     :
 */
void CMD_Add(char *cmd, uint8_t *pointer, uint32_t size, eCMDType cmdType, eValueType valueType)
{
  /*检查cmd上限*/
  if (_BiCount > MAX_CMD_COUNT)
    return;
  /*检查CMD长度*/
  if (strlen(cmd) > MAX_CMD_LENGTH)
    return;

  strcpy(_Bi[_BiCount].cmd, cmd);
  _Bi[_BiCount].paramPointer = pointer;
  _Bi[_BiCount].Size = size;
  _Bi[_BiCount].cmdType = cmdType;
  _Bi[_BiCount].ValueType = valueType;

  _BiCount++;
}

/**
 * @description: 获取cmd数量
 * @param      : 
 * @return     : cmd数量
 */
uint32_t CMD_GetCount(void)
{
  return _BiCount;
}

/**
 * @description: 获取cmd字符串
 * @param      : id,cmd序号
								 cmdstr，传入的指令
 * @return     : 
 */
void CMD_GetCmdInfo(int id, sBindingInfo *bi)
{
  memcpy(bi, &_Bi[id], sizeof(sBindingInfo));
}

/**
 * @description: 执行传入的cmd
 * @param      : cmd，传入的指令
 *               echo,回传字符串
 * @return     :
 */
void CMD_Excute(char *cmd, char *echo, void(eventHandle(char *cmd)))
{
  int biIndex = -1;
  bool isQuery = false;
  char *substr, *l, *r;
  char *cmdstr;
  char *params[10] = {0};
  int n = 0;
  uint8_t index = 0; /*记录指令数组下标*/

  /*copy cmd*/
  /*check cmd length*/
  if (strlen(cmd) > MAX_CMD_STR_LENGTH)
    return;
  /*0-查询结束符*/
  substr = strstr(cmd, "\r\n");
  if (substr == NULL)
    return;
  else
  {
    /*清除结束符*/
    cmdstr = strtok(cmd, "\r\n");
  }
  /*1-是否查询指令*/
  isQuery = cmdstr[strlen(cmdstr) - 1] == '?';
  if (isQuery)
  {
    cmdstr = strtok(cmd, "?");
  }
  else
  {
    /*提取命令*/
    cmdstr = strtok(cmdstr, ",");
    /*提取参数列表*/
    do
    {
      params[n] = strtok(NULL, ",");
      n++;
    } while (params[n - 1] != NULL);
    n--;
    if (n > MAX_PARAM_COUNT)
      return;
  }
  /*判定长度*/
  if (strlen(cmdstr) == 0)
    return;
  /*查询索引,判定指令是否是数组类型*/
  l = strstr(cmdstr, "[");
  r = strstr(cmdstr, "]");
	/*没有下标[]默认下标为1*/
  if (l == NULL || r == NULL)
    index = 0;
  else
  {
    /*获取数组下标*/
    substr = strtok(l, r);
    index = atoi(substr + 1);
    if (index <= 0)
      return;
    /*输入cmd字符串下标从1开始*/
    index -= 1;
    /*清除数组字符*/
    cmdstr = strtok(cmdstr, "[");
  }

  /*查询cmd索引*/
  biIndex = CMD_GetBandingIndex(cmdstr);
  if (biIndex == -1)
  {
    strcpy(echo, "unknow cmd\r\n");
    return;
  }

  /*指令处理*/
  if (_Bi[biIndex].cmdType == ctParam)
  {
    /*索引超出尺寸则退出*/
    if (index >= _Bi[biIndex].Size)
    {
      strcpy(echo, "index out of range\r\n");
      return;
    }
    /*非查询指令且无配置参数则返回错误*/
    if (!isQuery && params[0] == 0)
    {
	if(_Bi[biIndex].ValueType ==pSTRING)
		params[0]="";/*字符串无参数则赋值为空*/
	else/*其他类型提示错误*/
	{
		strcpy(echo, "no parameter\r\n");
		return;
	}
    }
    /*按类型处理指令*/
    switch (_Bi[biIndex].ValueType)
    {
    /*通用数据类型*/
    case pBYTE:
      CMD_BYTE(biIndex, index, isQuery, params, n, echo);
      break;
    case pINT16:
      CMD_INT16(biIndex, index, isQuery, params, n, echo);
      break;
    case pUINT16:
      CMD_UINT16(biIndex, index, isQuery, params, n, echo);
      break;
    case pINT32:
      CMD_INT32(biIndex, index, isQuery, params, n, echo);
      break;
    case pUINT32:
      CMD_UINT32(biIndex, index, isQuery, params, n, echo);
      break;
    case pFLOAT:
      CMD_FLOAT(biIndex, index, isQuery, params, n, echo);
      break;
    case pSTRING:
      CMD_STRING(biIndex, index, isQuery, params, n, echo);
      break;
    case pIP:
      CMD_IP(biIndex, index, isQuery, params, n, echo);
      break;
    case pMAC:
      CMD_MAC(biIndex, index, isQuery, params, n, echo);
      break;
      /*项目专用类型*/
    case pAI:
      CMD_AI(biIndex, index, isQuery, params, n, echo);
      break;
    case pDI:
      CMD_DI(biIndex, index, isQuery, params, n, echo);
      break;
    case pDO:
      CMD_DO(biIndex, index, isQuery, params, n, echo);
      break;
    case pMETER:
      CMD_METER(biIndex, index, isQuery, params, n, echo);
      break;
    default:
      break;
    }
    /*echo为空则无问题，返回ok*/
    if (strlen(echo) == 0)
      strcpy(echo, "ok\r\n");
    /*事件通知-指令事件*/
    eventHandle(_Bi[biIndex].cmd);
    /*事件通知-保存参数*/
    //eventHandle("save");
  }
  else if (_Bi[biIndex].cmdType == ctSystem)
  {
    /*事件通知*/
    eventHandle(cmdstr);
  }
}
