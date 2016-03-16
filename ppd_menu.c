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

#include "ppd_menu.h"
#include "ppd_options.h"
/**
 * @file ppd_menu.c handles the initialised and management of the menu 
 * array
 **/

/**
 * @param menu the menu item array to initialise
 **/
void init_menu( struct menu_item* menu)
{
	int i = 0;
	struct menu_item displayitems;
	char displayItems[MENU_NAME_LEN] = "Display Items";
	struct menu_item purchaseitems;
	char purchaseItems[MENU_NAME_LEN] = "Purchase Items";
	struct menu_item saveandexit;
	char saveAndExit[MENU_NAME_LEN] = "Save and Exit";
	struct menu_item additem;
	char addItem[MENU_NAME_LEN] = "Add Item";
	struct menu_item removeitem;
	char removeItem[MENU_NAME_LEN] = "Remove Item";
	struct menu_item displaycoins;
	char displayCoins[MENU_NAME_LEN] = "Display Coins";
	struct menu_item resetstock;
	char resetStock[MENU_NAME_LEN] = "Reset Stock";
	struct menu_item resetcoins;
	char resetCoins[MENU_NAME_LEN] = "Reset Coins";
	struct menu_item abortprogram;
	char abortProgram[MENU_NAME_LEN] = "Abort Program";
	
	strcpy(displayitems.name, displayItems);
	strcpy(purchaseitems.name, purchaseItems);
	strcpy(saveandexit.name, saveAndExit);
	strcpy(additem.name, addItem);
	strcpy(removeitem.name, removeItem);
	strcpy(displaycoins.name, displayCoins);
	strcpy(resetstock.name, resetStock);
	strcpy(resetcoins.name, resetCoins);
	strcpy(abortprogram.name, abortProgram);
	
	displayitems.function = display_items;
	purchaseitems.function = purchase_item;
	saveandexit.function = save_system;
	additem.function = add_item;
	removeitem.function = remove_item;
	displaycoins.function = display_coins;
	resetstock.function = reset_stock;
	resetcoins.function = reset_coins;
	abortprogram.function = abort_program;
	
	menu[i] = displayitems;
	i++;
	menu[i] = purchaseitems;
	i++;
	menu[i] = saveandexit;
	i++;
	menu[i] = additem;
	i++;
	menu[i] = removeitem;
	i++;
	menu[i] = displaycoins;
	i++;
	menu[i] = resetstock;
	i++;
	menu[i] = resetcoins;
	i++;
	menu[i] = abortprogram;
}

void display_menu(struct menu_item* menu)
{
	int i = 0;
	int optionNum;
	printf("\nMain Menu: \n");
	while(strcmp(menu[i].name, "Abort Program") != 0)
	{
		optionNum = i + 1;
		printf("\t%d. %s\n", optionNum, menu[i].name);
		if(strcmp(menu[i].name, "Save and Exit") == 0)
		{
			printf("Administrator-Only Menu: \n");
		};
		i++;
	};
	printf("\t%d. %s\n", optionNum+1, menu[i].name);
}
