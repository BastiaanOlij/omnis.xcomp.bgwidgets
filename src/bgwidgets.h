#ifndef bgwidgetsh
#define bgwidgetsh

#include <extcomp.he>
#include <extfval.he>
#include <gdi.he>
#include <hwnd.he>

#include <float.h>
#include <stdio.h>
#include <string.h>

#define OBJECT_COUNT 3 /* Number of controls within library */
#define LIB_RES_NAME 1000 /* Resource id of library name */
#define RADIUSRECT_ID 2000 /* Resource id of control within library */
#define RADIUSRECT_ICON 1 /* Resource bitmap id */
#define DONUT_ID 2001 /* Resource id of control within library */
#define DONUT_ICON 2 /* Resource bitmap id */
#define POLYGON_ID 2002 /* Resource id of control within library */
#define POLYGON_ICON 3 /* Resource bitmap id */

class bgwidget {
private:
	HWND mHwnd; /* hwnd for our background object, ONLY valid in design! */

protected:
	bool mUseHwnd; /* if true we can use mHwnd */

public:
	bgwidget(HWND pHwnd, bool pUseHwnd);
	virtual ~bgwidget();

	void invalidate();

	virtual qbool propCanAssign(qlong pPropId);
	virtual qbool setProperty(qlong pPropId, EXTfldval *pSetVal);
	virtual qbool getProperty(qlong pPropId, EXTfldval *pRetVal);
	void UpdateProperty(qlong pPropId = 0);

	virtual void paint(HDC hdc, qrect *pArea);
};

#endif
