#include "parser.h"
#include <assert.h>
int main(int argc, char** argv)
{
	FILE* fp = NULL;
	if (argc != 2)
		return printf("Input Arguement Error\n");
	fp = fopen(argv[1], "r");
	parse(fp);
	return 0;
}
