#include "bgwidgets.h"

enum radiusrectprops {
	rrectRadius = 1,
	rrectLineWidth,
	rrectlast
};

extern "C" ECOproperty RadiusRectProperties[];

class radiusrect : public bgwidget {
private:
	qlong						mRadius;																// Our radius
	qpat						mBackpattern;														// Our back pattern
	qcol						mForecolor, mBackcolor;									// Our forecolor and backcolor
	qlong						mBorderWidth;														// Our border width
	qcol            mBorderColor;                           // Our border color


public:
	radiusrect(HWND pHwnd, bool pUseHwnd);

	virtual qbool propCanAssign(qlong pPropId);
	virtual qbool setProperty(qlong pPropId, EXTfldval * pSetVal);
	virtual qbool getProperty(qlong pPropId, EXTfldval * pRetVal);

	virtual void paint(HDC hdc, qrect* pArea);
};
