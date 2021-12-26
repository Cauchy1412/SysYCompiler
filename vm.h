#include <bits/stdc++.h>
#include "define.h"
using namespace std;
#pragma once

void run()
{
    setVmIndex();
    bool flag = true;
    for (runIndex = 0; runIndex < midCodes.size(); runIndex++)
    {
        MidCode i = midCodes[runIndex];
        if (i.op == OpFuncDeclare)
            flag = false;
        if (i.op == OpMain)
        {
            flag = true;
        }
        if (flag == true)
            process(i);
        if (flag == false && i.op == OpFuncDeclare)
            process(i);
    }
}

void setVmIndex()
{
    for (int i = 0; i < midCodes.size(); i++)
    {
        MidCode nowMidCode = midCodes[i];
        int val = nowMidCode.symtabs[0].value;
        if (nowMidCode.op == OpLabel)
        {
            if (nowMidCode.symtabs[0].ConditionType == IFBEGINTYPE)
            {
                ifbeginMap.insert(pair<int, int>(val, i));
            }
            else if (nowMidCode.symtabs[0].ConditionType == IFELSETYPE)
            {
                ifelseMap.insert(pair<int, int>(val, i));
            }
            else if (nowMidCode.symtabs[0].ConditionType == IFENDTYPE)
            {
                ifendMap.insert(pair<int, int>(val, i));
            }
            else if (nowMidCode.symtabs[0].ConditionType == WHILEBEGINTYPE)
            {
                whilebeginMap.insert(pair<int, int>(val, i));
            }
            else if (nowMidCode.symtabs[0].ConditionType == WHILEENDTYPE)
            {
                whileendMap.insert(pair<int, int>(val, i));
            }
        }
        else if (nowMidCode.op == OpFuncDeclare)
        {
            funcMap.insert(pair<string, int>(nowMidCode.symtabs[0].IDENFRName, i));
        }
    }
}

int findLabelIndex(symtab sym)
{
    int val = sym.value;
    unordered_map<int, int>::iterator iter;
    if (sym.ConditionType == IFBEGINTYPE)
    {
        iter = ifbeginMap.find(val);
        if (iter != ifbeginMap.end())
        {
            return iter->second;
        }
        else
            return -1;
    }
    else if (sym.ConditionType == IFELSETYPE)
    {
        iter = ifelseMap.find(val);
        if (iter != ifelseMap.end())
        {
            return iter->second;
        }
        else
            return -1;
    }
    else if (sym.ConditionType == IFENDTYPE)
    {
        iter = ifendMap.find(val);
        if (iter != ifendMap.end())
        {
            return iter->second;
        }
        else
            return -1;
    }
    else if (sym.ConditionType == WHILEBEGINTYPE)
    {
        iter = whilebeginMap.find(val);
        if (iter != whilebeginMap.end())
        {
            return iter->second;
        }
        else
            return -1;
    }
    else if (sym.ConditionType == WHILEENDTYPE)
    {
        iter = whileendMap.find(val);
        if (iter != whileendMap.end())
        {
            return iter->second;
        }
        else
            return -1;
    }
    return -1;
}

// int findLabelIndex(symtab sym)
// {
//     int thisVal = sym.value;
//     symConditionType thisCon = sym.ConditionType;
//     for (int i = 0; i < midCodes.size(); i++)
//     {
//         MidCode nowMidCode = midCodes[i];
//         if (nowMidCode.op == OpLabel && nowMidCode.symtabs[0].value == thisVal && nowMidCode.symtabs[0].ConditionType == thisCon)
//         {
//             return i;
//         }
//     }
//     return -1;
// }

