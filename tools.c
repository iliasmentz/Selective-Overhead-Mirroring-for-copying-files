#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tools.h"

char * copystring(char * string)
{
	char * s;
	s = malloc((strlen(string)+1)*sizeof(char));
	strcpy(s, string);
	return s;

}

int read_data (int fd, char ** string){/* Read formated data */
	char temp[32];
	int i = 0, length = 0;
	if ( read ( fd, temp, 32 ) < 0 )	/* Get length of string */
		exit (-3);
	length = atoi(temp);
	char * buffer;
	*string= malloc(length*sizeof(length));
	buffer = *string ;
	while ( i < length )	/* Read $length chars */
		if ( i < ( i+= read (fd, &buffer[i], length - i)))
			exit (-3);
	return i;	/* Return size of string */
}

int write_data ( int fd, char* message ){/* Write formated data */
	char temp[32]; int length = 0;
	memset(temp, '\0', 32);
	length = strlen(message) + 1;	/* Find length of string */
	sprintf(temp, "%d", length);

	if( write (fd, temp, 32) < 0 )	/* Send length first */
		exit (-2);
	if( write (fd, message, length) < 0 )	/* Send string */
		exit (-2);
	return length;		/* Return size of string */
}

void perror_exit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
