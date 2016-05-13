/*
 *Copyright@2013 上海青凤致远地球物理地质勘探科技有限公司
 *All Right Reserved
 *
 *简    介：鼠标事件解析功能类。
 *作    者：GXX
 *完成日期：2015.6.29

 *功能类：SqlFunctions :最外层的包装类，client程序应该直接使用该类来调用数据库功能
         ISqlConnect, MysqlConnect, XmlConnect, BinConnect 数据库链接功能接口及各个类
         ISqlQuery, MysqlQuery, XmlQuery, BinQuery 数据库操作接口接各个类
         FSqlFactory, MysqlFactory, XmlFactory, BinFactory  数据库工厂来接口及各个不同类实现，是完成虚拟工厂
                                                            切换的必要部件
         LiField, LiRecord, LiTable, LiDataContext   数据字段，记录，表及交互信息集合类，是TSQL语言在c++中的对象映射
         LiXmlLinker, xmlfunctions  xml实际操作功能类
         ioBinFile 二进制数据库实际操作功能类
         Connections 数据库链接信息结构体，记录了数据库的ip，用户名，密码等
 *
 */
#ifndef SQLFUNCTIONS_H
#define SQLFUNCTIONS_H
#include <stdlib.h>
#include <time.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQueryModel>
#include <QDebug>
#include <QFile>
#include <QtXml>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "QList"
#include "commonfunction_c.h"
#include "common.h"
using namespace commonfunction_c;
#define CHARS_CONNECTIONS_ELEMENTNAME_DATABASE "database"
#define CHARS_CONNECTIONS_ELEMENTNAME_SERVER "server"
#define CHARS_CONNECTIONS_ELEMENTNAME_USERNAME "username"
#define CHARS_CONNECTIONS_ELEMENTNAME_PASSWORD "password"
#define CHARS_CONNECTIONS_ELEMENTNAME_CONNNAME "connname"
#define CHARS_CONNECTIONS_STATIC_CONNATION_NAME "static_mysql_connection"
#define INT_MAX_TABLECOUNT 10
#define _QCONDITIONLIST_VALID(added,arg_01,arg_02) \
    if (added >= arg_01-1 && added <= arg_01 \
    && added >= arg_02 - 1 && added <= arg_02)  \
    return true;  \
    else  \
    return false;

class ioBinFile;




enum eRegularTableStruct
{
    MustRegular = false,
    NoNeedRegular = true
};

enum eFType
{
    INT,
    DATETIME,
    NUMERIC,
    VARCHAR,
    CHAR
};

enum eAndOrNot
{
    AND,
    OR,
    NOT,//NOT MEANS NOTAND
    NOTOR
};

enum eLiSqlOperate
{
    liEmpty, //未正确设置
    liEqual,
    liBigger,
    liLesser,
    liEqlAndBig,
    liEqlAndLes,
    liBigAndLes,
    liNotNULL,
    liIsNULL,
    liLike
};

enum eRemoveMode
{
    RollbackWithAnyError,
    JustMakeConsistency,
    NoCareError
};

enum eFctType
{
    SQL2K = 10,
    SQL2005 = 11,
    MYSQL = 12,
    SQLLITE = 13,
    ORACLE11 = 14,
    XML = 99,
    BIN_LOG = 98
};


//Sql Connection struct
typedef struct Connections
{
    QString ConnName;
    QString Database;
    QString User;
    QString Passwd;
    QString Server;
}str_Connections;





/*******************************************************************************************************************************************/
//                                                                                                                                         //
//                                  QtLinq 仿照微软的LINQ，开发一个方便更换SQL类型的功能 gxx 2013 11 26                                          //
//                                                                                                                                         //
/*****************************************************************Begin*********************************************************************/


class LiRecords
{
public:
    LiRecords(QList<QSqlRecord> & Records);
    LiRecords();
    LiRecords(LiRecords &other);
    ~LiRecords();
    QSqlRecord First(); //set flag to first
    QSqlRecord Last();  //set flag to last;
    QSqlRecord Next();  //go next;
    QStringList NextFields(QStringList FieldNames);//get all field in one time;
    void SetRecords(QList<QSqlRecord> * Records); //直接设置值
    QList<QSqlRecord> GetRecords() const; //获取全部数据
    int GetCount() const; //获取数据数量
    bool AddRecord(QSqlRecord rcd);
private:
    QList<QSqlRecord> * plRecords;
    int iTotalCount;
    int iIndex;
    LiRecords * _LiRecords; //record self by construct ,record the source ptr by copy-construct
};


class LiTable
{
public:
    LiTable();
    LiTable(QString TableName);
    ~LiTable();
    QString GetName() const; //获取表名称
    void SetRecords(LiRecords * Records); //设定数据集
    void SetName(QString TableName); //设定数据表名称
    LiRecords GetRecords(); // 获取全表数据
    bool AppendRecord(QSqlRecord rcd);  //向对象添加数据
private:
    QString sTableName; //表名
    int iRecordsCount; //数据数量
    LiRecords *plRecords; //数据指针
    //the Field or condition linked Number
    int iCopyNum;
};


class LiField
{
public:
    LiField();
    LiField(QString FieldName);
    QString GetValue() const;
    QString GetName() const;
    QString GetTableName() const;
    QString GetTitle() const;
    eFType GetType() const;
    eAndOrNot GetAON() const;
    eLiSqlOperate GetSO() const;
    int GetLen() const;
    int GetPcs() const;
    bool IsPrime() const; //判断是否是逐渐
    bool IsIdentity() const; //判断是否自增
    bool IsUnique() const; //判断是否是unique字段



    void SetValue(QString Value);
    void SetName(QString FieldName);
    void SetTitle(QString Title);
    void SetType(eFType Type);
    void SetLen(int Len);
    void SetPrecision(int Pcs);
    void SetPrime(bool IsPrime);
    void SetIdentity(bool IsIdentity);
    void SetUnique(bool IsUnique);
    void SetAON(eAndOrNot AON);
    void SetSo(eLiSqlOperate SO);

    //Function below for normal field in select condition like this: SELECT Fieldname FROM TableName
    void Set(QString FieldName,eFType Type,int Len = 0, int Pcs=0);

