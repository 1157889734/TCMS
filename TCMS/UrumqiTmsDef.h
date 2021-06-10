
#define BIG_ENDIAN_F1H	0
#define BIG_ENDIAN_FFH 0 
#define BIG_ENDIAN_1F0H 0 
#define BIG_ENDIAN_1F1H 0 
#define BIG_ENDIAN_1F2H 0 
#define BIG_ENDIAN_80H 0 
#define BIG_ENDIAN_81H 0

typedef struct 
{ 
#if BIG_ENDIAN_F1H 
	BYTE line_id_h;			// 1   线路号 高字节 
	BYTE line_id_l;		    // 2
	BYTE train_id_1;		// 3   列车号 
	BYTE train_id_2;		// 4
	BYTE train_id_3; 		// 5
	BYTE car_id; 		    // 6
	BYTE reserve1; 			// 7
	BYTE reserve2; 			// 8
#else //小端
	BYTE line_id_l;		    // 2
	BYTE line_id_h;			// 1   线路号 高字节 	
	BYTE train_id_2;		// 3   列车号 
	BYTE train_id_1;		// 4
	BYTE reserve[4]; 		// 5-8 
#endif 
}CAR_ID_F1H_T;	//车号

typedef struct 
{ 
#if BIG_ENDIAN_FFH 
	BYTE year;			// 1   
	BYTE month;			// 2 
	BYTE date;			// 3 
	BYTE hour;			// 4 
	BYTE minute;		// 5 
	BYTE second;		// 6 
	BYTE time_valid;    // 7
	BYTE reserve1;      // 8
	BYTE reserve2[8];   // 9-16
#else 
	BYTE month;      // 2 
	BYTE year;      // 1   
	BYTE hour;      // 4 
	BYTE date;      // 3 
	BYTE second;      // 6 
	BYTE minute;      // 5 
	struct
	{
		BYTE Date_Time_Valid:1;	//严重故障
		BYTE res:7;
	}DateTimeValid_t;
	BYTE reserve;	
#endif 
}TIME_FFH_T;	//时间



typedef struct
{
#if BIG_ENDIAN_1F0H
	struct
	{
		BYTE ATC1_Serious_fault:1;	//严重故障
		BYTE ATC1_Medium_fault:1;
		BYTE res:1;
		BYTE CBTC_MODE:1;
		BYTE res:4;
	}fault_valid_t;
	BYTE res1;
	BYTE res2[6];
#else
	BYTE res;	//1
	struct
	{
		BYTE ATC1_Serious_fault:1;	//严重故障
		BYTE ATC1_Medium_fault:1;
		BYTE res:1;
		BYTE CBTC_MODE:1;
		BYTE res1:4;
	}fault_valid_t;	//2
	BYTE res2[6];	//8
#endif
}ATC_1F0H_T;	//ATC故障

typedef struct
{
#if BIG_ENDIAN_1F1H
	struct 
	{
		BYTE ATC1_ATO_active:1; //ATO模式激活
		BYTE ATC1_ATB_mode:1;
		BYTE ATC1_Prot_Manual_mode:1;
		BYTE ATC1_Rest_Manual_mode:1;
		BYTE ATC1_ATP_Reverse_mode:1;
		BYTE ATC1_Drive_command:1;
		BYTE ATC1_Brake_command:1;
		BYTE ATC1_Emergency_brake:1;
	}mode_info_t;
	struct
	{
		BYTE res1:2;
		BYTE ATC1_Drive_br_setp_valid:1; //牵引/制动参考值有效
		BYTE ATC1_Open_doors_side_A:1;
		BYTE ATC1_Open_doors_side_B:1;
		BYTE ATC1_Close_doors_side_A:1;
		BYTE ATC1_Close_doors_side_B:1;
		BYTE res2:1;
	}door_info_t;
	BYTE res3[2];
	BYTE ATC1_Drive_brake_setpoint_h;
	BYTE ATC1_Drive_brake_setpoint_l;
	BYTE res4[2];
#else
	struct
	{
		BYTE res1:2;
		BYTE ATC1_Drive_br_setp_valid:1; //牵引/制动参考值有效
		BYTE ATC1_Open_doors_side_A:1;
		BYTE ATC1_Open_doors_side_B:1;
		BYTE ATC1_Close_doors_side_A:1;
		BYTE ATC1_Close_doors_side_B:1;
		BYTE res2:1;
	}door_info_t;	//1
	struct 
	{
		BYTE ATC1_ATO_active:1; //ATO模式激活
		BYTE ATC1_ATB_mode:1;
		BYTE ATC1_Prot_Manual_mode:1;
		BYTE ATC1_Rest_Manual_mode:1;
		BYTE ATC1_ATP_Reverse_mode:1;
		BYTE ATC1_Drive_command:1;
		BYTE ATC1_Brake_command:1;
		BYTE ATC1_Emergency_brake:1;
	}mode_info_t;	//2
	BYTE res3[2];	//4
	BYTE ATC1_Drive_brake_setpoint_l;	//5
	BYTE ATC1_Drive_brake_setpoint_h;	//6
	BYTE res4[2];	//8
#endif
}ATC_1F1H_T;	//ATC快速

typedef struct
{
#if BIG_ENDIAN_1F2H
	BYTE ATC1_Time_year;
	BYTE ATC1_Time_Mon;	//ATC时钟-月
	BYTE ATC1_Time_Day;
	BYTE ATC1_Time_Hour;
	BYTE ATC1_Time_Min;
	BYTE ATC1_Time_Sec;
	struct
	{
		BYTE ATC1_Open_B_side_next:1;//下一站将打开B侧车门
		BYTE ATC1_Open_A_side_next:1;
		BYTE res1:6;
	}open_info_t;
	BYTE res2;
	BYTE ATC1_Train_number_h;
	BYTE ATC1_Train_number_l;
	BYTE ATC1_Current_velocity_h;
	BYTE ATC1_Current_velocity_l;
	BYTE ATC1_End_station_ID_h;
	BYTE ATC1_End_station_ID_l;
	BYTE ATC1_Current_station_ID_h;
	BYTE ATC1_Current_station_ID_l;
	BYTE ATC1_Next_station_ID_h;
	BYTE ATC1_Next_station_ID_l;
	BYTE ATC1_Location_info_h;
	BYTE ATC1_Location_info_l;	//20
	BYTE res3[4];
	BYTE ATC1_Heartbeat_h;
	BYTE ATC1_Heartbeat_l;
	BYTE res4[6];
#else
	BYTE ATC1_Time_Mon;	//ATC时钟-月
	BYTE ATC1_Time_year;
	BYTE ATC1_Time_Hour;
	BYTE ATC1_Time_Day;
	BYTE ATC1_Time_Sec;
	BYTE ATC1_Time_Min;
	BYTE res2;	//7
	struct
	{
		BYTE ATC1_Open_B_side_next:1;//下一站将打开B侧车门
		BYTE ATC1_Open_A_side_next:1;
		BYTE res1:6;
	}open_info_t;	//8
	BYTE ATC1_Train_number_l;
	BYTE ATC1_Train_number_h;
	BYTE ATC1_Current_velocity_l;
	BYTE ATC1_Current_velocity_h;
	BYTE ATC1_End_station_ID_l;
	BYTE ATC1_End_station_ID_h;
	BYTE ATC1_Current_station_ID_l;
	BYTE ATC1_Current_station_ID_h;
	BYTE ATC1_Next_station_ID_l;
	BYTE ATC1_Next_station_ID_h;
	BYTE ATC1_Location_info_l;
	BYTE ATC1_Location_info_h;//20
	BYTE res3[4];
	BYTE ATC1_Heartbeat_l;
	BYTE ATC1_Heartbeat_h;
	BYTE res4[6];
#endif
}ATC_1F2H_T;	//ATC慢速

typedef struct
{
#if BIG_ENDIAN_80H
	struct
	{
		BYTE Cbk_Left_all_doors_closed:1;	//列车左门已关好
		BYTE Cbk_Right_all_doors_closed:1;
		BYTE ATO_ATP_mode_active:1;
		BYTE PM_mode_active:1;
		BYTE res1:2;
		BYTE Manual_station_ann_leave:1;
		BYTE Manual_station_ann_arrive:1;
	}cbk_info_t;
	struct
	{
		BYTE Cab_manned_Tc1:1;
		BYTE Cab_manned_Tc2:1;
		BYTE Electrical_coupled_Tc1:1;
		BYTE Electrical_coupled_Tc2:1;
		BYTE open_Left_door:1;
		BYTE open_right_door:1;
		BYTE close_Left_door:1;
		BYTE close_right_door:1;
	}cab_info_t;
	BYTE res2;
	struct
	{
		BYTE Emergency_announcement_touch:1;
		BYTE res3:1;
		BYTE Announcement_Mode_ATC:1;
		BYTE Announcement_Mode_TCMS:1;
		BYTE Jump_station_up:1;
		BYTE Jump_station_down:1;
		BYTE Route_map_open:1;
		BYTE Route_map_close:1;
	}Emergency_info_t;
	BYTE Emergency_announcement_ID_h;
	BYTE Emergency_announcement_ID_l;
	BYTE HMI_start_ID_h;
	BYTE HMI_start_ID_l;
	BYTE HMI_next_ID_h;
	BYTE HMI_next_ID_l;
	BYTE HMI_end_ID_h;
	BYTE HMI_end_ID_l;
	BYTE HMI_Line_ID_h;
	BYTE HMI_Line_ID_l;
	BYTE HMI_Route_ID_h;
	BYTE HMI_Route_ID_l;
	BYTE Ref_Velocity_h;
	BYTE Ref_Velocity_l;
	BYTE VCM_Life_h;
	BYTE VCM_Life_l;
	BYTE Car1_Temp_h;
	BYTE Car1_Temp_l;
	BYTE Car2_Temp_h;
	BYTE Car2_Temp_l;
	BYTE Car3_Temp_h;
	BYTE Car3_Temp_l;
	BYTE Car4_Temp_h;
	BYTE Car4_Temp_l;
	BYTE Car5_Temp_h;
	BYTE Car5_Temp_l;
	BYTE Car6_Temp_h;
	BYTE Car6_Temp_l;
#else
	struct
	{
		BYTE Cab_manned_Tc1			:	1;	//Tc1端司机室占有
		BYTE Cab_manned_Tc2			:	1;
		BYTE Electrical_coupled_Tc1	:	1;
		BYTE Electrical_coupled_Tc2	:	1;
		BYTE open_Left_door			:	1;
		BYTE open_right_door		:	1;
		BYTE close_Left_door		:	1;
		BYTE close_right_door		:	1;
	}cab_info_t;						//1
	struct
	{
		BYTE Cbk_Left_all_doors_closed		:	1;	//列车左门已关好
		BYTE Cbk_Right_all_doors_closed		:	1;
		BYTE ATO_ATP_mode_active			:	1;
		BYTE PM_mode_active					:	1;
		BYTE res1							:	2;
		BYTE Manual_station_ann_leave		:	1;
		BYTE Manual_station_ann_arrive		:	1;
	}cbk_info_t;					    //2
	struct
	{
		BYTE Emergency_announcement_touch	:	1;
		BYTE res3							:	1;
		BYTE Announcement_Mode_ATC			:	1;
		BYTE Announcement_Mode_TCMS			:	1;
		BYTE Jump_station_up				:	1;
		BYTE Jump_station_down				:	1;
		BYTE Route_map_open					:	1;
		BYTE Route_map_close				:	1;
	}Emergency_info_t;				   //3
	BYTE res2;						   //4
	BYTE Emergency_announcement_ID_l;
	BYTE Emergency_announcement_ID_h;
	BYTE HMI_start_ID_l;
	BYTE HMI_start_ID_h;
	BYTE HMI_next_ID_l;
	BYTE HMI_next_ID_h;				   //10
	BYTE HMI_end_ID_l;
	BYTE HMI_end_ID_h;
	BYTE HMI_Line_ID_l;
	BYTE HMI_Line_ID_h;
	BYTE HMI_Route_ID_l;
	BYTE HMI_Route_ID_h;
	BYTE Ref_Velocity_l;
	BYTE Ref_Velocity_h;			   //18
	BYTE VCM_Life_l;
	BYTE VCM_Life_h;
	BYTE Car1_Temp_l;
	BYTE Car1_Temp_h;
	BYTE Car2_Temp_l;
	BYTE Car2_Temp_h;
	BYTE Car3_Temp_l;
	BYTE Car3_Temp_h;				   //26
	BYTE Car4_Temp_l;
	BYTE Car4_Temp_h;
	BYTE Car5_Temp_l;
	BYTE Car5_Temp_h;
	BYTE Car6_Temp_l;
	BYTE Car6_Temp_h;				   //32
#endif
}VCM_PA_80H;

typedef struct
{
#if BIG_ENDIAN_81H
	struct
	{
		BYTE Fire_alarm_1_1:1;
		BYTE Fire_alarm_2_1:1;
		BYTE Fire_alarm_3_1:1;
		BYTE Fire_alarm_4_1:1;
		BYTE Fire_alarm_5_1:1;
		BYTE res1:3;
	}fire_alarm1_t;
	struct
	{
		BYTE Door_emergency_release_1_1:1;
		BYTE Door_emergency_release_2_1:1;
		BYTE Door_emergency_release_3_1:1;
		BYTE Door_emergency_release_4_1:1;
		BYTE Door_emergency_release_5_1:1;
		BYTE Door_emergency_release_6_1:1;
		BYTE Door_emergency_release_7_1:1;
		BYTE Door_emergency_release_8_1:1;
	}door_emergency1_t;
	struct
	{
		BYTE Fire_alarm_1_2:1;
		BYTE Fire_alarm_2_2:1;
		BYTE Fire_alarm_3_2:1;
		BYTE Fire_alarm_4_2:1;
		BYTE Fire_alarm_5_2:1;
		BYTE res2:3;
	}fire_alarm2_t;
	struct
	{
		BYTE Door_emergency_release_1_2:1;
		BYTE Door_emergency_release_2_2:1;
		BYTE Door_emergency_release_3_2:1;
		BYTE Door_emergency_release_4_2:1;
		BYTE Door_emergency_release_5_2:1;
		BYTE Door_emergency_release_6_2:1;
		BYTE Door_emergency_release_7_2:1;
		BYTE Door_emergency_release_8_2:1;
	}door_emergency2_t;
	struct
	{
		BYTE Fire_alarm_1_3:1;
		BYTE Fire_alarm_2_3:1;
		BYTE Fire_alarm_3_3:1;
		BYTE Fire_alarm_4_3:1;
		BYTE Fire_alarm_5_3:1;
		BYTE res3:3;
	}fire_alarm3_t;
	struct
	{
		BYTE Door_emergency_release_1_3:1;
		BYTE Door_emergency_release_2_3:1;
		BYTE Door_emergency_release_3_3:1;
		BYTE Door_emergency_release_4_3:1;
		BYTE Door_emergency_release_5_3:1;
		BYTE Door_emergency_release_6_3:1;
		BYTE Door_emergency_release_7_3:1;
		BYTE Door_emergency_release_8_3:1;
	}door_emergency3_t;
	struct
	{
		BYTE Fire_alarm_1_4:1;
		BYTE Fire_alarm_2_4:1;
		BYTE Fire_alarm_3_4:1;
		BYTE Fire_alarm_4_4:1;
		BYTE Fire_alarm_5_4:1;
		BYTE res1:4;
	}fire_alarm4_t;
	struct
	{
		BYTE Door_emergency_release_1_4:1;
		BYTE Door_emergency_release_2_4:1;
		BYTE Door_emergency_release_3_4:1;
		BYTE Door_emergency_release_4_4:1;
		BYTE Door_emergency_release_5_4:1;
		BYTE Door_emergency_release_6_4:1;
		BYTE Door_emergency_release_7_4:1;
		BYTE Door_emergency_release_8_4:1;
	}door_emergency4_t;
	struct
	{
		BYTE Fire_alarm_1_5:1;
		BYTE Fire_alarm_2_5:1;
		BYTE Fire_alarm_3_5:1;
		BYTE Fire_alarm_4_5:1;
		BYTE Fire_alarm_5_5:1;
		BYTE res5:3;
	}fire_alarm5_t;
	struct
	{
		BYTE Door_emergency_release_1_5:1;
		BYTE Door_emergency_release_2_5:1;
		BYTE Door_emergency_release_3_5:1;
		BYTE Door_emergency_release_4_5:1;
		BYTE Door_emergency_release_5_5:1;
		BYTE Door_emergency_release_6_5:1;
		BYTE Door_emergency_release_7_5:1;
		BYTE Door_emergency_release_8_5:1;
	}door_emergency5_t;
	struct
	{
		BYTE Fire_alarm_1_6:1;
		BYTE Fire_alarm_2_6:1;
		BYTE Fire_alarm_3_6:1;
		BYTE Fire_alarm_4_6:1;
		BYTE Fire_alarm_5_6:1;
		BYTE res6:3;
	}fire_alarm6_t;
	struct
	{
		BYTE Door_emergency_release_1_6:1;
		BYTE Door_emergency_release_2_6:1;
		BYTE Door_emergency_release_3_6:1;
		BYTE Door_emergency_release_4_6:1;
		BYTE Door_emergency_release_5_6:1;
		BYTE Door_emergency_release_6_6:1;
		BYTE Door_emergency_release_7_6:1;
		BYTE Door_emergency_release_8_6:1;
	}door_emergency6_t;
	BYTE Media_host_volume_h;	//媒体主机音量
	BYTE Media_host_volume_l;
	BYTE res9[2];
#else
	struct
	{
		BYTE Door_emergency_release_1_1:1;	//1车1号门紧急解锁
		BYTE Door_emergency_release_2_1:1;
		BYTE Door_emergency_release_3_1:1;
		BYTE Door_emergency_release_4_1:1;
		BYTE Door_emergency_release_5_1:1;
		BYTE Door_emergency_release_6_1:1;
		BYTE Door_emergency_release_7_1:1;
		BYTE Door_emergency_release_8_1:1;
	}door_emergency1_t;	//1
	struct
	{
		BYTE Fire_alarm_1_1:1;
		BYTE Fire_alarm_2_1:1;
		BYTE Fire_alarm_3_1:1;
		BYTE Fire_alarm_4_1:1;
		BYTE Fire_alarm_5_1:1;
		BYTE res1:3;
	}fire_alarm1_t;	//2
	struct
	{
		BYTE Door_emergency_release_1_2:1;
		BYTE Door_emergency_release_2_2:1;
		BYTE Door_emergency_release_3_2:1;
		BYTE Door_emergency_release_4_2:1;
		BYTE Door_emergency_release_5_2:1;
		BYTE Door_emergency_release_6_2:1;
		BYTE Door_emergency_release_7_2:1;
		BYTE Door_emergency_release_8_2:1;
	}door_emergency2_t;
	struct
	{
		BYTE Fire_alarm_1_2:1;
		BYTE Fire_alarm_2_2:1;
		BYTE Fire_alarm_3_2:1;
		BYTE Fire_alarm_4_2:1;
		BYTE Fire_alarm_5_2:1;
		BYTE res2:3;
	}fire_alarm2_t;	//4
	struct
	{
		BYTE Door_emergency_release_1_3:1;
		BYTE Door_emergency_release_2_3:1;
		BYTE Door_emergency_release_3_3:1;
		BYTE Door_emergency_release_4_3:1;
		BYTE Door_emergency_release_5_3:1;
		BYTE Door_emergency_release_6_3:1;
		BYTE Door_emergency_release_7_3:1;
		BYTE Door_emergency_release_8_3:1;
	}door_emergency3_t;
	struct
	{
		BYTE Fire_alarm_1_3:1;
		BYTE Fire_alarm_2_3:1;
		BYTE Fire_alarm_3_3:1;
		BYTE Fire_alarm_4_3:1;
		BYTE Fire_alarm_5_3:1;
		BYTE res3:3;
	}fire_alarm3_t;	//6
	struct
	{
		BYTE Door_emergency_release_1_4:1;
		BYTE Door_emergency_release_2_4:1;
		BYTE Door_emergency_release_3_4:1;
		BYTE Door_emergency_release_4_4:1;
		BYTE Door_emergency_release_5_4:1;
		BYTE Door_emergency_release_6_4:1;
		BYTE Door_emergency_release_7_4:1;
		BYTE Door_emergency_release_8_4:1;
	}door_emergency4_t;
	struct
	{
		BYTE Fire_alarm_1_4:1;
		BYTE Fire_alarm_2_4:1;
		BYTE Fire_alarm_3_4:1;
		BYTE Fire_alarm_4_4:1;
		BYTE Fire_alarm_5_4:1;
		BYTE res1:3;
	}fire_alarm4_t;	//8
	struct
	{
		BYTE Door_emergency_release_1_5:1;
		BYTE Door_emergency_release_2_5:1;
		BYTE Door_emergency_release_3_5:1;
		BYTE Door_emergency_release_4_5:1;
		BYTE Door_emergency_release_5_5:1;
		BYTE Door_emergency_release_6_5:1;
		BYTE Door_emergency_release_7_5:1;
		BYTE Door_emergency_release_8_5:1;
	}door_emergency5_t;
	struct
	{
		BYTE Fire_alarm_1_5:1;
		BYTE Fire_alarm_2_5:1;
		BYTE Fire_alarm_3_5:1;
		BYTE Fire_alarm_4_5:1;
		BYTE Fire_alarm_5_5:1;
		BYTE res5:3;
	}fire_alarm5_t;	//10
	struct
	{
		BYTE Door_emergency_release_1_6:1;
		BYTE Door_emergency_release_2_6:1;
		BYTE Door_emergency_release_3_6:1;
		BYTE Door_emergency_release_4_6:1;
		BYTE Door_emergency_release_5_6:1;
		BYTE Door_emergency_release_6_6:1;
		BYTE Door_emergency_release_7_6:1;
		BYTE Door_emergency_release_8_6:1;
	}door_emergency6_t;
	struct
	{
		BYTE Fire_alarm_1_6:1;
		BYTE Fire_alarm_2_6:1;
		BYTE Fire_alarm_3_6:1;
		BYTE Fire_alarm_4_6:1;
		BYTE Fire_alarm_5_6:1;
		BYTE res6:3;
	}fire_alarm6_t;	//12
	BYTE Media_host_volume_l;	//媒体主机音量
	BYTE Media_host_volume_h;
	BYTE res9[2];	//16
#endif
}VCM_TV_81H;

