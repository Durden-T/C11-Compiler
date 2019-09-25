#include<string>
#include<iostream>

#include"lex.h"

using namespace std;

int main(int argc, char** argv)
{
    //�жϲ����Ƿ����
    if (argc == 1)
        throw ios_base::failure("Error:No parameter.");

    string filename = string(argv[1]);
    Lex lex(filename);
    lex.run();
    lex.showResult();

    return 0;
}