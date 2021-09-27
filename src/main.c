#include "list.h"

int main(int argc, char const *argv[])
{
	List *list_static = lscreate(sizeof(int));
	int a = 1;
	int b = 32;
	int c = 40;
	int n = 23;
	lspush(list_static, &a);
	lspush(list_static, &b);
	lspush(list_static, &c);
	lsinsert(list_static, 1, &n);
	lsprint(list_static, LSPRINT_INT);

	List *list_dynamic = lscreate(LSMODE_DYNAMIC);
	lsdpush(list_dynamic, "test1", 5);
	lsdpush(list_dynamic, "test2", 5);
	lsdpush(list_dynamic, "test3", 5);
	lsdinsert(list_dynamic, 4, "testaloalo", 10);
	lsprint(list_dynamic, LSPRINT_STR);

	lsfree(list_static);
	lsfree(list_dynamic);

	return 0;
}
