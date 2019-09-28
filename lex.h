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

//缓冲区大小
const size_t BUFSIZE = 4096;
//错误状态
const int ERROR = INT_MIN;
//结束及哨兵标志
const char END = '\0';

//标志word类型
const string ID = "identifier";

const string KEYWORD = "keyword";

const string NUMBER = "number";

const string OPERATOR = "operator";

const string DELIMITER = "delimiter";

//控制sizeof运算符
const string SIZEOF = "sizeof";


struct Error
{
    string word;
    int lineNumber;
    string line;
};


//词法生成器
class Lex
{
    using lexeme = pair<string, string>;

public:
    Lex(string&);

    ~Lex();

    void run();
    //添加词素
    inline void addLexeme(string& s, const string& type);

    void showResult();


private:
    //初始化词法分析过程中所需要的表
    void initTable();
    //获取下一个字符
    char nextChar();
    //回滚
    void rollback();
    //处理错误
    void handleError(char);
    //空白字符/界符/结束符，能够跳过
    inline bool canSkip(char);


    //文件输入流
    ifstream f;
    //当前的word
    string word;
    //当前的line
    string line;
    //缓冲区，BUFSIZE为4k（通常磁盘块的大小），设为2*(BUFSIZE+1)是为了一次读入，提高效率，同时设置了哨兵位
    char buffer[2 * (BUFSIZE + 1)];
    //当前输入位置
    size_t cur = 0;
    //词素和属性对应的表
    unordered_map<string, string> lexemes;
    //统计各种类型词素的数量
    map<string, vector<lexeme>*> types;
    //分类保存,由types代理
    vector<lexeme> id_, keyword_, number_, operator_, delimiter_;
    //保存所有错误
    vector<Error> errors;
    //行数
    int linesCount = 0;
    //字符数
    int charsCount = 0;
    //状态0时状态转换表
    static int stateTable[128];
    //关键词集
    static unordered_set<string> keywords;
    //查询界符集
    static bool delimiters[128];
    //种类集
    static const string CONSTTABLE[5];
};


#endif