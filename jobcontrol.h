#ifndef JOBCONTROL_H
#define JOBCONTROL_H
#include "commonfunction_c.h"
#include "clscli.h"
#include "common.h"
#include "netmessage.h"

#define INT_CHECK_CACULATOR 0
#define INT_ADD_CACULATOR 1
#define INT_REMOVE_CACULATOR 2
#define INT_JOBCONTROL_CREATOR 1
#define INT_JOBCONTROL_CLIENT 2
#define MAX_COMMAND_LENGTH 1024
#define MAX_ARG_LENGTH 2048
#define MIN_SHAREMEMORY_SIZE 4096
#define INT_ARGUMENT_JOBNUM_POS 0
#define INT_ARGUMENT_JOBNUM_LENGTH 2
#define INT_ARGUMENT_JOBNAME_LENGTH 8
#define INT_ARGUMENT_SHOTNUM_LENGTH 8
#define INT_ARGUMENT_JOBSHOT_LENGTH (INT_ARGUMENT_JOBNAME_LENGTH+INT_ARGUMENT_SHOTNUM_LENGTH)
#define INT_ARGUMENT_JOBKEY_POS 0
#define INT_ARGUMENT_JOBKEY_LENGTH 6

using namespace commonfunction_c;
namespace correspond_function
{
enum eJobStatus
{
    JS_WAIT = 0,
    JS_SETCMDOK = 0001, //服务端设置命令成功
    JS_WORKING = 0002,
    JS_SUSPEND = 0004,
    JS_SHOTDONE = 0010,//当前炮计算完成
    JS_SHOTRETURNED = 0020, //当前炮信息回传（服务端）完成
    JS_TOTALFINISHED = 0040,
    JS_ERROR = -01,
    JS_EMPTY = -02
};

enum eCaculatorStatus
{
    CS_EMPTY = 0,
    CS_FREE = 0001,
    CS_WORKING = 0002,
    CS_SHUTDOWN = 0004,
    CS_ERROR = 0010
};

enum eJobType
{
    JT_MAP_NORMAL = 0010,
    JT_REDUCE = 0020,
    JT_STACK = 0030
};

typedef struct JobInfos
{
    eCaculatorStatus caStatus;
    eJobStatus jobStatus;
    eJobType jobType; //计算任务类型
    int jobName;  //实际上是一个整形，每个计算任务都会有一个独有的name
    int caculatorId;
    time_t createTime; //shot信息生成时间（不是内存开辟时间，那个无需记录）
    time_t startTime;  //开始加算的时间
    time_t endTime;    //结束计算的时间
    char cmd[MAX_COMMAND_LENGTH];
    char arg[MAX_ARG_LENGTH];
    int getArgKeyNum(){
        return BaseFunctions::Chars2Int(arg,INT_ARGUMENT_JOBKEY_LENGTH);
    }

    JobInfos(){
        caStatus = CS_EMPTY;
        jobStatus = JS_EMPTY;
        jobType = JT_MAP_NORMAL;
        memset(cmd,0,MAX_COMMAND_LENGTH);//!watch out
        memset(arg,0,MAX_ARG_LENGTH);
    }

    JobInfos(JobInfos &other){
        if (this == &other) //ops... ...
            return;
        this->caStatus = other.caStatus;
        this->jobStatus = other.jobStatus;
        this->jobName = other.jobName;
        this->caculatorId = other.caculatorId;
        this->createTime = other.createTime;
        this->startTime = other.startTime;
        this->endTime = other.endTime;
        this->jobType = other.jobType;
        strncpy(this->cmd,other.cmd,MAX_COMMAND_LENGTH);
        strncpy(this->arg,other.arg,MAX_ARG_LENGTH);
    }

    JobInfos &operator=(JobInfos &right){
        if (this == &right)
            return *this;
        this->caStatus = right.caStatus;
        this->jobStatus = right.jobStatus;
        this->jobName = right.jobName;
        this->caculatorId = right.caculatorId;
        this->createTime = right.createTime;
        this->startTime = right.startTime;
        this->endTime = right.endTime;
        this->jobType = right.jobType;
        strncpy(this->cmd,right.cmd,MAX_COMMAND_LENGTH);
        strncpy(this->arg,right.arg,MAX_ARG_LENGTH);
        return *this;
    }
}stc_JobInfos;

class JobControl
{
public:
    JobControl(int type):m_Type(type){m_ConcurrentNum = 1;m_ID = NULL; m_IDCount = 0;m_Imp = NULL; m_Der = NULL;m_Jobs = NULL;m_Shared = false;}
    JobControl(int type, int maxConcurrent):m_Type(type),m_ConcurrentNum(maxConcurrent){m_ID = NULL; m_IDCount = 0;m_Imp = NULL; m_Der = NULL;m_Jobs = NULL;m_Shared = false;}
    JobControl(int type, int maxConcurrent, int *id, int idCount):m_Type(type),m_ConcurrentNum(maxConcurrent),m_IDCount(idCount){
        if (id == NULL){
            idCount = 0;
        }
        else
            m_ID = new int[idCount];
        for (int i = 0; i < idCount ; i++)
            m_ID[i] = id[i];
        m_Imp = NULL;
        m_Der = NULL;
        m_Jobs = NULL;
        m_Shared = false;
    }
    JobControl(JobControl &other){
        if (this == &other)
            return;
        this->m_Type = INT_JOBCONTROL_CLIENT;
        this->m_ConcurrentNum = other.m_ConcurrentNum;
        this->m_IDCount = other.m_IDCount;
        this->m_Imp = NULL;
        this->m_Der = NULL;
        this->m_Jobs = NULL;
        if (other.m_ID == NULL){
            this->m_IDCount = 0;
        }
        else
            this->m_ID = new int[this->m_IDCount];
        for (int i = 0; i < this->m_IDCount ; i++)
            m_ID[i] = other.m_ID[i];
        m_Shared = false;
    }



