#include <bits/stdc++.h>
#include "define.h"
using namespace std;
#pragma once

void changeSymName(symtab &sym)
{
    string nameStr = sym.IDENFRName;
    string blockStr = to_string(sym.block);
    string newNameStr = nameStr + "@" + blockStr;
    //cout << newNameStr << endl;
    sym.IDENFRName = newNameStr;
    for (auto &i : sym.params)
    {
        changeSymName(i);
    }
    for (auto &i : sym.initArray)
    {
        changeSymName(i);
    }
}

void addMidCode(Operator op, vector<symtab> symtabs)
{
    for (auto &i : symtabs)
    {
        changeSymName(i);
    }
    // cout << sym1.IDENFRName << endl;
    midCodes.push_back(MidCode(op, symtabs));
}

void addMidCode(Operator op, symtab sym1)
{
    vector<symtab> symtabs;
    changeSymName(sym1);
    symtabs.push_back(sym1);
    // cout << sym1.IDENFRName << endl;
    midCodes.push_back(MidCode(op, symtabs));
}
void addMidCode(Operator op, symtab sym1, symtab sym2)
{
    vector<symtab> symtabs;
    changeSymName(sym1);
    changeSymName(sym2);
    symtabs.push_back(sym1);
    symtabs.push_back(sym2);
    // cout << sym1.IDENFRName << endl;
    // cout << sym2.IDENFRName << endl;
    midCodes.push_back(MidCode(op, symtabs));
}
void addMidCode(Operator op, symtab sym1, symtab sym2, symtab sym3)
{
    vector<symtab> symtabs;
    changeSymName(sym1);
    changeSymName(sym2);
    changeSymName(sym3);
    symtabs.push_back(sym1);
    symtabs.push_back(sym2);
    symtabs.push_back(sym3);
    // cout << sym1.IDENFRName << endl;
    // cout << sym2.IDENFRName << endl;
    // cout << sym3.IDENFRName << endl;
    midCodes.push_back(MidCode(op, symtabs));
}

// string lw(string grf, string name)
// {
//     string str = "lw ";
//     str.append(grf);
//     str.append(", ");
//     int offset = getOffset(name);
//     scopeType nameScopeType = getScope(name);
//     string scopeName = (nameScopeType == Global) ? "$gp" : "$sp";
//     str.append(to_string(offset));
//     str.append("(");
//     str.append(scopeName);
//     str.append(")");
//     return str;
// }
// string sw(string grf, string name)
// {
//     string str = "sw ";
//     str.append(grf);
//     str.append(", ");
//     int offset = getOffset(name);
//     scopeType nameScopeType = getScope(name);
//     string scopeName = (nameScopeType == Global) ? "$gp" : "$sp";
//     str.append(to_string(offset));
//     str.append("(");
//     str.append(scopeName);
//     str.append(")");
//     return str;
// }

int getOffset(string name)
{
    for (auto i : genLocalSymbolTable)
    {
        if (name == i.IDENFRName)
            return i.offset;
    }
    for (auto i : genGlobalSymbolTable)
    {
        if (name == i.IDENFRName)
            return i.offset;
    }
    return -1;
}

scopeType getScope(string name)
{
    for (auto i : genLocalSymbolTable)
    {
        if (name == i.IDENFRName)
            return i.ScopeType;
    }
    for (auto i : genGlobalSymbolTable)
    {
        if (name == i.IDENFRName)
            return i.ScopeType;
    }
    return NoneScopeType;
}

