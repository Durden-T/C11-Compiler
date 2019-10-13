#ifndef __LEX_H__

#define __LEX_H__

#include<fstream>
#include<string>
#include<iostream>
#include<vector>
#include<unordered_map>
#include<map>

#include"common.h"
#include"tables.h"

using namespace std;

struct Error
{
    string word;
    int lineNumber = 0;
    string line;
};

//�ʷ�������
class Lex
{
    using lexeme = pair<string, string>;

public:
    Lex(string& fileName, Tables& _table);

    ~Lex();

    void run();
    //��Ӵ���
    inline void addLexeme(string& s, const string& type);

    void showResult();

private:
    //��ʼ��types�Ķ�Ӧ��ϵ
    void initTypes();
    //��ȡ��һ���ַ�
    char nextChar();
    //�ع�
    void rollback();
    //�������
    void handleError(char c);
    //�հ��ַ�/���/���������ܹ�����
    inline bool canSkip(char c);

    //����״̬��
    Tables& tables;
    //�ļ�������
    ifstream f;
    //��ǰ��word
    string word;
    //��ǰ��line
    string line;
    //��������BUFSIZEΪ4k��ͨ�����̿�Ĵ�С������Ϊ2*(BUFSIZE+1)��Ϊ��һ�ζ��룬���Ч�ʣ�ͬʱ�������ڱ�λ
    char buffer[2 * (BUFSIZE + 1)];
    //���غ����Զ�Ӧ�ı�
    unordered_map<string, string> lexemes;
    //ͳ�Ƹ������ʹ��ص�����
    map<string, vector<lexeme>*> types;
    //�������д���
    vector<Error> errors;
    //����
    int linesCount = 0;
    //�ַ���
    int charsCount = 0;
    //��ǰ����λ��
    size_t cur = 0;
    //���ౣ��,��types����
    vector<lexeme> id_, keyword_, number_, string_, operator_, delimiter_;
};

#endif
