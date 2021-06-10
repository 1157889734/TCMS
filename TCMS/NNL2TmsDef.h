
#define BIG_ENDIAN_1H 0 
#define BIG_ENDIAN_DH 0 
#define BIG_ENDIAN_EH 0 
#define BIG_ENDIAN_FH 0 
#define BIG_ENDIAN_1E8H 0 
#define BIG_ENDIAN_170H 0 
#define BIG_ENDIAN_171H 0

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
	BYTE car_id; 			// 5
	BYTE train_id_3; 		// 6
	BYTE reserve2; 			// 7
	BYTE reserve1; 			// 8 
#endif 
}CAR_ID_F1H_T; 

typedef struct 
{ 
#if BIG_ENDIAN_EH 
	BYTE tcms_h;      // 1  tcms 软件版本 高字节  
	BYTE tcms_l;      // 2 
#else 
	BYTE tcms_l;      // 2 
	BYTE tcms_h;      // 1   tcms 软件版本 高字节  
#endif 
}TCMS_VER_EH_T; 

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
	BYTE reserve1;       // 8
	BYTE time_valid;    // 7    
	BYTE reserve2[8];    // 9-16
#endif 
}TIME_FFH_T; 

typedef struct 
{ 
#if BIG_ENDIAN_1E8H 

	struct 
	{ 
		BYTE date_valid:1;    
		BYTE hour_valid:1; 
		BYTE minute_valid:1; 
		BYTE second_valid:1; 
		BYTE reserve:1; 
		BYTE broadcast_test_valid:1; 
		BYTE reserve2:1; 
		BYTE signal_valid:1; 
	}time_valid_t;               // 1 

	struct 
	{ 
		BYTE door_open_valid:1;    
		BYTE end_station_valid:1; 
		BYTE next_station_valid:1; 
		BYTE current_station_valid:1; 
		BYTE distance_to_next_valid:1; 
		BYTE distance_from_last_valid:1; 
		BYTE year_valid:1; 
		BYTE month_valid:1; 
	}signal_valid_t;                // 2 

	BYTE life;						    // 3
	BYTE door_open_next;			    // 4
	BYTE end_station_hh;				// 5
	BYTE end_station_hl;				// 6
	BYTE end_station_lh;				// 7
	BYTE end_station_ll;				// 8
	BYTE next_station_hh;				// 9
	BYTE next_station_hl;				// 10
	BYTE next_station_lh;				// 11
	BYTE next_station_ll;               // 12
	BYTE current_station_hh;			// 13
	BYTE current_station_hl;            // 14
	BYTE current_station_lh;			// 15
	BYTE current_station_ll;            // 16
	BYTE distance_to_next_h;			// 17
	BYTE distance_to_next_l;			// 18
	BYTE distance_from_last_h;		    // 19
	BYTE distance_from_last_l;			// 20
	BYTE year;						        // 21  
	BYTE month;								// 22
	BYTE date;						        // 23
	BYTE hour;						        // 24
	BYTE minute;							// 25
	BYTE second;							// 26
	BYTE broadcast_test;					// 27
	BYTE vobc;						        // 28
	BYTE reserve1[4];				        // 29-32	
#else 
	struct 
	{ 
		BYTE door_open_valid:1;    
		BYTE end_station_valid:1; 
		BYTE next_station_valid:1; 
		BYTE current_station_valid:1; 
		BYTE distance_to_next_valid:1; 
		BYTE distance_from_last_valid:1; 
		BYTE year_valid:1; 
		BYTE month_valid:1; 
	}signal_valid_t;               // 2 
	struct 
	{ 
		BYTE date_valid:1;    
		BYTE hour_valid:1; 
		BYTE minute_valid:1; 
		BYTE second_valid:1; 
		BYTE reserve:1; 
		BYTE broadcast_test_valid:1; 
		BYTE reserve2:1; 
		BYTE signal_valid:1; 
	}time_valid_t;                 // 1 

	BYTE door_open_next;			       // 4
	BYTE life;						       // 3

	BYTE end_station_hl;				// 6
	BYTE end_station_hh;				// 5
	BYTE end_station_ll;				// 8
	BYTE end_station_lh;				// 7

	BYTE next_station_hl;				// 10
	BYTE next_station_hh;				// 9
	BYTE next_station_ll;				// 12
	BYTE next_station_lh;				// 11

	BYTE current_station_hl;			// 14
	BYTE current_station_hh;			// 13
	BYTE current_station_ll;			// 16
	BYTE current_station_lh;			// 15

	BYTE distance_to_next_l;			// 18
	BYTE distance_to_next_h;			// 17

	BYTE distance_from_last_l;			// 20
	BYTE distance_from_last_h;		       // 19

	BYTE month;						// 22
	BYTE year;						       // 21  

	BYTE hour;						       // 24
	BYTE date;						       // 23

	BYTE second;						// 26
	BYTE minute;						// 25

	BYTE vobc;						       // 28
	BYTE broadcast_test;				// 27	
	BYTE reserve1[4];				       // 29-32	
#endif 
}ATC_1E8H_T; 


typedef struct 
{ 
#if BIG_ENDIAN_170H 
	struct 
	{ 
		BYTE a1_active:1;    
		BYTE a2_active:1; 
		BYTE a1_link:1; 
		BYTE a2_link:1; 
		BYTE left_door_open:1;      // 2s 脉冲开门命令 
		BYTE right_door_open:1; 
		BYTE left_alldoor_closed:1;   // 左门关好锁好    
		BYTE right_alldoor_closed:1; 
	}cab_info;               // 1 

	struct 
	{ 
		BYTE leave_broadcast:1;   // 2s 脉冲 
		BYTE arrive_broadcast:1; 
		BYTE cancel_broadcast:1; 
		BYTE emergency_broadcast:1; 
		BYTE skip_next_station:1;       
		BYTE skip_pre_station:1; 
		BYTE cycle_broadcast:1;    
		BYTE run_chg:1;         // 折返 
	}pa1_info;               // 2 

	struct 
	{ 
		BYTE left_door_close:1;	
		BYTE right_door_close:1;
		BYTE ato_active:1;
		BYTE cm_active:1;
		BYTE atp_off:1;   	
		BYTE auto_man_mode:1;   	//  自动1 手动0 //供参考
		BYTE hmi_led_valid:1;              // HMI激活LED文字显示信息  	        
		BYTE reserve:1;
	}pa2_info;               // 3    
	BYTE reserve1;            // 4 

	BYTE emergency_id_h;      // 5 
	BYTE emergency_id_l;         // 6 
	BYTE start_id_h;            // 7 
	BYTE start_id_l;            // 8 
	BYTE end_id_h;            // 9 
	BYTE end_id_l;            // 10 
	BYTE next_id_h;            // 11 
	BYTE next_id_l;            // 12 
	BYTE line_id_h;            // 13 
	BYTE line_id_l;            // 14 
	BYTE route_id_h;            // 15 
	BYTE route_id_l;            // 16 

	BYTE speed_h;            // 17 
	BYTE speed_l;            // 18 

	BYTE reserve2[12];                      // 19-30
	BYTE led_num;                            // 31
	BYTE reserve3;                            // 32
#else 
	struct
	{
		BYTE leave_broadcast:1;	// 2s 脉冲
		BYTE arrive_broadcast:1;
		BYTE cancel_broadcast:1;
		BYTE emergency_broadcast:1;
		BYTE skip_pre_station:1;   	
		BYTE skip_next_station:1;
		BYTE cycle_broadcast:1;	
		BYTE run_chg:1;			// 折返
	}pa1_info;					// 2
	struct
	{
		BYTE a1_active:1;	
		BYTE a2_active:1;
		BYTE a1_link:1;
		BYTE a2_link:1;
		BYTE left_door_open:1;   	// 2s 脉冲开门命令
		BYTE right_door_open:1;
		BYTE left_alldoor_closed:1;	// 左门关好锁好	
		BYTE right_alldoor_closed:1;
	}cab_info;					// 1

	BYTE reserve1;				// 4
	struct
	{
		BYTE left_door_close:1;	
		BYTE right_door_close:1;
		BYTE ato_active:1;
		BYTE cm_active:1;
		BYTE atp_off:1;   	
		BYTE auto_man_mode:1;   	//  自动1 手动0 //供参考
		BYTE hmi_led_valid:1;              // HMI激活LED文字显示信息  	        
		BYTE reserve:1;
	}pa2_info;					// 3	

	BYTE emergency_id_l;			// 6
	BYTE emergency_id_h;		       // 5
	BYTE start_id_l;				// 8
	BYTE start_id_h;				// 7
	BYTE end_id_l;				       // 10
	BYTE end_id_h;				// 9
	BYTE next_id_l;				// 12
	BYTE next_id_h;				// 11
	BYTE line_id_l;				       // 14
	BYTE line_id_h;				// 13
	BYTE route_id_l;				// 16
	BYTE route_id_h;				// 15

	BYTE speed_l;				       // 18
	BYTE speed_h;				       // 17

	BYTE reserve2[12];                      // 19-30

	BYTE reserve3;                            // 32
	BYTE led_num;                            // 31
#endif 
}TCMS_170H_T; 


typedef struct
{
#if BIG_ENDIAN_171H
	struct
	{
		BYTE a1_up:1;	
		BYTE b1_up:1;
		BYTE c1_up:1;
		BYTE c2_up:1;
		BYTE b2_up:1;   
		BYTE a2_up:1;
		BYTE res1:1;
		BYTE res2:1;  
	}fire_up_err;                           // 1

	struct
	{
		BYTE a1_down:1;	
		BYTE b1_down:1;
		BYTE c1_down:1;
		BYTE c2_down:1;
		BYTE b2_down:1;   
		BYTE a2_down:1;
		BYTE res1:1;
		BYTE res2:1;  
	}fire_up_err2;                           // 2

	struct
	{
		BYTE a1_car_door:1;	 
		BYTE a2_car_door:1;
		BYTE res:6;
	}fire_up_err3;                           // 3
	BYTE reserve4;                         // 4

	BYTE reserve5;                         // 5
	BYTE reserve6;                         // 6

	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}a1_door_unlock;				// 7
	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}a1_door_unlock2;				// 8

	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}b1_door_unlock;				// 9
	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}b1_door_unlock2;				// 10

	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}c1_door_unlock;				// 11
	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}c1_door_unlock2;				// 12

	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}c2_door_unlock;				// 13
	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}c2_door_unlock2;				// 14

	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}b2_door_unlock;				// 15
	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}b2_door_unlock2;				// 16

	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}a2_door_unlock;				// 17
	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}a2_door_unlock2;				// 18    

	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}a1_door_err1;				// 19
	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}a1_door_err2;				// 20

	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}b1_door_err1;				// 21

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}b1_door_err2;				// 22

	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}c1_door_err1;				// 23

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}c1_door_err2;				// 24

	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}c2_door_err1;				// 25

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}c2_door_err2;				// 26

	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}b2_door_err1;				// 27

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}b2_door_err2;				// 28

	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}a2_door_err1;				// 29

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}a2_door_err2;				// 30

	BYTE reserve3132[2];                  // 31-32

#else
	struct
	{
		BYTE a1_down:1;	
		BYTE b1_down:1;
		BYTE c1_down:1;
		BYTE c2_down:1;
		BYTE b2_down:1;   
		BYTE a2_down:1;
		BYTE res1:1;
		BYTE res2:1;  
	}fire_up_err;                           // 2
	struct
	{
		BYTE a1_up:1;	
		BYTE b1_up:1;
		BYTE c1_up:1;
		BYTE c2_up:1;
		BYTE b2_up:1;   
		BYTE a2_up:1;
		BYTE res1:1;
		BYTE res2:1;  
	}fire_up_err2;                           // 1

	BYTE reserve4;                         // 4
	struct
	{
		BYTE a1_car_door:1;	 
		BYTE a2_car_door:1;
		BYTE res:6;
	}fire_up_err3;                           // 3

	BYTE reserve6;                         // 6    
	BYTE reserve5;                         // 5

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}a1_door_unlock2;				// 8
	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}a1_door_unlock;				// 7

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}b1_door_unlock2;				// 10
	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}b1_door_unlock;				// 9

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}c1_door_unlock2;				// 12
	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}c1_door_unlock;				// 11

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}c2_door_unlock2;				// 14
	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}c2_door_unlock;				// 13

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}b2_door_unlock2;				// 16
	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}b2_door_unlock;				// 15

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}a2_door_unlock2;				// 18  
	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}a2_door_unlock;				// 17

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}a1_door_err2;				// 20
	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}a1_door_err1;				// 19

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}b1_door_err2;				// 22
	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}b1_door_err1;				// 21

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}c1_door_err2;				// 24
	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}c1_door_err1;				// 23

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}c2_door_err2;				// 26
	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}c2_door_err1;				// 25

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}b2_door_err2;				// 28
	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}b2_door_err1;				// 27

	struct
	{
		BYTE door_9:1;	
		BYTE door_10:1;
		BYTE reserve:6; 
	}a2_door_err2;				// 30
	struct
	{
		BYTE door_1:1;	
		BYTE door_2:1;
		BYTE door_3:1;
		BYTE door_4:1;
		BYTE door_5:1;   
		BYTE door_6:1;
		BYTE door_7:1;
		BYTE door_8:1;  
	}a2_door_err1;				// 29

	BYTE reserve3132[2];                  // 31-32
#endif
}TCMS_171H_T;

typedef struct 
{ 
	BYTE fram_begin;   				       // 0  0xFE

	/******车号端口 1H  Len:8******/
	CAR_ID_1H_T car_id_t;   			// 1-8

	/******生命信号端口 DH  Len:2******/
	LIFE_DH_T life_t;					// 9-10		

	/******TCMS软件版本 EH  Len:2******/
	TCMS_VER_EH_T tcms_ver_t; 		// 11-12 

	/******时间 FH  Len:16******/
	TIME_FH_T time_t; 				       // 13-28

	/******ATC端口信号 1E8H  Len:32******/	// 29-60
	ATC_1E8H_T atc_t;

	/******ATC端口信号 6E8H  Len:32******/	// 61-92
	ATC_1E8H_T atc_t2;

	/******TCMS端口信号 170H  Len:32******/	// 93-124
	TCMS_170H_T tcms_t;
	/******TCMS端口信号 170H  Len:32******/	// 125-156
	TCMS_171H_T tcms_t2;

	BYTE checksum;    				       //  157
	BYTE fram_end;   					//  158  0xFF	
}tms_packet_t;



#define BIG_ENDIAN_1F1H 0 
#define BIG_ENDIAN_1F2H 0 
#define BIG_ENDIAN_1F3H 0 