    JobControl &operator =(JobControl &right){
        if (this == &right)
            return *this;
        this->m_Type = INT_JOBCONTROL_CLIENT;
        this->m_ConcurrentNum = right.m_ConcurrentNum;
        this->m_IDCount = right.m_IDCount;
        if (right.m_ID == NULL){
            this->m_IDCount = 0;
        }
        else
            this->m_ID = new int[this->m_IDCount];
        for (int i = 0; i < this->m_IDCount ; i++)
            m_ID[i] = right.m_ID[i];
        this->m_Imp = NULL;
        this->m_Der = NULL;
        this->m_Jobs = NULL;
        m_Shared = false;
    }

    ~JobControl(){
        end();
        if (m_ID != NULL) delete [] m_ID;
        if (m_Der != NULL) delete m_Der;
        if (m_Imp != NULL) delete m_Imp;
    }
    bool begin(const char *path, int ipcKey, int maxConcurrent = -1, int *id = NULL, int idCount = -1);    //setup can only call once, if m_Id not NULL , return -1;
    bool end();
    int AddCaculator(int id); //return added caculator array index, -1 means fail
    int RemoveCaculator(int id); // -1 means fail

    int GetCount(){return this->m_IDCount;}
    bool UpdateInfos(JobInfos t,int id); // not recommand to use
    bool UpdateCaculatorStatus(eCaculatorStatus caStatus, int id);
    bool UpdateJobStatus(eJobStatus jobStatus, int id);
    bool UpdateTime(time_t *create,time_t *start, time_t *end,int id);// NULL point means do not update
    bool UpdateJobName(int name,int id);
    bool UpdateJobCmd(const char* cmd, int id);
    bool updateJobType(eJobType,int);

