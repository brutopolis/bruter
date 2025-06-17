#include <bruter.h>

BruterValue teste(BruterList* list)
{
	printf("Hello, World!\n");
	return bruter_value_i(42);
}


int main()
{
	BruterList* list = bruter_init(8, false, false);
	bruter_push(list, bruter_value_p(teste), "teste", 0);
	BruterValue result = bruter_run(list);
	printf("Result: %d\n", result.i);
	bruter_free(list);
	return 0;
}
