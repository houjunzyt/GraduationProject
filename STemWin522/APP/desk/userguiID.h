#ifndef _USERGUIID_H_
#define _USERGUIID_H_
//////TestAPP.c����ʹ�õ��ź�
//BagAPP�ڲ�С�ؼ�ID
#define GUI_ID_USER_APP (GUI_ID_USER)
#define ID_FRAMEWIN_0   (GUI_ID_USER_APP + 0x01)
#define ID_TEXT_0       (GUI_ID_USER_APP + 0x02)

//BookAPP�ڲ�С�ؼ�ID
#define ID_FRAMEWIN_1   (GUI_ID_USER_APP + 0x03)
#define ID_TEXT_1       (GUI_ID_USER_APP + 0x04)

//BookshAPP�ڲ�С�ؼ�ID
#define ID_FRAMEWIN_2   (GUI_ID_USER_APP + 0x05)
#define ID_TEXT_2       (GUI_ID_USER_APP + 0x06)

//BrowerAPP�ڲ�С�ؼ�ID
#define ID_FRAMEWIN_3   (GUI_ID_USER_APP + 0x07)
#define ID_TEXT_3       (GUI_ID_USER_APP + 0x08)
 
//////iconviewdemo.c����ʹ�õ��ź�
//�Զ����û��ź�
#define GUI_ID_USER_DESK              (GUI_ID_USER+0x20)  //1-0x20Ϊ������Ԥ�� 
#define ID_ICONVIEW_0                 (GUI_ID_USER_DESK + 0x01) //��ͼ�����
#define ID_TIMER_TIME                 (GUI_ID_USER_DESK + 0x02) //����ʱ��
#define ID_SPRITE_TIME                (GUI_ID_USER_DESK + 0x03)
#define ID_BUTTON_BKGND               (GUI_ID_USER_DESK + 0x04) //״̬������
#define ID_MENU_LOG                   (GUI_ID_USER_DESK + 0x05) //�����˵� kernel log
#define ID_MENU_PMGR                  (GUI_ID_USER_DESK + 0x06) //�����˵� process viewer
#define ID_MENU_EXIT                  (GUI_ID_USER_DESK + 0x07) //�����˵� cancel
#define ID_MENU_PERF                  (GUI_ID_USER_DESK + 0x08) //�����˵� cpu usage

#define ID_FRAMEWIN_PERFORMANCE       (GUI_ID_USER_DESK + 0x10)
#define ID_GRAPH_CPU                  (GUI_ID_USER_DESK + 0x11)
#define ID_BUTTON_HIDE                (GUI_ID_USER_DESK + 0x12)

#define ID_FRAMEWIN_KERNELLOG         (GUI_ID_USER_DESK + 0x20)
#define ID_BUTTON_CANCEL_KERNELLOG    (GUI_ID_USER_DESK + 0x21)
#define ID_MULTIEDIT_KERNELLOG        (GUI_ID_USER_DESK + 0x22)

#define ID_FRAMEWIN_PROCESSMANAGER       (GUI_ID_USER_DESK + 0x30)
#define ID_BUTTON_CANCEL_PROCESSMANAGER  (GUI_ID_USER_DESK + 0x31)
#define ID_LISTVIEW_PROCESSMANAGER       (GUI_ID_USER_DESK + 0x32)
#endif
