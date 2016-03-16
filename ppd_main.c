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

#include "ppd_main.h"
#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"

/**
 * @file ppd_main.c contains the main function implementation and any 
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/
int main(int argc, char **argv)
{
    /* validate command line arguments */

    /* represents the data structures to manage the system */
    struct ppd_system system;
	struct menu_item menu[MENU_OPTIONS];
	char input[MENU_INPUT_SIZE + 1];
	int inputNum = 0;
	char *alphaPtr = NULL;
	BOOLEAN inputValidation;
    
	if(argc != NUM_OF_INPUT_FILES)
	{
		printf("\nRequires 3 command line arguments(./[Program] [Stock file] [Coins file]).\n");
		exit(EXIT_SUCCESS);
	};
	verify_files(argv[2], argv[1]);
	/* init the system */
	system_init(&system);
    /* load data */
	load_data(&system, argv[2], argv[1]);
    /* test if everything has been initialised correctly */

    /* initialise the menu system */
	init_menu(menu);
	printf("\nSystem ready.\n");
    /* loop, asking for options from the menu */
	while(inputNum != ABORT_PROGRAM){
		inputValidation = TRUE;
		while(inputValidation == TRUE){
			display_menu(menu);
			printf("Select your option (1-9): ");
			fgets(input, MENU_INPUT_SIZE + 1, stdin);
			inputNum = strtol(input, &alphaPtr, 10);
			inputNum--;
			if(strchr(input, '\n') == NULL){
				read_rest_of_line();
				inputValidation = TRUE;
				printf("\nInvalid input, re-enter.\n");
			}
			else{
			inputValidation = FALSE;
			};
		};
		
		switch(inputNum){
			case DISPLAY_ITEMS:
			menu[DISPLAY_ITEMS].function(&system);
			break;
			case PURCHASE_ITEMS:
			menu[PURCHASE_ITEMS].function(&system);
			break;
			case SAVE_AND_EXIT:
			menu[SAVE_AND_EXIT].function(&system);
			break;
			case ADD_ITEM:
			menu[ADD_ITEM].function(&system);
			break;
			case REMOVE_ITEM:
			menu[REMOVE_ITEM].function(&system);
			break;
			case DISPLAY_COINS:
			menu[DISPLAY_COINS].function(&system);
			break;
			case RESET_STOCK:
			menu[RESET_STOCK].function(&system);
			break;
			case RESET_COINS:
			menu[RESET_COINS].function(&system);
			break;
			case ABORT_PROGRAM:
			menu[ABORT_PROGRAM].function(&system);
			break;
			default:
			printf("Input out of range, please re-enter(1-9): \n");
		};
    };

    /* make sure you always free all memory and close all files 
     * before you exit the program
     */
    return EXIT_SUCCESS;
}
