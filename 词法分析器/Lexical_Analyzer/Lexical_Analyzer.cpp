#include "Lexical_Analyzer.h"

using std::cout;
using std::vector;
using namespace std;

// �ж��Ƿ�Ϊ��ʶ��
bool isID(const std::string& str)
{
    if (std::isdigit(str[0])) // ������ַ������֣����Ǳ�ʶ��
        return false;
    int counter = 0;
    if (str[0] == '_') // ������ַ����»��ߣ����������1
        counter++;

    for (; counter < str.size(); counter++)
        if (!(isalnum(str[counter]))) // ����ַ��Ȳ�����ĸҲ�������֣����Ǳ�ʶ��
            return false;//isalnum����ַ����Ƿ�����ĸ���������

    return true;
}

// �ж��Ƿ�Ϊע��
bool isComment(const std::string& str)
{
    return str == "/*" || str == "//";
}

// �ж��Ƿ�ΪԤ�������
bool isPretreatment(const std::string& str) 
{
    return str == "#";
}

// �ж��Ƿ�Ϊ����
bool isDigit(const std::string& str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit); // �ж��ַ����е��ַ��Ƿ�������
}

// �ж��Ƿ�Ϊ�ַ���
bool isString(const std::string& str)
{
    return str[0] == '"' && str[str.size() - 1] == '"';
}

// �ж��Ƿ�Ϊ����ֵ
bool isBool(const std::string& str)
{
    return str == "true" || str == "false" || str == "TRUE" || str == "FALSE";
}

// �ж��Ƿ�Ϊ�����������֡��ַ����򲼶�ֵ��
bool isLiteral(const std::string& str)
{
    return isDigit(str) || isString(str) || isBool(str);
}

// �ж��Ƿ�Ϊ�ؼ���
bool isKeyword(const std::string& str)
{
    const vector<std::string> keywords{ "int", "float", "auto", "double", "do", "switch", "return","case","const","if"};
    for (const auto& keyword : keywords)
        if (keyword == str)
            return true;

    return false;
}

// �ж��Ƿ�Ϊ���
bool isStatement(const std::string& str)
{
    const vector<std::string> statements{ "for", "while"};
    for (const auto& statement : statements)
        if (statement == str)
            return true;

    return false;
}

// �ж��Ƿ�Ϊ������
bool isOperator(const std::string& str)
{
    const vector<std::string> operators{ "<", ">", "<=", ">=", "*","&" ,"+", "-", "/", "=", "-=", "*=", "+=", "/=", "++", "--", "==","||"};
    for (const auto& op : operators)
        if (op == str)
            return true;

    return false;
}

// �ж��Ƿ�Ϊ�ָ���
bool isSeparator(const std::string& str)
{
    const vector<std::string> Separators{ "{", "}", ",", "(", ")", ";" ,":"};
    for (const auto& separate : Separators)
        if (separate == str)
            return true;

    return false;
}

// �ж��Ƿ�Ϊ�Ƿ��ַ�����Ч�ַ�
bool isNotLegal(const std::string& str)
{
    return str == " " || str == "\n";
}

// ���ݱ�ǵ���������������Ĵʷ���Ԫ���
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

// ���ļ����дʷ�����
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

    bool miltiCm = false;//����ע��
    bool singleCm = false;//����ע��
    while (file >> std::noskipws >> ch)//��ʹ��std::noskipws ������ʱ�������������������հ��ַ�
    {   
        if (singleCm || miltiCm)
        {
            if (singleCm && ch == '\n')//һֱ����ע����һ�л���
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