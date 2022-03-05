/******************************************************************************

版权所有 (C), 2014-2020, 鼎飞科技(TopFlyTech)有限公司

 ******************************************************************************
  文 件 名   : Ota_constant.h
  版 本 号   : 初稿
  作    者   : Ben.You
  生成日期   : 2016年7月8日
  最近修改   :
  功能描述   : 宏常量定义，本文件由OTA方面控制, OTA可以进行修改
  函数列表   :
  修改历史   :
  1.日    期   : 2016年7月8日
    作    者   : Ben.You
    修改内容   : 创建文件

******************************************************************************/
#ifndef __PUB_CONSTANT_OTA_H__
#define __PUB_CONSTANT_OTA_H__

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#if defined(__GT_SUPPORT__)

/****************************************************************
功能开关:
__SERVER_LOCK__                            TODO
****************************************************************/
//#define __GPS_NET_TEST__                     // gps测试模式，使用gprs发送gps的具体信息
//#define __GT_NOSIM_NO12V_SHUTDOWN__          //无sim卡，检测12v 移除，3s关机。
//#define __GT_NOSIM_12V_SHUTDOWN__              //无sim卡，有12V, 5分钟关机
#define __GT_GSENSOR_CONTROL_GPS__           //检查GSENSOR 静止或运动，从而控制gps开关
#define __GPS_AUTO_UPDATE_TIME__             //gps定位成功时，自动更新时间
#define __SMS_MODE_SUPPORT__                 //支持sms模式设置
#define __CHECK_HEART_REPLY__                //检查心跳回复，10秒内未收到心跳回复，则重连网络
#define __CHECK_ALARM_REPLY__                //检查报警回复，30秒内未收到报警回复，则发短信、(sos)打电话
#define __GEOFENCE_SUPPORT__                 //电子围栏功能
#define __WPS_DOWNLOAD__                     //ota下载
//#define __SERVER_LOCK__                                  //锁定服务器的功能(锁定成功后登陆加密)
#define __FACTORY_TEST_MODE__                //工程模式测试
#define __LED_DISPLAY__                      //灯效控制
#define __GPS_REBOOT__                       //gps工作但是10分钟定不到位，复位gps芯片
#define __SOCKET_REBOOT__                    //网络工作，但是10分钟连接不上网络，则开关飞行模式
//#define __UBLOX_AGPS_SUPPORT__               //AGPS功能
#ifndef __UBLOX_SUPPORT__
//#define __UBLOX_SUPPORT__                    //UBLOX功能
#endif
//#define __MPU6500_GYROSCOPE_SUPPORT__        //GYRO功能
#define __BMX055_GYROSCOPE_SUPPORT__         //GYRO BMX055功能
//#define __BMA253_GSENSOR_SUPPORT__             // GSENSOR_BMA253功能
//#define __MC3413_GSENSOR_SUPPORT__             // GSENSOR_MC3413功能
//#define __UBLOX_AGPS_OFFLINE_SUPPORT__       // AGPS数据自动下载
#define __HISTORY_DATA_SAVE__                //历史数据的保存和发送，包括报警数据
#define __SOCKET_ACK_MNGR__      // socket 握手功能
#define __MULTIPLE_SERVERS_SUPPORT__      // 多IP上传数据功能
#define __OTA_IP3_SUPPORT__
//#define __CUSTOM_ALARM_MESSAGES__      // 自定义报警短信
#define __SOCKET_CONNECTION_MNGR__      // socket长连接短连接控制功能
#define __SMS_DATA_SAVE__               // 保存发送的信息，防止因重启等原因导致信息丢失
//#define __WATCH_DOG_SUPPORT__
#define __SECURITY_SUPPORT__
//#define __SOCKET_DATA_ENCRYPTION_SUPPORT__
//#define __BLUETOOTH_CERTIFICATION__   //Bluetooth认证策略
//#define __OTA_BLUETOOTH_AT_SUPPORT__ //BT 实现AT发送
#define __SOCKET_OSM_GOOGLE_ADDR_REQUEST__  //OSM ADDR REQUEST功能
#define __FLIGHT_MODE_SWITCH_FUNC__
//#define __MTK_GPS_AGPS_SUPPORT__               //GPS AGPS功能
//#define __OTA_TRACE_DEBUG_SUPPORT__
#define __CLOSE_SOME_FUNC_TO_REDUCE_OTA_ROM_SIZE__
//#define __MTK_GPS_POWER_SAVE_MODE_SUPPORT__
#define __BATCH_WRITE_NV_PARAMETER_FUNC__
#define __OTA_SET_PROTOCOL_AND_ACK_CONTROL__

#define __OTA_MODEM_SUPPORT__
#define __OTA_MODEM_AT_PC_MODE__
#define __OTA_BG96_DEBUG__
#define __OTA_ZIP_SUPPORT__
#define __OTA_DBG_CMD__
#define __OTA_MQTT_SUPPORT__
#define __OTA_BT_UL_SUPPORT__
#ifndef __PORTABLE_DEVICE__
#define __DRIVING_BEHAVIOR_MNGR__      // 驾驶行为数据管理
#endif
//外接GPS模块
#define __L76_GPS__

//log s输出到spi port，只用于调试，量产必须关闭
//#define __NEW_DBG_PORT__
//#define __OTA_UPGRADE_NO_CHECK__
//部分历史文件存到spi flash FATFS
#define __USE_SPI_FS__
#define __USE_CMD_TASK__
//#define __OTA_ACCE_CONVERT_PLANE__
//#define __OTA_DEBUG_ATC_CALLBACK__
#endif   // __GT_SUPPORT__

/****************************************************************
日志打印开关:
__TRACE_TO_NET__                      可以通过命令打开
****************************************************************/
//#define __TRACE_TO_NET__                //trace log 打印到网络上 ,以便远程调试


/*--OTA 版本控制--------------------------------------------------*/

#ifdef __SMART_LOCK_SUPPORT__
#define OTA_SW_VERSION          0x1000    //tc015x 的ota文件的版本号
#elif defined(__PORTABLE_DEVICE__)
#define OTA_SW_VERSION          0x1101    //ota文件的版本号，显示为1.1.2，前面的1为固定的,同时修改ota_common_get_cmd_reply_string_version和ota_gt_pac_login_data
#elif defined(__OTA_OBD_SUPPORT__)
#define OTA_SW_VERSION          0x126
#elif defined(__WIRED_DEVICE__)
#define OTA_SW_VERSION          0x1113
#else
#define OTA_SW_VERSION          0x426
#endif


/*--定义消息的multi-handler的标志--
  --OTA中没有multi-handler的消息注册机制, 但是为了能够使用multi-handler,
  --所以弄了个标志位来处理, 对于同一个消息ID, 最多可以定义32个标志,
  --也就是最多可以执行32个回调(这里的回调其实就是在同一个函数中,
  --同一个消息下的不同的if语句中执行的语句块, 至于为什么是32, 是因为
  --系统定义的OTA_MSG_MULTI_FLAG数据为unsigned int型, 也就是32个bit.)
*/
#define ILM_DBG_START_TIMER   (0x01 << 0)
#define ILM_DBG_FREE_SOCMNGR  (0x01 << 1)
#define ILM_DBG_POST_TEXT_SMS (0x01 << 2)
#define ILM_DBG_POST_DATA_SMS (0x01 << 3)


#define PROTOCOL_SOCKET_DATA_HEAD_LEN     15
#define PROTOCOL_SOCKET_TYPE_LOGIN        0x01
#ifndef __WIRED_DEVICE__
#define PROTOCOL_SOCKET_TYPE_GPS          0x02
#define PROTOCOL_SOCKET_TYPE_GPS_2        0x16
#else
#define PROTOCOL_SOCKET_TYPE_GPS          0x13
#endif
#define PROTOCOL_SOCKET_TYPE_HEARTBEAT    0x03
#ifndef __WIRED_DEVICE__
#define PROTOCOL_SOCKET_TYPE_ALARM        0x04
#define PROTOCOL_SOCKET_TYPE_ALARM_2      0x18
#else
#define PROTOCOL_SOCKET_TYPE_ALARM        0x14
#endif

/*协议常量定义*/
/*协议常量定义*/
#ifdef __OTA_OBD_SUPPORT__

#define PROTOCOL_SOCKET_DATA_HEAD              0x2626
#define OTA_PROTOCOL_DATA_FIRST_BYTE           0X26
#define PROTOCOL_SOCKET_TYPE_OBD               0x09
#define PROTOCOL_OX82_SOCKET_TYPE_OBD          0x82
#define OTA_0X8201_PACKET_DATA_LEN             16
#define HISTORY_OBD_MAX_FILE_SIZE              10240
#define HISTORY_OBD_MAX_FILE_INDEX             100
#define OTA_OBD_HISTORY_ITEM_HEAD_LEN          3
#define OTA_OBD_CMD_HEAD_LENGTH_COMMAND        6
#define OTA_OBD_CMD_HEAD_LENGTH_TAIL           6
#define OTA_OBD_PACKET_DATA_BUFFER_LEN         1300
#define OTA_PACKET_OBD_UPGRADE_DATA_BUFFER_LEN 1024
#define OTA_OBD_CMD_FIFO_QUEUE_NUMBER          10000
#elif __PORTABLE_DEVICE__
#define PROTOCOL_SOCKET_DATA_HEAD     0x2727
#define OTA_PROTOCOL_DATA_FIRST_BYTE  0X27
#else
#define PROTOCOL_SOCKET_DATA_HEAD     0x2525
#define OTA_PROTOCOL_DATA_FIRST_BYTE  0X25
#endif


