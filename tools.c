#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#include "tools.h"

char * copystring(char * string)
{
	char * s;
	s = malloc((strlen(string)+1)*sizeof(char));
	strcpy(s, string);
	return s;
}

void removeSubstring(char *s,const char *toremove)
{
  while( s=strstr(s,toremove) )
    memmove(s,s+strlen(toremove),1+strlen(s+strlen(toremove)));
}

struct hostent* find_hostent(char* host)
{
    int ip=1;
    int i;
		/*check if ip was given or host name*/
    for (i = 0; i < strlen(host); i++)
		{
			/*are the input valid for ip? */
        if(!(host[i]=='.') && !(host[i]>='0' && host[i]<='9') )
				{
					ip=0;
					break;
        }
    }
    if(ip==0){
        return gethostbyname(host);
    }else{
        struct sockaddr_in sa;
        inet_aton(host,(struct in_addr*)&sa);
        return gethostbyaddr(&sa, sizeof(sa), AF_INET);
    }
}

int read_data (int fd, char ** string){/* Read data */
	char temp[32];
	int i = 0, length = 0;
	int x;
	if ( (x =read ( fd, temp, 32 )) < 0 )	/* Get length of string */
		exit (-3);
	else if(x==0)
		return 0;
	length = atoi(temp);
	//printf("Length: %s %d\n", temp, length );
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

int send_data ( int fd, char* message, int length )
{/*this function can send bytes too*/
	char temp[32];
	memset(temp, '\0', 32);
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

char * CreateFolder(char * foldername)
{
	/*takes a path and creates all the folders that should exist*/
  struct tm *info;
	char * path;
	path = copystring(foldername);
	char * folder;
	char * previous;
	previous = malloc((strlen(foldername)+1)*sizeof(char));
	folder = malloc((strlen(foldername)+1)*sizeof(char));
	struct stat st = {0};
	char * local;
	local = strtok_r(path, "/", &path);
	if (local ==NULL)
	{
		free(path);
		free(folder);
		free(previous);
		/*check if the folder already exists*/
		if (stat(foldername, &st) == -1) {
				/*create the folder*/
				mkdir(foldername, 0777);
				printf("created %s\n", foldername );
		}
		return foldername;
	}
  if (stat(local, &st) == -1) {
      /*create the folder*/
      mkdir(local, 0777);
  }
  strcpy(folder, local);
	while((local = strtok_r(path, "/", &path))!=NULL)
	{
		strcpy(previous, folder);
		sprintf(folder, "%s/%s", previous, local);
		if (stat(folder, &st) == -1) {
				/*create the folder*/
				mkdir(folder, 0777);
		}
	}
	free(folder);
	free(previous);

	return foldername;
}

int  CreateFile(char * fullpath)
{/*create and open the output file and return the file descriptor*/

		int fd;
		char *last = strrchr(fullpath, '/');
		char * path = copystring(fullpath);
		removeSubstring(path, last);
		CreateFolder(path);
		if((fd = open(fullpath, O_APPEND|O_WRONLY|O_CREAT|O_TRUNC, 0777))==-1)
		{
			printf("%s\n", fullpath);
			perror_exit("open");
		}
		free(path);
		return fd;
}
