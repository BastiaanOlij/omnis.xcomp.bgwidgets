#include "polygon.h"

// our properties
ECOproperty PolygonProperties[] = {
    //  propid      resourceid,     datatype,       propflags                                   propFlags2, enumStart,  enumEnd
	anumForecolor,		0,			fftInteger,		EXTD_FLAG_PROPAPP, 0, 0, 0,        // $forecolor
	anumBackcolor,		0,			fftInteger,		EXTD_FLAG_PROPAPP, 0, 0, 0,        // $backcolor
	anumBorderColor,	0,			fftInteger,		EXTD_FLAG_PROPAPP, 0, 0, 0,   // $bordercolor
	anumBackpattern,    0,			fftInteger,		EXTD_FLAG_PROPAPP, 0, 0, 0,        // $backpattern
	polygonPoints,		4020,		fftCharacter,	EXTD_FLAG_PROPDATA + EXTD_FLAG_PWINDMLINE, 0, 0, 0,        // $points
	polygonLineWidth,	4001,		fftInteger,		EXTD_FLAG_PROPAPP, 0, 0, 0,        // $linewidth
};

polygon::polygon(HWND pHwnd, bool pUseHwnd) : bgwidget(pHwnd, pUseHwnd) {
    mForecolor = GDI_COLOR_QWHITE;
    mBackcolor = GDI_COLOR_QGRAY;
    mBorderColor = GDI_COLOR_QBLACK;
    mBorderWidth = 1;
    mBackpattern = 1;
    mNumPoints = 4;
    mPoints[0].h =  0; mPoints[0].v = 0;
    mPoints[1].h = -1; mPoints[1].v = 0;
    mPoints[2].h = -1; mPoints[2].v = -1;
    mPoints[3].h =  0; mPoints[3].v = -1;
};

// can we assign this property?
qbool polygon::propCanAssign(qlong pPropId) {
    switch (pPropId) {
        case polygonPoints: {
            return 1L;
        }; break;
        case polygonLineWidth: {
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
qbool polygon::setProperty(qlong pPropId, EXTfldval * pSetVal) {
    switch (pPropId) {
        case polygonPoints: {
			qchar	points[4096];
			int		len = 0, i = 0;
			qpoint	point;
			bool	isVert = false;
			bool	isNegative = false;
			bool	hasData = false;
			
			mNumPoints = 0;
			point.h = 0;
			point.v = 0;

			pSetVal->getChar(4096, points, len);
			
			while (i < len && mNumPoints < MAX_POINTS) {
				// note, even though we're dealing with qchar our first 128 characters have the same values
				if (points[i] == '\n' || points[i] == '\r') {
					if (hasData) {
						if (hasData && isNegative) {
							if (isVert) {
								point.v = point.v * -1;
							} else {
								point.h = point.h * -1;
							}
						}
						mPoints[mNumPoints++] = point;
						hasData = false; point.h = 0; point.v = 0; isVert = false; isNegative = false;
					}
				} else if (points[i] == ',') {
					if (isVert) {
						// just ignore
					} else {
						if (isNegative) {
							point.h = point.h * -1;
							isNegative = false;
						};
						isVert = true;
					}
				} else if (points[i] == '-') {
					isNegative = true;
				} else if (points[i] >= '0' && points[i] <= '9') {
					hasData = true;
					if (isVert) {
						point.v = (point.v * 10) + points[i] - '0';
					} else {
						point.h = (point.h * 10) + points[i] - '0';
					}
				}
				
				i++;
			}

			if (hasData && mNumPoints < MAX_POINTS) {
				if (isNegative) {
					if (isVert) {
						point.v = point.v * -1;
					} else {
						point.h = point.h * -1;
					}
				}
				mPoints[mNumPoints++] = point;
				hasData = false; point.h = 0; point.v = 0; isVert = false; isNegative = false;
			}
			
            invalidate();
            return 1L;
        }; break;
        case polygonLineWidth: {
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
qbool polygon::getProperty(qlong pPropId, EXTfldval * pRetVal) {
    switch (pPropId) {
        case polygonPoints: {
			char points[4096];
			qchar conv_points[4096];
			int len = 0;
			points[len] = 0;
			
			for (int i = 0; i < mNumPoints; i++) {
				if (i > 0) {
					points[len++] = '\r';
					points[len++] = '\n';
					points[len] = 0;
				};
				sprintf(&points[len], "%li,%li", mPoints[i].h,mPoints[i].v);
				len = strlen(points);
			};
			
			CHRunicode::utf8ToChar((qbyte *)points, len, conv_points, 4096);
			
			pRetVal->setChar(conv_points, len);
            return 1L;
        }; break;
        case polygonLineWidth: {
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
void polygon::paint(HDC hdc, qrect* pArea) {
    if (mNumPoints > 2) {
		qpoint      points[MAX_POINTS + 1];
        qcol		wasForeColor		= GDIgetTextColor(hdc);
        qcol		wasBackColor		= GDIgetBkColor(hdc);
        HBRUSH		patBrush			= GDIcreateBrush(mBackpattern);
	    HBRUSH      wasBrush			= GDIselectObject(hdc, patBrush);
		
		// workaround for SDK bug with GDIfillpoly
		if (mUseHwnd) {
			// left/top is at 0,0 not at pArea->left/top
			for (int i = 0; i < mNumPoints; i++) {
				points[i] = mPoints[i];
				if (points[i].h < 0) {
					points[i].h += pArea->right + 1 - pArea->left;
				};
				if (points[i].v < 0) {
					points[i].v += pArea->bottom + 1 - pArea->top;
				};
			};
		} else {
			// left/top is at pArea->left/top
			for (int i = 0; i < mNumPoints; i++) {
				points[i] = mPoints[i];
				if (points[i].h < 0) {
					points[i].h += pArea->right + 1;
				} else {
					points[i].h += pArea->left;
				};
				if (points[i].v < 0) {
					points[i].v += pArea->bottom + 1;
				} else {
					points[i].v += pArea->top;
				};
			};
		};
		
		// and close the loop
		points[mNumPoints] = points[0];
	
        if (mBackpattern < 15) {
            GDIsetTextColor(hdc, mForecolor);
            GDIsetBkColor(hdc, mBackcolor);
            GDIfillPoly(hdc, points, mNumPoints + 1);
        };
    
        if (mBorderWidth > 0) {
            HPEN    borderPen	= GDIcreatePen(mBorderWidth, mBorderColor, patFill);
            HPEN    oldPen	= GDIselectObject(hdc, borderPen);
        
            GDIframePoly(hdc, points, mNumPoints + 1);
			
			GDIselectObject(hdc, oldPen);
			GDIdeleteObject(borderPen);
		}
    
		// cleanup
		GDIselectObject(hdc, wasBrush);
		GDIdeleteObject(patBrush);
    
		// leave it as it was...
		GDIsetTextColor(hdc, wasForeColor);
		GDIsetBkColor(hdc, wasBackColor);
	};
};

