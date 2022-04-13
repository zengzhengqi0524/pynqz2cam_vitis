#include "emio_sccb_cfg.h"

void SCCB_SDA(u8 data)
{
	XGpioPs_WritePin(&gpiops_inst, EMIO_SDA_NUM, data);
}

void SCCB_SCL(u8 data)
{
	XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, data);
}

//初始化SCCB接口
u8 SCCB_Init(void)
{

	XGpioPs_Config *gpiops_cfg_ptr;
	int xStatus;
	gpiops_cfg_ptr = XGpioPs_LookupConfig(GPIOPS_ID);
	if (gpiops_cfg_ptr == NULL)
		return XST_FAILURE;
	xStatus = XGpioPs_CfgInitialize(&gpiops_inst, gpiops_cfg_ptr, gpiops_cfg_ptr->BaseAddr);
	if (XST_SUCCESS != xStatus)
		print("EMIO INIT FAILED \n\r");

	//设置IO方向
	XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_SCL_NUM, 1);
	XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_SDA_NUM, 1);

	//使能IO
	XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_SCL_NUM, 1);
	XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_SDA_NUM, 1);

	//初始化置1
	SCCB_SDA(1); //数据线高电平
	SCCB_SCL(1); //在时钟线高的时候数据线由高至低

	return xStatus;
}

void SCCB_Start(void)
{
	u32 value;

	SCCB_SDA(1); //数据线高电平
	SCCB_SCL(1); //在时钟线高的时候数据线由高至低
	usleep(50);
	SCCB_SDA(0);
	usleep(50);
	SCCB_SCL(0); //数据线恢复低电平，单操作函数必要
}

// SCCB停止信号
void SCCB_Stop(void)
{
	SCCB_SDA(0);
	usleep(50);
	SCCB_SCL(1);
	usleep(50);
	SCCB_SDA(1);
	usleep(50);
}

void SCCB_No_Ack(void)
{
	usleep(50);
	SCCB_SDA(1);
	SCCB_SCL(1);
	usleep(50);
	SCCB_SCL(0);
	usleep(50);
	SCCB_SDA(0);
	usleep(50);
}

// SCCB,写入一个字节
//返回值:0,成功;1,失败.
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j, res;
	for (j = 0; j < 8; j++) //循环8次发送数据
	{
		if (dat & 0x80)
			SCCB_SDA(1);
		else
			SCCB_SDA(0);
		dat <<= 1;
		usleep(50);
		SCCB_SCL(1);
		usleep(50);
		SCCB_SCL(0);
	}
	SCCB_SDA_IN(); //设置SDA为输入
	usleep(50);
	SCCB_SCL(1); //接收第九位,以判断是否发送成功
	usleep(50);
	if (SCCB_READ_SDA)
		res = 1; // SDA=1发送失败，返回1
	else
		res = 0; // SDA=0发送成功，返回0
	SCCB_SCL(0);
	SCCB_SDA_OUT(); //设置SDA为输出
	return res;
}

// SCCB 读取一个字节
//在SCL的上升沿,数据锁存
//返回值:读到的数据
u8 SCCB_RD_Byte(void)
{
	u8 temp = 0, j;
	SCCB_SDA_IN();			//设置SDA为输入
	for (j = 8; j > 0; j--) //循环8次接收数据
	{
		usleep(50);
		SCCB_SCL(1);
		temp = temp << 1;
		if (SCCB_READ_SDA)
			temp++;
		usleep(50);
		SCCB_SCL(0);
	}
	SCCB_SDA_OUT(); //设置SDA为输出
	return temp;
}
//写寄存器
//返回值:0,成功;1,失败.
u8 SCCB_WR_Reg(u8 reg, u8 data)
{
	u8 res = 0;
	SCCB_Start(); //启动SCCB传输
	if (SCCB_WR_Byte(SCCB_ID))
		res = 1; //写器件ID
	usleep(100);
	if (SCCB_WR_Byte(reg))
		res = 1; //写寄存器地址
	usleep(100);
	if (SCCB_WR_Byte(data))
		res = 1; //写数据
	SCCB_Stop();
	return res;
}
//读寄存器
//返回值:读到的寄存器值
u8 SCCB_RD_Reg(u8 reg)
{
	u8 val = 0;
	SCCB_Start();		   //启动SCCB传输
	SCCB_WR_Byte(SCCB_ID); //写器件ID
	usleep(100);
	SCCB_WR_Byte(reg); //写寄存器地址
	usleep(100);
	SCCB_Stop();
	usleep(100);
	//设置寄存器地址后，才是读
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID | 0X01); //发送读命令
	usleep(100);
	val = SCCB_RD_Byte(); //读取数据
	SCCB_No_Ack();
	SCCB_Stop();
	return val;
}
