#include "iconviewdemo.h"
#include "iconbitmap.h"
#include "background.h"
#include "DIALOG.h"
#include "FRAMEWIN.h"
#include "TestAPP.h"

#define ID_ICONVIEW_0   (GUI_ID_USER + 0x01)

WM_HWIN IconviewWin;    //ICONVIEW�ؼ�����

//appͼ�������ṹ��
typedef struct{
	const GUI_BITMAP 	*pBitmap;   //appͼ��
	const char			*pText;     //appͼ������
}BITMAP_ITEM;

//ICONVIEW��ʾ��APPͼ��
static const BITMAP_ITEM BitmapItemTab1[]={
	{&bmbag,		"Bag"},
	{&bmbook,		"Book"},
	{&bmbookshelf,	"Booksh"	},
	{&bmbrowser,	"Brower"},
	{&bmcalendar,	"Calendar"},
	{&bmclipboard,	"Clipboard"},
//	{&bmcompass,	"Compass"},
//	{&bmcompose,	"Cmpose"},
//	{&bmdocument,	"Cument"},
//	{&bmfolder,		"Folder"},
//	{&bmmail,		"Mail"},
//	{&bmpaint,		"Paint"},
//	{&bmpin,		"Pin"},
//	{&bmsettings,	"Set"},
//	{&bmtools,		"Tool"},
//	{&bmturntable,	"TurnTab"},
//	{&bmvideo,		"Video"},
//	{&bmwallet,		"Wallet"},
//	{&bmlightbulb,	"Light"},
//	{&bmstar,		"Star"},
};

//���洰��WM_HBKWIN�ص�����
void cb_BkWindow(WM_MESSAGE *pMsg)
{
	int Id;
	int NCode;
	static int Iconview0_Sel;
	
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
            GUI_SetBkColor(GUI_BLACK);
            GUI_Clear();                        
			GUI_DrawBitmap(&bmbackground,0,0);  //���Ʊ���ͼƬ
			break;
		case WM_NOTIFY_PARENT:
			Id=WM_GetId(pMsg->hWinSrc);
			NCode=pMsg->Data.v;
			switch(Id)
			{
				case ID_ICONVIEW_0:	//ICONVIEW0�ؼ�
					switch(NCode)	//֪ͨ����
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							Iconview0_Sel=ICONVIEW_GetSel(pMsg->hWinSrc);
							switch(Iconview0_Sel)
							{
								case 0:		//APP0
                                    if(BagAPPWin==NULL)CreateBagAPP();      //����BagAPP
									break;
								case 1:		//APP1
                                    if(BookAPPWin==NULL)CreateBookAPP();    //����BookAPP
									break;
                               	case 2:		//APP2
                                    if(BookshAPPWin==NULL)CreateBookshAPP();    //����BookshAPP
									break;
                                case 3:		//APP3
                                    if(BrowerAPPWin==NULL)CreateBrowerAPP();    //����BrowerAPP
									break;
							}
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
							break;	
					}
					break;
			}
			break;
		default:
			WM_DefaultProc(pMsg);
			break;		
	}
	
}

//ICONVIEW��ʾ����
void iconviewdemo(void) 
{
	int i;
	
	//���û��� 
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	
	WM_SetCallback(WM_HBKWIN,cb_BkWindow); //�������洰��WM_HBKWIN�Ļص�����
	//����һ��ICONVIEW��Ϊ������
	IconviewWin=ICONVIEW_CreateEx(0,0,                    //���Ͻ�����
							240,320,                        //С���ߵ�ˮƽ�ʹ�ֱ�ߴ�
							WM_HBKWIN,                      //������Ϊ��������
							WM_CF_SHOW|WM_CF_HASTRANS,      
							ICONVIEW_CF_AUTOSCROLLBAR_V,    //�Զ����Ӵ�ֱ������
							ID_ICONVIEW_0,                  //С����ID
							100,                            //ͼ���ˮƽ�ߴ�Ϊ100
							100);                           //ͼ��Ĵ�ֱ�ߴ�Ϊ100
	ICONVIEW_SetFont(IconviewWin,&GUI_Font16_ASCII);
	ICONVIEW_SetBkColor(IconviewWin,ICONVIEW_CI_SEL,GUI_WHITE|0X70000000);
	ICONVIEW_SetSpace(IconviewWin,GUI_COORD_X,15);
	ICONVIEW_SetSpace(IconviewWin,GUI_COORD_Y,15);
	ICONVIEW_SetIconAlign(IconviewWin,ICONVIEW_IA_HCENTER|ICONVIEW_IA_VCENTER);
	for(i=0;i<(GUI_COUNTOF(BitmapItemTab1));i++)
	{
		ICONVIEW_AddBitmapItem(IconviewWin,BitmapItemTab1[i].pBitmap,BitmapItemTab1[i].pText);
	}
}
