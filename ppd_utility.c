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

#include "ppd_utility.h"
/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function, 
 * here is probably a good spot.
 **/

void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!='\n' && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}

/**
 * @param system a pointer to a @ref ppd_system struct that contains
 * all the data in the system we are manipulating
 * @param coins_name the name of the coins file
 * @param stock name the name of the stock file
 **/
BOOLEAN load_data(struct ppd_system * system , const char * coins_name, 
const char * stock_name)
{
    FILE *stockFile;
	FILE *coinFile;
	 
	 system->stock_file_name = stock_name;
	 system->coin_file_name = coins_name;
	 
	 stockFile = fopen(stock_name,"r");
	 
	 if(stockFile != NULL)
	 {
		 struct ppd_node *currentNode, *newNode;
		 struct ppd_stock *newStock;
		 char *stockToken;
		 unsigned value;
		 char stockDelim[2] = "|";
		 char priceDelim[2] = ".";
		 char stockLine[STOCK_LINE_SIZE];
		 char *ptr;
		 
		 while(fgets(stockLine, sizeof stockLine, stockFile) != NULL)
		 {
			 if((newNode = malloc(sizeof(*newNode))) == 0){
				 printf("Unable to allocate memory for new node.");
			 };
			 
			 if((newStock = malloc(sizeof(*newStock))) == 0){
				 printf("Unable to allocate memory for new stock.");
			 };
			 
			 if(system->item_list->head == NULL){
				 system->item_list->head = newNode;
				 system->item_list->head->data = newStock;
				 system->item_list->head->next = NULL;
				 currentNode = system->item_list->head;
			 }
			 else{
				 currentNode->next = newNode;
				 currentNode = newNode;
				 currentNode->data = newStock;
			 };
			 
			 stockToken = strtok(stockLine, stockDelim);
			 strcpy(currentNode->data->id, stockToken);
			 
			 stockToken = strtok(NULL, stockDelim);
			 strcpy(currentNode->data->name, stockToken);
			 
			 stockToken = strtok(NULL, stockDelim);
			 strcpy(currentNode->data->desc, stockToken);
			 
			 stockToken = strtok(NULL,priceDelim);
			 value = strtol(stockToken, &ptr, 10);
			 currentNode->data->price.dollars = value;
			 
			 stockToken = strtok(NULL,stockDelim);
			 value = strtol(stockToken, &ptr, 10);
			 currentNode->data->price.cents = value;
			 
			 stockToken = strtok(NULL, stockDelim);
			 currentNode->data->on_hand = strtol(stockToken,&ptr,10);
			 
			 currentNode->next = NULL;
		 }
		 fclose(stockFile);
	 }
	 else
	 {
		 printf("Stock file unavailable.");
	 }
	 
	 coinFile = fopen(coins_name,"r");
	 if(coinFile != NULL)
	 {
		 char coinLine[COIN_LINE_SIZE];
		 char *coinToken;
		 char coinDelim[2] = ",";
		 enum denomination value;
		 int coinBuffer;
		 char *ptr;
		 while(fgets(coinLine, sizeof coinLine, coinFile) != NULL){
			 coinToken = strtok(coinLine, coinDelim);
			 coinBuffer = strtol(coinToken, &ptr, 10);
			 switch(coinBuffer){
				 case FIVE:
				 value = FIVE_CENTS;
				 break;
				 case TEN:
				 value = TEN_CENTS;
				 break;
				 case TWENTY:
				 value = TWENTY_CENTS;
				 break;
				 case FIFTY:
				 value = FIFTY_CENTS;
				 break;
				 case HUNDRED:
				 value = ONE_DOLLAR;
				 break;
				 case TWO_HUNDRED:
				 value = TWO_DOLLARS;
				 break;
				 case FIVE_HUNDRED:
				 value = FIVE_DOLLARS;
				 break;
				 case THOUSAND:
				 value = TEN_DOLLARS;
				 break;
				 default:
				 printf("\nInvalid denomination.\n");
			 };
			 
			 system->cash_register[value].denom = value;
			 coinToken = strtok(NULL, coinDelim);
			 coinBuffer = strtol(coinToken, &ptr, 10);
			 system->cash_register[value].count = coinBuffer;
		 };
		 fclose(coinFile);
	 }
	 else
	 {
		 printf("Coin file unavailable.");
	 };
	
    return FALSE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system)
{
    int i;
	unsigned init = 0;
	struct ppd_list *newList;
	for(i = 0; i < NUM_DENOMS; i++){
		system->cash_register[i].denom = 0;
		system->cash_register[i].count = 0;
	};
	
	if((newList = malloc(sizeof(*newList))) == 0){
		 printf("Unable to allocate memory for new list.");
	};
	system->item_list = newList;
	system->item_list->head = NULL;
	system->item_list->count = init;
	system->coin_file_name = NULL;
	system->stock_file_name = NULL;
	
    return FALSE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{
	struct ppd_node *currentNode = NULL;
	struct ppd_node *previousNode = NULL;
	
	currentNode = system->item_list->head;
	while(currentNode->next != NULL){
		free(currentNode->data);
		previousNode = currentNode;
		currentNode = currentNode->next;
		free(previousNode);
	};
	free(currentNode->data);
	free(currentNode);
	free(system->item_list);
}

void verify_files(const char * coins_name, const char * stock_name){
	FILE *stockFile;
	FILE *coinFile;
	char *stockToken;
	char *coinToken;
	char stockDelim[2] = "|";
	char priceDelim[2] = ".";
	char coinDelim[2] = ",";
	char stockLine[STOCK_LINE_SIZE];
	char coinLine[COIN_LINE_SIZE];
	unsigned value;
	char *ptr;
	int coinLineCount;
	
	stockFile = fopen(stock_name,"r");
	
	while(fgets(stockLine, sizeof stockLine, stockFile) != NULL){
		stockToken = strtok(stockLine, stockDelim);
		if(strchr(stockToken, 'I') == NULL || strlen(stockToken) != IDLEN){
			printf("\nStock file corrupted, exiting program.\n");
			exit(EXIT_SUCCESS);
		};
		stockToken = strtok(NULL, stockDelim);
		if(strlen(stockToken) == 0 || strlen(stockToken) > NAMELEN){
			printf("\nStock file corrupted, exiting program.\n");
			exit(EXIT_SUCCESS);
		};
		stockToken = strtok(NULL, stockDelim);
		if(strlen(stockToken) == 0 || strlen(stockToken) > DESCLEN){
			printf("\nStock file corrupted, exiting program.\n");
			exit(EXIT_SUCCESS);
		};
		stockToken = strtok(NULL,priceDelim);
		value = strtol(stockToken, &ptr, 10);
		if(stockToken == ptr){
			printf("\nStock file corrupted, exiting program.\n");
			exit(EXIT_SUCCESS);
		};
		stockToken = strtok(NULL,stockDelim);
		value = strtol(stockToken, &ptr, 10);
		if(stockToken == ptr){
			printf("\nStock file corrupted, exiting program.\n");
			exit(EXIT_SUCCESS);
		};
		stockToken = strtok(NULL, stockDelim);
		value = strtol(stockToken,&ptr,10);
		if(stockToken == ptr){
			printf("\nStock file corrupted, exiting program.\n");
			exit(EXIT_SUCCESS);
		};
	};
	
	fclose(stockFile);
	
	coinFile = fopen(coins_name, "r");
	coinLineCount = 0;
	
	while(fgets(coinLine, sizeof coinLine, coinFile) != NULL){
		coinToken = strtok(coinLine, coinDelim);
		value = strtol(coinToken, &ptr, 10);
			switch(value){
				case FIVE:
				value = FIVE_CENTS;
				coinLineCount++;
				break;
				case TEN:
				value = TEN_CENTS;
				coinLineCount++;
				break;
				case TWENTY:
				value = TWENTY_CENTS;
				coinLineCount++;
				break;
				case FIFTY:
				value = FIFTY_CENTS;
				coinLineCount++;
				break;
				case HUNDRED:
				value = ONE_DOLLAR;
				coinLineCount++;
				break;
				case TWO_HUNDRED:
				value = TWO_DOLLARS;
				coinLineCount++;
				break;
				case FIVE_HUNDRED:
				value = FIVE_DOLLARS;
				coinLineCount++;
				break;
				case THOUSAND:
				value = TEN_DOLLARS;
				coinLineCount++;
				break;
				default:
				printf("\nCoin file corrupted, exiting program.\n");
				exit(EXIT_SUCCESS);
			};
			coinToken = strtok(NULL, coinDelim);
			value = strtol(coinToken, &ptr, 10);
			if(coinToken == ptr){
				printf("\nCoin file corrupted, exiting program.\n");
				exit(EXIT_SUCCESS);
			};
			
	};
	if(coinLineCount != NUM_DENOMS){
		printf("\nCoin file corrupted, exiting program.\n");
		exit(EXIT_SUCCESS);
	};
	fclose(coinFile);
}
