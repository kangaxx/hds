#ifndef COMMONFUNCTION_C_H
#define COMMONFUNCTION_C_H
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <string>
#include <regex.h>
#include <assert.h>

using namespace std;
#define MAX_INIFILE_LINE_LENGTH 64
#define CHAR_INIFILE_PARAMETER_SPLIT_FLAG '='
#define DEFAULT_SHAREMEME_SIZE 65536
#define ERROR_SYSSHM_NORMAL -1
#define DEFAULT_INTCHAR_SIZE 9
#define DEFAULT_DATETIME_LEN 20
#define DEFAULT_MM_DD_HHMMSS_LEN 2
#define DEFAULT_DATETIME_FMT "%Y-%m-%d %I:%M:%S"
#define DEFAULT_LINUX_PATH_LENGTH 1024
#define CHARS_QMODEL_SYSENV_ROOT "QM_ROOT"
#define CHARS_QMODEL_LINUX_PATH_PARENT_FLAG "/.."
#define CHARS_QMODEL_CONFIG_PATH_NAME "/config/"
namespace commonfunction_c {

class BaseFunctions
{
public:
    enum eFindPathMode{FPM_ALL=0007,FPM_SYSENV=0001,FPM_PATHWORK=0002,FPM_PATHSTR=0004};
    BaseFunctions(){m_FileName = ""; m_FilePath = "";}
    BaseFunctions(string fileName, string filePath=""):m_FileName(fileName),m_FilePath(filePath){}
    ~BaseFunctions(){}
    static int GetWorkPath(char *dest); //get program run time path info!
    static const char* getConfigPath(int mode=FPM_ALL); //mode : FPM_ALL,exec in the following order: SYSENV,PATHSTR,PATHWORK
    static string GetParaByName(string fileName, string name);//work for new type ini,ex: "password=123456";
    static string GetParaByLine(string fileName, int lineNum);//work for old type,just have value ex : "123456"

    static char *Int2Chars(int in, char *out, int size=DEFAULT_INTCHAR_SIZE);
    static int Chars2Int(const char *in, int size=DEFAULT_INTCHAR_SIZE);
    static int Str2Int(string s);
    static int Str2Int(string s, int i);
    static string time2str(time_t time);//transfer time 2 string
    static time_t str2time(string str); //transfer string 2 time,from '1970-01-01 00:08:00'
    static string cutValue(string wholeStr,string regExp); //find regExp in wholeStr, in regExp, word "value" is the return,
                                                               //exp: reduce_type|value|,will get "aaa" in whole string "reduce_type|aaa|"
    static string combineFilePath(string path,string fileName); //combine file name and path name , will confirm if there is an '/' between path and file;
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //getProcArgv: to get process argument, set *title to argu name ,like "-l" ,
    //             it will try to match three mode successively, "-labc" , "-l=abc", "-l abc -other" ,
    //             if matched , result is true , and dest will set to abc.
    //             specially , "ls -l" or "ls -l -abc", still return true , dest set to '\0';
    //             dest could be NULL if no or not need value;
    static bool getProcArgv(int argc,const char *argv[],const char *title, char *dest);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
    string m_FileName,m_FilePath;
    static char *getSysEnvConfigPath(char *dest); //get system env for path , if not found env or path , rutern NULL;
    static int freadLine(void *buffer, int size, FILE *f);
    static int strcmp_nonsafe(const char *c1, const char *c2); // result 0  : c1 == c2 , 1 : c2 include c1 , -1 : c2 not include c1
};

typedef struct ShmInitials
{
    const char *path;// for linux sys/shm.h function now
    int ipcKey; //for linux sys/ipc.h ftok now
    int size;   //the share memory's capcity chars num;
}stc_ShmInitials;


template<class T>
class IShareMemImp
{
public:
    IShareMemImp(){}
    IShareMemImp(stc_ShmInitials &i) {}
    virtual ~IShareMemImp() = 0;
    virtual void Initial(stc_ShmInitials &i) = 0;
    virtual int CreateShmId() = 0; //create share memory and return sharemem id , which return by system function shmget,return -1 means error;
    virtual int GetShmId() = 0; //return the created share memory id(linux system ipc num),return -1 means error;
    virtual T *GetShmAddr() = 0; //Link share memory address point to link local point,return NULL Point means error;
    virtual int DtShm(T *t) = 0; //for linux shm ,it's detach local memory,not clear share memory ,return -1 means error;
    virtual int ClearShm() = 0; //for linux shm ,it's destory share memory both system and local;
};

template<class T>
class ShareMemImp_Linux:public IShareMemImp<T>
{
public:
    ShareMemImp_Linux(){m_Initial = NULL;}
    ShareMemImp_Linux(stc_ShmInitials &i):m_Initial(&i){}
    ~ShareMemImp_Linux();
    inline void Initial(stc_ShmInitials &i){m_Initial = &i;}
    inline int CreateShmId(){
        int shm_id = shmget(GetKey(),m_Initial->size,IPC_CREAT|IPC_EXCL|0666);
        cout << "create shm , id is : " << shm_id << endl;
        return shm_id;
    }
    inline int GetShmId(){
        int shm_id = shmget(GetKey(),0,0);
        return shm_id;
    }
    inline T *GetShmAddr(){
        T *result;
        int id = GetShmId();
        cout << "get shm addr by id : " << id << endl;
        if (id == ERROR_SYSSHM_NORMAL)
            return NULL;
        result = (T*)shmat(id,NULL,0);
        return result;
    }
    inline int DtShm(T *t){
        if (t == NULL)
            return ERROR_SYSSHM_NORMAL;
        return shmdt(t);
    }
    inline int ClearShm(){
        int temp = GetShmId();
        if (temp == ERROR_SYSSHM_NORMAL)
            return ERROR_SYSSHM_NORMAL;
        return shmctl(temp,IPC_RMID,NULL);
    }

private:

    stc_ShmInitials *m_Initial;
    inline int GetKey(){
        return ftok(this->m_Initial->path,this->m_Initial->ipcKey); //comment tempory
    }

};

template<class T>
class IShareMemAbstraction
{
public:
    virtual ~IShareMemAbstraction()=0;
    virtual int CreateShmId() = 0; //create share memory ,call imp's operation
    virtual int GetShmId() = 0; //call imp's operation
    virtual T *GetShmAddr() = 0; //call imp's operation
    virtual int DtShm(T *t) = 0; //call imp's operation
    virtual int ClearShm() = 0; //call imp's operation
};



template<class T>
class IShareMemDerive:public IShareMemAbstraction<T>
{
public:
    IShareMemDerive(IShareMemImp<T> &imp):m_Imp(&imp){}
    ~IShareMemDerive();
    inline int CreateShmId(){return m_Imp->CreateShmId();}
    inline int GetShmId(){return m_Imp->GetShmId();} //call imp's operation
    inline T *GetShmAddr(){return m_Imp->GetShmAddr();} //call imp's operation
    inline int DtShm(T *t){return m_Imp->DtShm(t);} //call imp's operation
    inline int ClearShm(){return m_Imp->ClearShm();} //call imp's operation
private:
    IShareMemImp<T> *m_Imp;
};

template<class T>
IShareMemAbstraction<T>::~IShareMemAbstraction()
{
    //nothing yet
}

template<class T>
IShareMemDerive<T>::~IShareMemDerive()
{
    //nothing yet
}

template<class T>
IShareMemImp<T>::~IShareMemImp()
{
    //nothing yet
}

template<class T>
ShareMemImp_Linux<T>::~ShareMemImp_Linux()
{
    //nothing yet
}

//环形双链表，存储数据高效，在无法确认数组长度时，链表无需反复申请释放大块内存的优势较明显。
template<class T>
class DuLink{
private:
    class Node{
    public:
        T data;
        Node *prior;
        Node *next;
        Node(T &element,Node *prior, Node *next):data(element){this->next = next;this->prior = prior; prior->next=next->prior = this;}
        Node():data(data){}
    };
    Node *head;
public:
    DuLink():head(new Node()){head->prior=head->next=head;}
    int size();

    T &operator [](int idx);
    //判断是否为空链
    bool isEmpty()const{return head==head->next?true:false;}
    //将元素添加至最后，注意node的指针设置
    void addToLast(const T& element){Node* ne=new Node(element,head->prior,head);head->prior=head->prior->next=ne;}
    //获取最后一个元素
    T getLastElement()const{assert(!isEmpty());return head->prior->data;}
    //删除最后一个元素，注意node的指针设置
    void delLastElement(){assert(!isEmpty());Node* p=head->prior->prior;delete head->prior;head->prior=p;p->next=head;}
    //修改最后一个元素
    void alterLastEmlent(const T& newElement){assert(!isEmpty());head->prior->data=newElement;}
    //插入元素
    void insert(T element, int pos){insertElement(element, pos);}
    void insertElement(T element, int pos);
    //获取元素
    T &get(int idx){return getElement(idx);}
    T &getElement(int idx);
    //删除元素
    T del(int idx){return delElement(idx);}
    T delElement(int idx);
    //修改元素
    void alter(const T &newElement, int idx){alterElement(newElement,idx);}
    void alterElement(const T &newElement,int idx);
    //查找元素
    int findElement(const T& element) const;
    //正序遍历
    void Traverse(void (*visit)(T&element));
    //逆序遍历
    void TraverseBack(void (*visit)(T&element));
    //清空链表
    void clear(){clearAllElement();}
    void clearAllElement();
    //销毁链表
    ~DuLink();
};


}


#endif // COMMONFUNCTION_C_H
