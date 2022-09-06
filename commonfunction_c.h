#ifndef COMMONFUNCTION_C_H
#define COMMONFUNCTION_C_H
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <time.h>
#include <string>
#include <assert.h>
#include <tchar.h>
#include <vector>
#include "common.h"
using namespace std;
#ifndef INT_MAKE_PATCH_SERIALNO
#define INT_MAKE_PATCH_SERIALNO "1.05.0000"
#define CALIBRATION_LINES_COUNT 202
#endif
static char g_file_read_line_char[MAX_LENGTH_TXT_LINE];
namespace commonfunction_c {
class BaseFunctions
{
public:
    enum eFindPathMode { FPM_ALL = 0007, FPM_SYSENV = 0001, FPM_PATHWORK = 0002, FPM_PATHSTR = 0004 };
    BaseFunctions() { m_FileName = ""; m_FilePath = ""; }
    BaseFunctions(string fileName, string filePath = "") :m_FileName(fileName), m_FilePath(filePath) {}
    ~BaseFunctions() {}

    //get program run time path info!
    static wstring GetWorkPath() {
        wstring wstr;
        unsigned long size = GetCurrentDirectory(0, NULL);
        wchar_t* path = new wchar_t[size];
#ifdef JUCE_WINDOWS
        throw "GetWorkPath uncompatabled with juce!";
#else
        if (GetCurrentDirectory(size, path) != 0)
        {
            wstr = path;
        }
#endif // JUCE_WINDOWS


        delete[] path;
        return wstr;
    }

    static void SetWorkPath(wstring path) {
        SetCurrentDirectory(LPCWSTR(path.c_str()));
    }

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
    static int Chars2Int(const char* in, int size = DEFAULT_INTCHAR_SIZE) {
        int out;
        sscanf_s(in, "%d", &out);
        return out;
    }

    static float Chars2Float(const char* in) {
        float out;
        sscanf_s(in, "%f", &out);
        return out;
    }

    static string Int2Str(int i) {
        char number[10] = { 0 };
        sprintf_s(number, 10, "%d", i);
        return string(number);
    }

    static string Long2Str(long l) {
        char number[20] = { 0 };
        sprintf_s(number, 20, "%ld", l);
        return string(number);
    }

    static string UnsignedLong2Str(unsigned long l) {
        char number[20] = { 0 };
        sprintf_s(number, 20, "%lu", l);
        return string(number);
    }

