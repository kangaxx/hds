/****************************************************************************
**
** Copyright (C) 2015 qfzy.
**
** class CommonPlug is create for Maininterface to call other
** plugin modules
** to use commonplug.h file , must insert word to .pro file
**
** version 1.00 by gxx 2015.08.13
**
** Commercial License Usage
** Company Licensees in progress.
**
****************************************************************************/

#ifndef CommonPlug_H
#define CommonPlug_H
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <QMessageBox>
#include <QDialog>
#include <thread>
#include <mutex>
#include "commonplug_global.h"
#include "sqlfunctions.h"
#include "FastDelegate.h"
#include "qm_common.h"

static std::mutex mtx;
#define CHARS_QMODEL_BASE_MODULE_CREATE "create"
#define CHARS_QMODEL_BASE_MODULE_DESTORY "destory"
#define QM_THREAD_ADD_ARGU(_arg_type,_arg_name) _arg_type _arg_name;
#define QM_THREAD_BEG_BEF_ARGU(_class_name) class _class_name{public:
#define QM_THREAD_BEG_AFT_ARGU void dofunc(){\
    mtx.lock();\
    std::lock_guard<std::mutex> lck(mtx,std::adopt_lock);
#define QM_THREAD_BEG(_class_name) class _class_name{public: void dofunc(){\
    mtx.lock();\
    std::lock_guard<std::mutex> lck(mtx,std::adopt_lock);
#define QM_THREAD_END(_class_name) }}; _class_name t;
#define QM_THREAD_RUN(_class_name) std::thread thd;\
    thd=std::thread(std::bind(&_class_name::dofunc,&t));\
    thd.detach();

using namespace fastdelegate;
typedef FastDelegate2<int,int> QM_SIGNAL;
typedef FastDelegate3<int,int,int> QM_SIGNAL_ARGU_1_INT;
using namespace std;
struct SysPipeParams
{
    std::string piName;
    int value;
    std::string Title;
};

class QM_SignalConnector
{
public:
    void postSignal(int moduleId,QEvent::Type event) const{this->m_signal(moduleId,event);}
    void postSignal(int moduleId,QEvent::Type event,int value) const{this->m_signal_argu_1(moduleId,event,value);}
    void setOwner(QWidget *);
private:
    const QWidget *m_owner;
    QM_SIGNAL m_signal;
    QM_SIGNAL_ARGU_1_INT m_signal_argu_1;
};

class moduleMainDlg
{
public:
    explicit moduleMainDlg(){
        m_event = QM_EVENT_BASE;
    }
    virtual ~moduleMainDlg(){}
    void SetSQLBase(FSqlFactory *pSql, QSqlDatabase *pDb) {pMySqlFct=pSql;pMyDb = pDb;}
    void SetId(int p,int i) {m_projId = p; m_indexId = i;}
    void addPipeParam(SysPipeParams p){ //setup pipe params , do not override
        m_params.push_back(p);
        return;
    }
    void eventLoop(){
        while(event_loop_ulimited){
            std::lock_guard<std::mutex> lck(mtx);
            switch(m_event){
            case QM_EVENT_BASE:
                //do nothing
                //qDebug() << "Warning !got BASE event!";
                break;
            case QM_EVENT_SYSTEM_CLOSE:
                this->filterQMEvent(m_event);
                m_event = QM_EVENT_BASE;
                break;
            case QM_EVENT_MODULE_PROJECT_CHANGED:
                this->filterQMEvent(m_event,m_event_argu_int);
                m_event = QM_EVENT_BASE;
                break;
            case QM_EVENT_MODULE_SGYDATA_CHANGED:
                this->filterQMEvent(m_event,m_event_argu_int);
                m_event = QM_EVENT_BASE;
                break;
            default:
                m_event = QM_EVENT_BASE;
                break;
            }
            usleep(DEFAULT_EVENT_SLEEP_TIME_USEC);
        }
    }
    void eventStart(){
        event_loop_ulimited = true;
        m_thd=std::thread(std::bind(&moduleMainDlg::eventLoop,this));
    }
    void QMEvent(QEvent::Type event){
        std::lock_guard<std::mutex> lck(mtx);
        this->m_event = event;
    }

    void QMEvent(QEvent::Type event,int value){
        std::lock_guard<std::mutex> lck(mtx);
        this->m_event = event;
        this->m_event_argu_int = value;
    }