typedef struct 
{ 
#if BIG_ENDIAN_1F1H 
	BYTE life;      // 1   
	BYTE reserve1;   // 2 

	struct 
	{ 
		BYTE pecu_alarm_1:1; 
		BYTE pecu_alarm_2:1; 
		BYTE pecu_alarm_3:1;       
		BYTE pecu_talk_1:1; 
		BYTE pecu_talk_2:1; 
		BYTE pecu_talk_3:1; 
		BYTE reserve:2; 
	}a1_pecu;      // 3 
	struct 
	{ 
		BYTE pecu_alarm_1:1; 
		BYTE pecu_alarm_2:1; 
		BYTE pecu_alarm_3:1;       
		BYTE pecu_talk_1:1; 
		BYTE pecu_talk_2:1; 
		BYTE pecu_talk_3:1; 
		BYTE reserve:2; 
	}b1_pecu;      // 4 

	struct 
	{ 
		BYTE pecu_alarm_1:1; 
		BYTE pecu_alarm_2:1; 
		BYTE pecu_alarm_3:1;       
		BYTE pecu_talk_1:1; 
		BYTE pecu_talk_2:1; 
		BYTE pecu_talk_3:1; 
		BYTE reserve:2; 
	}c1_pecu;      // 5 
	struct 
	{ 
		BYTE pecu_alarm_1:1; 
		BYTE pecu_alarm_2:1; 
		BYTE pecu_alarm_3:1;       
		BYTE pecu_talk_1:1; 
		BYTE pecu_talk_2:1; 
		BYTE pecu_talk_3:1; 
		BYTE reserve:2; 
	}c2_pecu;      // 6 

	struct 
	{ 
		BYTE pecu_alarm_1:1; 
		BYTE pecu_alarm_2:1; 
		BYTE pecu_alarm_3:1;       
		BYTE pecu_talk_1:1; 
		BYTE pecu_talk_2:1; 
		BYTE pecu_talk_3:1; 
		BYTE reserve:2; 
	}b2_pecu;      // 7 
	struct 
	{ 
		BYTE pecu_alarm_1:1; 
		BYTE pecu_alarm_2:1; 
		BYTE pecu_alarm_3:1;       
		BYTE pecu_talk_1:1; 
		BYTE pecu_talk_2:1; 
		BYTE pecu_talk_3:1; 
		BYTE reserve:2; 
	}a2_pecu;      // 8 

	BYTE reserve2[12];  // 9-20 

	BYTE start_station_h; // 21 
	BYTE start_station_l;  // 22 
	BYTE end_station_h;  // 23 
	BYTE end_station_l;   // 24 
	BYTE next_station_h; // 25 
	BYTE next_station_l;  // 26 
	BYTE line_id_h;          // 27 
	BYTE line_id_l;           // 28 
	BYTE route_id_h;       // 29 
	BYTE route_id_l;        // 30 
	BYTE current_station_h; // 31 
	BYTE current_station_l;  // 32 
#else 
	BYTE reserve1;   // 2 
	BYTE life;      // 1   

	struct 
	{ 
		BYTE pecu_alarm_1:1; 
		BYTE pecu_alarm_2:1; 
		BYTE pecu_alarm_3:1;       
		BYTE pecu_talk_1:1; 
		BYTE pecu_talk_2:1; 
		BYTE pecu_talk_3:1; 
		BYTE reserve:2; 
	}b1_pecu;      // 4 
	struct 
	{ 
		BYTE pecu_alarm_1:1; 
		BYTE pecu_alarm_2:1; 
		BYTE pecu_alarm_3:1;       
		BYTE pecu_talk_1:1; 
		BYTE pecu_talk_2:1; 
		BYTE pecu_talk_3:1; 
		BYTE reserve:2; 
	}a1_pecu;      // 3 

	struct 
	{ 
		BYTE pecu_alarm_1:1; 
		BYTE pecu_alarm_2:1; 
		BYTE pecu_alarm_3:1;       
		BYTE pecu_talk_1:1; 
		BYTE pecu_talk_2:1; 
		BYTE pecu_talk_3:1; 
		BYTE reserve:2; 
	}c2_pecu;      // 6 
	struct 
	{ 
		BYTE pecu_alarm_1:1; 
		BYTE pecu_alarm_2:1; 
		BYTE pecu_alarm_3:1;       
		BYTE pecu_talk_1:1; 
		BYTE pecu_talk_2:1; 
		BYTE pecu_talk_3:1; 
		BYTE reserve:2; 
	}c1_pecu;      // 5 

	struct 
	{ 
		BYTE pecu_alarm_1:1; 
		BYTE pecu_alarm_2:1; 
		BYTE pecu_alarm_3:1;       
		BYTE pecu_talk_1:1; 
		BYTE pecu_talk_2:1; 
		BYTE pecu_talk_3:1; 
		BYTE reserve:2; 
	}a2_pecu;      // 8 
	struct 
	{ 
		BYTE pecu_alarm_1:1; 
		BYTE pecu_alarm_2:1; 
		BYTE pecu_alarm_3:1;       
		BYTE pecu_talk_1:1; 
		BYTE pecu_talk_2:1; 
		BYTE pecu_talk_3:1; 
		BYTE reserve:2; 
	}b2_pecu;      // 7 


	BYTE reserve2[12];  // 9-20 
	BYTE start_station_l;  // 22 
	BYTE start_station_h; // 21 
	BYTE end_station_l;   // 24 
	BYTE end_station_h;  // 23 
	BYTE next_station_l;  // 26 
	BYTE next_station_h; // 25 
	BYTE line_id_l;           // 28 
	BYTE line_id_h;          // 27 
	BYTE route_id_l;        // 30 
	BYTE route_id_h;       // 29 
	BYTE current_station_l;  // 32 
	BYTE current_station_h; // 31    
#endif 
}STATUS_1F1H_T; 

typedef struct 
{ 
#if BIG_ENDIAN_1F2H 
	struct 
	{ 
		BYTE light_err:1; 
		BYTE medium_err:1; 
		BYTE serious_err:1;    
		BYTE reserve:5; 
	}sys_err_level;         // 1 
	struct 
	{ 
		BYTE a1_pis_err:1; 
		BYTE a2_pis_err:1; 
		BYTE a1_lcu_err:1; 
		BYTE b1_lcu_err:1; 
		BYTE c1_lcu_err:1;    
		BYTE c2_lcu_err:1; 
		BYTE b2_lcu_err:1; 
		BYTE a2_lcu_err:1;             
	}car_err;            // 2 

	struct 
	{ 
		BYTE pecu_err_1:1; 
		BYTE pecu_err_2:1; 
		BYTE pecu_err_3:1; 
		BYTE reserve:5;             
	}a1_pecu_err;         // 3 
	struct 
	{ 
		BYTE pecu_err_1:1; 
		BYTE pecu_err_2:1; 
		BYTE pecu_err_3:1; 
		BYTE reserve:5;             
	} b1_pecu_err;         // 4 

	struct 
	{ 
		BYTE pecu_err_1:1; 
		BYTE pecu_err_2:1; 
		BYTE pecu_err_3:1; 
		BYTE reserve:5;             
	}c1_pecu_err;         // 5 
	struct 
	{ 
		BYTE pecu_err_1:1; 
		BYTE pecu_err_2:1; 
		BYTE pecu_err_3:1; 
		BYTE reserve:5;             
	} c2_pecu_err;         // 6 

	struct 
	{ 
		BYTE pecu_err_1:1; 
		BYTE pecu_err_2:1; 
		BYTE pecu_err_3:1; 
		BYTE reserve:5;             
	}b2_pecu_err;         // 7 
	struct 
	{ 
		BYTE pecu_err_1:1; 
		BYTE pecu_err_2:1; 
		BYTE pecu_err_3:1; 
		BYTE reserve:5;             
	}a2_pecu_err;         // 8 

	struct 
	{ 
		BYTE drmd_err_1:1; 
		BYTE drmd_err_2:1; 
		BYTE drmd_err_3:1; 
		BYTE drmd_err_4:1; 
		BYTE drmd_err_5:1; 
		BYTE drmd_err_6:1; 
		BYTE drmd_err_7:1; 
		BYTE drmd_err_8:1; 
	}a1_drmd_err1;         // 9 
	struct 
	{ 
		BYTE drmd_err_9:1; 
		BYTE drmd_err_10:1; 
		BYTE reserve:6;             
	} a1_drmd_err2;         // 10 

	struct 
	{ 
		BYTE drmd_err_1:1; 
		BYTE drmd_err_2:1; 
		BYTE drmd_err_3:1; 
		BYTE drmd_err_4:1; 
		BYTE drmd_err_5:1; 
		BYTE drmd_err_6:1; 
		BYTE drmd_err_7:1; 
		BYTE drmd_err_8:1; 
	}b1_drmd_err1;         // 11 
	struct 
	{ 
		BYTE drmd_err_9:1; 
		BYTE drmd_err_10:1; 
		BYTE reserve:6;             
	} b1_drmd_err2;         // 12 

	struct 
	{ 
		BYTE drmd_err_1:1; 
		BYTE drmd_err_2:1; 
		BYTE drmd_err_3:1; 
		BYTE drmd_err_4:1; 
		BYTE drmd_err_5:1; 
		BYTE drmd_err_6:1; 
		BYTE drmd_err_7:1; 
		BYTE drmd_err_8:1; 
	}c1_drmd_err1;         // 13 
	struct 
	{ 
		BYTE drmd_err_9:1; 
		BYTE drmd_err_10:1; 
		BYTE reserve:6;             
	}c1_drmd_err2;         // 14 

	struct 
	{ 
		BYTE drmd_err_1:1; 
		BYTE drmd_err_2:1; 
		BYTE drmd_err_3:1; 
		BYTE drmd_err_4:1; 
		BYTE drmd_err_5:1; 
		BYTE drmd_err_6:1; 
		BYTE drmd_err_7:1; 
		BYTE drmd_err_8:1; 
	}c2_drmd_err1;         // 15 
	struct 
	{ 
		BYTE drmd_err_9:1; 
		BYTE drmd_err_10:1; 
		BYTE reserve:6;             
	}c2_drmd_err2;         // 16 

	struct 
	{ 
		BYTE drmd_err_1:1; 
		BYTE drmd_err_2:1; 
		BYTE drmd_err_3:1; 
		BYTE drmd_err_4:1; 
		BYTE drmd_err_5:1; 
		BYTE drmd_err_6:1; 
		BYTE drmd_err_7:1; 
		BYTE drmd_err_8:1; 
	}b2_drmd_err1;         // 17 
	struct 
	{ 
		BYTE drmd_err_9:1; 
		BYTE drmd_err_10:1; 
		BYTE reserve:6;             
	}b2_drmd_err2;         // 18 

	struct 
	{ 
		BYTE drmd_err_1:1; 
		BYTE drmd_err_2:1; 
		BYTE drmd_err_3:1; 
		BYTE drmd_err_4:1; 
		BYTE drmd_err_5:1; 
		BYTE drmd_err_6:1; 
		BYTE drmd_err_7:1; 
		BYTE drmd_err_8:1; 
	}a2_drmd_err1;         // 19 
	struct 
	{ 
		BYTE drmd_err_9:1; 
		BYTE drmd_err_10:1; 
		BYTE reserve:6;             
	} a2_drmd_err2;         // 20 

	struct 
	{ 
		BYTE a1_dst_led_err:1; 
		BYTE a2_dst_led_err:1; 
		BYTE a1_box_led_err:1; 
		BYTE a2_box_led_err:1; 
		BYTE reserve:4; 
	} car_err2;					// 21 
	BYTE reserve1;                  // 22 

	BYTE reserve2[10];            // 23-32 

#else 
	struct 
	{ 
		BYTE a1_pis_err:1; 
		BYTE a2_pis_err:1; 
		BYTE a1_lcu_err:1; 
		BYTE b1_lcu_err:1; 
		BYTE c1_lcu_err:1;    
		BYTE c2_lcu_err:1; 
		BYTE b2_lcu_err:1; 
		BYTE a2_lcu_err:1;             
	}car_err;            // 2 
	struct 
	{ 
		BYTE light_err:1; 
		BYTE medium_err:1; 
		BYTE serious_err:1;    
		BYTE reserve:5; 
	}sys_err_level;         // 1 

	struct 
	{ 
		BYTE pecu_err_1:1; 
		BYTE pecu_err_2:1; 
		BYTE pecu_err_3:1; 
		BYTE reserve:5;             
	} b1_pecu_err;         // 4 
	struct 
	{ 
		BYTE pecu_err_1:1; 
		BYTE pecu_err_2:1; 
		BYTE pecu_err_3:1; 
		BYTE reserve:5;             
	}a1_pecu_err;         // 3 

	struct 
	{ 
		BYTE pecu_err_1:1; 
		BYTE pecu_err_2:1; 
		BYTE pecu_err_3:1; 
		BYTE reserve:5;             
	} c2_pecu_err;         // 6 
	struct 
	{ 
		BYTE pecu_err_1:1; 
		BYTE pecu_err_2:1; 
		BYTE pecu_err_3:1; 
		BYTE reserve:5;             
	}c1_pecu_err;         // 5 

	struct 
	{ 
		BYTE pecu_err_1:1; 
		BYTE pecu_err_2:1; 
		BYTE pecu_err_3:1; 
		BYTE reserve:5;             
	}a2_pecu_err;         // 8 
	struct 
	{ 
		BYTE pecu_err_1:1; 
		BYTE pecu_err_2:1; 
		BYTE pecu_err_3:1; 
		BYTE reserve:5;             
	}b2_pecu_err;         // 7 

	struct 
	{ 
		BYTE drmd_err_9:1; 
		BYTE drmd_err_10:1; 
		BYTE reserve:6;             
	} a1_drmd_err2;         // 10 
	struct 
	{ 
		BYTE drmd_err_1:1; 
		BYTE drmd_err_2:1; 
		BYTE drmd_err_3:1; 
		BYTE drmd_err_4:1; 
		BYTE drmd_err_5:1; 
		BYTE drmd_err_6:1; 
		BYTE drmd_err_7:1; 
		BYTE drmd_err_8:1; 
	}a1_drmd_err1;         // 9 

	struct 
	{ 
		BYTE drmd_err_9:1; 
		BYTE drmd_err_10:1; 
		BYTE reserve:6;             
	} b1_drmd_err2;         // 12 
	struct 
	{ 
		BYTE drmd_err_1:1; 
		BYTE drmd_err_2:1; 
		BYTE drmd_err_3:1; 
		BYTE drmd_err_4:1; 
		BYTE drmd_err_5:1; 
		BYTE drmd_err_6:1; 
		BYTE drmd_err_7:1; 
		BYTE drmd_err_8:1; 
	}b1_drmd_err1;         // 11 

	struct 
	{ 
		BYTE drmd_err_9:1; 
		BYTE drmd_err_10:1; 
		BYTE reserve:6;             
	}c1_drmd_err2;         // 14 
	struct 
	{ 
		BYTE drmd_err_1:1; 
		BYTE drmd_err_2:1; 
		BYTE drmd_err_3:1; 
		BYTE drmd_err_4:1; 
		BYTE drmd_err_5:1; 
		BYTE drmd_err_6:1; 
		BYTE drmd_err_7:1; 
		BYTE drmd_err_8:1; 
	}c1_drmd_err1;         // 13 

	struct 
	{ 
		BYTE drmd_err_9:1; 
		BYTE drmd_err_10:1; 
		BYTE reserve:6;             
	}c2_drmd_err2;         // 16 
	struct 
	{ 
		BYTE drmd_err_1:1; 
		BYTE drmd_err_2:1; 
		BYTE drmd_err_3:1; 
		BYTE drmd_err_4:1; 
		BYTE drmd_err_5:1; 
		BYTE drmd_err_6:1; 
		BYTE drmd_err_7:1; 
		BYTE drmd_err_8:1; 
	}c2_drmd_err1;         // 15 

	struct 
	{ 
		BYTE drmd_err_9:1; 
		BYTE drmd_err_10:1; 
		BYTE reserve:6;             
	}b2_drmd_err2;         // 18 
	struct 
	{ 
		BYTE drmd_err_1:1; 
		BYTE drmd_err_2:1; 
		BYTE drmd_err_3:1; 
		BYTE drmd_err_4:1; 
		BYTE drmd_err_5:1; 
		BYTE drmd_err_6:1; 
		BYTE drmd_err_7:1; 
		BYTE drmd_err_8:1; 
	}b2_drmd_err1;         // 17 

	struct 
	{ 
		BYTE drmd_err_9:1; 
		BYTE drmd_err_10:1; 
		BYTE reserve:6;             
	} a2_drmd_err2;         // 20 
	struct 
	{ 
		BYTE drmd_err_1:1; 
		BYTE drmd_err_2:1; 
		BYTE drmd_err_3:1; 
		BYTE drmd_err_4:1; 
		BYTE drmd_err_5:1; 
		BYTE drmd_err_6:1; 
		BYTE drmd_err_7:1; 
		BYTE drmd_err_8:1; 
	}a2_drmd_err1;         // 19 

	BYTE reserve1;                  // 22 
	struct 
	{ 
		BYTE a1_dst_led_err:1; 
		BYTE a2_dst_led_err:1; 
		BYTE a1_box_led_err:1; 
		BYTE a2_box_led_err:1; 
		BYTE reserve:4; 
	} car_err2;         // 21    

	BYTE reserve2[10];            // 23-32 
#endif 
}ERR_1F2H_T; 

