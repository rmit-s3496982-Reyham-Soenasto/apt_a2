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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * @file ppd_coin.h defines the coin structure for managing currency in the
 * system. You should declare function prototypes for managing coins here
 * and implement them in ppd_coin.c
 **/
#ifndef PPD_COIN
#define PPD_COIN
#define COIN_DELIM ","
#define DISPLAY_LENGTH 10
struct ppd_system;
/**
 * enumeration representing the various types of currency available in
 * the system. 
 **/
enum denomination
{
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR, 
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};

enum cash_value
{
	FIVE = 5, TEN = 10, TWENTY = 20, FIFTY = 50, HUNDRED = 100, 
    TWO_HUNDRED = 200, FIVE_HUNDRED = 500, THOUSAND = 1000
};

/**
 * represents a coin type stored in the cash register. Each demonination
 * will have exactly one of these in the cash register.
 **/
struct coin
{
    /**
     * the denomination type
     **/
    enum denomination denom;
    /**
     * the count of how many of these are in the cash register
     **/
    unsigned count;
};

void coinToDenom(unsigned, char *);
#endif
