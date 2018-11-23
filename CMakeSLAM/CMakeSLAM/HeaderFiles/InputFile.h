#pragma once

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class InputFile
{
public:
    string fileName;
    string filePath;

    InputFile(string identifier, string file_path)
    {
        fileName = identifier;
        filePath = file_path;
        verify_existence(file_path);
    }

    bool is_readable() const
    {
        return readable;
    }

    void print() const
    {
        cout << "\t" << fileName << " = " << filePath << "\t[";
             
        if (readable)
            cout << "valid]\n";
        else
            cout << "invalid]\n";
    }

    void print_output_file() const
    {
        cout << "\t" << fileName << " = " << filePath << "\t[output -> always valid]";
    }

private:
    bool readable;

    void verify_existence(const string file_path)
    {
        try
        {
            ifstream stream;
            stream.open(file_path);
            if (stream.is_open())
            {
                readable = true;
            }
        }
        catch (exception& e)
        {
            readable = false;
        }
    }
};