#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

struct option long_options[] = 
{  
    { "bytes", 0, NULL, 0 },
    { "kilo", 0, NULL, 'b' },
    { "mega", 0, NULL, 1 },
    { "help", 0, NULL, 2 },
    { "seconds", 1, NULL, 's' },
    { "count", 1, NULL, 'c' },   
    { 0 , 0 , 0 , 0 },  
};  

typedef unsigned long long ull;

void help()
{
    printf("Usage:\n\thw0501 [options]\nOptions:\n\t-b, --bytes show output in bytes\n\t    --kilo show output in kilobytes (default)\n\t    --mega show output in megabytes\n\t-s N, --seconds N repeat printing every N seconds\n\t-c N, --count N repeat printing N times , then exit\n\t--help display this help and exit\n");
};

int main(int argc , char *argv[]) 
{
    FILE *info;    
    char buf[256], *temp = malloc(256);
    int counter = 0, c, interval = 0, times = 0, u = 1;
    double denominator = 1;
    char unit[3][3] = {"B", "KB", "MB"};
    while((c = getopt_long( argc, argv, "s:c:012b", long_options, 0)) != -1)
    {
        switch(c)
        {
            case 's':
                interval = strtol(optarg, NULL, 10);
                if(strtol(optarg, NULL, 10) == 0 && strlen(optarg) != 1)
                    printf("Default : 0s\n");                    
                break;
            case 'c':
                times = strtol(optarg, NULL, 10);
                if(strtol(optarg, NULL, 10) == 0 && strlen(optarg) != 1)
                    printf("Default : 0s\n");                    
                break;
            case 0:
                denominator = 0.001; u = 0; break;
            case 1:
                denominator = 1000; u = 1; break;
            case 2:
                help(); break;
        }
    }
    ull encode[3];
    
    for(int i = 0; i <= times; i++)
    {
        if((info = fopen("/proc/meminfo" , "rb")) == NULL)
        {
            printf( "File could not be opened!\n" );
            return 0;
        }
        for(int j = 0; j < 3; j++)
        {
            fgets(buf, sizeof(buf), info);
            sscanf(buf, "%s %llu", temp, &encode[j]);
        }
        double y = (double)encode[2]/(double)denominator;
        double x = (double)(encode[2])/(double)(encode[0]);
        x *= 100;
        printf("Available : %.lf %s (%.2lf", y, unit[u], x);
        putc('%', stdout);
        printf(")\n");
        
        rewind(info);
        if(i != times) sleep(interval);         
    }
    return 0;
}