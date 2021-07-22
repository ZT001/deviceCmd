# deviceCmd
使用示例：
配置指令示例		返回	查询指令示例	
SN,123456789012\r\n	OK	SN？\r\n		123456789012
UP:ComType,0\r\n		OK	UP:ComType？\r\n	0
UP:Prot,1\r\n		OK	UP:Prot？\r\n	1
UP:Ip,192.168.001.025\r\n	OK	UP:Ip?\r\n	192.168.001.025

代码调用：			
void CMD_Add(char *cmd, uint8_t *pointer, uint32_t size, eCMDType cmdType, eValueType valueType)			
/*cmd init 添加指令与参数绑定*/			
CMD_Add("SN", (uint8_t *)&_PROJECT_cfg.hreg.SN, 12, cParam, pCHAR);			
CMD_Add("UP:Type", (uint8_t *)&_PROJECT_cfg.UP_InterfaceType, sizeof(_PROJECT_cfg.UP_InterfaceType), cParam, pBYTE);			
CMD_Add("UP:AppId", (uint8_t *)&_PROJECT_cfg.UP_AppId, sizeof(_PROJECT_cfg.UP_AppId), cParam, pBYTE);			
CMD_Add("UP:Ip", (uint8_t *)&_PROJECT_cfg.hreg.UP_IP, sizeof(_PROJECT_cfg.hreg.UP_IP), cParam, pIP);			
CMD_Add("UP:Port", (uint8_t *)&_PROJECT_cfg.hreg.UP_Port, sizeof(_PROJECT_cfg.hreg.UP_Port), cParam, pUINT16);			
CMD_Add("SAVE", NULL, 0, cSystem, pNone);	
CMD_Add("BACKUP", NULL, 0, cSystem, pNone);	
CMD_Add("RECOVER", NULL, 0, cSystem, pNone);	
/*cmd 处理*/		
CMD_CmdExcute((char *)cmdIn, (char *)cmdEcho, Event);		
