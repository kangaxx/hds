/**************************************************
 *             Add by gxx  20140303               *
 *             字符命令控制模式相关功能。              *
 *************************************************/
#ifndef CLSCLI_H
#define CLSCLI_H
#include "stdio.h"
#include "stdlib.h"
#include "iostream"
#include "string.h"

using namespace std;
enum ECLIParaStatus
{
    NoCLIError,
    ParamRequired,
    ParamRepeated,
    NoPara,
    ParaError
};

class CCLI
{
public:
    CCLI(string &CMD);
    ~CCLI();

    void Debug(); //debug gxx 20140306

    //Getter and Putter
    /*******              begin               *********/
    string &GetAlgoName();
    int GetNum();
    int GetRepeat();
    string &GetFileName();
    string &GetOutputPath();
    string &GetExePath();
    string getUserName(){return m_userName;}
    string getNetMessage(){return m_netMessage;}
    string &GetArithParam(){return m_ArithParam;} //algo name
    string &GetCstParam(){return m_CstParam;}  //customer defined params!
    string &GetProcParam(){return m_netMessage;} //same as netMessage

    void SetAlgoName(string &AlgoName);
    void SetNum(int Num);
    void SetRepeat(int Repeat);
    void SetFileName(string &FileName);
    void SetOutputPath(string &OutputPath);
    void SetExePath(string &ExePath);
    void setUserName(string userName){m_userName = userName;}
    void setNetMessage(string netMessage){m_netMessage = netMessage;} //设定netmessage，netmessage是一组记录了jobname，shotnum等的字符串，长度固定
    void SetArithParam(string &ArithParam){m_ArithParam = ArithParam;}
    void SetCstParam(string &CstParam){m_CstParam = CstParam;}
    void SetProcParam(string &ProcParam){setNetMessage(ProcParam);}
    /********              end               **********/



private:
    string m_CMD;  //CLI模式下的输入或者其他程序调用时传入的完整命令。
    string m_AlgoName; //参数算法名称  命令行表示 -nXXXX 或者 name=XXXXX
    int m_Num;      //参数 编号    命令行表示 -iXXXX 或者 num=XXXX 默认为 1
    int m_Repeat;   //参数 重复次数 命令行表示 -rXXXX 或者 rep=XXXX 默认为 1
    string m_userName; //参数 任务所属用户名 -uXXX 或 user=XXXX 默认为空
    string m_FileName; //参数 原始文件名 中间文件和处理文件都会和原始文件名有一定的相关性 命令行表示 -sXXXX或者source=
    string m_OutputPath; //参数 强制指定输出路径 -oXXXX 或者 out=XXXX 不指定的话就使用资源文件缩在路径作为输出路径
    string m_netMessage; //参数 获取前端发送来的NetMessage对象字符 -pXXXX 或者 ppm=XXXX
    string m_ExePath; //表示当前程序所处路径
    string m_ArithParam;     //算法相关参数 -aXXXX 或者 ari = XXXX
    string m_CstParam;    //自定义参数 -c 或者 cst = XXX
    //analysis command line
    /*******              begin               *********/
    string &FindAlgoName(string &CMD, string &Dest);//参数算法名称  命令行表示 -nXXXX 或者 name=XXXXX
    string &FindNum(string &CMD, string &Dest);//参数 编号    命令行表示 -iXXXX 或者 num=XXXX 默认为 1
    string &FindRepeat(string &CMD, string &Dest);//参数 重复次数 命令行表示 -rXXXX 或者 rep=XXXX 默认为 1

    //参数 原始文件名 中间文件和处理文件都会和原始文件名有一定的相关性 命令行表示 -sXXXX 或者source=
    string &FindFileName(string &CMD, string &Dest);
    //参数 强制指定输出路径 -oXXXX 或者 out=XXXX 不指定的话就使用资源文件缩在路径作为输出路径
    string &FindOutputPath(string &CMD, string &Dest);
    //参数 计算任务所属用户，影响启动计算进程的参数 -uXXXX 或者 username=XXXX
    string &findUserName(string &CMD, string &Dest);
    //参数 获取前端发送来的NetMessage对象字符 -pXXXX 或者 pm=XXXX
    string &findNetMessage(string &CMD, string &Dest);
    //参数 算法相关参数 -aXXXX 或者 ari = XXXXX
    string &FindArithPara(string &CMD, string &Dest);
    //参数 自定义参数 -c 或者 cst = XXX
    string &FindCstPara(string &CMD, string &Dest);

    //SAME AS findNetMessage
    inline string &FindProcPara(string &CMD, string &Dest){return findNetMessage(CMD,Dest);}
    string &FindParaByName(string &CMD , string *pParamList[], string &dest, string Default);

    ECLIParaStatus ParamAnalysis(string &CMD, string *ParamFlags[],int Len, string &Dest);
    int CountParamTime(string &CMD, string *ParamFlags[], int Len);
    void InitialWithCMD(string &CMD);
    /********              end               **********/

};

#endif // CLSCLI_H
