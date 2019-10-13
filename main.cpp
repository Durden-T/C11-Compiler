#include<string>
#include<iostream>

#include"lex.h"
#include"tables.h"

using namespace std;

/*
���string ��ʶ��
buffer���
tables���
���ǹ�table���ļ���ȡ
*/
int main(int argc, char** argv)
{
    //�жϲ����Ƿ����
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
