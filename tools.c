#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"
char * copystring(char * string)
{
	char * s;
	s = malloc((strlen(string)+1)*sizeof(char));
	strcpy(s, string);
	return s;

}
