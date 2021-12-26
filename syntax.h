#include <bits/stdc++.h>
#include "define.h"
using namespace std;
#pragma once

void syntaxCompUnit()
{
    while (nextSym == "CONSTTK" || (isBType() && word[indexNum].symbol == "IDENFR" && word[indexNum + 1].symbol != "LPARENT"))
    {
        isDecl = true;
        syntaxDecl();
        isDecl = false;
    }
    while (nextSym == "VOIDTK" || (isBType() && word[indexNum].symbol == "IDENFR" && word[indexNum + 1].symbol == "LPARENT"))
    {
        syntaxFuncDef();
    }
    if (nextSym == "INTTK" && word[indexNum].symbol == "MAINTK")
    {
        syntaxMainFuncDef();
    }
    if (syntaxTry == false)
        outFile << '<' << "CompUnit" << '>' << endl;
}

void syntaxMainFuncDef()
{
    currentReturnBlock = blockNum + 1;
    int errorLine = 0;
    scope = Local;
    if (nextSym == "INTTK")
    {
        funcReturnTypes.push_back(IntType);
        //currentFuncReturnType = IntType;
        printWordSymbol();
        getNextSym();
        if (nextSym == "MAINTK")
        {
            addMidCode(OpMain, symtab("main", FuncType, IntType));
            printWordSymbol();
            getNextSym();
            if (nextSym == "LPARENT")
            {
                printWordSymbol();
                getNextSym();
                if (nextSym == "RPARENT")
                {
                    printWordSymbol();
                    getNextSym();
                }
                else
                {
                    int errorLine = word[indexNum - 2].line;
                    addError(errorLine, 'j');
                }
                syntaxBlock();
            }
        }
        funcReturnTypes.pop_back();
    }
    errorLine = word[indexNum - 2].line;
    if (funcHaveReturn == false)
        addError(errorLine, 'g');
    funcHaveReturn = false;
    if (syntaxTry == false)
        outFile << '<' << "MainFuncDef" << '>' << endl;
}

void syntaxDecl()
{
    if (nextSym == "CONSTTK")
    {
        isConst = true;
        syntaxConstDecl();
        isConst = false;
    }
    else if (isBType())
    {
        syntaxVarDecl();
    }
}

void syntaxConstDecl()
{
    if (nextSym == "CONSTTK")
    {
        printWordSymbol();
        getNextSym();
        if (isBType())
        {
            printWordSymbol();
            getNextSym();
            syntaxConstDef();
            while (nextSym == "COMMA")
            {
                printWordSymbol();
                getNextSym();
                syntaxConstDef();
            }
            if (nextSym == "SEMICN")
            {
                printWordSymbol();
                getNextSym();
            }
            else
            {
                int errorLine = word[indexNum - 2].line;
                addError(errorLine, 'i');
            }
        }
    }
    if (syntaxTry == false)
        outFile << '<' << "ConstDecl" << '>' << endl;
}

void syntaxConstDef()
{
    isDef = true;
    string constName;
    symTabItemType itemType = ConstType;
    symTabValType valType = NoneValType;
    int dimNum = 0;
    int errorLine = 0;
    if (nextSym == "IDENFR")
    {
        constName = word[indexNum - 1].content;
        errorLine = word[indexNum - 1].line;
        printWordSymbol();
        getNextSym();
        while (nextSym == "LBRACK")
        {
            dimNum++;
            printWordSymbol();
            getNextSym();
            isDim = true;
            syntaxConstExp();
            cleardimStr();
            isDim = false;
            if (nextSym == "RBRACK")
            {
                printWordSymbol();
                getNextSym();
            }
            else
            {
                int errorLine = word[indexNum - 2].line;
                addError(errorLine, 'k');
            }
        }
        if (nextSym == "ASSIGN")
        {
            isAssign = true;
            printWordSymbol();
            getNextSym();
            isInit = true;
            syntaxConstInitVal();
            isInit = false;
        }
    }
    if (dimNum == 0)
        valType = IntType;
    else if (dimNum == 1)
        valType = OneDimType;
    else if (dimNum == 2)
        valType = TwoDimType;
    bool flag = insert(constName, itemType, valType);
    symtab OpSym1 = symtab();
    OpSym1 = findSymbol(constName);
    if (isDecl == false)
    {
        if (isAssign == false)
        {
            symtab tempSym1 = symtab("ConstInt0!", 0, INTGEN);
            vector<symtab> tempList(OpSym1.len, tempSym1);
            initList = tempList;
        }
        OpSym1.initArray = initList;
        addMidCode(OpArrayInit, OpSym1);
    }
    else
    {
        addMidCode(OpDeclare, OpSym1);
    }
    initList.clear();
    if (flag == false)
        addError(errorLine, 'b');
    if (syntaxTry == false)
        outFile << '<' << "ConstDef" << '>' << endl;
    isDef = false;
    isAssign = false;
}

void syntaxConstInitVal()
{
    if (nextSym != "LBRACE")
    {
        isExp = true;
        syntaxConstExp();
        if (isInit)
            initList.push_back(currentSym);
        isExp = false;
        clearExpStr();
    }
    else if (nextSym == "LBRACE")
    {
        printWordSymbol();
        getNextSym();
        if (nextSym != "RBRACE")
        {
            syntaxConstInitVal();
            while (nextSym == "COMMA")
            {
                printWordSymbol();
                getNextSym();
                syntaxConstInitVal();
            }
        }
        if (nextSym == "RBRACE")
        {
            printWordSymbol();
            getNextSym();
        }
    }
    if (syntaxTry == false)
        outFile << '<' << "ConstInitVal" << '>' << endl;
}

void syntaxConstExp()
{
    syntaxAddExp();
    if (syntaxTry == false)
        outFile << '<' << "ConstExp" << '>' << endl;
}

