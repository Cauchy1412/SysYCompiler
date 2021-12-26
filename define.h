#include <bits/stdc++.h>
using namespace std;

#define MAX 1000000
#define RESERVE_WORD_NUM 12
#define WORD_NUM 66666

#pragma once

enum symbol
{
    IDENFR,
    INTCON,
    STRCON,
    MAINTK,
    CONSTTK,
    INTTK,
    BREAKTK,
    CONTINUETK,
    IFTK,
    ELSETK,
    WHILETK,
    GETINTTK,
    PRINTFTK,
    RETURNTK,
    VOIDTK,
    NOT,
    AND,
    OR,
    PLUS,
    MINU,
    MULT,
    DIV,
    MOD,
    LSS,
    LEQ,
    GRE,
    GEQ,
    EQL,
    NEQ,
    ASSIGN,
    SEMICN,
    COMMA,
    LPARENT,
    RPARENT,
    LBRACK,
    RBRACK,
    LBRACE,
    RBRACE,
    ANNOTATION,
    NONE
};

enum Operator
{
    OpAdd,
    OpMinus,
    OpMulti,
    OpDiv,
    OpMod,
    OpNot,
    OpAssign,
    OpGet,
    OpOut,
    OpLabel,
    OpDeclare,
    OpArrayInit,
    OpArrayGet,
    OpBge,
    OpBgt,
    OpBle,
    OpBlt,
    OpBeq,
    OpBne,
    OpAnd,
    OpOr,
    OpJ,
    OpBr,
    OpFuncDeclare,
    OpFuncCall,
    OpFuncReturn,
    OpPushParam,
    OpMain,
    OpMv,
    OpNone
};

struct errorInf
{
    int lineNum;
    char errorCode;
    errorInf(int line, char code)
    {
        this->lineNum = line;
        this->errorCode = code;
    }
    bool operator<(const errorInf &other) const
    {
        //return lineNum < other.lineNum;
        if (lineNum < other.lineNum)
            return true;
        else if ((lineNum == other.lineNum) && errorCode < other.errorCode)
            return true;
        else
            return false;
    }
    bool operator==(const errorInf &other) const
    {
        return (lineNum == other.lineNum) && (errorCode == other.errorCode);
    }
};

struct Word
{
    string symbol;
    string content;
    int line;
    Word()
    {
        this->symbol = "";
        this->content = "";
        this->line = 0;
    }
};

string symStr[] = {
    "IDENFR", "INTCON", "STRCON", "MAINTK", "CONSTTK", "INTTK", "BREAKTK", "CONTINUETK", "IFTK", "ELSETK",
    "WHILETK", "GETINTTK", "PRINTFTK", "RETURNTK", "VOIDTK", "NOT", "AND", "OR", "PLUS", "MINU",
    "MULT", "DIV", "MOD", "LSS", "LEQ", "GRE", "GEQ", "EQL", "NEQ",
    "ASSIGN", "SEMICN", "COMMA", "LPARENT", "RPARENT", "LBRACK", "RBRACK", "LBRACE", "RBRACE", "ANNOTATION", "NONE"};

const string reserverWord[] = {
    "main", "const", "int", "break", "continue", "if", "else",
    "while", "getint", "printf", "return", "void"};

const string symTabItemTypeStr[] = {
    "ConstType", "VarType", "ParaType", "FuncType", "NoneItemType"};

const string symTabValTypeStr[] = {
    "IntType", "VoidType", "OneDimType", "TwoDimType", "NoneValType"};

const string OperatorStr[] = {
    "OpAdd", "OpMinus", "OpMulti", "OpDiv", "OpMod", "OpNot", "OpAssign", "OpGet", "OpOut",
    "OpLabel", "OpDeclare", "OpArrayInit", "OpArrayGet", "OpBge",
    "OpBgt", "OpBle", "OpBlt", "OpBeq", "OpBne", "OpAnd", "OpOr", "OpJ", "OpBr", "OpFuncDeclare",
    "OpFuncCall", "OpFuncReturn", "OpPushParam", "OpMain", "OpMv", "OpNone"};

enum symTabItemType { ConstType,
                      VarType,
                      ParaType,
                      FuncType,
                      NoneItemType
};

enum symTabValType
{
    IntType,
    VoidType,
    OneDimType,
    TwoDimType,
    NoneValType
};

