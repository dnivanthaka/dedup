#include <string>
#include <vector>

#include "FileEntry.h"

FileEntry::FileEntry(std::string name, std::string location, std::string sh)
{
    firstFoundName = name;
    firstFoundLocation = location;
    sha1sum = sh;
}

FileEntry::~FileEntry()
{

}

void FileEntry::setFirstFoundName(std::string n)
{
    firstFoundName = n;
}

void FileEntry::setFirstFoundLocation(std::string l)
{
    firstFoundLocation = l;
}

void FileEntry::setSha1sum(std::string s)
{
    sha1sum = s;
}

void FileEntry::insertDuplicate(std::string d)
{
    duplicates.push_back(d);
}

unsigned int FileEntry::getDuplicateCount()
{
    return duplicates.size();
}