// int findLabelIndex(symtab sym)
// {
//     int thisVal = sym.value;
//     symConditionType thisCon = sym.ConditionType;
//     if (thisCon == WHILEBEGINTYPE || thisCon == IFBEGINTYPE)
//     {
//         for (int i = runIndex; i > 0; i--)
//         {
//             MidCode nowMidCode = midCodes[i];
//             if (nowMidCode.op == OpLabel && nowMidCode.symtabs[0].value == thisVal && nowMidCode.symtabs[0].ConditionType == thisCon)
//             {
//                 return i;
//             }
//         }
//     }
//     else
//     {
//         for (int i = runIndex; i < midCodes.size(); i++)
//         {
//             MidCode nowMidCode = midCodes[i];
//             if (nowMidCode.op == OpLabel && nowMidCode.symtabs[0].value == thisVal && nowMidCode.symtabs[0].ConditionType == thisCon)
//             {
//                 return i;
//             }
//         }
//     }

//     return -1;
// }

int findFuncIndex(symtab sym)
{
    unordered_map<string, int>::iterator iter;
    iter = funcMap.find(sym.IDENFRName);
    if (iter != funcMap.end())
    {
        return iter->second;
    }
    else
        return -1;
}

// int findFuncIndex(symtab sym)
// {
//     for (int i = 0; i < midCodes.size(); i++)
//     {
//         MidCode nowMidCode = midCodes[i];
//         if (nowMidCode.op == OpFuncDeclare && nowMidCode.symtabs[0].IDENFRName == sym.IDENFRName)
//         {
//             return i;
//         }
//     }
//     return -1;
// }

void process(MidCode i)
{
    //cout << OperatorStr[i.op] << " " << i.symtabs[0].IDENFRName << endl;
    Operator op = i.op;
    if (op == OpLabel)
        judgeLabel(i);
    if (noRunAndBlockVector.back() == 0 && noRunOrBlockVector.back() == 0)
        execute(i);
    else
    {
        //cout << "=====" << OperatorStr[i.op] << " " << i.symtabs[0].IDENFRName << "=====" << endl;
    }
    // if (op == OpLabel)
    //     execute(i);
    // else if (andNoRunFlag == false && orNoRunFlag == false)
    //     execute(i);
}

void judgeLabel(MidCode midcode)
{
    symtab sym = midcode.symtabs[0];
    symConditionType con = sym.ConditionType;
    if (con == ORENDTYPE)
    {
        if (sym.value == noRunOrBlockVector.back())
        {
            noRunOrBlockVector.pop_back();
            //cout << "===ORENDTYPE===" << endl;
        }
    }
    if (con == ANDENDTYPE)
    {
        if (sym.value == noRunAndBlockVector.back())
        {
            noRunAndBlockVector.pop_back();
            //cout << "===ANDENDTYPE===" << endl;
        }
    }
}

void execute(MidCode i)
{
    // executeNum++;
    // clock_t timeStart, timeEnd;
    // timeStart = clock();
    //coutSingleMidCode(i);
    //cout << OperatorStr[i.op] << " " << i.symtabs[0].IDENFRName << endl;
    Operator op = i.op;
    //cout << "op: " << OperatorStr[op] << endl;
    if (op == OpAdd || op == OpMinus || op == OpMulti || op == OpDiv || op == OpMod || op == OpBge ||
        op == OpBgt || op == OpBle || op == OpBlt || op == OpBeq || op == OpBne)
        runCalc(i);
    if (op == OpNot)
        runCalNot(i);
    if (op == OpAssign)
        runAssign(i);
    if (op == OpGet)
        runGet(i);
    if (op == OpOut)
        runPrint(i);
    if (op == OpLabel)
        runLabel(i);
    if (op == OpDeclare)
        runDecl(i);
    if (op == OpArrayInit)
        runArrayInit(i);
    if (op == OpArrayGet)
        runArrayGet(i);
    if (op == OpFuncDeclare)
        runFuncDecl(i);
    if (op == OpFuncCall)
        runFuncCall(i);
    if (op == OpFuncReturn)
        runFuncReturn(i);
    if (op == OpPushParam)
        runPushParam(i);
    if (op == OpBr)
        runBr(i);
    if (op == OpJ)
        runJ(i);
    if (op == OpAnd)
        runAnd(i);
    if (op == OpOr)
        runOr(i);
    if (op == OpMv)
        runMv(i);
    // timeEnd = clock();
    // vmOpRunTime[op] += double(timeEnd - timeStart) / CLOCKS_PER_SEC;
}

