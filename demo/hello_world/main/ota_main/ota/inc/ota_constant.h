/******************************************************************************

��Ȩ���� (C), 2014-2020, ���ɿƼ�(TopFlyTech)���޹�˾

 ******************************************************************************
  �� �� ��   : Ota_constant.h
  �� �� ��   : ����
  ��    ��   : Ben.You
  ��������   : 2016��7��8��
  ����޸�   :
  ��������   : �곣�����壬���ļ���OTA�������, OTA���Խ����޸�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��7��8��
    ��    ��   : Ben.You
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __PUB_CONSTANT_OTA_H__
#define __PUB_CONSTANT_OTA_H__

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#if defined(__GT_SUPPORT__)

/****************************************************************
���ܿ���:
__SERVER_LOCK__                            TODO
****************************************************************/
//#define __GPS_NET_TEST__                     // gps����ģʽ��ʹ��gprs����gps�ľ�����Ϣ
//#define __GT_NOSIM_NO12V_SHUTDOWN__          //��sim�������12v �Ƴ���3s�ػ���
//#define __GT_NOSIM_12V_SHUTDOWN__              //��sim������12V, 5���ӹػ�
#define __GT_GSENSOR_CONTROL_GPS__           //���GSENSOR ��ֹ���˶����Ӷ�����gps����
#define __GPS_AUTO_UPDATE_TIME__             //gps��λ�ɹ�ʱ���Զ�����ʱ��
#define __SMS_MODE_SUPPORT__                 //֧��smsģʽ����
#define __CHECK_HEART_REPLY__                //��������ظ���10����δ�յ������ظ�������������
#define __CHECK_ALARM_REPLY__                //��鱨���ظ���30����δ�յ������ظ����򷢶��š�(sos)��绰
#define __GEOFENCE_SUPPORT__                 //����Χ������
#define __WPS_DOWNLOAD__                     //ota����
//#define __SERVER_LOCK__                                  //�����������Ĺ���(�����ɹ����½����)
#define __FACTORY_TEST_MODE__                //����ģʽ����
#define __LED_DISPLAY__                      //��Ч����
#define __GPS_REBOOT__                       //gps��������10���Ӷ�����λ����λgpsоƬ
#define __SOCKET_REBOOT__                    //���繤��������10�������Ӳ������磬�򿪹ط���ģʽ
//#define __UBLOX_AGPS_SUPPORT__               //AGPS����
#ifndef __UBLOX_SUPPORT__
//#define __UBLOX_SUPPORT__                    //UBLOX����
#endif
//#define __MPU6500_GYROSCOPE_SUPPORT__        //GYRO����
#define __BMX055_GYROSCOPE_SUPPORT__         //GYRO BMX055����
//#define __BMA253_GSENSOR_SUPPORT__             // GSENSOR_BMA253����
//#define __MC3413_GSENSOR_SUPPORT__             // GSENSOR_MC3413����
//#define __UBLOX_AGPS_OFFLINE_SUPPORT__       // AGPS�����Զ�����
#define __HISTORY_DATA_SAVE__                //��ʷ���ݵı���ͷ��ͣ�������������
#define __SOCKET_ACK_MNGR__      // socket ���ֹ���
#define __MULTIPLE_SERVERS_SUPPORT__      // ��IP�ϴ����ݹ���
#define __OTA_IP3_SUPPORT__
//#define __CUSTOM_ALARM_MESSAGES__      // �Զ��屨������
#define __SOCKET_CONNECTION_MNGR__      // socket�����Ӷ����ӿ��ƹ���
#define __SMS_DATA_SAVE__               // ���淢�͵���Ϣ����ֹ��������ԭ������Ϣ��ʧ
//#define __WATCH_DOG_SUPPORT__
#define __SECURITY_SUPPORT__
//#define __SOCKET_DATA_ENCRYPTION_SUPPORT__
//#define __BLUETOOTH_CERTIFICATION__   //Bluetooth��֤����
//#define __OTA_BLUETOOTH_AT_SUPPORT__ //BT ʵ��AT����
#define __SOCKET_OSM_GOOGLE_ADDR_REQUEST__  //OSM ADDR REQUEST����
#define __FLIGHT_MODE_SWITCH_FUNC__
//#define __MTK_GPS_AGPS_SUPPORT__               //GPS AGPS����
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
#define __DRIVING_BEHAVIOR_MNGR__      // ��ʻ��Ϊ���ݹ���
#endif
//���GPSģ��
#define __L76_GPS__

