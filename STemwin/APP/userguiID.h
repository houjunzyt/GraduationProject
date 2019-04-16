#ifndef _USERGUIID_H_
#define _USERGUIID_H_
/////TestAPP.c里面使用的信号
//BagAPP内部小控件ID
#define GUI_ID_USER_APP (GUI_ID_USER)
#define ID_FRAMEWIN_0   (GUI_ID_USER_APP + 0x01)
#define ID_TEXT_BAG       (GUI_ID_USER_APP + 0x02)

//BookAPP内部小控件ID
#define ID_FRAMEWIN_1   (GUI_ID_USER_APP + 0x03)
#define ID_TEXT_BOOK      (GUI_ID_USER_APP + 0x04)

//BookshAPP内部小控件ID
#define ID_FRAMEWIN_2   (GUI_ID_USER_APP + 0x05)
#define ID_TEXT_SH       (GUI_ID_USER_APP + 0x06)

//BrowerAPP内部小控件ID
#define ID_FRAMEWIN_3   (GUI_ID_USER_APP + 0x07)
#define ID_TEXT_BR       (GUI_ID_USER_APP + 0x08)
 
//////iconviewdemo.c里面使用的信号
//自定义用户信号
#define GUI_ID_USER_DESK              (GUI_ID_USER+0x20)  //1-0x20为给以上预留 
#define ID_ICONVIEW_0                 (GUI_ID_USER_DESK + 0x01) //对图标操作
#define ID_TIMER_TIME                 (GUI_ID_USER_DESK + 0x02) //窗口时间
#define ID_SPRITE_TIME                (GUI_ID_USER_DESK + 0x03)
#define ID_BUTTON_BKGND               (GUI_ID_USER_DESK + 0x04) //状态栏按键
#define ID_MENU_LOG                   (GUI_ID_USER_DESK + 0x05) //按键菜单 kernel log
#define ID_MENU_PMGR                  (GUI_ID_USER_DESK + 0x06) //按键菜单 process viewer
#define ID_MENU_EXIT                  (GUI_ID_USER_DESK + 0x07) //按键菜单 cancel
#define ID_MENU_PERF                  (GUI_ID_USER_DESK + 0x08) //按键菜单 cpu usage

#define ID_FRAMEWIN_PERFORMANCE       (GUI_ID_USER_DESK + 0x0A)
#define ID_GRAPH_CPU                  (GUI_ID_USER_DESK + 0x0B)
#define ID_BUTTON_HIDE                (GUI_ID_USER_DESK + 0x0C)

#define ID_FRAMEWIN_KERNELLOG         (GUI_ID_USER_DESK + 0x0D)
#define ID_BUTTON_CANCEL_KERNELLOG    (GUI_ID_USER_DESK + 0x0E)
#define ID_MULTIEDIT_KERNELLOG        (GUI_ID_USER_DESK + 0x0F)

#define ID_FRAMEWIN_PROCESSMANAGER       (GUI_ID_USER_DESK + 0x10)
#define ID_BUTTON_CANCEL_PROCESSMANAGER  (GUI_ID_USER_DESK + 0x11)
#define ID_LISTVIEW_PROCESSMANAGER       (GUI_ID_USER_DESK + 0x12)

//系统设置APP信号
#define GUI_ID_USER_SYSTEMSET   (GUI_ID_USER+0x20)

#define ID_FRAMEWIN_INFO        (GUI_ID_USER_SYSTEMSET + 0x01)
#define ID_IMAGE_INFO           (GUI_ID_USER_SYSTEMSET + 0x02)
#define ID_TEXT_BOARD           (GUI_ID_USER_SYSTEMSET + 0x03)
#define ID_TEXT_CORE            (GUI_ID_USER_SYSTEMSET + 0x04)
#define ID_TEXT_CPU             (GUI_ID_USER_SYSTEMSET + 0x05)
#define ID_TEXT_VERSION         (GUI_ID_USER_SYSTEMSET + 0x06)
#define ID_TEXT_COPYRIGHT       (GUI_ID_USER_SYSTEMSET + 0x07)
#define ID_MULTIPAGE            (GUI_ID_USER_SYSTEMSET + 0x08)
#define ID_IMAGE_LOGO           (GUI_ID_USER_SYSTEMSET + 0x09)    

