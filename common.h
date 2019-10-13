#ifndef __COMMON_H__

#define __COMMON_H__

#include<string>

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

//字符/字符串常量 例如 char *a="hello world"
const string STRING = "string";

const string OPERATOR = "operator";

const string DELIMITER = "delimiter";

//种类集
const string CONSTTABLE[] = { ID, KEYWORD, NUMBER, STRING, OPERATOR, DELIMITER };

//控制sizeof运算符
const string SIZEOF = "sizeof";

//控制转义符 用于识别字符/字符串常量中的\'
const char ESCAPE = '\\';

#endif
