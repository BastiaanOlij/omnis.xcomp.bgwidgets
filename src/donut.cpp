#include "donut.h"
#include "math.h"

#ifndef pi
#define pi 3.141592653589793238462643
#endif

// our properties
ECOproperty DonutProperties[] = {
	//  propid      resourceid,     datatype,       propflags                                   propFlags2, enumStart,  enumEnd
	donutInnerRadius,	4010,	fftInteger, EXTD_FLAG_PROPAPP, 0, 0, 0,		// $innerradius
    donutBaseColor,     4011,   fftInteger, EXTD_FLAG_PROPAPP | EXTD_FLAG_PWINDCOL, 0, 0, 0,
    donutColorIndex,    4012,   fftInteger, EXTD_FLAG_PROPAPP | EXTD_FLAG_RUNTIMEONLY, 0, 0, 0,
    donutColorAngle,    4013,   fftInteger, EXTD_FLAG_PROPAPP | EXTD_FLAG_RUNTIMEONLY, 0, 0, 0,
    donutColor,         4014,   fftInteger, EXTD_FLAG_PROPAPP | EXTD_FLAG_RUNTIMEONLY, 0, 0, 0,
    donutOffsetAngle,   4015,   fftInteger, EXTD_FLAG_PROPAPP, 0, 0, 0,
};

donut::donut(HWND pHwnd, bool pUseHwnd) : bgwidget(pHwnd, pUseHwnd) {
	mInnerRadius = 5;
    mBaseColor = GDI_COLOR_QGREEN;
    mColorIndex = 0;
    mOffsetAngle = 0;
    for (int i = 0; i < DONUT_MAX_COLORS; i++) {
        mColorAngles[i] = 0;
        mColors[i] = GDI_COLOR_QDEFAULT;
    };
};

// can we assign this property?
qbool donut::propCanAssign(qlong pPropId) {
	switch (pPropId) {
		case donutInnerRadius: {
			return 1L;
		}; break;
        case donutBaseColor: {
            return 1L;
        }; break;
        case donutColorIndex: {
            return 1L;
        }; break;
        case donutColorAngle: {
            return 1L;
        }; break;
        case donutColor: {
            return 1L;
        }; break;
        case donutOffsetAngle: {
            return 1L;
        }; break;
		default: {
			// huh?
			
		}; break;
	};

	return 0L;
};