typedef struct 
{ 
	BYTE fram_begin;   								// 0  0xFE

	/******车号端口 F1H  Len:8******/
	CAR_ID_F1H_T car_id_t;		   					// 1-8

	/******时间 FFH  Len:8******/
	TIME_FFH_T time_t; 								// 9-16

	/******ATC1故障数据 1F0H  Len:8******/			// 17-24
	ATC_1F0H_T atc_t;

	/******ATC1快速状态数据 1F1H  Len:8******/		// 25-32
	ATC_1F1H_T atc_t2;

	/******ATC1慢速状态数据 1F2H  Len:32******/		// 33-64
	ATC_1F2H_T atc_t3;

	/******VCM发给PIS_PA控制端口 80H  Len:32******/	// 65-96
	VCM_PA_80H vcm_t;

	/******VCM发给PIS_TV控制端口 81H  Len:16******/	// 97-112
	VCM_TV_81H vcm_t2;

	BYTE checksum;    								//  113
	BYTE fram_end;   								//  114  0xFF	
}tms_packet_t;


/*****************************PIS协议**************************************/
#define BIG_ENDAAN_180H 0
#define BIG_ENDIAN_181H 0 
#define BIG_ENDIAN_182H 0 

typedef struct
{
#if BIG_ENDAAN_180H
	BYTE res1;
	struct
	{
		BYTE Serious_fault:1;
		BYTE General_failure:1;
		BYTE Minor_fault:1;
		BYTE res2:5;
	}fault_level_t;
	struct
	{
		BYTE ACSU1:1;
		BYTE ACSU6:1;
		BYTE PACU1:1;
		BYTE PACU2:1;
		BYTE PACU3:1;
		BYTE PACU4:1;
		BYTE PACU5:1;
		BYTE PACU6:1;
	}comm_failt_t;
	struct
	{
		BYTE AC_1:1;
		BYTE AC_6:1;
		BYTE PA_1:1;
		BYTE PA_2:1;
		BYTE PA_3:1;
		BYTE PA_4:1;
		BYTE PA_5:1;
		BYTE PA_6:1;
	}cu_failt_t;
	struct
	{
		BYTE PECU_comm_1_3:1;	//3车PECU1通讯错误
		BYTE PECU_comm_2_3:1;
		BYTE PECU_comm_3_3:1;
		BYTE PECU_comm_4_3:1;
		BYTE res3:4;
	}car3_comm_error_t;
	struct
	{
		BYTE PECU_comm_1_1:1;
		BYTE PECU_comm_2_1:1;
		BYTE PECU_comm_3_1:1;
		BYTE PECU_comm_4_1:1;
		BYTE PECU_comm_1_2:1;
		BYTE PECU_comm_2_2:1;
		BYTE PECU_comm_3_2:1;
		BYTE PECU_comm_4_2:1;
	}car1_2_comm_error_t;
	struct
	{
		BYTE PECU_comm_1_4:1;	//4车PECU1通讯错误
		BYTE PECU_comm_2_4:1;
		BYTE PECU_comm_3_4:1;
		BYTE PECU_comm_4_4:1;
		BYTE res4:4;
	}car4_comm_error_t;
	struct
	{
		BYTE PECU_comm_1_6:1;
		BYTE PECU_comm_2_6:1;
		BYTE PECU_comm_3_6:1;
		BYTE PECU_comm_4_6:1;
		BYTE PECU_comm_1_5:1;
		BYTE PECU_comm_2_5:1;
		BYTE PECU_comm_3_5:1;
		BYTE PECU_comm_4_5:1;
	}car5_6_comm_error_t;
	BYTE res5[2];
	struct
	{
		BYTE res6[5];
		BYTE auto_mode:1;
		BYTE semi_auto_mode:1;
		BYTE manual_mode:1;
	}select_mode_t;
	struct
	{
		BYTE CRCK_Master:1;
		BYTE CRCK_Slave:1;
		BYTE PA_active:1;
		BYTE Cab_to_PA:1;
		BYTE Radio_to_PA:1;
		BYTE Cab_to_Cab:1;
		BYTE Drive_to_PECU:1;
		BYTE Emc_announce_activated:1;
	}acsu_info_t;
	struct
	{
		BYTE PECU_call1_3:1;	//3车PECU1呼叫
		BYTE PECU_call2_3:1;
		BYTE PECU_call3_3:1;
		BYTE PECU_call4_3:1;
		BYTE res7:4;
	}car3_pecu_call_t;
	struct
	{
		BYTE PECU_call1_1:1;
		BYTE PECU_call2_1:1;
		BYTE PECU_call3_1:1;
		BYTE PECU_call4_1:1;
		BYTE PECU_call1_2:1;
		BYTE PECU_call2_2:1;
		BYTE PECU_call3_2:1;
		BYTE PECU_call4_2:1;
	}car1_2_pecu_call_t;
	struct
	{
		BYTE PECU_call1_4:1;	//4车PECU1呼叫
		BYTE PECU_call2_4:1;
		BYTE PECU_call3_4:1;
		BYTE PECU_call4_4:1;
		BYTE res8:4;
	}car4_pecu_call_t;
	struct
	{
		BYTE PECU_call1_6:1;
		BYTE PECU_call2_6:1;
		BYTE PECU_call3_6:1;
		BYTE PECU_call4_6:1;
		BYTE PECU_call1_5:1;
		BYTE PECU_call2_5:1;
		BYTE PECU_call3_5:1;
		BYTE PECU_call4_5:1;
	}car5_6_pecu_call_t;
	struct
	{
		BYTE PECU_act1_3:1;
		BYTE PECU_act2_3:1;
		BYTE PECU_act3_3:1;
		BYTE PECU_act4_3:1;
		BYTE res9:4;
	}car3_pecu_act_t;
	struct
	{
		BYTE PECU_act1_1:1;
		BYTE PECU_act2_1:1;
		BYTE PECU_act3_1:1;
		BYTE PECU_act4_1:1;
		BYTE PECU_act1_2:1;
		BYTE PECU_act2_2:1;
		BYTE PECU_act3_2:1;
		BYTE PECU_act4_2:1;
	}car1_2_pecu_act_t;
	struct
	{
		BYTE PECU_act1_4:1;
		BYTE PECU_act2_4:1;
		BYTE PECU_act3_4:1;
		BYTE PECU_act4_4:1;
		BYTE res10:4;
	}car4_pecu_act_t;
	struct
	{
		BYTE PECU_act1_5:1;
		BYTE PECU_act2_5:1;
		BYTE PECU_act3_5:1;
		BYTE PECU_act4_5:1;
		BYTE PECU_act1_6:1;
		BYTE PECU_act2_6:1;
		BYTE PECU_act3_6:1;
		BYTE PECU_act4_6:1;
	}car5_6_pecu_act_t;
	BYTE Cbk_start_station_ID_h;
	BYTE Cbk_start_station_ID_l;
	BYTE Cbk_end_Station_ID_h;
	BYTE Cbk_end_Station_ID_l;
	BYTE Cbk_current_station_ID_h;
	BYTE Cbk_current_station_ID_l;
	BYTE Cbk_line_ID_h;
	BYTE Cbk_line_ID_l;
	BYTE Cbk_route_ID_h;
	BYTE Cbk_route_ID_l;
	BYTE Heartbeat_h;
	BYTE Heartbeat_l;	//32
#else
	struct
	{
		BYTE Serious_fault:1;
		BYTE General_failure:1;
		BYTE Minor_fault:1;
		BYTE res2:5;
	}fault_level_t;	//1
	BYTE res1;
	struct
	{
		BYTE AC_1:1;		//总的ACSU
		BYTE AC_6:1;
		BYTE PA_1:1;		//总的PACU
		BYTE PA_2:1;
		BYTE PA_3:1;
		BYTE PA_4:1;
		BYTE PA_5:1;
		BYTE PA_6:1;
	}cu_failt_t;	//3
	struct
	{
		BYTE ACSU1:1;		//总的ACSU通信故障
		BYTE ACSU6:1;
		BYTE PACU1:1;		//总的PACU通信故障
		BYTE PACU2:1;
		BYTE PACU3:1;
		BYTE PACU4:1;
		BYTE PACU5:1;
		BYTE PACU6:1;
	}comm_failt_t;
	struct
	{
		BYTE PECU_comm_1_1:1;
		BYTE PECU_comm_2_1:1;
		BYTE PECU_comm_3_1:1;
		BYTE PECU_comm_4_1:1;
		BYTE PECU_comm_1_2:1;
		BYTE PECU_comm_2_2:1;
		BYTE PECU_comm_3_2:1;
		BYTE PECU_comm_4_2:1;
	}car1_2_comm_error_t;	//5
	struct
	{
		BYTE PECU_comm_1_3:1;	//3车PECU1通讯错误
		BYTE PECU_comm_2_3:1;
		BYTE PECU_comm_3_3:1;
		BYTE PECU_comm_4_3:1;
		BYTE res3:4;
	}car3_comm_error_t;
	struct
	{
		BYTE PECU_comm_1_6:1;
		BYTE PECU_comm_2_6:1;
		BYTE PECU_comm_3_6:1;
		BYTE PECU_comm_4_6:1;
		BYTE PECU_comm_1_5:1;
		BYTE PECU_comm_2_5:1;
		BYTE PECU_comm_3_5:1;
		BYTE PECU_comm_4_5:1;
	}car5_6_comm_error_t;	//7
	struct
	{
		BYTE PECU_comm_1_4:1;	//4车PECU1通讯错误
		BYTE PECU_comm_2_4:1;
		BYTE PECU_comm_3_4:1;
		BYTE PECU_comm_4_4:1;
		BYTE res4:4;
	}car4_comm_error_t;
	BYTE res5[2];	//10
	struct
	{
		BYTE CRCK_Master	:1;	//ACSU主控
		BYTE CRCK_Slave		:1;	//ACSU副控
		BYTE PA_active		:1; 
		BYTE Cab_to_PA		:1;	//人工
		BYTE Radio_to_PA	:1;	//OCC
		BYTE Cab_to_Cab		:1;
		BYTE Drive_to_PECU	:1; //紧急对讲
		BYTE Emc_announce_activated:1;//紧急对讲激活
	}acsu_info_t;	//11
	struct
	{
		BYTE res6			:5;
		BYTE auto_mode		:1;		//自动模式
		BYTE semi_auto_mode	:1;		//半自动模式
		BYTE manual_mode	:1;		//手动模式
	}select_mode_t;
	struct
	{
		BYTE PECU_call1_1:1;
		BYTE PECU_call2_1:1;
		BYTE PECU_call3_1:1;
		BYTE PECU_call4_1:1;
		BYTE PECU_call1_2:1;
		BYTE PECU_call2_2:1;
		BYTE PECU_call3_2:1;
		BYTE PECU_call4_2:1;
	}car1_2_pecu_call_t;	//13
	struct
	{
		BYTE PECU_call1_3:1;	//3车PECU1呼叫
		BYTE PECU_call2_3:1;
		BYTE PECU_call3_3:1;
		BYTE PECU_call4_3:1;
		BYTE res7:4;
	}car3_pecu_call_t;		//14
	struct
	{
		BYTE PECU_call1_6:1;
		BYTE PECU_call2_6:1;
		BYTE PECU_call3_6:1;
		BYTE PECU_call4_6:1;
		BYTE PECU_call1_5:1;
		BYTE PECU_call2_5:1;
		BYTE PECU_call3_5:1;
		BYTE PECU_call4_5:1;
	}car5_6_pecu_call_t;	//15
	struct
	{
		BYTE PECU_call1_4:1;	//4车PECU1呼叫
		BYTE PECU_call2_4:1;
		BYTE PECU_call3_4:1;
		BYTE PECU_call4_4:1;
		BYTE res8:4;
	}car4_pecu_call_t;	//16
	struct
	{
		BYTE PECU_act1_1:1;
		BYTE PECU_act2_1:1;
		BYTE PECU_act3_1:1;
		BYTE PECU_act4_1:1;
		BYTE PECU_act1_2:1;
		BYTE PECU_act2_2:1;
		BYTE PECU_act3_2:1;
		BYTE PECU_act4_2:1;
	}car1_2_pecu_act_t;
	struct
	{
		BYTE PECU_act1_3:1;
		BYTE PECU_act2_3:1;
		BYTE PECU_act3_3:1;
		BYTE PECU_act4_3:1;
		BYTE res9:4;
	}car3_pecu_act_t;	//18
	struct
	{
		BYTE PECU_act1_6:1;
		BYTE PECU_act2_6:1;
		BYTE PECU_act3_6:1;
		BYTE PECU_act4_6:1;
		BYTE PECU_act1_5:1;
		BYTE PECU_act2_5:1;
		BYTE PECU_act3_5:1;
		BYTE PECU_act4_5:1;
	}car6_5_pecu_act_t;
	struct
	{
		BYTE PECU_act1_4:1;
		BYTE PECU_act2_4:1;
		BYTE PECU_act3_4:1;
		BYTE PECU_act4_4:1;
		BYTE res10:4;
	}car4_pecu_act_t;				//20

	BYTE Cbk_start_station_ID_l;
	BYTE Cbk_start_station_ID_h;
	BYTE Cbk_end_Station_ID_l;
	BYTE Cbk_end_Station_ID_h;
	BYTE Cbk_current_station_ID_l;
	BYTE Cbk_current_station_ID_h;	//26
	BYTE Cbk_line_ID_l;
	BYTE Cbk_line_ID_h;
	BYTE Cbk_route_ID_l;
	BYTE Cbk_route_ID_h;
	BYTE Heartbeat_l;
	BYTE Heartbeat_h;				//32
#endif
}PA_DIAGNOSE_STATUS_180H_T;

