#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 void createContentServer(char * addr, int p, char * file, int d )
 {
   ContentServer * cs;
   cs = malloc(sizeof(ContentServer));

   cs->Address = copystring(addr);
   cs->Port = p;
   cs->dirorfile = copystring(file);
   cs->delay = d;
   return cs;
 }

 void deleteContentServer(ContentServer * cs)
 {
   free(cs->Address);
   free(cs->dirorfile);
 }