void runFuncCall(MidCode midCode)
{
    //cout << "call " << midCode.symtabs[0].IDENFRName << endl;
    // bool flag = false;
    // for (auto i : midCodes)
    // {
    //     if (flag == true)
    //     {
    //         execute(i);
    //     }
    //     if (i.op == OpFuncDeclare && i.symtabs[0].IDENFRName == midCode.symtabs[0].IDENFRName)
    //         flag = true;
    //     if (i.op == OpFuncReturn)
    //         flag = false;
    // }
    int index = runIndex;
    runIndexVector.push_back(index);
    symtab sym = midCode.symtabs[0];
    runIndex = findFuncIndex(sym);
    //runIndex = findFuncIndex(sym)-1;
    // vmFuncSymNameVector.push_back(sym.IDENFRName);
    // vmFuncBlockNum++;
}

void runFuncReturn(MidCode midCode)
{
    symtab sym = midCode.symtabs[0];
    if (sym.ValType == VoidType)
    {
        ;
    }
    else if (sym.ValType == IntType)
    {
        funcRetVal = GetVal(sym);
        funcRetValQueue.push(funcRetVal);
    }

    if (runIndexVector.size() != 0)
    {
        vmFuncEnd();
    }
    //cout << "===runIndex size: " << runIndexVector.size() << "===" << endl;
    //cout << "===runIndex: " << runIndex << "===" << endl;
    //cout << "=== return: " << funcRetVal << "===" << endl;
}

void vmFuncEnd()
{
    vmFuncClear();
    runIndex = runIndexVector.back();
    runIndexVector.pop_back();
    vmFuncSymNameVector.pop_back();
    vmFuncBlockNum--;
}

void vmFuncClear()
{
    string funcName = getOriginFuncName(vmFuncSymNameVector.back());
    for (auto &i : vmSymList)
    {
        if (i.belongFuncName == funcName)
        {
            while (i.vmSymVector.size() > 0 && i.vmSymVector.back().funcBlockNum >= vmFuncBlockNum)
            {
                //cout << "===pop " << i.vmSymVector.back().IDENFRName << "===" << endl;
                i.vmSymVector.pop_back();
            }
        }
    }
}

void runMv(MidCode midCode)
{
    symtab sym1 = midCode.symtabs[0];
    symtab sym2 = midCode.symtabs[1];
    // if (!haveSymInVm(sym1))
    // {
    //     addVmSymList(sym1);
    // }
    addVmSymList(sym1);
    SetVal(sym1, sym2);
    //cout << "===mv " << sym1.IDENFRName << " =" << GetVal(sym1) << "===" << endl;
}

void runAnd(MidCode midCode)
{
    symtab sym1 = midCode.symtabs[0];
    symtab sym2 = midCode.symtabs[1];
    // if (!haveSymInVm(sym2))
    // {
    //     addVmSymList(sym2);
    // }
    int val1;
    int val2 = GetVal(sym2);
    int val;
    if (val2 == 0)
    {
        val = 0;
    }
    else if (val2 == 1)
    {
        val1 = GetVal(sym1);
        val = val1;
        if (val1 == 0)
        {
            //cout << "===0===" << endl;
            noRunAndBlockVector.push_back(andBlockNumVector.back());
        }
    }
    if (val == 1)
        andNoRunFlag = false;
    else
        andNoRunFlag = true;
    SetVal(sym2, val);
}

