/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2015 Assignment #2
 * Full Name        : Reyham Soenasto
 * Student Number   : s3496982
 * Course Code      : COSC1076_1550
 * Program Code     : BP094
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include "ppd_coin.h"

 /**
  * @file ppd_coin.c implement functions here for managing coins in the
  * "cash register" contained in the @ref ppd_system struct.
  **/

 void coinToDenom(unsigned value, char * display){
	 switch(value){
		 case FIVE:
		 strcpy(display, "5c");
		 break;
		 case TEN:
		 strcpy(display, "10c");
		 break;
		 case TWENTY:
		 strcpy(display, "20c");
		 break;
		 case FIFTY:
		 strcpy(display, "50c");
		 break;
		 case HUNDRED:
		 strcpy(display, "$1");
		 break;
		 case TWO_HUNDRED:
		 strcpy(display, "$2");
		 break;
		 case FIVE_HUNDRED:
		 strcpy(display, "$5");
		 break;
		 case THOUSAND:
		 strcpy(display, "$10");
		 break;
		 default:
		 printf("\nInvalid value denomination.\n");
	 };
 }