#include "bgwidgets.h"

#define DONUT_MAX_COLORS 512

enum donutprop {
	donutInnerRadius = 1,
	donutBaseColor,
	donutColorIndex,
	donutColorAngle,
	donutColor,
	donutOffsetAngle,
	donutLast
};

extern "C" ECOproperty DonutProperties[];

class donut : public bgwidget {
private:
	qlong mInnerRadius;
	qcol mBaseColor;
	int mColorIndex;
	int mColorAngles[DONUT_MAX_COLORS];
	qcol mColors[DONUT_MAX_COLORS];
	float mOffsetAngle;

	void addInnerPoints(qpoint *pOuterPoints, const qpoint *pInnerPoints, int pNumPoints);

public:
	donut(HWND pHwnd, bool pUseHwnd);

	virtual qbool propCanAssign(qlong pPropId);
	virtual qbool setProperty(qlong pPropId, EXTfldval *pSetVal);
	virtual qbool getProperty(qlong pPropId, EXTfldval *pRetVal);

	virtual void paint(HDC hdc, qrect *pArea);
};
