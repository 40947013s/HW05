#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <math.h>

#define capital(x) ( x >= 0 && x <= 25 )
#define lower(x) ( x >= 26 && x <= 51 )
#define number(x) ( x >= 52 && x <= 61 )
#define plus(x) (x == 62 )

#define Capital(x) ( x >= 65 && x <= 90 )
#define Lower(x) ( x >= 97 && x <= 122 )
#define Number(x) ( x >= 48 && x <= 57 )
#define Plus(x) (x == 43 )

char ascii[128];

struct option long_options[] = 
{  
    { "enc", 1, NULL, 'e' },
    { "dec", 1, NULL, 'd' },
    { "output", 1, NULL, 'o' },   
    { 0 , 0 , 0 , 0 },  
};  

void change( uint8_t *arr , int32_t size , FILE *output )
{
    uint8_t b1[size][8];
    for(int i=0;i<size;i++)
    {
        int32_t count = 7;
        for(int j=0;j<8;j++)
        {
            b1[i][j] = 0;
        }
        while( arr[i] > 0 )
        {
            b1[i][count] = arr[i] % 2;
            arr[i] /= 2;
            count--;
        }
    }
    int32_t all = size * 8;
    int32_t times = all / 24 * 4;
    int32_t time2 = (all % 24) ;
    
    if( time2 / 6 * 6 != time2 )
    {
        time2 %= 6;
    }
    else
    {
        time2 = 0;
    }

    int32_t Q = all / 6;
    int32_t R = all % 6;
    uint8_t b2[times+4][6];
    
    int32_t d[Q];
    for(int i=0;i<times+4;i++)
    {
        memset( b2[i] , 0 , 6 );
    }
    for(int i=0;i<Q*6+time2;i++)
    {
        b2[i/6][i%6] = 0;
        b2[i/6][i%6] = b1[i/8][i%8];
        //printf("%d ",b1[i/8][i%8]);
    }
    if( time2 != 0 )
    {
        Q++;
    }
    for(int i=0;i<Q;i++)
    {
        d[i] = 0;
        for(int j=0;j<6;j++)
        {
            d[i] += b2[i][j] * pow(2,5-j);
        }
        for(int j=0;j<6;j++)
        {
            //printf("%d ",b2[i][j]);
        }
        //printf("%d\n",d[i]);
    }
    char D[Q];
    for(int i=0;i<Q;i++)
    {
        D[i] = 0;
        if( capital(d[i]) )
        {
            D[i] = d[i] + 65;
        }
        else if( lower(d[i]) )
        {
            D[i] = d[i] + 71;
        }
        else if( number(d[i]) )
        {
            D[i] = d[i] - 4;
        }
        else if( plus(d[i]) )
        {
            D[i] = 43;
        }
        else 
        {
            D[i] = 47;
        }
        //printf( "%c" , D[i] );
    }
    int32_t trivial = ( ( times + 4 ) * 6 - all) / 6;
    char ans[Q+trivial];
    for(int i=0;i<Q+trivial;i++)
    {
        if( i < Q )
        {
            ans[i] = D[i];
        }
        else if( i < Q && trivial!= 4 )
        {
            ans[i] = '=';
        }
    }
    fwrite( ans , strlen(ans) , 1 , output );
} 




void change_1( uint8_t *arr , int32_t size , FILE *output )
{
    uint8_t modify[size];
    for(int i=0;i<size;i++)
    {
        modify[i] = 0;
    }
    for(int i=0;i<size;i++)
    {
        if( Capital(arr[i]) )
        {
            modify[i] = arr[i] - 65;
        }
        else if( Lower(arr[i]) )
        {
            modify[i] = arr[i] - 71;
        }
        else if( Number(arr[i]) )
        {
            modify[i] = arr[i] + 4;
        }
        else if( Plus(arr[i]) )
        {
            modify[i] = 62;
        }
        else 
        {
            modify[i] = 63;
        }
        
    }
    uint8_t b1[size][6];
    for(int i=0;i<size;i++)
    {
        int32_t count = 5;
        for(int j=0;j<6;j++)
        {
            b1[i][j] = 0;
        }
        //printf("%2d ",modify[i]);
        while( modify[i] > 0 )
        {
            b1[i][count] = modify[i] % 2;
            modify[i] /= 2;
            count--;
        }
    }
    
    int32_t all = size * 6;    
    int32_t Q = all / 8;

    uint8_t b2[Q][8];
    
    char d[Q];
    for(int i=0;i<Q*8;i++)
    {
        b2[i/8][i%8] = 0;
        b2[i/8][i%8] = b1[i/6][i%6];
    }
    for(int i=0;i<Q;i++)
    {
        d[i] = 0;
        for(int j=0;j<8;j++)
        {
            d[i] += b2[i][j] * pow(2,7-j);
        }
    }
    char D[Q];
    for(int i=0;i<Q;i++)
    {
        D[i] = d[i];
        //printf("%c", ascii[d[i]] );
    }
    fwrite( D , Q , 1 , output );
} 

int main(  int argc , char *argv[] ) 
{
    FILE *input ,*output;
    int32_t c = 0;
    int32_t index = 0;
    int32_t condition = 0;
    char Filename[1024] , filename[1024];
    uint8_t *Content;
    while ( ( c = getopt_long( argc, argv, "e:d:o:", long_options, &index ) ) != -1 )
    {
        switch( c )
        {
            case 'o':
            strcpy( Filename , optarg );
            break;

            case 'd':
            condition = 2;
            strcpy( filename , optarg );
            break;

            case 'e':
            condition = 1;
            strcpy( filename , optarg );
            break;
            
        }
        if( condition == 1 )
        {
            input = fopen( filename , "rb+" );
            output = fopen( Filename , "wb+" );
            fseek( input , 0 , SEEK_END );
            int32_t size = ftell( input );
            fseek( input , 0 , SEEK_SET );
            uint8_t *Content = malloc( size );
            fread( Content , size , 1 , input );
            change( Content , size , output );
        }
        else if( condition == 2 )
        {
            input = fopen( optarg , "rb+" );
            output = fopen( Filename , "wb+" );
            fseek( input , 0 , SEEK_END );
            int32_t Size = ftell( input );
            fseek( input , 0 , SEEK_SET );
            uint8_t *content = malloc( Size );
            fread( content , Size , 1 , input );
            for(int i=Size-1;i>=0;i--)
            {
                if( content[i] == 61 )
                {
                    Size --;
                }
                else
                {
                    break;
                }
            }
            change_1( content , Size , output );
        }
    }
    return 0;
}