#include"lex.h"


//初始化部分表
int Lex::stateTable[128] = { ERROR };

unordered_set<string> Lex::keywords = { "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "_Bool" };

bool Lex::delimiters[128] = { false };


Lex::Lex(string& fileName) : f(fileName)
{
    //打开文件出错
    if (!f)
        throw ios_base::failure("Failed in opening file.");

    //清空缓存区
    memset(buffer, 0, BUFSIZE * sizeof(char));
    //读前一半
    f.read(buffer, BUFSIZE);
    linesCount = (buffer[0] == END ? 0 : 1);
    buffer[BUFSIZE] = buffer[2 * BUFSIZE + 1] = END;
    initTable();
}

Lex::~Lex()
{
    f.close();
}

void Lex::run()
{
    //当前字符
    char c;
    //判断结束
    bool end = false;
    //自动机初始状态为0
    int state = 0;

    while (!end)
    {
        c = nextChar();

        //根据状态转换
        switch (state)
        {
            //初始状态
        case 0:
            //处理空白字符
            if (isspace(c))
            {
                if (c == '\n')
                    ++linesCount;
                continue;
            }
            else
            {
                //清空word
                word.clear();
                word += c;
                if (delimiters[c])
                    addLexeme(word, DELIMITER);
                else if (c == END)
                    end = true;
                //根据状态表转换
                else
                    state = stateTable[c];
            }
            break;

            //处理标识符/关键字/sizeof运算符
        case 1:
            if (!isalnum(c))
            {
                rollback();
                string ans;
                if (keywords.count(word))
                    ans = KEYWORD;
                //处理sizeof运算符
                else if (word == SIZEOF)
                    ans = OPERATOR;
                else
                    ans = ID;
                addLexeme(word, ans);

                state = 0;
            }
            break;

            //处理常数部分
        case 2:
            if (!isdigit(c))
            {
                if (c == '.')
                    state = 3;
                else if (c == 'e' || c == 'E')
                    state = 5;
                else if (canSkip(c))
                {
                    rollback();
                    addLexeme(word, NUMBER);
                    state = 0;
                }
                else
                    state = ERROR;
            }
            break;

            //处理常数部分
        case 3:
            if (isdigit(c))
                state = 4;
            else
                state = ERROR;
            break;

            //处理常数部分
        case 4:
            if (!isdigit(c))
            {
                if (c == 'e' || c == 'E')
                    state = 5;
                else if (canSkip(c))
                {
                    rollback();
                    addLexeme(word, NUMBER);
                    state = 0;
                }
                else
                    state = ERROR;
            }
            break;

            //处理常数部分
        case 5:
            if (isdigit(c))
                state = 7;
            else if (c == '+' || c == '-')
                state = 6;
            else
                state = ERROR;
            break;

            //处理常数部分
        case 6:
            if (isdigit(c))
                state = 7;
            else
                state = ERROR;
            break;

            //处理常数部分
        case 7:
            if (!isdigit(c))
            {
                if (canSkip(c))
                {
                    rollback();
                    addLexeme(word, NUMBER);
                    state = 0;
                }
                else
                    state = ERROR;
            }
            break;

            //处理/ /= // /* 部分
        case 8:
            if (c == '=')
            {
                addLexeme(word, OPERATOR);
                state = 0;
            }
            else if (c == '/')
                state = 9;
            else if (c == '*')
                state = 10;
            else
            {
                rollback();
                addLexeme(word, OPERATOR);
                state = 0;
            }
            break;

            //处理/ /= // /* 部分
        case 9:
            if (c == '\n' || c == END)
            {
                state = 0;
                rollback();
            }
            break;

            //处理/ /= // /* 部分
        case 10:
            if (c == '*')
                state = 11;
            else if (c == '\n')
                ++linesCount;
            else if (c == END)
                state = ERROR;
            break;

            //处理/ /= // /* 部分
        case 11:
            state = (c == '/' ? 0 : 10);
            break;

            //处理例如* *=等一系列最长长度为2的运算符
        case 12:
            if (c != '=')
                rollback();
            addLexeme(word, OPERATOR);
            state = 0;
            break;

            //处理例如+ ++ +=等一系列最长长度为2且共有3个的运算符
        case 13:
            if (c != word[0] && c != '=')
                rollback();
            addLexeme(word, OPERATOR);
            state = 0;
            break;

            //处理例如> >> >= >>= 等一系列最长长度为3的运算符 部分
        case 14:
            if (c == word[0])
                state = 15;
            else
            {
                if (c != '=')
                    rollback();
                addLexeme(word, OPERATOR);
                state = 0;
            }
            break;

            //处理例如> >> >= >>= 等一系列最长长度为3的运算符 部分
        case 15:
            if (c != '=')
                rollback();
            addLexeme(word, OPERATOR);
            state = 0;
            break;

            //错误状态
        default:
            if (c == END)
                end = true;
            handleError(c);
            state = 0;
            break;
        }
    }
}

void Lex::showResult()
{
    //输出错误
    cout << "Errors : " << errors.size() << endl;
    for (auto& error : errors)
        cout << "Error:\"" << error.first << "\" in lines " << error.second << endl;

    //输出统计信息
    cout << "Lines : " << linesCount << endl << "Characters : " << charsCount << endl << "Identifiers : " << typesCount[ID] << endl;

    cout << "Keywords : " << typesCount[KEYWORD] << endl << "Numbers : " << typesCount[NUMBER] << endl;

    cout << "Operators : " << typesCount[OPERATOR] << endl << "Delimiters : " << typesCount[DELIMITER] << endl;
}


void Lex::initTable()
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

    //清空界符表
    memset(delimiters, false, 128 * sizeof(bool));
    //设置界符转换
    delimiters['{'] = delimiters['}'] = delimiters['['] = delimiters[']'] = delimiters['('] = delimiters[')'] = delimiters['\\'] = delimiters['"'] = delimiters[';'] = true;
}

inline void Lex::addLexeme(string& s, const string& type)
{
    //若s第一次出现
    if (!lexemes.count(s))
        //若type为常数或标识符，则标志为常数或标识符，否则标志为该符号
        lexemes[s] = (type == NUMBER || type == ID ? type : s);
    ++typesCount[type];
}

char Lex::nextChar()
{
    if (buffer[cur] == END)
    {
        //指向左半边
        if (cur == BUFSIZE)
        {
            //填充右半边
            f.read(buffer + BUFSIZE + 1, BUFSIZE);
            ++cur;
        }
        //指向右半边
        else if (cur == 2 * BUFSIZE + 1)
        {
            //填充左半边
            f.read(buffer, BUFSIZE);
            cur = 0;
        }
    }
    //否则增加字符数
    else
        ++charsCount;
    //将此字符添加到word中
    word += buffer[cur];
    return buffer[cur++];
}

void Lex::rollback()
{
    //此时cur指向缓冲区边界，无法回滚
    if (cur == 0 || cur == BUFSIZE + 1)
    {
        cerr << "Rollback too many chars." << endl;
        exit(-1);
    }
    //若不是结束符，则字符数-1
    if (buffer[--cur] != END)
        --charsCount;
    //从word中删除当前字符
    word.pop_back();
}

void Lex::handleError(char c)
{
    rollback();
    errors.emplace_back(word, linesCount);
    //不停读入下个字符直到遇到空白字符/界符/结束符
    while (!canSkip(c))
        c = nextChar();
    //将t添加到errors中
}

inline bool Lex::canSkip(char c)
{
    return isspace(c) || delimiters[c] || c == END;
}