    //Function below for some TSQL what return u already intput ,like : SELECT 'INPUT' AS X FROM TableName
    void Set(eFType Type, QString Value, int Len = 0, int Pcs=0,QString Title = NULL);

    LiTable * SetTableLink(LiTable * Table);
private:
    LiTable * pTable;
    QString sValue;
    QString sFieldName;
    QString sTitle;
    eFType eFieldType;
    eAndOrNot eAON;
    eLiSqlOperate eSO;
    int iLength;
    int iPrecision;
    bool bIsPrime;
    bool bIsIdentity;
    bool bIsUnique;

};

//二进制日志型数据库，因为没有额外的表结构定义，所以写数据时需要明确提供表结构
//已经把原来的LiTableName改为一个自定义类
//typedef QString LiTableName;
class LiTableName
{
protected:
    LiTableName(){;}
public:
    LiTableName(QString tableName);
    LiTableName(const char tableName[]):m_tableName(QString(tableName)){;}
    operator QString(){
        return this->m_tableName;
    }

    LiTableName &operator=(QString right){
        this->m_tableName = right;
        return *this;
    }

    LiTableName &operator=(const char right[]){
        this->m_tableName = QString(right);
        return *this;
    }
    void doAddField(const LiField &field);

    const QList<LiField> &getFields();
    int transFields2Chars(char *out);// 将成员变量m_fields装换成一个字符串，可以被写如filemagic或其他地方
    //  来作为二进制数据文件表结构的标号
private:
    QString m_tableName;
    QList<LiField> m_fields;
    bool checkFieldDefined(const LiField &field); //in litable's field must defined completely!
    bool addField(const LiField &field);
};

class LiDataContext
{
public:
    LiDataContext();
    ~LiDataContext();
    //SetTableName functions:
    //give same TableName to all Fields , and this DataContext will run on all of it.
    //WARNING: if already have two or more tables , throw exception.
    //AND : After SetTableName,  we won't allow add table count , but still allow change table name
    //create by gxx 2013 11 27
    void SetTableName(QString TableName);



    //Normally add Field and Condition will return true , otherwise tablename conflict the order .(see comment About SetTableName) gxx 2013 11 27
    bool AddField(LiField * Field);
    bool AddField(QString & FieldName, eFType FieldType,QString Value = NULL,int Len = 0 , int Pcs =0);
    bool AddField(QString & FieldName, eFType FieldType,int Len=0,int Pcs=0,QString Title = NULL);
    bool AddCondition(LiField * CondField);
    bool setQueryNum(int num);//if use dc to query ,can set out data num;(in mysql ,it's limit func)
    bool AddConditions(QList<LiField*> CondFields, eAndOrNot GroupAON);
    bool AddOrder(LiField *orderField); //if use in query ,can add order field
    QStringList* GetTableNames(QStringList &result) const;
    QList<LiField> GetFieldsByTableName(QString & TableName);
    QList<LiField> GetConditionsByTableName(QString & TableName) const;
    QList<LiField> getOrderFieldByTableName(QString &TableName) const;
    //    bool AddField(LiTable * pTable, QString & FieldName, eFType FieldType,int Len=0,int Pcs=0,QString Title = NULL);
    //    bool AddField(LiTable * pTable, QString & FieldName, eFType FieldType,int Len=0,int Pcs=0,QString Title = NULL,QString Value = NULL);
    //bool MoveFieldBetweenTable(QString FieldName, QString OldTableName, QString NewTableName);
    //bool AddCondition(QString & ConditonName,QString & Value, eAndOrNot AON);
    int getQueryNum(){return m_qryNum;}
private:
    QList<LiField> *lFields;
    QList<LiField> *lConditions;
    QList<LiField> *m_orders;
    LiTable* lpTables[INT_MAX_TABLECOUNT]; // maybe turn to const better? gxx 2013 11 27
    int FieldCount;
    int m_qryNum;
    bool bCanInsert;
    bool bCanUpdate;
    bool bCanDel;
    bool bSameTable;

    //GetTableNum will reset lTableNames,to make Consistency.
    int GetTableNum();
    //add a Table to lTableNames, and return the table name nums.
    void AddTable(QString TableName);
    LiField* MakeGroupCondsToCond(QList<LiField*> GroupCond, eAndOrNot GroupAON, LiField * result);
};


//LINQ FOR XML
class LiXmlLinker : public QSqlDatabase
{
public:
    LiXmlLinker(const QString & Database,const QString & FileName);
    LiXmlLinker(const LiXmlLinker &other);
    LiXmlLinker(Connections Conn);
    LiXmlLinker& operator =(const LiXmlLinker&);
    ~LiXmlLinker();
    QFile *GetFile() const;
    QString GetDbName();
private:
    QString m_FileName;
    QString m_Database;
    QFile * m_pFile;
};


//以下是二进制文件底层操作的功能类，该类的单元测试另外存放
struct log_file_magic
{
    log_file_magic();
    int fileType;
    char fieldDefine[INT_SQL_FILEMAGIC_FIELDDEFINE_Size];
};

//这个是用来记录某张表内某个自增字段的当前信息的
struct idx_field_record
{
    idx_field_record();
    idx_field_record &operator=(const idx_field_record &right);

    char tableName[MAX_INT_SQL_LOGFILE_TABLENAME_LENGTH];
    char fieldName[MAX_INT_SQL_LOGFILE_FIELDNAME_LENGTH];
    int start;
    int ident;
    int current;
};

class ioBinFile :public QSqlDatabase
{
public:
    ioBinFile(const char *path,const char *file, log_file_magic fm);
    ioBinFile(string fileName, string idxName, log_file_magic fm);
    ~ioBinFile(){if (m_fileText) delete [] m_fileText;}

    void setFieldDefine(char *src,size_t size = INT_SQL_FILEMAGIC_FIELDDEFINE_Size){
        if (src == NULL){
            qDebug() << "WARNING, ioBinLog set field define fail cause by input char is NULL";
            return;
        }
        strncpy(m_fileMagic.fieldDefine,src,size) ;
        return;
    }

    const char *getFileDef(){
        return m_fileMagic.fieldDefine;
    }