#define PROTOCOL_SOCKET_TYPE_0X05         0x05
#define PROTOCOL_SOCKET_TYPE_0X06         0x06
#define PROTOCOL_SOCKET_TYPE_0X07         0x07

#define PROTOCOL_SOCKET_TYPE_CURR_POS     0x06

#ifdef __OTA_BT_UL_SUPPORT__
#define PROTOCOL_SOCKET_TYPE_BTSPP        0x10
#define PROTOCOL_SOCKET_TYPE_BTSPP_2      0x12

#endif // __OTA_BT_UL_SUPPORT__

#define PROTOCOL_SOCKET_TYPE_0X11         0x11
#ifdef __SMART_LOCK_SUPPORT__
#define PROTOCOL_SOCKET_TYPE_0X14         0x14  //用于开锁、关锁事件上报
#endif
#define PROTOCOL_SOCKET_TYPE_0X15         0x15

#define PROTOCOL_SOCKET_TYPE_MAX          0x15

#define PROTOCOL_SOCKET_TYPE_SETTING      0x81
#define OTA_VAR_LEN_HISTORY_ITEM_HEAD_LEN      3

#define OTA_SMS_LEN_MAX    198

#ifdef __OTA_BT_UL_SUPPORT__
#define PROTOCOL_OX83_SOCKET_TYPE_BTSPP   0x83
#define HISTORY_BTSPP_MAX_FILE_SIZE       10240
#define HISTORY_BTSPP_MAX_FILE_INDEX      90
#define OTA_BTSPP_PACKET_DATA_BUFFER_LEN  1300
#define OTA_BTSPP_HISTORY_ITEM_HEAD_LEN   3
#define OTA_BTSPP_PACKET_DATA_BUFFER_LEN_AES  1311 // ((1300-15)/16 + 1)*16 + 15
#define OTA_BTSPP_PACKET_DATA_BUFFER_LEN_MD5  1308
#endif // __OTA_BT_UL_SUPPORT__


#ifdef __TRACE_TO_NET__
#define HISTORY_LOG_MAX_FILE_SIZE       10240
#define HISTORY_LOG_MAX_FILE_INDEX      10
#define HISTORY_LOG_PACKET_SEND_SIZE     1340
#endif

#define PROTOCOL_0X81_TYPE_SETTING       0x01
#define PROTOCOL_0X81_TYPE_BROADCAST     0x02
#define PROTOCOL_0X81_TYPE_FORWARD       0x03
#define PROTOCOL_0X81_TYPE_LOCKSERVER    0x04
#define PROTOCOL_0X81_TYPE_BILL_CHECK    0x05

#define PROTOCOL_LOG_DATA_HEAD           0x2424
#define OTA_PROTOCOL_LOG_FIRST_BYTE      0X24
#define PROTOCOL_LOG_TYPE_GPS            0X01
#define PROTOCOL_LOG_TYPE_SYSTEM         0X02
#define PROTOCOL_LOG_TYPE_OTA            0X03
#define PROTOCOL_LOG_TYPE_ASSERT         0X04

#if defined(__PORTABLE_DEVICE__) || defined(__WIRED_DEVICE__)
#define PROTOCOL_ALARM_TYPE_UNMOUNT                 0x01         // 拆机
#define PROTOCOL_ALARM_TYPE_OPENCOVER               0x02         // 拆盖
#define PROTOCOL_ALARM_TYPE_SOS                     0x03         // SOS
#define PROTOCOL_ALARM_TYPE_OPENBOX                 0x04         // 拆箱
#define PROTOCOL_ALARM_TYPE_FREEFALL                0x05         // 跌落
#define PROTOCOL_ALARM_TYPE_LOW_BATTERY             0x06
#define PROTOCOL_ALARM_TYPE_LOW_BATTERY_PRESET      0x07         // 低电恢复
#define PROTOCOL_ALARM_TYPE_HIGH_TEMP               0x08         //机内温度过高警报
#define PROTOCOL_ALARM_TYPE_MOTION                  0x09         // 移动
#define PROTOCOL_ALARM_TYPE_COLLSION                0x10
#define PROTOCOL_ALARM_TYPE_TILT                    0x11         // 倾斜
#define PROTOCOL_ALARM_TYPE_USB_CHARGING            0x12         // 外接usb充电连接
#define PROTOCOL_ALARM_TYPE_USB_CHARGE_REMOVE       0x13         // 外接usb充电断开
#define PROTOCOL_ALARM_TYPE_FENCE_IN                0x14
#define PROTOCOL_ALARM_TYPE_FENCE_OUT               0x15
#define PROTOCOL_ALARM_TYPE_ACC_ON                  0x16
#define PROTOCOL_ALARM_TYPE_ACC_OFF                 0x17
#define PROTOCOL_ALARM_TYPE_IDLE_ON                 0x18
#define PROTOCOL_ALARM_TYPE_IDLE_OFF                0x19
#define PROTOCOL_ALARM_TYPE_POWERON                 0x20
#define PROTOCOL_ALARM_TYPE_UNMOUNT_REC             0x21         // 拆机
#define PROTOCOL_ALARM_TYPE_OPENCOVER_REC           0x22         // 拆盖
#define PROTOCOL_ALARM_TYPE_OPENBOX_REC             0x23         // 拆箱
#define PROTOCOL_ALARM_TYPE_FREEFALL_REC            0x24         // 跌落
#define PROTOCOL_ALARM_TYPE_HIGH_TEMP_REC           0x25         //机内温度过高警报
#define PROTOCOL_ALARM_TYPE_MOTION_REC              0x26         // 移动
#define PROTOCOL_ALARM_TYPE_COLLSION_REC            0x27
#define PROTOCOL_ALARM_TYPE_TILT_REC                0x28         // 倾斜
#define PROTOCOL_ALARM_TYPE_POWEROFF                0x29
#define PROTOCOL_ALARM_TYPE_LOW_TEMP                0x30
#define PROTOCOL_ALARM_TYPE_LOW_TEMP_REC            0x31

#define PROTOCOL_ALARM_TYPE_MAX_GENERAL             40

#define PROTOCOL_ALARM_TYPE_DIN1_ON                 0x40
#define PROTOCOL_ALARM_TYPE_DIN1_OFF                0x41
#define PROTOCOL_ALARM_TYPE_DIN2_ON                 0x42
#define PROTOCOL_ALARM_TYPE_DIN2_OFF                0x43
#define PROTOCOL_ALARM_TYPE_DIN3_ON                 0x44
#define PROTOCOL_ALARM_TYPE_DIN3_OFF                0x45
#define PROTOCOL_ALARM_TYPE_DIN4_ON                 0x46
#define PROTOCOL_ALARM_TYPE_DIN4_OFF                0x47
#define PROTOCOL_ALARM_TYPE_DIN5_ON                 0x48
#define PROTOCOL_ALARM_TYPE_DIN5_OFF                0x49
#define PROTOCOL_ALARM_TYPE_DIN6_ON                 0x50
#define PROTOCOL_ALARM_TYPE_DIN6_OFF                0x51
#define PROTOCOL_ALARM_TYPE_DIN7_ON                 0x52
#define PROTOCOL_ALARM_TYPE_DIN7_OFF                0x53
#define PROTOCOL_ALARM_TYPE_ADD_ADC_1               0x54
#define PROTOCOL_ALARM_TYPE_ADD_REC_ADC_1           0x55
#define PROTOCOL_ALARM_TYPE_REDUCE_ADC_1            0x56
#define PROTOCOL_ALARM_TYPE_REDUCE_REC_ADC_1        0x57
#define PROTOCOL_ALARM_TYPE_ADD_ADC_2               0x58
#define PROTOCOL_ALARM_TYPE_ADD_REC_ADC_2           0x59
#define PROTOCOL_ALARM_TYPE_REDUCE_ADC_2            0x60
#define PROTOCOL_ALARM_TYPE_REDUCE_REC_ADC_2        0x61
#define PROTOCOL_ALARM_TYPE_ADD_ADC_3               0x62
#define PROTOCOL_ALARM_TYPE_ADD_REC_ADC_3           0x63
#define PROTOCOL_ALARM_TYPE_REDUCE_ADC_3            0x64
#define PROTOCOL_ALARM_TYPE_REDUCE_REC_ADC_3        0x65
#define PROTOCOL_ALARM_TYPE_JAMMERD_ENTER           0x66
#define PROTOCOL_ALARM_TYPE_JAMMERD_EXIT            0x67
#define PROTOCOL_ALARM_TYPE_CUT_12V                 0x68
#define PROTOCOL_ALARM_TYPE_RECOVER_12V             0x69
#define PROTOCOL_ALARM_TYPE_HIGH_SPEED              0x70
#ifndef __WIRED_DEVICE__
#define PROTOCOL_ALARM_TYPE_HIGH_SPEED_REC          0x71
#define PROTOCOL_ALARM_TYPE_DRAG                    0x72
#define PROTOCOL_ALARM_TYPE_VIBRATION               0x73
#define PROTOCOL_ALARM_TYPE_GOOGLE                  0x74
#define PROTOCOL_ALARM_TYPE_ANTITHEFT               0x75
#define PROTOCOL_ALARM_TYPE_EXT_POWER_LOWER_PRESET  0x76
#define PROTOCOL_ALARM_TYPE_MAX                     76
#define PROTOCOL_ALARM_TYPE_ORI_MAX                 76
#else
#define PROTOCOL_ALARM_TYPE_DRAG                    0x71
#define PROTOCOL_ALARM_TYPE_VIBRATION               0x72
#define PROTOCOL_ALARM_TYPE_GOOGLE                  0x73
#define PROTOCOL_ALARM_TYPE_ANTITHEFT               0x74
#define PROTOCOL_ALARM_TYPE_EXT_POWER_LOWER_PRESET  0x75
#define PROTOCOL_ALARM_TYPE_SHORT_STOP              0x76
#define PROTOCOL_ALARM_TYPE_ORI_MAX                 75
#define PROTOCOL_ALARM_TYPE_MAX                     76
#endif
#define PROTOCOL_EVENT_TYPE_MAX_WIRED               (PROTOCOL_ALARM_TYPE_MAX-PROTOCOL_ALARM_TYPE_MAX_GENERAL)
#else
#define PROTOCOL_ALARM_TYPE_CUT_12V                 0x01
#define PROTOCOL_ALARM_TYPE_LOW_BATTERY             0x02
#define PROTOCOL_ALARM_TYPE_SOS                     0x03
#define PROTOCOL_ALARM_TYPE_HIGH_SPEED              0x04
#define PROTOCOL_ALARM_TYPE_FENCE_IN                0x05
#define PROTOCOL_ALARM_TYPE_FENCE_OUT               0x06
#define PROTOCOL_ALARM_TYPE_DRAG                    0x07
#define PROTOCOL_ALARM_TYPE_VIBRATION               0x08
#define PROTOCOL_ALARM_TYPE_GOOGLE                  0x09
#define PROTOCOL_ALARM_TYPE_ANTITHEFT               0x10
#define PROTOCOL_ALARM_TYPE_ADD_ADC_1               0x11
#define PROTOCOL_ALARM_TYPE_REDUCE_ADC_1            0x12
#define PROTOCOL_ALARM_TYPE_ADD_ADC_2               0x13
#define PROTOCOL_ALARM_TYPE_REDUCE_ADC_2            0x14
#define PROTOCOL_ALARM_TYPE_ACC_ON                  0x15
#define PROTOCOL_ALARM_TYPE_ACC_OFF                 0x16
#define PROTOCOL_ALARM_TYPE_DIN2_ON                 0x17
#define PROTOCOL_ALARM_TYPE_DIN2_OFF                0x18
#define PROTOCOL_ALARM_TYPE_IDLE_ON                 0x19
#define PROTOCOL_ALARM_TYPE_IDLE_OFF                0x20
#define PROTOCOL_ALARM_TYPE_JAMMERD_ENTER           0x21
#define PROTOCOL_ALARM_TYPE_JAMMERD_EXIT            0x22
#define PROTOCOL_ALARM_TYPE_RECOVER_12V             0x23
#define PROTOCOL_ALARM_TYPE_EXT_POWER_LOWER_PRESET  0x24
#define PROTOCOL_ALARM_TYPE_DIN3_ON                 0x25
#define PROTOCOL_ALARM_TYPE_DIN3_OFF                0x26
#define PROTOCOL_ALARM_TYPE_DIN4_ON                 0x27
#define PROTOCOL_ALARM_TYPE_DIN4_OFF                0x28
#define PROTOCOL_ALARM_TYPE_HIGH_SPEED_REC          0x29
#define PROTOCOL_ALARM_TYPE_SHORT_STOP              0x30
#define PROTOCOL_ALARM_TYPE_MAX                     30
#define PROTOCOL_ALARM_TYPE_ORI_MAX                 29
#endif