typedef struct
{
#if BIG_ENDIAN_181H
	BYTE acsu1_version_h;   // 1 
	BYTE acsu1_version_l;   // 2 

	BYTE acsu6_version_h;   // 3 
	BYTE acsu6_version_l;   // 4 

	BYTE pacu1_version_h;   // 5
	BYTE pacu1_version_l;   // 6 

	BYTE pacu2_version_h;   // 7
	BYTE pacu2_version_l;   // 8 

	BYTE pacu3_version_h;   // 9 
	BYTE pacu3_version_l;   // 10 

	BYTE pacu4_version_h;   // 11
	BYTE pacu4_version_l;   // 12 

	BYTE pacu5_version_h;   // 13 
	BYTE pacu5_version_l;   // 14 

	BYTE pacu6_version_h;   // 15 
	BYTE pacu6_version_l;   // 16 

	BYTE reserve1[16];      // 17-32 
#else 
	BYTE acsu1_version_l;   // 2 
	BYTE acsu1_version_h;   // 1 

	BYTE acsu6_version_l;   // 4 
	BYTE acsu6_version_h;   // 3 

	BYTE pacu1_version_l;   // 6 
	BYTE pacu1_version_h;   // 5

	BYTE pacu2_version_l;   // 8 
	BYTE pacu2_version_h;   // 7

	BYTE pacu3_version_l;   // 10 
	BYTE pacu3_version_h;   // 9 

	BYTE pacu4_version_l;   // 12 
	BYTE pacu4_version_h;   // 11

	BYTE pacu5_version_l;   // 14 
	BYTE pacu5_version_h;   // 13 

	BYTE pacu6_version_l;   // 16 
	BYTE pacu6_version_h;   // 15 

	BYTE reserve1[16];      // 17-32 
#endif
}PA_VER_181H_T;

typedef struct
{
#if BIG_ENDIAN_182H
	BYTE res1;
	struct
	{
		BYTE tc1_monitoring_fault:1;
		BYTE tc2_monitoring_fault:1;
		BYTE tc1_media_fault:1;
		BYTE tc2_media_fault:1;
		BYTE tc1_ipc_fault:1;
		BYTE tc2_ipc_fault:1;
		BYTE res2:2;
	}tc_fault_t;
	BYTE res3;
	struct
	{
		BYTE res4:2;
		BYTE a1_ipc1_fault:1;
		BYTE a1_ipc2_fault:1;
		BYTE a1_ipc3_fault:1;
		BYTE b1_ipc1_fault:1;
		BYTE b1_ipc2_fault:1;
		BYTE b1_ipc3_fault:1;
	}a1_b1_ipc_fault_t;
	BYTE res5;
	struct
	{
		BYTE res6:2;
		BYTE c1_ipc1_fault:1;
		BYTE c1_ipc2_fault:1;
		BYTE c1_ipc3_fault:1;
		BYTE c2_ipc1_fault:1;
		BYTE c2_ipc2_fault:1;
		BYTE c2_ipc3_fault:1;
	}c1_c2_ipc_fault_t;
	BYTE res7;
	struct
	{
		BYTE res8:2;
		BYTE b2_ipc1_fault:1;
		BYTE b2_ipc2_fault:1;
		BYTE b2_ipc3_fault:1;
		BYTE a2_ipc1_fault:1;
		BYTE a2_ipc2_fault:1;
		BYTE a2_ipc3_fault:1;
	}b2_a2_ipc_fault_t;
	BYTE monitoring_sw_ver_2;
	BYTE monitoring_sw_ver_1;
	BYTE res9;
	BYTE monitoring_sw_ver_3;
	BYTE media_sw_ver_2;
	BYTE media_sw_ver_1;
	BYTE media_volume;
	BYTE media_sw_ver_3;
#else
	struct
	{
		BYTE tc1_monitoring_fault:1;
		BYTE tc2_monitoring_fault:1;
		BYTE tc1_media_fault:1;
		BYTE tc2_media_fault:1;
		BYTE tc1_ipc_fault:1;
		BYTE tc2_ipc_fault:1;
		BYTE res2:2;
	}tc_fault_t;	//1
	BYTE res1;	//2
	struct
	{
		BYTE res4:2;
		BYTE a1_ipc1_fault:1;
		BYTE a1_ipc2_fault:1;
		BYTE a1_ipc3_fault:1;
		BYTE b1_ipc1_fault:1;
		BYTE b1_ipc2_fault:1;
		BYTE b1_ipc3_fault:1;
	}a1_b1_ipc_fault_t;	//3
	BYTE res3;	//4
	struct
	{
		BYTE res6:2;
		BYTE c1_ipc1_fault:1;
		BYTE c1_ipc2_fault:1;
		BYTE c1_ipc3_fault:1;
		BYTE c2_ipc1_fault:1;
		BYTE c2_ipc2_fault:1;
		BYTE c2_ipc3_fault:1;
	}c1_c2_ipc_fault_t;	//5
	BYTE res5;
	struct
	{
		BYTE res8:2;
		BYTE b2_ipc1_fault:1;
		BYTE b2_ipc2_fault:1;
		BYTE b2_ipc3_fault:1;
		BYTE a2_ipc1_fault:1;
		BYTE a2_ipc2_fault:1;
		BYTE a2_ipc3_fault:1;
	}b2_a2_ipc_fault_t;	//7
	BYTE res7;	//8
	BYTE monitoring_sw_ver_1;
	BYTE monitoring_sw_ver_2;
	BYTE monitoring_sw_ver_3;
	BYTE res9;	//12
	BYTE media_sw_ver_1;
	BYTE media_sw_ver_2;
	BYTE media_sw_ver_3;
	BYTE media_volume;	//16
#endif
}TV_DIAGNOSE_VER_182H_T;

typedef struct 
{ 
	BYTE fram_begin;   // 0 0xFE 

	/******PA诊断和状态数据端口 180H  Len:32******/ 
	PA_DIAGNOSE_STATUS_180H_T pa_status_t;           // 1-32 

	/******PA版本数据端口 181H  Len:32******/ 
	PA_VER_181H_T pa_ver_t;							 // 33-64 

	/******TV诊断和版本数据端口 182H  Len:16******/ 
	TV_DIAGNOSE_VER_182H_T tv_ver_t;				 // 65-80

	BYTE checksum;									 //  81
	BYTE fram_end;									 //  82 0xFF    
}tms_recv_packet_t;


