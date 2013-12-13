//
//  testException.c
//  CObjects
//
//  Created by George Boumis on 13/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <cobj.h>
#include <setjmp.h>

int main () {
	COTRY {
		COTRY {
			puts("ola kala 1");
			COTHROW(6);
		}
		COCATCH(5) {
			puts("handled exception nested");
		}
		COOTHER {
			puts("default nested");
		}
		COFINALLY {
			puts("finally nested");
		}
		COEND
	}
	COCATCH(5) {
		puts("handled exception");
		COHANDLE();
	}
	COOTHER {
		puts("default");
	}
	COFINALLY {
		puts("finally");
	}
	COEND
	
	//		struct exception_context_list_item_t econtext;
	//		COExceptionLink(&econtext);
	//		int res = setjmp(econtext.context);
	//		switch(res) {
	//			case COCODE: {
	//				{
	//					struct exception_context_list_item_t econtext2;
	//					COExceptionLink(&econtext2);
	//					int res2 = setjmp(econtext2.context);
	//					switch(res2) {
	//						case COCODE: {
	//							puts("ola kala 1");
	//							CORaise(5);
	//						}
	//							break;
	//						case 5:
	//							puts("handled exception nested");
	//
	//							break;
	//						default:
	//							puts("default nested");
	//							break;
	//						case COFINALLY:
	//							puts("finally nested");
	//					}
	//					COExceptionUnlink(&econtext2);
	//				}
	//			}
	//			case 5:
	//				puts("handled exception");
	//				COHandle(&econtext);
	//				break;
	//			default:
	//				puts("default");
	//				break;
	//			case COFINALLY:
	//				puts("finally");
	//		}
	//		COExceptionUnlink(&econtext);
	//	}
	return 0;
}
