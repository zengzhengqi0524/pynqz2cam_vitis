/*
 * @Author:
 * @Description:
 * @Date: 2022-04-13 14:50:07
 * @LastEditTime: 2022-04-13 18:13:17
 * @FilePath: \src\emio_sccb_cfg\emio_sccb_cfg.h
 */
#ifndef _EMIO_SCCB_CFG
#define _EMIO_SCCB_CFG
#include "xgpiops.h"
#include "sleep.h"

static XGpioPs gpiops_inst; // PS 端 GPIO 驱动实例

#define GPIOPS_ID XPAR_XGPIOPS_0_DEVICE_ID // PS 端 GPIO 器件 ID

#define EMIO_SCL_NUM 54
#define EMIO_SDA_NUM 55

#define SCCB_ID 0X60 // OV2640 SCCB ID

// IO操作函数
#define SCCB_READ_SDA XGpioPs_ReadPin(&gpiops_inst, EMIO_SDA_NUM)

// IO方向设置
#define SCCB_SDA_IN() XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_SDA_NUM, 0);
#define SCCB_SDA_OUT() XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_SDA_NUM, 1);

void SCCB_SDA(u8 data);
void SCCB_SCL(u8 data);
u8 SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_Ack(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg, u8 data);
u8 SCCB_RD_Reg(u8 reg);

#endif
