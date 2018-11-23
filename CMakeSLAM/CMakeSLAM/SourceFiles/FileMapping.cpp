#include "../HeaderFiles/FileMapping.h"
#include "../HeaderFiles/CrashAndBurn.h"

using namespace std;

FileMapping::FileMapping()
{
    mapping = list<tuple<string, InputFile>>();
    num_mappings = 0;  
}

void FileMapping::add_file(string identifier, InputFile file)
{
    if (check_for_dups(file))
    {
        num_mappings++;
        const auto fileTuple = tuple<string, InputFile>(identifier, file);
        mapping.push_back(fileTuple);
    }
}

InputFile FileMapping::get_file(const string& identifier)
{
    for (auto fileTuple : mapping)
    {
        if (get<0>(fileTuple) == identifier)
        {
            return get<1>(fileTuple);
        }
    }

    // mapping does not exist
    CrashAndBurn("The file identifier " + identifier + "does not exist.\n");
    return InputFile("error", "error");
}

bool FileMapping::empty() const
{
    return num_mappings <= 0;
}

bool FileMapping::check_for_dups(InputFile file)
{
    for (auto fileTuple : mapping)
    {
        if (get<0>(fileTuple) == file.fileName)
        {
            if (get<1>(fileTuple).filePath != file.filePath)
            {
                CrashAndBurn("The file identifier [" + file.fileName + "] is ambiguous.\n");
            }
            return false;
        }
    }

    return true;
}