// void genGlobalVar()
// {
//     mipsCodes.push_back(".text");
//     int offset = 0;
//     for (auto i : midCodes)
//     {
//         if (i.op == OpFuncDeclare)
//             break;
//         if (i.op == OpDeclare)
//         {
//             symtab sym = i.symtabs[0];
//             sym.offset = offset;
//             genGlobalSymbolTable.push_back(sym);
//             if (sym.ValType == IntType)
//             {
//                 string str = "li $t0, ";
//                 str.append(to_string(sym.value));
//                 mipsCodes.push_back(str);
//                 str = "sw $t0, ";
//                 str.append(to_string(sym.offset));
//                 str.append("($gp)   # ");
//                 str.append(sym.IDENFRName);
//                 mipsCodes.push_back(str);
//                 offset += 4;
//             }
//             else if (sym.ValType == OneDimType || sym.ValType == TwoDimType)
//             {
//                 int len = 0;
//                 if (sym.ValType == OneDimType)
//                     len = sym.dimension1;
//                 if (sym.ValType == TwoDimType)
//                     len = sym.dimension1 * sym.dimension2;
//                 for (int i = 0; i < len; i++)
//                 {
//                     string str = "li $t0, ";
//                     str.append(to_string(sym.elementArray[i]));
//                     mipsCodes.push_back(str);
//                     str = "sw $t0, ";
//                     str.append(to_string(offset));
//                     str.append("($gp)   # ");
//                     str.append(sym.IDENFRName);
//                     str.append("[");
//                     str.append(to_string(i));
//                     str.append("]");
//                     mipsCodes.push_back(str);
//                     offset += 4;
//                 }
//             }
//         }
//     }
// }

// void genStr()
// {
//     mipsCodes.push_back(".data");
//     int index = 0;
//     for (auto i : strList)
//     {
//         string str = "string";
//         str.append(to_string(index++));
//         str.append(": .asciiz \"");
//         str.append(i);
//         str.append("\"");
//         mipsCodes.push_back(str);
//     }
// }

