#include <bits/stdc++.h>
#include "define.h"
using namespace std;
#pragma once

void getChar()
{
    if (isNewline())
    {
        line++;
    }
    indexNum++;
}

void clearToken()
{
    token.clear();
}

bool isCarriage()
{
    if (input[indexNum] == '\r')
        return true;
    else
        return false;
}

bool isSpace()
{
    if (input[indexNum] == ' ')
        return true;
    else
        return false;
}

bool isNewline()
{
    if (input[indexNum] == '\n')
        return true;
    else
        return false;
}

bool isTab()
{
    if (input[indexNum] == '\t')
        return true;
    else
        return false;
}

bool isLetter()
{
    if ((input[indexNum] >= 'A') && (input[indexNum] <= 'Z') || (input[indexNum] >= 'a') && (input[indexNum] <= 'z'))
        return true;
    else
        return false;
}

bool isDigit()
{
    if ((input[indexNum] >= '0') && (input[indexNum] <= '9'))
        return true;
    else
        return false;
}

bool isColon()
{
    if (input[indexNum] == ':')
        return true;
    else
        return false;
}

bool isComma()
{
    if (input[indexNum] == ',')
        return true;
    else
        return false;
}

bool isSemi()
{
    if (input[indexNum] == ';')
        return true;
    else
        return false;
}

bool isEqu()
{
    if (input[indexNum] == '=')
        return true;
    else
        return false;
}

bool isPlus()
{
    if (input[indexNum] == '+')
        return true;
    else
        return false;
}

bool isMinus()
{
    if (input[indexNum] == '-')
        return true;
    else
        return false;
}

bool isDiv()
{
    if (input[indexNum] == '/')
        return true;
    else
        return false;
}

bool isStar()
{
    if (input[indexNum] == '*')
        return true;
    else
        return false;
}

bool isMod()
{
    if (input[indexNum] == '%')
        return true;
    else
        return false;
}

bool isLpar()
{
    if (input[indexNum] == '(')
        return true;
    else
        return false;
}

bool isRpar()
{
    if (input[indexNum] == ')')
        return true;
    else
        return false;
}

void catToken()
{
    token = token + input[indexNum];
}

void retract()
{
    indexNum--;
}

int reserver()
{
    for (int i = 0; i < RESERVE_WORD_NUM; i++)
    {
        if (token == reserverWord[i])
            return i + 3;
    }
    return 0;
}

int transNum()
{
    return atoi(token.c_str());
}

void error()
{
    //
}

symbol getSym()
{
    clearToken();
    while (isSpace() || isNewline() || isTab() || isCarriage())
    {
        getChar();
    }
    if (isLetter() || (input[indexNum] == '_'))
    {
        while (isLetter() || isDigit() || (input[indexNum] == '_'))
        {
            catToken();
            getChar();
        }
        retract();
        int result = reserver();
        return symbol(result);
    }
    else if (input[indexNum] == '\"')
    {
        catToken();
        getChar();
        bool flag = false;
        while (input[indexNum] != '\"')
        {
            if (!((input[indexNum] == 32) || (input[indexNum] == 33) || (input[indexNum] == 37) || (input[indexNum] >= 40 && input[indexNum] <= 126)))
            {
                flag = true;
            }
            if ((input[indexNum] == '\\') && (input[indexNum + 1] != 'n'))
            {
                flag = true;
            }
            if ((input[indexNum] == '%') && (input[indexNum + 1] != 'd'))
            {
                flag = true;
            }
            catToken();
            getChar();
        }
        if (flag)
        {
            addError(line, 'a');
        }
        // while ((input[indexNum] == 32) || (input[indexNum] == 33) || (input[indexNum] == 37) || (input[indexNum] >= 40 && input[indexNum] <= 126))
        // {
        //     catToken();
        //     getChar();
        // }
        if (input[indexNum] == '\"')
        {
            catToken();
            return STRCON;
        }
    }
    else if (isDigit())
    {
        while (isDigit())
        {
            catToken();
            getChar();
        }
        retract();
        num = transNum();
        return INTCON;
    }
    else if (isPlus())
    {
        catToken();
        return PLUS;
    }
    else if (isMinus())
    {
        catToken();
        return MINU;
    }
    else if (isStar())
    {
        catToken();
        return MULT;
    }
    else if (isMod())
    {
        catToken();
        return MOD;
    }
    else if (isLpar())
    {
        catToken();
        return LPARENT;
    }
    else if (isRpar())
    {
        catToken();
        return RPARENT;
    }
    else if (input[indexNum] == '[')
    {
        catToken();
        return LBRACK;
    }
    else if (input[indexNum] == ']')
    {
        catToken();
        return RBRACK;
    }
    else if (input[indexNum] == '{')
    {
        catToken();
        return LBRACE;
    }
    else if (input[indexNum] == '}')
    {
        catToken();
        return RBRACE;
    }
    else if (isComma())
    {
        catToken();
        return COMMA;
    }
    else if (isSemi())
    {
        catToken();
        return SEMICN;
    }
    else if (isDiv())
    {
        catToken();
        getChar();
        if (isStar())
        {
            do
            {
                do
                {
                    getChar();
                } while (!isStar());
                do
                {
                    getChar();
                    if (isDiv())
                        return ANNOTATION;
                } while (isStar());

            } while (!isStar());
        }
        else if (isDiv())
        {
            do
            {
                getChar();
            } while (!isNewline());
            return ANNOTATION;
        }
        else
        {
            retract();
            return DIV;
        }
    }
    else if (input[indexNum] == '!')
    {
        catToken();
        getChar();
        if (input[indexNum] == '=')
        {
            catToken();
            return NEQ;
        }
        else
        {
            retract();
            return NOT;
        }
    }
    else if (input[indexNum] == '&')
    {
        catToken();
        getChar();
        if (input[indexNum] == '&')
        {
            catToken();
            return AND;
        }
    }
    else if (input[indexNum] == '|')
    {
        catToken();
        getChar();
        if (input[indexNum] == '|')
        {
            catToken();
            return OR;
        }
    }
    else if (input[indexNum] == '<')
    {
        catToken();
        getChar();
        if (input[indexNum] == '=')
        {
            catToken();
            return LEQ;
        }
        else
        {
            retract();
            return LSS;
        }
    }
    else if (input[indexNum] == '>')
    {
        catToken();
        getChar();
        if (input[indexNum] == '=')
        {
            catToken();
            return GEQ;
        }
        else
        {
            retract();
            return GRE;
        }
    }
    else if (input[indexNum] == '=')
    {
        catToken();
        getChar();
        if (input[indexNum] == '=')
        {
            catToken();
            return EQL;
        }
        else
        {
            retract();
            return ASSIGN;
        }
    }
    return NONE;
}

void printWordSymbol()
{
    if (isExp == true)
    {
        expStr.append(word[printWordIndex].content);
    }
    if (isDim == true)
    {
        dimStr.append(word[printWordIndex].content);
    }
    if (syntaxTry == false)
    {
        outFile << word[printWordIndex].symbol << " " << word[printWordIndex].content << endl;
        // cout << word[printWordIndex].symbol << " " << word[printWordIndex].content << " " << word[printWordIndex].line << endl;
        printWordIndex++;
    }
}