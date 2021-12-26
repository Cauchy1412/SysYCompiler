#include <bits/stdc++.h>
#include "define.h"
using namespace std;
#pragma once

void printError()
{
    sort(errorArray.begin(), errorArray.end(), less<errorInf>());
    //errorArray.erase(unique(errorArray.begin(), errorArray.end()), errorArray.end());
    for (auto i : errorArray)
    {
        errorFile << i.lineNum << " " << i.errorCode << endl;
    }
}

void addError(int line, char code)
{
    if (syntaxTry == false)
    {
        errorInf inf = errorInf(line, code);
        errorArray.push_back(inf);
    }
}

bool insert(string name, symTabItemType itemType, symTabValType valType)
{

    // cout << "insert " << name << " ";
    // cout << "type " << symTabItemTypeStr[itemType];
    // cout << " " << symTabValTypeStr[valType] << endl;
    if (findInCurrentSymTab(name))
    {
        return false;
    }
    symtab insertSym = symtab(name, itemType, valType);
    //cout << name << " " << insertSym.block << endl;
    //if (isDef == true && scope == Global)
    if (isDecl || isConst)
    {
        if (valType == IntType)
        {
            insertSym.len = 1;
            if (isAssign)
            {
                //cout << "???" <<endl;
                insertSym.value = expValList.back();
                expValList.pop_back();
            }
            else
            {
                insertSym.value = 0;
            }
        }
        if (valType == OneDimType)
        {
            insertSym.len = dimValList[0];
            if (isAssign)
            {
                insertSym.elementArray = expValList;
                expValList.clear();
            }
            else
            {
                vector<int> temp(dimValList[0]);
                insertSym.elementArray = temp;
            }
            insertSym.dimension1 = dimValList[0];
            //cout << name << " dim1: " << insertSym.dimension1 << endl;
            dimValList.clear();
        }
        if (valType == TwoDimType)
        {
            insertSym.len = dimValList[0] * dimValList[1];
            if (isAssign)
            {
                insertSym.elementArray = expValList;
                expValList.clear();
            }
            else
            {
                vector<int> temp(dimValList[0] * dimValList[1]);
                insertSym.elementArray = temp;
            }
            insertSym.dimension1 = dimValList[0];
            insertSym.dimension2 = dimValList[1];
            //cout << name << " dim1: " << insertSym.dimension1 << " dim2: " << insertSym.dimension2 << endl;
            dimValList.clear();
        }
    }
    else if (isDef)
    {
        if (valType == IntType)
        {
            insertSym.len = 1;
            if (isAssign)
            {
                // insertSym.value = expValList.back();
                // expValList.pop_back();
                insertSym.value = 0;
            }
            else
            {
                insertSym.value = 0;
            }
        }
        if (valType == OneDimType)
        {
            insertSym.len = dimValList[0];
            if (isAssign)
            {
                // insertSym.elementArray = expValList;
                // expValList.clear();
                vector<int> temp(dimValList[0]);
                insertSym.elementArray = temp;
            }
            else
            {
                vector<int> temp(dimValList[0]);
                insertSym.elementArray = temp;
            }
            insertSym.dimension1 = dimValList[0];
            //cout << name << " dim1: " << insertSym.dimension1 << endl;
            dimValList.clear();
        }
        if (valType == TwoDimType)
        {
            insertSym.len = dimValList[0] * dimValList[1];
            if (isAssign)
            {
                // insertSym.elementArray = expValList;
                // expValList.clear();
                vector<int> temp(dimValList[0] * dimValList[1]);
                insertSym.elementArray = temp;
            }
            else
            {
                vector<int> temp(dimValList[0] * dimValList[1]);
                insertSym.elementArray = temp;
            }
            insertSym.dimension1 = dimValList[0];
            insertSym.dimension2 = dimValList[1];
            //cout << name << " dim1: " << insertSym.dimension1 << " dim2: " << insertSym.dimension2 << endl;
            dimValList.clear();
        }
    }
    if (itemType == ParaType && valType == TwoDimType)
    {
        insertSym.dimension2 = dimValList[0];
        dimValList.clear();
    }
    if (scope == Global)
    {
        globalSymbolTable.push_back(insertSym);
    }
    else
    {
        localSymbolTable.push_back(insertSym);
    }
    if (itemType == FuncType)
    {
        scope = Global;
    }
    else if (itemType == ParaType)
    {
        globalSymbolTable[globalSymbolTable.size() - 1].params.push_back(insertSym);
    }

    return true;
}