    template<class T> void write(const T &data){
        FILE *f;
        f = fopen(m_fileName,"wb");
        if (!f)
            qDebug() << "ERROR: write ,open file error" ;

        fwrite(&m_fileMagic,sizeof(log_file_magic),1,f);
        int i = fwrite(&data,sizeof(T),1,f);
        if (i != sizeof(T))
            qDebug() << "Waning: io log file write byte not current, data size :" << sizeof(T)
                     << " but write byte:" << i << endl;

        if (fclose(f) != 0)
            qDebug() << QString("WARNING: template write fclose return fail!");
        return;
    }

    template<class T> vector<T> readall(int begin = 0){
        if (begin < 0 )
            throw "ERROR, file io read log failed, cause by argument wrong!";
        vector<T> rst;
        FILE *f;
        f = fopen(m_fileName,"rb");
        if (!f)
            throw QString("ERROR, read file failed, maybe file[%1] not exists!").arg(m_fileName);

        log_file_magic fm;
        fread(&fm,sizeof(log_file_magic),1,f);
        if (begin > 0)
            fseek(f,long(sizeof(T) * begin),SEEK_CUR);
        while(!feof(f)){
            T temp;
            fread(&temp,sizeof(T),1,f);
            rst.push_back(temp);
        }
        if (fclose(f) != 0)
            qDebug() << QString("WARNING: vector<T> readall fclose return fail!");
        return rst;
    }
    //对于表结构中自增长字段，代码中添加一个中方法处理一种存储与固定位置的子增长记录文件
    //自增长记录之所以不放在数据文件内是因为自增长的参照是基于全表，其增长是跨文件的。
    //使用该函数需要提供一个回调，逐个判断索引文件中的索引，如果不能正常找到索引文件程序会创建一个
    //如果找到了索引（返回true），程序提供的另一段代码会处理索引信息并返回处理后的索引数据(并返回true)， io会根据新数据更新文件
    template<class T> bool updateIdxWithlck(log_file_magic fm, T &compare,bool isRecord(const T &,const T &), bool identIdx(T &)){
        int fd;
        struct flock lock;
        fd = ::open(m_idxName,O_RDWR|O_CREAT, 0644);
        if (fd <0){
            perror("updateWithlck():");
            throw "ERROR, insert/update data fail, cause by idx file path error!";
        }
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_len = 0;
        lock.l_start = 0;

        int lockTry = 0;
        while (lockTry++ < MAX_INT_BINLOG_FCN_LOCK_TRY_COUNT){
            if (fcntl(fd,F_SETLK, &lock) != 0)
            //        perror("fcntl()");  //maybe too many waning!
                usleep(200);
            else
                break;
        }

        //未能成功锁住索引文件，可能是过高的并发造成，一般常规日志只记录特殊信息，所以可能是某些错误引发了日志潮
        if (lockTry > MAX_INT_BINLOG_FCN_LOCK_TRY_COUNT){
            perror("fcntl()");
            return false;
        }

        struct stat thStat;
        if (fstat(fd,&thStat) == -1)
            perror("fstat()");
        if (thStat.st_size < sizeof(log_file_magic)){
            //idx file is empty , write an log_file_magic

            ::write(fd, &fm, sizeof(log_file_magic));
            //and create an zero T(with default construct) and call new Idx;
            identIdx(compare);
            if (::write(fd, &compare, sizeof(T)) != sizeof(T))
                qDebug() << QString("WARNING : write idx file size not compare");
            return true;
        }

        //如果刚才没有return，说明这是一个有效文件
        lseek(fd, sizeof(log_file_magic), SEEK_SET);
        long l = sizeof(log_file_magic);
        while (l < thStat.st_size){
            T t;
            long size = ::read(fd, &t, sizeof(T));
            l += size;

            if (size != sizeof(T))
                continue;// 没有正确读到数据
            if (isRecord(compare,t)){
                compare = t;
                identIdx(compare);
                lseek(fd, -1 *size, SEEK_CUR);
                if (::write(fd, &compare, sizeof(T)) != sizeof(T))
                    qDebug() << QString("WARNING : write idx file size not compare");
                return true;
            }
        }
        return false;
    }


    bool insert(char *rcdStr, int length);
    template<class T> bool insert(const T &t){
        FILE *f;

        //判断文件是否存在，不存在则创建文件并写入filemagic
        f = fopen(m_fileName, "r");
        if (!f)
            writeMagic();
        else if(!checkMagic(f))
            throw QString("ERROR,ioBin template insert failed cause by filemagic not compared!");
        //追加数据
        f = fopen(m_fileName,"ab+");
        if (!f)
            throw QString("ERROR:IOBin insert fail , maybe file[%] not exist").arg(m_fileName);
        int i = fwrite(&t,sizeof(T),1,f);
        if (i != 1)
            qDebug() << "WARNING, write struct num[" << i << "] ,should be [1]!" << endl;
        if (fclose(f) != 0)
            qDebug() << QString("WARNING: insert fclose with ab+ return fail!");
        return true;
    }
    //尝试统计文件内记录的条数，正常情况返回 >= 0, fail return -1
    template<class T> int getRecordNum(){
        int rst = 0;
        FILE *f;
        //判断文件是否存在，不存在则创建文件并写入filemagic
        f = fopen(m_fileName, "r");
        if (!f)
            return -1;

        int i = fseek(f,0L,SEEK_END);
        if (i != 0)
            return -1;
        long tmp = ftell(f);
        tmp -= sizeof(log_file_magic);//去掉文件头信息
        rst = int(tmp / sizeof(T));
        if (fclose(f) != 0)
            qDebug() << QString("WARNING: insert fclose with ab+ return fail!");
        return rst;
    }

    char *readall(int *size);
    template<class T> vector<T> read(int begin=0, int end=0){
        if (begin > end || begin < 0 )
            throw "ERROR, file io read log failed, cause by argument wrong!";
        vector<T> rst;
        FILE *f;
        f = fopen(m_fileName,"rb");
        if (!f)
            qDebug() << "ERROR: read ,open file error" << endl;

        log_file_magic fm;
        fread(&fm,sizeof(log_file_magic),1,f);
        if (begin > 0)
            fseek(f,long(sizeof(T) * begin),SEEK_CUR);
        while(!feof(f) && end -- >= begin){
            T temp;
            fread(&temp,sizeof(T),1,f);
            rst.push_back(temp);
        }
        if (fclose(f) != 0)
            qDebug() << QString("WARNING: read fclose return fail!");
        return rst;
    }

