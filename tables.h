#ifndef __TABLES__H__

#define __TABLES__H__

#include<string>
#include<unordered_set>

#include"common.h"

using namespace std;

class Tables
{
public:
    //��ʼ���ʷ���������������Ҫ�ı�
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

    //״̬0ʱ״̬ת����
    static int stateTable[128];
    //�ؼ��ʼ�
    static unordered_set<string> keywords;
    //��ѯ�����
    static bool delimiters[128];
};

#endif