//log s�����spi port��ֻ���ڵ��ԣ���������ر�
//#define __NEW_DBG_PORT__
//#define __OTA_UPGRADE_NO_CHECK__
//������ʷ�ļ��浽spi flash FATFS
#define __USE_SPI_FS__
#define __USE_CMD_TASK__
//#define __OTA_ACCE_CONVERT_PLANE__
//#define __OTA_DEBUG_ATC_CALLBACK__
#endif   // __GT_SUPPORT__

/****************************************************************
��־��ӡ����:
__TRACE_TO_NET__                      ����ͨ�������
****************************************************************/
//#define __TRACE_TO_NET__                //trace log ��ӡ�������� ,�Ա�Զ�̵���


/*--OTA �汾����--------------------------------------------------*/

#ifdef __SMART_LOCK_SUPPORT__
#define OTA_SW_VERSION          0x1000    //tc015x ��ota�ļ��İ汾��
#elif defined(__PORTABLE_DEVICE__)
#define OTA_SW_VERSION          0x1101    //ota�ļ��İ汾�ţ���ʾΪ1.1.2��ǰ���1Ϊ�̶���,ͬʱ�޸�ota_common_get_cmd_reply_string_version��ota_gt_pac_login_data
#elif defined(__OTA_OBD_SUPPORT__)
#define OTA_SW_VERSION          0x126
#elif defined(__WIRED_DEVICE__)
#define OTA_SW_VERSION          0x1113
#else
#define OTA_SW_VERSION          0x426
#endif


/*--������Ϣ��multi-handler�ı�־--
  --OTA��û��multi-handler����Ϣע�����, ����Ϊ���ܹ�ʹ��multi-handler,
  --����Ū�˸���־λ������, ����ͬһ����ϢID, �����Զ���32����־,
  --Ҳ����������ִ��32���ص�(����Ļص���ʵ������ͬһ��������,
  --ͬһ����Ϣ�µĲ�ͬ��if�����ִ�е�����, ����Ϊʲô��32, ����Ϊ
  --ϵͳ�����OTA_MSG_MULTI_FLAG����Ϊunsigned int��, Ҳ����32��bit.)
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

/*Э�鳣������*/
/*Э�鳣������*/
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
#define PROTOCOL_SOCKET_TYPE_0X14         0x14  //���ڿ����������¼��ϱ�
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
#define PROTOCOL_ALARM_TYPE_UNMOUNT                 0x01         // ���
#define PROTOCOL_ALARM_TYPE_OPENCOVER               0x02         // ���
#define PROTOCOL_ALARM_TYPE_SOS                     0x03         // SOS
#define PROTOCOL_ALARM_TYPE_OPENBOX                 0x04         // ����
#define PROTOCOL_ALARM_TYPE_FREEFALL                0x05         // ����
#define PROTOCOL_ALARM_TYPE_LOW_BATTERY             0x06
#define PROTOCOL_ALARM_TYPE_LOW_BATTERY_PRESET      0x07         // �͵�ָ�
#define PROTOCOL_ALARM_TYPE_HIGH_TEMP               0x08         //�����¶ȹ��߾���
#define PROTOCOL_ALARM_TYPE_MOTION                  0x09         // �ƶ�
#define PROTOCOL_ALARM_TYPE_COLLSION                0x10
#define PROTOCOL_ALARM_TYPE_TILT                    0x11         // ��б
#define PROTOCOL_ALARM_TYPE_USB_CHARGING            0x12         // ���usb�������
#define PROTOCOL_ALARM_TYPE_USB_CHARGE_REMOVE       0x13         // ���usb���Ͽ�
#define PROTOCOL_ALARM_TYPE_FENCE_IN                0x14
#define PROTOCOL_ALARM_TYPE_FENCE_OUT               0x15
#define PROTOCOL_ALARM_TYPE_ACC_ON                  0x16
#define PROTOCOL_ALARM_TYPE_ACC_OFF                 0x17
#define PROTOCOL_ALARM_TYPE_IDLE_ON                 0x18
#define PROTOCOL_ALARM_TYPE_IDLE_OFF                0x19
#define PROTOCOL_ALARM_TYPE_POWERON                 0x20
#define PROTOCOL_ALARM_TYPE_UNMOUNT_REC             0x21         // ���
#define PROTOCOL_ALARM_TYPE_OPENCOVER_REC           0x22         // ���
#define PROTOCOL_ALARM_TYPE_OPENBOX_REC             0x23         // ����
#define PROTOCOL_ALARM_TYPE_FREEFALL_REC            0x24         // ����
#define PROTOCOL_ALARM_TYPE_HIGH_TEMP_REC           0x25         //�����¶ȹ��߾���
#define PROTOCOL_ALARM_TYPE_MOTION_REC              0x26         // �ƶ�
#define PROTOCOL_ALARM_TYPE_COLLSION_REC            0x27
#define PROTOCOL_ALARM_TYPE_TILT_REC                0x28         // ��б
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


