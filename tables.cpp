#include "tables.h"

//初始化部分表
int Tables::stateTable[] = { ERROR };

unordered_set<string> Tables::keywords = { "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "_Bool" };

bool Tables::delimiters[] = { false };

Tables::Tables()
{
    //清空stateTable表
    memset(stateTable, ERROR, 128 * sizeof(int));
    //设置字母转换
    for (char c = 'a'; c != 'z'; ++c)
        stateTable[c] = stateTable[c + 'A' - 'a'] = 1;
    //设置数字转换
    for (char i = '0'; i <= '9'; ++i)
        stateTable[i] = 2;
    //设置运算符转换
    stateTable['/'] = 8;
    stateTable['*'] = stateTable['\\'] = stateTable['='] = stateTable['!'] = stateTable['^'] = stateTable['%'] = 12;
    stateTable['+'] = stateTable['-'] = stateTable['|'] = stateTable['&'] = 13;
    stateTable['>'] = stateTable['<'] = 14;
    stateTable['\''] = stateTable['"'] = 16;
    //清空界符表
    memset(delimiters, false, 128 * sizeof(bool));
    //设置界符转换
    delimiters['{'] = delimiters['}'] = delimiters['['] = delimiters[']'] = delimiters['('] = delimiters[')'] = delimiters[';'] = delimiters['.'] = true;
}