#define ID_CHECKBOX_SPRITES     (GUI_ID_USER_SYSTEMSET + 0x0A)
#define ID_CHECKBOX_CPU180      (GUI_ID_USER_SYSTEMSET + 0x0B)
#define ID_CHECKBOX_FLEXSKIN    (GUI_ID_USER_SYSTEMSET + 0x0C)

#define ID_TEXT_WIFI            (GUI_ID_USER_SYSTEMSET + 0x0D)
#define ID_TEXT_WARNING1        (GUI_ID_USER_SYSTEMSET + 0x0E)
#define ID_SPINBOX_HOUR         (GUI_ID_USER_SYSTEMSET + 0x0F)
#define ID_TEXT_0               (GUI_ID_USER_SYSTEMSET + 0x10)
#define ID_TEXT_1               (GUI_ID_USER_SYSTEMSET + 0x11)
#define ID_SPINBOX_MINUTE       (GUI_ID_USER_SYSTEMSET + 0x12)
#define ID_BUTTON_APPLYDATESETTINGS  (GUI_ID_USER_SYSTEMSET + 0x13)
#define ID_BUTTON_APPLYCLKSETTINGS   (GUI_ID_USER_SYSTEMSET + 0x14)
#define ID_BUTTON_1             (GUI_ID_USER_SYSTEMSET + 0x15)
#define ID_TEXT_2               (GUI_ID_USER_SYSTEMSET + 0x16)
#define ID_CALENDAR             (GUI_ID_USER_SYSTEMSET + 0x17)
#define ID_SPINBOX_SEC          (GUI_ID_USER_SYSTEMSET + 0x18)
#define ID_BUTTON_WIFI          (GUI_ID_USER_SYSTEMSET + 0x19)
#define ID_BUTTON_Temp          (GUI_ID_USER_SYSTEMSET + 0x1A)
#define ID_TEXT_Temp            (GUI_ID_USER_SYSTEMSET + 0x1B)

//温度APP
#define GUI_ID_USER_Temp  (GUI_ID_USER+0x60)
#define ID_GRAPH_Temp  	  (GUI_ID_USER_Temp +0x01)
#define ID_FRAMEWIN_Temp  (GUI_ID_USER_Temp +0x02)
#define ID_FRAMEWIN_T0  (GUI_ID_USER_Temp + 0x03)
#define ID_IMAGE_0  (GUI_ID_USER_Temp + 0x05)
#define ID_IMAGE_1  (GUI_ID_USER_Temp + 0x06)
#define ID_TEXT_T0  (GUI_ID_USER_Temp + 0x07)
#define ID_TEXT_T1  (GUI_ID_USER_Temp + 0x09)
#define ID_TEXT_T2  (GUI_ID_USER_Temp + 0x0A)
#define ID_TEXT_T3  (GUI_ID_USER_Temp + 0x0B)
#define ID_TEXT_T4  (GUI_ID_USER_Temp + 0x0C)
#define ID_TEXT_T5  (GUI_ID_USER_Temp + 0x0D)
#define ID_TEXT_T6  (GUI_ID_USER_Temp + 0x0E)
#define ID_TEXT_T7  (GUI_ID_USER_Temp + 0x0F)
#define ID_IMAGE_T0  (GUI_ID_USER_Temp + 0x10)
#define ID_IMAGE_T1  (GUI_ID_USER_Temp + 0x11)


//湿度APP
#define GUI_ID_USER_Hum (GUI_ID_USER+0x80)
#define ID_FRAMEWIN_H0  (GUI_ID_USER_Hum + 0x00)
#define ID_IMAGE_H0     (GUI_ID_USER_Hum + 0x01)
#define ID_IMAGE_H1     (GUI_ID_USER_Hum + 0x02)
#define ID_IMAGE_H2     (GUI_ID_USER_Hum + 0x03)
#define ID_TEXT_H0      (GUI_ID_USER_Hum + 0x04)
#define ID_TEXT_H1      (GUI_ID_USER_Hum + 0x05)






#endif