/*��λ��ʽ�궨��*/
#define STL_MODE_GPS_GLONASS      0
#define STL_MODE_GPS_GALILEO      1
#define STL_MODE_GPS_BEIDOU       2

/*�������Ժ궨��*/
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

/* ��ǰʹ��sorsen���ͺ궨��*/
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



/* ���������궨��*/

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

/*���ܳ����궨��*/
#define RLYMODE_SAFE_MODE         0
#define RLYMODE_RUDE_MODE         1
#define RLYMODE_ALTERNATE_MODE    2  // RELAY = 1(�ж���·)������£�relay���ӶϿ�2s�л�һ��
//��rlymode��ֵ��С��3������(���ߣ�������Χ����λ�ƣ�����)���������ʱ��
//�Զ����ͣ���rlymode��ʱ���ָ���·
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

//trackģʽ��0ΪGPRS,1Ϊ��GPRS��SMS�� ��0��1�������Ϊ����ģʽ
#define DATA_TRACKER_MODE_GPRS                  0
#define DATA_TRACKER_MODE_NO_TRACKER            1
#define DATA_TRACHER_MODE_SMS                   2 // ��0��1�������Ϊ����ģʽ�����ŷ��ͼ��Ϊtrakeָ���·���ֵ

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

#define HISTORY_MAX_ITEM_INDEX           47500           //��ʷ�ļ���������
#define HISTORY_NUMBER_EACH_PACKET       15
#define HISTORY_NUMBER_EACH_FILE         78
#define HISTORY_MAX_FILE_SIZE            5928           //��ʷ�ļ�78�����ݣ�
#define HISTORY_READ_PACKET_SIZE         1140
#define HISTORY_PACKET_SEND_SIZE         1245         //��ʷ����ÿ�η��͵����ݰ�������15������
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


#define HISTORY_MAX_ALARM_ITEM_INDEX     4000          //��ʷ�ļ�������
#define HISTORY_ALARM_NUMBER_EACH_FILE   40
#define HISTORY_MAX_ALARM_FILE_SIZE      3040         //alarm�ļ�40������
#define HISTORY_ALARM_SIZE_EACH_ITEM     76

#elif defined(__PORTABLE_DEVICE__)
#define OTA_HISTORY_FLAG_INDEX           15