void runOr(MidCode midCode)
{
    symtab sym1 = midCode.symtabs[0];
    symtab sym2 = midCode.symtabs[1];
    // if (!haveSymInVm(sym2))
    // {
    //     addVmSymList(sym2);
    // }
    int val1;
    int val2 = GetVal(sym2);
    int val;
    if (val2 == 1)
    {
        val = 1;
    }
    else if (val2 == 0)
    {
        val1 = GetVal(sym1);
        val = val1;
        if (val1 == 1)
        {
            //cout << "===1===" << endl;
            noRunOrBlockVector.push_back(orBlockNumVector.back());
        }
    }
    if (val == 0)
        orNoRunFlag = false;
    else
        orNoRunFlag = true;
    SetVal(sym2, val);
}

void runBr(MidCode midCode)
{
    symtab sym1 = midCode.symtabs[0];
    symtab sym2 = midCode.symtabs[1];
    int val1 = GetVal(sym1);
    if (val1 == 0)
    {
        jump(sym2);
    }
}

void runJ(MidCode midCode)
{
    symtab sym1 = midCode.symtabs[0];
    jump(sym1);
}

void jump(symtab sym)
{
    int index = findLabelIndex(sym);
    runIndex = index - 1;
}

void runCalNot(MidCode midCode)
{
    symtab sym1 = midCode.symtabs[0];
    symtab sym2 = midCode.symtabs[1];
    int val1 = GetVal(sym1);
    int val = 0;
    val = (val1 == 0) ? 1 : 0;
    sym2.value = val;
    addVmSymList(sym2);
}

void runCalc(MidCode midCode)
{
    symtab sym1 = midCode.symtabs[0];
    symtab sym2 = midCode.symtabs[1];
    symtab sym3 = midCode.symtabs[2];
    int val1 = GetVal(sym1);
    int val2 = GetVal(sym2);
    int val = 0;
    if (midCode.op == OpAdd)
        val = val1 + val2;
    if (midCode.op == OpMinus)
        val = val1 - val2;
    if (midCode.op == OpMulti)
        val = val1 * val2;
    if (midCode.op == OpDiv)
        val = val1 / val2;
    if (midCode.op == OpMod)
        val = val1 % val2;
    if (midCode.op == OpBge)
        val = val1 >= val2;
    if (midCode.op == OpBgt)
        val = val1 > val2;
    if (midCode.op == OpBle)
        val = val1 <= val2;
    if (midCode.op == OpBlt)
        val = val1 < val2;
    if (midCode.op == OpBeq)
        val = val1 == val2;
    if (midCode.op == OpBne)
        val = val1 != val2;
    sym3.value = val;
    //changeSymInVm(sym3, val);
    //cout << sym3.IDENFRName << " = " << GetVal(sym3) << endl;
    addVmSymList(sym3);
}

void runAssign(MidCode midCode)
{
    symtab sym1 = midCode.symtabs[0];
    symtab sym2 = midCode.symtabs[1];
    //cout << "==assign begin==" << endl;
    SetVal(sym1, sym2);
    //cout << "==assign end==" << endl;
}

void runGet(MidCode midCode)
{
    //cout << "Please write a num : ";
    symtab sym = midCode.symtabs[0];
    int tempInt;
    cin >> tempInt;
    sym.value = tempInt;
    addVmSymList(sym);
    //cout << endl;
}

void runPrint(MidCode midCode)
{
    if (midCode.symtabs[0].GenType == STRINGGEN)
    {
        string str = strList[midCode.symtabs[1].value];
        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] == '\\' && str[i + 1] == 'n')
            {
                resultFile << "\n";
                //cout << "\n";
                i++;
            }
            else
            {
                resultFile << str[i];
                //cout << str[i];
            }
        }
        //cout << strList[midCode.symtabs[1].value];
    }
    else if (midCode.symtabs[0].GenType == EXPRESSIONGEN)
    {
        resultFile << GetVal(midCode.symtabs[1]);
        //cout << GetVal(midCode.symtabs[1]);
    }
}