    bool FreeInfos(int id);
    void ClosePipeCmd(const char *path, int ipcKey);  //to directly close mem-pipe,return false; //can not do closePipe in normal function call(if call shmInitial before ,cpc will return false)
    int getJobCount(int jobName);
    int analyseJobCMD(bool (*ptr)(const char*), int jobName); //通过函数指针让外部的逻辑函数来分析内部cmd的值，返回符合条件的job总数
    bool Shared(){return m_Shared;} //if share memory linked ,return true , else return false;
    int appendMapJob(int jobName);  //when append one new map job,increase reduce job arg count!
    JobInfos &GetInfos(int id);

private:
    bool m_Shared; //if share memory linked ,return true , else return false;
    int m_Type; //jobControl类型 1 Creator ，负责开启及清空共享内存 ， 2 Client ，只负责读写不服则系统内存管理
    int m_ConcurrentNum;//最大并行计算进程数,因为共享内存是进程开启时开辟，所以最大进程数和共享内存一样不可在运行中修改
    int *m_ID;  //激活的计算进程编号
    int m_IDCount; //激活的计算进程总数
    JobInfos *m_Jobs; //记录当前计算命令相关信息，结构数组
    IShareMemImp<stc_JobInfos> *m_Imp;  // 桥模式设计的内存共享执行类
    IShareMemDerive<stc_JobInfos> *m_Der; //内存共享管道ipc管理类
    ShmInitials m_ShmIni;
    /////////////////////////////////////////////////////////////////////////////////////////
    /// \brief confirm
    /// \param id
    /// \param performance
    /// \return
    /////do confirm before add or remove caculator , check job mission, log any warning or error;
    ///when add , return -1 means can not add , other num means index for id ,if id >= count means new id
    ///   otherwise ,means repeat
    ///when remove , return -1 means can not remove (not found or caculator in working)
    ///  all job control by caculator must be finished  before remove

    int confirm(int id, int performance/* add = 1, remove = 2 , use marco recommand! */);
    /////////////////////////////////////////////////////////////////////////////////////////
    int CreateInfos();  //shm functions , any creator use this function to create sys shm
    int ConnectInfos();  // get shm address ,connect sys mem and local point
    int GetShmAddr();
    int DtInfos();      //detache loacl point and sys mem
    int ClearInfos();   // clear and delete sys shm, only creator type use
    void shmInitial(const char *path, int ipcKey);

    bool JobInitials();//job initials, create m_jobs id infos base m_Id;
    int findReduceIndex(int jobName); //to find an reduce process in shm , if no reduce ,return -1
};

//共享内存中包含2048个字符的定长参数，本类负责写入和读出
//前2个字节存储map&stack类型的job数量
//之后会有 16* jobNum个字节存储每个map&stack job的shot数量,这16个字节是按照jobName(8)+shotCount(8)组成的
class jncArgFunctions
{
public:
    jncArgFunctions(){;}
    jncArgFunctions(char *c){strncpy(m_chars,c,MAX_ARG_LENGTH);}
    void setChars(char *c){strncpy(m_chars,c,MAX_ARG_LENGTH);}
    const char *getChars();
    int getMapJobNum(); //total job&stack num
    void addMapJob(int jobName); //new jobName will increase mapJobNum
    void moveMapJob(int jobName); //move jobName will decrease mapJobNum
private:
    char m_chars[MAX_ARG_LENGTH];
    bool isFreeMemEnough(int); //check Free memory enough ,must call before append new args .
    inline int jobNumUse(){return (INT_ARGUMENT_JOBNUM_LENGTH+getMapJobNum()*INT_ARGUMENT_JOBSHOT_LENGTH);}
    void appendChar(const char* chars,int pos,int length);
    void removeChar(int pos , int length);
    inline int sizeMoved(int pos, int length){
        assert(pos >= 0);
        assert(length > 0);
        int sizeMoved = MAX_ARG_LENGTH-pos-length;
        assert(sizeMoved >= 0);
        return sizeMoved;
    }
};

}
#endif // JOBCONTROL_H
