#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int is_file(const char *path)
{
    struct stat path_stat;
    
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int main(int argc, char *argv[])
{
    DIR *dir;
    struct dirent *pDirent;
    char pathBuff[100];
    
    if(argc == 1){
        printf("Usage [program] directory\n");
        return 2;
    }
    
    if((dir = opendir(argv[1])) == NULL){
        printf("Error reading directory: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    
    while( (pDirent = readdir(dir)) != NULL ){
        printf ("[%s] ", pDirent->d_name);
        
        sprintf(pathBuff, "%s/%s", argv[1], pDirent->d_name);
        
        if(is_file(pathBuff)){
            printf("This is a file\n");
        }else{
            printf("This is a folder\n");
        }
    }
    
    closedir(dir);

    return 0;
}