enum symGenType
{
    INTGEN,
    STRINGGEN,
    EXPRESSIONGEN,
    FINDGEN,
    NoneGenSymType
};

enum scopeType
{
    Global,
    Local,
    NoneScopeType
};

enum symConditionType
{
    IFBEGINTYPE,
    IFELSETYPE,
    IFENDTYPE,
    WHILEBEGINTYPE,
    WHILEENDTYPE,
    ORBEGINTYPE,
    ORENDTYPE,
    ANDBEGINTYPE,
    ANDENDTYPE,
    FUNCENDTYPE,
    NoneConditionType
};

enum conScopeType
{
    orScope,
    andScope,
    noneConScope
};

enum funcScopeType
{
    inFuncType,
    outFuncType
};

scopeType scope = Global;
int blockNum = 0;
vector<int> blockNumVector = {0};

struct symtab
{
    string IDENFRName;                 //符号名
    symTabItemType ItemType;           //ConstType,VarType,ParaType,FuncType,NoneItemType
    symTabValType ValType;             //IntType,VoidType,OneDimType,TwoDimType,NoneValType
    int dimension1;                    //一维的尺度
    int dimension2;                    //二维的尺度
    vector<symtab> params;             //函数参数
    int block;                         //Block编号
    int offset;                        //地址偏移量
    int value;                         //代表的值
    symGenType GenType;                //代码生成所属类型
    vector<int> elementArray;          //数组中元素的值
    scopeType ScopeType;               //全局还是局部
    vector<symtab> initArray;          //初始化数组sym
    int len;                           //所占长度
    int index;                         //所处位置
    int findIndex1;                    //查找位置
    int findIndex2;                    //查找数组中位置
    string funcName;                   //参数对应的函数名
    int dimNum;                        //几维
    symConditionType ConditionType;    //条件类型
    funcScopeType FuncScopeType;       //在函数内还是函数外
    vector<symtab> vmSymVector;        //vm中函数中变量压栈
    int funcBlockNum;                  //vm中在哪个函数block中
    string belongFuncName;             //vm中在哪个函数名中
    symtab(string name, symTabItemType itemType, symTabValType valType, symGenType genType)
    {
        IDENFRName = name;
        ItemType = itemType;
        ValType = valType;
        dimension1 = 0;
        dimension2 = 0;
        block = blockNumVector.back();
        offset = 0;
        value = 0;
        GenType = genType;
        ScopeType = scope;
        len = 0;
        index = -1;
        findIndex1 = -1;
        findIndex2 = -1;
        funcName = "";
        dimNum = 0;
        ConditionType = NoneConditionType;
        FuncScopeType = outFuncType;
        funcBlockNum = 0;
        belongFuncName = "";
    }
    symtab(string name, symTabItemType itemType, symTabValType valType)
    {
        IDENFRName = name;
        ItemType = itemType;
        ValType = valType;
        dimension1 = 0;
        dimension2 = 0;
        block = blockNumVector.back();
        offset = 0;
        value = 0;
        GenType = NoneGenSymType;
        ScopeType = scope;
        len = 0;
        index = -1;
        findIndex1 = -1;
        findIndex2 = -1;
        funcName = "";
        dimNum = 0;
        ConditionType = NoneConditionType;
        FuncScopeType = outFuncType;
        funcBlockNum = 0;
        belongFuncName = "";
    }
    symtab(string name, int val, symGenType genType)
    {
        IDENFRName = name;
        ItemType = NoneItemType;
        ValType = NoneValType;
        dimension1 = 0;
        dimension2 = 0;
        block = blockNumVector.back();
        offset = 0;
        value = val;
        GenType = genType;
        ScopeType = scope;
        len = 0;
        index = -1;
        findIndex1 = -1;
        findIndex2 = -1;
        funcName = "";
        dimNum = 0;
        ConditionType = NoneConditionType;
        FuncScopeType = outFuncType;
        funcBlockNum = 0;
        belongFuncName = "";
    }
    symtab(string name, int val, symConditionType conditionType)
    {
        IDENFRName = name;
        ItemType = NoneItemType;
        ValType = NoneValType;
        dimension1 = 0;
        dimension2 = 0;
        block = blockNumVector.back();
        offset = 0;
        value = val;
        GenType = NoneGenSymType;
        ScopeType = scope;
        len = 0;
        index = -1;
        findIndex1 = -1;
        findIndex2 = -1;
        funcName = "";
        dimNum = 0;
        ConditionType = conditionType;
        FuncScopeType = outFuncType;
        funcBlockNum = 0;
        belongFuncName = "";
    }
    symtab()
    {
        IDENFRName = "null";
        ItemType = NoneItemType;
        ValType = NoneValType;
        dimension1 = 0;
        dimension2 = 0;
        block = blockNumVector.back();
        offset = 0;
        value = 0;
        GenType = NoneGenSymType;
        ScopeType = scope;
        len = 0;
        index = -1;
        findIndex1 = -1;
        findIndex2 = -1;
        funcName = "";
        dimNum = 0;
        ConditionType = NoneConditionType;
        FuncScopeType = outFuncType;
        funcBlockNum = 0;
        belongFuncName = "";
    }
};