    //if read data from some exists file , fielddefine use disk file's magic at first;
    void readFieldDef(QString &fieldDef);
protected:
    bool checkMagic(FILE *f);
    void writeMagic();
private:
    char m_fileName[INT_SQL_LOGFILE_FILENAME_LENGTH];
    char m_idxName[INT_SQL_LOGFILE_FILENAME_LENGTH];
    char *m_fileText;
    log_file_magic m_fileMagic; //include filetype , 0 table-bin 1 schdule-bin
    //  and   filedefine
};


/************************************************************************************************************/
//QConditionList ,  new query type will make coding easliy!
class LiConditionList
{
public:
    LiConditionList(){ /*do nothing */ }
    explicit LiConditionList(int i){
        if (i != 0)
            throw "ERROR : can not set LiCondition by int (except 0)!";
    }

    inline LiConditionList &operator=(LiConditionList &other){
        if (&other == this) return *this;
        this->m_AON=other.m_AON;
        this->m_Operate=other.m_Operate;
        this->m_values=other.m_values;
        return *this;
    }

    inline LiConditionList &operator=(int i){
        if (i == 0)
            return *this;
        else
            throw "ERROR : can not set LiCondition by int (except 0)!";
    }

    LiConditionList &operator <<(eLiSqlOperate value);
    LiConditionList &operator <<(eAndOrNot value);
    LiConditionList &operator <<(QString value);
    LiConditionList &operator <<(int value);
    LiConditionList &operator <<(long value);
    LiConditionList &operator <<(float value);
    LiConditionList &operator <<(const char* value);
    friend class ISqlQuery;
    //    friend class MysqlQuery;
    //    friend class SqlliteQuery;
    //    friend class XmlQuery;
    friend class SqlFunctions;
private:
    QList<eLiSqlOperate> m_Operate;
    QList<eAndOrNot> m_AON;
    QStringList m_values;
    inline bool isValid(eLiSqlOperate &){
        _QCONDITIONLIST_VALID(m_Operate.size(),m_AON.size(),(m_values.size()/2));
    }
    inline bool isValid(eAndOrNot &){
        _QCONDITIONLIST_VALID(m_AON.size(),m_Operate.size(),(m_values.size()/2));
    }
    inline bool isValid(QString &){
        _QCONDITIONLIST_VALID((m_values.size()/2),m_AON.size(),m_Operate.size());
    }
    inline bool isComplete(){
        if (m_Operate.size() == 0 && m_AON.size() == 0 && m_values.size() == 0)
            return true;
        else if (m_Operate.size() == m_AON.size() && m_Operate.size() == m_values.size()/2
                 && m_values.size() % 2 == 0 && m_Operate.size() > 0)
            return true;
        else
            return false;
    }

    inline eLiSqlOperate getOperator(int i) {
        if (!isComplete())
            throw QString("Error, LiConditionList Operator-member append in-complete!");
        if (i >= m_Operate.size() || i < 0)
            throw QString("Error, LiConditionList getOperator index out of range!");
        return m_Operate.at(i);
    }

    inline eLiSqlOperate getOperator(LiConditionList &list, int i){
        return list.getOperator(i);
    }

    inline eAndOrNot getAON(int i){
        if (!isComplete())
            throw QString("Error, LiConditionList AON-member append in-complete!");
        if (i >= m_AON.size() || i < 0)
            throw QString("Error, LiConditionList getAON index out of range!");
        return m_AON.at(i);
    }

    inline eAndOrNot getAON(LiConditionList &list, int i){
        return list.getAON(i);
    }

    QString getFieldName(int i){
        if (!isComplete())
            throw QString("Error, LiConditionList string member append in-complete!");
        if (i >= m_values.size()/2 || i < 0)
            throw QString("Error, LiConditionList getFieldName index out of range!");
        return m_values.at(i*2); //name is always forward to value , and index is from 0!
    }


    inline QString getFieldName(LiConditionList &list, int i){
        return list.getFieldName(i);
    }

    QString getFieldValue(int i){
        if (!isComplete())
            throw QString("Error, LiConditionList string member append in-complete!");
        if (i >= m_values.size()/2 || i < 0)
            throw QString("Error, LiConditionList getFieldValue index out of range!");
        return m_values.at(i*2 + 1); //value is always backward to name , and index is from 0!
    }

    inline QString getFieldValue(LiConditionList &list, int i){
        return list.getFieldValue(i);
    }
};

class LiResultList
{
public:
    LiResultList(){;}
    void insert(QStringList value){m_value.append(value);}
    void setName(QStringList name){m_name = name;}
    int size(){return getCount();}
    inline int getCount(){return m_value.size();}
    QStringList &at(int i){
        if (i < m_value.size())
            return m_value[i];
        else
            throw QString("Error linq result out of range!");
    }

    const QString &at(int i,QString name){
        QStringList temp = this->at(i);
        return temp.at(m_name.indexOf(name));
    }

    QStringList &operator[](int i){return at(i);}
private:
    QList<QStringList> m_value;
    QStringList m_name;
};

/***************************************************************end *******************************************/


/**********************************                           Linq  Function                       *****************************************/
/************************************************                   End                    *************************************************/

/********************************************************************************************************************************************
//                                                                                                                                         //
//                                                          Database  Begin                                                                //
//                                                                                                                                         //
********************************************************************************************************************************************/


//数据库链接
class ISqlConnect
{
public:
    virtual QSqlDatabase * iniDBConnect(const QString & Server ,const QString & UserName,const QString & Password,
                                        const QString & Database, const QString & ConnName)=0;

};

class MysqlConnect :public ISqlConnect
{
public:
    virtual QSqlDatabase * iniDBConnect(const QString & Server,const QString & UserName,const QString & Password,
                                        const QString & Database,const QString & ConnName);
};

class SqlliteConnect :public ISqlConnect
{
public:
    virtual QSqlDatabase * iniDBConnect(const QString& Server,const QString& UserName,const QString& Password,
                                        const QString& Database,const QString& ConnName);
};