/*定位方式宏定义*/
#define STL_MODE_GPS_GLONASS      0
#define STL_MODE_GPS_GALILEO      1
#define STL_MODE_GPS_BEIDOU       2

/*报警语言宏定义*/
#define OTA_LANGUAGE_CHINESE          0
#define OTA_LANGUAGE_ENGLISH          1
#define OTA_LANGUAGE_FRENCH           2
#define OTA_LANGUAGE_SPANISH          3
#define OTA_LANGUAGE_GERMAN           4
#define OTA_LANGUAGE_PORTUGUESE       5
#define OTA_LANGUAGE_PERSIAN          6
#define OTA_LANGUAGE_INDONESIAN       7
#define OTA_LANGUAGE_RUSSIAN          8
#define OTA_LANGUAGE_TURKISH          9

/* 当前使用sorsen类型宏定义*/
#define OTA_GYROSCOPE_BMX055          0
#define OTA_GYROSCOPE_MPU6500         1
#define OTA_GYROSCOPE_BMA253          2
#define OTA_GYROSCOPE_BMA250E         3
#define OTA_GYROSCOPE_MC3413          4
#define OTA_GYROSCOPE_BMC156          5
#define OTA_GYROSCOPE_BMG160          6

#define OTA_DATA_ECP_TYPE_NONE        0
#define OTA_DATA_ECP_TYPE_MD5         1
#define OTA_DATA_ECP_TYPE_AES         2
#define OTA_DATA_ECP_TYPE_BLOWFISH    3

#define OTA_SET_ACK_TYPE_LOGIN        0x01
#ifndef __WIRED_DEVICE__
#define OTA_SET_ACK_TYPE_GPS          0x02
#else
#define OTA_SET_ACK_TYPE_GPS          0x2000
#endif
#define OTA_SET_ACK_TYPE_HEARTBEAT    0x04
#ifndef __WIRED_DEVICE__
#define OTA_SET_ACK_TYPE_ALARM        0x08
#else
#define OTA_SET_ACK_TYPE_ALARM        0x4000
#endif
#define OTA_SET_ACK_TYPE_0X05         0x10
#define OTA_SET_ACK_TYPE_0X06         0x20
#define OTA_SET_ACK_TYPE_0X07         0x40
#define OTA_SET_ACK_TYPE_BT_MAC       0x80
#define OTA_SET_ACK_TYPE_RS232        0x100
#define OTA_SET_ACK_TYPE_OBD          0x200
#define OTA_SET_ACK_TYPE_BTSPP        0x400
#define OTA_SET_ACK_TYPE_0X11         0x800
#define OTA_SET_ACK_TYPE_0X15         0x1000
#ifdef __SMART_LOCK_SUPPORT__
#define OTA_SET_ACK_TYPE_0X14         0x2000
#endif


#if defined(__PORTABLE_DEVICE__)
#define OTA_SET_ACK_TYPE_COMMON_ALL   (OTA_SET_ACK_TYPE_LOGIN \
                                    |OTA_SET_ACK_TYPE_HEARTBEAT \
                                    |OTA_SET_ACK_TYPE_ALARM)

#define OTA_SET_PROT_TYPE_COMMON_ALL   (OTA_SET_ACK_TYPE_LOGIN \
                                    |OTA_SET_ACK_TYPE_GPS \
                                    |OTA_SET_ACK_TYPE_HEARTBEAT \
                                    |OTA_SET_ACK_TYPE_ALARM \
                                    |OTA_SET_ACK_TYPE_0X05)
#else

#define OTA_SET_ACK_TYPE_COMMON_ALL   (OTA_SET_ACK_TYPE_LOGIN \
                                        |OTA_SET_ACK_TYPE_HEARTBEAT \
                                        |OTA_SET_ACK_TYPE_ALARM \
                                        |OTA_SET_ACK_TYPE_0X05 \
                                        |OTA_SET_ACK_TYPE_0X06 \
                                        |OTA_SET_ACK_TYPE_0X07 \
                                        |OTA_SET_ACK_TYPE_BTSPP)

#define OTA_SET_PROT_TYPE_COMMON_ALL   (OTA_SET_ACK_TYPE_LOGIN \
                                        |OTA_SET_ACK_TYPE_GPS \
                                        |OTA_SET_ACK_TYPE_HEARTBEAT \
                                        |OTA_SET_ACK_TYPE_ALARM \
                                        |OTA_SET_ACK_TYPE_0X05 \
                                        |OTA_SET_ACK_TYPE_0X06 \
                                        |OTA_SET_ACK_TYPE_0X07 \
                                        |OTA_SET_ACK_TYPE_BTSPP \
                                        |OTA_SET_ACK_TYPE_0X11)

#endif



/* 基本常量宏定义*/

#define OTA_FEATURE_DISABLE           0
#define OTA_FEATURE_ENABLE            1
#define OTA_FEATURE_IGNORE            0xFF

#define OTA_POST_MSG_TYPE_SINGLE      0x00
#define OTA_INT64U_MAX                0xffffffffffffffffULL


#define CSMCC_INTERNATIONAL_ADDR      145
#define OTA_ORIG_PIN_LOCK_CODE        "1234"
#define OTA_DEFAULT_APN_STRING        "T2001"

#define OTA_GEOFENCE_DATA_FILE_NAME   "geofencedata"
#define OTA_ALARM_SMS_DATA_FILE_NAME  "alarmsmspdu"

#define OTA_SMS_DATA_BEGIN_FLAG       "BGN"
#define OTA_GSENSOR_WM_TRIG_CNT       16

/*功能常量宏定义*/
#define RLYMODE_SAFE_MODE         0
#define RLYMODE_RUDE_MODE         1
#define RLYMODE_ALTERNATE_MODE    2  // RELAY = 1(切断油路)的情况下，relay连接断开2s切换一次
//当rlymode的值不小于3，且有(剪线，出电子围栏，位移，防盗)等情况发生时，
//自动断油，并rlymode的时间后恢复油路
#define RLYMODE_TIMER_MODE        3
#define RLYMODE_TIMER_MAX         255

#define OTA_USER_FLAG_IP1     0x01
#define OTA_USER_FLAG_IP2     0x02
#define OTA_USER_FLAG_ALL     0x03

#define OTA_USER_FLAG_ALL_FIRST_MQTT     0x07
#define OTA_USER_FLAG_ALL_FISRT_NO_MQTT  0x0B


