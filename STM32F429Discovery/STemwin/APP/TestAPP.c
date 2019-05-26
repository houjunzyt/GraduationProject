#include "TestAPP.h"
#include "DIALOG.h"
#include "FRAMEWIN.h"
#include "userguiid.h"

WM_HWIN BagAPPWin=NULL;         //BagAPP����
WM_HWIN BookAPPWin=NULL;        //BookAPP����
WM_HWIN BookshAPPWin=NULL;      //BookshAPP����
WM_HWIN BrowerAPPWin=NULL;      //BrowerAPP����

//BagAPP��Դ��
static const GUI_WIDGET_CREATE_INFO BagAPPDialogCreate[]=
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 240, 240,0,0x64},
    { TEXT_CreateIndirect, "Text", ID_TEXT_BAG, 200, 200, 200, 20, 0, 0x64, 0 },
};

//BookAPP��Դ��
static const GUI_WIDGET_CREATE_INFO BookAPPDialogCreate[]=
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_1, 0, 0, 240, 240,FRAMEWIN_CF_MOVEABLE,0x64},
    { TEXT_CreateIndirect, "Text", ID_TEXT_BOOK, 70,85, 200, 20, 0, 0x64, 0 },
};

//BookshAPP��Դ��
static const GUI_WIDGET_CREATE_INFO BookshAPPDialogCreate[]=
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_2, 0, 0, 240, 240,FRAMEWIN_CF_MOVEABLE,0x64},
    { TEXT_CreateIndirect, "Text", ID_TEXT_SH,150,100, 200, 20, 0, 0x64, 0 },
};

//BrowerAPP��Դ��
static const GUI_WIDGET_CREATE_INFO BrowerAPPDialogCreate[]=
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_3, 0, 0, 240, 240,0,0x64},
    { TEXT_CreateIndirect, "Text", ID_TEXT_BR,200, 230, 200, 20, 0, 0x64, 0 },
};

//BagAPP�Ļص�����
static void _cbBagAPPDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
	int     NCode;
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
            //��ʼ��FRAMWIN
            hItem=pMsg->hWin;
            FRAMEWIN_SetText(hItem, "Bag APP");
            FRAMEWIN_SetFont(hItem,GUI_FONT_20_ASCII);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER);
            FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,2); //��ӹرհ�ť
            FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //�����󻯰�ť
            FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //�����С����ť
        
            //��ʼ��TEXT
            hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_BAG);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            TEXT_SetText(hItem, "This is Bag APP");
            TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
            break;
        case WM_NOTIFY_PARENT:
			NCode=pMsg->Data.v;
            if(NCode==WM_NOTIFICATION_CHILD_DELETED) BagAPPWin=0;//�Ӵ���ɾ����
			break;
        default:
            WM_DefaultProc(pMsg);
  }
}

//BookAPP�Ļص�����
static void _cbBookAPPDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
	int     NCode;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_RED);
            GUI_Clear();
            break;
        case WM_INIT_DIALOG:
            //��ʼ��FRAMWIN
            hItem=pMsg->hWin;
            FRAMEWIN_SetText(hItem, "Book APP");
            FRAMEWIN_SetFont(hItem,GUI_FONT_20_ASCII);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER);
            FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,2); //��ӹرհ�ť
            FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //�����󻯰�ť
            FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //�����С����ť
        
            //��ʼ��TEXT
            hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_BOOK);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            TEXT_SetText(hItem, "This is Book APP");
            TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
            break;
        case WM_NOTIFY_PARENT:
			NCode=pMsg->Data.v;
            if(NCode==WM_NOTIFICATION_CHILD_DELETED) BookAPPWin=0;//�Ӵ���ɾ����
			break;
        default:
            WM_DefaultProc(pMsg);
  }
}

//BookshAPP�Ļص�����
static void _cbBookshAPPDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
	int     NCode;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_BLUE);
            GUI_Clear();
            break;   
        case WM_INIT_DIALOG:
            //��ʼ��FRAMWIN
            hItem=pMsg->hWin;
            FRAMEWIN_SetText(hItem, "Booksh APP");
            FRAMEWIN_SetFont(hItem,GUI_FONT_20_ASCII);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER);
            FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,2); //��ӹرհ�ť
            FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //�����󻯰�ť
            FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //�����С����ť
        
            //��ʼ��TEXT
            hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_SH);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            TEXT_SetText(hItem, "This is Booksh APP");
            TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
            break;
        case WM_NOTIFY_PARENT:
			NCode=pMsg->Data.v;
            if(NCode==WM_NOTIFICATION_CHILD_DELETED) BookshAPPWin=0;//�Ӵ���ɾ����
			break;
        default:
            WM_DefaultProc(pMsg);
  }
}

//BrowerAPP�Ļص�����
static void _cbBrowerAPPDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
	int     NCode;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_YELLOW);
            GUI_Clear();
            break;  
        case WM_INIT_DIALOG:
            //��ʼ��FRAMWIN
            hItem=pMsg->hWin;
            FRAMEWIN_SetText(hItem, "Brower APP");
            FRAMEWIN_SetFont(hItem,GUI_FONT_20_ASCII);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER);
            FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,2); //��ӹرհ�ť
            FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //�����󻯰�ť
            FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //�����С����ť
        
            //��ʼ��TEXT
            hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_BR);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            TEXT_SetText(hItem, "This is Brower APP");
            TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
            break;
        case WM_NOTIFY_PARENT:
			NCode=pMsg->Data.v;
            if(NCode==WM_NOTIFICATION_CHILD_DELETED) BrowerAPPWin=0;//�Ӵ���ɾ����
			break;
        default:
            WM_DefaultProc(pMsg);
  }
}

//����BagAPP
void CreateBagAPP(void) 
{
    BagAPPWin=GUI_CreateDialogBox(BagAPPDialogCreate,GUI_COUNTOF(BagAPPDialogCreate),_cbBagAPPDialog,WM_HBKWIN,0,0);
}

//����BookAPP
void CreateBookAPP(void) 
{
    BookAPPWin=GUI_CreateDialogBox(BookAPPDialogCreate,GUI_COUNTOF(BookAPPDialogCreate),_cbBookAPPDialog,WM_HBKWIN,0,0);
}

//����BookshAPP
void CreateBookshAPP(void) 
{
    BookshAPPWin=GUI_CreateDialogBox(BookshAPPDialogCreate,GUI_COUNTOF(BookshAPPDialogCreate),_cbBookshAPPDialog,WM_HBKWIN,0,0);
}

//����BrowerAPP
void CreateBrowerAPP(void) 
{
    BrowerAPPWin=GUI_CreateDialogBox(BrowerAPPDialogCreate,GUI_COUNTOF(BrowerAPPDialogCreate),_cbBrowerAPPDialog,WM_HBKWIN,0,0);
}