class XmlConnect : public ISqlConnect
{
public:
    virtual QSqlDatabase * iniDBConnect(const QString & Server,const QString & UserName,const QString & Password,
                                        const QString & Database,const QString & ConnName);
};


class BinLogConnect : public ISqlConnect
{
public:
    BinLogConnect(Connections conn):m_conn(conn){;}
    virtual QSqlDatabase *iniDBConnect(const QString &Server, const QString &userName, const QString &pw,
                                       const QString &db, const QString &connName);
private:
    Connections m_conn;
};

//出错信息
class ISqlErrorMsg
{
public:
    virtual ~ISqlErrorMsg()=0;
protected:
    const virtual QString * lastError() = 0;
};

class MysqlErrorMsg:public ISqlErrorMsg
{
protected:
    virtual const QString * lastError();
    bool setMessage(const QString & ErrorMsg);
private :
    const QString * pErrorMsg;

};

class SqlliteErrorMsg:public ISqlErrorMsg
{
protected:
    virtual const QString *lastError();
    bool setMessage(const QString & ErrorMsg);
private :
    const QString * pErrorMsg;
};

class XmlErrorMsg:public ISqlErrorMsg
{
protected:
    virtual const QString *lastError();
    bool setMessage(const QString & ErrorMsg);
private :
    const QString * pErrorMsg;
};


class BinLogErrorMsg:public ISqlErrorMsg
{
protected:
    virtual const QString *lastError();
    bool setMessage(const QString &ErrorMsg);
private:
    const QString *pErrorMsg;
};

//查询返回QSqlQuery
class ISqlQuery
{
public:
    ISqlQuery(){/*do nothing*/}
    ISqlQuery(const ISqlQuery&){/*do nothing*/}
    virtual ~ISqlQuery();

    virtual QSqlQuery query(QSqlDatabase pDB,QString & SqlCmd,QString & sErr) = 0;
    virtual QSqlQuery query(QSqlDatabase pDB,QString & SqlCmd) = 0;
    virtual QSqlQuery query(QString & SqlCmd)=0;
    //GetTable() : return Table's record from sql database , base on Fields and Conditions gxx 2013 11 27
    virtual LiTable& query(QSqlDatabase pDB, LiDataContext &dc, LiTable &table)=0;
    virtual LiTable& query(LiDataContext &dc, LiTable & table)=0;

    //DoInsert function:
    //Insert records to one or more tables , and return result :
    //    <0 means insert fail,
    //    >= 0 means insert success and insert records count(absolutely , insert 0 rows still a success result)
    //create by gxx 2013 11 27
    virtual int DoInsert(QSqlDatabase pDB, LiDataContext &dc)=0;

    //DoUpdate function , create by gxx 2013 11 27 , nearly DoInsert but this function will do data update
    virtual int DoUpdate(QSqlDatabase pDB,LiDataContext &dc) = 0;
    //new Linq class type and code easily.because of TSQL language's rule ,
    //values must set AON = "AND",Operator = "liEqual"

    //DoUpdate function , create by gxx 2013 11 27 , del data in Tables
    virtual int DoDel(QSqlDatabase pDB,LiDataContext &dc) = 0;

    //create by gxx 20140408 new Insert function ,no need QSqlDatabase
    virtual int DoInsert(LiDataContext &dc)=0;
    virtual int DoUpdate(LiDataContext &dc)=0;
    virtual int DoDel(LiDataContext &dc)=0;
    /**********************************************************************************************/
    //new Linq class type and code easily. Call deriverd functions finish work,oop
    //add by gxx 2015 09 01
    virtual LiResultList query(QStringList &fields, LiTableName tableName, LiConditionList *conditions=0);
    virtual LiTable &query(QStringList &fields,LiTableName tableName,LiTable &table,LiConditionList *conditions=0);
    virtual int DoInsert(LiConditionList &values, LiTableName tableName);
    virtual int DoUpdate(LiConditionList &values,LiConditionList &conditions,LiTableName tableName);
    virtual int DoUpdate(LiConditionList &values, LiTableName tableName, LiConditionList *conditions =0);
    virtual int DoDel(LiConditionList &conditions,LiTableName tableName);
    virtual int DoDel(LiTableName tableName, LiConditionList *conditions = 0);
    /**********************************************************************************************/
    //Remove Data, create by gxx 2013 11 27, this function will del data and move it to a backup table
    //notice remove mode :
    //   RollbackWithAnyError mode,  it won't del or move any row cause any error then return a -1 as result.
    //   JustMakeConsistency mode ,  it just care the row must be inserted to backup which deleted success. and will return the Num of deleted rows. return -1 means TSql Error.
    //   NoCareError,                this mode runs fastest, and very easy make garbage. return -1 means a error before any Data Events
    //                               else return num of deleted rows (and discard backup num).
    virtual int DoRemove(QSqlDatabase pDB,LiDataContext &dc) = 0;
    virtual QSqlDatabase *getDB()=0;
};

class MysqlQuery:public ISqlQuery
{
public:
    MysqlQuery(QSqlDatabase *db);
    MysqlQuery(MysqlQuery &other);
    ~MysqlQuery();
    virtual QSqlQuery query(QSqlDatabase pDB,QString & SqlCmd,QString & sErr); //根据TSql命令在已经建立的数据库链接上查询数据，出错信息返回到sErr
    virtual QSqlQuery query(QSqlDatabase pDB, QString & SqlCmd); //根据TSql命令在已经建立的数据库链接上查询数据
    virtual QSqlQuery query(QString & SqlCmd); //根据TSql命令在已经建立的数据库链接上查询数据