#define HISTORY_MAX_ITEM_INDEX           49000           //��ʷ�ļ���������
#define HISTORY_NUMBER_EACH_PACKET       18
#define HISTORY_NUMBER_EACH_FILE         90
#define HISTORY_MAX_FILE_SIZE            5940          //��ʷ�ļ�90�����ݣ�
#define HISTORY_READ_PACKET_SIZE         1188
#define HISTORY_PACKET_SEND_SIZE         1314           //��ʷ����ÿ�η��͵����ݰ�������18������
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
#define HISTORY_MAX_ALARM_ITEM_INDEX     5000          //��ʷ�ļ�������
#define HISTORY_MAX_ALARM_FILE_SIZE      3300         //alarm�ļ�50������
#define HISTORY_ALARM_SIZE_EACH_ITEM     66
#elif defined(__WIRED_DEVICE__)
#define OTA_HISTORY_FLAG_INDEX           24

#define HISTORY_MAX_ITEM_INDEX           49000           //��ʷ�ļ���������
#define HISTORY_NUMBER_EACH_PACKET       18
#define HISTORY_NUMBER_EACH_FILE         90
#define HISTORY_READ_PACKET_SIZE         1188
#define HISTORY_PACKET_SEND_SIZE         1314           //��ʷ����ÿ�η��͵����ݰ�������18������

#define HISTORY_READ_PACKET_SIZE_AES     1152 //72*16
#define HISTORY_NUMBER_EACH_PACKET_AES   16
/*#define HISTORY_PACKET_SEND_SIZE_AES     1264 // 79*16
#define OTA_GPS_PACKET_DATA_LEN_AES      79*/  //((63-15)/16 + 1)*16 + 15

#define HISTORY_READ_PACKET_SIZE_MD5     1184 //74*16
#define HISTORY_NUMBER_EACH_PACKET_MD5   16
/*#define HISTORY_PACKET_SEND_SIZE_MD5     1296 // 16*(81)
#define OTA_GPS_PACKET_DATA_LEN_MD5      81*/


#define HISTORY_ALARM_NUMBER_EACH_FILE   50
#define HISTORY_MAX_ALARM_ITEM_INDEX     5000          //��ʷ�ļ�������

#else

#define OTA_HISTORY_FLAG_INDEX           24

#define HISTORY_MAX_ITEM_INDEX           49000           //��ʷ�ļ���������
#define HISTORY_NUMBER_EACH_PACKET       19
#define HISTORY_NUMBER_EACH_FILE         95
#define HISTORY_MAX_FILE_SIZE            5795           //��ʷ�ļ�95�����ݣ�
#define HISTORY_READ_PACKET_SIZE         1159
#define HISTORY_PACKET_SEND_SIZE         1292           //��ʷ����ÿ�η��͵����ݰ�������19������
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
#define HISTORY_MAX_ALARM_ITEM_INDEX     5000          //��ʷ�ļ�������
#define HISTORY_MAX_ALARM_FILE_SIZE      3050         //alarm�ļ�50������
#define HISTORY_ALARM_SIZE_EACH_ITEM     61


#endif



#define OTA_PACKET_LEN_INDEX              4

#define OTA_DRIVE_SPEED_CNT_MAX           5


#define OTA_ACCELERATION_Z_ORG                  980
#define OTA_ACCE_LIST_BUFF_SIZE                 100

#define OTA_ACCIDENT_LIST_BUFF_SIZE             15

#ifdef __OTA_OBD_SUPPORT__
#define HISTORY_MAX_ACCIDENT_ITEM_INDEX         400          //��������ʷ�ļ�������
#define HISTORY_MAX_ACCIDENT_FILE_SIZE          13770         // 459byte * 30
#define HISTORY_ACCIDENT_NUMBER_EACH_FILE       30
#define HISTORY_ACCIDENT_NUMBER_EACH_PACKET     1
#define HISTORY_ACCIDENT_SIZE_EACH_ITEM         459
#define OTA_ACCIDENT_PACKET_DATA_MAX_LEN        466
#define OTA_ACCIDENT_PACKET_DATA_MAX_LEN_MD5    474  /// 466+8
#define OTA_ACCIDENT_PACKET_DATA_MAX_LEN_AES    479 // ((466-15)/16 + 1)*16 + 15
#else
#define HISTORY_MAX_ACCIDENT_ITEM_INDEX         500          //��������ʷ�ļ�������
#define HISTORY_ACCIDENT_NUMBER_EACH_FILE       30
#define HISTORY_ACCIDENT_NUMBER_EACH_PACKET     1
#define HISTORY_MAX_ACCIDENT_FILE_SIZE          12870         // 429byte * 30
#define HISTORY_ACCIDENT_SIZE_EACH_ITEM         429
#define OTA_ACCIDENT_PACKET_DATA_MAX_LEN        436

#define OTA_ACCIDENT_PACKET_DATA_MAX_LEN_MD5    444  /// 436+8
#define OTA_ACCIDENT_PACKET_DATA_MAX_LEN_AES    447 // ((436-15)/16 + 1)*16 + 15
#endif

#define HISTORY_MAX_BEHAV_GPS_ITEM_INDEX     2000          //��������ʷ�ļ�������
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
#define HISTORY_MAX_BEHAV_ACCE_ITEM_INDEX     2000          //��������ʷ�ļ�������
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
#define HISTORY_MAX_LOCK_ITEM_INDEX     4000        // �洢����ʷ����������
#define HISTORY_MAX_LOCK_FILE_SIZE      8800        // ÿ���ļ��洢�������ܴ�С   ������*��ʷ����size  200*43
#define HISTORY_LOCK_NUMBER_EACH_FILE   200         // ÿ���ļ����Դ洢����ʷ����������
#define HISTORY_LOCK_SIZE_EACH_PACKET   880         // ÿ������Ӧ����ʷ���ݴ洢��С������*��ʷ����size  20*43
#define HISTORY_LOCK_SIZE_SEND_PACKET   1020        // ������ʷ����ʱ��ÿ�����Ĵ�С:  ����*�ϴ�����size  20*50
#define HISTORY_LOCK_NUMBER_EACH_PACKET 20          // ������ʷ����ʱ��ÿ������������������
#define HISTORY_LOCK_SIZE_EACH_ITEM     44          // ������ʷ���ݴ�С����ԭʼ������7���ֽڡ�������ʷ����size
#define OTA_LOCK_PACKET_DATA_LEN        51          // �ϴ��ĵ������ݴ�С���ϴ�����size
#endif

#define OTA_EXTERNAL_POWER_MEASURE_TIMES   7

#define AGPS_SERVER_IN_CHAINA   0
#define AGPS_SERVER_OVERSEA     1

#define OTA_AGPS_ONLINE_VALID_2_HOURS           2
#define OTA_AGPS_ONLINE_VALID_3_DAYS_WHEN_MIX   3
#define OTA_AGPS_OFFLINE_VALID_14_DAYS          14

#define OTA_SOCKET_IMPORTANT_SHAKE_HANDS_MODE   0   //�����Ҫ���ݽ�������
#define OTA_SOCKET_DO_NOT_SHAKE_HANDS_MODE      1   //ֻ��������
#define OTA_SOCKET_COMPLETELY_SHAKE_HANDS_MODE  2   //ȫ����

#define OTA_MULTIPLE_SERVERS_ALONE_MODE         0   //IP1��IP2����ʹ��ģʽ
#define OTA_MULTIPLE_SERVERS_SIMULTANEOUS_MODE  1   //IP1��IP2ͬʱʹ��ģʽ

#define OTA_SOCKET_CONNECTION_LONG_LINK_MODE      0   //������ģʽ
#define OTA_SOCKET_CONNECTION_ALTERNATE_LINK_MODE 1   //�������ӽ�����ģʽ
#define OTA_SOCKET_CONNECTION_SHORT_LINK_MODE     2   //������ģʽ

#define OTA_SEND_ALERT_SMS_ANY_REPLY_MODE              0   //�����ܾ�����Ϣ��������û�лظ������ᷢ������Ӧ���õĶ��Ÿ�����Ա����
//#define OTA_SEND_ALERT_SMS_DO_NOT_REPLY_DELAY_15S_MODE 1   //������15����û�лظ��ľͷ���Ӧ�ı������ţ����15���ڵõ��������Ļظ����Ͳ������õĶ���
#define OTA_DO_NOT_SEND_ALERT_SMS_MODE                 1   //����ȡ�����ö��ŷ��ͣ�������û�����ù���Ա����

#define OTA_OPER_TYPE_LONGIN_STATUS_OK           0
#define OTA_OPER_TYPE_LONGIN_STATUS_LOGGING      1
#define OTA_OPER_TYPE_LONGIN_STATUS_WAITSET      2
#define OTA_OPER_TYPE_LONGIN_STATUS_SETTING      3
#define OTA_OPER_TYPE_LONGIN_STATUS_CLOSED       4
#define OTA_OPER_TYPE_HEART_COUNT_ZERO           5
#define OTA_OPER_TYPE_HEART_COUNT_ONE            6



/*������������*/
#define OTA_COUNT_MIN                       0
#define OTA_ACC_AC_STATE_VALID_CNT_MAX      10     //ACC ����AC����5s����ͬһ��״̬������Ϊ���豸״̬Ϊ��ǰ״̬
#define OTA_MOVE_COUNT_MAX                  10    // GSENSOR���𶯵��ж���Ҫ������30������Ϊ�ն����˶���
#define OTA_GPS_FILTER_STATIC_CNT_MAX       5    //����̬Ư�ƹ���5�ζ�û���˶�������Ϊ�Ǿ�ֹ��
#define OTA_GPS_FILTER_MOVE_CNT_MAX         4    //acconʱ����̬Ư�ƹ���1�ζ��˶�������Ϊ���˶���
#define OTA_GPS_ACCOFF_FILTER_MOVE_CNT_MAX  2    //accoffʱ����̬Ư�ƹ���1�ζ��˶�������Ϊ���˶���
#define OTA_SOCKET_CONNECT_TIME_MAX         240
#define OTA_REBOOT_CNT_MAX                  600     //login10����û���յ�response����������10���������ϣ�gps10����δ�ɹ���λ������
#define OTA_REBOOT_CNT_WHEN_RECIVE_SMS      510   //�յ��¶��ţ��ӳ�90s����
#define OTA_GPS_NOT_OPEN_TIMER_OUT          60
#define OTA_GEOFENCE_CHECK_INTERVAL         3
#define OTA_MANAGER_NUMBER_FAILED_MAX_CNT   10
#define OTA_ALARM_CHECK_RESPONSE_TIME_MAX   5
#define OTA_SAVE_DIS_TO_NV_CNT              6
#define OTA_FEED_WATCH_DOG_INTERVAL         1
#define OTA_LOGIN_FIRST_INTERVAL            26
#define OTA_LOGIN_SECOND_INTERVAL           51


/*�ַ������ȳ����궨��*/
#define OTA_IMEI_BYTE_SIZE         8              //������8���ֽڴ洢
#define OTA_IMEI_STR_LEN           16             //imei���ַ�������Ϊ16
#define OTA_IMSI_STR_LEN           16
#define OTA_SERVER_ENCODE_DATA_LEN 8
#define OTA_PLMN_STR_LEN           7
#define OTA_COUNTRY_CODE_STR_LEN   3

#define OTA_PASSWORD_MAXLEN            10             // �豸password������������õ�
#define OTA_PASSWORD_MINLEN            4
#define OTA_IP_MAXLEN                  16             // IP����
#define OTA_DOMAIN_MAXLEN              100            //��������
#define OTA_PORT_MAXLEN                10             //�˿ڳ���

#define OTA_APN_MAXLEN                 50             // apn ��󳤶�
#define OTA_NUMBER_MAXLEN              30             // ������󳤶�
#define OTA_GEOFENCE_FILE_SIZE         1000           // ����Χ����󳤶�
#define OTA_UPGRADE_FILENAME_MAXLEN    200            //OTA���ذ�������󳤶�
#define OTA_UPGRADE_FILEAPPENDIX_LEN   10
#define OTA_ALARM_DATA_NUMBER          22             //�洢�ľ�������
#define OTA_CMD_MAX_LEN                30             // �·�ָ����󳤶�
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


#define GPRS_SEND_DATA_SIZE        1300           // ����Χ���ظ�����
#define OTA_LOG_DATA_MAXLEN        150
#define OTA_GLOBAL_VAR_FILE_MAXLEN 400

