#ifndef GUICONF_H
#define GUICONF_H

/*********************************************************************
*
*       Multi layer/display support
*/
#define GUI_NUM_LAYERS            2    // Maximum number of available layers

/*********************************************************************
*
*       Multi tasking support
*/

#define GUI_OS                    (1)


/*********************************************************************
*
*       Configuration of touch support
*/
#ifndef   GUI_SUPPORT_TOUCH
  #define GUI_SUPPORT_TOUCH       (1)  // Support touchscreen
#endif

/*********************************************************************
*
*       Default font
*/
#define GUI_DEFAULT_FONT          &GUI_Font6x8

/*********************************************************************
*
*         Configuration of available packages
*/
#define GUI_SUPPORT_MOUSE             (0)    /* Support a mouse */
#define GUI_WINSUPPORT                (1)    /* Use window manager */
#define GUI_SUPPORT_MEMDEV            (1)    /* Memory device package available */
#define GUI_SUPPORT_DEVICES           (1)    /* Enable use of device pointers */

#endif  /* Avoid multiple inclusion */
