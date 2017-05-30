#ifndef __TOOLS_HEADER__
#define __TOOLS_HEADER__

char * copystring(char * string);
void perror_exit(char *message);

int read_data (int fd, char **buffer);
int write_data ( int fd, char* message );
char * CreateFolder(char * foldername);


#endif