void syntaxVarDecl()
{
    if (isBType())
    {
        printWordSymbol();
        getNextSym();
        syntaxVarDef();
        while (nextSym == "COMMA")
        {
            printWordSymbol();
            getNextSym();
            syntaxVarDef();
        }
        if (nextSym == "SEMICN")
        {
            printWordSymbol();
            getNextSym();
        }
        else
        {
            int errorLine = word[indexNum - 2].line;
            addError(errorLine, 'i');
        }
    }
    if (syntaxTry == false)
        outFile << '<' << "VarDecl" << '>' << endl;
}

void syntaxVarDef()
{
    isDef = true;
    string varName;
    symTabItemType itemType = VarType;
    symTabValType valType = NoneValType;
    int dimNum = 0;
    int errorLine = 0;
    if (nextSym == "IDENFR")
    {
        varName = word[indexNum - 1].content;
        errorLine = word[indexNum - 1].line;
        printWordSymbol();
        getNextSym();
        while (nextSym == "LBRACK")
        {
            dimNum++;
            printWordSymbol();
            getNextSym();
            isDim = true;
            syntaxConstExp();
            cleardimStr();
            isDim = false;
            if (nextSym == "RBRACK")
            {
                printWordSymbol();
                getNextSym();
            }
            else
            {
                int errorLine = word[indexNum - 2].line;
                addError(errorLine, 'k');
            }
        }
        if (nextSym == "ASSIGN")
        {
            isAssign = true;
            printWordSymbol();
            getNextSym();
            isInit = true;
            syntaxInitVal();
            isInit = false;
        }
    }
    if (dimNum == 0)
        valType = IntType;
    else if (dimNum == 1)
        valType = OneDimType;
    else if (dimNum == 2)
        valType = TwoDimType;
    bool flag = insert(varName, itemType, valType);
    symtab OpSym1 = symtab();
    OpSym1 = findSymbol(varName);
    if (isDecl == false)
    {
        if (isAssign == false)
        {
            symtab tempSym1 = symtab("ConstInt0!", 0, INTGEN);
            vector<symtab> tempList(OpSym1.len, tempSym1);
            initList = tempList;
        }
        OpSym1.initArray = initList;
        addMidCode(OpArrayInit, OpSym1);
    }
    else
    {
        addMidCode(OpDeclare, OpSym1);
    }
    initList.clear();
    if (flag == false)
        addError(errorLine, 'b');
    if (syntaxTry == false)
        outFile << '<' << "VarDef" << '>' << endl;
    isDef = false;
    isAssign = false;
}

void syntaxInitVal()
{
    if (nextSym != "LBRACE")
    {
        isExp = true;
        syntaxExp();
        if (isInit)
            initList.push_back(currentSym);
        isExp = false;
        clearExpStr();
    }
    else if (nextSym == "LBRACE")
    {
        printWordSymbol();
        getNextSym();
        if (nextSym != "RBRACE")
        {
            syntaxInitVal();
            while (nextSym == "COMMA")
            {
                printWordSymbol();
                getNextSym();
                syntaxInitVal();
            }
        }
        if (nextSym == "RBRACE")
        {
            printWordSymbol();
            getNextSym();
        }
    }
    if (syntaxTry == false)
        outFile << '<' << "InitVal" << '>' << endl;
}

void syntaxFuncDef()
{
    currentReturnBlock = blockNum + 1;
    localUpdate();
    string funName;
    symTabItemType itemType = FuncType;
    symTabValType valType = NoneValType;
    symtab tempSym1 = symtab();
    int errorLine = 0;
    if (isBType() || nextSym == "VOIDTK")
    {
        if (isBType())
        {
            valType = IntType;
        }
        else if (nextSym == "VOIDTK")
        {
            valType = VoidType;
        }
        funcReturnTypes.push_back(valType);
        syntaxFuncType();
        if (nextSym == "IDENFR")
        {
            funName = word[indexNum - 1].content;
            errorLine = word[indexNum - 1].line;
            bool flag = insert(funName, itemType, valType);
            if (flag == false)
                addError(errorLine, 'b');
            printWordSymbol();
            getNextSym();
            scope = Local;
            if (nextSym == "LPARENT")
            {
                printWordSymbol();
                getNextSym();
                if (nextSym != "RPARENT")
                {
                    syntaxFuncFParams();
                }
                if (nextSym == "RPARENT")
                {
                    printWordSymbol();
                    getNextSym();
                }
                else
                {
                    int errorLine = word[indexNum - 2].line;
                    addError(errorLine, 'j');
                }
                tempSym1 = findSymbol(funName);
                if (syntaxTry == false)
                    addMidCode(OpFuncDeclare, tempSym1);
                syntaxBlock();
            }
            scope = Global;
        }
        funcReturnTypes.pop_back();
    }
    errorLine = word[indexNum - 2].line;
    if ((valType == IntType) && (funcHaveReturn == false))
        addError(errorLine, 'g');
    funcHaveReturn = false;
    localUpdate();
    currentReturnBlock = -1;
    symtab tempSym2 = tempSym1;
    tempSym2.ConditionType = FUNCENDTYPE;
    if (syntaxTry == false)
        addMidCode(OpLabel, tempSym2);
    if (syntaxTry == false)
        outFile << '<' << "FuncDef" << '>' << endl;
}

void syntaxFuncType()
{
    if (nextSym == "INTTK" || nextSym == "VOIDTK")
    {
        printWordSymbol();
        getNextSym();
    }
    if (syntaxTry == false)
        outFile << '<' << "FuncType" << '>' << endl;
}

void syntaxFuncFParams()
{
    blockNum++;
    blockNumVector.push_back(blockNum);
    syntaxFuncFParam();
    while (nextSym == "COMMA")
    {
        printWordSymbol();
        getNextSym();
        syntaxFuncFParam();
    }
    blockNum--;
    blockNumVector.pop_back();
    if (syntaxTry == false)
        outFile << '<' << "FuncFParams" << '>' << endl;
}