static void OnRefreshFeedbackPanel()
{
	char szCarTxt[24][32]={_T("严重故障"),_T("一般故障"),_T("轻微故障"),_T("监控软件版本号"),_T("媒体软件版本"),_T("媒体主机音量"),//6
		_T("TC1监控主机故障"),_T("TC1媒体主机故障"),_T("TC1司机摄像头故障"),_T("TC2监控主机故障"),_T("TC2媒体主机故障"),_T("TC2司机摄像头故障"),//6
		_T("ACSU主控"),_T("ACSU 副控"),_T("PA功能激活"),_T("人工广播"),_T("OCC 广播"),_T("司机对讲"),//6
		_T("紧急对讲"),_T("紧急对讲激活"),_T("全自动模式"),_T("半自动模式"),_T("手动模式")};//5

	char szAcsuTxt[6][32]={_T("1车ACSU故障"), _T("6车ACSU故障"), _T("ACSU1通讯故障"), _T("ACSU6通讯故障"), _T("ACSU1版本:"), _T("ACSU版本6:")};//6

	char szTitlTxt[6][32]={_T("A1 车"), _T("B1 车"), _T("C1 车"), _T("C2 车"), _T("B2 车"), _T("A2 车")};//6

	char szTxt[20][32]={_T("PACU故障"),//1
		_T("PACU通讯故障"),
		_T("摄像头1故障"), _T("摄像头2故障"), _T("摄像头3故障"), //4
		_T("PACU版本:"),//1
		_T("PECU通讯-1"), _T("PECU通讯-2"), _T("PECU通讯-3"),_T("PECU通讯-4"),//4
		_T("PECU呼叫-1"), _T("PECU呼叫-2"), _T("PECU呼叫-3"), _T("PECU呼叫-4"),//4
		_T("PECU激活-1"), _T("PECU激活-2"),_T("PECU激活-3"), _T("PECU激活-4")};//4

	//全车
	int index=0;
	for(int j=0;j<4;j++)
	{
		for(int i=0; i<6; i++)
		{
			if(3==j && 5==i)
			{
				BH_SetDrawLineText(i, j, _T(""));
				continue;
			}
			BH_SetDrawLineText(i,j,szCarTxt[index++]);
		}
	}
/*
	for (int i=0; i<6; i++)//6节车厢
	{
		//BH_SetDrawLineText(i, 4, _T(""));//空格

		BH_SetDrawLineText(i, 4, szTitlTxt[i]);
		for (int j=0; j<19; j++)
		{
			if(j == 0)
			{
				BH_SetDrawLineText(i, j+5, szTxt[j]);
			}
			else if(j == 1)
			{
				if (i == 0)
				{
					BH_SetDrawLineText(i, j+5, "ACSU1故障");
				}
				else if (i == 5)
				{
					BH_SetDrawLineText(i, j+5, "ACSU6故障");
				}
			}
			else
			{
				BH_SetDrawLineText(i, j+6, szTxt[j-1]);
				if ( (j==0 || j==6) && !(i==0 || i==5) )
				{
					BH_SetDrawLineText(i, j+6, _T(""));
				}
			}
		
		}	
	}
*/
	for (int i=0; i<6; i++)//6节车厢
	{
		BH_SetDrawLineText(i, 4, _T(""));
		BH_SetDrawLineText(i, 5, szAcsuTxt[i]);
		BH_SetDrawLineText(i, 6, szTitlTxt[i]);
		for (int j=0; j<18; j++)
		{
			BH_SetDrawLineText(i, j+7, szTxt[j]);
		}	
	}
}

