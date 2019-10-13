#include "tables.h"

//��ʼ�����ֱ�
int Tables::stateTable[] = { ERROR };

unordered_set<string> Tables::keywords = { "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "_Bool" };

bool Tables::delimiters[] = { false };

Tables::Tables()
{
    //���stateTable��
    memset(stateTable, ERROR, 128 * sizeof(int));
    //������ĸת��
    for (char c = 'a'; c != 'z'; ++c)
        stateTable[c] = stateTable[c + 'A' - 'a'] = 1;
    //��������ת��
    for (char i = '0'; i <= '9'; ++i)
        stateTable[i] = 2;
    //���������ת��
    stateTable['/'] = 8;
    stateTable['*'] = stateTable['\\'] = stateTable['='] = stateTable['!'] = stateTable['^'] = stateTable['%'] = 12;
    stateTable['+'] = stateTable['-'] = stateTable['|'] = stateTable['&'] = 13;
    stateTable['>'] = stateTable['<'] = 14;
    stateTable['\''] = stateTable['"'] = 16;
    //��ս����
    memset(delimiters, false, 128 * sizeof(bool));
    //���ý��ת��
    delimiters['{'] = delimiters['}'] = delimiters['['] = delimiters[']'] = delimiters['('] = delimiters[')'] = delimiters[';'] = delimiters['.'] = true;
}