struct MidCode
{
    Operator op;
    vector<symtab> symtabs;
    MidCode(Operator Op, vector<symtab> Symtabs) : op(Op), symtabs(Symtabs) {}
};

ifstream inFile;
ofstream outFile;
ofstream errorFile;
ofstream mipsFile;
ofstream midCodeFile;
ofstream resultFile;
char input[MAX];
string token;
int indexNum = 0;
int len = 0;
int wordIndex = 0;
string nextSym;
symbol sym;
int num;
Word word[WORD_NUM];
int printWordIndex = 0;
int line = 1;
vector<errorInf> errorArray;
int symTabNum = 0;
vector<symtab> globalSymbolTable;
vector<symtab> localSymbolTable;
int saveNum;
string saveSym;
bool syntaxTry = false;
string currentFuncName;
int tmpErrorLineNum;
symTabValType currentFuncParaValType = NoneValType;
symTabValType currentFuncReturnType = NoneValType;
bool funcHaveReturn = false;
vector<symTabValType> funcReturnTypes;
int inLoopStack;
int currentReturnBlock = -1;
vector<MidCode> midCodes;
vector<string> mipsCodes;
vector<symtab> genGlobalSymbolTable;
vector<symtab> genLocalSymbolTable;
vector<string> strList;
stack<char> opter; //运算符栈
stack<int> opval;  //操作数栈
symtab currentSym = symtab();
vector<Operator> genOperatorList;
bool isDef = false;
string expStr = "";
bool isExp = false;
vector<int> expValList;
string dimStr = "";
bool isDim = false;
vector<int> dimValList;
bool isAssign = false;
int genSymNameIndex = 0;
bool isDecl = false;
bool isConst = false;
bool isInit = false;
vector<symtab> initList;
symtab returnSym = symtab("@ret", VarType, IntType);
vector<symtab> vmSymList;
int vmSymIndex = 0;
int funcRetVal = 0;
vector<symtab> vmParaList;
int ifBlockNum = 0;
int whileBlockNum = 0;
int orBlockNum = 0;
int andBlockNum = 0;
vector<int> ifBlockNumVector = {0};
vector<int> whileBlockNumVector = {0};
vector<int> orBlockNumVector = {0};
vector<int> andBlockNumVector = {0};
int conditionResult = 0;
vector<conScopeType> conScopes = {noneConScope};
bool andNoRunFlag = false;
bool orNoRunFlag = false;
int runIndex = 0;
vector<symtab> whileBeginSymVector;
vector<symtab> whileEndSymVector;
vector<int> noRunOrBlockVector = {0};
vector<int> noRunAndBlockVector = {0};
vector<int> runIndexVector;
unordered_map<int, int> ifbeginMap;
unordered_map<int, int> ifelseMap;
unordered_map<int, int> ifendMap;
unordered_map<int, int> whilebeginMap;
unordered_map<int ,int> whileendMap;
unordered_map<string, int> funcMap;
unordered_map<string, int> vmSymIndexMap;
vector<string> vmFuncSymNameVector;
queue<int> funcRetValQueue;
int vmFuncBlockNum = 0;
int executeNum = 0;
double vmOpRunTime[100];