    /***********************************************************************************************************/
    //QM_Signal function below
    void postSignal(int moduleId, QEvent::Type event){m_sigConn->postSignal(moduleId,event);} //send signal to Mainwindow
    void postSignal(int moduleId, QEvent::Type event, int value){m_sigConn->postSignal(moduleId,event,value);} //send signal to Mainwindow
    virtual void filterQMEvent(QEvent::Type event)=0;
    virtual void filterQMEvent(QEvent::Type ,int)= 0;
    void setConn(QM_SignalConnector *conn){m_sigConn = conn;}
    /***********************************************************************************************************/
    virtual bool CheckProject(int)=0;
    virtual bool CheckFileIndex(int,int)=0;
    virtual bool isValid()=0;
    virtual bool init()=0;
    virtual int showDlg()=0;

    int GetParamValue(std::string piName){
        for (unsigned int i=0;i<m_params.size();i++){
            if (m_params[i].piName==piName)
                return m_params[i].value;
        }
        throw "ERROR, system error please contact software support";
    }

protected:
    int m_projId, m_indexId;
    FSqlFactory *pMySqlFct;
    QSqlDatabase *pMyDb;
    vector<SysPipeParams> m_params;
    int GetParamIdx(std::string piName){ //get pipe params index, return -1 means not found.
        for(unsigned int i = 0;i<m_params.size();i++){
            if (m_params[i].piName == piName)
                return i;
        }
        return -1;
    }


    std::string GetParamTitle(std::string piName){
        for (unsigned int i=0;i<m_params.size();i++){
            if (m_params[i].piName==piName)
                return m_params[i].Title;
        }
        throw "ERROR, system error please contact software support";
    }
    const QM_SignalConnector *m_sigConn;
    std::thread m_thd;
    QEvent::Type m_event;
    int m_event_argu_int;
    bool event_loop_ulimited;
};

class COMMONPLUGSHARED_EXPORT CommonPlug
{
protected:
    int m_projId;
    int m_indexId;
    FSqlFactory *pSqlFct;
    QSqlDatabase *db;
    vector<SysPipeParams> m_params;
public:
    moduleMainDlg *m_mmDlg; // mmDlg address record;
    CommonPlug():m_projId(0),m_indexId(0){m_mmDlg = NULL;}
    CommonPlug(moduleMainDlg *md):m_projId(0),m_indexId(0){m_mmDlg = md;}
    ~CommonPlug(){}
    void Set_Num(int p,int i){m_projId = p; m_indexId = i;}
    void Set_SqlBase(FSqlFactory *pSql, QSqlDatabase *pDb){pSqlFct=pSql; db=pDb;}
    void Initial(int intParaNum,const int *para, int size);
    int SetPipeParam(SysPipeParams p){ //setup pipe params , call by Maininterface, do not override
        if (m_mmDlg==NULL)
            return -1;
        m_mmDlg->addPipeParam(p);
        return 0;
    }

    inline void addPipeParam(std::string name, int value , std::string title){
        SysPipeParams temp;
        temp.piName = name;
        temp.Title = title;
        temp.value = value;
        m_mmDlg->addPipeParam(temp);
    }

    int CallWidget(){
        try
        {
            if (m_mmDlg==NULL)
                throw "CRITICAL ERROR,module main dialog ptr is NULL!";
            m_mmDlg->SetSQLBase(pSqlFct,db);
            m_mmDlg->SetId(m_projId,m_indexId);
            if (m_mmDlg->CheckProject(m_projId)==false)
                throw "ERROR,module main dialog check project error!";
            if (m_mmDlg->CheckFileIndex(m_projId,m_indexId)==false)
                throw "ERROR,module main dialog check file index error!";
            if (m_mmDlg->isValid()==false)
                throw "ERROR,module main dialog check in-Valid!";
            if (m_mmDlg->init()==false)
                throw "ERROR,module main dialog init false!";
            m_mmDlg->showDlg();
        }
        catch (char* exception)
        {
            QMessageBox::critical(NULL,"ERROR",exception);
        }
        catch (const char* exception)
        {
            QMessageBox::critical(NULL,"ERROR",exception);
        }
        catch (QString exception)
        {
            QMessageBox::critical(NULL,"ERROR",exception);
        }
    }
    void CloseWidget(){}
    void setCONNECT(QM_SignalConnector *conn){m_mmDlg->setConn(conn);}
};

// the types of the class factories
typedef CommonPlug* create_t();
#endif // COMMONLPLUG_H