typedef struct 
{ 
#if BIG_ENDIAN_1F3H 
	BYTE a1_pis_version_h;   // 1 
	BYTE a1_pis_version_l;   // 2 

	BYTE a1_lcu_version_h;   // 3 
	BYTE a1_lcu_version_l;   // 4 

	BYTE b1_lcu_version_h;   // 5 
	BYTE b1_lcu_version_l;   // 6 

	BYTE c1_lcu_version_h;   // 7 
	BYTE c1_lcu_version_l;   // 8 

	BYTE c2_lcu_version_h;   // 9 
	BYTE c2_lcu_version_l;   // 10 

	BYTE b2_lcu_version_h;   // 11 
	BYTE b2_lcu_version_l;   // 12 

	BYTE a2_lcu_version_h;   // 13 
	BYTE a2_lcu_version_l;   // 14 

	BYTE a2_pis_version_h;   // 15 
	BYTE a2_pis_version_l;   // 16 

	BYTE reserve1[16];      // 17-32 

#else 
	BYTE a1_pis_version_l;   // 2 
	BYTE a1_pis_version_h;   // 1 

	BYTE a1_lcu_version_l;   // 4 
	BYTE a1_lcu_version_h;   // 3 

	BYTE b1_lcu_version_l;   // 6 
	BYTE b1_lcu_version_h;   // 5 

	BYTE c1_lcu_version_l;   // 8 
	BYTE c1_lcu_version_h;   // 7 

	BYTE c2_lcu_version_l;   // 10 
	BYTE c2_lcu_version_h;   // 9 

	BYTE b2_lcu_version_l;   // 12 
	BYTE b2_lcu_version_h;   // 11 

	BYTE a2_lcu_version_l;   // 14 
	BYTE a2_lcu_version_h;   // 13 

	BYTE a2_pis_version_l;   // 16 
	BYTE a2_pis_version_h;   // 15 

	BYTE reserve1[16];      // 17-32 
#endif 
}VER_1F3H_T; 


typedef struct 
{ 
	BYTE fram_begin;   // 0 0xFE 

	/******PIS状态数据端口 1f1H  Len:32******/ 
	STATUS_1F1H_T status_t;            // 1-32 

	/******PIS诊断数据端口 1f2H  Len:32******/ 
	ERR_1F2H_T err_t;               // 33-64 

	/******PIS版本数据端口 1f3H  Len:32******/ 
	VER_1F3H_T ver_t;               // 65-96 

	BYTE checksum;    //  97 
	BYTE fram_end;      //  98 0xFF    
}tms_recv_packet_t;


static void OnRefreshFeedbackPanel()
{
	char szTitlTxt[6][32]={_T("A1 车"), _T("B1 车"), _T("C1 车"), _T("C2 车"), _T("B2 车"), _T("A2 车")};

	char szTxt[6][32]={_T("报警器报警-1"), _T("报警器报警-2"), _T("报警器报警-3"),  _T("报警器通话-1"), _T("报警器通话-2"), _T("报警器通话-3")};

	char szDiagTxt[17][32]={
		_T("PIS 故障"),	_T("LCU 故障"),
		_T("紧急报警器-1"), _T("紧急报警器-2"), _T("紧急报警器-3"),
		_T("动态地图-1"), _T("动态地图-2"), _T("动态地图-3"), _T("动态地图-4"),	_T("动态地图-5"),
		_T("动态地图-6"), _T("动态地图-7"), _T("动态地图-8"), _T("动态地图-9"),	_T("动态地图-10"),
		_T("终点站屏"), _T("广播盒")
	};

	for (int i=0; i<6; i++)//6节车厢
	{
		BH_SetDrawLineText(i, 0, szTitlTxt[i]);
		for (int j=0; j<6; j++)
		{
			BH_SetDrawLineText(i, j+1, szTxt[j]);
		}	

		BH_SetDrawLineText(i, 7, _T(""));//空格
		for (int k=0; k<17; k++)
		{
			BH_SetDrawLineText(i, k+8, szDiagTxt[k]);
			if ( (k==0 || k==16 || k==15) && !(i==0 || i==5) )
			{
				BH_SetDrawLineText(i, k+8, _T(""));
			}
		}
	}
}