// void genFunc(string name)
// {
//     string str = name;
//     str.append(":");
//     mipsCodes.push_back(str);
//     int flag = 0;
//     int offset = 0;
//     symtab tempSym = symtab("null", NoneItemType, NoneValType);
//     for (auto i : midCodes)
//     {
//         if (i.op == OpLabel && i.symtabs[0].IDENFRName == name)
//             flag = 1;
//         if (flag == 1)
//         {
//             int len = 1;
//             if (i.op == OpDeclare || i.op == OpArrayInit)
//             {
//                 tempSym = i.symtabs[0];
//                 len = tempSym.len;
//                 tempSym.offset = offset;
//                 genLocalSymbolTable.push_back(tempSym);
//                 offset += 4 * len;
//             }
//             else if (i.op == OpAdd || i.op == OpMinus || i.op == OpMinus || i.op == OpDiv || i.op == OpMod)
//             {
//                 tempSym = i.symtabs[2];
//                 tempSym.len = 1;
//                 len = 1;
//                 tempSym.offset = offset;
//                 genLocalSymbolTable.push_back(tempSym);
//                 offset += 4 * len;
//             }
//         }
//     }
//     str = "addi $sp, $sp, -";
//     str.append(to_string(offset));
//     mipsCodes.push_back(str);
//     for (auto i : genLocalSymbolTable)
//     {
//         if (i.ValType == IntType)
//         {
//             str = "li $t0, ";
//             str.append(to_string(i.value));
//             mipsCodes.push_back(str);
//             str = "sw $t0, ";
//             str.append(to_string(i.offset));
//             str.append("($sp)    #");
//             str.append(i.IDENFRName);
//             mipsCodes.push_back(str);
//         }
//         else if (i.ValType == OneDimType || i.ValType == TwoDimType)
//         {
//             for (int index = 0; index < i.len; index++)
//             {
//                 str = "li $t0, ";
//                 if (!i.elementArray.empty())
//                 {
//                     str.append(to_string(i.elementArray[index]));
//                 }
//                 else
//                 {
//                     str.append(to_string(0));
//                 }
//                 mipsCodes.push_back(str);
//                 str = "sw $t0, ";
//                 str.append(to_string(i.offset + index * 4));
//                 str.append("($sp)    #");
//                 str.append(i.IDENFRName);
//                 str.append("[");
//                 str.append(to_string(index));
//                 str.append("]");
//                 mipsCodes.push_back(str);
//             }
//         }
//     }
//     flag = 0;
//     for (auto i : midCodes)
//     {
//         if (i.op == OpLabel && i.symtabs[0].IDENFRName == name)
//             flag = 1;
//         if (flag == 1)
//         {
//             switch (i.op)
//             {
//             case OpGet:
//                 str = "li $v0, 5";
//                 mipsCodes.push_back(str);
//                 str = "syscall  # getint ";
//                 str.append(i.symtabs[0].IDENFRName);
//                 mipsCodes.push_back(str);
//                 str = sw("$v0", i.symtabs[0].IDENFRName);
//                 mipsCodes.push_back(str);
//                 break;
//             case OpOut:
//                 if (i.symtabs[0].GenType == EXPRESSIONGEN)
//                 {
//                     if (i.symtabs[1].GenType == INTGEN)
//                     {
//                         str = "li $a0, ";
//                         str.append(to_string(i.symtabs[1].value));
//                     }
//                     else
//                     {
//                         str = lw("$a0", i.symtabs[1].IDENFRName);
//                     }
//                     mipsCodes.push_back(str);
//                     str = "li $v0, 1";
//                     mipsCodes.push_back(str);
//                     str = "syscall    #printf ";
//                     str.append(i.symtabs[1].IDENFRName);
//                     mipsCodes.push_back(str);
//                 }
//                 else if (i.symtabs[0].GenType == STRINGGEN)
//                 {
//                     str = "li $v0, 4";
//                     mipsCodes.push_back(str);
//                     str = "la $a0, ";
//                     str.append(i.symtabs[1].IDENFRName);
//                     str.append(to_string(i.symtabs[1].value));
//                     mipsCodes.push_back(str);
//                     mipsCodes.push_back("syscall    # print string");
//                 }
//                 break;
//             case OpAdd:
//                 if (i.symtabs[0].GenType == INTGEN)
//                 {
//                     str = "li $t0, ";
//                     str.append(to_string(i.symtabs[0].value));
//                 }
//                 else
//                 {
//                     str = lw("$t0", i.symtabs[0].IDENFRName);
//                 }
//                 mipsCodes.push_back(str);
//                 if (i.symtabs[1].GenType == INTGEN)
//                 {
//                     str = "li $t0, ";
//                     str.append(to_string(i.symtabs[1].value));
//                 }
//                 else
//                 {
//                     str = lw("$t0", i.symtabs[1].IDENFRName);
//                 }
//                 mipsCodes.push_back(str);
//                 str = "addu $t2, $t0, $t1";
//                 mipsCodes.push_back(str);
//                 str = sw("$t2", i.symtabs[2].IDENFRName);
//                 mipsCodes.push_back(str);
//                 break;
//             case OpMinus:
//                 if (i.symtabs[0].GenType == INTGEN)
//                 {
//                     str = "li $t0, ";
//                     str.append(to_string(i.symtabs[0].value));
//                 }
//                 else
//                 {
//                     str = lw("$t0", i.symtabs[0].IDENFRName);
//                 }
//                 mipsCodes.push_back(str);
//                 if (i.symtabs[1].GenType == INTGEN)
//                 {
//                     str = "li $t0, ";
//                     str.append(to_string(i.symtabs[1].value));
//                 }
//                 else
//                 {
//                     str = lw("$t0", i.symtabs[1].IDENFRName);
//                 }
//                 mipsCodes.push_back(str);
//                 str = "subu $t2, $t0, $t1";
//                 mipsCodes.push_back(str);
//                 str = sw("$t2", i.symtabs[2].IDENFRName);
//                 mipsCodes.push_back(str);
//                 break;
//             case OpMulti:
//                 if (i.symtabs[0].GenType == INTGEN)
//                 {
//                     str = "li $t0, ";
//                     str.append(to_string(i.symtabs[0].value));
//                 }
//                 else
//                 {
//                     str = lw("$t0", i.symtabs[0].IDENFRName);
//                 }
//                 mipsCodes.push_back(str);
//                 if (i.symtabs[1].GenType == INTGEN)
//                 {
//                     str = "li $t0, ";
//                     str.append(to_string(i.symtabs[1].value));
//                 }
//                 else
//                 {
//                     str = lw("$t0", i.symtabs[1].IDENFRName);
//                 }
//                 mipsCodes.push_back(str);
//                 str = "mult $t0, $t1";
//                 mipsCodes.push_back(str);
//                 str = "mflo $t2";
//                 mipsCodes.push_back(str);
//                 str = sw("$t2", i.symtabs[2].IDENFRName);
//                 mipsCodes.push_back(str);
//                 break;
//             case OpDiv:
//                 if (i.symtabs[0].GenType == INTGEN)
//                 {
//                     str = "li $t0, ";
//                     str.append(to_string(i.symtabs[0].value));
//                 }
//                 else
//                 {
//                     str = lw("$t0", i.symtabs[0].IDENFRName);
//                 }
//                 mipsCodes.push_back(str);
//                 if (i.symtabs[1].GenType == INTGEN)
//                 {
//                     str = "li $t0, ";
//                     str.append(to_string(i.symtabs[1].value));
//                 }
//                 else
//                 {
//                     str = lw("$t0", i.symtabs[1].IDENFRName);
//                 }
//                 mipsCodes.push_back(str);
//                 str = "div $t0, $t1";
//                 mipsCodes.push_back(str);
//                 str = "mflo $t2";
//                 mipsCodes.push_back(str);
//                 str = sw("$t2", i.symtabs[2].IDENFRName);
//                 mipsCodes.push_back(str);
//                 break;
//             case OpMod:
//                 if (i.symtabs[0].GenType == INTGEN)
//                 {
//                     str = "li $t0, ";
//                     str.append(to_string(i.symtabs[0].value));
//                 }
//                 else
//                 {
//                     str = lw("$t0", i.symtabs[0].IDENFRName);
//                 }
//                 mipsCodes.push_back(str);
//                 if (i.symtabs[1].GenType == INTGEN)
//                 {
//                     str = "li $t0, ";
//                     str.append(to_string(i.symtabs[1].value));
//                 }
//                 else
//                 {
//                     str = lw("$t0", i.symtabs[1].IDENFRName);
//                 }
//                 mipsCodes.push_back(str);
//                 str = "div $t0, $t1";
//                 mipsCodes.push_back(str);
//                 str = "mfhi $t2";
//                 mipsCodes.push_back(str);
//                 str = sw("$t2", i.symtabs[2].IDENFRName);
//                 mipsCodes.push_back(str);
//                 break;
//             case OpAssign:
//                 if (i.symtabs[1].GenType == INTGEN)
//                 {
//                     str = "li $t0, ";
//                     str.append(to_string(i.symtabs[1].value));
//                 }
//                 else
//                 {
//                     str = lw("$t0", i.symtabs[1].IDENFRName);
//                 }
//                 mipsCodes.push_back(str);
//                 str = sw("$t0", i.symtabs[0].IDENFRName);
//                 mipsCodes.push_back(str);
//                 break;
//             default:
//                 break;
//             }
//         }
//     }
// }

