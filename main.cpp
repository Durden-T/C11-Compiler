/*
基本要求： 1、文档（至少包括：运行/开发环境及版本号；设计、实现、测试的说明和描述；运行截图等） 2、源代码（含有必要的注释语句） 3、测试用例集（至少5个） 4、所有内容最终打包提交系统 特别提示： 1、一定按期提交实践报告，超过截止日期，系统将拒绝接受。 2、若是因为系统原因导致无法提交，请及时告知助教，并将包括学号、提交日期时间等在内的截图连同实践报告的压缩版，邮件发送zhang_yj@263.net，并在邮件主题注明 原因+学号+姓名+上机作业N（N为实际编号，比如第1次，即N为1） 3、若使用C/C++之外的其他实现语言，除了达到基本要求之外，请务必在文档中写明使用该语言实现的充分理由，并给出可执行文件。
msvc++14.1
*/
#include<string>
#include<iostream>

#include"lex.h"

using namespace std;

int main(int argc, char** argv)
{
    //判断参数是否存在
    if (argc == 1)
        throw ios_base::failure("Error:No parameter.");

    string filename = string(argv[1]);
    Lex lex(filename);

    lex.run();
    lex.showResult();

    return 0;
}


