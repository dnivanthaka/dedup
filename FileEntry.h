#ifndef FILEENTRY_H
#define FILEENTRY_H
#include <vector>

class FileEntry{
    public:
        FileEntry(std::string name, std::string location, std::string sh);
        ~FileEntry();
        void setFirstFoundName(std::string n);
        void setFirstFoundLocation(std::string l);
        void setSha1sum(std::string s);
        std::string getFirstFoundName() {return firstFoundName;};
        std::string getFirstFoundLocation() {return firstFoundLocation;};
        std::string getSha1sum() {return sha1sum;};
        void insertDuplicate(std::string d);
        unsigned int getDuplicateCount();
        std::vector<std::string> getDuplicatesList();
    private:
        std::string firstFoundName;
        std::string firstFoundLocation;
        std::string sha1sum;
        std::vector<std::string> duplicates;
};
#endif