// set a property value
qbool donut::setProperty(qlong pPropId, EXTfldval * pSetVal) {
	switch (pPropId) {
		case donutInnerRadius: {
			mInnerRadius = pSetVal->getLong();
            if (mInnerRadius < 2) mInnerRadius = 2;
			invalidate();
			return 1L;
		}; break;
        case donutBaseColor: {
			mBaseColor = pSetVal->getLong();
			invalidate();
			return 1L;
        }; break;
        case donutColorIndex: {
			mColorIndex = pSetVal->getLong() - 1;
            if (mColorIndex < 0) {
                mColorIndex = 0;
            } else if (mColorIndex >= DONUT_MAX_COLORS) {
                mColorIndex = DONUT_MAX_COLORS - 1;
            };
            UpdateProperty(donutColorAngle);
            UpdateProperty(donutColor);
			return 1L;
        }; break;
        case donutColorAngle: {
			mColorAngles[mColorIndex] = pSetVal->getLong();
			invalidate();
			return 1L;
        }; break;
        case donutColor: {
			mColors[mColorIndex] = pSetVal->getLong();
			invalidate();
			return 1L;
        }; break;
        case donutOffsetAngle: {
            int offset = pSetVal->getLong();
            mOffsetAngle = offset;
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
qbool donut::getProperty(qlong pPropId, EXTfldval * pRetVal) {
	switch (pPropId) {
		case donutInnerRadius: {
			pRetVal->setLong(mInnerRadius);
			return 1L;
		}; break;
        case donutBaseColor: {
			pRetVal->setLong(mBaseColor);
			return 1L;
		}; break;
        case donutColorIndex: {
			pRetVal->setLong(mColorIndex + 1);
			return 1L;
        }; break;
        case donutColorAngle: {
			pRetVal->setLong(mColorAngles[mColorIndex]);
			return 1L;
        }; break;
        case donutColor: {
			pRetVal->setLong(mColors[mColorIndex]);
			return 1L;
        }; break;
        case donutOffsetAngle: {
            int offset = floor(mOffsetAngle);
            pRetVal->setLong(offset);
            return 1L;
        }; break;
		default: {
			// huh?
		}; break;
	};

	return 0L;
};

void donut::addInnerPoints(qpoint * pOuterPoints, const qpoint * pInnerPoints, int pNumPoints) {
    for (int i = 0; i < pNumPoints; i++) {
        pOuterPoints[pNumPoints + i] = pInnerPoints[pNumPoints - i - 1];
    };
};

// You need to paint your control
void donut::paint(HDC hdc, qrect* pArea) {
    qpoint      outerPoints[1024], innerPoints[512];
    qcol        wasColor;
    HBRUSH      brush = GDIcreateBrush(patFill);
    HBRUSH      wasBrush = GDIselectObject(hdc, brush);
    int         numPoints=0;
    qpoint      center, offset;
    int         maxRadius;
    int         colorIndex = 0;
    float       startAngle = 0;
    float       currAngle = 0;
    float       increaseAngle = 5.0;

    // remember our current color
    wasColor = GDIgetTextColor(hdc);

    // workaround for SDK bug with GDIfillpoly
    if (mUseHwnd) {
        offset.h = 0;
        offset.v = 0;
    } else {
        // omnis is supposed to adjust this but..
        offset.h = pArea->left;
        offset.v = pArea->top;
    };
    
    // find our center
    center.h = pArea->width() / 2;
    center.v = pArea->height() / 2;
    maxRadius = center.h < center.v ? center.v : center.h;
    
    // determine our angle increase by our radius
    increaseAngle = floor(360.0f / maxRadius);
    if (increaseAngle < 1.0) {
        increaseAngle = 1.0;
    };
    
    // start our circle
    outerPoints[0].h = offset.h + center.h + floor(sin(mOffsetAngle * pi / 180.0) * center.h);
    outerPoints[0].v = offset.v + center.v - floor(cos(mOffsetAngle * pi / 180.0) * center.v);
    innerPoints[0].h = offset.h + center.h + floor(sin(mOffsetAngle * pi / 180.0) * (center.h - mInnerRadius));
    innerPoints[0].v = offset.v + center.v - floor(cos(mOffsetAngle * pi / 180.0) * (center.v - mInnerRadius));
    numPoints++;
    
    while ((currAngle < 360.0) && (numPoints < 512)) {
        currAngle += increaseAngle;
        if (currAngle > 360.0) {
            // complete our donut
            currAngle = 360.0;
        };
        
        // check for break
        while (colorIndex < DONUT_MAX_COLORS && mColorAngles[colorIndex] < currAngle) {
            if (mColorAngles[colorIndex] != 0) {
                if ((numPoints > 1) && (mColors[colorIndex] != GDI_COLOR_QDEFAULT)) {
                    addInnerPoints(outerPoints, innerPoints, numPoints);
                    GDIsetTextColor(hdc, mColors[colorIndex]);
                    GDIfillPoly(hdc, outerPoints, numPoints + numPoints);
                };
                
                // start a new arc
                outerPoints[0] = outerPoints[numPoints - 1];
                innerPoints[0] = innerPoints[numPoints - 1];
                numPoints = 1;
            };
            
            colorIndex++;
        };
        
        // add our point
        outerPoints[numPoints].h = offset.h + center.h + floor(sin((mOffsetAngle + currAngle) * pi / 180.0) * center.h);
        outerPoints[numPoints].v = offset.v + center.v - floor(cos((mOffsetAngle + currAngle) * pi / 180.0) * center.v);
        innerPoints[numPoints].h = offset.h + center.h + floor(sin((mOffsetAngle + currAngle) * pi / 180.0) * (center.h - mInnerRadius));
        innerPoints[numPoints].v = offset.v + center.v - floor(cos((mOffsetAngle + currAngle) * pi / 180.0) * (center.v - mInnerRadius));
        numPoints++;
    };


    if ((numPoints > 1) && (mBaseColor != GDI_COLOR_QDEFAULT)) {
        addInnerPoints(outerPoints, innerPoints, numPoints);
        GDIsetTextColor(hdc, mBaseColor);
        GDIfillPoly(hdc, outerPoints, numPoints + numPoints);
    };

    // cleanup
    GDIsetTextColor(hdc, wasColor);
    GDIselectObject(hdc, wasBrush);
    GDIdeleteObject(brush);
};
