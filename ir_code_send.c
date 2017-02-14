#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "irdb_manager.h"
#include "dev_info.h"

void send_ir_code_general(char *code, int len, int key_id)
{
	int i;
	char send_buf[32];

	memset(send_buf, 0, sizeof(send_buf));
#if 0
	for(i = 0; i < len; i++) {
		printf("0x%02hhx ", code[i]);
	}
	printf("\n");
#endif
	send_buf[0] = 0x30;
	send_buf[1] = 0x00;
	
	send_buf[2] = code[0];
	
	send_buf[3] = code[key_id];
	send_buf[4] = code[key_id+1];

	send_buf[5] = code[len-4];
	send_buf[6] = code[len-3];
	send_buf[7] = code[len-2];
	send_buf[8] = code[len-1];

	printf("ir_code is : ");
	for(i = 0; i < 9; i++) {
		printf("0x%02hhx ", send_buf[i]);
		send_buf[9] += send_buf[i];
	}
	printf("0x%02hhx\n", send_buf[9]);
	//send_ir_code();
}

void send_ir_code_arc(char *code, int len, arc_info_t* arc_info)
{
	int i;
	char send_buf[32];
	int data_len;

#if 0
	for(i = 0; i < len; i++) {
		printf("0x%02hhx ", code[i]);
	}
	printf("\n");
#endif
	if(code[9] != len -10) {
		printf("code data error\n");
		return;
	}
	
	memset(send_buf, 0, sizeof(send_buf));
	send_buf[0] = 0x30;
	send_buf[1] = 0x01;
	for(i = 0; i < len; i++) {
		send_buf[2+i] = code[i];
	}
	data_len = len + 2;
	if(arc_info != NULL) {
		send_buf[4+0] = arc_info->temp;
		send_buf[4+1] = arc_info->wind_volume;
		send_buf[4+2] = arc_info->wind_dir;
		send_buf[4+3] = arc_info->auto_wind_dir;
		send_buf[4+4] = arc_info->power_status;
		send_buf[4+5] = arc_info->key_code;
		send_buf[4+6] = arc_info->arc_mode;
	}

	printf("send ir_code is : ");
	for(i = 0; i < data_len; i++) {
		printf("0x%02hhx ", send_buf[i]);
		send_buf[data_len] += send_buf[i];
	}
	data_len += 1;
	printf("0x%02hhx\n", send_buf[data_len]);
}
