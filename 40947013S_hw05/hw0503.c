#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

#define capital(x) (x >= 0 && x <= 25)
#define lower(x) (x >= 26 && x <= 51)
#define number(x) (x >= 52 && x <= 61)

#define Capital(x) (x >= 65 && x <= 90)
#define Lower(x) (x >= 97 && x <= 122)
#define Number(x) (x >= 48 && x <= 57)

struct option long_options[] = 
{  
    {"enc", 1, NULL, 'e'},
    {"dec", 1, NULL, 'd'},
    {"output", 1, NULL, 'o'},   
    {0 , 0 , 0 , 0},  
};  

FILE *input, *output;

uint8_t *encode(uint8_t *content, int have)
{
    uint8_t *res = calloc(4, sizeof(uint8_t));
    bool *bool_con = malloc(24);
    
    for(int i = 0; i < 3; i++)
    {
        int count = (i+1)*8-1;
        while(content[i] != 0)
        {
            bool_con[count--] = content[i]%2;
            content[i] = content[i] >> 1;            
        }     
    }
    for(int i = 0; i < 4; i++)
    {
        int end = (i+1)*6-1;
        for(int j = 0; j < 6; j++)
            res[i] += bool_con[end-j] << j;           if(i*6 > have*8 && res[i] == 0) res[i] = 61; // '='
        else if(capital(res[i])) res[i] += 65;
        else if(lower(res[i])) res[i] += 71;
        else if(number(res[i])) res[i] -= 4;
        else if(res[i] == 62) res[i] = 43;
        else if(res[i] == 63) res[i] = 47;
    }
    return res;
}

void *decode(uint8_t *content)
{
    uint8_t *res = calloc(3, sizeof(uint8_t));
    bool *bool_con = malloc(24);
    int limit = 4;
    
    for(int i = 0; i < 4; i++)
    {
        if(Capital(content[i])) content[i] -= 65;
        else if(Lower(content[i])) content[i] -= 71;
        else if(Number(content[i])) content[i] += 4;
        else if(content[i] == 43) content[i] = 62;
        else if(content[i] == 47) content[i] = 63;
        else
        {
            limit = i; break;
        }
        int count = (i+1)*6-1;
        while(content[i] != 0)
        {
            bool_con[count--] = content[i]%2;
            content[i] = content[i] >> 1;            
        }    
        
    }
    int last = limit == 4 ? 3 : ((limit+1)*6)/8-1;
    for(int i = 0; i < last; i++)
    {
        int end = (i+1)*8-1;
        for(int j = 0; j < 8; j++)
            res[i] += bool_con[end-j] << j;
        fwrite(&res[i], 1, 1, output);
    }
    return res;
}

int main(int argc , char *argv[]) 
{
    int32_t c = 0;
    bool is_decode = false, is_encode = false;
    char *n1 = malloc(1024), *n2 = malloc(1024);
    
    while ((c = getopt_long(argc, argv, "e:d:o:", long_options, 0)) != -1 )
    {
        switch(c)
        {
            case 'o':
            n1 = optarg; break;
            case 'd':
            is_decode = true; 
            n2 = optarg; break;
            case 'e':
            is_encode = true; 
            n2 = optarg; break;          
        }
    }
    if(is_encode&is_decode)
    {
        printf("Can only use one of it.\n");
        exit(0);
    }
    if((input = fopen(n2 , "rb")) == NULL)
    {
        printf("File could not be opened!\n");
        return 0;
    }        
    if((output = fopen(n1 , "wb+")) == NULL)
    {
        printf("File could not be opened!\n");
        return 0;
    }
    
    if(is_encode)
    {
        while(!feof(input))
        {
            uint8_t *content = malloc(3);
            uint8_t *en_content = malloc(4);
            int i;
            for(i = 0; i < 3; )
            {
                fread(&(content[i]), 1, sizeof(uint8_t), input);
                if(!feof(input)) i++;
                else break;
            }
            if(i == 0 && feof(input)) break;            
            en_content = encode(content, i);
            
            fwrite(en_content, 1, 4, output);            
        }
    }
    if(is_decode)
    {
        fseek(input, 0, SEEK_END);
        int32_t size = ftell(input);
        fseek(input, 0, SEEK_SET);
        if(size%4 != 0)
        {
            printf("%s is not encode by Base64!\n", n2);
            exit(0);
        }
        while(!feof(input))
        {
            uint8_t *content = malloc(4);
            uint8_t *de_content = malloc(3);
            int i;
            for(i = 0; i < 4; )
            {
                fread(&(content[i]), 1, sizeof(uint8_t), input);
                if(!feof(input)) i++;
                else break;
            }
            if(i == 0 && feof(input)) break; 
            decode(content);          
        }
    }
    fclose(input);
    fclose(output);
    return 0;
}