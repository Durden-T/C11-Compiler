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

//词法生成器
class Lex
{
    using lexeme = pair<string, string>;

public:
    Lex(string& fileName, Tables& _table);

    ~Lex();

    void run();
    //添加词素
    inline void addLexeme(string& s, const string& type);

    void showResult();

private:
    //初始化types的对应关系
    void initTypes();
    //获取下一个字符
    char nextChar();
    //回滚
    void rollback();
    //处理错误
    void handleError(char c);
    //空白字符/界符/结束符，能够跳过
    inline bool canSkip(char c);

    //各种状态表
    Tables& tables;
    //文件输入流
    ifstream f;
    //当前的word
    string word;
    //当前的line
    string line;
    //缓冲区，BUFSIZE为4k（通常磁盘块的大小），设为2*(BUFSIZE+1)是为了一次读入，提高效率，同时设置了哨兵位
    char buffer[2 * (BUFSIZE + 1)];
    //词素和属性对应的表
    unordered_map<string, string> lexemes;
    //统计各种类型词素的数量
    map<string, vector<lexeme>*> types;
    //保存所有错误
    vector<Error> errors;
    //行数
    int linesCount = 0;
    //字符数
    int charsCount = 0;
    //当前输入位置
    size_t cur = 0;
    //分类保存,由types代理
    vector<lexeme> id_, keyword_, number_, string_, operator_, delimiter_;
};

#endif