void syntaxFuncFParam()
{
    string paraName;
    symTabItemType itemType = ParaType;
    symTabValType valType = NoneValType;
    int dimNum = 0;
    int errorLine = 0;
    if (isBType())
    {
        printWordSymbol();
        getNextSym();
        if (nextSym == "IDENFR")
        {
            paraName = word[indexNum - 1].content;
            errorLine = word[indexNum - 1].line;
            printWordSymbol();
            getNextSym();
            if (nextSym == "LBRACK")
            {
                dimNum++;
                printWordSymbol();
                getNextSym();
                if (nextSym == "RBRACK")
                {
                    printWordSymbol();
                    getNextSym();
                }
                else
                {
                    int errorLine = word[indexNum - 2].line;
                    addError(errorLine, 'k');
                }
                while (nextSym == "LBRACK")
                {
                    dimNum++;
                    printWordSymbol();
                    getNextSym();
                    if (dimNum == 1)
                    {
                        syntaxConstExp();
                    }
                    else if (dimNum == 2)
                    {
                        isDim = true;
                        syntaxConstExp();
                        cleardimStr();
                        isDim = false;
                    }
                    if (nextSym == "RBRACK")
                    {
                        printWordSymbol();
                        getNextSym();
                    }
                    else
                    {
                        int errorLine = word[indexNum - 2].line;
                        addError(errorLine, 'k');
                    }
                }
            }
        }
    }
    if (dimNum == 0)
        valType = IntType;
    else if (dimNum == 1)
        valType = OneDimType;
    else if (dimNum == 2)
        valType = TwoDimType;
    bool flag = insert(paraName, itemType, valType);
    if (flag == false)
        addError(errorLine, 'b');
    if (syntaxTry == false)
        outFile << '<' << "FuncFParam" << '>' << endl;
}

void syntaxBlock()
{
    int blockNumBackUp = blockNum;
    blockNum++;
    blockNumVector.push_back(blockNum);
    int localSymTabSize = localSymbolTable.size();
    if (nextSym == "LBRACE")
    {
        printWordSymbol();
        getNextSym();
        while (nextSym != "RBRACE")
        {
            syntaxBlockItem();
        }
        if (nextSym == "RBRACE")
        {
            printWordSymbol();
            getNextSym();
        }
    }
    blockNumVector.pop_back();
    while ((localSymbolTable.size() > 0) && (localSymbolTable.back().block > blockNumBackUp))
    {
        localSymbolTable.pop_back();
    }
    // while (localSymbolTable.size() > localSymTabSize)
    // {
    //     localSymbolTable.pop_back();
    // }
    if (syntaxTry == false)
        outFile << '<' << "Block" << '>' << endl;
}

void syntaxBlockItem()
{
    if (nextSym == "CONSTTK" || isBType())
    {
        syntaxDecl();
    }
    else
    {
        syntaxStmt();
    }
}

