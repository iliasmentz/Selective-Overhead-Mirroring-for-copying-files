#ifndef __TOOLS_HEADER__
#define __TOOLS_HEADER__

char * copystring(char * string);
void perror_exit(char *message);

int read_data (int fd, char **buffer);
int write_data ( int fd, char* message );
int send_data ( int fd, char* message, int length );
char * CreateFolder(char * foldername);
int CreateFile(char * path);
char* replace_char(char* str, char find, char replace);
struct hostent* find_hostent(char* host);

#endif