static void OnRefreshState(tms_recv_packet_t stRecvpacket)
{
	BYTE bCarInfo[6][4];
	bCarInfo[0][0] = stRecvpacket.pa_status_t.fault_level_t.Serious_fault;	//严重故障
	bCarInfo[1][0] = stRecvpacket.pa_status_t.fault_level_t.General_failure;
	bCarInfo[2][0] = stRecvpacket.pa_status_t.fault_level_t.Minor_fault;
	bCarInfo[0][1] = stRecvpacket.tv_ver_t.tc_fault_t.tc1_monitoring_fault;	//TC1监控主机故障
	bCarInfo[1][1] = stRecvpacket.tv_ver_t.tc_fault_t.tc1_media_fault;
	bCarInfo[2][1] = stRecvpacket.tv_ver_t.tc_fault_t.tc1_ipc_fault;
	bCarInfo[3][1] = stRecvpacket.tv_ver_t.tc_fault_t.tc2_monitoring_fault;
	bCarInfo[4][1] = stRecvpacket.tv_ver_t.tc_fault_t.tc2_media_fault;
	bCarInfo[5][1] = stRecvpacket.tv_ver_t.tc_fault_t.tc2_ipc_fault;
	bCarInfo[0][2] = stRecvpacket.pa_status_t.acsu_info_t.CRCK_Master;		//ACSU主控
	bCarInfo[1][2] = stRecvpacket.pa_status_t.acsu_info_t.CRCK_Slave;
	bCarInfo[2][2] = stRecvpacket.pa_status_t.acsu_info_t.PA_active;
	bCarInfo[3][2] = stRecvpacket.pa_status_t.acsu_info_t.Cab_to_PA;
	bCarInfo[4][2] = stRecvpacket.pa_status_t.acsu_info_t.Radio_to_PA;
	bCarInfo[5][2] = stRecvpacket.pa_status_t.acsu_info_t.Cab_to_Cab;
	bCarInfo[0][3] = stRecvpacket.pa_status_t.acsu_info_t.Drive_to_PECU;	//紧急对讲
	bCarInfo[1][3] = stRecvpacket.pa_status_t.acsu_info_t.Emc_announce_activated;
	bCarInfo[2][3] = stRecvpacket.pa_status_t.select_mode_t.auto_mode;		//全自动模式
	bCarInfo[3][3] = stRecvpacket.pa_status_t.select_mode_t.semi_auto_mode;
	bCarInfo[4][3] = stRecvpacket.pa_status_t.select_mode_t.manual_mode;

	//列
	for (int i = 0; i < 6; i++)
	{
		//行
		for (int j = 0;j < 4; j++)
		{
			//[0,3] - [0,5] 第1行第4列以后不做判断
			if(i > 2 && 0 == j)
			{
				continue;
			}
			
			BH_SetCarVerticalState(i, j, bCarInfo[i][j] == 1 ? Enum_FAULT : Enum_Normal);//正常：故障
			
		}
	}

	//第六行
	WORD wAcsuVersion[2]={0};
	CString strVer1;
	wAcsuVersion[0] = stRecvpacket.pa_ver_t.acsu1_version_l | stRecvpacket.pa_ver_t.acsu1_version_h << 8;
	wAcsuVersion[1] = stRecvpacket.pa_ver_t.acsu6_version_l | stRecvpacket.pa_ver_t.acsu6_version_h << 8;

	//Modify by Johnny in 2017-08-02 20:06:05
	BH_SetCarVerticalState(0, 6, stRecvpacket.pa_status_t.cu_failt_t.AC_1   == 1 ? Enum_FAULT : Enum_Normal);//正常：故障
	BH_SetCarVerticalState(1, 6, stRecvpacket.pa_status_t.cu_failt_t.AC_6   == 1 ? Enum_FAULT : Enum_Normal);//正常：故障
	BH_SetCarVerticalState(2, 6, stRecvpacket.pa_status_t.comm_failt_t.ACSU1== 1 ? Enum_FAULT : Enum_Normal);//正常：故障
	BH_SetCarVerticalState(3, 6, stRecvpacket.pa_status_t.comm_failt_t.ACSU6== 1 ? Enum_FAULT : Enum_Normal);//正常：故障

	//版本
	strVer1.Format(_T("ACSU版本V%d"),wAcsuVersion[0]);
	BH_SetDrawLineText(4,6,strVer1.GetBuffer());
	strVer1.ReleaseBuffer();
	strVer1.Format(_T("ACSU版本V%d"),wAcsuVersion[1]);
	BH_SetDrawLineText(5,6,strVer1.GetBuffer());
	strVer1.ReleaseBuffer();

	//7-25行
	BYTE bCarState[6][19];
	//a1
	bCarState[0][0]=stRecvpacket.pa_status_t.cu_failt_t.PA_1;					//1车ACSU故障
	bCarState[0][1]=stRecvpacket.pa_status_t.comm_failt_t.PACU1;				//1车PACU故障
	bCarState[0][2]=stRecvpacket.tv_ver_t.a1_b1_ipc_fault_t.a1_ipc1_fault;		//摄像头1故障
	bCarState[0][3]=stRecvpacket.tv_ver_t.a1_b1_ipc_fault_t.a1_ipc2_fault;
	bCarState[0][4]=stRecvpacket.tv_ver_t.a1_b1_ipc_fault_t.a1_ipc3_fault;
	bCarState[0][5]=stRecvpacket.pa_status_t.car1_2_comm_error_t.PECU_comm_1_1;	//1车PECU1通讯错误
	bCarState[0][6]=stRecvpacket.pa_status_t.car1_2_comm_error_t.PECU_comm_2_1;
	bCarState[0][7]=stRecvpacket.pa_status_t.car1_2_comm_error_t.PECU_comm_3_1;
	bCarState[0][8]=stRecvpacket.pa_status_t.car1_2_comm_error_t.PECU_comm_4_1;
	bCarState[0][9]=stRecvpacket.pa_status_t.car1_2_pecu_call_t.PECU_call1_1;	//1车PECU1呼叫
	bCarState[0][10]=stRecvpacket.pa_status_t.car1_2_pecu_call_t.PECU_call2_1;
	bCarState[0][11]=stRecvpacket.pa_status_t.car1_2_pecu_call_t.PECU_call3_1;
	bCarState[0][12]=stRecvpacket.pa_status_t.car1_2_pecu_call_t.PECU_call4_1;
	bCarState[0][13]=stRecvpacket.pa_status_t.car1_2_pecu_act_t.PECU_act1_1;	//1车PECU1激活
	bCarState[0][14]=stRecvpacket.pa_status_t.car1_2_pecu_act_t.PECU_act2_1;
	bCarState[0][15]=stRecvpacket.pa_status_t.car1_2_pecu_act_t.PECU_act3_1;
	bCarState[0][16]=stRecvpacket.pa_status_t.car1_2_pecu_act_t.PECU_act4_1;
	//b1
	bCarState[1][0]=stRecvpacket.pa_status_t.cu_failt_t.PA_2;
	bCarState[1][1]=stRecvpacket.pa_status_t.comm_failt_t.PACU2;
	bCarState[1][2]=stRecvpacket.tv_ver_t.a1_b1_ipc_fault_t.b1_ipc1_fault;
	bCarState[1][3]=stRecvpacket.tv_ver_t.a1_b1_ipc_fault_t.b1_ipc2_fault;
	bCarState[1][4]=stRecvpacket.tv_ver_t.a1_b1_ipc_fault_t.b1_ipc3_fault;
	bCarState[1][5]=stRecvpacket.pa_status_t.car1_2_comm_error_t.PECU_comm_1_2;
	bCarState[1][6]=stRecvpacket.pa_status_t.car1_2_comm_error_t.PECU_comm_2_2;
	bCarState[1][7]=stRecvpacket.pa_status_t.car1_2_comm_error_t.PECU_comm_3_2;
	bCarState[1][8]=stRecvpacket.pa_status_t.car1_2_comm_error_t.PECU_comm_4_2;
	bCarState[1][9]=stRecvpacket.pa_status_t.car1_2_pecu_call_t.PECU_call1_2;
	bCarState[1][10]=stRecvpacket.pa_status_t.car1_2_pecu_call_t.PECU_call2_2;
	bCarState[1][11]=stRecvpacket.pa_status_t.car1_2_pecu_call_t.PECU_call3_2;
	bCarState[1][12]=stRecvpacket.pa_status_t.car1_2_pecu_call_t.PECU_call4_2;
	bCarState[1][13]=stRecvpacket.pa_status_t.car1_2_pecu_act_t.PECU_act1_2;
	bCarState[1][14]=stRecvpacket.pa_status_t.car1_2_pecu_act_t.PECU_act2_2;
	bCarState[1][15]=stRecvpacket.pa_status_t.car1_2_pecu_act_t.PECU_act3_2;
	bCarState[1][16]=stRecvpacket.pa_status_t.car1_2_pecu_act_t.PECU_act4_2;
	//c1
	bCarState[2][0]=stRecvpacket.pa_status_t.cu_failt_t.PA_3;
	bCarState[2][1]=stRecvpacket.pa_status_t.comm_failt_t.PACU3;
	bCarState[2][2]=stRecvpacket.tv_ver_t.c1_c2_ipc_fault_t.c1_ipc1_fault;
	bCarState[2][3]=stRecvpacket.tv_ver_t.c1_c2_ipc_fault_t.c1_ipc2_fault;
	bCarState[2][4]=stRecvpacket.tv_ver_t.c1_c2_ipc_fault_t.c1_ipc3_fault;
	bCarState[2][5]=stRecvpacket.pa_status_t.car3_comm_error_t.PECU_comm_1_3;
	bCarState[2][6]=stRecvpacket.pa_status_t.car3_comm_error_t.PECU_comm_2_3;
	bCarState[2][7]=stRecvpacket.pa_status_t.car3_comm_error_t.PECU_comm_3_3;
	bCarState[2][8]=stRecvpacket.pa_status_t.car3_comm_error_t.PECU_comm_4_3;
	bCarState[2][9]=stRecvpacket.pa_status_t.car3_pecu_call_t.PECU_call1_3;
	bCarState[2][10]=stRecvpacket.pa_status_t.car3_pecu_call_t.PECU_call2_3;
	bCarState[2][11]=stRecvpacket.pa_status_t.car3_pecu_call_t.PECU_call3_3;
	bCarState[2][12]=stRecvpacket.pa_status_t.car3_pecu_call_t.PECU_call4_3;
	bCarState[2][13]=stRecvpacket.pa_status_t.car3_pecu_act_t.PECU_act1_3;
	bCarState[2][14]=stRecvpacket.pa_status_t.car3_pecu_act_t.PECU_act2_3;
	bCarState[2][15]=stRecvpacket.pa_status_t.car3_pecu_act_t.PECU_act3_3;
	bCarState[2][16]=stRecvpacket.pa_status_t.car3_pecu_act_t.PECU_act4_3;
	//c2
	bCarState[3][0]=stRecvpacket.pa_status_t.cu_failt_t.PA_4;
	bCarState[3][1]=stRecvpacket.pa_status_t.comm_failt_t.PACU4;
	bCarState[3][2]=stRecvpacket.tv_ver_t.c1_c2_ipc_fault_t.c2_ipc1_fault;
	bCarState[3][3]=stRecvpacket.tv_ver_t.c1_c2_ipc_fault_t.c2_ipc2_fault;
	bCarState[3][4]=stRecvpacket.tv_ver_t.c1_c2_ipc_fault_t.c2_ipc3_fault;
	bCarState[3][5]=stRecvpacket.pa_status_t.car4_comm_error_t.PECU_comm_1_4;
	bCarState[3][6]=stRecvpacket.pa_status_t.car4_comm_error_t.PECU_comm_2_4;
	bCarState[3][7]=stRecvpacket.pa_status_t.car4_comm_error_t.PECU_comm_3_4;
	bCarState[3][8]=stRecvpacket.pa_status_t.car4_comm_error_t.PECU_comm_4_4;
	bCarState[3][9]=stRecvpacket.pa_status_t.car4_pecu_call_t.PECU_call1_4;
	bCarState[3][10]=stRecvpacket.pa_status_t.car4_pecu_call_t.PECU_call2_4;
	bCarState[3][11]=stRecvpacket.pa_status_t.car4_pecu_call_t.PECU_call3_4;
	bCarState[3][12]=stRecvpacket.pa_status_t.car4_pecu_call_t.PECU_call4_4;
	bCarState[3][13]=stRecvpacket.pa_status_t.car4_pecu_act_t.PECU_act1_4;
	bCarState[3][14]=stRecvpacket.pa_status_t.car4_pecu_act_t.PECU_act2_4;
	bCarState[3][15]=stRecvpacket.pa_status_t.car4_pecu_act_t.PECU_act3_4;
	bCarState[3][16]=stRecvpacket.pa_status_t.car4_pecu_act_t.PECU_act4_4;
	//b2
	bCarState[4][0]=stRecvpacket.pa_status_t.cu_failt_t.PA_5;
	bCarState[4][1]=stRecvpacket.pa_status_t.comm_failt_t.PACU5;
	bCarState[4][2]=stRecvpacket.tv_ver_t.b2_a2_ipc_fault_t.b2_ipc1_fault;
	bCarState[4][3]=stRecvpacket.tv_ver_t.b2_a2_ipc_fault_t.b2_ipc2_fault;
	bCarState[4][4]=stRecvpacket.tv_ver_t.b2_a2_ipc_fault_t.b2_ipc3_fault;
	bCarState[4][5]=stRecvpacket.pa_status_t.car5_6_comm_error_t.PECU_comm_1_5;
	bCarState[4][6]=stRecvpacket.pa_status_t.car5_6_comm_error_t.PECU_comm_2_5;
	bCarState[4][7]=stRecvpacket.pa_status_t.car5_6_comm_error_t.PECU_comm_3_5;
	bCarState[4][8]=stRecvpacket.pa_status_t.car5_6_comm_error_t.PECU_comm_4_5;
	bCarState[4][9]=stRecvpacket.pa_status_t.car5_6_pecu_call_t.PECU_call1_5;
	bCarState[4][10]=stRecvpacket.pa_status_t.car5_6_pecu_call_t.PECU_call2_5;
	bCarState[4][11]=stRecvpacket.pa_status_t.car5_6_pecu_call_t.PECU_call3_5;
	bCarState[4][12]=stRecvpacket.pa_status_t.car5_6_pecu_call_t.PECU_call4_5;
	bCarState[4][13]=stRecvpacket.pa_status_t.car6_5_pecu_act_t.PECU_act1_5;
	bCarState[4][14]=stRecvpacket.pa_status_t.car6_5_pecu_act_t.PECU_act2_5;
	bCarState[4][15]=stRecvpacket.pa_status_t.car6_5_pecu_act_t.PECU_act3_5;
	bCarState[4][16]=stRecvpacket.pa_status_t.car6_5_pecu_act_t.PECU_act4_5;
	//a2
	bCarState[5][0]=stRecvpacket.pa_status_t.cu_failt_t.PA_6;
	bCarState[5][1]=stRecvpacket.pa_status_t.comm_failt_t.PACU6;
	bCarState[5][2]=stRecvpacket.tv_ver_t.b2_a2_ipc_fault_t.a2_ipc1_fault;
	bCarState[5][3]=stRecvpacket.tv_ver_t.b2_a2_ipc_fault_t.a2_ipc2_fault;
	bCarState[5][4]=stRecvpacket.tv_ver_t.b2_a2_ipc_fault_t.a2_ipc3_fault;
	bCarState[5][5]=stRecvpacket.pa_status_t.car5_6_comm_error_t.PECU_comm_1_6;
	bCarState[5][6]=stRecvpacket.pa_status_t.car5_6_comm_error_t.PECU_comm_2_6;
	bCarState[5][7]=stRecvpacket.pa_status_t.car5_6_comm_error_t.PECU_comm_3_6;
	bCarState[5][8]=stRecvpacket.pa_status_t.car5_6_comm_error_t.PECU_comm_4_6;
	bCarState[5][9]=stRecvpacket.pa_status_t.car5_6_pecu_call_t.PECU_call1_6;
	bCarState[5][10]=stRecvpacket.pa_status_t.car5_6_pecu_call_t.PECU_call2_6;
	bCarState[5][11]=stRecvpacket.pa_status_t.car5_6_pecu_call_t.PECU_call3_6;
	bCarState[5][12]=stRecvpacket.pa_status_t.car5_6_pecu_call_t.PECU_call4_6;
	bCarState[5][13]=stRecvpacket.pa_status_t.car6_5_pecu_act_t.PECU_act1_6;
	bCarState[5][14]=stRecvpacket.pa_status_t.car6_5_pecu_act_t.PECU_act2_6;
	bCarState[5][15]=stRecvpacket.pa_status_t.car6_5_pecu_act_t.PECU_act3_6;
	bCarState[5][16]=stRecvpacket.pa_status_t.car6_5_pecu_act_t.PECU_act4_6;


	//Add by Johnny in 2017-08-02 15:20:05
	int nStart = stRecvpacket.pa_status_t.Cbk_start_station_ID_l;
	int nEnd = stRecvpacket.pa_status_t.Cbk_end_Station_ID_l;
	int nCur = stRecvpacket.pa_status_t.Cbk_current_station_ID_l;
	
	//没有Nxt
	BH_Man_Auto_Set_Station(nCur, 1, nStart, nEnd);

	for (int i=0; i<6; i++)//6节车厢
	{
		for (int j=0;j<19; j++)
		{
			if (j<5)
			{
				BH_SetCarVerticalState(i, j+6, bCarState[i][j] == 1 ? Enum_FAULT : Enum_Normal);//正常：故障
			}
			else if (j>=5&&j<9)
			{
				BH_SetCarVerticalState(i, j+9, bCarState[i][j] == 1 ? Enum_PECU : Enum_Normal);//正常：通讯
			}
			else if(j>=9 && j<13)
			{
				BH_SetCarVerticalState(i, j+9, bCarState[i][j] == 1 ? Enum_CALL : Enum_Normal);//正常：呼叫
			}
			else if(j>=13)
			{
				BH_SetCarVerticalState(i, j+9, bCarState[i][j] == 1 ? Enum_ACTIVE : Enum_Normal);//正常：激活
			}
		}
	}
	//版本
	WORD wVersion[8]={0};
	CString strVer;
	wVersion[0]=stRecvpacket.pa_ver_t.acsu1_version_l | stRecvpacket.pa_ver_t.acsu1_version_h<<8;
	wVersion[1]=stRecvpacket.pa_ver_t.acsu6_version_l | stRecvpacket.pa_ver_t.acsu6_version_h<<8;
	wVersion[2]=stRecvpacket.pa_ver_t.pacu1_version_l | stRecvpacket.pa_ver_t.pacu1_version_h<<8;
	wVersion[3]=stRecvpacket.pa_ver_t.pacu2_version_l | stRecvpacket.pa_ver_t.pacu2_version_h<<8;
	wVersion[4]=stRecvpacket.pa_ver_t.pacu3_version_l | stRecvpacket.pa_ver_t.pacu3_version_h<<8;
	wVersion[5]=stRecvpacket.pa_ver_t.pacu4_version_l | stRecvpacket.pa_ver_t.pacu4_version_h<<8;
	wVersion[6]=stRecvpacket.pa_ver_t.pacu5_version_l | stRecvpacket.pa_ver_t.pacu5_version_h<<8;
	wVersion[7]=stRecvpacket.pa_ver_t.pacu6_version_l | stRecvpacket.pa_ver_t.pacu6_version_h<<8;
// 	strVer.Format(_T("ACSU版本V%d"),wVersion[0]);
// 	BH_SetDrawLineText(0,11,strVer.GetBuffer());
// 	strVer.ReleaseBuffer();
// 	strVer.Format(_T("ACSU版本V%d"),wVersion[1]);
// 	BH_SetDrawLineText(5,11,strVer.GetBuffer());
// 	strVer.ReleaseBuffer();
	strVer.Format(_T("PACU版本V%d"),wVersion[2]);
	BH_SetDrawLineText(0,12,strVer.GetBuffer());
	strVer.ReleaseBuffer();
	strVer.Format(_T("PACU版本V%d"),wVersion[3]);
	BH_SetDrawLineText(1,12,strVer.GetBuffer());
	strVer.ReleaseBuffer();
	strVer.Format(_T("PACU版本V%d"),wVersion[4]);
	BH_SetDrawLineText(2,12,strVer.GetBuffer());
	strVer.ReleaseBuffer();
	strVer.Format(_T("PACU版本V%d"),wVersion[5]);
	BH_SetDrawLineText(3,12,strVer.GetBuffer());
	strVer.ReleaseBuffer();
	strVer.Format(_T("PACU版本V%d"),wVersion[6]);
	BH_SetDrawLineText(4,12,strVer.GetBuffer());
	strVer.ReleaseBuffer();
	strVer.Format(_T("PACU版本V%d"),wVersion[7]);
	BH_SetDrawLineText(5,12,strVer.GetBuffer());
	strVer.ReleaseBuffer();

	//监控/媒体版本
	CString strMonitoring_sw_ver, strMedia_sw_ver,strMedia_volume;
	strMonitoring_sw_ver.Format(_T("监控软件版本V%d.%d.%d"),stRecvpacket.tv_ver_t.monitoring_sw_ver_1,stRecvpacket.tv_ver_t.monitoring_sw_ver_2,stRecvpacket.tv_ver_t.monitoring_sw_ver_3);
	strMedia_sw_ver.Format(_T("媒体软件版本V%d.%d.%d"),stRecvpacket.tv_ver_t.media_sw_ver_1,stRecvpacket.tv_ver_t.media_sw_ver_2,stRecvpacket.tv_ver_t.media_sw_ver_3);
	strMedia_volume.Format(_T("媒体主机音量:%d"),stRecvpacket.tv_ver_t.media_sw_ver_1,stRecvpacket.tv_ver_t.media_volume%100);
	BH_SetDrawLineText(3,0,strMonitoring_sw_ver.GetBuffer());
	strMonitoring_sw_ver.ReleaseBuffer();
	BH_SetDrawLineText(4,0,strMedia_sw_ver.GetBuffer());
	strMedia_sw_ver.ReleaseBuffer();
	BH_SetDrawLineText(5,0,strMedia_volume.GetBuffer());
	strMedia_volume.ReleaseBuffer();
}