void runLabel(MidCode midCode)
{
    symtab sym1 = midCode.symtabs[0];
    int num = sym1.value;
    if (sym1.ConditionType == ORBEGINTYPE)
    {
        orBlockNumVector.push_back(num);
        conScopes.push_back(orScope);
        orNoRunFlag = false;
    }
    else if (sym1.ConditionType == ORENDTYPE)
    {
        orBlockNumVector.pop_back();
        conScopes.pop_back();
        orNoRunFlag = false;
    }
    else if (sym1.ConditionType == ANDBEGINTYPE)
    {
        andBlockNumVector.push_back(num);
        conScopes.push_back(andScope);
        andNoRunFlag = false;
    }
    else if (sym1.ConditionType == ANDENDTYPE)
    {
        andBlockNumVector.pop_back();
        conScopes.pop_back();
        andNoRunFlag = false;
    }
    else if (sym1.ConditionType == FUNCENDTYPE)
    {
        if (runIndexVector.size() != 0)
        {
            vmFuncEnd();
        }
    }
    else if (sym1.ConditionType == IFBEGINTYPE)
    {
    }
    else if (sym1.ConditionType == IFELSETYPE)
    {
    }
    else if (sym1.ConditionType == IFENDTYPE)
    {
    }
}

void runDecl(MidCode midCode)
{
    symtab sym = midCode.symtabs[0];
    addVmSymList(sym);
}

void runArrayInit(MidCode midCode)
{
    symtab sym = midCode.symtabs[0];
    //cout << "arrayInit " << sym.IDENFRName << endl;
    if (sym.ValType == IntType)
    {
        //sym.value = sym.value;
        //cout << sym.IDENFRName << " size: " << sym.initArray.size() << endl;
        if (sym.initArray.size() == 0)
            sym.value = 0;
        else
            sym.value = GetVal(sym.initArray[0]);
    }
    else if (sym.ValType == OneDimType || sym.ValType == TwoDimType)
    {
        vector<int> valList;
        for (auto i : sym.initArray)
        {
            symtab tempSym = i;
            int tempVal = GetVal(tempSym);
            valList.push_back(tempVal);
        }
        sym.elementArray = valList;
    }
    addVmSymList(sym);
}

void runArrayGet(MidCode midCode)
{
    symtab sym1 = midCode.symtabs[0];
    symtab sym2 = midCode.symtabs[1];
    symtab sym3 = midCode.symtabs[2];
    sym3.GenType = FINDGEN;
    sym3.ValType = sym1.ValType;
    sym3.dimension1 = sym1.dimension1;
    sym3.dimension2 = sym1.dimension2;
    //cout << "===flag1===" << endl;
    sym1 = findSymInVm(sym1);
    //cout << "===flag2===" << endl;
    if (sym1.GenType != FINDGEN)
    {
        //cout << "===sym1.GenType != FINDGEN===" << endl;
        sym3.findIndex1 = sym1.index;
        sym3.findIndex2 = GetVal(sym2);
    }
    else if (sym1.GenType == FINDGEN)
    {
        //cout << "===ArrayGet Find Gen===" << endl;
        //cout << "===findIndex1 " << sym1.findIndex1 << "===" << endl;
        sym3.findIndex1 = sym1.findIndex1;
        if (sym1.dimNum == sym3.dimNum)
        {
            //cout << "===sym1.dimNum == sym3.dimNum " << sym1.dimNum << "===" << endl;
            sym3.findIndex2 = sym1.findIndex2;
        }
        else
        {
            int index1 = sym1.findIndex2;
            int index2 = index1 * sym1.dimension2 + GetVal(sym2);
            sym3.findIndex2 = index2;
            //cout << "=== index2: " << index2 << "===" << endl;
        }
    }
    //cout << "===arrayget " << sym1.IDENFRName << " " << sym2.IDENFRName << " " << sym3.IDENFRName << " " << sym3.findIndex1 << " " << sym3.findIndex2 << "===" << endl;
    addVmSymList(sym3);
}

