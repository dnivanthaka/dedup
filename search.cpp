#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <map>

#include "FileEntry.h"

std::map<std::string, FileEntry *> filesList;

std::string exec(std::string cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd.c_str(), "r");
    if(pipe != NULL){
        
        while (!feof(pipe)) {
                if (fgets(buffer, 128, pipe) != NULL)
                    result += buffer;
        }
            
        pclose(pipe);
    }
    return result;
}

std::string getsha1sum(std::string path)
{
    //std::string line = exec("echo \""+path+"\" | sed 's/ /\\ /g' | /usr/bin/sha1sum");
    std::string line = exec("/usr/bin/sha1sum \""+path+"\"");
    return line.substr(0, line.find(" "));
}

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
            
            //Check if checksum is not equal
            std::string tmpsum = getsha1sum(full_path);
            
            //if(filesList.find(tmpsum) == filesList.end()){
            if(filesList.find(std::string(pDirent->d_name)) == filesList.end()){
                FileEntry *tfe = new FileEntry(std::string(pDirent->d_name), full_path, tmpsum);
                //filesList.insert(std::pair<std::string, FileEntry *>(tmpsum, tfe));
                filesList.insert(std::pair<std::string, FileEntry *>(std::string(pDirent->d_name), tfe));
                //printf("SUM = %s - [%s]\n", tmpsum.c_str(), full_path.c_str());
            }else{
                //printf("DUPLICATE\n");
                //std::map<std::string, FileEntry *>::iterator it = filesList.find(tmpsum);
                std::map<std::string, FileEntry *>::iterator it = filesList.find(std::string(pDirent->d_name)); 
                if(it != filesList.end()){
                    FileEntry *fe = it->second;
                    fe->insertDuplicate(full_path);
                }
            }
            
        }
        
        //printf ("This is a file [%s]\n", pDirent->d_name);
    }
    
    closedir(dir);
}

int main(int argc, char *argv[])
{
    
    if(argc == 1){
        printf("Usage [program] directory\n");
        return 2;
    }
    
    traverse(argv[1]);
    
    std::map<std::string, FileEntry *>::iterator it;
    for (it=filesList.begin(); it!=filesList.end(); ++it){
        //std::cout << it->first << " => " << it->second << '\n';
        //printf("KEY = %s, VALUE = %s\n", it->first.c_str(), it->second.c_str());
        FileEntry *fe = it->second;
        //printf("NAME = %s\n", fe->getFirstFoundName().c_str());
        //printf("COUNT = %d\n", fe->getDuplicateCount());
        if(fe->getDuplicateCount() > 0){
            printf("Duplicate found, %s, %s\n", fe->getFirstFoundName().c_str(), fe->getFirstFoundLocation().c_str());
        }
        
        //Deleting space
        delete fe;
    }

    return 0;
}