static void OnDataViewDisPlay(DWORD dwItem, void *pView, void *pVoid)
{
#define _HighlightElement(a, b) for (int n = a; n <= b; n++) BH_SetElementHighlight(pView, n);
	CString strDisplay = _T("No item selected.");
	switch (dwItem)
	{
	case 0:
		_HighlightElement(0, 0);
		strDisplay.Format(_T("First %02X"), BH_GetViewDataByte(pView, 0));
		break;
	case 1:
	case 2:
	case 3:
	case 4: 
	case 5: 
	case 6: 
	case 7:	
	case 8:
		{
			_HighlightElement(1, 8);
			strDisplay.Format(_T("CAR_ID_1H_T %02X %02X %02X \n %02X %02X %02X %02X %02X"), 
				BH_GetViewDataByte(pView, 1), BH_GetViewDataByte(pView, 2), BH_GetViewDataByte(pView, 3), BH_GetViewDataByte(pView, 4),
				BH_GetViewDataByte(pView, 5), BH_GetViewDataByte(pView, 6), BH_GetViewDataByte(pView, 7), BH_GetViewDataByte(pView, 8));
		}
		break;
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		_HighlightElement(9, 14);//时间
#if BIG_ENDIAN_FH
		strDisplay.Format(_T("Date Time: %04d-%02d-%02d %02d:%02d:%02d"), BH_GetViewDataByte(pView, 13)+2000, BH_GetViewDataByte(pView, 14), BH_GetViewDataByte(pView, 15), 
			BH_GetViewDataByte(pView, 16),	BH_GetViewDataByte(pView, 17), BH_GetViewDataByte(pView, 18));
#else
		strDisplay.Format(_T("Date Time: %04d-%02d-%02d %02d:%02d:%02d"), BH_GetViewDataByte(pView, 10)+2000, BH_GetViewDataByte(pView, 9), BH_GetViewDataByte(pView, 12), 
			BH_GetViewDataByte(pView, 11),	BH_GetViewDataByte(pView, 14), BH_GetViewDataByte(pView, 13));
#endif
		break;
	case 15:
		//Add by Johnny in 2017-08-01 14:53:53
		_HighlightElement(15, 15);
		strDisplay.Format(_T("Time_Set_Flag: %d\n"), BH_GetViewDataByte(pView, 15));
		break;
	case 16:
	case 17:
		break;
	case 18:
		{
			_HighlightElement(18, 18);
			BYTE val = BH_GetViewDataByte(pView, 18);
			strDisplay.Format(_T("ATC1_Serious_fault: %d\n"), val&1);
			strDisplay.AppendFormat(_T("ATC1_Medium_fault: %d\n"), val>>1&1);
			strDisplay.AppendFormat(_T("CBTC_MODE: %d\n"),val>>3&1);
		}
		break;
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
		break;
	case 25:
		{
			_HighlightElement(25, 25);
			BYTE val = BH_GetViewDataByte(pView, 25);
			strDisplay.Format(_T("ATC1_Drive_br_setp_valid: %d\n"), val>>1&1);
			strDisplay.AppendFormat(_T("ATC1_Open_doors_side_A: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("ATC1_Open_doors_side_B: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("ATC1_Close_doors_side_A: %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("ATC1_Close_doors_side_B: %d \n"), val>>5&1);
		}
		break;
	case 26:
		{
			_HighlightElement(26, 26);
			BYTE val = BH_GetViewDataByte(pView, 26);
			strDisplay.Format(_T("ATC1_ATO_active: %d\n"), val&1);
			strDisplay.AppendFormat(_T("ATC1_ATB_mode: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("ATC1_Prot_Manual_mode: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("ATC1_Rest_Manual_mode: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("ATC1_ATP_Reverse_mode: %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("ATC1_Drive_command: %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("ATC1_Brake_command: %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("ATC1_Emergency_brake: %d \n"), val>>7&1);
		}
		break;
	case 27:
	case 28:
		break;
	case 29:
	case 30:
		{
			_HighlightElement(29, 30);
			strDisplay.Format(_T("ATC1_Drive_brake_setpoint: %d\n"), BH_GetViewDataByte(pView, 30)<<8|BH_GetViewDataByte(pView, 29));		
		}
		break;
	case 31:
	case 32:
		break;
	case 33://ATC慢速
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
		_HighlightElement(33, 38);
		strDisplay.Format(_T("ATC1_DateTime: %04d-%02d-%02d %02d:%02d:%02d"), BH_GetViewDataByte(pView, 34)+2000, BH_GetViewDataByte(pView, 33), BH_GetViewDataByte(pView, 36), 
			BH_GetViewDataByte(pView, 35),	BH_GetViewDataByte(pView, 38), BH_GetViewDataByte(pView, 37));
		break;
	case 39:
		break;
	case 40:
		{
			_HighlightElement(40, 40);
			BYTE val = BH_GetViewDataByte(pView, 40);
			strDisplay.Format(_T("ATC1_Open_B_side_next: %d\n"), val&1);
			strDisplay.AppendFormat(_T("ATC1_Open_A_side_next: %d \n"), val>>1&1);
		}
		break;
	case 41:
	case 42:
		_HighlightElement(41, 42);
		strDisplay.Format(_T("ATC1_Train_number: %d\n"), BH_GetViewDataByte(pView, 42)<<8|BH_GetViewDataByte(pView, 41));		
		break;
	case 43:
	case 44:
		_HighlightElement(43, 44);
		strDisplay.Format(_T("ATC1_Current_velocity: %d\n"), BH_GetViewDataByte(pView, 44)<<8|BH_GetViewDataByte(pView, 43));		
		break;
	case 45:
	case 46:
		_HighlightElement(45, 46);
		strDisplay.Format(_T("ATC1_End_station_ID: %d\n"), BH_GetViewDataByte(pView, 46)<<8|BH_GetViewDataByte(pView, 45));
		break;
	case 47:
	case 48:
		_HighlightElement(47, 48);
		strDisplay.Format(_T("ATC1_Current_station_ID: %d\n"), BH_GetViewDataByte(pView, 48)<<8|BH_GetViewDataByte(pView, 47));
		break;	
	case 49:		
	case 50:
		_HighlightElement(49, 50);
		strDisplay.Format(_T("ATC1_Next_station_ID: %d\n"), BH_GetViewDataByte(pView, 50)<<8|BH_GetViewDataByte(pView, 49));
		break;	
	case 51:
	case 52:
		_HighlightElement(51, 52);
		strDisplay.Format(_T("ATC1_Location_info: %d\n"), BH_GetViewDataByte(pView, 52)<<8|BH_GetViewDataByte(pView, 51));
		break;	
	case 53:
	case 54:
	case 55:
	case 56:
		break;
	case 57:
	case 58:
		_HighlightElement(57, 58);
		//Modify by Johnny in 2017-08-01 14:47:38
		//strDisplay.Format(_T("ATC1_Heartbeat: %d\n"), BH_GetViewDataByte(pView, 56)<<8|BH_GetViewDataByte(pView, 55));
		strDisplay.Format(_T("ATC1_Heartbeat: %d\n"), BH_GetViewDataByte(pView, 58)<<8|BH_GetViewDataByte(pView, 57));
		break;
	case 59:
	case 60:
	case 61:
	case 62:
	case 63:
	case 64:
		break;
	case 65://VCM->PIS_PA
		{
			_HighlightElement(65, 65);
			BYTE val = BH_GetViewDataByte(pView, 65);
			strDisplay.Format(_T("Cab_manned_Tc1: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Cab_manned_Tc2: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("Electrical_coupled_Tc1: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Electrical_coupled_Tc2: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("open_Left_door: %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("open_right_door: %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("close_Left_door: %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("close_right_door: %d \n"), val>>7&1);
		}
		break;
	case 66:
		{
			_HighlightElement(66, 66);
			BYTE val = BH_GetViewDataByte(pView, 66);
			strDisplay.Format(_T("Cbk_Left_all_doors_closed: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Cbk_Right_all_doors_closed: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("ATO/ATP_mode_active: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("PM_mode_active: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Manual_station_ann_leave: %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("Manual_station_ann_arrive: %d \n"), val>>7&1);
		}
		break;
	case 67:
		{
			_HighlightElement(67, 67);
			BYTE val = BH_GetViewDataByte(pView, 67);
			strDisplay.Format(_T("Emergency_announcement_touch: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Announcement_Mode_ATC: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Announcement_Mode_TCMS: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Jump_station_up: %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("Jump_station_down: %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("Route_map_open: %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("Route_map_close: %d \n"), val>>7&1);
		}
		break;
	case 68:
		break;
	case 69:
	case 70:
		_HighlightElement(69, 70);
		strDisplay.Format(_T("Emergency_announcement_ID: %d\n"), BH_GetViewDataByte(pView, 70)<<8|BH_GetViewDataByte(pView, 55));
		break;
	case 71:
	case 72:
		_HighlightElement(71, 72);
		//Modify by Johnny in 2017-08-01 14:43:56
		//strDisplay.Format(_T("HMI_start_ID: %d\n"), BH_GetViewDataByte(pView, 82)<<8|BH_GetViewDataByte(pView, 55));
		strDisplay.Format(_T("HMI_start_ID: %d\n"), BH_GetViewDataByte(pView, 72)<<8|BH_GetViewDataByte(pView, 71));
		break;
	case 73:
	case 74:
		_HighlightElement(73, 74);
		strDisplay.Format(_T("HMI_next_ID: %d\n"), BH_GetViewDataByte(pView, 74)<<8|BH_GetViewDataByte(pView, 73));
		break;
	case 75:
	case 76:
		_HighlightElement(75, 76);
		strDisplay.Format(_T("HMI_end_ID: %d\n"), BH_GetViewDataByte(pView, 76)<<8|BH_GetViewDataByte(pView, 75));
		break;
	case 77:
	case 78:
		_HighlightElement(77, 78);
		strDisplay.Format(_T("HMI_Line_ID: %d\n"), BH_GetViewDataByte(pView, 78)<<8|BH_GetViewDataByte(pView, 77));
		break;
	case 79:
	case 80:
		_HighlightElement(79, 80);
		strDisplay.Format(_T("HMI_Route_ID: %d\n"), BH_GetViewDataByte(pView, 80)<<8|BH_GetViewDataByte(pView, 79));
		break;
	case 81:
	case 82:
		_HighlightElement(81, 82);
		strDisplay.Format(_T("Ref_Velocity: %d\n"), BH_GetViewDataByte(pView, 82)<<8|BH_GetViewDataByte(pView, 81));
		break;
	case 83:
	case 84:
		_HighlightElement(83, 84);
		strDisplay.Format(_T("VCM_Life: %d\n"), BH_GetViewDataByte(pView, 84)<<8|BH_GetViewDataByte(pView, 83));
		break;
	case 85:
	case 86:
		_HighlightElement(85, 86);
		strDisplay.Format(_T("Car1_Temp: %d\n"), BH_GetViewDataByte(pView, 86)<<8|BH_GetViewDataByte(pView, 85));
		break;
	case 87:
	case 88:
		_HighlightElement(87, 88);
		strDisplay.Format(_T("Car2_Temp: %d\n"), BH_GetViewDataByte(pView, 88)<<8|BH_GetViewDataByte(pView, 87));
		break;
	case 89:
	case 90:
		_HighlightElement(89, 90);
		strDisplay.Format(_T("Car3_Temp: %d\n"), BH_GetViewDataByte(pView, 90)<<8|BH_GetViewDataByte(pView, 89));
		break;
	case 91:
	case 92:
		_HighlightElement(91, 92);
		strDisplay.Format(_T("Car4_Temp: %d\n"), BH_GetViewDataByte(pView, 92)<<8|BH_GetViewDataByte(pView, 91));
		break;
	case 93:	
	case 94:
		_HighlightElement(93, 94);
		strDisplay.Format(_T("Car5_Temp: %d\n"), BH_GetViewDataByte(pView, 94)<<8|BH_GetViewDataByte(pView, 93));
		break;
	case 95:	
	case 96:
		_HighlightElement(95, 96);
		strDisplay.Format(_T("Car6_Temp: %d\n"), BH_GetViewDataByte(pView, 86)<<8|BH_GetViewDataByte(pView, 85));
		break;
	case 97://VCM->PIS_TV
		{
			_HighlightElement(97, 97);
			BYTE val = BH_GetViewDataByte(pView, 97);
			strDisplay.Format(_T("Door_emergency_release_1_1: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_2_1: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_3_1: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_4_1: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_5_1: %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_6_1: %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_7_1: %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_8_1: %d \n"), val>>7&1);
		}
		break;
	case 98:
		{
			_HighlightElement(98, 98);
			BYTE val = BH_GetViewDataByte(pView, 98);
			strDisplay.Format(_T("Fire_alarm_1_1: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Fire_alarm_2_1: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("Fire_alarm_3_1: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Fire_alarm_4_1: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Fire_alarm_5_1: %d \n"), val>>4&1);
		}
		break;
	case 99:
		{
			_HighlightElement(99, 99);
			BYTE val = BH_GetViewDataByte(pView, 99);
			strDisplay.Format(_T("Door_emergency_release_1_2: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_2_2: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_3_2: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_4_2: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_5_2: %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_6_2: %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_7_2: %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_8_2: %d \n"), val>>7&1);
		}
		break;
	case 100:
		{
			_HighlightElement(100, 100);
			BYTE val = BH_GetViewDataByte(pView, 100);
			strDisplay.Format(_T("Fire_alarm_1_2: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Fire_alarm_2_2: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("Fire_alarm_3_2: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Fire_alarm_4_2: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Fire_alarm_5_2: %d \n"), val>>4&1);
		}
		break;
	case 101:
		{
			_HighlightElement(101, 101);
			BYTE val = BH_GetViewDataByte(pView, 101);
			strDisplay.Format(_T("Door_emergency_release_1_3: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_2_3: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_3_3: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_4_3: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_5_3: %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_6_3: %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_7_3: %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_8_3: %d \n"), val>>7&1);
		}
		break;
	case 102:
		{
			_HighlightElement(102, 102);
			BYTE val = BH_GetViewDataByte(pView, 102);
			strDisplay.Format(_T("Fire_alarm_1_3: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Fire_alarm_2_3: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("Fire_alarm_3_3: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Fire_alarm_4_3: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Fire_alarm_5_3: %d \n"), val>>4&1);
		}
		break;
	case 103:
		{
			_HighlightElement(103, 103);
			BYTE val = BH_GetViewDataByte(pView, 103);
			strDisplay.Format(_T("Door_emergency_release_1_4: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_2_4: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_3_4: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_4_4: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_5_4: %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_6_4: %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_7_4: %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_8_4: %d \n"), val>>7&1);
		}
		break;
	case 104:
		{
			_HighlightElement(104, 104);
			BYTE val = BH_GetViewDataByte(pView, 104);
			strDisplay.Format(_T("Fire_alarm_1_4: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Fire_alarm_2_4: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("Fire_alarm_3_4: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Fire_alarm_4_4: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Fire_alarm_5_4: %d \n"), val>>4&1);
		}
		break;
	case 105:
		{
			_HighlightElement(105, 105);
			BYTE val = BH_GetViewDataByte(pView, 105);
			strDisplay.Format(_T("Door_emergency_release_1_5: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_2_5: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_3_5: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_4_5: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_5_5: %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_6_5: %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_7_5: %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_8_5: %d \n"), val>>7&1);
		}
		break;
	case 106:
		{
			_HighlightElement(106, 106);
			BYTE val = BH_GetViewDataByte(pView, 106);
			strDisplay.Format(_T("Fire_alarm_1_5: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Fire_alarm_2_5: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("Fire_alarm_3_5: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Fire_alarm_4_5: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Fire_alarm_5_5: %d \n"), val>>4&1);
		}
		break;
	case 107:
		{
			_HighlightElement(107, 107);
			BYTE val = BH_GetViewDataByte(pView, 107);
			strDisplay.Format(_T("Door_emergency_release_1_6: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_2_6: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_3_6: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_4_6: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_5_6: %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_6_6: %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_7_6: %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("Door_emergency_release_8_6: %d \n"), val>>7&1);
		}
		break;
	case 108:
		{
			_HighlightElement(108, 108);
			BYTE val = BH_GetViewDataByte(pView, 108);
			strDisplay.Format(_T("Fire_alarm_1_6: %d\n"), val&1);
			strDisplay.AppendFormat(_T("Fire_alarm_2_6: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("Fire_alarm_3_6: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("Fire_alarm_4_6: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("Fire_alarm_5_6: %d \n"), val>>4&1);
		}
		break;
	case 109:
	case 110:
		_HighlightElement(109, 110);
		strDisplay.Format(_T("Media_host_volume: %d\n"), BH_GetViewDataByte(pView, 110)<<8|BH_GetViewDataByte(pView, 109));
		break;
	case 111:
	case 112:
		break;
	case 113:
		_HighlightElement(113, 113);
		strDisplay.Format(_T("Check Sum: %d\n"), BH_GetViewDataByte(pView, 113));
		break;
	case 114:
		_HighlightElement(114, 114);
		strDisplay.Format(_T("End Flag: %d\n"), BH_GetViewDataByte(pView, 114));
		break;
	default:
		break;
	};
#undef _HighlightElement

 	BH_SetDataTextToStatic(strDisplay.GetBuffer(0));
	strDisplay.ReleaseBuffer();
}