void runFuncDecl(MidCode midCode)
{
    symtab sym = midCode.symtabs[0];
    string funcName = getOriginFuncName(sym.IDENFRName);
    //string funcName = sym.IDENFRName;
    for (auto i : sym.params)
    {
        i.funcName = funcName;
        i.FuncScopeType = inFuncType;
        i.belongFuncName = funcName;
        //cout << i.funcName << endl;
        if (i.ValType == IntType)
            i.dimNum = 0;
        else if (i.ValType == OneDimType)
            i.dimNum = 1;
        else if (i.ValType == TwoDimType)
            i.dimNum = 2;
        addVmSymList(i);
    }
}

void runPushParam(MidCode midCode)
{
    symtab sym = midCode.symtabs[0];
    vmFuncSymNameVector.push_back(sym.IDENFRName);
    vmFuncBlockNum++;
    vector<symtab> symParams = sym.params;
    int paramIndex = 0;
    bool flag = false;
    //cout << "funcName " << sym.IDENFRName << endl;
    // for (auto i : symParams)
    // {
    //     cout << "push " << i.IDENFRName << endl;
    // }
    string funcName = getOriginFuncName(sym.IDENFRName);
    //cout << "push " << funcName << " params: " << endl;
    //cout << funcParamIndexs.size() << endl;
    for (auto i : vmSymList)
    {
        if (i.funcName == funcName)
        {
            symtab tempSym = symParams[paramIndex++];
            tempSym = findSymInVm(tempSym);
            i.funcBlockNum = vmFuncBlockNum;
            i.FuncScopeType = inFuncType;
            i.vmSymVector.clear();
            //i.belongFuncName = getOriginFuncName(sym.IDENFRName);
            if (i.ValType == IntType)
            {
                i.value = GetVal(tempSym);
            }
            else
            {
                i.GenType = FINDGEN;
                i.ValType = tempSym.ValType;
                i.dimension1 = tempSym.dimension1;
                i.dimension2 = tempSym.dimension2;
                i.findIndex1 = tempSym.findIndex1;
                i.findIndex2 = tempSym.findIndex2;
            }
            addVmSymList(i);
            //cout << "===" << sym.IDENFRName << "push " << i.IDENFRName << " " << GetVal(i) << "===" << endl;
        }
    }
    //getchar();
}

void SetSym(symtab sym1, symtab sym2)
{
    sym1 = findSymInVm(sym1);
    vmSymList[sym1.index].GenType = FINDGEN;
    vmSymList[sym1.index].ValType = sym2.ValType;
    vmSymList[sym1.index].dimension1 = sym2.dimension1;
    vmSymList[sym1.index].dimension2 = sym2.dimension2;
    vmSymList[sym1.index].findIndex1 = sym2.findIndex1;
    vmSymList[sym1.index].findIndex2 = sym2.findIndex2;
    //vmSymList[sym1.index].dimNum = sym2.dimNum;
    //getchar();
}

int GetVal(symtab sym)
{
    int ret = 0;
    sym = findSymInVm(sym);
    //cout << "=== gentype " << sym.GenType << "===" << endl;
    //cout << "valtype: "<<symTabValTypeStr[sym.ValType]<<endl;
    //cout << sym.findIndex1 << " " << sym.findIndex2 << "!!!" << endl;
    //cout << vmSymList[sym.index].IDENFRName << endl;
    if (sym.IDENFRName == "@ret@0")
    {
        ret = funcRetVal;
        // ret = funcRetValQueue.front();
        // funcRetValQueue.pop();
    }
    else if (sym.GenType == FINDGEN)
    {
        // cout << "findgen" << endl;
        // cout << sym.IDENFRName << endl;
        // cout << sym.ValType << endl;
        // cout << sym.findIndex1 << " " << sym.findIndex2 << endl;
        // cout << vmSymList[sym.findIndex1].elementArray.size() << endl;
        // cout << vmSymList[sym.findIndex1].IDENFRName << endl;
        symtab tempSym = symtab();
        tempSym = vmSymList[sym.findIndex1];
        if (tempSym.FuncScopeType == inFuncType)
        {
            tempSym = tempSym.vmSymVector.back();
        }
        if (sym.ValType == IntType)
            ret = tempSym.value;
        else if (sym.ValType == OneDimType || sym.ValType == TwoDimType)
            ret = tempSym.elementArray[sym.findIndex2];
        //cout << "findgen end" << endl;
    }
    else if (sym.GenType == INTGEN)
    {
        //cout << "intgen" << endl;
        ret = sym.value;
    }
    else
    {
        //cout << "elsegen" << endl;
        //sym = findSymInVm(sym);
        //ret = vmSymList[sym.index].value;
        ret = sym.value;
    }
    //cout << "=== getval " << sym.IDENFRName << " " << ret << "===" << endl;
    return ret;
}