#define OTA_GEOFENCE_ITEM_HEAD_LEN   3
#define OTA_ALARM_SMS_ITEM_HEAD_LEN  2

#ifdef __SOCKET_OSM_GOOGLE_ADDR_REQUEST__
#define OTA_OSM_GOOGLE_URL_PACKET_DATA_LEN       230
#define OTA_DOMAIN_ADDR_REQUEST_MAXLEN           31            //��������
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
#define OTA_LOCK_TIME_INDEX             15   //Э�������У�ʱ��(6���ֽ�)��λ��
#define OTA_LOCK_TIME_HIS_INDEX         8    //��ŵ���ʷ�����У�ʱ��(6���ֽ�)��λ�ã���Ϊȥ����IMEI,λ�û�ǰ��

#define OTA_LOCK_NFCID_CNT_SHOW         10   //��ѯNFC��id������������mcu��bg95֮��Ĵ����������ƣ���������ÿ��ֻ�ܲ鵽10��NFCid
#define OTA_LOCK_NFCID_CNT_MAX          50   //NFC��id����������������ֵҪ��MCU��һ��
#define OTA_LOCK_NFCID_MAX              20   //NFC��id����󳤶���BG95�˵���󳤶ȣ����ֵҪ��MCU��һ��,id�洢����󳤶���10���ֽڣ�������ʾ����asc�룬��Ҫ20���ֽ�
#define OTA_LOCK_NFCID_MCU_MAX          10   //NFC��id����MCU����󳤶���10���ֽڣ�������ʾ����asc�룬��Ҫ20���ֽ�
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

/*��ʱ����ʱʱ�䳣���궨��*/
#define OTA_TIMER_OTA_FILE_DELAY              5*1000
#define OTA_TIMER_DELAY_MAIN                  1*1000
#define OTA_TIMER_SEND_GPS_FIRST_TIME         7   //��������login 7s���һ�η���gps����
#define OTA_TIMER_DELAY_DISCONNECT            (3*1000)
#define OTA_TIMER_ONE_SEC_DELAY               (1000)
#define OTA_TIMER_THREE_SEC_DELAY             (3*1000)
#define OTA_TIMER_FIVE_SEC_DELAY              (5*1000)
#define OTA_TIMER_DELAY_ADDR_DELAY            (15*1000)
#define OTA_TIMER_DELAY_AGPS_OFFLINE          (10*1000)

#define OTA_TIMER_ALARM_TIMEOUT_RESEND        9*1000
#define OTA_TIMER_DELAY_CALL_SOS              10*1000
#define OTA_TIMER_DELAY_CHECK_SOCKET_RESPONSE 15*1000   //��ʱ15s����������û�����alarm�ϱ���Ӧ
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
/*������λת���궨��*/
#ifndef OTA_M_PI
#define OTA_M_PI 3.1415926
#endif
#ifndef OTA_TWO_PI
#define OTA_TWO_PI 3.1415926 * 2
#endif

#ifndef OTA_RAD_TO_DEG
#define OTA_RAD_TO_DEG 57.2957795  // ����ת�Ƕȵ�ת����= 180/PI
#endif

#ifndef OTA_DEG_TO_RAD
#define OTA_DEG_TO_RAD 0.0174533 // �Ƕ�ת���ȵ�ת����= PI/180
#endif

#ifndef OTA_KM_TO_MILE
#define OTA_KM_TO_MILE  0.6214    //ǧ��תΪ����= 1/1.609
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
#define OTA_BLE_SENSOR_TICK_MIN                     100 /*Լ������: OTA_BLE_SENSOR_TICK_MAX < OTA_BLE_SENSOR_TICK_MIN*2*/
#define OTA_BLE_SENSOR_TICK_LOST_CNT                6   /*6������û���ٴ�ɨ������Ϊ��ʧ�������ʱʱ����OTA_BLE_SENSOR_TICK_LOST_CNT��OTA_TIMER_BTSENSOR_DELAY��ͬ����*/
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

