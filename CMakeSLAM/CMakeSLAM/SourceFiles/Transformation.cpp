#include <iostream>
#include <cstdlib>
#include <string>

#include "../HeaderFiles/Transformation.h"
#include "../HeaderFiles/CrashAndBurn.h"

using namespace std;

Transformation::Transformation(FileMapping mapping, string transformation)
{
    file_mapping = mapping;
    input_transformation_string_ = transformation;

    if (input_transformation_string_.empty()) {
        CrashAndBurn("Transformation string empty.\n");
    }

    transformation_name_ = parse_trans_name(input_transformation_string_);
    transformation_args_ = parse_trans_args(input_transformation_string_);
    transformation_type_ = error_trans;
    python_args_ = parse_python_args(transformation_args_);
}

/*
 * Depending on the name of the transformation defined, this method looks
 * at the possible name,enum pairings in transformation mappings and 
 * properly decides what enum to assign.
 */
void Transformation::parse_enum()
{
    for (auto element : transformation_mappings)
    {
        if (transformation_name_ == get<0>(element))
        {
            transformation_type_ = get<1>(element);
        }
    }

    if (transformation_type_ == error_trans) {
        CrashAndBurn("Transformation input unrecognized:\n\t[" 
            + transformation_name_
            + "] did not match any of the defined transformations\n");
    }
}

void Transformation::print() const
{
    cout << "\t" << transformation_name_ << " was called using:\n"
         << "\t\t" << transformation_args_ << "\n";
}

/*
 * Executes the transformation
 */
void Transformation::transform() const
{
    switch(transformation_type_)
    {
    case concat_qual_query:
        execute_concat_qual();
        break;
    case align_from_list:
        break;
    case test_trans:
        break;
    case error_trans:

    default:
        CrashAndBurn("Transformation input unrecognized:\n\t[" 
            + transformation_name_
            + "] did not match any of the defined transformations\n");
    }
}





/*
 * cuts the input string down to just the name of the transformation
 */
string Transformation::parse_trans_name(string input)
{
    const auto index = input.find('(');
    if (index < 1) {
        CrashAndBurn("Transformation input unrecognized:\n"
            + input
            + "\n\tUsage:\t<transformation>(<input>-><output>)\n");
    }

    return remove_whitespace(input.substr(0, index));
}

/*
 * cuts the input string down to just the name of the transformation
 */
string Transformation::parse_trans_args(string input)
{
    const auto index1 = input.find('(');
    const auto index2 = input.find(')');
    if (index1 < 1) {
        const string error;
        CrashAndBurn(error + "Transformation input unrecognized\n"
            + "\tUsage:\t<transformation>(<input>-><output>)\n");
    }

    return remove_whitespace(input.substr(index1 + 1, index2 - index1 - 1));
}

/*
 * Parses the input and output file args from the specified arguments
 */
list<string> Transformation::parse_python_args(string arg_str)
{
    list<string> args = list<string>();
    args.emplace_back(arg_str);
    return args;
}


/*
 * This method removes whitespace (space or tab chars) from the beginning and
 * end of str
 */
string Transformation::remove_whitespace(string str)
{
    int start = 0;
    size_t end = str.length() - 1;

    // ascii for space and tab char
    while (str.at(start) == 32 || str.at(start) == 9) { start++; }
    while (str.at(end) == 32 || str.at(end) == 9) { end--; }
    return str.substr(start, end - start + 1);
}

void Transformation::execute_concat_qual()
{
    cout << "executing concat_qual...\n";
    list<string> list_a = list<string>({ "hello", "world" });
    fork_and_execute(list_a);
}

/*
 * forks process, waits for child process, then continues
 */
void Transformation::fork_and_execute(list<string> args)
{    
    system("python ./PythonScripts/concatQualQuery.py ./PythonScripts/CPBWGS_11_1_genome_xad.fasta.fas.OGS ./PythonScripts/xad.list TEST_output.txt");
}
