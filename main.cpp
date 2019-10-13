#include<string>
#include<iostream>

#include"lex.h"
#include"tables.h"

using namespace std;

/*
添加string 及识别
buffer抽出
tables抽出
考虑过table从文件读取
*/
int main(int argc, char** argv)
{
    //判断参数是否存在
    if (argc == 1)
    {
        cerr<<"Error:No parameter."<<endl;
        return -1;
    }

    Tables tables;

    string filename = string(argv[1]);
    try
    {
        Lex lex(filename, tables);
        lex.run();
        lex.showResult();
    }
    catch (exception & e)
    {
        cerr << e.what() << endl;
        return -2;
    }

    return 0;
}
