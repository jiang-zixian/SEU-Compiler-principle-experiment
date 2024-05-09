#include "Lexical_Analyzer.h"

using std::cout;
using std::vector;
using namespace std;

// 判断是否为标识符
bool isID(const std::string& str)
{
    if (std::isdigit(str[0])) // 如果首字符是数字，则不是标识符
        return false;
    int counter = 0;
    if (str[0] == '_') // 如果首字符是下划线，则计数器加1
        counter++;

    for (; counter < str.size(); counter++)
        if (!(isalnum(str[counter]))) // 如果字符既不是字母也不是数字，则不是标识符
            return false;//isalnum检测字符串是否由字母和数字组成

    return true;
}

// 判断是否为注释
bool isComment(const std::string& str)
{
    return str == "/*" || str == "//";
}

// 判断是否为预处理语句
bool isPretreatment(const std::string& str) 
{
    return str == "#";
}

// 判断是否为数字
bool isDigit(const std::string& str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit); // 判断字符串中的字符是否都是数字
}

// 判断是否为字符串
bool isString(const std::string& str)
{
    return str[0] == '"' && str[str.size() - 1] == '"';
}

// 判断是否为布尔值
bool isBool(const std::string& str)
{
    return str == "true" || str == "false" || str == "TRUE" || str == "FALSE";
}

// 判断是否为字面量（数字、字符串或布尔值）
bool isLiteral(const std::string& str)
{
    return isDigit(str) || isString(str) || isBool(str);
}

// 判断是否为关键字
bool isKeyword(const std::string& str)
{
    const vector<std::string> keywords{ "int", "float", "auto", "double", "do", "switch", "return","case","const","if"};
    for (const auto& keyword : keywords)
        if (keyword == str)
            return true;

    return false;
}

// 判断是否为语句
bool isStatement(const std::string& str)
{
    const vector<std::string> statements{ "for", "while"};
    for (const auto& statement : statements)
        if (statement == str)
            return true;

    return false;
}

// 判断是否为操作符
bool isOperator(const std::string& str)
{
    const vector<std::string> operators{ "<", ">", "<=", ">=", "*","&" ,"+", "-", "/", "=", "-=", "*=", "+=", "/=", "++", "--", "==","||"};
    for (const auto& op : operators)
        if (op == str)
            return true;

    return false;
}

// 判断是否为分隔符
bool isSeparator(const std::string& str)
{
    const vector<std::string> Separators{ "{", "}", ",", "(", ")", ";" ,":"};
    for (const auto& separate : Separators)
        if (separate == str)
            return true;

    return false;
}

// 判断是否为非法字符或无效字符
bool isNotLegal(const std::string& str)
{
    return str == " " || str == "\n";
}

// 根据标记的类型输出其所属的词法单元类别
void printToken(const std::string& token)
{
    if (isOperator(token))
        cout << token << "\t\toperator" << endl;
    else if(isPretreatment(std::string(1, token[0])))
        cout << token << "\t\tpretreatment" << endl;
    else if (isSeparator(token))
        cout << token << "\t\tseparator" << endl;
    else if (isKeyword(token))
        cout << token << "\t\tkeyword" << endl;
    else if (isStatement(token))
        cout << token << "\t\tstatement" << endl;
    else if (isLiteral(token))
    {
        if (isDigit(token)) {
            cout << token << "\t\tliteral:digit" << endl;
        }
        else if (isString(token)) {
            cout << token << "\t\tliteral:string" << endl;
        }
        else {
            cout << token << "\t\tliteral:bool" << endl;
        }
    }  
    else if (isID(token))
        cout << token << "\t\tidentifier" << endl;
    else if (isComment(token))
        cout << token << "\t\tcomment" << endl;
    else
        throw std::runtime_error("Invalid token: " + token);
}

// 对文件进行词法分析
void lexicalAnalyze(const std::string& nameOfFile)
{
    char ch;
    std::string buffer;
    std::fstream file(nameOfFile, std::fstream::in);

    if (!file.is_open())
    {
        cout << "error while opening the file\n";
        exit(0);
    }

    bool miltiCm = false;//多行注释
    bool singleCm = false;//单行注释
    while (file >> std::noskipws >> ch)//当使用std::noskipws 操作符时，输入流将不会跳过空白字符
    {   
        if (singleCm || miltiCm)
        {
            if (singleCm && ch == '\n')//一直读到注释这一行换行
                singleCm = false;

            if (miltiCm && ch == '*')
            {
                file >> ch;
                if (ch == EOF)
                    break;

                if (ch == '/')
                    miltiCm = false;
            }
            continue;
        }

        if (ch == '/')
        {
            std::string comm(1, ch);
            file >> ch;
            if (ch == EOF)
            {
                printToken(comm);
                break;
            }

            if (ch == '*')
            {
                miltiCm = true;
                comm += ch;
            }
            else if (ch == '/')
            {
                singleCm = true;
                comm += ch;
            }

            //if (miltiCm || singleCm)
            //{
                printToken(comm);
                continue;
           // }
        }

        if (isNotLegal(std::string(1, ch)))
        {
            if (!buffer.empty())
            {
                printToken(buffer);
                buffer = "";
            }
            continue;
        }

        if (isOperator(std::string(1, ch)) && !isOperator(buffer))
        {
            if (!buffer.empty())
            {
                printToken(buffer);
                buffer = "";
            }
        }

        if (!isOperator(std::string(1, ch)) && isOperator(buffer))
        {
            printToken(buffer);
            buffer = "";
        }

        if (isSeparator(std::string(1, ch)))
        {
            if (!buffer.empty())
            {
                printToken(buffer);
                buffer = "";
            }
            if (isSeparator(std::string(1, ch)))
            {
                printToken(std::string(1, ch));
                continue;
            }
        }
        buffer += ch;
    }
    file.close();
}