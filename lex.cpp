#include"lex.h"

Lex::Lex(string& fileName, Tables& _table) : f(fileName), tables(_table)
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
    initTypes();
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
                {
                    ++linesCount;
                    line.clear();
                }
                continue;
            }
            else
            {
                //���word
                word.clear();
                word += c;
                if (tables.isDelimiter(c))
                    addLexeme(word, DELIMITER);
                else if (c == END)
                    end = true;
                //����״̬��ת��
                else
                    state = tables.switchState(c);
            }
            break;

            //�����ʶ��/�ؼ���/sizeof�����
        case 1:
            if (!isalnum(c))
            {
                rollback();
                string ans;
                if (tables.isKeyword(word))
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

            //��������> >> >= >>= ��һϵ�������Ϊ3�������
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

            //��������> >> >= >>= ��һϵ�������Ϊ3�������
        case 15:
            if (c != '=')
                rollback();
            addLexeme(word, OPERATOR);
            state = 0;
            break;

            //�����ַ�/�ַ�������
        case 16:
            if (c == '\n')
            {
                ++linesCount;
                rollback();
                state = ERROR;
            }
            else if (c == END)
                state = ERROR;
            //��ǰ�ַ�Ϊ'��"����δ��ת��
            else if (c == word[0] && word[word.size() - 2] != ESCAPE)
            {
                //��Ŀǰ����Ϊ�ַ�����ʱ�����жϵ���������һ���ַ�����'\n'����Ϊ3
                if (word[0] == '\'' && word.size() != 3)
                    state = ERROR;
                else
                {
                    addLexeme(word, STRING);
                    state = 0;
                }
            }
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
    for (Error& error : errors)
        cout << "Error: " << error.word << " in lines " << error.lineNumber << " : " << error.line << endl;

    //���ͳ����Ϣ
    cout << "\nLines : " << linesCount << endl << "Characters : " << charsCount;
    for (const string& type : CONSTTABLE)
    {
        cout << endl << type << " : " << types[type]->size() << endl;
        for (lexeme& l : *types[type])
            cout << "{" << l.first << ',' << l.second << "}\n";
    }
}

void Lex::initTypes()
{
    types[ID] = &id_;
    types[KEYWORD] = &keyword_;
    types[NUMBER] = &number_;
    types[STRING] = &string_;
    types[OPERATOR] = &operator_;
    types[DELIMITER] = &delimiter_;
}

inline void Lex::addLexeme(string& s, const string& type)
{
    //��s��һ�γ���
    if (!lexemes.count(s))
    {
        //��typeΪ�������ʶ�������־Ϊ�������ʶ���������־Ϊ�÷���
        string value = (type == NUMBER || type == STRING || type == ID ? type : s);
        lexemes[s] = value;
        types[type]->emplace_back(s, value);
    }
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
    line += buffer[cur];
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
    line.pop_back();
}

void Lex::handleError(char c)
{
    rollback();

    Error t;
    t.word = word;
    t.lineNumber = linesCount;

    //��ͣ�����¸��ַ�ֱ�������հ��ַ�/���/������
    do
    {
        c = nextChar();
    } while (!canSkip(c));

    t.line = line;
    //��t��ӵ�errors��
    errors.emplace_back(t);
}

inline bool Lex::canSkip(char c)
{
    return isspace(c) || tables.isDelimiter(c) || c == END;
}
