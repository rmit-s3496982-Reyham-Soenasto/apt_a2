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

#include "ppd_options.h"

/**
 * @file ppd_options.c this is where you need to implement the main 
 * options for your program. You may however have the actual work done
 * in functions defined elsewhere. 
 * @note if there is an error you should handle it within the function
 * and not simply return FALSE unless it is a fatal error for the 
 * task at hand. You want people to use your software, afterall, and
 * badly behaving software doesn't get used.
 **/

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this particular function should never fail.
 **/
BOOLEAN display_items(struct ppd_system * system)
{
	struct ppd_node *currentNode;
	char id[IDLEN + 1];
	char name[NAMELEN + 1];
	unsigned dollars, cents, available;
	printf("\nItems Menu\n----------\n");
	printf("%-8s|%-20s |%-10s |%s\n", "ID", "Name", "Available", "Price");
	printf("--------------------------------------------------\n");
	
	currentNode = system->item_list->head;
	if(system->item_list->head != NULL){
	while(currentNode != NULL)
	{
		strcpy(id,currentNode->data->id);
		strcpy(name,currentNode->data->name);
		dollars = currentNode->data->price.dollars;
		cents = currentNode->data->price.cents;
		available = currentNode->data->on_hand;
		
		printf("%-8s|%-20s |%-10d |$ %d.%02d\n", id, name, available, dollars, cents);
		
		currentNode = currentNode->next;
	};
	}
	else{
		printf("Stock system empty.");
	};
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a purchase succeeds and false when it does not
 **/
BOOLEAN purchase_item(struct ppd_system * system)
{
	struct ppd_node *currentNode, *selectedNode;
	char input[IDLEN+2];
	char money[PRICELEN+2];
	char *pos;
	BOOLEAN valid = FALSE;
	BOOLEAN available = TRUE;
	BOOLEAN validCash = FALSE;
	double remainingCost, displayedPrice;
	unsigned cost;
	unsigned insertedCash = 0;
	char *moneyInput;
	unsigned dueChange = 0; 
	double totalChange = 0;
	char *change = NULL;
	unsigned refund[MAX_REFUND] = {0,0,0,0,0,0,0,0,0,0};
	enum cash_value availableCash[] = {FIVE, TEN, TWENTY, FIFTY, HUNDRED, TWO_HUNDRED, FIVE_HUNDRED, THOUSAND};
	int i = 0;
	int j = 0;
	
	/*Request which item to purchase*/
    printf("\nPurchase Item: \n-------------\n");
	printf("Please enter the id of the item you wish to purchase: ");
	
	while(valid == FALSE || available == FALSE)
	{
		currentNode = system->item_list->head;
		fgets(input, IDLEN+2, stdin);
		if(strchr(input, '\n') == NULL){read_rest_of_line();};
		if ((pos = strchr(input, '\n')) != NULL){*pos = '\0';};
		while(currentNode != NULL)
		{
			if((strcmp(input, currentNode->data->id)) == 0){
				if(currentNode->data->on_hand == 0){
					available = FALSE;
					valid = TRUE;
				}
				else{
					selectedNode = currentNode;
					valid = TRUE;
					available = TRUE;
				};
			};
			currentNode = currentNode->next;
		};
		
		if(valid == FALSE)
		{
			if(available == TRUE)
			{
				printf("\nInvalid id, please re-enter id: ");
			}
			else
			{
				printf("\nOut of stock, please select another item: ");
			};
		};
	};
	/*Request payment*/
	printf("\nYou have selected '%s - %s'. \nThis will cost you $%d.%02d.", selectedNode->data->name, selectedNode->data->desc, 
	selectedNode->data->price.dollars, selectedNode->data->price.cents);
	printf("\nPlease hand over the money - type in the value of each note/coin in cents.\n");
	/*Convert item price into cents*/
	cost = (selectedNode->data->price.dollars * DOLLAR_CONVERSION) + selectedNode->data->price.cents;
	remainingCost = cost;
	while(insertedCash <= cost)
	{
		displayedPrice = remainingCost / DOLLAR_CONVERSION;
		printf("You still need to give us $%.2f: ", displayedPrice);
		fgets(money, PRICELEN+2, stdin);
		strtol(money, &moneyInput, 10);
		i = 0;
		while(i < NUM_DENOMS){
			if(availableCash[i] == strtol(money, &moneyInput, 10)){
				validCash = TRUE;
			};
			i++;
		};
		if(validCash == TRUE){
			insertedCash += strtol(money, &moneyInput, 10);
			remainingCost -= strtol(money, &moneyInput, 10);
		}
		else{
			printf("That money is not valid.\n");
		};
		if(insertedCash >= cost){
			break;
		};
		if(strchr(money, '\n') == NULL){read_rest_of_line();};
	};
	/*Return change*/
	if(insertedCash > cost)
	{
		dueChange = insertedCash - cost;
		
		while(dueChange >= availableCash[0])
		{
			i = NUM_DENOMS - 1;
			while(i >= 0)
			{
				j = 0;
				while(dueChange >= availableCash[i])
				{
					if(system->cash_register[i].count != 0)
					{
						while(refund[j] != 0){j++;};
						refund[j] = availableCash[i];
						dueChange -= availableCash[i];
						system->cash_register[i].count--;
					}
					else
					{
						i--;
					};
				};
				i--;
			};
		};
	};
	/*Check if system has enough change*/
	if(dueChange > 0){
		j = 0;
		while(refund[j] != 0){
			for(i = 0; i <= NUM_DENOMS - 1; i++){
				if(refund[j] == availableCash[i]){
					system->cash_register[i].count++;
				};
			};
			j++;
		};
		printf("System does not have enough change, sorry.\n");
	}
	else{
		j = 0;
		selectedNode->data->on_hand--;
		if(refund[j] != 0){
			while(refund[j] != 0){
				totalChange += refund[j];
				j++;
			};
			totalChange /= DOLLAR_CONVERSION;
			sprintf(money, "%.2f", totalChange);
			printf("Thank you. Here is your %s, and your change of $%s:\n", selectedNode->data->name, money);
			j = 0;
			while(refund[j] != 0){
				change = malloc(sizeof(change));
				coinToDenom(refund[j], change);
				printf("%s ", change);
				free(change);
				j++;
			};
		}
		else{
			printf("\nThank you. Here is your %s, and your change of $0.\n", selectedNode->data->name);
		};
	};
	
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a save succeeds and false when it does not
 **/
BOOLEAN save_system(struct ppd_system * system)
{
    struct ppd_node *currentNode;
	struct ppd_stock *currentStock;
	int i;
	enum cash_value centValue;
	FILE *stockFile;
	FILE *coinFile;
	/*Save stock file*/
	stockFile = fopen(system->stock_file_name, "w");
	
	currentNode = system->item_list->head;
	while(currentNode != NULL){
		currentStock = currentNode->data;
		fprintf(stockFile, "%s|%s|%s|%d.%d|%d\n", currentStock->id, currentStock->name, currentStock->desc, currentStock->price.dollars
		, currentStock->price.cents, currentStock->on_hand);
		currentNode = currentNode->next;
	}
	fclose(stockFile);
	/*Save coin file*/
	coinFile = fopen(system->coin_file_name, "w");
	for(i = 0; i < NUM_DENOMS; i++){
		switch(system->cash_register[i].denom){
			case FIVE_CENTS:
			centValue = FIVE;
			break;
			case TEN_CENTS:
			centValue = TEN;
			break;
			case TWENTY_CENTS:
			centValue = TWENTY;
			break;
			case FIFTY_CENTS:
			centValue = FIFTY;
			break;
			case ONE_DOLLAR:
			centValue = HUNDRED;
			break;
			case TWO_DOLLARS:
			centValue = TWO_HUNDRED;
			break;
			case FIVE_DOLLARS:
			centValue = FIVE_HUNDRED;
			break;
			case TEN_DOLLARS:
			centValue = THOUSAND;
			break;
			default:
			printf("\nInvalid value denomination.\n");
		};
		fprintf(coinFile, "%d,%d\n", centValue, system->cash_register[i].count);
	};
	fclose(coinFile);
	
	system_free(system);
	
	exit(EXIT_SUCCESS);
	
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when adding an item succeeds and false when it does not
 **/
BOOLEAN add_item(struct ppd_system * system)
{
    struct ppd_node *currentNode;
	struct ppd_node *newNode;
	struct ppd_stock *newStock;
	char id[IDLEN+1];
	char *idPrefix;
	char newId[IDLEN+1];
	char idAppendage[IDLEN];
	int idNum;
	char name[NAMELEN+1];
	char desc[DESCLEN+1];
	char price[PRICELEN+1];
	char *priceBuffer;
	unsigned dollars, cents;
	char *delim, *invalidDollars, *invalidCents;
	const char decimal[2] = ".";
	const char dot = '.';
	char *pos;
	BOOLEAN validPrice = FALSE;
	
	if((newNode = malloc(sizeof(*newNode))) == 0){
		printf("Unable to allocate memory for new node.");
	};
	if((newStock = malloc(sizeof(*newStock))) == 0){
		printf("Unable to allocate memory for new stock.");
	};
	
	/*If no items currently in list - otherwise else.*/
	if(system->item_list->head == NULL){
		system->item_list->head = newNode;
		system->item_list->head->data = newStock;
		printf("\nThis new meal item will have the Item id of I0001.\n");
		strcpy(newNode->data->id, "I0001");
		
		printf("Enter the item name: ");
		fgets(name, NAMELEN+1, stdin);
		if(strchr(name, '\n') == NULL){read_rest_of_line();};
		if ((pos = strchr(name, '\n')) != NULL){*pos = '\0';};
		strcpy(newNode->data->name, name);
		
		printf("Enter the item description: ");
		fgets(desc, DESCLEN+1, stdin);
		if(strchr(desc, '\n') == NULL){read_rest_of_line();};
		if ((pos = strchr(desc, '\n')) != NULL){*pos = '\0';};
		strcpy(newNode->data->desc, desc);
		
		newNode->data->on_hand = DEFAULT_STOCK_LEVEL;
		
		while(validPrice == FALSE){
			printf("Enter the price for this item: ");
			fgets(price, PRICELEN+1, stdin);
			if(strchr(price, '\n') == NULL){read_rest_of_line();};
			if ((pos = strchr(price, '\n')) != NULL){*pos = '\0';};
			if((delim = strchr(price, dot)) != NULL){
				priceBuffer = strtok(price, decimal);
				dollars = strtol(priceBuffer, &invalidDollars, 10);
				priceBuffer = strtok(NULL, decimal);
				cents = strtol(priceBuffer, &invalidCents, 10);
				if(invalidCents != priceBuffer){
					newNode->data->price.dollars = dollars;
					newNode->data->price.cents = cents;
					validPrice = TRUE;
				}
				else{
					printf("Invalid character format, please re-input in this format($[].[])\n");
				};
			}
			else{
				printf("There is no '.' decimal point, please re-input in this format($[].[])\n");
			};
		};
		
		system->item_list->head = newNode;
		currentNode = system->item_list->head;
	}
	else{
		currentNode = system->item_list->head;
		while(currentNode->next != NULL)
		{
			currentNode = currentNode->next;
		};
		
		strcpy(id, currentNode->data->id);
		
		currentNode->next = newNode;
		currentNode = currentNode->next;
		currentNode->data = newStock;
		
		pos = strchr(id, 'I');
		*pos = '0';
		idNum = strtol(id, &idPrefix, 10);
		idNum++;
		sprintf(idAppendage, "%0*d", ID_FORMAT_SPECIFIER, idNum);
		strcpy(newId, "I");
		strcat(newId, idAppendage);
		printf("This new meal item will have the Item id of %s.\n", newId);
		strcpy(currentNode->data->id, newId);
		
		printf("Enter the item name: ");
		fgets(name, NAMELEN+1, stdin);
		if(strchr(name, '\n') == NULL){read_rest_of_line();};
		if ((pos = strchr(name, '\n')) != NULL){*pos = '\0';};
		strcpy(currentNode->data->name, name);
		
		printf("Enter the item description: ");
		fgets(desc, DESCLEN+1, stdin);
		if(strchr(desc, '\n') == NULL){read_rest_of_line();};
		if ((pos = strchr(desc, '\n')) != NULL){*pos = '\0';};
		strcpy(currentNode->data->desc, desc);
		
		while(validPrice == FALSE){
			printf("Enter the price for this item: ");
			fgets(price, PRICELEN+1, stdin);
			if(strchr(price, '\n') == NULL){read_rest_of_line();};
			if ((pos = strchr(price, '\n')) != NULL){*pos = '\0';};
			if((delim = strchr(price, dot)) != NULL){
				priceBuffer = strtok(price, decimal);
				dollars = strtol(priceBuffer, &invalidDollars, 10);
				priceBuffer = strtok(NULL, decimal);
				cents = strtol(priceBuffer, &invalidCents, 10);
				if(invalidCents != priceBuffer){
					currentNode->data->price.dollars = dollars;
					currentNode->data->price.cents = cents;
					validPrice = TRUE;
				}
				else{
					printf("Invalid character format, please re-input in this format($[].[])\n");
				};
			}
			else{
				printf("There is no '.' decimal point, please re-input in this format($[].[])\n");
			};
		};
		
		currentNode->data->on_hand = DEFAULT_STOCK_LEVEL;
		
		currentNode->next = NULL;
	};
	
	printf("\nThis item '%s - %s' has no been added to the menu.\n", currentNode->data->name, currentNode->data->desc);
	
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when removing an item succeeds and false when it does not
 **/
BOOLEAN remove_item(struct ppd_system * system)
{
	char input[IDLEN+2];
	char *pos;
	struct ppd_node *currentNode = NULL; 
	struct ppd_node *selectedNode = NULL;
	struct ppd_node *previousNode = NULL;
	struct ppd_node *followingNode = NULL;
	BOOLEAN valid = FALSE;
	
    printf("Enter the id of the item to remove from the menu: ");
	
	while(valid == FALSE)
	{
		currentNode = system->item_list->head;
		fgets(input, IDLEN+2, stdin);
		if(strchr(input, '\n') == NULL){read_rest_of_line();};
		if ((pos = strchr(input, '\n')) != NULL){*pos = '\0';};
		while(currentNode != NULL)
		{
			if((strcmp(input, currentNode->data->id)) == 0){
				selectedNode = currentNode;
				valid = TRUE;
			};
			currentNode = currentNode->next;
		};
		if(valid == FALSE){
			printf("\nInvalid id, please re-enter id: ");
		};
	};
	/*Do this if selected is first in list*/
	if(selectedNode == system->item_list->head){
		system->item_list->head = selectedNode->next;
	}
	else{
		/*Do this if selected is last in list*/
		if(selectedNode->next == NULL){
			currentNode = system->item_list->head;
			while(currentNode->next != selectedNode){
				currentNode = currentNode->next;
			};
			currentNode->next = NULL;
		}
		/*Do this if selected is in middle of list*/
		else{
			previousNode = system->item_list->head;
			while(previousNode->next != selectedNode){
				previousNode = previousNode->next;
			};
			followingNode = selectedNode->next;
			previousNode->next = followingNode;
		};
	};
	
	printf("\n%s - %s - %s\n", selectedNode->data->id, selectedNode->data->name, selectedNode->data->desc);
	printf("has been removed from the system.\n");
	
	free(selectedNode->data);
	free(selectedNode);
	
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_stock(struct ppd_system * system)
{
	struct ppd_node *currentNode = NULL;
	
	currentNode = system->item_list->head;
	while(currentNode->next != NULL){
		currentNode->data->on_hand = DEFAULT_STOCK_LEVEL;
		currentNode = currentNode->next;
	};
	currentNode->data->on_hand = DEFAULT_STOCK_LEVEL;
	printf("\nAll stock has been reset to the default level of %d.\n", DEFAULT_STOCK_LEVEL);
	
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_coins(struct ppd_system * system)
{
	int i;
	
	for(i = 0; i < NUM_DENOMS; i++){
		system->cash_register[i].count = DEFAULT_COIN_COUNT;
	};
	
    printf("\nAll coins have been reset to the default level of %d.\n", DEFAULT_COIN_COUNT);
	
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system * system)
{
	int i;
	char display[DENOM_PRINT];
	
    printf("\nCoins Summary\n");
	printf("-------------\n");
	printf("%-12s | %s\n", "Denomination", "Count");
	printf("---------------------\n");
	for(i = 0; i < NUM_DENOMS; i++){
		switch(system->cash_register[i].denom){
			case FIVE_CENTS:
			strcpy(display, "5 cents");
			break;
			case TEN_CENTS:
			strcpy(display, "10 cents");
			break;
			case TWENTY_CENTS:
			strcpy(display, "20 cents");
			break;
			case FIFTY_CENTS:
			strcpy(display, "50 cents");
			break;
			case ONE_DOLLAR:
			strcpy(display, "1 dollar");
			break;
			case TWO_DOLLARS:
			strcpy(display, "2 dollar");
			break;
			case FIVE_DOLLARS:
			strcpy(display, "5 dollar");
			break;
			case TEN_DOLLARS:
			strcpy(display, "10 dollar");
			break;
			default:
			printf("\nInvalid value denomination.\n");
		};
		printf("%-12s | %d\n", display, system->cash_register[i].count);
	};
	
    return FALSE;
}

BOOLEAN abort_program(struct ppd_system * system)
{
	system_free(system);
	printf("\nProgram exited.\n\n");
	exit(EXIT_SUCCESS);
	
	return FALSE;
}