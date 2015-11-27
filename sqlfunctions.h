#ifndef SQLFUNCTIONS_H
#define SQLFUNCTIONS_H
#include "stdlib.h"
#include "time.h"
#include "QtSql/QSqlDatabase"
#include "QtSql/QSqlDriver"
#include "QtSql/QSqlError"
#include "QtSql/QSqlQuery"
#include "QtSql/QSqlRecord"
#include "QtSql/QSqlField"
#include "QtSql/QSqlQueryModel"
#include "QDebug"
#include "QFile"
#include "QtXml"
#include "QtXml/QDomDocument"
#include "QtXml/QDomNode"

#include "QList"
#include "commonfunction_c.h"
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
    LOG_BIN = 98
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

typedef QString LiTableName;

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
    void SetRecords(QList<QSqlRecord> * Records);
    QList<QSqlRecord> GetRecords() const;
    int GetCount() const;
    bool AddRecord(QSqlRecord rcd);
private:
    QList<QSqlRecord> * plRecords;
    int iTotalCount;
    int iIndex;
    LiRecords * _LiRecords; //record self by construct  ,record the source ptr by copy-construct
};


class LiTable
{
public:
    LiTable();
    LiTable(QString TableName);
    ~LiTable();
    QString GetName() const;
    void SetRecords(LiRecords * Records);
    void SetName(QString TableName);
    LiRecords GetRecords();
    bool AppendRecord(QSqlRecord rcd);
private:
    QString sTableName;
    int iRecordsCount;
    LiRecords *plRecords;
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
    bool IsPrime() const;
    bool IsIdentity() const;
    bool IsUnique() const;



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
    virtual const QString * lastError();
    bool setMessage(const QString & ErrorMsg);
private :
    const QString * pErrorMsg;
};

class XmlErrorMsg:public ISqlErrorMsg
{
protected:
    virtual const QString * lastError();
    bool setMessage(const QString & ErrorMsg);
private :
    const QString * pErrorMsg;
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
    virtual QSqlQuery query(QSqlDatabase pDB,QString & SqlCmd,QString & sErr);
    virtual QSqlQuery query(QSqlDatabase pDB, QString & SqlCmd);
    virtual QSqlQuery query(QString & SqlCmd);

    virtual LiTable& query(QSqlDatabase db, LiDataContext &dc, LiTable &table);
    virtual LiTable& query(LiDataContext &dc, LiTable &table);
    virtual int DoInsert(LiDataContext &dc);
    virtual int DoInsert(QSqlDatabase pDB, LiDataContext &dc);
    virtual int DoUpdate(QSqlDatabase pDB, LiDataContext &dc);
    virtual int DoUpdate(LiDataContext &dc);
    virtual int DoDel(QSqlDatabase pDB,LiDataContext &dc);
    virtual int DoDel(LiDataContext &dc);
    virtual int DoRemove(QSqlDatabase pDB,LiDataContext &dc);
    virtual QSqlDatabase *getDB(){return m_pDb;}
private:
    //record Database info for ISqlQuery
    QString GenerateCMD(QString cmd, QString fieldName, QString fieldValue ,eAndOrNot fieldAON, eLiSqlOperate fieldSo);
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
    virtual QSqlQuery query(QSqlDatabase pDB,QString & SqlCmd,QString & sErr);
    virtual QSqlQuery query(QSqlDatabase pDB, QString & SqlCmd);
    virtual QSqlQuery query(QString & SqlCmd);

    virtual LiTable& query(QSqlDatabase pDB, LiDataContext &dc, LiTable &table);
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


class SqlFunctions
{
public:
    explicit SqlFunctions();
    ~SqlFunctions();
    static FSqlFactory &Create(eFctType type, QString iniFile, QString Path= QString("")); //create sql link by ini file
    static FSqlFactory &Create(eFctType type, string filePath);
    static FSqlFactory &Create(eFctType type, Connections conn);
    virtual FSqlFactory &Create(eFctType type, Connections Conn, FSqlFactory **SqlDest ); // Create the factory 's type depand on param
    static void GetSqlLink(QString iniFilePath, QString iniFileName, QString database, QString connName, Connections &conn);
    static LiResultList rcdToLiResult(LiTable &,const QStringList &); //transfer LiTable which include records into liresult;
    static int listToFields(LiConditionList &values, LiField *fields);

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
//                                            数据库相关函数扩展功能     gxx 2015 08 11                                                        //
//                                                                                                                                         //
/*****************************************************************Begin*********************************************************************/
//namespace SqlExtend{
//
//
//}

#endif // SQLFUNCTIONS_H
