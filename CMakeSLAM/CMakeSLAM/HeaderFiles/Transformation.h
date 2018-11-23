#pragma once

#include <string>
#include <list>

#include "FileMapping.h"

using namespace std;

class Transformation
{
public:
    /*************************************************************************/
    enum Trans_enum {error_trans, test_trans, concat_qual_query, align_from_list};
    /*************************************************************************/    

    string input_transformation_string_;

    Transformation(FileMapping mapping, string transformation);
    void parse_enum();
    void print() const;
    void transform() const;

private:
    list<tuple<string, Trans_enum>> transformation_mappings { 
        {"test", test_trans},
        {"concatQualQuery", concat_qual_query},
        {"alignFromList", align_from_list},
        {"ERROR", error_trans}
    };

    tuple<string, list<string>> cancat_qual_query_exec {
        "concatQualQuery.py",
        {"concatQualQuery.py"}
    };
        
    string transformation_name_;
    string transformation_args_;

    FileMapping file_mapping;
    Trans_enum transformation_type_;
    list<string> python_args_;
       
    static string parse_trans_name(string input);
    static string parse_trans_args(string input);
    static string remove_whitespace(string str);
    static list<string> parse_python_args(string arg_str);

    static void execute_concat_qual();
    static void fork_and_execute(list<string> args);
};