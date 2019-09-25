#include"lex.h"


//��ʼ�����ֱ�
int Lex::stateTable[128] = { ERROR };

unordered_set<string> Lex::keywords = { "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "_Bool" };

bool Lex::delimiters[128] = { false };


Lex::Lex(string& fileName) : f(fileName)
{
    //���ļ�����
    if (!f)
        throw ios_base::failure("Failed in opening file.");

    //��ջ�����
    memset(buffer, 0, BUFSIZE * sizeof(char));
    //��ǰһ��
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
    //��ǰ�ַ�
    char c;
    //�жϽ���
    bool end = false;
    //�Զ�����ʼ״̬Ϊ0
    int state = 0;

    while (!end)
    {
        c = nextChar();

        //����״̬ת��
        switch (state)
        {
            //��ʼ״̬
        case 0:
            //����հ��ַ�
            if (isspace(c))
            {
                if (c == '\n')
                    ++linesCount;
                continue;
            }
            else
            {
                //���word
                word.clear();
                word += c;
                if (delimiters[c])
                    addLexeme(word, DELIMITER);
                else if (c == END)
                    end = true;
                //����״̬��ת��
                else
                    state = stateTable[c];
            }
            break;

            //�����ʶ��/�ؼ���/sizeof�����
        case 1:
            if (!isalnum(c))
            {
                rollback();
                string ans;
                if (keywords.count(word))
                    ans = KEYWORD;
                //����sizeof�����
                else if (word == SIZEOF)
                    ans = OPERATOR;
                else
                    ans = ID;
                addLexeme(word, ans);

                state = 0;
            }
            break;

            //����������
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

            //����������
        case 3:
            if (isdigit(c))
                state = 4;
            else
                state = ERROR;
            break;

            //����������
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

            //����������
        case 5:
            if (isdigit(c))
                state = 7;
            else if (c == '+' || c == '-')
                state = 6;
            else
                state = ERROR;
            break;

            //����������
        case 6:
            if (isdigit(c))
                state = 7;
            else
                state = ERROR;
            break;

            //����������
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

            //����/ /= // /* ����
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

            //����/ /= // /* ����
        case 9:
            if (c == '\n' || c == END)
            {
                state = 0;
                rollback();
            }
            break;

            //����/ /= // /* ����
        case 10:
            if (c == '*')
                state = 11;
            else if (c == '\n')
                ++linesCount;
            else if (c == END)
                state = ERROR;
            break;

            //����/ /= // /* ����
        case 11:
            state = (c == '/' ? 0 : 10);
            break;

            //��������* *=��һϵ�������Ϊ2�������
        case 12:
            if (c != '=')
                rollback();
            addLexeme(word, OPERATOR);
            state = 0;
            break;

            //��������+ ++ +=��һϵ�������Ϊ2�ҹ���3���������
        case 13:
            if (c != word[0] && c != '=')
                rollback();
            addLexeme(word, OPERATOR);
            state = 0;
            break;

            //��������> >> >= >>= ��һϵ�������Ϊ3������� ����
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

            //��������> >> >= >>= ��һϵ�������Ϊ3������� ����
        case 15:
            if (c != '=')
                rollback();
            addLexeme(word, OPERATOR);
            state = 0;
            break;

            //����״̬
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
    //�������
    cout << "Errors : " << errors.size() << endl;
    for (auto& error : errors)
        cout << "Error:\"" << error.first << "\" in lines " << error.second << endl;

    //���ͳ����Ϣ
    cout << "Lines : " << linesCount << endl << "Characters : " << charsCount << endl << "Identifiers : " << typesCount[ID] << endl;

    cout << "Keywords : " << typesCount[KEYWORD] << endl << "Numbers : " << typesCount[NUMBER] << endl;

    cout << "Operators : " << typesCount[OPERATOR] << endl << "Delimiters : " << typesCount[DELIMITER] << endl;
}


void Lex::initTable()
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

    //��ս����
    memset(delimiters, false, 128 * sizeof(bool));
    //���ý��ת��
    delimiters['{'] = delimiters['}'] = delimiters['['] = delimiters[']'] = delimiters['('] = delimiters[')'] = delimiters['\\'] = delimiters['"'] = delimiters[';'] = true;
}

inline void Lex::addLexeme(string& s, const string& type)
{
    //��s��һ�γ���
    if (!lexemes.count(s))
        //��typeΪ�������ʶ�������־Ϊ�������ʶ���������־Ϊ�÷���
        lexemes[s] = (type == NUMBER || type == ID ? type : s);
    ++typesCount[type];
}

char Lex::nextChar()
{
    if (buffer[cur] == END)
    {
        //ָ������
        if (cur == BUFSIZE)
        {
            //����Ұ��
            f.read(buffer + BUFSIZE + 1, BUFSIZE);
            ++cur;
        }
        //ָ���Ұ��
        else if (cur == 2 * BUFSIZE + 1)
        {
            //�������
            f.read(buffer, BUFSIZE);
            cur = 0;
        }
    }
    //���������ַ���
    else
        ++charsCount;
    //�����ַ���ӵ�word��
    word += buffer[cur];
    return buffer[cur++];
}

void Lex::rollback()
{
    //��ʱcurָ�򻺳����߽磬�޷��ع�
    if (cur == 0 || cur == BUFSIZE + 1)
    {
        cerr << "Rollback too many chars." << endl;
        exit(-1);
    }
    //�����ǽ����������ַ���-1
    if (buffer[--cur] != END)
        --charsCount;
    //��word��ɾ����ǰ�ַ�
    word.pop_back();
}

void Lex::handleError(char c)
{
    rollback();
    errors.emplace_back(word, linesCount);
    //��ͣ�����¸��ַ�ֱ�������հ��ַ�/���/������
    while (!canSkip(c))
        c = nextChar();
    //��t��ӵ�errors��
}

inline bool Lex::canSkip(char c)
{
    return isspace(c) || delimiters[c] || c == END;
}
