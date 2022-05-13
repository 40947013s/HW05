#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

struct option long_options[] = 
{  
    { NULL, 1, NULL, 't' },
    { NULL, 1, NULL, 'T' },
    { NULL, 1, NULL, 'a' },
    { NULL, 1, NULL, 'A' },
    { NULL, 1, NULL, 'y' },
    { NULL, 1, NULL, 'c' },   
    { NULL, 1, NULL, 'g' },   
    { NULL, 0, NULL, 'l' },   
    { NULL, 0, NULL, 'd' },   
    { NULL, 0, NULL, 'h' },   
    { 0 , 0 , 0 , 0 },  
};  

typedef struct s_tag
{
    char header[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[28];
    char zero;
    char track;
    char genre;
}Tag; Tag ori; 

void help()
{
    printf("usage: hw0502 -[tTaAycg] 'value' file1 [file2...]\n\t hw0502 -d file1 [file2...]\n");
    printf("\t hw0502 -l file1 [file2...]\n");
    printf("\t hw0502 -h\n -t Modifies a Title tag\n");
    printf(" -T Modifies a Track tag\n -a Modifies an Artist tag\n -A Modifies an Album tag\n -y Modifies a Year tag\n -c Modifies a Comment tag\n -g Modifies a Genre tag\n -l Lists an ID3 tag\n -d Deletes an ID3 tag\n -h Displays this help info\n");
}

void list_print(char *str)
{
    printf("Title : ");
    for(int i = 0; i < 30; i++) printf("%c", ori.title[i]);
    printf("\n");
    printf("Artist: ");
    for(int i = 0; i < 30; i++) printf("%c", ori.artist[i]);
    printf("\n");
    printf("Album : ");
    for(int i = 0; i < 30; i++) printf("%c", ori.album[i]);
    printf("\n");
    printf("Year: ");
    for(int i = 0; i < 4; i++) printf("%c", ori.year[i]);
    printf("\n");
    if(ori.genre > 126) printf("Genre: NULL\n");
    else printf("Genre: %s (%d)\n", str, ori.genre);
    printf("Comment: ");
    for(int i = 0; i < 28; i++) printf("%c", ori.comment[i]);
    printf("\n");
    printf("Track: %c\n\n", ori.track);    
}

int find(char *str)
{
    int count = 0;
    FILE *fp = fopen("genre.txt", "r");
    char s[128];
    while(fgets(s, 128, fp))
    {
        if(strncmp(s, str, strlen(s)-1) == 0) return count;
        count++;
    }
    return count;
}

bool IsYear(char *year)
{
    for(int i = 0; i < strlen(year); i++)
        if(year[i] > '9' || year[i] < '0')
            return false;
    return true;
}

char *deal(char *str, int length, int size)
{
    char *res = calloc(size, size);
    strncpy(res, str, length);
    return res;
}

int main(int argc , char *argv[]) 
{
    int limit, c, state; char *temp = malloc(1024);
    bool list = false, del = false;   
    

    while((c = getopt_long( argc, argv, "t:T:a:A:y:c:g:ldh", long_options, 0)) != -1)
    {
        switch(c)
        {
            case 't':
                limit = strlen(optarg) >= 30 ? 30 : strlen(optarg);
                temp = optarg; state = 1; break;
            case 'T':
                limit = 1; temp = optarg; state = 2;break;
            case 'a':
                limit = strlen(optarg) >= 30 ? 30 : strlen(optarg);
                temp = optarg; state = 3; break;
            case 'A':
                limit = strlen(optarg) >= 30 ? 30 : strlen(optarg);
                temp = optarg; state = 4; break;
            case 'y':
                limit = strlen(optarg) >= 4 ? 4 : strlen(optarg);
                temp = optarg; state = 5; limit = IsYear ? limit : 0; break;
            case 'c':
                limit = strlen(optarg) >= 30 ? 30 : strlen(optarg);
                temp = optarg; state = 6; break;
            case 'g':
                temp = optarg; state = 7; break;
                break;
            case 'l':
                list = true; break;
            case 'd':
                del = true; break;
            case 'h':
                help(); break;
        }
    }
    for(int i = (del|list) ? 2 : 3; i < argc; i++)
    {
        FILE *fp;        
        if((fp = fopen(argv[i] , "rb+")) == NULL)
        {
            printf("%s could not be opened!\n", argv[i]);
            continue;
        }

        fseek(fp, 0L, SEEK_END);
        int size = ftell(fp)-128;
        rewind(fp);
        
        fseek(fp, -128, SEEK_END);
        fread(&ori, sizeof(Tag), 1, fp);
        
        
        if(strncmp(ori.header, "tag", 3) != 0 && strncmp(ori.header, "TAG", 3) != 0)
        {
            printf("%s do not have id3v1 header!\n", argv[i]);
            continue;
        }
        if(list == true)
        {
            char *str = malloc(128);
            printf("%s :\n", argv[i]);
            FILE *fp = fopen("genre.txt", "r");
            if(ori.genre > 126) str = 0;
            else
            {
                for(int j = 0; j < ori.genre; j++)
                    fgets(str, 128, fp);
                str[strlen(str)-1] = 0;
            }
            list_print(str);
            break;
        }
        
        FILE *tmp = fopen("temp.mp3", "w");
        rewind(fp);
        
        char *c = malloc(size);
        fread(c, size, 1, fp);
        fwrite(c, size, 1, tmp);
        
        if(state == 1) strncpy(ori.title, deal(temp, limit, 30), 30);
        if(state == 2) ori.track = temp[0];
        if(state == 3) strncpy(ori.artist, deal(temp, limit, 30), 30);
        if(state == 4) strncpy(ori.album, deal(temp, limit, 30), 30);
        if(state == 5) strncpy(ori.year, deal(temp, limit, 4), 4);
        if(state == 6) strncpy(ori.comment, deal(temp, limit, 28), 28);
        if(state == 7) ori.genre = find(temp);
        
        if(del == false)
        {
            fwrite(&ori, sizeof(Tag), 1, tmp);
            printf("1");
        }
        fclose(fp); fclose(tmp);
        FILE *new_fp = fopen(argv[i] , "w");
        FILE *read = fopen("temp.mp3", "r");

        size = del ? size : size+128;
        c = malloc(size);
        fread(c, sizeof(char), size, read);
        fwrite(c, sizeof(char), size, new_fp);
        remove("temp.mp3");
    } 
    return 0;
}