static void OnRefreshState(tms_recv_packet_t stRecvpacket)
{
	BYTE bCarState[6][6];
	bCarState[0][0]=stRecvpacket.status_t.a1_pecu.pecu_alarm_1;
	bCarState[0][1]=stRecvpacket.status_t.a1_pecu.pecu_alarm_2;
	bCarState[0][2]=stRecvpacket.status_t.a1_pecu.pecu_alarm_3;
	bCarState[0][3]=stRecvpacket.status_t.a1_pecu.pecu_talk_1;
	bCarState[0][4]=stRecvpacket.status_t.a1_pecu.pecu_talk_2;
	bCarState[0][5]=stRecvpacket.status_t.a1_pecu.pecu_talk_3;

	bCarState[1][0]=stRecvpacket.status_t.b1_pecu.pecu_alarm_1;
	bCarState[1][1]=stRecvpacket.status_t.b1_pecu.pecu_alarm_2;
	bCarState[1][2]=stRecvpacket.status_t.b1_pecu.pecu_alarm_3;
	bCarState[1][3]=stRecvpacket.status_t.b1_pecu.pecu_talk_1;
	bCarState[1][4]=stRecvpacket.status_t.b1_pecu.pecu_talk_2;
	bCarState[1][5]=stRecvpacket.status_t.b1_pecu.pecu_talk_3;

	bCarState[2][0]=stRecvpacket.status_t.c1_pecu.pecu_alarm_1;
	bCarState[2][1]=stRecvpacket.status_t.c1_pecu.pecu_alarm_2;
	bCarState[2][2]=stRecvpacket.status_t.c1_pecu.pecu_alarm_3;
	bCarState[2][3]=stRecvpacket.status_t.c1_pecu.pecu_talk_1;
	bCarState[2][4]=stRecvpacket.status_t.c1_pecu.pecu_talk_2;
	bCarState[2][5]=stRecvpacket.status_t.c1_pecu.pecu_talk_3;

	bCarState[3][0]=stRecvpacket.status_t.c2_pecu.pecu_alarm_1;
	bCarState[3][1]=stRecvpacket.status_t.c2_pecu.pecu_alarm_2;
	bCarState[3][2]=stRecvpacket.status_t.c2_pecu.pecu_alarm_3;
	bCarState[3][3]=stRecvpacket.status_t.c2_pecu.pecu_talk_1;
	bCarState[3][4]=stRecvpacket.status_t.c2_pecu.pecu_talk_2;
	bCarState[3][5]=stRecvpacket.status_t.c2_pecu.pecu_talk_3;

	bCarState[4][0]=stRecvpacket.status_t.b2_pecu.pecu_alarm_1;
	bCarState[4][1]=stRecvpacket.status_t.b2_pecu.pecu_alarm_2;
	bCarState[4][2]=stRecvpacket.status_t.b2_pecu.pecu_alarm_3;
	bCarState[4][3]=stRecvpacket.status_t.b2_pecu.pecu_talk_1;
	bCarState[4][4]=stRecvpacket.status_t.b2_pecu.pecu_talk_2;
	bCarState[4][5]=stRecvpacket.status_t.b2_pecu.pecu_talk_3;

	bCarState[5][0]=stRecvpacket.status_t.a2_pecu.pecu_alarm_1;
	bCarState[5][1]=stRecvpacket.status_t.a2_pecu.pecu_alarm_2;
	bCarState[5][2]=stRecvpacket.status_t.a2_pecu.pecu_alarm_3;
	bCarState[5][3]=stRecvpacket.status_t.a2_pecu.pecu_talk_1;
	bCarState[5][4]=stRecvpacket.status_t.a2_pecu.pecu_talk_2;
	bCarState[5][5]=stRecvpacket.status_t.a2_pecu.pecu_talk_3;

	BYTE bCarError[6][17];

	//a1
	bCarError[0][0]=stRecvpacket.err_t.car_err.a1_pis_err;
	bCarError[0][1]=stRecvpacket.err_t.car_err.a1_lcu_err;
	//报警器
	bCarError[0][2]=stRecvpacket.err_t.a1_pecu_err.pecu_err_1;
	bCarError[0][3]=stRecvpacket.err_t.a1_pecu_err.pecu_err_2;
	bCarError[0][4]=stRecvpacket.err_t.a1_pecu_err.pecu_err_3;

	//动态地图
	bCarError[0][5]=stRecvpacket.err_t.a1_drmd_err1.drmd_err_1;
	bCarError[0][6]=stRecvpacket.err_t.a1_drmd_err1.drmd_err_2;
	bCarError[0][7]=stRecvpacket.err_t.a1_drmd_err1.drmd_err_3;
	bCarError[0][8]=stRecvpacket.err_t.a1_drmd_err1.drmd_err_4;
	bCarError[0][9]=stRecvpacket.err_t.a1_drmd_err1.drmd_err_5;
	bCarError[0][10]=stRecvpacket.err_t.a1_drmd_err1.drmd_err_6;
	bCarError[0][11]=stRecvpacket.err_t.a1_drmd_err1.drmd_err_7;
	bCarError[0][12]=stRecvpacket.err_t.a1_drmd_err1.drmd_err_8;
	bCarError[0][13]=stRecvpacket.err_t.a1_drmd_err2.drmd_err_9;
	bCarError[0][14]=stRecvpacket.err_t.a1_drmd_err2.drmd_err_10;

	bCarError[0][15]=stRecvpacket.err_t.car_err2.a1_dst_led_err;
	bCarError[0][16]=stRecvpacket.err_t.car_err2.a1_box_led_err;

	//b1
	bCarError[1][1]=stRecvpacket.err_t.car_err.b1_lcu_err;
	//报警器
	bCarError[1][2]=stRecvpacket.err_t.b1_pecu_err.pecu_err_1;
	bCarError[1][3]=stRecvpacket.err_t.b1_pecu_err.pecu_err_2;
	bCarError[1][4]=stRecvpacket.err_t.b1_pecu_err.pecu_err_3;

	//动态地图
	bCarError[1][5]=stRecvpacket.err_t.b1_drmd_err1.drmd_err_1;
	bCarError[1][6]=stRecvpacket.err_t.b1_drmd_err1.drmd_err_2;
	bCarError[1][7]=stRecvpacket.err_t.b1_drmd_err1.drmd_err_3;
	bCarError[1][8]=stRecvpacket.err_t.b1_drmd_err1.drmd_err_4;
	bCarError[1][9]=stRecvpacket.err_t.b1_drmd_err1.drmd_err_5;
	bCarError[1][10]=stRecvpacket.err_t.b1_drmd_err1.drmd_err_6;
	bCarError[1][11]=stRecvpacket.err_t.b1_drmd_err1.drmd_err_7;
	bCarError[1][12]=stRecvpacket.err_t.b1_drmd_err1.drmd_err_8;
	bCarError[1][13]=stRecvpacket.err_t.b1_drmd_err2.drmd_err_9;
	bCarError[1][14]=stRecvpacket.err_t.b1_drmd_err2.drmd_err_10;


	//c1
	bCarError[2][1]=stRecvpacket.err_t.car_err.c1_lcu_err;
	//报警器
	bCarError[2][2]=stRecvpacket.err_t.c1_pecu_err.pecu_err_1;
	bCarError[2][3]=stRecvpacket.err_t.c1_pecu_err.pecu_err_2;
	bCarError[2][4]=stRecvpacket.err_t.c1_pecu_err.pecu_err_3;

	//动态地图
	bCarError[2][5]=stRecvpacket.err_t.c1_drmd_err1.drmd_err_1;
	bCarError[2][6]=stRecvpacket.err_t.c1_drmd_err1.drmd_err_2;
	bCarError[2][7]=stRecvpacket.err_t.c1_drmd_err1.drmd_err_3;
	bCarError[2][8]=stRecvpacket.err_t.c1_drmd_err1.drmd_err_4;
	bCarError[2][9]=stRecvpacket.err_t.c1_drmd_err1.drmd_err_5;
	bCarError[2][10]=stRecvpacket.err_t.c1_drmd_err1.drmd_err_6;
	bCarError[2][11]=stRecvpacket.err_t.c1_drmd_err1.drmd_err_7;
	bCarError[2][12]=stRecvpacket.err_t.c1_drmd_err1.drmd_err_8;
	bCarError[2][13]=stRecvpacket.err_t.c1_drmd_err2.drmd_err_9;
	bCarError[2][14]=stRecvpacket.err_t.c1_drmd_err2.drmd_err_10;

	//c2
	bCarError[3][1]=stRecvpacket.err_t.car_err.c2_lcu_err;
	//报警器
	bCarError[3][2]=stRecvpacket.err_t.c2_pecu_err.pecu_err_1;
	bCarError[3][3]=stRecvpacket.err_t.c2_pecu_err.pecu_err_2;
	bCarError[3][4]=stRecvpacket.err_t.c2_pecu_err.pecu_err_3;

	//动态地图
	bCarError[3][5]=stRecvpacket.err_t.c2_drmd_err1.drmd_err_1;
	bCarError[3][6]=stRecvpacket.err_t.c2_drmd_err1.drmd_err_2;
	bCarError[3][7]=stRecvpacket.err_t.c2_drmd_err1.drmd_err_3;
	bCarError[3][8]=stRecvpacket.err_t.c2_drmd_err1.drmd_err_4;
	bCarError[3][9]=stRecvpacket.err_t.c2_drmd_err1.drmd_err_5;
	bCarError[3][10]=stRecvpacket.err_t.c2_drmd_err1.drmd_err_6;
	bCarError[3][11]=stRecvpacket.err_t.c2_drmd_err1.drmd_err_7;
	bCarError[3][12]=stRecvpacket.err_t.c2_drmd_err1.drmd_err_8;
	bCarError[3][13]=stRecvpacket.err_t.c2_drmd_err2.drmd_err_9;
	bCarError[3][14]=stRecvpacket.err_t.c2_drmd_err2.drmd_err_10;

	//b2
	bCarError[4][1]=stRecvpacket.err_t.car_err.b2_lcu_err;
	//报警器
	bCarError[4][2]=stRecvpacket.err_t.b2_pecu_err.pecu_err_1;
	bCarError[4][3]=stRecvpacket.err_t.b2_pecu_err.pecu_err_2;
	bCarError[4][4]=stRecvpacket.err_t.b2_pecu_err.pecu_err_3;

	//动态地图
	bCarError[4][5]=stRecvpacket.err_t.b2_drmd_err1.drmd_err_1;
	bCarError[4][6]=stRecvpacket.err_t.b2_drmd_err1.drmd_err_2;
	bCarError[4][7]=stRecvpacket.err_t.b2_drmd_err1.drmd_err_3;
	bCarError[4][8]=stRecvpacket.err_t.b2_drmd_err1.drmd_err_4;
	bCarError[4][9]=stRecvpacket.err_t.b2_drmd_err1.drmd_err_5;
	bCarError[4][10]=stRecvpacket.err_t.b2_drmd_err1.drmd_err_6;
	bCarError[4][11]=stRecvpacket.err_t.b2_drmd_err1.drmd_err_7;
	bCarError[4][12]=stRecvpacket.err_t.b2_drmd_err1.drmd_err_8;
	bCarError[4][13]=stRecvpacket.err_t.b2_drmd_err2.drmd_err_9;
	bCarError[4][14]=stRecvpacket.err_t.b2_drmd_err2.drmd_err_10;

	//a2
	bCarError[5][0]=stRecvpacket.err_t.car_err.a2_pis_err;
	bCarError[5][1]=stRecvpacket.err_t.car_err.b2_lcu_err;
	//报警器
	bCarError[5][2]=stRecvpacket.err_t.a2_pecu_err.pecu_err_1;
	bCarError[5][3]=stRecvpacket.err_t.a2_pecu_err.pecu_err_2;
	bCarError[5][4]=stRecvpacket.err_t.a2_pecu_err.pecu_err_3;

	//动态地图
	bCarError[5][5]=stRecvpacket.err_t.a2_drmd_err1.drmd_err_1;
	bCarError[5][6]=stRecvpacket.err_t.a2_drmd_err1.drmd_err_2;
	bCarError[5][7]=stRecvpacket.err_t.a2_drmd_err1.drmd_err_3;
	bCarError[5][8]=stRecvpacket.err_t.a2_drmd_err1.drmd_err_4;
	bCarError[5][9]=stRecvpacket.err_t.a2_drmd_err1.drmd_err_5;
	bCarError[5][10]=stRecvpacket.err_t.a2_drmd_err1.drmd_err_6;
	bCarError[5][11]=stRecvpacket.err_t.a2_drmd_err1.drmd_err_7;
	bCarError[5][12]=stRecvpacket.err_t.a2_drmd_err1.drmd_err_8;
	bCarError[5][13]=stRecvpacket.err_t.a2_drmd_err2.drmd_err_9;
	bCarError[5][14]=stRecvpacket.err_t.a2_drmd_err2.drmd_err_10;

	bCarError[5][15]=stRecvpacket.err_t.car_err2.a2_dst_led_err;
	bCarError[5][16]=stRecvpacket.err_t.car_err2.a2_box_led_err;

	for (int i=0; i<6; i++)//6节车厢
	{
		for (int j=0;j<6; j++)
		{
			if (j<3)
			{
				BH_SetCarVerticalState(i, j+1, bCarState[i][j]==1?Enum_Alarm:Enum_Normal);
			}
			else if (j>=3)
			{
				BH_SetCarVerticalState(i, j+1, bCarState[i][j]==1?Enum_PECU:Enum_Normal);
			}

		}	

		for (int k=0; k<17; k++)
		{
			BH_SetCarVerticalState(i, k+8, bCarError[i][k]==1?Enum_FAULT:Enum_Normal);
		}
	}
}
/*
static void OnDataViewDisPlay(DWORD dwItem, void *pView, void *pVoid)
{
#define _HighlightElement(a, b) for (int n = a; n <= b; n++) TMS_SetElementHighlight(pView, n);
	CString strDisplay = _T("No item selected.");
	switch (dwItem)
	{
	case 0:
		_HighlightElement(0, 0);
		strDisplay.Format(_T("First %02X"), TMS_GetViewDataByte(pView, 0));
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
				TMS_GetViewDataByte(pView, 1), TMS_GetViewDataByte(pView, 2), TMS_GetViewDataByte(pView, 3), TMS_GetViewDataByte(pView, 4),
				TMS_GetViewDataByte(pView, 5), TMS_GetViewDataByte(pView, 6), TMS_GetViewDataByte(pView, 7), TMS_GetViewDataByte(pView, 8));
		}
		break;
	case 9:
	case 10:
		_HighlightElement(9, 10);
		strDisplay.Format(_T("life_t : %02X %02X"), TMS_GetViewDataByte(pView, 9), TMS_GetViewDataByte(pView, 10));
		break;
	case 11:
	case 12:
		_HighlightElement(11, 12);
		strDisplay.Format(_T("TCMS_VER_EH_T %02X %02X"), TMS_GetViewDataByte(pView, 11), TMS_GetViewDataByte(pView, 12));
		break;
	case 13:
	case 14:	
	case 15:
	case 16:
	case 17:
	case 18:
		_HighlightElement(13, 18);
#if BIG_ENDIAN_FH
		strDisplay.Format(_T("Date Time: %04d-%02d-%02d %02d:%02d:%02d"), TMS_GetViewDataByte(pView, 13)+2000, TMS_GetViewDataByte(pView, 14), TMS_GetViewDataByte(pView, 15), 
			TMS_GetViewDataByte(pView, 16),	TMS_GetViewDataByte(pView, 17), TMS_GetViewDataByte(pView, 18));
#else
		strDisplay.Format(_T("Date Time: %04d-%02d-%02d %02d:%02d:%02d"), TMS_GetViewDataByte(pView, 14)+2000, TMS_GetViewDataByte(pView, 13), TMS_GetViewDataByte(pView, 16), 
			TMS_GetViewDataByte(pView, 15),	TMS_GetViewDataByte(pView, 18), TMS_GetViewDataByte(pView, 17));
#endif
		break;
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:		
	case 27:
	case 28:
		_HighlightElement(19, 28);
		strDisplay.Format(_T("Time reserve"));
		break;
	case 29:
		{
			_HighlightElement(29, 29);
			BYTE val = TMS_GetViewDataByte(pView, 29);		
			strDisplay.Format(_T("signal_valid_t door_open_valid: %d\n"), val&1);
			strDisplay.AppendFormat(_T("signal_valid_t end_station_valid %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("signal_valid_t next_station_valid %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("signal_valid_t current_station_valid %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("signal_valid_t distance_to_next_valid %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("signal_valid_t distance_from_last_valid %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("signal_valid_t year_valid %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("signal_valid_t month_valid %d \n"), val>>7&1);
		}
		break;
	case 30:
		{
			_HighlightElement(30, 30);
			BYTE val = TMS_GetViewDataByte(pView, 30);		
			strDisplay.Format(_T("time_valid_t date_valid: %d\n"), val&1);
			strDisplay.AppendFormat(_T("signal_valid_t hour_valid %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("signal_valid_t minute_valid %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("signal_valid_t second_valid %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("signal_valid_t reserve %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("signal_valid_t broadcast_test_valid %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("signal_valid_t reserve2 %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("signal_valid_t signal_valid %d \n"), val>>7&1);
		}
		break;
	case 31:
		_HighlightElement(31, 31);
		strDisplay.Format(_T("door_open_next: %d\n"), TMS_GetViewDataByte(pView, 31));
		break;
	case 32://生命信号
		{		
			_HighlightElement(32, 32);
			strDisplay.Format(_T("life %02X"), TMS_GetViewDataByte(pView, 32));			
		}
		break;
	case 33:
	case 34:
	case 35:
	case 36:
		{
			_HighlightElement(33, 36);
			strDisplay.Format(_T("end_station "));
		}
		break;
	case 37:
	case 38:
	case 39:
	case 40:
		_HighlightElement(37, 40);
		strDisplay.Format(_T("next_station"));
		break;
	case 41:
	case 42:
	case 43:
	case 44:
		_HighlightElement(41, 44);
		strDisplay.Format(_T("current_station"));
		break;
	case 45:
	case 46:
	case 47:
	case 48:
		_HighlightElement(45, 48);
		strDisplay.Format(_T("distance_to_next  %d \ndistance_from_last %d"),
			(TMS_GetViewDataByte(pView, 46)<<8)|(TMS_GetViewDataByte(pView, 45)), 
			(TMS_GetViewDataByte(pView, 48)<<8)|(TMS_GetViewDataByte(pView, 47)));
		break;	
	case 49:		
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
		_HighlightElement(49, 54);
		strDisplay.Format(_T("Date Time: %04d-%02d-%02d %02d:%02d:%02d"), TMS_GetViewDataByte(pView, 50)+2000, 
			TMS_GetViewDataByte(pView, 49), TMS_GetViewDataByte(pView, 52), 
			TMS_GetViewDataByte(pView, 51),	TMS_GetViewDataByte(pView, 54), TMS_GetViewDataByte(pView, 53));
		break;
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
		_HighlightElement(55, 60);
		strDisplay.Format(_T("ATC_1E8H_T End"));
		break;
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
	case 91:
	case 92:
		_HighlightElement(61, 92);
		strDisplay.Format(_T("ATC端口信号 6E8H"));
		break;
	case 93:	
		_HighlightElement(93, 93);
		{
			BYTE val = TMS_GetViewDataByte(pView, 93);
			strDisplay.Format(_T("leave_broadcast: %d\n"), val&1);
			strDisplay.AppendFormat(_T("arrive_broadcast:    %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("cancel_broadcast:    %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("emergency_broadcast: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("skip_pre_station:    %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("skip_next_station:   %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("cycle_broadcast:     %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("run_chg:             %d \n"), val>>7&1);
		}
		break;
	case 94:
		{
			_HighlightElement(94, 94);
			BYTE val = TMS_GetViewDataByte(pView, 94);
			strDisplay.Format(_T("a1_active:        %d \n"), val&1);
			strDisplay.AppendFormat(_T("a2_active:  %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("a1_link:    %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("a2_link:	%d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("left_door_open:    %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("right_door_open:   %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("left_alldoor_closed:     %d \n"), val>>6&1);
			strDisplay.AppendFormat(_T("right_alldoor_closed:    %d \n"), val>>7&1);
			break;
		}
		break;	
	case 95:	
		break;
	case 96:
		{
			_HighlightElement(96, 96);
			BYTE val = TMS_GetViewDataByte(pView, 95);
			strDisplay.Format(_T("left_door_close: %d\n"), val&1);
			strDisplay.AppendFormat(_T("right_door_close: %d \n"), val>>1&1);
			strDisplay.AppendFormat(_T("ato_active: %d \n"), val>>2&1);
			strDisplay.AppendFormat(_T("cm_active: %d \n"), val>>3&1);
			strDisplay.AppendFormat(_T("atp_off: %d \n"), val>>4&1);
			strDisplay.AppendFormat(_T("auto_man_mode: %d \n"), val>>5&1);
			strDisplay.AppendFormat(_T("hmi_led_valid: %d \n"), val>>6&1);
		}
		break;
	case 97:
	case 98:
		_HighlightElement(97, 98);
		strDisplay.Format(_T("leave_broadcast: %d\n"), TMS_GetViewDataByte(pView, 98)<<8|TMS_GetViewDataByte(pView, 97));
		break;
	case 99:
	case 100:
		_HighlightElement(99, 100);
		strDisplay.Format(_T("start_id_h: %d\n"), TMS_GetViewDataByte(pView, 100)<<8|TMS_GetViewDataByte(pView, 99));
		break;
	case 101:
	case 102:
		_HighlightElement(101, 102);
		strDisplay.Format(_T("end_id_h: %d\n"), TMS_GetViewDataByte(pView, 102)<<8|TMS_GetViewDataByte(pView, 101));
		break;
	case 103:
	case 104:
		_HighlightElement(103, 104);
		strDisplay.Format(_T("next_id_h: %d\n"), TMS_GetViewDataByte(pView, 104)<<8|TMS_GetViewDataByte(pView, 103));
		break;
	case 105:
	case 106:
		_HighlightElement(105, 106);
		strDisplay.Format(_T("line_id_h: %d\n"), TMS_GetViewDataByte(pView, 106)<<8|TMS_GetViewDataByte(pView, 105));
		break;
	case 107:
	case 108:
		_HighlightElement(107, 108);
		strDisplay.Format(_T("route_id_h: %d\n"), TMS_GetViewDataByte(pView, 108)<<8|TMS_GetViewDataByte(pView, 107));
		break;
	case 109:
	case 110:
		_HighlightElement(109, 110);
		strDisplay.Format(_T("speed_h: %d\n"), TMS_GetViewDataByte(pView, 110)<<8|TMS_GetViewDataByte(pView, 109));
		break;
	case 111:
	case 112:
	case 113:
	case 114:
	case 115:
	case 116:
	case 117:
	case 118:
	case 119:
	case 120:
	case 121:
	case 122:
	case 123:
	case 124:
		_HighlightElement(111, 124);
		strDisplay.Format(_T("TCMS端口信号 170H"));		
		break;
	case 125:
	case 126:
	case 127:
	case 128:
	case 129:
	case 130:
	case 131:
	case 132:
	case 133:
	case 134:
	case 135:
	case 136:
	case 137:
	case 138:
	case 139:
	case 140:
	case 141:
	case 142:
	case 143:
	case 144:
	case 145:
	case 146:
	case 147:
	case 148:
	case 149:
	case 150:
	case 151:
	case 152:
	case 153:
	case 154:
	case 155:
	case 156:
		_HighlightElement(125, 156);
		strDisplay.Format(_T("TCMS端口信号 TCMS端口信号 171H"));
		break;
	default:
		break;
	};
#undef _HighlightElement

	TMS_SetDataTextToStatic(strDisplay.GetBuffer(0));
}*/