bool findInCurrentSymTab(string name)
{
    if (scope == Global)
    {
        for (auto i : globalSymbolTable)
        {
            if ((i.IDENFRName == name) && (i.block == blockNumVector.back()))
            {
                return true;
            }
        }
    }
    else
    {
        for (auto i : localSymbolTable)
        {
            if ((i.IDENFRName == name) && (i.block == blockNumVector.back()))
            {
                return true;
            }
        }
    }
    return false;
}

bool findInAllSymTab(string name)
{
    for (auto i : globalSymbolTable)
    {
        if (i.IDENFRName == name)
        {
            return true;
        }
    }
    for (auto i : localSymbolTable)
    {
        if (i.IDENFRName == name)
        {
            return true;
        }
    }
    return false;
}

symTabItemType findItemTypeInAllSymTab(string name)
{
    symTabItemType itemType = NoneItemType;
    for (auto i : globalSymbolTable)
    {
        if (i.IDENFRName == name)
        {
            itemType = i.ItemType;
        }
    }
    for (auto i : localSymbolTable)
    {
        if (i.IDENFRName == name)
        {
            itemType = i.ItemType;
        }
    }
    return itemType;
}

symTabValType findValTypeInAllSymTab(string name)
{
    symTabValType valType = NoneValType;
    for (auto i : globalSymbolTable)
    {
        if (i.IDENFRName == name)
        {
            valType = i.ValType;
        }
    }
    for (auto i : localSymbolTable)
    {
        if (i.IDENFRName == name)
        {
            valType = i.ValType;
        }
    }
    return valType;
}

vector<symtab> getFuncParams(string name)
{
    for (auto i : globalSymbolTable)
    {
        if (i.IDENFRName == name)
        {
            return i.params;
        }
    }
    vector<symtab> empty;
    return empty;
}

symtab findSymbol(string name)
{
    for (auto i = localSymbolTable.rbegin(); i != localSymbolTable.rend(); i++)
    {
        if ((*i).IDENFRName == name)
            return *i;
    }
    // for (auto i : localSymbolTable)
    // {
    //     if (i.IDENFRName == name)
    //         return i;
    // }
    for (auto i : globalSymbolTable)
    {
        if (i.IDENFRName == name)
            return i;
    }
    //cout << "Can not find!" << endl;
    return symtab();
}

void localUpdate()
{
    localSymbolTable.clear();
    //scope = Global;
}

void printGlobalTable()
{
    for (auto i : globalSymbolTable)
    {
        cout << i.IDENFRName << " " << i.ItemType << endl;
    }
}
void printAllTable()
{
    for (auto i : localSymbolTable)
    {
        cout << i.IDENFRName << " " << symTabItemTypeStr[i.ItemType] << endl;
    }
    for (auto i : globalSymbolTable)
    {
        cout << i.IDENFRName << " " << symTabItemTypeStr[i.ItemType] << endl;
    }
}

void changeSymVal(int val)
{
    if (scope == Global)
    {
        (*(globalSymbolTable.end() - 1)).value = val;
    }
    else
    {
        (*(localSymbolTable.end() - 1)).value = val;
    }
}

void printAllTableVal()
{
    for (auto i : globalSymbolTable)
    {
        printSymVal(i);
        //cout << i.IDENFRName << " " << i.value << endl;
    }
    for (auto i : localSymbolTable)
    {
        printSymVal(i);
        //cout << i.IDENFRName << " " << i.value << endl;
    }
}

void printGenTableVal()
{
    for (auto i : genGlobalSymbolTable)
    {
        //printSymVal(i);
        //cout << i.IDENFRName << " " << i.value << endl;
    }
    for (auto i : genLocalSymbolTable)
    {
        //printSymVal(i);
        //cout << i.IDENFRName << " " << i.value << endl;
    }
}

void printVmSymListVal()
{
    cout << endl;
    for (auto i : vmSymList)
    {
        //if (i.IDENFRName[0] != '%')
            printSymVal(i);
    }
}

void printSymVal(symtab sym)
{
    if (sym.ValType == IntType)
    {
        cout << sym.IDENFRName << " " << sym.value << endl;
    }
    else if (sym.ValType == OneDimType || sym.ValType == TwoDimType)
    {
        cout << sym.IDENFRName << " ";
        for (auto i : sym.elementArray)
        {
            cout << i << " ";
        }
        cout << endl;
    }
}