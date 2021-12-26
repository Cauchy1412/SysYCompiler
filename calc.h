#include <bits/stdc++.h>
#include "define.h"
using namespace std;
#pragma once

int getValue(string name, vector<int> dims)
{
    symtab sym = symtab();
    int len = dims.size();
    bool find = false;
    vector<int> valArray;
    for (auto i : localSymbolTable)
    {
        if (i.IDENFRName == name && find == false)
        {
            sym = i;
            //return i.value;
            find = true;
        }
    }
    for (auto i : globalSymbolTable)
    {
        if (i.IDENFRName == name && find == false)
        {
            sym = i;
            //return i.value;
            find = true;
        }
    }
    if (find == false)
    {
        cout << "Can't find " << name << "!" << endl;
        return -1;
    }
    valArray = sym.elementArray;
    if (len == 0)
    {
        return sym.value;
    }
    else if (len == 1)
    {
        int l1 = dims[0];
        return valArray[l1];
    }
    else if (len == 2)
    {
        int l1 = dims[0];
        int l2 = dims[1];
        int index = l1 * sym.dimension2 + l2;
        return valArray[index];
    }
    return -1;
}

int getThetaIndex(char theta) //获取theta所对应的索引
{
    int index = 0;
    switch (theta)
    {
    case '!':
        index = 0;
        break;
    case '+':
        index = 1;
        break;
    case '-':
        index = 2;
        break;
    case '*':
        index = 3;
        break;
    case '/':
        index = 4;
        break;
    case '%':
        index = 5;
        break;
    case '(':
        index = 6;
        break;
    case ')':
        index = 7;
        break;
    case '#':
        index = 8;
    default:
        break;
    }
    return index;
}

char getThetaPriority(char theta1, char theta2) //获取theta1与theta2之间的优先级
{
    const char priority[][9] = //算符间的优先级关系
        {
            {'>', '>', '>', '>', '>', '>', '<', '>', '>'},
            {'<', '>', '>', '<', '<', '<', '<', '>', '>'},
            {'<', '>', '>', '<', '<', '<', '<', '>', '>'},
            {'<', '>', '>', '>', '>', '>', '<', '>', '>'},
            {'<', '>', '>', '>', '>', '>', '<', '>', '>'},
            {'<', '>', '>', '>', '>', '>', '<', '>', '>'},
            {'<', '<', '<', '<', '<', '<', '<', '=', '0'},
            {'>', '>', '>', '>', '>', '>', '0', '>', '>'},
            {'<', '<', '<', '<', '<', '<', '<', '0', '='},
        };

    int index1 = getThetaIndex(theta1);
    int index2 = getThetaIndex(theta2);
    return priority[index1][index2];
}

int thetaCal() //计算b theta a
{
    char theta = opter.top();
    opter.pop();
    int a, b;
    a = opval.top();
    opval.pop();
    if (theta != '!')
    {
        b = opval.top();
        opval.pop();
    }
    switch (theta)
    {
    case '!':
        return -a;
    case '+':
        return b + a;
    case '-':
        return b - a;
    case '*':
        return b * a;
    case '/':
        return b / a;
    case '%':
        return b % a;
    default:
        return 0;
        break;
    }
}

int getCalResult(string str) //表达式求值
{
    bool calFlag = false;
    int sign = 1;
    str.append("#");
    int index = 0;
    while (!opter.empty())
        opter.pop();
    while (!opval.empty())
        opval.pop();
    opter.push('#'); //首先将'#'入栈opter
    int counter = 0; //添加变量counter表示有多少个数字相继入栈，实现多位数的四则运算
    char c = str[index++];
    while (c != '#' || opter.top() != '#') //终止条件
    {
        if (isdigit(c)) //如果c在'0'~'9'之间
        {
            if (counter == 1) //counter==1表示上一字符也是数字，所以要合并，比如12*12，要算12，而不是单独的1和2
            {
                int t = opval.top();
                opval.pop();
                opval.push(t * 10 + (c - '0'));
                counter = 1;
            }
            else
            {
                opval.push((c - '0')); //将c对应的数值入栈opval
                counter++;
            }
            c = str[index++];
            calFlag = true;
        }
        else if (isalpha(c) || c == '_')
        {
            counter = 0;
            string name = "";
            name += c;
            c = str[index++];
            vector<int> dims;
            while (isalpha(c) || c == '_' || isdigit(c))
            {
                name += c;
                c = str[index++];
            }
            while (c == '[')
            {
                c = str[index++];
                string dimStr = "";
                while (c != ']')
                {
                    dimStr += c;
                    c = str[index++];
                }
                int dimNum = getCalResult(dimStr);
                dims.push_back(dimNum);
                c = str[index++];
            }
            index--;
            int symVal = getValue(name, dims);
            opval.push(symVal);
            c = str[index++];
            calFlag = true;
        }
        else
        {
            counter = 0; //counter置零
            if (calFlag == false && (c == '+' || c == '-'))
            {
                if (c == '+')
                    sign = sign;
                else if (c == '-')
                    sign = -sign;
                if (str[index] != '+' && str[index] != '-')
                {
                    if (sign == -1)
                    {
                        c = '!';
                        switch (getThetaPriority(opter.top(), c)) //获取运算符栈opter栈顶元素与c之间的优先级，用'>'，'<'，'='表示
                        {
                        case '<': //<则将c入栈opter
                            opter.push(c);
                            c = str[index++];
                            break;
                        case '=': //=将opter栈顶元素弹出，用于括号的处理
                            opter.pop();
                            c = str[index++];
                            break;
                        case '>': //>则计算
                            opval.push(thetaCal());
                        }
                        sign = 1;
                    }
                    else
                        c = str[index++];
                    calFlag = true;
                }
                else
                    c = str[index++];
            }
            else
            {
                switch (getThetaPriority(opter.top(), c)) //获取运算符栈opter栈顶元素与c之间的优先级，用'>'，'<'，'='表示
                {
                case '<': //<则将c入栈opter
                    calFlag = false;
                    opter.push(c);
                    c = str[index++];
                    break;
                case '=': //=将opter栈顶元素弹出，用于括号的处理
                    opter.pop();
                    c = str[index++];
                    break;
                case '>': //>则计算
                    opval.push(thetaCal());
                }
            }
        }
    }
    return opval.top(); //返回opval栈顶元素的值
}