void SetVal(symtab sym1, symtab sym2)
{
    int val = GetVal(sym2);
    SetVal(sym1, val);
}

void SetVal(symtab sym, int val)
{
    sym = findSymInVm(sym);
    if (sym.GenType == FINDGEN)
    {
        if (vmSymList[sym.findIndex1].FuncScopeType == inFuncType)
        {
            if (sym.ValType == IntType)
                vmSymList[sym.findIndex1].vmSymVector[vmSymList[sym.findIndex1].vmSymVector.size() - 1].value = val;
            else if (sym.ValType == OneDimType || sym.ValType == TwoDimType)
                vmSymList[sym.findIndex1].vmSymVector[vmSymList[sym.findIndex1].vmSymVector.size() - 1].elementArray[sym.findIndex2] = val;
        }
        else
        {
            if (sym.ValType == IntType)
                vmSymList[sym.findIndex1].value = val;
            else if (sym.ValType == OneDimType || sym.ValType == TwoDimType)
                vmSymList[sym.findIndex1].elementArray[sym.findIndex2] = val;
        }
    }
    else
    {
        if (vmSymList[sym.index].FuncScopeType == inFuncType)
        {
            vmSymList[sym.index].vmSymVector[vmSymList[sym.index].vmSymVector.size() - 1].value = val;
        }
        else
        {
            vmSymList[sym.index].value = val;
        }
    }
    //cout << "=== setval " << vmSymList[sym.index].IDENFRName << " " << val << "===" << endl;
}

void addVmSymList(symtab sym)
{
    // if (haveSymInVm(sym) == false)
    // {
    //     sym.index = vmSymIndex++;
    //     vmSymList.push_back(sym);
    // }
    // else
    // {
    //     //cout << "!!!" << endl;
    //     for (auto &i : vmSymList)
    //     {
    //         if (sym.IDENFRName == i.IDENFRName)
    //         {
    //             i = sym;
    //             return;
    //         }
    //     }
    // }

    //vmSymList.push_back(sym);

    // sym.index = vmSymIndex++;
    // vmSymList.push_back(sym);
    // map<string, int>::iterator iter;
    // iter = vmSymIndexMap.find(sym.IDENFRName);
    // if (iter == vmSymIndexMap.end())
    // {
    //     vmSymIndexMap.insert(pair<string, int>(sym.IDENFRName, sym.index));
    // }
    // else
    // {
    //     vmSymIndexMap[sym.IDENFRName] = sym.index;
    // }
    if (isInVmFunc() == false)
    {
        //cout << "===not in VmFunc===" << endl;
        unordered_map<string, int>::iterator iter = vmSymIndexMap.find(sym.IDENFRName);
        if (iter == vmSymIndexMap.end())
        {
            sym.index = vmSymIndex++;
            vmSymList.push_back(sym);
            vmSymIndexMap.insert(pair<string, int>(sym.IDENFRName, sym.index));
        }
        else
        {
            int index = vmSymList[iter->second].index;
            vmSymList[iter->second] = sym;
            vmSymList[iter->second].index = index;
        }
    }
    else
    {
        //cout << "===in VmFunc===" << endl;
        sym.FuncScopeType = inFuncType;
        sym.funcBlockNum = vmFuncBlockNum;
        //sym.funcName = getOriginFuncName(vmFuncSymNameVector.back());
        unordered_map<string, int>::iterator iter = vmSymIndexMap.find(sym.IDENFRName);
        if (iter == vmSymIndexMap.end())
        {
            symtab tempSym = symtab();
            tempSym.IDENFRName = sym.IDENFRName;
            tempSym.FuncScopeType = inFuncType;
            tempSym.index = vmSymIndex++;
            tempSym.belongFuncName = getOriginFuncName(vmFuncSymNameVector.back());
            sym.index = tempSym.index;
            tempSym.vmSymVector.push_back(sym);
            vmSymList.push_back(tempSym);
            vmSymIndexMap.insert(pair<string, int>(tempSym.IDENFRName, tempSym.index));
        }
        else
        {
            if (vmSymList[iter->second].vmSymVector.size() == 0 || vmSymList[iter->second].vmSymVector.back().funcBlockNum != vmFuncBlockNum)
            {
                //cout << "===insert in empty VmSymVector===" << endl;
                sym.index = iter->second;
                vmSymList[iter->second].vmSymVector.push_back(sym);
            }
            else
            {
                //cout << "===insert in not empty VmSymVector===" << endl;
                sym.index = iter->second;
                vmSymList[iter->second].vmSymVector[vmSymList[iter->second].vmSymVector.size() - 1] = sym;
            }
        }
    }

    //vmSymIndexMap.insert(pair<string, int>(sym.IDENFRName, sym.index));
}