void generate()
{
    printMidCode();
    //genStr();
    //genGlobalVar();
    //genFunc("main");
    //printMips();
}

void printSingleMidCode(MidCode i)
{
    string name = i.symtabs[0].IDENFRName;
    string itemType = symTabItemTypeStr[i.symtabs[0].ItemType];
    string valType = symTabValTypeStr[i.symtabs[0].ValType];
    if (i.op == OpDeclare)
    {
        //genGlobalSymbolTable.push_back(i.symtabs[0]);
        midCodeFile << "Declare " << name << " (" << itemType << " " << valType << ")";
        if (i.symtabs[0].dimension1 != 0 || i.symtabs[0].dimension2 != 0)
        {
            midCodeFile << " (" << i.symtabs[0].dimension1 << " " << i.symtabs[0].dimension2 << ")";
            for (auto iter : i.symtabs[0].elementArray)
                midCodeFile << " " << iter;
        }
        else
        {
            midCodeFile << " " << i.symtabs[0].value;
        }
        midCodeFile << endl;
    }
    if (i.op == OpLabel)
    {
        midCodeFile << "Label " << name << endl;
    }
    if (i.op == OpGet)
    {
        midCodeFile << "Get " << name << endl;
    }
    if (i.op == OpOut)
    {
        midCodeFile << "Print ";
        if (i.symtabs[0].GenType == STRINGGEN)
        {
            midCodeFile << "string" << i.symtabs[1].value;
        }
        else if (i.symtabs[0].GenType == EXPRESSIONGEN)
        {
            midCodeFile << name << " " << i.symtabs[1].IDENFRName;
        }
        midCodeFile << endl;
    }
    if (i.op == OpAssign)
    {
        midCodeFile << "Assign " << name << " (" << valType << ") ";
        midCodeFile << i.symtabs[1].IDENFRName << " (" << symTabValTypeStr[i.symtabs[1].ValType] << ") ";
        midCodeFile << endl;
    }
    if (i.op == OpAdd)
    {
        midCodeFile << "Add " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpMinus)
    {
        midCodeFile << "Minus " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpMulti)
    {
        midCodeFile << "Multi " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpDiv)
    {
        midCodeFile << "Div " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpMod)
    {
        midCodeFile << "Mod " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpNot)
    {
        midCodeFile << "Not " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << endl;
    }
    if (i.op == OpArrayInit)
    {
        if (i.symtabs[0].ValType == IntType)
        {
            midCodeFile << "ArrayInit " << i.symtabs[0].IDENFRName;
        }
        else if (i.symtabs[0].ValType == OneDimType)
        {
            midCodeFile << "ArrayInit " << i.symtabs[0].IDENFRName << "[" << i.symtabs[0].dimension1 << "]";
        }
        else if (i.symtabs[0].ValType == TwoDimType)
            midCodeFile << "ArrayInit " << i.symtabs[0].IDENFRName << "[" << i.symtabs[0].dimension1 << "][" << i.symtabs[0].dimension2 << "]";
        midCodeFile << " (" << itemType << " " << valType << ")";
        int len = i.symtabs[0].initArray.size();
        midCodeFile << " " << len;
        midCodeFile << endl;
        for (int iter = 0; iter < len; iter++)
        {
            midCodeFile << i.symtabs[0].IDENFRName;
            if (i.symtabs[0].ValType != IntType)
                midCodeFile << "[" << iter << "]";
            midCodeFile << " = " << i.symtabs[0].initArray[iter].IDENFRName << endl;
        }
    }
    if (i.op == OpArrayGet)
    {
        midCodeFile << "ArrayGet " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpBeq)
    {
        midCodeFile << "Beq " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpBne)
    {
        midCodeFile << "Bne " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpBgt)
    {
        midCodeFile << "Bgt " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpBge)
    {
        midCodeFile << "Bge " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpBlt)
    {
        midCodeFile << "Blt " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpBle)
    {
        midCodeFile << "Ble " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpAnd)
    {
        midCodeFile << "And " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << endl;
    }
    if (i.op == OpOr)
    {
        midCodeFile << "Or " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << endl;
    }
    if (i.op == OpJ)
    {
        midCodeFile << "J " << i.symtabs[0].IDENFRName << endl;
    }
    if (i.op == OpBr)
    {
        midCodeFile << "Br " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << endl;
    }
    if (i.op == OpFuncDeclare)
    {
        midCodeFile << "FuncDeclare " << i.symtabs[0].IDENFRName;
        midCodeFile << " Params: ";
        for (auto iter : i.symtabs[0].params)
        {
            midCodeFile << iter.IDENFRName << "(" << symTabValTypeStr[iter.ValType] << ") ";
        }
        midCodeFile << endl;
    }
    if (i.op == OpFuncCall)
    {
        midCodeFile << "FuncCall " << i.symtabs[0].IDENFRName << endl;
    }
    if (i.op == OpFuncReturn)
    {
        if (i.symtabs[0].ValType == IntType)
            midCodeFile << "FuncReturn " << i.symtabs[0].IDENFRName << endl;
        else if (i.symtabs[0].ValType == VoidType)
            midCodeFile << "FuncReturn " << i.symtabs[0].IDENFRName << endl;
        //cout << "function return type " << i.symtabs[0].ValType << endl;
    }
    if (i.op == OpPushParam)
    {
        for (auto iter : i.symtabs[0].params)
            midCodeFile << "push " << iter.IDENFRName << endl;
    }
    if (i.op == OpMain)
    {
        midCodeFile << "Main Func Start " << i.symtabs[0].IDENFRName << endl;
    }
    if (i.op == OpMv)
    {
        midCodeFile << "mv " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << endl;
    }
}

