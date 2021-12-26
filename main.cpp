#include <bits/stdc++.h>
#include "define.h"
#include "handle.h"
#include "lexical.h"
#include "syntax.h"
#include "generate.h"
#include "calc.h"
#include "vm.h"
using namespace std;

int main()
{
    inFile.open("testfile.txt");
    outFile.open("output.txt");
    errorFile.open("error.txt");
    //mipsFile.open("mips.txt");
    midCodeFile.open("midCode.txt");
    resultFile.open("pcoderesult.txt");
    char c;
    while (~(c = (char)inFile.get()))
    {
        input[indexNum] = c;
        indexNum++;
    }
    len = indexNum;
    indexNum = 0;
    while (input[indexNum] != '\0')
    {
        symbol sym = getSym();
        if (sym >= IDENFR && sym < ANNOTATION)
        {
            word[wordIndex].symbol = symStr[sym];
            word[wordIndex].content = token;
            word[wordIndex].line = line;
            wordIndex++;
        }
        else if (sym == ANNOTATION)
            ;
        else
        {
            error();
        }
        getChar();
    }
    indexNum = 0;
    syntaxAnalyz();
    printError();
    //printAllTableVal();
    generate();
    //printGenTableVal();
    run();
    //printVmSymListVal();
    // cout << vmSymList.size() << " " << vmSymIndexMap.size() << " " << midCodes.size() << endl;
    // cout << executeNum << endl;
    // for (int i = 0; i < 30; i++)
    // {
    //     cout << OperatorStr[i] << " time " << vmOpRunTime[i] << endl;
    // }
    inFile.close();
    outFile.close();
    errorFile.close();
    //mipsFile.close();
    midCodeFile.close();
    resultFile.close();
    return 0;
}