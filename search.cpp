#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <map>


std::map<std::string, std::string> filesList;

ssize_t get_size(const char *path)
{
    struct stat path_stat;
    
    stat(path, &path_stat);
    return path_stat.st_size;
}

int is_dir(const char *path)
{
    struct stat path_stat;
    
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

int is_file(const char *path)
{
    struct stat path_stat;
    
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void traverse(const char *path){
    DIR *dir;
    struct dirent *pDirent;
    std::string full_path;
    
    if((dir = opendir(path)) == NULL){
        printf("Error reading directory: %s\n", path);
        exit(EXIT_FAILURE);
    }
    
    while( (pDirent = readdir(dir)) != NULL ){
        //printf ("[%s]\n", pDirent->d_name);
        
        if(strcmp(pDirent->d_name, ".") == 0 || strcmp(pDirent->d_name, "..") == 0){
            continue;
        }
        
        //sprintf(pathBuff, "%s/%s", path, pDirent->d_name);
        full_path = std::string(path) + "/" + std::string(pDirent->d_name);
        
        if(is_dir(full_path.c_str())){
            //printf ("DIRECTORY [%s]\n", pDirent->d_name);
            traverse(full_path.c_str());
        }else{
            //printf ("FILE [%s]\n", pDirent->d_name);
            
            //Check if name is not equal
            if(filesList.find(pDirent->d_name) == filesList.end()){
                filesList.insert(std::pair<std::string, std::string>(pDirent->d_name, full_path));
            }else{
                //printf("File Exists: [%s] [%s]\n", pathBuff, (filesList.find(pDirent->d_name)->second).c_str());
                
                if(get_size(full_path.c_str()) == get_size((filesList.find(pDirent->d_name)->second).c_str())){
                    printf("File Exists: [%s] [%s] | Size = %ld\n", full_path.c_str(), (filesList.find(pDirent->d_name)->second).c_str(), get_size(full_path.c_str()));
                    //printf("File size is equal %ld\n", get_size(pathBuff));
                }
            }
        }
        
        //printf ("This is a file [%s]\n", pDirent->d_name);
    }
    
    closedir(dir);
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
    
    traverse(argv[1]);
    
    /*std::map<std::string, std::string>::iterator it;
    for (it=filesList.begin(); it!=filesList.end(); ++it){
        //std::cout << it->first << " => " << it->second << '\n';
        printf("KEY = %s, VALUE = %s\n", it->first.c_str(), it->second.c_str());
    }*/

    return 0;
}