#define OTA_PACKET_DATA_TYPE_GPS          0x01
#define OTA_PACKET_DATA_TYPE_ALARM        0x02
#define OTA_PACKET_DATA_TYPE_BEHAV_GPS    0x03
#define OTA_PACKET_DATA_TYPE_BEHAV_ACCE   0x04
#define OTA_PACKET_DATA_TYPE_ACCIDENT     0x05
#define OTA_PACKET_DATA_TYPE_BLUETOOTH    0x06
#define OTA_PACKET_DATA_TYPE_RS232        0x07
#define OTA_PACKET_DATA_TYPE_OBD          0x08
#define OTA_PACKET_DATA_TYPE_BTSPP        0x09
#define OTA_PACKET_DATA_TYPE_WIFI         0x0A
#ifdef __SMART_LOCK_SUPPORT__
#define OTA_PACKET_DATA_TYPE_LOCK         0x0B
#endif

#define OTA_PACKET_DATA_TYPE_BLE_PLUG     0xE1
#define OTA_PACKET_DATA_TYPE_RS232_PLUG   0xE2
#ifndef __WIRED_DEVICE__
#define OTA_LOGIN_PACKET_DATA_LEN        25
#else
#define OTA_LOGIN_PACKET_DATA_LEN        21
#endif
#define OTA_GPS_TEST_PACKET_DATA_LEN     500
#define OTA_HEARTBEAT_PACKET_DATA_LEN    15
#define OTA_0X8182_PACKET_DATA_LEN       16
#ifdef __OTA_OBD_SUPPORT__
#define OTA_0X82_PACKET_DATA_LEN         15
#endif // __OTA_OBD_SUPPORT__

#ifdef __OTA_BT_UL_SUPPORT__
#define OTA_0X83_PACKET_DATA_LEN         15
#endif // __OTA_BT_UL_SUPPORT__
#define OTA_LOCKSERVER_PACKET_DATA_LEN   17
#define OTA_GETCMD_PACKET_DATA_LEN       300
#ifndef __OTA_PROTOCOL_USE_OLD_VERSION__
#define OTA_LOGIN_PACKET_DATA_LEN_AES    31 // ((21-15)/16 + 1)*16 + 15
#define OTA_LOGIN_PACKET_DATA_LEN_MD5    29 //21+8

#define OTA_0X8182_PACKET_DATA_LEN_AES    31 // ((16-15)/16 + 1)*16 + 15
#define OTA_0X8182_PACKET_DATA_LEN_MD5    24 // 16+8
#endif

#define OTA_PLUGIN_PACKET_DATA_LEN        17

#define OTA_DATA_HEAD_WITHOUT_IMEI        7
#define OTA_DATA_BEGIN_WITHOUT_HEAD       15   // index begin with 0

//track模式，0为GPRS,1为非GPRS非SMS， 非0非1的情况下为短信模式
#define DATA_TRACKER_MODE_GPRS                  0
#define DATA_TRACKER_MODE_NO_TRACKER            1
#define DATA_TRACHER_MODE_SMS                   2 // 非0非1的情况下为短信模式，短信发送间隔为trake指令下发的值

#define OTA_AGPS_DOWNLOAD_MODE_OFFLINE   0
#define OTA_AGPS_DOWNLOAD_MODE_ONLINE    1
#define OTA_AGPS_DOWNLOAD_MODE_MIX       2

#define OTA_SAVE_AGPS_ONLINE_DOWNLOAD_TIME     1
#define OTA_SAVE_AGPS_OFFLINE_DOWNLOAD_TIME    2

#define OTA_MANAGE_NUMBER_NUMBER_MAX 4
#define OTA_GEOFENCE_NUMBER_MAX      64
#define OTA_BLUETOOTH_MAC_NUMBER_MAX 50
#define OTA_BLE_SCAN_RECORD_MAX      5

#define OTA_BT_CS_TYPE_NONE          0
#define OTA_BT_CS_TYPE_X             1
#define OTA_BT_CS_TYPE_Y             2
#define OTA_BT_CS_TYPE_Z             3

#define OTA_SOCKET_PACKET_SIZE_MAX      1390
#if defined(__OTA_OBD_SUPPORT__)
#define OTA_HISTORY_FLAG_INDEX           24

#define HISTORY_MAX_ITEM_INDEX           47500           //历史文件的最大个数
#define HISTORY_NUMBER_EACH_PACKET       15
#define HISTORY_NUMBER_EACH_FILE         78
#define HISTORY_MAX_FILE_SIZE            5928           //历史文件78条数据，
#define HISTORY_READ_PACKET_SIZE         1140
#define HISTORY_PACKET_SEND_SIZE         1245         //历史数据每次发送的数据包包含的15条数据
#define HISTORY_SIZE_EACH_ITEM           76
#define OTA_GPS_PACKET_DATA_LEN          83

#define HISTORY_READ_PACKET_SIZE_AES     1079 //83*13
#define HISTORY_NUMBER_EACH_PACKET_AES   13
#define HISTORY_PACKET_SEND_SIZE_AES     1235 // 95*13
#define OTA_GPS_PACKET_DATA_LEN_AES      95  //((83-15)/16 + 1)*16 + 15

#define HISTORY_READ_PACKET_SIZE_MD5     1176 //84*14
#define HISTORY_NUMBER_EACH_PACKET_MD5   14
#define HISTORY_PACKET_SEND_SIZE_MD5     1274 // 14*(83 + 8)
#define OTA_GPS_PACKET_DATA_LEN_MD5      91


#define HISTORY_MAX_ALARM_ITEM_INDEX     4000          //历史文件最大个数
#define HISTORY_ALARM_NUMBER_EACH_FILE   40
#define HISTORY_MAX_ALARM_FILE_SIZE      3040         //alarm文件40条数据
#define HISTORY_ALARM_SIZE_EACH_ITEM     76

#elif defined(__PORTABLE_DEVICE__)
#define OTA_HISTORY_FLAG_INDEX           15

#define HISTORY_MAX_ITEM_INDEX           49000           //历史文件的最大个数
#define HISTORY_NUMBER_EACH_PACKET       18
#define HISTORY_NUMBER_EACH_FILE         90
#define HISTORY_MAX_FILE_SIZE            5940          //历史文件90条数据，
#define HISTORY_READ_PACKET_SIZE         1188
#define HISTORY_PACKET_SEND_SIZE         1314           //历史数据每次发送的数据包包含的18条数据
#define HISTORY_SIZE_EACH_ITEM           66
#define OTA_GPS_PACKET_DATA_LEN          73

#define HISTORY_READ_PACKET_SIZE_AES     1152 //72*16
#define HISTORY_NUMBER_EACH_PACKET_AES   16
#define HISTORY_PACKET_SEND_SIZE_AES     1264 // 79*16
#define OTA_GPS_PACKET_DATA_LEN_AES      79  //((63-15)/16 + 1)*16 + 15

#define HISTORY_READ_PACKET_SIZE_MD5     1184 //74*16
#define HISTORY_NUMBER_EACH_PACKET_MD5   16
#define HISTORY_PACKET_SEND_SIZE_MD5     1296 // 16*(81)
#define OTA_GPS_PACKET_DATA_LEN_MD5      81


#define HISTORY_ALARM_NUMBER_EACH_FILE   50
#define HISTORY_MAX_ALARM_ITEM_INDEX     5000          //历史文件最大个数
#define HISTORY_MAX_ALARM_FILE_SIZE      3300         //alarm文件50条数据
#define HISTORY_ALARM_SIZE_EACH_ITEM     66
#elif defined(__WIRED_DEVICE__)
#define OTA_HISTORY_FLAG_INDEX           24

#define HISTORY_MAX_ITEM_INDEX           49000           //历史文件的最大个数
#define HISTORY_NUMBER_EACH_PACKET       18
#define HISTORY_NUMBER_EACH_FILE         90
#define HISTORY_READ_PACKET_SIZE         1188
#define HISTORY_PACKET_SEND_SIZE         1314           //历史数据每次发送的数据包包含的18条数据

#define HISTORY_READ_PACKET_SIZE_AES     1152 //72*16
#define HISTORY_NUMBER_EACH_PACKET_AES   16
/*#define HISTORY_PACKET_SEND_SIZE_AES     1264 // 79*16
#define OTA_GPS_PACKET_DATA_LEN_AES      79*/  //((63-15)/16 + 1)*16 + 15

#define HISTORY_READ_PACKET_SIZE_MD5     1184 //74*16
#define HISTORY_NUMBER_EACH_PACKET_MD5   16
/*#define HISTORY_PACKET_SEND_SIZE_MD5     1296 // 16*(81)
#define OTA_GPS_PACKET_DATA_LEN_MD5      81*/


#define HISTORY_ALARM_NUMBER_EACH_FILE   50
#define HISTORY_MAX_ALARM_ITEM_INDEX     5000          //历史文件最大个数

#else

#define OTA_HISTORY_FLAG_INDEX           24

#define HISTORY_MAX_ITEM_INDEX           49000           //历史文件的最大个数
#define HISTORY_NUMBER_EACH_PACKET       19
#define HISTORY_NUMBER_EACH_FILE         95
#define HISTORY_MAX_FILE_SIZE            5795           //历史文件95条数据，
#define HISTORY_READ_PACKET_SIZE         1159
#define HISTORY_PACKET_SEND_SIZE         1292           //历史数据每次发送的数据包包含的19条数据
#define HISTORY_SIZE_EACH_ITEM           61
#define OTA_GPS_PACKET_DATA_LEN          68

