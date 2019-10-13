#ifndef __COMMON_H__

#define __COMMON_H__

#include<string>

using namespace std;

//��������С
const size_t BUFSIZE = 4096;
//����״̬
const int ERROR = INT_MIN;
//�������ڱ���־
const char END = '\0';

//��־word����
const string ID = "identifier";

const string KEYWORD = "keyword";

const string NUMBER = "number";

//�ַ�/�ַ������� ���� char *a="hello world"
const string STRING = "string";

const string OPERATOR = "operator";

const string DELIMITER = "delimiter";

//���༯
const string CONSTTABLE[] = { ID, KEYWORD, NUMBER, STRING, OPERATOR, DELIMITER };

//����sizeof�����
const string SIZEOF = "sizeof";

//����ת��� ����ʶ���ַ�/�ַ��������е�\'
const char ESCAPE = '\\';

#endif
