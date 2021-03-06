/* strings -  like unix (berkeley) utility

 this program may be used to find printable string sequences in any file
 for this program, a string is anything that has at least four printable
 characters in it (this limit can be changed with the I switch)
 for example:

   strings msdos.sys
     will print strings contained in the file "msdos.sys" (thrilling!)

   strings -o -I8 data
     will print all strings of at least eight characters in the
     file "data", each string preceeded by a number representing
     its position in the file

 this program can be piped to if need be

 caveats:
  strings idea of printable characters does not include tabs, or any
  byte with the high bit set.  some programs, particularly those in the
  land of CPM, set the high bit to indicate an end of a string

 comments about this can reach me via BIX (jmckim)
 or telephone (216 835 3845)
 or USPS (Jim McKim, 30224 Wolf Rd., Bay Village, OH 44140)

 (i gladly accept bug reports; bug fixes will make me ecstatic)

 the compiled version of this came out of the aztec c compiler
*/

/* #define UNIX */
#define MSDOS

#include <stdio.h>

#define IsPrintable(c) ((c)>=' ' && (c)<='~')

FILE *infile;

char *pname;
char *iap;
int max_index = 4;
int index=0;
int printed=0;
int iarg = 1;
int o_flag=0, end_flag=0;
long int count=0, position;
char *noline="\0";

char *malloc();
FILE *fopen();

void TellHow()
{
    fprintf(stderr,"find strings in a binary file - by jim mckim 6/1/1986\n"\
    "usage: strings [-flags] <input >output\n"\
    "where <flags> are:\n"\
    "o\tpreceed each line with offset into file\n"\
    "In\tdefine minimum length of a string to be <n>\n"\
    );
    exit(-1);
}



main(argc,argv)
int argc;
char *argv[];
{
    char *strg;
    int c;

#ifdef UNIX
    pname = argv[0];
#endif

#ifdef MSDOS
    /* help for a feeble but ubiquitous operating system */
    pname = "strings";
#endif

    /* get the switches */
    while ((iarg <= argc) && (*argv[iarg]=='-') && !end_flag)
    {
        iap = argv[iarg++] + 1;
        while (*iap && !end_flag)
            switch (*iap++)
            {
            case 'o':
                o_flag = 1;
                break;
            case 'I':
                if (*iap)
                    max_index = atoi(iap);
                iap = noline;
                break;
            case '-':
                end_flag = 1;
                break;
            default:
                TellHow();
            }
    }

    if (iarg == argc)
        infile = stdin;
    else
    {
        if ((infile = fopen(argv[iarg],"r")) == NULL)
        {
            fprintf(stderr,"%s: unable to read file \"%s\"\n",pname,argv[iarg]);
            exit(-1);
        }
    }

    if ((strg = malloc(max_index)) == NULL)
    {
        fprintf(stderr,"%s: not enough memory\n",pname);
        exit(-1);
    }

    max_index -= 1;

    while ((c=getc(infile)) != EOF)
    {
        count++;
        if (IsPrintable(c))
            if (index==max_index)
            {
                if (!printed)
                {
                    if (o_flag)
                        printf("%ld\t",position);
                    for (index=0; index<max_index; index++)
                        putchar(*(strg+index));
                    printed = 1;
                }
                putchar(c);
            }
            else
            {
                if (index==0)
                    position = count;
                *(strg+index++) = c;
            }
        else
        {
            if (printed)
            {
                printed = 0;
                putchar('\n');
            }
            index = 0;
        }
    }
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                      