    virtual LiTable& query(QSqlDatabase db, LiDataContext &dc, LiTable &table); //根据TSql命令对象在已经建立的数据库链接上查询数据
    virtual LiTable& query(LiDataContext &dc, LiTable &table); //根据TSql命令对象在已经建立的数据库链接上查询数据
    virtual int DoInsert(LiDataContext &dc); //根据TSql命令对象在已经建立的数据库链接上执行插入数据的操作
    virtual int DoInsert(QSqlDatabase pDB, LiDataContext &dc); //根据TSql命令对象在已经建立的数据库链接上执行插入数据的操作
    virtual int DoUpdate(QSqlDatabase pDB, LiDataContext &dc); //根据TSql命令对象在已经建立的数据库链接上执行更新数据的操作
    virtual int DoUpdate(LiDataContext &dc); //根据TSql命令对象在已经建立的数据库链接上执行更新数据的操作
    virtual int DoDel(QSqlDatabase pDB,LiDataContext &dc); //根据TSql命令对象在已经建立的数据库链接上执行删除数据的操作
    virtual int DoDel(LiDataContext &dc); //根据TSql命令对象在已经建立的数据库链接上执行删除数据的操作
    virtual int DoRemove(QSqlDatabase pDB,LiDataContext &dc); //根据TSql命令对象在已经建立的数据库链接上执行删除数据的操作
    virtual QSqlDatabase *getDB(){return m_pDb;} //返回数据库链接指针
private:
    //record Database info for ISqlQuery
    QString getLicenseContion(const bool &cond); //将校验码直接加到数据库条件语句内直接影响程序查询结果
    QString GenerateCMD(QString cmd, QString fieldName, QString fieldValue ,eAndOrNot fieldAON, eLiSqlOperate fieldSo);
    bool OpenDB(QSqlDatabase &pDB, int n = 3); //尝试链接数据库，直到链接成功或者超过尝试次数超过限定（默认三次，可以用参数调整）
    QSqlDatabase *m_pDb;
};



class SqlliteQuery : public ISqlQuery
{
public:
    virtual QSqlQuery query(QSqlDatabase pDB,QString & SqlCmd,QString & sErr);
    virtual QSqlQuery query(QSqlDatabase pDB,QString & SqlCmd);
    virtual QSqlQuery query(QString & SqlCmd);

    virtual LiTable& query(QSqlDatabase pDB, LiDataContext &dc, LiTable &table);
    virtual LiTable& query(LiDataContext &dc, LiTable &table);
    virtual int DoInsert(LiDataContext &dc);
    virtual int DoInsert(QSqlDatabase pDB, LiDataContext &dc);
    virtual int DoUpdate(QSqlDatabase pDB,LiDataContext &dc);
    virtual int DoUpdate(LiDataContext &dc);
    virtual int DoDel(QSqlDatabase pDB,LiDataContext &dc);
    virtual int DoDel(LiDataContext &dc);
    virtual int DoRemove(QSqlDatabase pDB,LiDataContext &dc);
    virtual QSqlDatabase *getDB(){return NULL;}
};

class XmlQuery : public ISqlQuery
{
public:
    XmlQuery(Connections Conn);
    XmlQuery(XmlErrorMsg* ErrMsg,LiXmlLinker *Linker=0);

    ~XmlQuery();
    //以下是接口函数实现，详见ISqlQuery
    virtual QSqlQuery query(QSqlDatabase pDB,QString & SqlCmd,QString & sErr);
    virtual QSqlQuery query(QSqlDatabase pDB, QString & SqlCmd);
    virtual QSqlQuery query(QString & SqlCmd);

    virtual LiTable &query(QSqlDatabase pDB, LiDataContext &dc, LiTable &table);
    virtual LiTable& query(LiDataContext &dc, LiTable &table);
    virtual int DoInsert(LiDataContext &dc);
    virtual int DoInsert(QSqlDatabase pDB, LiDataContext &dc);
    virtual int DoUpdate(QSqlDatabase pDB, LiDataContext &dc);
    virtual int DoUpdate(LiDataContext &dc);
    virtual int DoDel(QSqlDatabase pDB,LiDataContext &dc);
    virtual int DoDel(LiDataContext &dc);
    virtual int DoRemove(QSqlDatabase pDB,LiDataContext &dc);
    virtual QSqlDatabase *getDB(){return m_pLinker;}
private:
    //record xml file info for ISqlQuery
    LiXmlLinker *m_pLinker;
    XmlErrorMsg *m_pErrMsg;
};


class BinLogQuery: public ISqlQuery
{
public:
    BinLogQuery(Connections conn);
    BinLogQuery(QString fileName, int fileType, char *def);
    //    BinLogQuery(BinErrorMsg *ErrMsg, ioBinFile *bin = 0);
    ~BinLogQuery(){if (m_link != NULL) delete m_link;}
    virtual QSqlQuery query(QSqlDatabase pDB, QString &, QString &);
    virtual QSqlQuery query(QSqlDatabase, QString &);
    virtual QSqlQuery query(QString &);

    virtual LiTable& query(QSqlDatabase pDB, LiDataContext &dc, LiTable &table);//QSqlDatabase is no use actually,same as function below
    virtual LiTable& query(LiDataContext &dc, LiTable &table);
    virtual int DoInsert(LiDataContext &dc);
    virtual int DoInsert(QSqlDatabase, LiDataContext &dc);
    virtual int DoUpdate(QSqlDatabase , LiDataContext &){throw QString("ERROR,binlog update not finished now!");}
    virtual int DoUpdate(LiDataContext &){throw QString("ERROR,binlog update not finished now!");}
    virtual int DoDel(QSqlDatabase ,LiDataContext &){throw QString("ERROR,binlog update not del now!");}
    virtual int DoDel(LiDataContext &){throw QString("ERROR,binlog update not del now!");}
    virtual int DoRemove(QSqlDatabase ,LiDataContext &){throw QString("ERROR,binlog remove not finished now!");}
    virtual QSqlDatabase *getDB(){return m_link;}

    static bool isIdx(const idx_field_record &compare, const idx_field_record &value){
        if (strncmp(compare.fieldName, value.fieldName,MAX_INT_SQL_LOGFILE_FIELDNAME_LENGTH) == 0
                && strncmp(compare.tableName, value.tableName, MAX_INT_SQL_LOGFILE_TABLENAME_LENGTH) == 0
                && compare.fieldName[0] != '\0' && compare.tableName[0] != '\0')
            return true;
        else
            return false;
    }

    static bool identIdx(idx_field_record &idx){
        if (idx.ident <= 0 || idx.start <= 0)
            return false;
        if (idx.current < idx.start)
            idx.current = idx.start;
        else
            idx.current += idx.ident;
        return true;
    }

private:
    ioBinFile *m_link;

