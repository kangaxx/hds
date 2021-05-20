#ifndef COMMONFUNCTION_C_H
#define COMMONFUNCTION_C_H
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <time.h>
#include <string>
#include <assert.h>
#include "common.h"
using namespace std;
#ifndef INT_MAKE_PATCH_SERIALNO
#define INT_MAKE_PATCH_SERIALNO "1.05.0000"
#endif

namespace commonfunction_c {

class BaseFunctions
{
public:
    enum eFindPathMode { FPM_ALL = 0007, FPM_SYSENV = 0001, FPM_PATHWORK = 0002, FPM_PATHSTR = 0004 };
    BaseFunctions() { m_FileName = ""; m_FilePath = ""; }
    BaseFunctions(string fileName, string filePath = "") :m_FileName(fileName), m_FilePath(filePath) {}
    ~BaseFunctions() {}
    static int GetWorkPath(char* dest);//get program run time path info!
    static const char* getConfigPath(int mode = FPM_ALL); //mode : FPM_ALL,exec in the following order: SYSENV,PATHSTR,PATHWORK
    static string GetParaByName(string fileName, string name);//work for new type ini,ex: "password=123456";
    static string GetParaByName_safe(string fileName, string name);
    static string GetParaByLine(string fileName, int lineNum);//work for old type,just have value ex : "123456"

    //找到数组的极值点（数组两端的两个元素不参与计算，因为无法判断它的另外一侧是什么情况）
    //参数含义，array 数组指针， size 数组总长度 ， index 待判断的位置, n 判断范围, type 判断类型（最大，最小或者全部）
    static int findInflection(const int* array, int size, int start, int end, int type) {
        assert(start >= 0);
        assert(end > start);
        if (end >= size)
            end = size - 1;
        int value = array[start];
        int result = start;
        for (int i = start + 1; i <= end; ++i) {
            if (type == TYPE_INFLECTION_MAX && array[i] >= value) {
                if ((i != end) || (array[i] > value)) {
                    result = i;
                    value = array[i];
                } 
            } else if (type == TYPE_INFLECTION_MIN && array[i] <= value) {
                if ((i != end) || (array[i] < value)) {
                    result = i;
                    value = array[i];
                }
            }

        }
        //两个边缘点不能判断是否是拐点，如果极值点在边缘则返回 -1 表示没有找到极致
        if (result == start || result == end)
            return -1;
        else
            return result;
    }

    //求一段数组内整数的平均值
    static int getArrayAverage(int* array, int size, int start, int end) {
        assert(start >= 0);
        assert(end > start);
        if (end >= size)
            end = size - 1;
        int count = end - start + 1;
        assert(count > 0);
        int sum = 0;
        for (int i = start; i <= end; ++i) {
            sum += array[i];
        }
        return sum / count;
    }

    static char* Int2Chars(int in, char* out, int size = DEFAULT_INTCHAR_SIZE, int radix = DEFAULT_INTCHAR_BASE) {
        _itoa_s(in, out, size, radix);
        return out;
    }

    static char* Long2Chars(long in, char* out, int size = DEFAULT_LONGCHAR_SIZE) {
        sprintf_s(out, size, "%ld", in);
        return out;
    }

    static long Chars2Long(char* in) {
        long out;
        sscanf_s(in, "%ld", &out);
        return out;
    }
    static int Chars2Int(const char* in, int size = DEFAULT_INTCHAR_SIZE);
    static int Str2Int(string s) {
        Str2Int(s, 0);
    }
    static int Str2Int(string s, int i) {
        int result = 0;
        const char* c = &s[0];
        while (*c != '\0') {
            if (*c < 48 || *c > 57)
                return i;
            result *= 10;
            result += *c - 48;
            ++c;
        }
        return result;
    }

    static string f2str(float in, const char* precision = "%.8f") {
        char out[64];
        sprintf_s(out, 64, precision, in);
        return string(out);
    }

    static float str2f(string in) {
        float out;
        sscanf_s(in.c_str(), "%f", &out);
        return out;
    }
#ifdef  __IS_WIN__
    static TCHAR* DWORD2WinLog(DWORD in, TCHAR* out) {
        wsprintf(out, _T("%d"), in);//应用
        return out;
    }


    static string ws2s(const wstring& ws) {

        size_t convertedChars = 0;
        //string curLocale = setlocale(LC_ALL, NULL); //curLocale="C"
        //setlocale(LC_ALL, "chs"); //comment by gxx, 默认情况下编译器会自动设置本地化
        const wchar_t* wcs = ws.c_str();
        size_t dByteNum = sizeof(wchar_t) * ws.size() + 1;

        char* dest = new char[dByteNum];
        wcstombs_s(&convertedChars, dest, dByteNum, wcs, _TRUNCATE);
        string result = dest;
        delete[] dest;
        //setlocale(LC_ALL, curLocale.c_str());
        return result;
    }

