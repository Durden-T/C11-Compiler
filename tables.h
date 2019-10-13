#ifndef __TABLES__H__

#define __TABLES__H__

#include<string>
#include<unordered_set>

#include"common.h"

using namespace std;

class Tables
{
public:
    //初始化词法分析过程中所需要的表
    Tables();

    bool isDelimiter(char c)
    {
        return delimiters[c];
    }

    int switchState(char c)
    {
        return stateTable[c];
    }

    bool isKeyword(const string& word)
    {
        return keywords.count(word);
    }

private:

    //状态0时状态转换表
    static int stateTable[128];
    //关键词集
    static unordered_set<string> keywords;
    //查询界符集
    static bool delimiters[128];
};

#endif