void syntaxStmt()
{
    int errorLine = 0;
    symTabItemType itemType = NoneItemType;
    if (nextSym == "IDENFR" && (word[indexNum].symbol == "ASSIGN" || word[indexNum].symbol == "LBRACK"))
    {
        symtab sym = findSymbol(word[indexNum - 1].content);
        itemType = findItemTypeInAllSymTab(word[indexNum - 1].content);
        errorLine = word[indexNum - 1].line;
        saveNowEnv();
        syntaxLVal();
        int flag = (nextSym == "ASSIGN") ? 0 : 1;
        recoverEnv();
        if (flag == 0)
        {
            syntaxLVal();
            if (nextSym == "ASSIGN")
            {
                symtab tempSym1 = symtab();
                tempSym1 = currentSym;
                //cout << "???" << endl;
                if (itemType == ConstType)
                    addError(errorLine, 'h');
                printWordSymbol();
                getNextSym();
                if (nextSym != "GETINTTK")
                {
                    syntaxExp();
                    if (nextSym == "SEMICN")
                    {
                        printWordSymbol();
                        getNextSym();
                    }
                    else
                    {
                        errorLine = word[indexNum - 2].line;
                        addError(errorLine, 'i');
                    }
                }
                else if (nextSym == "GETINTTK")
                {
                    //addGenOp(OpGet);
                    symtab tempSym2 = symtab(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
                    addMidCode(OpGet, tempSym2);
                    currentSym = tempSym2;
                    printWordSymbol();
                    getNextSym();
                    if (nextSym == "LPARENT")
                    {
                        printWordSymbol();
                        getNextSym();
                        if (nextSym == "RPARENT")
                        {
                            printWordSymbol();
                            getNextSym();
                        }
                        else
                        {
                            errorLine = word[indexNum - 2].line;
                            addError(errorLine, 'j');
                        }
                        if (nextSym == "SEMICN")
                        {
                            printWordSymbol();
                            getNextSym();
                        }
                        else
                        {
                            errorLine = word[indexNum - 2].line;
                            addError(errorLine, 'i');
                        }
                    }
                }
                //?
                addMidCode(OpAssign, tempSym1, currentSym);
            }
        }
        else if (flag == 1)
        {
            syntaxExp();
            if (nextSym == "SEMICN")
            {
                printWordSymbol();
                getNextSym();
            }
            else
            {
                errorLine = word[indexNum - 2].line;
                addError(errorLine, 'i');
            }
        }
    }
    else if (nextSym == "LBRACE")
    {
        syntaxBlock();
    }
    else if (nextSym == "IFTK")
    {
        ifBlockNum++;
        int nowIfBlockNum = ifBlockNum;
        symtab tempSym1 = symtab("ifbegin" + to_string(nowIfBlockNum), nowIfBlockNum, IFBEGINTYPE);
        symtab tempSym2 = symtab("ifelse" + to_string(nowIfBlockNum), nowIfBlockNum, IFELSETYPE);
        symtab tempSym3 = symtab("ifend" + to_string(nowIfBlockNum), nowIfBlockNum, IFENDTYPE);
        addMidCode(OpLabel, tempSym1);
        printWordSymbol();
        getNextSym();
        if (nextSym == "LPARENT")
        {
            printWordSymbol();
            getNextSym();
            syntaxCond();
            if (nextSym == "RPARENT")
            {
                printWordSymbol();
                getNextSym();
            }
            else
            {
                errorLine = word[indexNum - 2].line;
                addError(errorLine, 'j');
            }
            addMidCode(OpBr, currentSym, tempSym2);
            syntaxStmt();
            addMidCode(OpJ, tempSym3);
            addMidCode(OpLabel, tempSym2);
            if (nextSym == "ELSETK")
            {
                printWordSymbol();
                getNextSym();
                syntaxStmt();
            }
            addMidCode(OpLabel, tempSym3);
        }
    }
    else if (nextSym == "WHILETK")
    {
        whileBlockNum++;
        int nowWhileBlockNum = whileBlockNum;
        symtab tempSym1 = symtab("whilebegin" + to_string(nowWhileBlockNum), nowWhileBlockNum, WHILEBEGINTYPE);
        symtab tempSym2 = symtab("whileend" + to_string(nowWhileBlockNum), nowWhileBlockNum, WHILEENDTYPE);
        whileBeginSymVector.push_back(tempSym1);
        whileEndSymVector.push_back(tempSym2);
        addMidCode(OpLabel, tempSym1);
        inLoopStack++;
        printWordSymbol();
        getNextSym();
        if (nextSym == "LPARENT")
        {
            printWordSymbol();
            getNextSym();
            syntaxCond();
            if (nextSym == "RPARENT")
            {
                printWordSymbol();
                getNextSym();
            }
            else
            {
                errorLine = word[indexNum - 2].line;
                addError(errorLine, 'j');
            }
            addMidCode(OpBr, currentSym, tempSym2);
            syntaxStmt();
            addMidCode(OpJ, tempSym1);
        }
        whileBeginSymVector.pop_back();
        whileEndSymVector.pop_back();
        inLoopStack--;
        addMidCode(OpLabel, tempSym2);
    }
    else if (nextSym == "BREAKTK")
    {
        errorLine = word[indexNum - 1].line;
        if (inLoopStack == 0)
            addError(errorLine, 'm');
        printWordSymbol();
        getNextSym();
        if (nextSym == "SEMICN")
        {
            printWordSymbol();
            getNextSym();
        }
        else
        {
            errorLine = word[indexNum - 2].line;
            addError(errorLine, 'i');
        }
        addMidCode(OpJ, whileEndSymVector.back());
    }
    else if (nextSym == "CONTINUETK")
    {
        errorLine = word[indexNum - 1].line;
        if (inLoopStack == 0)
            addError(errorLine, 'm');
        printWordSymbol();
        getNextSym();
        if (nextSym == "SEMICN")
        {
            printWordSymbol();
            getNextSym();
        }
        else
        {
            errorLine = word[indexNum - 2].line;
            addError(errorLine, 'i');
        }
        addMidCode(OpJ, whileBeginSymVector.back());
    }
    else if (nextSym == "RETURNTK")
    {
        //cout << currentReturnBlock << " " << blockNumVector.back() << endl;
        if (currentReturnBlock == blockNumVector.back())
            funcHaveReturn = true;
        printWordSymbol();
        getNextSym();
        int flag = 0;
        currentFuncReturnType = funcReturnTypes.back();
        if (nextSym != "SEMICN")
        {
            // currentFuncReturnType = funcReturnTypes.back();
            if (currentFuncReturnType == VoidType)
            {
                errorLine = word[indexNum - 2].line;
                flag = 1;
                //addError(errorLine, 'f');
            }
            syntaxExp();
        }
        if (nextSym == "SEMICN")
        {
            printWordSymbol();
            getNextSym();
        }
        else
        {
            errorLine = word[indexNum - 2].line;
            flag = 2;
            //addError(errorLine, 'i');
        }
        if (flag == 1)
            addError(errorLine, 'f');
        else if (flag == 2)
            addError(errorLine, 'i');
        symtab tempSym1 = symtab();
        if (currentFuncReturnType == IntType)
        {
            tempSym1 = currentSym;
            tempSym1.ValType = currentFuncReturnType;
        }
        else if (currentFuncReturnType == VoidType)
        {
            tempSym1 = symtab();
            tempSym1.ValType = currentFuncReturnType;
        }
        // else
        // {
        //     cout << "???" << endl;
        // }
        addMidCode(OpFuncReturn, tempSym1);
        //currentSym = returnSym;
    }
    else if (nextSym == "PRINTFTK")
    {
        errorLine = word[indexNum - 1].line;
        printWordSymbol();
        getNextSym();
        if (nextSym == "LPARENT")
        {
            printWordSymbol();
            getNextSym();
            if (nextSym == "STRCON")
            {
                string formatString = word[indexNum - 1].content;
                string strForSpilit = formatString.substr(1, formatString.length() - 2);
                vector<string> strs = splitWithPattern(strForSpilit, "%d");
                int len = strs.size();
                int strIter = 0;
                int formatCharNum = 0;
                int expNum = 0;
                for (int i = 0; i < formatString.length() - 1; i++)
                {
                    if ((formatString[i] == '%') && (formatString[i + 1] == 'd'))
                    {
                        formatCharNum++;
                    }
                }
                printWordSymbol();
                getNextSym();
                if (len == 0)
                {
                    strs.push_back("");
                }
                int index = addStr(strs[strIter++]);
                addMidCode(OpOut, symtab("string", index, STRINGGEN), symtab("string", index, STRINGGEN));
                while (nextSym == "COMMA")
                {
                    expNum++;
                    printWordSymbol();
                    getNextSym();
                    syntaxExp();
                    addMidCode(OpOut, symtab("expression", 0, EXPRESSIONGEN), currentSym);
                    index = addStr(strs[strIter++]);
                    addMidCode(OpOut, symtab("string", index, STRINGGEN), symtab("string", index, STRINGGEN));
                }
                if (formatCharNum != expNum)
                    addError(errorLine, 'l');
                formatCharNum = 0;
                expNum = 0;
                if (nextSym == "RPARENT")
                {
                    printWordSymbol();
                    getNextSym();
                }
                else
                {
                    errorLine = word[indexNum - 2].line;
                    addError(errorLine, 'j');
                }
                if (nextSym == "SEMICN")
                {
                    printWordSymbol();
                    getNextSym();
                }
                else
                {
                    errorLine = word[indexNum - 2].line;
                    addError(errorLine, 'i');
                }
            }
        }
    }
    else
    {
        if (nextSym != "SEMICN")
        {
            syntaxExp();
        }
        if (nextSym == "SEMICN")
        {
            printWordSymbol();
            getNextSym();
        }
        else
        {
            errorLine = word[indexNum - 2].line;
            addError(errorLine, 'i');
        }
    }
    if (syntaxTry == false)
        outFile << '<' << "Stmt" << '>' << endl;
}

/*
void syntaxAddExp()
{
    syntaxMulExp();
    syntaxAddExpExtend();
    if (syntaxTry == false)
        outFile << '<' << "AddExp" << '>' << endl;
}
*/

void syntaxAddExp()
{
    syntaxMulExp();
    if (syntaxTry == false)
        outFile << '<' << "AddExp" << '>' << endl;
    while (nextSym == "PLUS" || nextSym == "MINU")
    {
        symtab tempSym1 = currentSym;
        Operator op;
        if (nextSym == "PLUS")
            //addGenOp(OpAdd);
            op = OpAdd;
        else if (nextSym == "MINU")
            //addGenOp(OpMinus);
            op = OpMinus;
        printWordSymbol();
        getNextSym();
        syntaxMulExp();
        if (syntaxTry == false)
        {
            outFile << '<' << "AddExp" << '>' << endl;
        }
        if (syntaxTry == false && isDecl == false)
        {
            symtab tempSym2 = currentSym;
            symtab tempSym3 = symtab(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
            addMidCode(op, tempSym1, tempSym2, tempSym3);
            currentSym = tempSym3;
        }
    }
}

void syntaxAddExpExtend()
{
    if (nextSym == "PLUS" || nextSym == "MINU")
    {
        symtab tempSym1 = currentSym;
        Operator op;
        if (syntaxTry == false)
            outFile << '<' << "AddExp" << '>' << endl;
        if (nextSym == "PLUS")
            //addGenOp(OpAdd);
            op = OpAdd;
        else if (nextSym == "MINU")
            //addGenOp(OpMinus);
            op = OpMinus;
        printWordSymbol();
        getNextSym();
        syntaxMulExp();
        syntaxAddExpExtend();
        if (syntaxTry == false && isDecl == false)
        {
            symtab tempSym2 = currentSym;
            symtab tempSym3 = symtab(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
            addMidCode(op, tempSym1, tempSym2, tempSym3);
            currentSym = tempSym3;
        }
    }
}

/*
void syntaxMulExp()
{
    syntaxUnaryExp();
    syntaxMulExpExtend();
    if (syntaxTry == false)
        outFile << '<' << "MulExp" << '>' << endl;
}
*/

void syntaxMulExp()
{
    syntaxUnaryExp();
    if (syntaxTry == false)
        outFile << '<' << "MulExp" << '>' << endl;
    while (nextSym == "MULT" || nextSym == "DIV" || nextSym == "MOD")
    {
        symtab tempSym1 = currentSym;
        Operator op;
        if (nextSym == "MULT")
            //addGenOp(OpMulti);
            op = OpMulti;
        else if (nextSym == "DIV")
            //addGenOp(OpDiv);
            op = OpDiv;
        else if (nextSym == "MOD")
            //addGenOp(OpMod);
            op = OpMod;
        printWordSymbol();
        getNextSym();
        syntaxUnaryExp();
        if (syntaxTry == false)
            outFile << '<' << "MulExp" << '>' << endl;
        if (syntaxTry == false && isDecl == false)
        {
            symtab tempSym2 = currentSym;
            symtab tempSym3(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
            addMidCode(op, tempSym1, tempSym2, tempSym3);
            currentSym = tempSym3;
        }
    }
}

void syntaxMulExpExtend()
{
    if (nextSym == "MULT" || nextSym == "DIV" || nextSym == "MOD")
    {
        symtab tempSym1 = currentSym;
        Operator op;
        if (syntaxTry == false)
            outFile << '<' << "MulExp" << '>' << endl;
        if (nextSym == "MULT")
            //addGenOp(OpMulti);
            op = OpMulti;
        else if (nextSym == "DIV")
            //addGenOp(OpDiv);
            op = OpDiv;
        else if (nextSym == "MOD")
            //addGenOp(OpMod);
            op = OpMod;
        printWordSymbol();
        getNextSym();
        syntaxUnaryExp();
        syntaxMulExpExtend();
        if (syntaxTry == false && isDecl == false)
        {
            symtab tempSym2 = currentSym;
            symtab tempSym3(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
            addMidCode(op, tempSym1, tempSym2, tempSym3);
            currentSym = tempSym3;
        }
    }
}

void syntaxUnaryExp()
{
    string symName;
    int errorLine;
    if (nextSym == "IDENFR" && word[indexNum].symbol == "LPARENT")
    {
        symName = word[indexNum - 1].content;
        errorLine = word[indexNum - 1].line;
        currentFuncName = word[indexNum - 1].content;
        tmpErrorLineNum = word[indexNum - 1].line;
        bool symNameInAllSymTab = findInAllSymTab(symName);
        if (symNameInAllSymTab == false)
            addError(errorLine, 'c');
        funcReturnTypes.push_back(findValTypeInAllSymTab(symName));
        //currentFuncReturnType = findValTypeInAllSymTab(symName);
        printWordSymbol();
        getNextSym();
        if (nextSym == "LPARENT")
        {
            printWordSymbol();
            getNextSym();
            vector<symtab> params = getFuncParams(symName); //函数定义
            int paraNum = 0;                                //函数实现
            int funcRParamNum = params.size();
            bool flag = false;
            int indexNumBackUp = indexNum;
            vector<symtab> currentFuncParas;
            symtab tempSym1 = symtab();
            if (nextSym != "RPARENT")
            {
                //syntaxFuncRParams();
                //original syntaxFuncRParams as follows
                vector<symTabValType> currentFuncParaValTypes;
                currentFuncParaValType = IntType;
                syntaxExp();
                symtab tempSym1 = symtab();
                tempSym1 = currentSym;
                currentFuncParas.push_back(tempSym1);
                paraNum++;
                currentFuncParaValTypes.push_back(currentFuncParaValType);
                while (nextSym == "COMMA")
                {
                    currentFuncParaValType = IntType;
                    printWordSymbol();
                    getNextSym();
                    syntaxExp();
                    tempSym1 = currentSym;
                    currentFuncParas.push_back(tempSym1);
                    paraNum++;
                    currentFuncParaValTypes.push_back(currentFuncParaValType);
                }
                if (paraNum == funcRParamNum)
                {
                    for (int i = 0; i < funcRParamNum; i++)
                    {
                        if (currentFuncParaValTypes[i] != params[i].ValType)
                            flag = true;
                    }
                }
                if (syntaxTry == false)
                    outFile << '<' << "FuncRParams" << '>' << endl;
            }
            else
            {
                paraNum = 0;
            }
            symtab tempSym2 = symtab();
            tempSym2.IDENFRName = symName;
            tempSym2.params = currentFuncParas;
            addMidCode(OpPushParam, tempSym2);
            symtab tempSym3 = symtab();
            tempSym3 = findSymbol(symName);
            addMidCode(OpFuncCall, tempSym3);
            currentSym = returnSym;
            symtab tempSym4(genNewSymName(), VarType, IntType);
            addMidCode(OpMv, tempSym4, currentSym);
            currentSym = tempSym4;
            if (indexNumBackUp == indexNum)
                paraNum = 0;
            if ((paraNum != funcRParamNum) && (symNameInAllSymTab == true))
                addError(tmpErrorLineNum, 'd');
            if ((flag) && (symNameInAllSymTab == true))
                addError(tmpErrorLineNum, 'e');
            if (nextSym == "RPARENT")
            {
                printWordSymbol();
                getNextSym();
            }
            else
            {
                int errorLine = word[indexNum - 2].line;
                addError(errorLine, 'j');
            }
        }
        currentFuncParaValType = findValTypeInAllSymTab(currentFuncName);
        funcReturnTypes.pop_back();
    }
    else if (nextSym == "PLUS" || nextSym == "MINU" || nextSym == "NOT")
    {
        bool flag1 = (nextSym == "MINU");
        bool flag2 = (nextSym == "NOT");
        syntaxUnaryOp();
        syntaxUnaryExp();
        if (flag1 && syntaxTry == false && isDecl == false)
        {
            // if (currentSym.GenType == INTGEN)
            //     currentSym.value *= -1;
            // else
            // {
            symtab tempSym1(genNewSymName(), VarType, IntType);
            addMidCode(OpMulti, currentSym, symtab("ConstInt-1!", -1, INTGEN), tempSym1);
            currentSym = tempSym1;
            // }
        }
        if (flag2 && syntaxTry == false && isDecl == false)
        {
            symtab tempSym1(genNewSymName(), VarType, IntType);
            addMidCode(OpNot, currentSym, tempSym1);
            currentSym = tempSym1;
        }
    }
    else
    {
        syntaxPrimaryExp();
    }
    if (syntaxTry == false)
        outFile << '<' << "UnaryExp" << '>' << endl;
}

void syntaxUnaryOp()
{
    if (nextSym == "PLUS" || nextSym == "MINU" || nextSym == "NOT")
    {
        printWordSymbol();
        getNextSym();
    }
    if (syntaxTry == false)
        outFile << '<' << "UnaryOp" << '>' << endl;
}

void syntaxPrimaryExp()
{
    if (nextSym == "LPARENT")
    {
        printWordSymbol();
        getNextSym();
        syntaxExp();
        if (nextSym == "RPARENT")
        {
            printWordSymbol();
            getNextSym();
        }
        else
        {
            int errorLine = word[indexNum - 2].line;
            addError(errorLine, 'j');
        }
    }
    else if (nextSym == "IDENFR")
    {
        syntaxLVal();
    }
    else if (nextSym == "INTCON")
    {
        if (syntaxTry == false)
        {
            int tempVal = atoi(word[printWordIndex].content.c_str());
            string tempName = "ConstInt" + word[printWordIndex].content + "!";
            symtab tempSym1 = symtab(tempName, tempVal, INTGEN);
            currentSym = tempSym1;
        }
        printWordSymbol();
        getNextSym();
        if (syntaxTry == false)
            outFile << '<' << "Number" << '>' << endl;
    }
    if (syntaxTry == false)
        outFile << '<' << "PrimaryExp" << '>' << endl;
}

//original syntaxFuncRParams
//now is discarded
void syntaxFuncRParams()
{
    string name = currentFuncName;
    int paraNum = 0;
    vector<symTabValType> currentFuncParaValTypes;
    currentFuncParaValType = IntType;
    syntaxExp();
    paraNum++;
    currentFuncParaValTypes.push_back(currentFuncParaValType);
    while (nextSym == "COMMA")
    {
        currentFuncParaValType = IntType;
        printWordSymbol();
        getNextSym();
        syntaxExp();
        paraNum++;
        currentFuncParaValTypes.push_back(currentFuncParaValType);
    }
    vector<symtab> params = getFuncParams(name);
    int funcRParamNum = params.size();
    if (paraNum != funcRParamNum)
        addError(tmpErrorLineNum, 'd');
    bool flag = false;
    for (int i = 0; i < funcRParamNum; i++)
    {
        if (currentFuncParaValTypes[i] != params[i].ValType)
            flag = true;
    }
    if ((paraNum == funcRParamNum) && (flag == true))
    {
        addError(tmpErrorLineNum, 'e');
    }
    if (syntaxTry == false)
        outFile << '<' << "FuncRParams" << '>' << endl;
}

void syntaxCond()
{
    syntaxLOrExp();
    if (syntaxTry == false)
        outFile << '<' << "Cond" << '>' << endl;
}

/*
void syntaxLOrExp()
{
    syntaxLAndExp();
    syntaxLOrExpExtend();
    if (syntaxTry == false)
        outFile << '<' << "LOrExp" << '>' << endl;
}
*/

void syntaxLOrExp()
{
    orBlockNum++;
    int nowOrBlockNum = orBlockNum;
    symtab tempSym1 = symtab(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
    tempSym1.value = 0;
    symtab constInt0("ConstInt0!", 0, INTGEN);
    symtab labelSym1 = symtab("OrBegin" + to_string(nowOrBlockNum), nowOrBlockNum, ORBEGINTYPE);
    symtab labelSym2 = symtab("OrEnd" + to_string(nowOrBlockNum), nowOrBlockNum, ORENDTYPE);
    addMidCode(OpLabel, labelSym1);
    addMidCode(OpMv, tempSym1, constInt0);
    //vector<symtab> orSyms;
    syntaxLAndExp();
    addMidCode(OpOr, currentSym, tempSym1);
    //orSyms.push_back(currentSym);
    if (syntaxTry == false)
        outFile << '<' << "LOrExp" << '>' << endl;
    while (nextSym == "OR")
    {
        //addMidCode(OpBr, currentSym, labelSym1);
        //symtab tempSym1 = currentSym;
        //Operator op = OpOr;
        printWordSymbol();
        getNextSym();
        syntaxLAndExp();
        addMidCode(OpOr, currentSym, tempSym1);
        //orSyms.push_back(currentSym);
        // if (syntaxTry == false)
        // {
        //     symtab tempSym2 = currentSym;
        //     symtab tempSym3 = symtab(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
        //     addMidCode(op, tempSym1, tempSym2, tempSym3);
        //     currentSym = tempSym3;
        // }
        if (syntaxTry == false)
            outFile << '<' << "LOrExp" << '>' << endl;
    }
    //symtab tempSym1 = symtab(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
    //orSyms.push_back(tempSym1);
    //addMidCode(OpOr, orSyms);
    //currentSym = tempSym1;
    addMidCode(OpLabel, labelSym2);
    currentSym = tempSym1;
}

void syntaxLOrExpExtend()
{
    if (nextSym == "OR")
    {
        if (syntaxTry == false)
            outFile << '<' << "LOrExp" << '>' << endl;
        printWordSymbol();
        getNextSym();
        syntaxLAndExp();
        syntaxLOrExpExtend();
    }
}

/*
void syntaxLAndExp()
{
    syntaxEqExp();
    syntaxLAndExpExtend();
    if (syntaxTry == false)
        outFile << '<' << "LAndExp" << '>' << endl;
}
*/

void syntaxLAndExp()
{
    andBlockNum++;
    int nowAndBlockNum = andBlockNum;
    symtab tempSym1 = symtab(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
    tempSym1.value = 1;
    symtab constInt1("ConstInt1!", 1, INTGEN);
    symtab labelSym1 = symtab("AndBegin" + to_string(nowAndBlockNum), nowAndBlockNum, ANDBEGINTYPE);
    symtab labelSym2 = symtab("AndEnd" + to_string(nowAndBlockNum), nowAndBlockNum, ANDENDTYPE);
    addMidCode(OpLabel, labelSym1);
    addMidCode(OpMv, tempSym1, constInt1);
    syntaxEqExp();
    addMidCode(OpAnd, currentSym, tempSym1);
    if (syntaxTry == false)
        outFile << '<' << "LAndExp" << '>' << endl;
    while (nextSym == "AND")
    {
        // symtab tempSym1 = currentSym;
        // Operator op = OpAnd;
        printWordSymbol();
        getNextSym();
        syntaxEqExp();
        addMidCode(OpAnd, currentSym, tempSym1);
        // if (syntaxTry == false)
        // {
        //     symtab tempSym2 = currentSym;
        //     symtab tempSym3 = symtab(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
        //     addMidCode(op, tempSym1, tempSym2, tempSym3);
        //     currentSym = tempSym3;
        // }
        if (syntaxTry == false)
            outFile << '<' << "LAndExp" << '>' << endl;
    }
    addMidCode(OpLabel, labelSym2);
    currentSym = tempSym1;
}

void syntaxLAndExpExtend()
{
    if (nextSym == "AND")
    {
        if (syntaxTry == false)
            outFile << '<' << "LAndExp" << '>' << endl;
        printWordSymbol();
        getNextSym();
        syntaxEqExp();
        syntaxLAndExpExtend();
    }
}

/*
void syntaxEqExp()
{
    syntaxRelExp();
    syntaxEqExpExtend();
    if (syntaxTry == false)
        outFile << '<' << "EqExp" << '>' << endl;
}
*/

void syntaxEqExp()
{
    syntaxRelExp();
    if (syntaxTry == false)
        outFile << '<' << "EqExp" << '>' << endl;
    while (nextSym == "EQL" || nextSym == "NEQ")
    {
        symtab tempSym1 = currentSym;
        Operator op;
        if (nextSym == "EQL")
            op = OpBeq;
        else if (nextSym == "NEQ")
            op = OpBne;
        printWordSymbol();
        getNextSym();
        syntaxRelExp();
        if (syntaxTry == false)
            outFile << '<' << "EqExp" << '>' << endl;
        if (syntaxTry == false && isDecl == false)
        {
            symtab tempSym2 = currentSym;
            symtab tempSym3(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
            addMidCode(op, tempSym1, tempSym2, tempSym3);
            currentSym = tempSym3;
        }
    }
}

void syntaxEqExpExtend()
{
    if (nextSym == "EQL" || nextSym == "NEQ")
    {
        Operator op;
        if (syntaxTry == false)
            outFile << '<' << "EqExp" << '>' << endl;
        if (nextSym == "EQL")
            op = OpBeq;
        else if (nextSym == "NEQ")
            op = OpBne;
        printWordSymbol();
        getNextSym();
        syntaxRelExp();
        syntaxEqExpExtend();
    }
}

/*
void syntaxRelExp()
{
    syntaxAddExp();
    syntaxRelExpExtend();
    if (syntaxTry == false)
        outFile << '<' << "RelExp" << '>' << endl;
}
*/

void syntaxRelExp()
{
    syntaxAddExp();
    if (syntaxTry == false)
        outFile << '<' << "RelExp" << '>' << endl;
    while (nextSym == "LSS" || nextSym == "LEQ" || nextSym == "GRE" || nextSym == "GEQ")
    {
        symtab tempSym1 = currentSym;
        Operator op;
        if (nextSym == "LSS")
            op = OpBlt;
        else if (nextSym == "LEQ")
            op = OpBle;
        else if (nextSym == "GRE")
            op = OpBgt;
        else if (nextSym == "GEQ")
            op = OpBge;
        printWordSymbol();
        getNextSym();
        syntaxAddExp();
        if (syntaxTry == false)
            outFile << '<' << "RelExp" << '>' << endl;
        if (syntaxTry == false && isDecl == false)
        {
            symtab tempSym2 = currentSym;
            symtab tempSym3 = symtab(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
            addMidCode(op, tempSym1, tempSym2, tempSym3);
            currentSym = tempSym3;
        }
    }
}

void syntaxRelExpExtend()
{
    if (nextSym == "LSS" || nextSym == "LEQ" || nextSym == "GRE" || nextSym == "GEQ")
    {
        Operator op;
        if (syntaxTry == false)
            outFile << '<' << "RelExp" << '>' << endl;
        if (nextSym == "LSS")
            op = OpBlt;
        else if (nextSym == "LEQ")
            op = OpBle;
        else if (nextSym == "GRE")
            op = OpBgt;
        else if (nextSym == "GEQ")
            op = OpBge;
        printWordSymbol();
        getNextSym();
        syntaxAddExp();
        syntaxRelExpExtend();
    }
}

void syntaxLVal()
{
    string symName;
    int errorLine = 0;
    symTabValType tmpValType = NoneValType;
    int dimNum = 0;
    symtab dim1Sym = symtab();
    symtab dim2Sym = symtab();
    if (nextSym == "IDENFR")
    {
        symName = word[indexNum - 1].content;
        errorLine = word[indexNum - 1].line;
        tmpValType = findValTypeInAllSymTab(symName);
        if (tmpValType == IntType)
            dimNum = 0;
        else if (tmpValType == OneDimType)
            dimNum = 1;
        else if (tmpValType == TwoDimType)
            dimNum = 2;
        if (findInAllSymTab(symName) == false)
            addError(errorLine, 'c');
        printWordSymbol();
        getNextSym();
        int loopNum = 0;
        while (nextSym == "LBRACK")
        {
            loopNum++;
            dimNum--;
            printWordSymbol();
            getNextSym();
            syntaxExp();
            if (loopNum == 1)
                dim1Sym = currentSym;
            if (loopNum == 2)
                dim2Sym = currentSym;
            if (nextSym == "RBRACK")
            {
                printWordSymbol();
                getNextSym();
            }
            else
            {
                int errorLine = word[indexNum - 2].line;
                addError(errorLine, 'k');
            }
        }
    }
    if (dimNum == 0)
        currentFuncParaValType = IntType;
    else if (dimNum == 1)
        currentFuncParaValType = OneDimType;
    else if (dimNum == 2)
        currentFuncParaValType = TwoDimType;
    if (syntaxTry == false && isDecl == false)
    {
        symtab tempSym3(genNewSymName(), VarType, findSymbol(symName).ValType, EXPRESSIONGEN);
        tempSym3.dimNum = dimNum;
        symtab constInt0("ConstInt0!", 0, INTGEN);
        if (dimNum == 0)
        {
            if (tmpValType == IntType)
            {
                addMidCode(OpArrayGet, findSymbol(symName), constInt0, tempSym3);
            }
            else if (tmpValType == OneDimType)
            {
                addMidCode(OpArrayGet, findSymbol(symName), dim1Sym, tempSym3);
            }
            else if (tmpValType == TwoDimType)
            {
                symtab tempSym1(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
                symtab tempSym2(genNewSymName(), VarType, IntType, EXPRESSIONGEN);
                //symtab defDim2Sym(genNewSymName(), findSymbol(symName).dimension2, INTGEN);
                //cout << "===" << findSymbol(symName).IDENFRName << " " << symName << " " << findSymbol(symName).dimension2 << "===" << endl;
                symtab defDim2Sym("ConstInt" + to_string(findSymbol(symName).dimension2) + "!", findSymbol(symName).dimension2, INTGEN);
                addMidCode(OpMulti, dim1Sym, defDim2Sym, tempSym1);
                addMidCode(OpAdd, tempSym1, dim2Sym, tempSym2);
                addMidCode(OpArrayGet, findSymbol(symName), tempSym2, tempSym3);
            }
        }
        else if (dimNum == 1)
        {
            if (tmpValType == OneDimType)
            {
                addMidCode(OpArrayGet, findSymbol(symName), constInt0, tempSym3);
            }
            else if (tmpValType == TwoDimType)
            {
                addMidCode(OpArrayGet, findSymbol(symName), dim1Sym, tempSym3);
            }
        }
        else if (dimNum == 2)
        {
            if (tmpValType == TwoDimType)
            {
                addMidCode(OpArrayGet, findSymbol(symName), constInt0, tempSym3);
            }
        }
        currentSym = tempSym3;
    }
    if (syntaxTry == false)
        outFile << '<' << "LVal" << '>' << endl;
}

void syntaxExp()
{
    syntaxAddExp();
    if (syntaxTry == false)
        outFile << '<' << "Exp" << '>' << endl;
}

void syntaxAnalyz()
{
    getNextSym();
    syntaxCompUnit();
}

void getNextSym()
{
    line = word[indexNum].line;
    nextSym = word[indexNum++].symbol;
}

void saveNowEnv()
{
    saveNum = indexNum;
    saveSym = nextSym;
    syntaxTry = true;
}

void recoverEnv()
{
    indexNum = saveNum;
    nextSym = saveSym;
    syntaxTry = false;
}

bool isBType()
{
    if (nextSym == "INTTK")
        return true;
    else
        return false;
}