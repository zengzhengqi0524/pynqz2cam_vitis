/*
 * @Author:
 * @Description:
 * @Date: 2022-04-13 18:07:59
 * @LastEditTime: 2022-04-13 18:19:55
 * @FilePath: \src\helloworld.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xaxivdma_i.h"
#include "display_ctrl_hdmi/display_ctrl.h"
#include "vdma_api/vdma_api.h"
#include "emio_sccb_cfg/emio_sccb_cfg.h"
#include "ov2640/ov2640.h"

//宏定义
#define DYNCLK_BASEADDR XPAR_AXI_DYNCLK_0_BASEADDR //动态时钟基地址
#define VDMA_ID XPAR_AXIVDMA_0_DEVICE_ID		   // VDMA器件ID
#define DISP_VTC_ID XPAR_VTC_0_DEVICE_ID		   // VTC器件ID

//全局变量
// frame buffer的起始地址
unsigned int const frame_buffer_addr = (XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x1000000);
XAxiVdma vdma;
DisplayCtrl dispCtrl;
VideoMode vd_mode;

int main(void)
{
	u32 status;
	//	u16 cmos_h_pixel;                    //ov2640 DVP 输出水平像素点数
	//	u16 cmos_v_pixel;                    //ov2640 DVP 输出垂直像素点数
	//	u16 total_h_pixel;                   //ov2640 水平总像素大小
	//	u16 total_v_pixel;                   //ov2640 垂直总像素大小

	//	cmos_h_pixel = 800;                 //设置OV2640输出分辨率为1280*720
	//	cmos_v_pixel = 600;
	//	total_h_pixel = 1600;
	//	total_v_pixel = 1200;

	SCCB_Init(); //初始化EMIO
	status = OV2640_Init();
	if (status == 0)
		xil_printf("OV2640 detected successful!\r\n");
	else
		xil_printf("OV2640 detected failed!\r\n");

	vd_mode = VMODE_800x600;

	//配置VDMA
	run_vdma_frame_buffer(&vdma, VDMA_ID, vd_mode.width, vd_mode.height,
						  frame_buffer_addr, 0, 0, BOTH);
	//初始化Display controller
	DisplayInitialize(&dispCtrl, DISP_VTC_ID, DYNCLK_BASEADDR);
	//设置VideoMode
	DisplaySetMode(&dispCtrl, &vd_mode);
	DisplayStart(&dispCtrl);

	return 0;
}