    //将符合筛选条件（cond）的数据中的，符合映射集要求的字段（fieldWanted）添加到LiTable &dest中
    void appendField2LiTable(const char *values, int valueLen, QString fieldDef,
                             QList<LiField> fieldWanted, QList<LiField> conds, LiTable &dest);
    int checkFieldDef(const QString &fieldDef, QStringList &fieldDefList){ //return one record char num
        fieldDefList = fieldDef.split(CHAR_SQL_FILEMAGIC_FIELDDEF_FIELD_SPLITER, QString::SkipEmptyParts);
        int begin = -1, end = -1;
        if (fieldDefList.size() <= 0)
            return -1;
        else if(fieldDefList[0].split(CHAR_SQL_FILEMAGIC_FIELDDEF_ELEMENT_SPLITER,QString::SkipEmptyParts).size()
                != INT_SQL_FILEMAGIC_FIELDDEFINE_ELEMENT_NUM)
            return -1;
        checkOneField(fieldDefList,fieldDefList.size()-1,begin, end);

        return end + 1;
    }


    QString checkOneField(const QStringList &theFieldDef,int idx, int &begin, int &end){  //将表结构字段设置列表中的某个字段的设置（字段名，起始位置，长度）传入，结果返回字段结束位置（如果是最后一个字段，这是返回的就是整条rcd的长度）
        if (idx < 0 || idx >= theFieldDef.size())
            throw QString("ERROR: checkOneField define failed cause by argument, idx = {%1} , theFieldDef.size() = {%2}!").arg(idx).arg(theFieldDef.size());
        QStringList fieldDefToList = theFieldDef[idx].split(CHAR_SQL_FILEMAGIC_FIELDDEF_ELEMENT_SPLITER,QString::SkipEmptyParts);
        if (fieldDefToList.size() != INT_SQL_FILEMAGIC_FIELDDEFINE_ELEMENT_NUM)
            throw QString("ERROR: field define to (name,begin-pos,length,ident) 4 unit , but theFieldDef[%1] = {%2}, it's not 3 unit in file!").arg(idx).arg(theFieldDef[idx]);
        bool okPos, okLen;
        begin = fieldDefToList[1].toInt(&okPos);
        end = begin + fieldDefToList[2].toInt(&okLen)-1;
        if (!okPos || !okLen || begin < 0 || end <= 0  || end < begin)
            throw QString("ERROR: field define ,theFieldDef[%1] = {%2}, pos num or len num error!").arg(idx).arg(theFieldDef[idx]);
        return fieldDefToList[0];
    }

    QString checkOneField(const QStringList &theFieldDef,int idx, int &begin, int &end, bool &isIdent){  //将表结构字段设置列表中的某个字段的设置（字段名，起始位置，长度）传入，结果返回字段结束位置（如果是最后一个字段，这是返回的就是整条rcd的长度）
        if (idx < 0 || idx >= theFieldDef.size())
            throw QString("ERROR: checkOneField define failed cause by argument, idx = {%1} , theFieldDef.size() = {%2}!").arg(idx).arg(theFieldDef.size());
        QStringList fieldDefToList = theFieldDef[idx].split(CHAR_SQL_FILEMAGIC_FIELDDEF_ELEMENT_SPLITER,QString::SkipEmptyParts);
        if (fieldDefToList.size() != INT_SQL_FILEMAGIC_FIELDDEFINE_ELEMENT_NUM)
            throw QString("ERROR: field define to (name,begin-pos,length,ident) 4 unit , but theFieldDef[%1] = {%2}, it's not 3 unit in file!").arg(idx).arg(theFieldDef[idx]);
        bool okPos, okLen;
        begin = fieldDefToList[1].toInt(&okPos);
        end = begin + fieldDefToList[2].toInt(&okLen)-1;
        if (!okPos || !okLen || begin < 0 || end <= 0  || end < begin)
            throw QString("ERROR: field define ,theFieldDef[%1] = {%2}, pos num or len num error!").arg(idx).arg(theFieldDef[idx]);
        if (fieldDefToList[3].toInt() == 1)
            isIdent = true;
        else if (fieldDefToList[3].toInt() == 0)
            isIdent = false;
        else
            throw QString("ERROR: field define error , ident flag out of rang[0,1], theFieldDef[%1] = {%2}!").arg(idx).arg(theFieldDef[idx]);

        return fieldDefToList[0];
    }
};

/**********************************                              Database                          *****************************************/
/************************************************                   End                    *************************************************/


/*******************************************************************************************************************************************/
//                                                                                                                                         //
//                                            SQL FACTORY，工厂模式数据库功能设计框架 gxx 2013 11 26                                           //
//                                                                                                                                         //
/*****************************************************************Begin*********************************************************************/

//Sql Factory
class FSqlFactory
{
public:
    virtual ~FSqlFactory()=0;
    virtual ISqlConnect * sqlConnectFct()=0;    //to get a sql connect , for some special function. Normally sqlQryFct will create one connect auto.
    virtual ISqlErrorMsg * sqlErrFct()=0;
    virtual ISqlQuery * sqlQryFct()=0;
    virtual void setConn(Connections conn) = 0;
    //new function below for increase factory's design gxx 20140326
};


class MysqlFactory : public FSqlFactory
{
public:
    MysqlFactory(Connections Conn);
    ~MysqlFactory();
    virtual ISqlConnect * sqlConnectFct();
    virtual ISqlErrorMsg * sqlErrFct();
    virtual ISqlQuery * sqlQryFct();
    void setConn(Connections conn){
        m_Conn = conn;
        m_db = new QSqlDatabase;
        *m_db = QSqlDatabase::addDatabase("QMYSQL",m_Conn.ConnName);
        m_db->setHostName(conn.Server);
        m_db->setUserName(conn.User);
        m_db->setPassword(conn.Passwd);
        m_db->setDatabaseName(conn.Database);
    }
    ISqlQuery *sqlQryFct(MysqlConnect Conn,MysqlErrorMsg ErrMsg);
private :

    Connections m_Conn;
    QSqlDatabase *m_db;
};

