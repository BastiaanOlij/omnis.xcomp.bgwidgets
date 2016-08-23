#include "RadiusRect.h"

// our properties
ECOproperty RadiusRectProperties[] = {
	//  propid      resourceid,     datatype,       propflags                                   propFlags2, enumStart,  enumEnd
	anumForecolor,		0,		fftInteger, EXTD_FLAG_PROPAPP, 0, 0, 0,		// $forecolor
	anumBackcolor,		0,		fftInteger, EXTD_FLAG_PROPAPP, 0, 0, 0,		// $backcolor
	anumBorderColor,	0,		fftInteger, EXTD_FLAG_PROPAPP, 0, 0, 0,   // $bordercolor
	anumBackpattern,	0,		fftInteger, EXTD_FLAG_PROPAPP, 0, 0, 0,		// $backpattern
	rrectRadius,			4000,	fftInteger, EXTD_FLAG_PROPAPP, 0, 0, 0,		// $radius
	rrectLineWidth,   4001, fftInteger, EXTD_FLAG_PROPAPP, 0, 0, 0,		// $linewidth
};

radiusrect::radiusrect(HWND pHwnd, bool pUseHwnd) : bgwidget(pHwnd, pUseHwnd) {
	mRadius = 10;
	mForecolor = GDI_COLOR_QWHITE;
	mBackcolor = GDI_COLOR_QGRAY;
	mBorderWidth = 1;
	mBorderColor = GDI_COLOR_QBLACK;
	mBackpattern = 1;
};

// can we assign this property?
qbool radiusrect::propCanAssign(qlong pPropId) {
	switch (pPropId) {
		case rrectRadius: {
			return 1L;
		}; break;
		case rrectLineWidth: {
			return 1L;
		}; break;
		case anumForecolor: {
			return 1L;
		}; break;
		case anumBackcolor: {
			return 1L;
		}; break;
		case anumBorderColor: {
			return 1L;
		}; break;
		case anumBackpattern: {
			return 1L;
		}; break;
		default: {
			// huh?
			
		}; break;
	};

	return 0L;
};

// set a property value
qbool radiusrect::setProperty(qlong pPropId, EXTfldval * pSetVal) {
	switch (pPropId) {
		case rrectRadius: {
			mRadius = pSetVal->getLong();
			invalidate();
			return 1L;
		}; break;
		case rrectLineWidth: {
			mBorderWidth = pSetVal->getLong();
			invalidate();
			return 1L;
		}; break;
		case anumForecolor: {
			mForecolor = pSetVal->getLong();
			invalidate();
			return 1L;
		}; break;
		case anumBackcolor: {
			mBackcolor = pSetVal->getLong();
			invalidate();
			return 1L;
		}; break;
		case anumBorderColor: {
			mBorderColor = pSetVal->getLong();
			invalidate();
			return 1L;
		}; break;
		case anumBackpattern: {
			mBackpattern = pSetVal->getLong();
			invalidate();
			return 1L;
		}; break;
		default: {
			// huh?
			
		}; break;
	};

	return 0L;
};

// get a property value
qbool radiusrect::getProperty(qlong pPropId, EXTfldval * pRetVal) {
	switch (pPropId) {
		case rrectRadius: {
			pRetVal->setLong(mRadius);
			return 1L;
		}; break;
		case rrectLineWidth: {
			pRetVal->setLong(mBorderWidth);
			return 1L;
		}; break;
		case anumForecolor: {
			pRetVal->setLong(mForecolor);
			return 1L;
		}; break;
		case anumBackcolor: {
			pRetVal->setLong(mBackcolor);
			return 1L;
		}; break;
		case anumBorderColor: {
			pRetVal->setLong(mBorderColor);
			return 1L;
		}; break;
		case anumBackpattern: {
			pRetVal->setLong(mBackpattern);
			return 1L;
		}; break;
		default: {
			// huh?
		}; break;
	};

	return 0L;
};

// You need to paint your control
void radiusrect::paint(HDC hdc, qrect* pArea) {
	qcol		wasForeColor		= GDIgetTextColor(hdc);
	qcol		wasBackColor		= GDIgetBkColor(hdc);
	HBRUSH	patBrush				= GDIcreateBrush(mBackpattern);

	if (mBackpattern < 15) {
		GDIsetTextColor(hdc, mForecolor);
		GDIsetBkColor(hdc, mBackcolor);
		GDIfillRoundRect(hdc, pArea, mRadius, mRadius, patBrush);
	};

	if (mBorderWidth > 0) {
		HPEN	borderPen = GDIcreatePen(mBorderWidth, mBorderColor, patFill);
		HPEN	oldPen		= GDIselectObject(hdc, borderPen);

		GDIframeRoundRect(hdc, pArea, mRadius, mRadius);

		GDIselectObject(hdc, oldPen);
	}

	// cleanup
	GDIdeleteObject(patBrush);

	// leave it as it was...
	GDIsetTextColor(hdc, wasForeColor);
	GDIsetBkColor(hdc, wasBackColor);
};
