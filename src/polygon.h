#include "bgwidgets.h"

#define MAX_POINTS 256

enum polygonProps {
	polygonPoints = 1,
	polygonLineWidth,
	polygonLast
};

extern "C" ECOproperty PolygonProperties[];

class polygon : public bgwidget {
private:
	qpat			mBackpattern;                                                        // Our back pattern
	qcol			mForecolor, mBackcolor;                                    // Our forecolor and backcolor
	qlong			mBorderWidth;                                                        // Our border width
	qcol			mBorderColor;                           // Our border color
	qpoint			mPoints[MAX_POINTS];
    int				mNumPoints;
    
public:
	polygon(HWND pHwnd, bool pUseHwnd);
    
	virtual qbool propCanAssign(qlong pPropId);
	virtual qbool setProperty(qlong pPropId, EXTfldval * pSetVal);
	virtual qbool getProperty(qlong pPropId, EXTfldval * pRetVal);
    
	virtual void paint(HDC hdc, qrect* pArea);
};

