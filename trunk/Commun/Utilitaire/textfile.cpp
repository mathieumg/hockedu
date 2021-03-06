// textfile.cpp
//
// simple reading and writing for text files
//
// www.lighthouse3d.com
//
// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given
//////////////////////////////////////////////////////////////////////

#include "textfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *textFileRead( const char *fn )
{
   char *content = NULL;

   if ( fn != NULL )
   {
      FILE *fp;// = fopen(fn,"r");
	  fopen_s(&fp, fn, "r");
      if ( fp != NULL )
      {
         fseek( fp, 0, SEEK_END );
         int count = ftell(fp);
         rewind(fp);

         if ( count > 0 )
         {
            content = (char *) malloc(sizeof(char) * (count+1) );
            count = (int)fread( content, sizeof(char), count, fp );
            content[count] = '\0';
         }
         fclose(fp);
      }
   }
   return content;
}

int textFileWrite( const char *fn, const char *s )
{
   int status = 0;
   if ( fn != NULL )
   {
      FILE *fp;// = fopen(fn,"w");
	  fopen_s(&fp, fn, "w");
      if ( fp != NULL )
      {
         if ( fwrite( s, sizeof(char), strlen(s), fp ) == strlen(s) )
            status = 1;
         fclose(fp);
      }
   }
   return status;
}
