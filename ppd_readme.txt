Student id: s3496982
Name: Reyham Soenasto

-Added functions:
	-void coinToDenom(unsigned, char *);
	-void display_menu(struct menu_item*);
	-BOOLEAN abort_program(struct ppd_system *);
	-void verify_files(const char *, const char *);

-Added enum cash_value to represent denomination values.

-Program doesn't possess the "#14 Return to menu functionality".

-There is a bug when saving where price is stored as x.5 instead of x.05.
(A blackboard discussion says it can be overlooked)

-verify_files function is used to check if data files are valid, although because
I don't know what invalid data to expect - it may not work too well.