#define HISTORY_READ_PACKET_SIZE_AES     1104 //69*16
#define HISTORY_NUMBER_EACH_PACKET_AES   16
#define HISTORY_PACKET_SEND_SIZE_AES     1264 // 79*16
#define OTA_GPS_PACKET_DATA_LEN_AES      79  //((68-15)/16 + 1)*16 + 15

#define HISTORY_READ_PACKET_SIZE_MD5     1173 //69*17
#define HISTORY_NUMBER_EACH_PACKET_MD5   17
#define HISTORY_PACKET_SEND_SIZE_MD5     1292 // 17*(68 + 8)
#define OTA_GPS_PACKET_DATA_LEN_MD5      76


#define HISTORY_ALARM_NUMBER_EACH_FILE   50
#define HISTORY_MAX_ALARM_ITEM_INDEX     5000          //历史文件最大个数
#define HISTORY_MAX_ALARM_FILE_SIZE      3050         //alarm文件50条数据
#define HISTORY_ALARM_SIZE_EACH_ITEM     61


#endif



#define OTA_PACKET_LEN_INDEX              4

#define OTA_DRIVE_SPEED_CNT_MAX           5


#define OTA_ACCELERATION_Z_ORG                  980
#define OTA_ACCE_LIST_BUFF_SIZE                 100

#define OTA_ACCIDENT_LIST_BUFF_SIZE             15

#ifdef __OTA_OBD_SUPPORT__
#define HISTORY_MAX_ACCIDENT_ITEM_INDEX         400          //陀螺仪历史文件最大个数
#define HISTORY_MAX_ACCIDENT_FILE_SIZE          13770         // 459byte * 30
#define HISTORY_ACCIDENT_NUMBER_EACH_FILE       30
#define HISTORY_ACCIDENT_NUMBER_EACH_PACKET     1
#define HISTORY_ACCIDENT_SIZE_EACH_ITEM         459
#define OTA_ACCIDENT_PACKET_DATA_MAX_LEN        466
#define OTA_ACCIDENT_PACKET_DATA_MAX_LEN_MD5    474  /// 466+8
#define OTA_ACCIDENT_PACKET_DATA_MAX_LEN_AES    479 // ((466-15)/16 + 1)*16 + 15
#else
#define HISTORY_MAX_ACCIDENT_ITEM_INDEX         500          //陀螺仪历史文件最大个数
#define HISTORY_ACCIDENT_NUMBER_EACH_FILE       30
#define HISTORY_ACCIDENT_NUMBER_EACH_PACKET     1
#define HISTORY_MAX_ACCIDENT_FILE_SIZE          12870         // 429byte * 30
#define HISTORY_ACCIDENT_SIZE_EACH_ITEM         429
#define OTA_ACCIDENT_PACKET_DATA_MAX_LEN        436

#define OTA_ACCIDENT_PACKET_DATA_MAX_LEN_MD5    444  /// 436+8
#define OTA_ACCIDENT_PACKET_DATA_MAX_LEN_AES    447 // ((436-15)/16 + 1)*16 + 15
#endif

#define HISTORY_MAX_BEHAV_GPS_ITEM_INDEX     2000          //陀螺仪历史文件最大个数
#ifdef __OTA_OBD_SUPPORT__
#define HISTORY_MAX_BEHAV_GPS_FILE_SIZE      11400         // 57byte * 200
#define HISTORY_BEHAV_GPS_NUMBER_EACH_FILE   200
#define HISTORY_BEHAV_GPS_SIZE_EACH_PACKET   1140
#define HISTORY_BEHAV_GPS_SIZE_SEND_PACKET   1280
#define HISTORY_BEHAV_GPS_NUMBER_EACH_PACKET 20
#define HISTORY_BEHAV_GPS_SIZE_EACH_ITEM     57
#define OTA_BEHAV_GPS_PACKET_DATA_LEN        64
#define HISTORY_BEHAV_GPS_SIZE_EACH_PACKET_AES   1040 // 65*16
#define HISTORY_BEHAV_GPS_NUMBER_EACH_PACKET_AES 16
#define HISTORY_BEHAV_GPS_SIZE_SEND_PACKET_AES   1264 //79*16
#define OTA_BEHAV_GPS_PACKET_DATA_LEN_AES        79   //((64-15)/16 + 1)*16 + 15

#define HISTORY_BEHAV_GPS_SIZE_EACH_PACKET_MD5   1170 // 65*18
#define HISTORY_BEHAV_GPS_NUMBER_EACH_PACKET_MD5 18
#define HISTORY_BEHAV_GPS_SIZE_SEND_PACKET_MD5   1296 // 72*18
#define OTA_BEHAV_GPS_PACKET_DATA_LEN_MD5        72 // 64 + 8

#else
#define HISTORY_MAX_BEHAV_GPS_FILE_SIZE      10600         // 53byte * 200
#define HISTORY_BEHAV_GPS_NUMBER_EACH_FILE   200
#define HISTORY_BEHAV_GPS_SIZE_EACH_PACKET   1060
#define HISTORY_BEHAV_GPS_SIZE_SEND_PACKET   1200
#define HISTORY_BEHAV_GPS_NUMBER_EACH_PACKET 20
#define HISTORY_BEHAV_GPS_SIZE_EACH_ITEM     53
#define OTA_BEHAV_GPS_PACKET_DATA_LEN        60


#define HISTORY_BEHAV_GPS_SIZE_EACH_PACKET_AES   1220 // 61*20
#define HISTORY_BEHAV_GPS_NUMBER_EACH_PACKET_AES 20
#define HISTORY_BEHAV_GPS_SIZE_SEND_PACKET_AES   1260 //63*20
#define OTA_BEHAV_GPS_PACKET_DATA_LEN_AES        63   //((60-15)/16 + 1)*16 + 15

#define HISTORY_BEHAV_GPS_SIZE_EACH_PACKET_MD5   1159 // 61*19
#define HISTORY_BEHAV_GPS_NUMBER_EACH_PACKET_MD5 19
#define HISTORY_BEHAV_GPS_SIZE_SEND_PACKET_MD5   1292 // 68*19
#define OTA_BEHAV_GPS_PACKET_DATA_LEN_MD5        68 // 60 + 8

#endif
#define HISTORY_MAX_BEHAV_ACCE_ITEM_INDEX     2000          //陀螺仪历史文件最大个数
#ifdef __OTA_OBD_SUPPORT__
#define HISTORY_MAX_BEHAV_ACCE_FILE_SIZE      7800         // 39byte * 200
#define HISTORY_BEHAV_ACCE_NUMBER_EACH_FILE   200
#define HISTORY_BEHAV_ACCE_SIZE_EACH_PACKET   780
#define HISTORY_BEHAV_ACCE_SIZE_SEND_PACKET   920
#define HISTORY_BEHAV_ACCE_NUMBER_EACH_PACKET 20
#define HISTORY_BEHAV_ACCE_SIZE_EACH_ITEM     39
#define OTA_BEHAV_ACCE_PACKET_DATA_LEN        46

#define HISTORY_BEHAV_ACCE_SIZE_EACH_PACKET_AES   940 //20*47
#define HISTORY_BEHAV_ACCE_NUMBER_EACH_PACKET_AES 20
#define HISTORY_BEHAV_ACCE_SIZE_SEND_PACKET_AES   940 //20*47
#define OTA_BEHAV_ACCE_PACKET_DATA_LEN_AES        47  // ((46-15)/16 + 1)*16 + 15

#define HISTORY_BEHAV_ACCE_SIZE_EACH_PACKET_MD5   940
#define HISTORY_BEHAV_ACCE_NUMBER_EACH_PACKET_MD5 20
#define HISTORY_BEHAV_ACCE_SIZE_SEND_PACKET_MD5   1080 //20*(54)
#define OTA_BEHAV_ACCE_PACKET_DATA_LEN_MD5        54  //  46 + 8
#else
#define HISTORY_MAX_BEHAV_ACCE_FILE_SIZE      7400         // 37byte * 200
#define HISTORY_BEHAV_ACCE_NUMBER_EACH_FILE   200
#define HISTORY_BEHAV_ACCE_SIZE_EACH_PACKET   740
#define HISTORY_BEHAV_ACCE_SIZE_SEND_PACKET   880
#define HISTORY_BEHAV_ACCE_NUMBER_EACH_PACKET 20
#define HISTORY_BEHAV_ACCE_SIZE_EACH_ITEM     37
#define OTA_BEHAV_ACCE_PACKET_DATA_LEN        44

#define HISTORY_BEHAV_ACCE_SIZE_EACH_PACKET_AES   900 //20*45
#define HISTORY_BEHAV_ACCE_NUMBER_EACH_PACKET_AES 20
#define HISTORY_BEHAV_ACCE_SIZE_SEND_PACKET_AES   940 //20*47
#define OTA_BEHAV_ACCE_PACKET_DATA_LEN_AES        47  // ((44-15)/16 + 1)*16 + 15

#define HISTORY_BEHAV_ACCE_SIZE_EACH_PACKET_MD5   900
#define HISTORY_BEHAV_ACCE_NUMBER_EACH_PACKET_MD5 20
#define HISTORY_BEHAV_ACCE_SIZE_SEND_PACKET_MD5   1040 //20*(52)
#define OTA_BEHAV_ACCE_PACKET_DATA_LEN_MD5        52  //  44 + 8
#endif
#ifdef __WIFI_LOCATION_SUPPORT__
#define HISTORY_MAX_WIFI_ITEM_INDEX     4000
#define HISTORY_MAX_WIFI_FILE_SIZE      8200         // 41byte * 200
#define HISTORY_WIFI_NUMBER_EACH_FILE   200
#define HISTORY_WIFI_SIZE_EACH_PACKET   820
#define HISTORY_WIFI_SIZE_SEND_PACKET   960
#define HISTORY_WIFI_NUMBER_EACH_PACKET 20
#define HISTORY_WIFI_SIZE_EACH_ITEM     41
#define OTA_WIFI_PACKET_DATA_LEN        48
#endif