    static int Str2Int(string s) {
        return Str2Int(s, 0);
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

    static string d2str(double in, const char* precision = "%.8lf") {
        char out[128];
        sprintf_s(out, 128, precision, in);
        return string(out);
    }

    static float str2f(string in) {
        float out;
        sscanf_s(in.c_str(), "%f", &out);
        return out;
    }

    static double str2d(string in) {
        double out;
        sscanf_s(in.c_str(), "%lf", &out);
        return out;
    }

    static string string_format(string format, ...) {
        //分析format结构
        int var_count = 0;
        int length = format.length();
        for (int i = 0; i < format.length(); ++i) {
            if (format[i] == '%' && i < format.length() - 1) {
                i++;
                if (format[i] == 'c' || format[i] == 'd' || format[i] == 'f' || format[i] == 's')
                    var_count++;
            }
        }
        //评估总长度
        va_list ap;
        va_start(ap, var_count);
        for (int i = 0; i < format.length(); ++i) {
            if (format[i] == '%' && i < format.length() - 1) {
                i++;
                if (format[i] == 'c') {
                    length ++;
                    va_arg(ap, char);
                }
                else if (format[i] == 's') {
                    char* arg_chars = va_arg(ap, char*);
                    while (*arg_chars != '\0') {
                        arg_chars++;
                        length++;
                    }
                }
                else if (format[i] == 'd') {
                    int arg_int = va_arg(ap, int);
                    while (arg_int > 0) {
                        arg_int /= 10;
                        length++;
                    }
                }
                else if (format[i] == 'f') {
                    int arg_float = va_arg(ap, int);
                    int arg_int = int(arg_float);
                    while (arg_int > 0) {
                        arg_int /= 10;
                        length++;
                    }
                }
            }
        }
        va_end(ap);
        length += 6;
        //转换字符串
        va_start(ap, var_count);

        //sprintf_s(message, 2048, format.c_str(), exception.ErrorCode(),
    //exception.ProcName().TextA(),
    //exception.ErrorMessage().TextA());
        va_end(ap);
        return string("");
    }

    static bool isFolderExist(const char* name)
    {
        return (_access(name, 0) == 0);
    }

    static bool isFolderExist(string name) {
        return (_access(name.c_str(), 0) == 0);
    }

    static void get_file_names(string dir, string file_type, vector<string>& file_names, int& result_count)
    {
        result_count = 0;
        string buffer;
        if (file_type.empty())
            file_type = ".*";
        if (file_type.find_first_of('.') == 0)
            buffer = dir + "\\*" + file_type;
        else
            buffer = dir + "\\*." + file_type; //设定后缀名的时候忘记写. 了？
        _finddata_t fileInfo;   //存放文件信息的结构体
        intptr_t hFile;
        hFile = _findfirst(buffer.c_str(), &fileInfo); //找第一个文件
        if (hFile == -1L) {
            //没找到指定类型的文件
            return;
        }
        else {
            do {
                result_count++;
                file_names.push_back(dir + "\\" + fileInfo.name);
            } while (_findnext(hFile, &fileInfo) == 0);  //如果找到下个文件的名字成功的话就返回0,否则返回-1  
            _findclose(hFile);
        }
    }

    //逐级创建路径
    static void createDirectoryW(wstring wstrDir) // 创建复目录
    {
        if (wstrDir.length() <= 3)//是根目录，无需创建目录
        {
            return;
        }
        if (wstrDir[wstrDir.length() - 1] == '\\')   // 将路径改为目录
        {
            wstrDir.erase(wstrDir.end() - 1);
        }
        // 修改文件属性
        WIN32_FIND_DATA wfd;
        HANDLE hFind = FindFirstFile(wstrDir.c_str(), &wfd); // 查找
        
        if (hFind != INVALID_HANDLE_VALUE)
        {
            FindClose(hFind);
            if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                return;
        }
        // 创建当前目录的地目录失败
        if (CreateDirectory(wstrDir.c_str(), NULL) == false)
        {// 退到上一级目录
            wstring wstrNewDir = wstrDir;
            while (wstrNewDir[wstrNewDir.length() - 1] != '\\')     // 撤到当前目录的上一个目录
            {
                wstrNewDir.erase(wstrNewDir.length() - 1);
            }
            // delete '\\' 
            wstrNewDir.erase(wstrNewDir.length() - 1); // delete '\\' 
            // 递归进入
            createDirectoryW(wstrNewDir);  // 递归本函数，再创建目录
            // 递归退出后创建之前失败的目录
            CreateDirectory(wstrDir.c_str(), NULL);  // 递归返回，在存在的目录上再建目录
        }// 多级目录创建成功
        return;
    }

    static int32_t createDirectory(const char* directoryPath) {
        return createDirectory(const_cast<char*>(directoryPath));
    }

    static int32_t createDirectory(char* directoryPath)
    {
        size_t dirPathLen = 0;
        if (directoryPath != NULL) {
            dirPathLen = strlen(directoryPath);
        }
        assert(dirPathLen <= FILENAME_MAX);
        char tmpDirPath[FILENAME_MAX] = { 0 };
        for (size_t i = 0; i < dirPathLen; ++i)
        {
            tmpDirPath[i] = directoryPath[i];
            if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/' || i == dirPathLen - 1)
            {
                if (!isFolderExist(tmpDirPath))
                {
#ifdef __IS_LINUX__
                    int ret = 0;
                    ret = _mkdir(tmpDirPath);
#else
                    BOOL ret = CreateDirectory(LPCWSTR(tmpDirPath), NULL);
#endif
                    if (ret != 0)
                        return -1;
                }
            }
        }
        return 0;
    }

    //combine file name and path name , will confirm if there is an '/' between path and file;
    static string combineFilePath(string path, string fileName) {
        if (path[path.size() - 1] == '\\' || path[path.size()] == '/')
            return path + fileName;
        else
            return path + '\\' + fileName;

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
        char chTmp[26] = { '\0' };
        struct tm p;
        localtime_s(&p, &time);
        p.tm_year = p.tm_year + 1900;
        p.tm_mon = p.tm_mon + 1;
        snprintf(chTmp, sizeof(chTmp), "%04d-%02d-%02d-%02d-%02d-%04d",
            p.tm_year, p.tm_mon, p.tm_mday, p.tm_hour, p.tm_min, p.tm_sec);
        strDateStr = chTmp;
        return 0;
    }

    static string time2str(const time_t& time) {
        char chTmp[26] = { '\0' };
        struct tm p;
        localtime_s(&p, &time);
        p.tm_year = p.tm_year + 1900;
        p.tm_mon = p.tm_mon + 1;
        snprintf(chTmp, sizeof(chTmp), "%04d-%02d-%02d %02d:%02d:%04d",
            p.tm_year, p.tm_mon, p.tm_mday, p.tm_hour, p.tm_min, p.tm_sec);
        return chTmp;
    }

    static string getNowTimeString() {
        time_t now;
        time(&now);
        return time2str(now);
    }

    static char* freadline(FILE* fp)
    {
        int count = 0;
        for (int i = 0; i < MAX_LENGTH_TXT_LINE; ++i)
            g_file_read_line_char[i] = '0';
        g_file_read_line_char[count++] = getc(fp);
        if (g_file_read_line_char[0] == EOF)
            return NULL;
        while ((count < MAX_LENGTH_TXT_LINE) && (g_file_read_line_char[count - 1] != '\n')) {
            int read_char = getc(fp);
            if (read_char >= 0)
                g_file_read_line_char[count++] = read_char;
            else {
                count++;
                break;
            }
        }
        g_file_read_line_char[count - 1] = '\0';
        return g_file_read_line_char;
    }

    static time_t str2time(string str); //transfer string 2 time,from '1970-01-01 00:08:00'
    //将日期转换位各种格式的int值，暂时只支持年加日（天数），后续增强各种功能。
    static int time2int(time_t time, string ="yyddd"); //transfer time 2 int
    static string cutValue(string wholeStr,string regExp); //find regExp in wholeStr, in regExp, word "value" is the return,
    //exp: reduce_type|value|,will get "aaa" in whole string "reduce_type|aaa|"
    
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
        ~Node() { if (prior != NULL) prior->next = next; if (next != NULL) next->prior = prior; }
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
        ~Node() { prior->next = next; next->prior = prior; delete data; }
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
}; // end DuLink


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

////////////////////////////////////////////////////////////////////////////////
//                              Base64 压缩解压缩                             //
////////////////////////////////////////////////////////////////////////////////
class ZBase64
{
public:
    /*编码
    DataByte
        [in]输入的数据长度,以字节为单位
    */
    string Encode(const unsigned char* Data, int DataByte) {
        //编码表
        const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        //返回值
        string strEncode;
        unsigned char Tmp[4] = { 0 };
        int LineLength = 0;
        for (int i = 0; i < (int)(DataByte / 3); i++)
        {
            Tmp[1] = *Data++;
            Tmp[2] = *Data++;
            Tmp[3] = *Data++;
            strEncode += EncodeTable[Tmp[1] >> 2];
            strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
            strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
            strEncode += EncodeTable[Tmp[3] & 0x3F];
            if (LineLength += 4, LineLength == 76) { strEncode += "\r\n"; LineLength = 0; }
        }
        //对剩余数据进行编码
        int Mod = DataByte % 3;
        if (Mod == 1)
        {
            Tmp[1] = *Data++;
            strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
            strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
            strEncode += "==";
        }
        else if (Mod == 2)
        {
            Tmp[1] = *Data++;
            Tmp[2] = *Data++;
            strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
            strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
            strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
            strEncode += "=";
        }

        return strEncode;
    }
    /*解码
    DataByte
        [in]输入的数据长度,以字节为单位
    OutByte
        [out]输出的数据长度,以字节为单位,请不要通过返回值计算
        输出数据的长度
    */
    string Decode(const char* Data, int DataByte, int& OutByte){
        //解码表
        const char DecodeTable[] =
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            62, // '+'
            0, 0, 0,
            63, // '/'
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
            0, 0, 0, 0, 0, 0, 0,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
            13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
            0, 0, 0, 0, 0, 0,
            26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
            39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
        };
        //返回值
        string strDecode;
        int nValue;
        int i = 0;
        while (i < DataByte)
        {
            if (*Data != '\r' && *Data != '\n')
            {
                nValue = DecodeTable[*Data++] << 18;
                nValue += DecodeTable[*Data++] << 12;
                strDecode += (nValue & 0x00FF0000) >> 16;
                OutByte++;
                if (*Data != '=')
                {
                    nValue += DecodeTable[*Data++] << 6;
                    strDecode += (nValue & 0x0000FF00) >> 8;
                    OutByte++;
                    if (*Data != '=')
                    {
                        nValue += DecodeTable[*Data++];
                        strDecode += nValue & 0x000000FF;
                        OutByte++;
                    }
                }
                i += 4;
            }
            else// 回车换行,跳过
            {
                Data++;
                i++;
            }
        }
        return strDecode;
    }
};

#endif // COMMONFUNCTION_C_H