    static wstring s2ws(const string& s) {
        size_t convertedChars = 0;
        //string curLocale = setlocale(LC_ALL, NULL);   //curLocale="C"
        //setlocale(LC_ALL, "chs"); 
        const char* source = s.c_str();
        size_t charNum = sizeof(char) * s.size() + 1;
        wchar_t* dest = new wchar_t[charNum];
        mbstowcs_s(&convertedChars, dest, charNum, source, _TRUNCATE);
        wstring result = dest;
        delete[] dest;
        //setlocale(LC_ALL, curLocale.c_str());
        return result;
    }
#endif // __IS_WIN__
    static int time2str(const time_t& time, string& strDateStr) {
        char chTmp[20] = { '\0' };
        struct tm p;
        localtime_s(&p, &time);
        p.tm_year = p.tm_year + 1900;
        p.tm_mon = p.tm_mon + 1;
        snprintf(chTmp, sizeof(chTmp), "%04d-%02d-%02d-%04d",
            p.tm_year, p.tm_mon, p.tm_mday, p.tm_sec);
        strDateStr = chTmp;
        return 0;
    }

    static time_t str2time(string str); //transfer string 2 time,from '1970-01-01 00:08:00'
    //将日期转换位各种格式的int值，暂时只支持年加日（天数），后续增强各种功能。
    static int time2int(time_t time, string ="yyddd"); //transfer time 2 int
    static string cutValue(string wholeStr,string regExp); //find regExp in wholeStr, in regExp, word "value" is the return,
    //exp: reduce_type|value|,will get "aaa" in whole string "reduce_type|aaa|"
    static string combineFilePath(string path,string fileName); //combine file name and path name , will confirm if there is an '/' between path and file;
    static string combineFilePath_shout(string path,string fileName); //combine file , if file not access , will throw error
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //getProcArgv: to get process argument, set *title to argu name ,like "-l" ,
    //             it will try to match three mode successively, "-labc" , "-l=abc", "-l abc -other" ,
    //             if matched , result is true , and dest will set to abc.
    //             specially , "ls -l" or "ls -l -abc", still return true , dest set to '\0';
    //             dest could be NULL if no or not need value;
    static bool getProcArgv(int argc,const char *argv[],const char *title, char *dest);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief getSystemLogPath
    /// \param dest
    /// \param subPath
    ///  get QModeling's system log file path , base on env("QM_ROOT"), return path or null(fail, no env or other)
    static char *getSystemLogPath(char *dest);
    static char *getSystemIconPath(char *dest);
    //根据参数计算出软件版权期限
    static int getLicenseLimitDay(int dayBegin, int dayNum);


private:
    string m_FileName,m_FilePath;