class SqlliteFactory : public FSqlFactory
{
public:
    virtual ISqlConnect * sqlConnectFct();
    virtual ISqlErrorMsg * sqlErrFct();
    virtual ISqlQuery * sqlQryFct();
    void setConn(Connections){throw "sql lite function not finished yet!";}
    ISqlQuery *sqlQryFct(SqlliteConnect Conn,SqlliteErrorMsg ErrMsg);
};

class XmlFactory : public FSqlFactory
{
public:
    XmlFactory(Connections Conn);
    ~XmlFactory();
    virtual ISqlConnect * sqlConnectFct();
    virtual ISqlErrorMsg * sqlErrFct();
    virtual ISqlQuery * sqlQryFct();
    void setConn(Connections conn){m_Conn=conn;}
private:
    Connections m_Conn;
};

class BinLogFactory : public FSqlFactory
{
public:
    BinLogFactory(Connections conn):m_Conn(conn){;}
    ~BinLogFactory();
    virtual ISqlConnect *sqlConnectFct();
    virtual ISqlErrorMsg *sqlErrFct();
    virtual ISqlQuery *sqlQryFct();
    void setConn(Connections conn){m_Conn=conn;}
private:
    Connections m_Conn;
};


//以下是功能包装类，一般的程序初始话sqlfunction对象都应该使用下面这个类
class SqlFunctions
{
public:
    //--------------------------------------------------------------------------------------------//
    //in xml link , conn.connName set to fileName , other element is free .
    //--------------------------------------------------------------------------------------------//
    //for binlog , conn.Server change to log fileType , and conn.Database set to fielddef
    // conn.connName set to fileName;
    //--------------------------------------------------------------------------------------------//
    explicit SqlFunctions();
    ~SqlFunctions();
    //以下五个都是创建数据库链接的函数
    static FSqlFactory &Create(eFctType type, QString iniFile, QString Path= QString("")); //create sql link by ini file
    static FSqlFactory &Create(eFctType type, string filePath);
    static FSqlFactory &Create(eFctType type, Connections conn);
    static FSqlFactory &Create(eFctType type, LiTableName tblStruct, QString connName, QString server, QString , QString , QString);
    static FSqlFactory &Create(eFctType type, LiTableName tblStruct, QString fileName, int fileType);

    virtual FSqlFactory &Create(eFctType type, Connections Conn, FSqlFactory **SqlDest ); // Create the factory 's type depand on param
    static void GetSqlLink(QString iniFilePath, QString iniFileName, QString database, QString connName, Connections &conn);
    static LiResultList rcdToLiResult(LiTable &,const QStringList &); //transfer LiTable which include records into liresult;
    static int listToFields(LiConditionList &values, LiField *fields); //将数据条件列表转换成指针
    static int isFieldInList(const QList<LiField> &fields,const QString &fieldName); //判定某个字段是否在query的结果集中（并不是每次query都会需要整个表的所有字段）
    static bool isRecordMeetCond(const QList<LiField> &cond, QSqlRecord rcd); //判定某条数据是否符合筛选条件
    static bool valueCmp(QString value, QString cond, eLiSqlOperate opera); //数据比较，一样返回true否则返回false
    static bool dayInLicense(); //判断是否程序是否在有效期内，使用默认参数
    static bool dayInLicense(int type, int dayBegin, int dayNum); //判断是否程序是否在有效期内，可以传参数
};


/*******************************************************************************************************************************************/
//                                                                                                                                         //
//                                            将XML文件视作数据库结构形式的功能代码     gxx 2014 03 21                                           //
//                                                                                                                                         //
/*****************************************************************Begin*********************************************************************/
class xmlfunctions
{
public:
    xmlfunctions(QFile *File,QString Database);
    //linker function: sqlfunctions::GetConditions; FieldIntegrity is MustRegular means :
    //                                                                      'AND' condition field must be found and equal.
    //                                                                       Otherwise , function will ignore the 'AND' condition if not found.
    int FltRecord(const QString &TableName, LiField Conditions[], int ConditionNum, QStringList FieldNames,
                  LiTable &Dest, eRegularTableStruct FieldIntegrity = MustRegular);
    // update the record, FieldIntegrity same as FltRecord
    int UpdRecord(QFile *File, const QString &TableName, LiField Conditions[], int ConditionNum, LiField Fields[], int FieldNum,
                  eRegularTableStruct FieldIntegrity = MustRegular);
    // do delete,FieldIntegrity same as FltRecord
    int DelRecord(QFile *File, const QString &TableName, LiField Conditions[], int ConditionNum, eRegularTableStruct FieldIntegrity = MustRegular);
    //insert record, feilds include either SQL command field , and xmlfunctions will care to default value field which not in sql command
    void InsertRecord(QFile *File,const QString &TableName, LiField Fields[],int FieldsNum);

    QList<LiField> GetDefaultFields(const QString &TableName, const QDomNode &Table);
    void UpdateXML(QFile *File);
private:
    QDomNode m_DatabaseTree;
    QDomNode GetTableByName(const QString &TableName, QDomNode &Database);
    void AppendNode2LiTable(const QDomNode &Record, QStringList FieldNames, LiTable &Dest);
    //these two function below is to do sql cmd with <prompty/>, what function not require in mysql class because done by Mysql Service!
    void UpdateRcdWithPrompty(QDomNode &Record,QDomDocument Doc,const QDomNode &Table);
    int GetMaxValueByFieldName(const QString FieldName,const QDomNode &Table);//get one field max value in one table , all non-intergrate translate to -1
    void UpdateOrInsertFieldToRecord(QDomNode &Record, const QString FieldName , int FieldValue, QDomDocument Doc);   //if record alread has field ,then update value ,otherwise , we should insert new field
    bool AnalyseCMDWithPrompty(const QDomNode &Table);

    bool AnalyseRecord(QDomNode &Record, LiField Conditions[],int ConditionNum,eRegularTableStruct FieldIntegrity);
    bool ValueCmp(QString value, QString cond, eLiSqlOperate opera);
};



/*******************************************************************************************************************************************/
//                                                                                                                                         //
//                                            二进制LOG文件读写     gxx 2015 08 11                                                        //
//                                                                                                                                         //
/*****************************************************************Begin*********************************************************************/







#endif // SQLFUNCTIONS_H