symtab findSymInVm(symtab sym)
{
    //cout << sym.IDENFRName << endl;
    bool find = false;
    symtab ret = symtab();
    if (sym.IDENFRName == "@ret@0")
    {
        return sym;
    }
    else if (sym.GenType == INTGEN)
    {
        return sym;
    }
    else
    {
        unordered_map<string, int>::iterator iter;
        iter = vmSymIndexMap.find(sym.IDENFRName);
        if (iter != vmSymIndexMap.end())
        {
            ret = vmSymList[iter->second];
            if (ret.FuncScopeType == inFuncType)
            {
                ret = ret.vmSymVector.back();
            }
            find = true;
        }
        // map<string, int>::iterator iter;
        // iter = vmSymIndexMap.find(sym.IDENFRName);
        // if (iter != vmSymIndexMap.end())
        // {
        //     cout << "find index1 " << iter->second << endl;
        // }
        // int t;
        // for (int i = 0; i < vmSymList.size(); i++)
        // {
        //     if (sym.IDENFRName == vmSymList[i].IDENFRName)
        //     {
        //         t=i;

        //     }
        // }
        // cout << "find index2 " << t << endl;
        // for (auto i : vmSymList)
        // {
        //     if (sym.IDENFRName == i.IDENFRName)
        //     {
        //         ret = i;
        //         find = true;
        //         //return i;
        //     }
        // }
    }
    if (find == false)
    {
        cout << "Can not find " << sym.IDENFRName << " !" << endl;
    }
    return ret;
}

bool isInVmFunc()
{
    if (vmFuncSymNameVector.size() != 0)
        return true;
    else
        return false;
}
// bool haveSymInVm(symtab sym)
// {
//     bool find = false;
//     for (auto i : vmSymList)
//     {
//         if (sym.IDENFRName == i.IDENFRName)
//         {
//             find = true;
//         }
//     }
//     return find;
// }

// void changeSymInVm(symtab sym, int val)
// {
//     bool find = false;
//     for (auto &i : vmSymList)
//     {
//         if (sym.IDENFRName == i.IDENFRName)
//         {
//             i.value = val;
//             find = true;
//             return;
//         }
//     }
//     if (find == false)
//     {
//         //sym.value = val;
//         addVmSymList(sym);
//     }
// }

// symtab getTrueSym(symtab sym)
// {
//     sym = findSymInVm(sym);
//     if (sym.GenType == FINDGEN)
//     {
//         return vmSymList[sym.findIndex1];
//     }
//     else
//     {
//         return sym;
//     }
// }