#ifdef __SMART_LOCK_SUPPORT__
#define HISTORY_MAX_LOCK_ITEM_INDEX     4000        // 存储的历史数据总条数
#define HISTORY_MAX_LOCK_FILE_SIZE      8800        // 每个文件存储的数据总大小   总条数*历史数据size  200*43
#define HISTORY_LOCK_NUMBER_EACH_FILE   200         // 每个文件可以存储的历史数据总条数
#define HISTORY_LOCK_SIZE_EACH_PACKET   880         // 每个包对应的历史数据存储大小：条数*历史数据size  20*43
#define HISTORY_LOCK_SIZE_SEND_PACKET   1020        // 发送历史数据时，每个包的大小:  条数*上传数据size  20*50
#define HISTORY_LOCK_NUMBER_EACH_PACKET 20          // 发送历史数据时，每个包包含的数据条数
#define HISTORY_LOCK_SIZE_EACH_ITEM     44          // 保存历史数据大小，比原始数据少7个字节。保存历史数据size
#define OTA_LOCK_PACKET_DATA_LEN        51          // 上传的单条数据大小。上传数据size
#endif

#define OTA_EXTERNAL_POWER_MEASURE_TIMES   7

#define AGPS_SERVER_IN_CHAINA   0
#define AGPS_SERVER_OVERSEA     1

#define OTA_AGPS_ONLINE_VALID_2_HOURS           2
#define OTA_AGPS_ONLINE_VALID_3_DAYS_WHEN_MIX   3
#define OTA_AGPS_OFFLINE_VALID_14_DAYS          14

#define OTA_SOCKET_IMPORTANT_SHAKE_HANDS_MODE   0   //针对重要数据进行握手
#define OTA_SOCKET_DO_NOT_SHAKE_HANDS_MODE      1   //只发不握手
#define OTA_SOCKET_COMPLETELY_SHAKE_HANDS_MODE  2   //全握手

#define OTA_MULTIPLE_SERVERS_ALONE_MODE         0   //IP1和IP2单独使用模式
#define OTA_MULTIPLE_SERVERS_SIMULTANEOUS_MODE  1   //IP1和IP2同时使用模式

#define OTA_SOCKET_CONNECTION_LONG_LINK_MODE      0   //长连接模式
#define OTA_SOCKET_CONNECTION_ALTERNATE_LINK_MODE 1   //长短连接交替结合模式
#define OTA_SOCKET_CONNECTION_SHORT_LINK_MODE     2   //短连接模式

#define OTA_SEND_ALERT_SMS_ANY_REPLY_MODE              0   //代表不管警报信息服务器有没有回复，都会发内置相应配置的短信给管理员号码
//#define OTA_SEND_ALERT_SMS_DO_NOT_REPLY_DELAY_15S_MODE 1   //代表在15秒内没有回复的就发相应的报警短信，如果15秒内得到服务器的回复，就不发内置的短信
#define OTA_DO_NOT_SEND_ALERT_SMS_MODE                 1   //代表取消内置短信发送，不管有没有设置管理员号码

#define OTA_OPER_TYPE_LONGIN_STATUS_OK           0
#define OTA_OPER_TYPE_LONGIN_STATUS_LOGGING      1
#define OTA_OPER_TYPE_LONGIN_STATUS_WAITSET      2
#define OTA_OPER_TYPE_LONGIN_STATUS_SETTING      3
#define OTA_OPER_TYPE_LONGIN_STATUS_CLOSED       4
#define OTA_OPER_TYPE_HEART_COUNT_ZERO           5
#define OTA_OPER_TYPE_HEART_COUNT_ONE            6



/*计数常量定义*/
#define OTA_COUNT_MIN                       0
#define OTA_ACC_AC_STATE_VALID_CNT_MAX      10     //ACC 或者AC连续5s处于同一个状态，则认为该设备状态为当前状态
#define OTA_MOVE_COUNT_MAX                  10    // GSENSOR和震动的判断需要连续震动30次则认为终端是运动的
#define OTA_GPS_FILTER_STATIC_CNT_MAX       5    //当静态漂移过滤5次都没有运动，则认为是静止的
#define OTA_GPS_FILTER_MOVE_CNT_MAX         4    //accon时当静态漂移过滤1次都运动，则认为是运动的
#define OTA_GPS_ACCOFF_FILTER_MOVE_CNT_MAX  2    //accoff时当静态漂移过滤1次都运动，则认为是运动的
#define OTA_SOCKET_CONNECT_TIME_MAX         240
#define OTA_REBOOT_CNT_MAX                  600     //login10分钟没有收到response、网络连接10分钟连不上，gps10分钟未成功定位则重启
#define OTA_REBOOT_CNT_WHEN_RECIVE_SMS      510   //收到新短信，延迟90s重启
#define OTA_GPS_NOT_OPEN_TIMER_OUT          60
#define OTA_GEOFENCE_CHECK_INTERVAL         3
#define OTA_MANAGER_NUMBER_FAILED_MAX_CNT   10
#define OTA_ALARM_CHECK_RESPONSE_TIME_MAX   5
#define OTA_SAVE_DIS_TO_NV_CNT              6
#define OTA_FEED_WATCH_DOG_INTERVAL         1
#define OTA_LOGIN_FIRST_INTERVAL            26
#define OTA_LOGIN_SECOND_INTERVAL           51


/*字符串长度常量宏定义*/
#define OTA_IMEI_BYTE_SIZE         8              //可以用8个字节存储
#define OTA_IMEI_STR_LEN           16             //imei的字符串长度为16
#define OTA_IMSI_STR_LEN           16
#define OTA_SERVER_ENCODE_DATA_LEN 8
#define OTA_PLMN_STR_LEN           7
#define OTA_COUNTRY_CODE_STR_LEN   3

#define OTA_PASSWORD_MAXLEN            10             // 设备password，在命令发送中用到
#define OTA_PASSWORD_MINLEN            4
#define OTA_IP_MAXLEN                  16             // IP长度
#define OTA_DOMAIN_MAXLEN              100            //域名长度
#define OTA_PORT_MAXLEN                10             //端口长度

#define OTA_APN_MAXLEN                 50             // apn 最大长度
#define OTA_NUMBER_MAXLEN              30             // 号码最大长度
#define OTA_GEOFENCE_FILE_SIZE         1000           // 电子围栏最大长度
#define OTA_UPGRADE_FILENAME_MAXLEN    200            //OTA下载包名字最大长度
#define OTA_UPGRADE_FILEAPPENDIX_LEN   10
#define OTA_ALARM_DATA_NUMBER          22             //存储的警报个数
#define OTA_CMD_MAX_LEN                30             // 下发指令最大长度
#define OTA_AT_SEND_BUF_MAXLEN         151
#define OTA_USSD_CMD_MAXLEN            30

#define OTA_SMS_ONE_PART_MAX_LEN       153
#define OTA_SMS_NORMAL_MAX_LEN         160
#define OTA_SMS_PART_MAX_CNT           4
#define OTA_SMS_BUF_MAX_LEN            153 * 4

#define OTA_SMS_UNICODE_BUF_MAX_LEN    536 // 67 * 2* 4

#ifdef __SMART_LOCK_SUPPORT__
#define OTA_MODEL_NAME_MAXLEN          20
#else
#define OTA_MODEL_NAME_MAXLEN          10
#endif

#define OTA_FILE_PATH_MAXLEN       50

#define OTA_POSTION_LEN                12
#if defined(__PORTABLE_DEVICE__) || defined(__WIRED_DEVICE__)
#define OTA_GPS_PACKET_POSTION_OFFSET  23
#elif __OTA_OBD_SUPPORT__
#define OTA_GPS_PACKET_POSTION_OFFSET  46
#else
#define OTA_GPS_PACKET_POSTION_OFFSET  50
#endif

#define OTA_RFID_SN_LEN                 8
#define OTA_RFID_NUM_MAX                1500

#define OTA_TPMS_ID_LEN                 3
#define OTA_TPMS_ID_LEN_MAX             6
#define OTA_TPMS_ID_NUM_MAX             0x16


#define GPRS_SEND_DATA_SIZE        1300           // 电子围栏回复过大
#define OTA_LOG_DATA_MAXLEN        150
#define OTA_GLOBAL_VAR_FILE_MAXLEN 400

#define OTA_GEOFENCE_ITEM_HEAD_LEN   3
#define OTA_ALARM_SMS_ITEM_HEAD_LEN  2

#ifdef __SOCKET_OSM_GOOGLE_ADDR_REQUEST__
#define OTA_OSM_GOOGLE_URL_PACKET_DATA_LEN       230
#define OTA_DOMAIN_ADDR_REQUEST_MAXLEN           31            //域名长度
#endif

#define OTA_DNS_SERVER_NUM              2

#define OTA_CMD_SEND_REPLY_PACKET_DATA_LEN  512

#define OTA_GPS_WAIT_FIX_MAX_CNT            90
#ifdef __OTA_OBD_SUPPORT__
#define OTA_GPS_DATA_TIME_INDEX         40
#define OTA_GPS_DATA_TIME_HIS_INDEX     33
#elif defined(__PORTABLE_DEVICE__)
#define OTA_GPS_DATA_TIME_INDEX       17
#define OTA_GPS_DATA_TIME_HIS_INDEX   10
#elif defined(__WIRED_DEVICE__)
#define OTA_GPS_DATA_TIME_INDEX         52
#define OTA_GPS_DATA_TIME_HIS_INDEX     45
#else
#define OTA_GPS_DATA_TIME_INDEX         44
#define OTA_GPS_DATA_TIME_HIS_INDEX     37
#endif
#define OTA_BEHAV_DATA_TIME_INDEX       16
#define OTA_BEHAV_DATA_TIME_HIS_INDEX   9
#define OTA_WIFI_TIME_INDEX             15
#define OTA_WIFI_TIME_HIS_INDEX         8
#ifdef __SMART_LOCK_SUPPORT__
#define OTA_LOCK_TIME_INDEX             15   //协议数据中，时间(6个字节)的位置
#define OTA_LOCK_TIME_HIS_INDEX         8    //存放的历史数据中，时间(6个字节)的位置，因为去掉了IMEI,位置会前移

#define OTA_LOCK_NFCID_CNT_SHOW         10   //查询NFC卡id的数量，由于mcu和bg95之间的传输数据限制，这里限制每次只能查到10个NFCid
#define OTA_LOCK_NFCID_CNT_MAX          50   //NFC卡id的最大存放数量，这个值要与MCU端一致
#define OTA_LOCK_NFCID_MAX              20   //NFC卡id的最大长度在BG95端的最大长度，这个值要与MCU端一致,id存储的最大长度是10个字节，但是显示采用asc码，需要20个字节
#define OTA_LOCK_NFCID_MCU_MAX          10   //NFC卡id的在MCU端最大长度是10个字节，但是显示采用asc码，需要20个字节
#endif
#define OTA_VAR_LEN_DATA_TIME_INDEX     15

#ifdef __BATCH_WRITE_NV_PARAMETER_FUNC__
#define OTA_CONFIGNV_ERROR_DATA_MAXLEN        125

#define OTA_CONFIGNV_TIMER           1
#define OTA_CONFIGNV_RELINK          2
#define OTA_CONFIGNV_HBT             3
#define OTA_CONFIGNV_GMT             4
#define OTA_CONFIGNV_TRACK           5
#define OTA_CONFIGNV_APN             6
#define OTA_CONFIGNV_MILEAGEC        7
#define OTA_CONFIGNV_LOCKTRACKER     8
#define OTA_CONFIGNV_LOCKSIM         9
#define OTA_CONFIGNV_LOCKAPN         10
#define OTA_CONFIGNV_ANALOG          11
#define OTA_CONFIGNV_GSENSOR         12
#define OTA_CONFIGNV_VIBRATION       13
#define OTA_CONFIGNV_ANTITHEFT       14
#define OTA_CONFIGNV_ACCDET          15
#define OTA_CONFIGNV_RELAY           16
#define OTA_CONFIGNV_RLYMODE         17
#define OTA_CONFIGNV_SPEED           18
#define OTA_CONFIGNV_IDLE            19
#define OTA_CONFIGNV_DMODE           20
#define OTA_CONFIGNV_STL             21
#define OTA_CONFIGNV_DIGITAL2        22
#define OTA_CONFIGNV_RANGE           23
#define OTA_CONFIGNV_LGA             24
#define OTA_CONFIGNV_UNIT            25
#define OTA_CONFIGNV_DRAG            26
#define OTA_CONFIGNV_UPLOAD          27
#define OTA_CONFIGNV_ALERT           28
#define OTA_CONFIGNV_2IP             29
#define OTA_CONFIGNV_AGPSM           30
#define OTA_CONFIGNV_AEPOWER         31
#define OTA_CONFIGNV_LOG             32
#define OTA_CONFIGNV_LINK            33
#define OTA_CONFIGNV_RESPONSE        34
#define OTA_CONFIGNV_ROAM            35
#define OTA_CONFIGNV_SPEAKER         36
#define OTA_CONFIGNV_ECP             37
#define OTA_CONFIGNV_SETKEY          38
#define OTA_CONFIGNV_CATCHGPS        39
#define OTA_CONFIGNV_MANAGERA        40
#define OTA_CONFIGNV_252501          41
#define OTA_CONFIGNV_252502          42
#define OTA_CONFIGNV_252503          43
#define OTA_CONFIGNV_252504          44
#define OTA_CONFIGNV_252505          45
#define OTA_CONFIGNV_252506          46
#define OTA_CONFIGNV_252507          47
#define OTA_CONFIGNV_252508          48
#define OTA_CONFIGNV_252509          49
#define OTA_CONFIGNV_BSDB            50
#define OTA_CONFIGNV_BADB            51
#define OTA_CONFIGNV_BAUD            52
#define OTA_CONFIGNV_BTMAC           53
#define OTA_CONFIGNV_IMEI            54
#define OTA_CONFIGNV_IP              55
#define OTA_CONFIGNV_IP2             56
#define OTA_CONFIGNV_JAMMERD         57
#define OTA_CONFIGNV_SETACK          58
#define OTA_CONFIGNV_GOOGLEM         59
#define OTA_CONFIGNV_BT_NAME         60
#endif

#ifdef __DRIVING_BEHAVIOR_MNGR__
#define OTA_DRIVE_BRAKE_WITH_HIGH      0
#define OTA_DRIVE_ACCEL_WITH_HIGH      1
#define OTA_DRIVE_BRAKE_WITH_MEDIUM    2
#define OTA_DRIVE_ACCEL_WITH_MEDIUM    3
#define OTA_DRIVE_BRAKE_WITH_SLOW      4
#define OTA_DRIVE_ACCEL_WITH_SLOW      5
#endif

#define OTA_MODEM_OPERATOR_LEN_MAX    40
#define OTA_MODEM_BAND_LEN_MAX        15
#define OTA_MODME_ACCESS_TECH_LEN_MAX 10

/*定时器延时时间常量宏定义*/
#define OTA_TIMER_OTA_FILE_DELAY              5*1000
#define OTA_TIMER_DELAY_MAIN                  1*1000
#define OTA_TIMER_SEND_GPS_FIRST_TIME         7   //开机或者login 7s后第一次发送gps数据
#define OTA_TIMER_DELAY_DISCONNECT            (3*1000)
#define OTA_TIMER_ONE_SEC_DELAY               (1000)
#define OTA_TIMER_THREE_SEC_DELAY             (3*1000)
#define OTA_TIMER_FIVE_SEC_DELAY              (5*1000)
#define OTA_TIMER_DELAY_ADDR_DELAY            (15*1000)
#define OTA_TIMER_DELAY_AGPS_OFFLINE          (10*1000)

#define OTA_TIMER_ALARM_TIMEOUT_RESEND        9*1000
#define OTA_TIMER_DELAY_CALL_SOS              10*1000
#define OTA_TIMER_DELAY_CHECK_SOCKET_RESPONSE 15*1000   //延时15s检查服务器有没有针对alarm上报响应
#define OTA_TIMER_HEARTBEAT_TIMEOUT           30*1000
#define OTA_TIMER_DELAY_SEND_SMS              30*1000
#define OTA_TIMER_DELAY_REBOOT                15000
#define OTA_TIMER_SOCKET_DOWNLOAD_TIMEOUT     120*1000
#define OTA_TIMER_DELAY_STOP_SMS_FORWARD      180*1000
#define OTA_TIMER_DELAY_MOVE_UPGRADE_OTA      3000

#define OTA_TIMER_DELAY_TURN_OFF_LED          300*1000
#define OTA_TIMER_DELAY_DOWNLOAD_TIMEOUT      1800*1000
#define OTA_TIMER_DELAY_UPDATE_MCU_TIMEOUT    600*1000
#define OTA_TIMER_DELAY_SHORT_LINK_DISCON1    600*1000
#define OTA_TIMER_DELAY_SHORT_LINK_DISCON2    180*1000
#define OTA_TIMER_SHORT_LINK_TIMEOUT          60*1000
#define OTA_TIMER_BT_ACK_TIMEOUT              60*1000
#define OTA_TIMER_DELAY_GOOGLE_SWITCH_OSM_SOC 10*1000

#define OTA_TIMER_BT_DELAY_NEW_SEARCH_MAC     3*1000
#define OTA_TIMER_INIT_OBD_TIMEOUT            6*1000
#define OTA_TIMER_CHECK_OBD_TIMEOUT           1*1000
#define OTA_TIMER_DELAY_AUTH_TIME             900*1000
#define OTA_TIMER_DELAY_FEED_WDG_FINISH       10
#define OTA_TIMER_ACCOFF_POWER_SAVE_MAX_CNT   75
#define OTA_TIMER_SOC_CONN_TIMEOUT_CNT        160*1000
#define OTA_TIMER_DELAY_CLOSE_TOPFLY2         3*1000
#define OTA_TIMER_SOCKET_ACK_TIMEOUT          1000
#define OTA_TIMER_RS232_WAIT_NOTIFY_TIMEOUT   5*1000
#define OTA_TIMER_RS232_WAIT_LOOP_TIMEOUT     3*1000
#define OTA_TIMER_BTSENSOR_DELAY              60*1000
#define OTA_TIMER_BLE_SCAN_DELAY              2000
#define OTA_TIMER_OTA_OK_TIMEROUT             300*1000
#define OTA_TIMER_UPG_DIRECTLY_DELAY          3*1000
#define OTA_TIMER_OBD_DATA_SEND_TIME          60*1000

#define OTA_RS232_RX_TIMER_PERIOD             (3*1000)
#define OTA_RS232_FP_TIMER_FEATURE            (1*1000)
#define OTA_RS232_FP_TIMER_DOWNLOAD           (6*1000)
#define OTA_RS232_FP_TIMER_PROGRAM            (200)
#define OTA_COMMON_DATA_SEND_DELAY            (5*1000)
#define OTA_COMMON_DATA_SEND_PERIOD           (1*1000)
#define OTA_TIMER_GPS_ACCON_TIMEOUT           180*1000
#define OTA_TIMER_SECURITY_AUTH_DELAY         30*1000

