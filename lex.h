#ifndef __LEX_H__

#define __LEX_H__

#include<fstream>
#include<string>
#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<map>

using namespace std;

//��������С
const size_t BUFSIZE = 4096;
//����״̬
const int ERROR = INT_MIN;
//�������ڱ���־
const char END = '\0';

//��־word����
const string ID = "identifier";

const string KEYWORD = "keyword";

const string NUMBER = "number";

const string OPERATOR = "operator";

const string DELIMITER = "delimiter";

//����sizeof�����
const string SIZEOF = "sizeof";


//�ʷ�������
class Lex
{
public:
    Lex(string&);

    ~Lex();

    void run();

    void showResult();


private:
    //��ʼ���ʷ���������������Ҫ�ı�
    void initTable();
    //��Ӵ���
    inline void addLexeme(string& s, const string& type);
    //��ȡ��һ���ַ�
    char nextChar();
    //�ع�
    void rollback();
    //�������
    void handleError(char);
    //�հ��ַ�/���/���������ܹ�����
    inline bool canSkip(char);

    //�ļ�������
    ifstream f;
    //��ǰ��word
    string word;
    //��������BUFSIZEΪ4k��ͨ�����̿�Ĵ�С������Ϊ2*(BUFSIZE+1)��Ϊ��һ�ζ��룬���Ч�ʣ�ͬʱ�������ڱ�λ
    char buffer[2 * (BUFSIZE + 1)];
    //��ǰ����λ��
    size_t cur = 0;
    //���غ����Զ�Ӧ�ı�
    unordered_map<string, string> lexemes;
    //ͳ�Ƹ������ʹ��ص�����
    map<string, int> typesCount;
    //�������д���
    vector<pair<string,int>> errors;
    //����
    int linesCount;
    //�ַ���
    int charsCount = 0;
    //״̬0ʱ״̬ת����
    static int stateTable[128];
    //�ؼ��ʼ�
    static unordered_set<string> keywords;
    //��ѯ�����
    static bool delimiters[128];
};


#endif