    static char *getSysEnvConfigPath(char *dest); //get system env for path , if not found env or path , rutern NULL;
    static int freadLine(void *buffer, int size, FILE *f);
    static int strcmp_nonsafe(const char *c1, const char *c2); // result 0  : c1 == c2 , 1 : c2 include c1 , -1 : c2 not include c1
};

#ifdef __IS_LINUX__
typedef struct ShmInitials
{
    const char *path;// for linux sys/shm.h function now
    int ipcKey; //for linux sys/ipc.h ftok now
    int size;   //the share memory's capcity chars num;
}stc_ShmInitials;

//桥接模式设计下的共享内存管理模组，将LINUX或其他系统不同的共享内存行为包装成相同的上层行为，供用户使用
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
    ShareMemImp_Linux(){;}
    ShareMemImp_Linux(stc_ShmInitials &i):m_Initial(i){}
    ~ShareMemImp_Linux();
    inline void Initial(stc_ShmInitials &i){m_Initial = i;}
    inline int CreateShmId(){
        //开辟共享内存空间
        int shm_id = shmget(GetKey(),m_Initial.size,IPC_CREAT|IPC_EXCL|0666);
        return shm_id;
    }
    inline int GetShmId(){
        int shm_id = shmget(GetKey(),0,0);
        return shm_id;
    }
    inline T *GetShmAddr(){
        T *result;
        int id = GetShmId();
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
    stc_ShmInitials m_Initial;
    inline int GetKey(){
        return ftok(this->m_Initial.path,this->m_Initial.ipcKey); //comment tempory
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
#endif // __IS_LINUX__

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
        ~Node(){prior->next = next; next->prior = prior;}
    };
    Node *head;
    Node *current;
public:
    DuLink():head(new Node()){head->prior=head->next=head; current = NULL;}
    int size(){
        int count = 0;
        for (Node *p=head->next;p!= head;p=p->next)
            count ++;
        return count;
    }

    T &operator [](int idx){return getElement(idx);}
    //判断是否为空链
    bool isEmpty()const{return head==head->next?true:false;}
    //将元素添加至最后，注意node的指针设置
    void addToLast(const T& element){Node* ne=new Node(element,head->prior,head);}
	inline void insertToLast(const T& element) { return addToLast(element); }
    //获取最后一个元素
    T getLastElement()const{assert(!isEmpty());return head->prior->data;}
    //删除最后一个元素，注意node的指针设置
    void delLastElement(){assert(!isEmpty());Node* p=head->prior;delete p;}
    //修改最后一个元素
    void alterLastElement(const T& newElement){assert(!isEmpty());head->prior->data=newElement;}
    //插入元素
    void insert(T element, int pos){insertElement(element, pos);}
    void insertElement(T element, int pos){
        assert(pos >= 0);
        assert(pos < size());
        Node *p = head;
        while(pos-- != 0)
            p=p->next;
        new Node(element,p->prior,p);
    }

    //获取元素
    T &get(int idx){return getElement(idx);}
    T &getElement(int idx){
        assert(idx >= 0);
        assert(idx < size());
        Node *p = head->next;
        while(idx-- != 0)
            p=p->next;
        return p->data;
    }
    //删除元素
    T del(int idx){return delElement(idx);}
    T delElement(int idx){
        assert(idx >= 0);
        assert(idx < size());
        Node *t = head->next;
        while(idx-- != 0)
            t = t->next;
        T ret = t->data;
        delete t;
        return ret;
    }

    //修改元素
    void alter(const T &newElement, int idx){alterElement(newElement,idx);}
    void alterElement(const T &newElement,int idx){
        assert(idx >= 0);
        assert(idx < size());
        Node *t = head->next;
        while (idx-- != 0)
            t= t->next;
        t->data = newElement;
        return;
    }

    //查找元素 ， 返回值小于0表示未找到元素
    int findElement(const T& element) const{
        char *ori, *in;
        int result = 0;
        Node *t = head->next;
        while (t != head){
            ori = reinterpret_cast<char *>(&(t->data));
            in = reinterpret_cast<char *>(&element);
            if (strncmp(ori,in,sizeof(T)) == 0)
                return result;
            result++;
            t= t->next;
        }
        return -1;
    }
    //正序遍历
    void Traverse(void (*visit)(T&element)){
        Node *t = head->next;
        while(t != head){
            visit(t->data);
            t = t->next;
        }
    }

    //逆序遍历
    void TraverseBack(void (*visit)(T&element)){
        Node *t = head->prior;
        while (t != head){
            visit(t->data);
            t= t->prior;
        }
    }

    //清空链表
    void clear(){clearAllElement();}
    void clearAllElement(){
        Node *t,*p;
        p = head->next;
        while(p != head){
            t = p->next;
            delete p;
            p = t;
        }
    }

    //游标操作
    int getCurrentIdx(){
        assert(size() > 0);
        assert(head != head->next);
        int result = 0;
        if (current == NULL || current == head)
            return 0;
        else{
            Node *t = head->next;
            while (current != t){
                result++;
                t = t->next;
            }
        }
    }

    T& getCurrent(){
        assert(size() > 0);
        assert(head != head->next);
        if (current == NULL || current == head)
            current = head->next;
        return current->data;
    }
    T& goNext(){
        assert(size() > 0);
        assert(head != head->next);
        getCurrent();
        Node *p = current->next;
        if (p == head)
            p = p->next;
        current = p;
        return current->data;
    }

    T& goPrior(){
        assert(size() > 0);
        assert(head != head->prior);
        getCurrent();
        Node *p = current->prior;
        if (p == head)
            p = p->prior;
        current = p;
        return current->data;
    }

    //销毁链表
    ~DuLink(){
        clearAllElement();
        if (head != NULL){
            delete head;
            head = NULL;
        }
    }
};

//特化模板
//环形双链表，存储数据高效，在无法确认数组长度时，链表无需反复申请释放大块内存的优势较明显。
template<class T>
class DuLink<T*>
{
private:
    class Node{
    public:
        const T *data;
        Node *prior;
        Node *next;
        Node(const T *element,Node *prior, Node *next):data(element){this->next = next;this->prior = prior; prior->next=next->prior = this;}
        Node():data(NULL){}
        ~Node(){prior->next = next; next->prior = prior;}
    };
    Node *head;
    Node *current;
public:
    DuLink():head(new Node()){head->prior=head->next=head; current = NULL;}
    int size(){
        int count = 0;
        for (Node *p=head->next;p!= head;p=p->next)
            count ++;
        return count;
    }

