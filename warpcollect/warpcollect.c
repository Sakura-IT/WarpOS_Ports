#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

const char amigaversion[] = "$VER: Warpcollect 0.4 (04.05.2021)";

char *progname;

#if defined (_LINUX_)

#else
/* From libnix */
static char *amigapath(const char *path)
{ static char *s1=NULL;
  char *s2,*s3;
  int c;

  if(s1!=NULL)
    free(s1);

  s1=strdup(path);
  if(s1==NULL)
  { errno=ENOMEM;
    return NULL; }
  
  s3=s2=s1; /* Replace multiple following '/' by single ones */
  do
  { if(*s2=='/')
      while(s2[1]=='/')
        s2++;
    *s3++=*s2;
  }while(*s2++);

  s3=s2=s1; /* Remove single dots '.' as directory names */
  c=1;
  do
  { while(c&&s2[0]=='.'&&(s2[1]=='/'||s2[1]=='\0'))
    { s2++;
      if(*s2=='/')
        s2++;
    }
    *s3++=*s2;
    c=0;
    if(*s2=='/')
      c=1;
  }while(*s2++);
  
  s3=s2=s1; /* Remove double dots '..' as directory names */
  c=1;
  do
  { if(c&&s2[0]=='.'&&s2[1]=='.')
    { if(s2[2]=='/')
        s2+=2; 
      else if(s2[2]=='\0')
      { *s3++='/';
        s2+=2; }
    }
    *s3++=*s2;
    c=0;
    if(*s2=='/')
      c=1;
  }while(*s2++);

  if(*s1=='/') /* Convert names beginning with '/' */
  { s3=s2=s1;
    s2++;
    if(*s2=='/'||*s2=='\0') /* The root directory */
      return "SYS:";
    while(*s2!='/'&&*s2!='\0')
      *s3++=*s2++;
    *s3++=':';
    if(*s2=='/')
      s2++;
    do
      *s3++=*s2;
    while(*s2++);
  }

  return s1;
}

static void copyfile(char *infile, char *outfile)
{
	FILE *in, *out;
	size_t len;
	int done = 0;
	char *buffer;

	in = fopen(infile, "rb");
	out = fopen(outfile, "wb");
	buffer = malloc(8192);

	if (buffer == NULL) {
		fprintf(stderr, "%s: Out of memory\n", progname);
		exit(20);
	}

	if (in == NULL || out == NULL) {
		fprintf(stderr, "%s: Warning, Can't copy ELF file\n", progname);
		return;
	}

	while(done == 0) {
		len = fread(buffer, 1, 8192, in);
		if (len !=0)
			fwrite(buffer, 1, len, out);
		else
			done = 1;
	}

	free(buffer);
	fclose(out);
	fclose(in);
}
#endif
int main(int argc, char **argv)
{
	int i, len = 0;
	char *temp, *outname;
	char *exec, *exec2, *exec3;
	char *env;
#if defined (_LINUX_)
    char *exec4;
#endif
    
	progname = argv[0];

	for(i = 1; i < argc; i++) {
		len = len + 1 + strlen(argv[i]);
	}

	exec = malloc(len + 256);
	exec2 = malloc(len + 256);
	exec3 = malloc(len + 256);
#if defined (_LINUX_)
    exec4 = malloc(len + 256);
#endif
    temp = malloc(len + 256);


	if (temp == NULL || exec == NULL || exec2 == NULL || exec3 == NULL) {
		fprintf(stderr, "%s: Out of memory\n", progname);
		return 20;
	}

	strcpy(temp, "a.out");
#if defined (_LINUX_)    
	strcpy(exec, "/gg/bin/ppc-morphos-ld");
#else
    strcpy(exec, "/gg/bin/ld");
#endif
    
	/* Get the output file name and copy the commandline */
	for(i = 1; i < argc; i++) {
		sprintf(exec, "%s %s", exec, argv[i]);
		if (strcmp(argv[i], "-o") == 0 && argc-1 > 1)
			strcpy(temp, argv[i+1]);
	}

	/* elf2exe2 doesn't support unixpaths :( */
#if defined (_LINUX_)
    outname = strdup(temp);
#else
	outname = amigapath(temp);
	if (outname == NULL) {
		fprintf(stderr, "%s: Out of memory\n", progname);
		return 20;
	}
#endif
	sprintf(exec2, "elf2exe2 %s %s", outname, outname);
	sprintf(temp, "%s.warpelf", outname);
#if defined (_LINUX_)
	sprintf(exec3, "ppc-morphos-strip -R .gnu.attributes %s", outname);
    sprintf(exec4, "cp %s %s", outname, temp);
#else
    sprintf(exec3, "strip -R .gnu.attributes %s", outname);
#endif
	i = system(exec); /* Execute the linker */
	if (i != 0)
		return errno;
	
    env = getenv("KEEPWARPELF");
	if (env != NULL && atoi(env) > 0)
#if defined (_LINUX_)
        i = system(exec4);
        if (i != 0) {
            i = errno;
            return i;
        }
#else
		copyfile(outname, temp);
#endif    
	i = system(exec3); /* Execute the strip */
	if (i != 0)
		return errno;

#if defined (_LINUX_)
    
#else
	i = system(exec2); /* Execute elf2exe2 */
	if (i != 0) {
		i = errno;
		remove(outname);
		remove(temp);
		return i;
	}
#endif
    free(exec);
    free(exec2);
    free(exec3);
#if defined (_LINUX_)
    free(exec4);
#endif
    free(temp);
	return 0;
}