#ifdef __OTA_OBD_SUPPORT__
#define OTA_OBD_UPG_RETRY_MAX                  5
#define OTA_TIMER_OBD_UPG_MONITOR_DELAY        2000
#define OTA_TIMER_OBD_UPG_FAIL_RETRY_DELAY     300*1000
#define OTA_TIMER_OBD_UPG_RETRY_DELAY          500
#define OTA_TIMER_OBD_UPG_DATA_DELAY           5000
#define OTA_TIMER_OBD_UPG_RECOVERY_SOCKET      10*1000
#define OTA_OBD_CHECK_OBD_OK_TIMES             30
#define OTA_OBD_RESET_OBD_TIMES                3
#define OTA_OBD_RX_TIMEOUT_CNT                 5
#define OTA_OBD_SET_MIL_FUEL_TIME              3
#endif
#define OTA_TIMER_BT_INQ_TIMEOUT              90*1000
#define OTA_HISTORY_DATA_SEND_INTERVAL        3
#define OTA_TIMER_REQ_CSQ_LOOP                5*1000
#define OTA_TIMER_REQ_REG_LOOP                30*1000
#define OTA_TIMER_SET_NW_PARAM_DELAY          8*1000
#define OTA_TIME_LOW_POWER_ENTER_DELAY        5*1000
#define OTA_TIME_POWEROFF_TIMEOUT             180*1000
#define OTA_TIMER_PARSE_BATCHSET_DELAY        60*1000
#define OTA_TIMER_OPEN_ALL_FUNC_DELAY         15*1000
#define OTA_TIMER_FILE_DOWNLOAD_TIMEOUT       300*1000
#define OTA_ACCIDENT_CNT_MAX                10
#define OTA_ACCIDENT_DUR                    5

#define OTA_GT_BT_SEARCH_TYPE_CERTIFICATION   1
#define OTA_GT_BT_SEARCH_TYPE_HEADSET         2
#define OTA_GT_BT_SEARCH_TYPE_BLE_DEV         4

#define OTA_HISTORY_BEHAV_ACCE_FLAG_INDEX     22
#define OTA_HISTORY_ACCIDENT_FLAG_INDEX       22

#define OTA_WHITE_LIST_NUMBER_CNT             10
#define OTA_OPERATOR_LIST_NUMBER_CNT          50

#define OTA_ROAMING_WHITE_LIST_CNT_1          20
#define OTA_ROAMING_WHITE_LIST_CNT_2          30
#define OTA_ROAMING_WHITE_LIST_CNT            50
#define OTA_NO_ACK_TIMEOUT_CNT                30
#define OTA_NO_SLEEP_SEND_DATA_TIMER          260
#define OTA_CHARGING_WAKE_UP_TIMER            300
#define OTA_MCU_QUERY_INTERVAL                8000
#define OTA_NO_NETWORK_TIME_MAX               3

#define OTA_UPLOAD_INTELLIGENCE_ACCON         86400 //15MIN
#define OTA_UPLOAD_INTELLIGENCE_ACCOFF        86400 //1hour

#define OTA_INTELLIGENCE_POWER_ENTER          3650  //3.55V
#define OTA_INTELLIGENCE_POWER_OUT            3750  //3.65V

#ifdef __SMART_LOCK_SUPPORT__
#define OTA_INTELLIGENCE_POWER_ENTER_NRF      3400  //3.4V
#define OTA_INTELLIGENCE_POWER_OUT_NRF        3500  //3.5V
#else
#define OTA_INTELLIGENCE_POWER_ENTER_NRF      3500  //3.4V    ->3.5V
#define OTA_INTELLIGENCE_POWER_OUT_NRF        3600  //3.5V    ->3.6
#endif

#define OTA_MCU_HBT_INTERVAL                  15000
#define OTA_GSENSOR_IGNORE_CNT                5

#define OTA_SIMLOCK_NUMERIC_LEN               6
#define OTA_SIMLOCK_NUMERIC_MAX               8
#define OTA_SIMLOCK_DECODE_LEN                8
#define OTA_SIMUNLOCK_FAILED_MAX              5
#define OTA_RTINF_SAVE_DELAYED                15

#define OTA_UPGRADE_FAILED_TIME_MAX           3
#define OTA_UPGRADE_FREE_CLU_NEED             760

#define OTA_AT_ERROR_TYPE_MAX                 20
/*度量单位转换宏定义*/
#ifndef OTA_M_PI
#define OTA_M_PI 3.1415926
#endif
#ifndef OTA_TWO_PI
#define OTA_TWO_PI 3.1415926 * 2
#endif

#ifndef OTA_RAD_TO_DEG
#define OTA_RAD_TO_DEG 57.2957795  // 弧度转角度的转换率= 180/PI
#endif

#ifndef OTA_DEG_TO_RAD
#define OTA_DEG_TO_RAD 0.0174533 // 角度转弧度的转换率= PI/180
#endif

#ifndef OTA_KM_TO_MILE
#define OTA_KM_TO_MILE  0.6214    //千米转为海里= 1/1.609
#endif
#define OTA_MILE_TO_KM  1.6093

#define OTA_FILE_MAX_SIZE 700*1024

#define OTA_TIME_HOUR_TO_DAY    1/24
#define OTA_TIME_DAY_TO_HOUR    24
#define OTA_TIME_MIN_TO_HOUR    1/60
#define OTA_TIME_SECOND_TO_MIN  1/60
#define OTA_TIME_MINUTE_TO_MILLSECOND   (60*1000)
#define OTA_TIMER_SECOND_TO_MILLSECOND  (1*1000)
#define OTA_TIMER_MILLSECOND_TO_SECOND  1/1000
#define OTA_TIME_MINUTE_TO_SECOND       60

#define OTA_ANGLE_VALUE  G_VAL->gt_angle_value

#define CUSTOM_SMS_UNICODE L"SMS"

#define OTA_AT_CMD_HEAD                    "AT$OTAATCMD=\""
#define OTA_BT_CALIBRATION_REQ_STRING      "topfly device begin installation"
#define OTA_BT_CALIBRATION_ACK_STRING      "topfly device begin installation ack"
#define OTA_BT_CALIBRATION_FINISH_STRING   "topfly device complete installation"
#define OTA_BT_CMD_STRING_LEN      120

#define OTA_BLE_CONN_MAX                            4
#define OTA_BLE_SENSOR_CONFIG_MAX                   200
#define OTA_BLE_SENSOR_SCANNED_MAX                  (OTA_BLE_SENSOR_CONFIG_MAX*2)
#define OTA_BLE_SENSOR_TICK_MAX                     150
#define OTA_BLE_SENSOR_TICK_MIN                     100 /*约束条件: OTA_BLE_SENSOR_TICK_MAX < OTA_BLE_SENSOR_TICK_MIN*2*/
#define OTA_BLE_SENSOR_TICK_LOST_CNT                6   /*6分钟内没被再次扫到，视为丢失。具体计时时间由OTA_BLE_SENSOR_TICK_LOST_CNT和OTA_TIMER_BTSENSOR_DELAY共同决定*/
#define OTA_BLE_SENSOR_TICK_INITIAL                 0
#define OTA_NETWORK_REBOOT_CNT_MAX                  2
#define OTA_GT_GET_CELL_INTERVAL_MIN                80
#define OTA_GT_EXPORT_REPLY_HEAD_LEN                52

#define OTA_BLE_SENSOR_NAME_LEN                     6
#define OTA_BLE_LIST_MAX                            10

#define OTA_GPG_FILTER_DIS                          200

#define OTA_GPS_SV_NUM_MAX                          40
#define OTA_SOC_NO_ACK_TIME_MAX                     3
#define OTA_GT_ACK_HANDLE_INTERVAL                  5

#define OTA_UPGRADE_POWER_LIMIT_ST                     3400
#define OTA_UPGRADE_POWER_LIMIT_NRF                    3300

#define OTA_CHECK_SIM_CNT_MAX          3
#define OTA_ADC_FIFO_SIZE_MIN          5

#define OTA_ACCIDENT_TYPE_ROLLOVER     0
#define OTA_ACCIDENT_TYPE_COLLISION    1

#define OTA_RS232_S_VERSION  "VERSION:V"
#define OTA_RS232_S_BEGIN    "DEVICE_BEGIN"
#define OTA_RS232_S_BAUD     "BAUD:"
#define OTA_RS232_S_NOTIFY   "NOTIFY:"
#define OTA_RS232_S_CMD      "CMD:"
#define OTA_RS232_S_ACK      "ACK:"
#define OTA_RS232_S_ACK_FAIL "ACK_FAIL:"
#define OTA_RS232_S_OPERATOR "OPERATOR:"
#define OTA_RS232_S_END      "DEVICE_END"

#define OTA_FP_UPG_FILENAME  "fp.bin"
#define OTA_MCU_UPG_FILENAME  "mcu_upg"
#define OTA_OBD_UPG_FILENAME  "obd_upg"


#define UPG_APPENDIX_BIN     ".bin"
#define UPG_APPENDIX_7Z      ".7z"
#define UPG_APPENDIX_ZIP     ".zip"

//#define dbg_prompt(fmt_, ...) do{ota_notice(fmt_, ##__VA_ARGS__);/*qapi_Timer_Sleep(100, QAPI_TIMER_UNIT_MSEC, false );*/}while(0)

#endif/*__PUB_CONSTANT_OTA_H__*/