    const T *operator [](int idx){return getElement(idx);}
    //判断是否为空链
    bool isEmpty()const{return head==head->next?true:false;}
    //将元素添加至最后，注意node的指针设置
    void addToLast(const T* element){new Node(element,head->prior,head);}
	inline void insertToLast(const T *element) { return addToLast(element); }
    //获取最后一个元素
    const T *getLastElement()const{assert(!isEmpty());return head->prior->data;}
    //删除最后一个元素，注意node的指针设置
    void delLastElement(){assert(!isEmpty());Node* p=head->prior;delete p;}
    //修改最后一个元素
    void alterLastEmlent(const T* newElement){assert(!isEmpty());head->prior->data=newElement;}
    //插入元素
    void insert(T element, int pos = 0){insertElement(element, pos);}
    void insertElement(T *element, int pos = 0){
        assert(pos >= 0);
        assert(pos < size());
        Node *p = head;
        while(pos-- != 0)
            p=p->next;
        new Node(element,p->prior,p);
    }

    //获取元素
    const T *get(int idx){return getElement(idx);}
    const T *getElement(int idx){
        assert(idx >= 0);
        assert(idx < size());
        Node *p = head->next;
        while(idx-- != 0)
            p=p->next;
        return p->data;
    }
    //删除元素
    const T *del(int idx){return delElement(idx);}
    const T *delElement(int idx){
        assert(idx >= 0);
        assert(idx < size());
        Node *t = head->next;
        while(idx-- != 0)
            t = t->next;
        const T *ret = t->data;
        delete t;
        return ret;
    }

    //修改元素
    void alter(const T *newElement, int idx){alterElement(newElement,idx);}
    void alterElement(const T *newElement,int idx){
        assert(idx >= 0);
        assert(idx < size());
        Node *t = head->next;
        while (idx-- != 0)
            t= t->next;
        t->data = newElement;
        return;
    }

    //查找元素 ， 返回值小于0表示未找到元素
    int findElement(const T* element) const{
        char *ori, *in;
        int result = 0;
        Node *t = head->next;
        while (t != head){
            ori = reinterpret_cast<char *>(&(t->data));
            in = reinterpret_cast<char *>(&element);
            if (strncmp(ori,in,sizeof(T)) == 0)
                return result;
            result++;
            t= t->next;
        }
        return -1;
    }
    //正序遍历
    void Traverse(void (*visit)(T* element)){
        Node *t = head->next;
        while(t != head){
            visit(t->data);
            t = t->next;
        }
    }

    //逆序遍历
    void TraverseBack(void (*visit)(T* element)){
        Node *t = head->prior;
        while (t != head){
            visit(t->data);
            t= t->prior;
        }
    }

    //清空链表
    void clear(){clearAllElement();}
    void clearAllElement(){
        Node *t,*p;
        p = head->next;
        while(p != head){
            t = p->next;
            delete p;
            p = t;
        }
    }

    //游标操作
    int getCurrentIdx(){
        assert(size() > 0);
        assert(head != head->next);
        int result = 0;
        if (current == NULL || current == head)
            return 0;
        else{
            Node *t = head->next;
            while (current != t){
                result++;
                t = t->next;
            }
        }
        return result;
    }

    const T* getCurrent(){
        assert(size() > 0);
        assert(head != head->next);
        if (current == NULL || current == head)
            current = head->next;
        return current->data;
    }
    const T* goNext(){
        assert(size() > 0);
        assert(head != head->next);
        getCurrent();
        Node *p = current->next;
        if (p == head)
            p = p->next;
        current = p;
        return current->data;
    }

    const T* goPrior(){
        assert(size() > 0);
        assert(head != head->prior);
        getCurrent();
        Node *p = current->prior;
        if (p == head)
            p = p->prior;
        current = p;
        return current->data;
    }

    //销毁链表
    ~DuLink(){
        clearAllElement();
        if (head != NULL){
            delete head;
            head = NULL;
        }
    }
};

///////////////////////////////////////////////////////////////////////////////////
//    TO COMPILE QfDebug, please add make patch num to .pro file, e.x:           //
//      DEFINES += "INT_MAKE_PATCH_SERIALNO=1.0f"                                //
//      QfDebug won't work in release mode(it will do nothing)                   //
//    qfDebug will print input info if arg 'PatchNum' bigger than DEFINES        //
///////////////////////////////////////////////////////////////////////////////////

template<class T> void qfDebug(T info,const char *patchNum)
{
    if (strcmp(patchNum , INT_MAKE_PATCH_SERIALNO) >= 0)
        cout << "Debug info : " << info  << " (patch no " << patchNum << ")" << endl;
}


template<> void qfDebug<const char*>(const char* info,const char *PatchNum);


}


#endif // COMMONFUNCTION_C_H