void coutSingleMidCode(MidCode i)
{
    string name = i.symtabs[0].IDENFRName;
    string itemType = symTabItemTypeStr[i.symtabs[0].ItemType];
    string valType = symTabValTypeStr[i.symtabs[0].ValType];
    if (i.op == OpDeclare)
    {
        //genGlobalSymbolTable.push_back(i.symtabs[0]);
        cout << "Declare " << name << " (" << itemType << " " << valType << ")";
        if (i.symtabs[0].dimension1 != 0 || i.symtabs[0].dimension2 != 0)
        {
            cout << " (" << i.symtabs[0].dimension1 << " " << i.symtabs[0].dimension2 << ")";
            for (auto iter : i.symtabs[0].elementArray)
                cout << " " << iter;
        }
        else
        {
            cout << " " << i.symtabs[0].value;
        }
        cout << endl;
    }
    if (i.op == OpLabel)
    {
        cout << "Label " << name << endl;
    }
    if (i.op == OpGet)
    {
        cout << "Get " << name << endl;
    }
    if (i.op == OpOut)
    {
        cout << "Print ";
        if (i.symtabs[0].GenType == STRINGGEN)
        {
            cout << "string" << i.symtabs[1].value;
        }
        else if (i.symtabs[0].GenType == EXPRESSIONGEN)
        {
            cout << name << " " << i.symtabs[1].IDENFRName;
        }
        cout << endl;
    }
    if (i.op == OpAssign)
    {
        cout << "Assign " << name << " (" << valType << ") ";
        cout << i.symtabs[1].IDENFRName << " (" << symTabValTypeStr[i.symtabs[1].ValType] << ") ";
        cout << endl;
    }
    if (i.op == OpAdd)
    {
        cout << "Add " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpMinus)
    {
        cout << "Minus " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpMulti)
    {
        cout << "Multi " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpDiv)
    {
        cout << "Div " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpMod)
    {
        cout << "Mod " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpNot)
    {
        cout << "Not " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << endl;
    }
    if (i.op == OpArrayInit)
    {
        if (i.symtabs[0].ValType == IntType)
        {
            cout << "ArrayInit " << i.symtabs[0].IDENFRName;
        }
        else if (i.symtabs[0].ValType == OneDimType)
        {
            cout << "ArrayInit " << i.symtabs[0].IDENFRName << "[" << i.symtabs[0].dimension1 << "]";
        }
        else if (i.symtabs[0].ValType == TwoDimType)
            cout << "ArrayInit " << i.symtabs[0].IDENFRName << "[" << i.symtabs[0].dimension1 << "][" << i.symtabs[0].dimension2 << "]";
        cout << " (" << itemType << " " << valType << ")";
        int len = i.symtabs[0].initArray.size();
        cout << " " << len;
        cout << endl;
        for (int iter = 0; iter < len; iter++)
        {
            cout << i.symtabs[0].IDENFRName;
            if (i.symtabs[0].ValType != IntType)
                cout << "[" << iter << "]";
            cout << " = " << i.symtabs[0].initArray[iter].IDENFRName << endl;
        }
    }
    if (i.op == OpArrayGet)
    {
        cout << "ArrayGet " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpBeq)
    {
        cout << "Beq " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpBne)
    {
        cout << "Bne " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpBgt)
    {
        cout << "Bgt " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpBge)
    {
        cout << "Bge " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpBlt)
    {
        cout << "Blt " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpBle)
    {
        cout << "Ble " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << " " << i.symtabs[2].IDENFRName << endl;
    }
    if (i.op == OpAnd)
    {
        cout << "And " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << endl;
    }
    if (i.op == OpOr)
    {
        cout << "Or " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << endl;
    }
    if (i.op == OpJ)
    {
        cout << "J " << i.symtabs[0].IDENFRName << endl;
    }
    if (i.op == OpBr)
    {
        cout << "Br " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << endl;
    }
    if (i.op == OpFuncDeclare)
    {
        cout << "FuncDeclare " << i.symtabs[0].IDENFRName;
        cout << " Params: ";
        for (auto iter : i.symtabs[0].params)
        {
            cout << iter.IDENFRName << "(" << symTabValTypeStr[iter.ValType] << ") ";
        }
        cout << endl;
    }
    if (i.op == OpFuncCall)
    {
        cout << "FuncCall " << i.symtabs[0].IDENFRName << endl;
    }
    if (i.op == OpFuncReturn)
    {
        if (i.symtabs[0].ValType == IntType)
            cout << "FuncReturn " << i.symtabs[0].IDENFRName << endl;
        else if (i.symtabs[0].ValType == VoidType)
            cout << "FuncReturn " << i.symtabs[0].IDENFRName << endl;
        //cout << "function return type " << i.symtabs[0].ValType << endl;
    }
    if (i.op == OpPushParam)
    {
        for (auto iter : i.symtabs[0].params)
            cout << "push " << iter.IDENFRName << endl;
    }
    if (i.op == OpMain)
    {
        cout << "Main Func Start " << i.symtabs[0].IDENFRName << endl;
    }
    if (i.op == OpMv)
    {
        cout << "mv " << i.symtabs[0].IDENFRName << " " << i.symtabs[1].IDENFRName << endl;
    }
}

