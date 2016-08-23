#include "bgwidgets.h"
#include "RadiusRect.h"

// Your generic object constructor
bgwidget::bgwidget(HWND pHwnd, bool pUseHwnd) {
	mHwnd = pHwnd;
	mUseHwnd = pUseHwnd;
};

// Generic destruction
bgwidget::~bgwidget() {
	// just a placeholder
};

void bgwidget::invalidate() {
	if (mUseHwnd) {
		WNDinvalidateRect(mHwnd, NULL);
	} else {
		ECOinvalBackObj();
	};
};


// can we assign this property?
qbool bgwidget::propCanAssign(qlong pPropId) {
	// just a placeholder
	return 0L;
};

// set a property value
qbool bgwidget::setProperty(qlong pPropId, EXTfldval * pSetVal) {
	// just a placeholder
	return 0L;
};

// get a property value
qbool bgwidget::getProperty(qlong pPropId, EXTfldval * pRetVal) {
	// just a placeholder

    return 0L;
};

// You need to paint your control
void bgwidget::paint(HDC hdc, qrect* pArea) {
	// just a placeholder
};

// Component library entry point (name as declared in resource 31000 )
extern "C" LRESULT OMNISWNDPROC bgwidgetsWndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam, EXTCompInfo* eci) {
	// Initialize callback tables - THIS MUST BE DONE
	ECOsetupCallbacks(hwnd, eci);
	switch (Msg) {
		// WM_ERASEBKGND - we don't erase the background for background objects
		case WM_ERASEBKGND: {
			return 1L;
		}; break;

		// ECM_BOBJ_EXERASE - is sent to our component when Omnis wants to know if our background objects background needs to be excluded from clearing
		case ECM_BOBJ_EXERASE: return 1L;

		// ECM_PRINT - background objects get this instead of the WM_PAINT as they are painted directly on the main window
		case ECM_PRINT: {
			// First find the object in the libraries chain of objects
			WNDpaintStruct* paintStruct = (WNDpaintStruct*)lParam;

			bgwidget* object = (bgwidget*)ECOfindObject(eci, hwnd);
			// and if its good, call the paint function
			if (object != NULL) {
				object->paint(paintStruct->hdc, &paintStruct->rcPaint);
				return qtrue;
			};
		}; break;

		// ECM_GETVERSION ask for version info
		case ECM_GETVERSION: {
			qshort	major = OMNISSDK;
			qshort	minor = 1;

			return ECOreturnVersion(major, minor);
		} break;


		// ECM_OBJCONSTRUCT - this is a message to create a new object.
		case ECM_OBJCONSTRUCT: {
			// Allocate a new object, for now always assume its radiusrect
			bgwidget* object;
			if (eci->mCompId == OBJECT_ID1) {
				object = new radiusrect(hwnd, !(wParam & ECM_WFLAG_NOHWND));
			};

			// and insert into a chain of objects. The OMNIS library will maintain this chain
			ECOinsertObject( eci, hwnd, (void*)object );
			return qtrue;
		}; break;
			
		// ECM_OBJDESTRUCT - this is a message to inform you to delete the object
		case ECM_OBJDESTRUCT: {
			// First find the object in the libraries chain of objects,
			// this call if ok also removes the object from the chain.
			bgwidget* object = (bgwidget*)ECOremoveObject( eci, hwnd );
			if ( NULL!=object ) {
				// Now you can delete the object you previous allocated
				// Note: The hwnd passed on ECM_OBJCONSTRUCT should not be deleted, as
				// it was created and will be destroyed by OMNIS
				delete object; // note, our destructor is declared virtual so this will call the correct destructor
			};
			return qtrue;
		}; break;
	 		
		// ECM_CONNECT - this message is sent once per OMNIS session and should not be confused
		// with ECM_OBJCONSTRUCT which is sent once per object. ECM_CONNECT can be used if load other libraries
		// once or other general global actions that need to be done only once.
		//
		// For most components this can be removed - see other BLYTH component examples
		case ECM_CONNECT: {
			return EXT_FLAG_LOADED | EXT_FLAG_BCOMPONENTS | EXT_FLAG_ALWAYS_USABLE | EXT_FLAG_REMAINLOADED; // Return external flags
		}; break;
      
		// ECM_DISCONNECT - this message is sent only once when the OMNIS session is ending and should not be confused
		// with ECM_OBJDESTRUCT which is sent once per object. ECM_DISCONNECT can be used to free other libraries
		// loaded on ECM_CONNECT or other general global actions that need to be done only once.
		//
		// For most components this can be removed - see other BLYTH component examples
		case ECM_DISCONNECT: {
			return qtrue;
		}; break;

		// ECM_GETCOMPLIBINFO - this is sent by OMNIS to find out the name of the library, and
		// the number of components this library supports
		case ECM_GETCOMPLIBINFO: {
			return ECOreturnCompInfo( gInstLib, eci, LIB_RES_NAME, OBJECT_COUNT );
		}; break;

		// ECM_GETCOMPICON - this is sent by OMNIS to get an icon for the OMNIS component store and
		// external component browser. You need to always supply an icon in your resource file.
		case ECM_GETCOMPICON: {
			// OMNIS will call you once per component for an icon.
			// GENERIC_ICON is defined in the header and included in the resource file
			if ( eci->mCompId==OBJECT_ID1 ) {
				return ECOreturnIcon(gInstLib, eci, RADIUSRECT_ICON);
			};
			return qfalse;
		}; break;

		// ECM_GETCOMPID - this message is sent by OMNIS to get information about each component in this library
		// wParam is a number from 1 to the number of components return on the ECM_GETCOMPLIBINFO message.
		//
		// For each call you should return the internal object ID and the type of object it is.
		// The object id will be used for other calls such as ECM_GETCOMPICON
		//
		// The object type is for example : cObjType_Basic 		- a basic window object or
		//																	cRepObjType_Basic	- a basic report object.
		// 																	There are others 	- see BLYTH examples and headers
		case ECM_GETCOMPID: {
			if (wParam == 1) {
				return ECOreturnCompID( gInstLib, eci, OBJECT_ID1, cObjType_Basic );
			};
			return 0L;
		}; break;
            
		////////////////////////////////////////////////////////////////////////
		// stuff for properties
        
		case ECM_GETPROPNAME: {
			return ECOreturnProperties(gInstLib, eci, RadiusRectProperties, 4 + rrectlast - 1); // add our 4 build in properties to our count
		}; break;
        
		case ECM_PROPERTYCANASSIGN: {
			bgwidget* object = (bgwidget*)ECOfindObject(eci, hwnd);
			if (object != NULL) {
				return object->propCanAssign(ECOgetId(eci));
			};
		}; break;

		case ECM_SETPROPERTY: {
			bgwidget* object = (bgwidget*)ECOfindObject(eci, hwnd);
			if (object != NULL) {
				EXTParamInfo* param = ECOfindParamNum( eci, 1 );
				if ( param ) {
					EXTfldval fval( (qfldval)param->mData );
					qbool result = object->setProperty(ECOgetId(eci), &fval);
					if (result == 1L) {
						return 1L;
					};
				};
			};
		}; break;

		case ECM_GETPROPERTY: {
			bgwidget* object = (bgwidget*)ECOfindObject(eci, hwnd);
			if (object != NULL) {
				EXTfldval fval;
				qbool result = object->getProperty(ECOgetId(eci), &fval);
				if (result == 1L) {
					ECOaddParam(eci,&fval);
					return 1L;
				};
			};
		}; break;

		case ECM_ISUNICODE:	{
			#ifdef isunicode
				return qtrue;
			#else
				return qfalse;
			#endif
		}; break;
	 };

	 // As a final result this must ALWAYS be called. It handles all other messages that this component
	 // decides to ignore.
	 return WNDdefWindowProc(hwnd,Msg,wParam,lParam,eci);
};
