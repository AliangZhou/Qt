#ifndef PUBLIC_H
#define PUBLIC_H

#include "typedef.h"

#include <QDebug>
#include <QVariant>

#define TOUCH_SCREEN_WIDTH  	800
#define TOUCH_SCREEN_HIGH   	480

#define FONT_NAME          		"mshei"
#define FONT_SIZE_14        	14
#define FONT_SIZE_18        	18
#define FONT_SIZE_24        	24
#define FONT_SIZE_32			24
#define FONT_WEIGHT				50

#define BUTTON_WIDTH_SIZE   	110
#define BUTTON_WIDTH_SIZE1  	200
#define BUTTON_HEIGHT_SIZE  	45

#define LANGUAGE_ENGLISH		"/opt/language/english.qm"
#define	LANGUAGE_CH          	"/opt/language/zh_cn.qm"
#define LANGUAGE_HK          	"/opt/language/zh_hk.qm"

#define RETURN_ERROR			0
#define RETURN_OK				1

#define BUTTUN_STYLE_CENTER           "border:1px solid #000000;border-radius:5px;background-color:white;color:black;text-align:center;"
#define BUTTUN_STYLE_LEFT             "border:1px solid #000000;border-radius:5px;background-color:#F5F5F5;color:black;text-align:left;"
#define DISABLE_BUTTUN_STYLE_SHEET  	"border:1px solid #000000;border-radius:5px;background-color:black;color:white;text-align:left;"

#define UPG_VERSION        		"CONSOLE  VER:%02d.%02d.%02d.%s"

#define msgDataMaxLen     		4096

#define COMMAND_MAX_LEN 		4096

#define DEFINE_SOH_VALUE   0x20484F53

typedef struct
{
    int msgId;
	int dataLen;
    char data[msgDataMaxLen];
}msg;

typedef struct
{
    long int msgType;
    msg msgObj;
}msgDataType;

enum
{
    english = 0,
    chinese_s,
    chinese_t,
    languageNum,
};

#define TCP_DATA_MAX_LEN    	1024*1024
#define UART_DATA_MAX_LEN    	1024*1024

#define DISPLAY_STRING			0x1010

#define TCP_SERVER_PORT         8080
#define BROADCAST_SERVER_PORT   8081

typedef struct
{
    U16 commandId;
    U08 dataType;
    U16 datalen;
    U08 data[COMMAND_MAX_LEN];
}dataFrameContent;

#define SET_POINT_POS 0x0001

Q_DECLARE_METATYPE(dataFrameContent)

#define SOFT_RELEASE									(1)

#if SOFT_RELEASE
#define PRINTF_GUI_COMMAND_DATA_ENABLE 					(0)//GUI 参数信息
#define DEBUG_COMMON_INFORMATION_DISPLAY_ENABLE 		(1)//普通打印信息
#define DEBUG_KEY_INFORMATION_DISPLAY_ENABLE 			(0)//关键打印信息
#define DEBUG_OK_INFORMATION_DISPLAY_ENABLE 			(0)//正确打印信息
#define DEBUG_ERROR_INFORMATION_DISPLAY_ENABLE 			(0)//错误打印信息
#define DEBUG_SOCKET_INFORMATION_DISPLAY_ENABLE 		(0)//socket调试信息
#else
#define PRINTF_GUI_COMMAND_DATA_ENABLE 					(1)
#define DEBUG_COMMON_INFORMATION_DISPLAY_ENABLE 		(1)
#define DEBUG_KEY_INFORMATION_DISPLAY_ENABLE 			(1)
#define DEBUG_OK_INFORMATION_DISPLAY_ENABLE 			(1)
#define DEBUG_ERROR_INFORMATION_DISPLAY_ENABLE 			(1)
#define DEBUG_SOCKET_INFORMATION_DISPLAY_ENABLE 		(1)
#endif

#if PRINTF_GUI_COMMAND_DATA_ENABLE//GUI传递给Server的命令参数信息
#define PRINTF_GUI_COMMAND_DATA
#endif

#if DEBUG_COMMON_INFORMATION_DISPLAY_ENABLE
#define DEBUG_COMMON_INFORMATION_DISPLAY//普通调试信息
#endif

#if DEBUG_KEY_INFORMATION_DISPLAY_ENABLE
#define DEBUG_KEY_INFORMATION_DISPLAY//关键调试信息
#endif

#if DEBUG_OK_INFORMATION_DISPLAY_ENABLE
#define DEBUG_OK_INFORMATION_DISPLAY//程序运行正确信息
#endif

#if DEBUG_ERROR_INFORMATION_DISPLAY_ENABLE
#define DEBUG_ERROR_INFORMATION_DISPLAY//程序运行错误信息
#endif

#if DEBUG_SOCKET_INFORMATION_DISPLAY_ENABLE
#define DEBUG_SOCKET_INFORMATION_DISPLAY//关于socket的调试信息
#endif

#endif //PUBLIC_H