void printMidCode()
{
    int index = 0;
    for (auto i : strList)
    {
        midCodeFile << "string" << index << ": "
                    << "\"" << i << "\"" << endl;
        index++;
    }
    for (auto i : midCodes)
    {
        printSingleMidCode(i);
    }
}

void printMips()
{
    for (auto i : mipsCodes)
    {
        mipsFile << i << endl;
    }
}

void addGenOp(Operator op)
{
    if (syntaxTry == false)
        genOperatorList.push_back(op);
}

Operator getGenOp()
{
    if (syntaxTry == false)
    {
        Operator op = genOperatorList.back();
        genOperatorList.pop_back();
        return op;
    }
    return OpNone;
}

void clearExpStr()
{
    if (isDecl || isConst)
    {
        int val = getCalResult(expStr);
        expValList.push_back(val);
    }
    //cout << "ExpVal: " << val << endl;
    expStr = "";
}

void cleardimStr()
{
    if (isDim)
    {
        int val = getCalResult(dimStr);
        dimValList.push_back(val);
        //cout << "DimVal: " << val << endl;
    }
    dimStr = "";
}

vector<string> splitWithPattern(const string &str, const string &pattern)
{
    vector<string> resVec;

    if ("" == str)
    {
        return resVec;
    }
    //方便截取最后一段数据
    string strs = str + pattern;

    size_t pos = strs.find(pattern);
    size_t size = strs.size();

    while (pos != string::npos)
    {
        string x = strs.substr(0, pos);
        resVec.push_back(x);
        strs = strs.substr(pos + pattern.size(), size);
        pos = strs.find(pattern);
    }

    return resVec;
}

int addStr(string str)
{
    int size = strList.size();
    strList.push_back(str);
    return size;
}

string genNewSymName()
{
    string str = "%";
    str.append(to_string(genSymNameIndex++));
    return str;
}

string getOriginFuncName(string str)
{
    int index = str.find("@");
    return str.substr(0, index);
}