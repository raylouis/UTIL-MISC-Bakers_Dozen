/* $title:      'TOLOWER.C ===== Create script to rename files =====' */
/* $subtitle:   'Elipse Ltd.  [jms]             Revised: 2001 Jan 21' */


#include <stdio.h>
#include <string.h>

#ifdef __AIX__
    #define RENAME  "mv"
#else
    #define RENAME  "ren"
#endif

int main( int argc, char *argv[] );
static void toLower( const char *cmd );


/* MAIN ===== Starting point ===== $pageif:6 */

int main( int argc, char *argv[] )
    {                                   /* main */
    char *cmd = RENAME;
    fprintf( stderr, "\nCreate lower case rename script\n" );
    if ( argc > 1 )
        cmd = argv[1];
    toLower( cmd );
    return 0;
    }                                   /* main */


/* TOLOWER ===== Body of program ===== $pageif:6 */

static void toLower( const char *cmd )
    {                                   /* toLower */
    char buf1[1024], buf2[1024];
    memset( buf1, '\0', sizeof (buf1) );
    memset( buf1, '\0', sizeof (buf1) );

    while ( scanf( "%s", buf1 ) == 1 )
        {
        if ( *buf1 != '.' )
            {
            strcpy( buf2, buf1 );
            strlwr( buf2 );
            if (strcmp( buf2, buf1 ) != 0)
                printf( "%s %s %s\n", cmd, buf1, buf2 );
            }
        }
    }                                   /* toLower */