void getChar();
void clearToken();
bool isCarriage();
bool isSpace();
bool isNewline();
bool isTab();
bool isLetter();
bool isDigit();
bool isColon();
bool isComma();
bool isSemi();
bool isEqu();
bool isPlus();
bool isMinus();
bool isDiv();
bool isStar();
bool isMod();
bool isLpar();
bool isRpar();
bool isBType();
void catToken();
void retract();
int reserver();
int transNum();
void error();
symbol getSym();
void printWordSymbol();
void syntaxCompUnit();
void syntaxMainFuncDef();
void syntaxDecl();
void syntaxConstDecl();
void syntaxConstDef();
void syntaxConstInitVal();
void syntaxConstExp();
void syntaxVarDecl();
void syntaxVarDef();
void syntaxInitVal();
void syntaxFuncDef();
void syntaxFuncType();
void syntaxFuncFParams();
void syntaxFuncFParam();
void syntaxBlock();
void syntaxBlockItem();
void syntaxStmt();
void syntaxAddExp();
void syntaxMulExp();
void syntaxUnaryOp();
void syntaxUnaryExp();
void syntaxPrimaryExp();
void syntaxFuncRParams();
void syntaxCond();
void syntaxLOrExp();
void syntaxLAndExp();
void syntaxEqExp();
void syntaxRelExp();
void syntaxLVal();
void syntaxExp();
void syntaxAnalyz();
void syntaxAddExpExtend();
void syntaxMulExpExtend();
void syntaxRelExpExtend();
void syntaxEqExpExtend();
void syntaxLAndExpExtend();
void syntaxLOrExpExtend();
void printError();
void addError(int line, char code);
void getNextSym();
bool insert(string name, symTabItemType itemType, symTabValType valType);
bool findInCurrentSymTab(string name);
bool findInAllSymTab(string name);
symTabItemType findItemTypeInAllSymTab(string name);
symTabValType findValTypeInAllSymTab(string name);
vector<symtab> getFuncParams(string name);
void localUpdate();
void saveNowEnv();
void recoverEnv();
void printGlobalTable();
void printAllTable();
void addMidCode(Operator op, symtab sym1);
void addMidCode(Operator op, symtab sym1, symtab sym2);
void addMidCode(Operator op, symtab sym1, symtab sym2, symtab sym3);
string lw(string grf, string name);
string sw(string grf, string name);
int getOffset(string name);
scopeType getScope(string name);
int getValue(string name, vector<int> dims);
int calValue(string name);
void genGlobalVar();
void genStr();
void genFunc(string name);
void generate();
void printMidCode();
void printMips();
int getThetaIndex(char theta);
char getThetaPriority(char theta1, char theta2);
int thetaCal(int b, char theta, int a);
int getCalResult(string str);
symtab findSymbol(string name);
void changeSymVal(int val);
void addGenOp(Operator op);
Operator getGenOp();
void clearExpStr();
void printAllTableVal();
void printGenTableVal();
void printVmSymListVal();
void printSymVal(symtab sym);
void cleardimStr();
vector<string> splitWithPattern(const string &str, const string &pattern);
int addStr(string str);
string genNewSymName();
void run();
void execute(MidCode i);
void runCalc(MidCode midCode);
void runAssign(MidCode midCode);
void runGet(MidCode midCode);
void runPrint(MidCode midCode);
void runLabel(MidCode midCode);
void runDecl(MidCode midCode);
void runArrayInit(MidCode midCode);
void runArrayGet(MidCode midCode);
void runArrayPush(MidCode midCode);
void runFuncDecl(MidCode midCode);
void runFuncCall(MidCode midCode);
void runFuncReturn(MidCode midCode);
void runPushParam(MidCode midCode);
int GetVal(symtab sym);
void SetVal(symtab sym1, symtab sym2);
void addVmSymList(symtab sym);
void addVmParaList(symtab sym);
symtab findSymInVm(symtab sym);
void changeSymName(symtab &sym);
string getOriginFuncName(string str);
void changeSymInVm(symtab sym, int val);
bool haveSymInVm(symtab sym);
symtab getTrueSym(symtab sym);
void runCalNot(MidCode midCode);
void runBr(MidCode midCode);
void runJ(MidCode midCode);
void runAnd(MidCode midCode);
void runOr(MidCode midCode);
void process(MidCode i);
void SetVal(symtab sym, int val);
void printSingleMidCode(MidCode i);
void runMv(MidCode midCode);
int findLabelIndex(symtab sym);
void jump(symtab sym);
void judgeLabel(MidCode midcode);
int findFuncIndex(symtab sym);
void SetSym(symtab sym1, symtab sym2);
void setVmIndex();
void vmFuncEnd();
bool isInVmFunc();
void vmFuncClear();
void coutSingleMidCode(MidCode i);