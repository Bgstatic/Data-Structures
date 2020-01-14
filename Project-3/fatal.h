#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Error( Str )        FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )
