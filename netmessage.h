/**************************************************
 *             Add by gxx  20140926               *
 *           用于压缩并发系统传输信息的字符数量，        *
 *           只传最少的字符用于回传确认计算任务！        *
 *************************************************/
#ifndef NETMESSAGE_H
#define NETMESSAGE_H
#include <iostream>
#include <math.h>
#include <sstream>
using namespace std;

//////////////////////////////////////
#define INT_NETMESSAGE_UNIT_NUM 4
enum eNMQueue
{
    MISSIONID = 1,
    CURRSHOTNUM = 2,
    MAXSHOTNUM = 3,
    JOBTYPE = 4
};
///////////////////////////////////////

class NetMessage
{
public:
    NetMessage();
    NetMessage(int MissionId, int CurrentShotNum, int MaxShotNum, int type, int MemberLength);
    NetMessage(string Infos, int MemberLength);
    NetMessage(char Infos[], int MemberLength);
    NetMessage(char Infos[], int MemberLength, int TotalLength);
    void Set(int MissionId,int CurrentShotNum,int MaxShotNum,int type, int MemberLength);

    void Get(int *MissionId,int *CurrentShotNum,int *MaxShotNum,int *type, int *MemberLength = 0);
    string GetString();
    char *GetChars(int Length);

    char* Int2Chars(int In,char* Out);
    int Chars2Int(const char *in);
private:
    int m_MissionId;
    int m_CurrentShotNum;
    int m_MaxShotNum;
    int m_type; //job type , 1 = map or stack job , 2 = reduce job
    int m_MemberLength;
    string m_StrInfos;

    void SetString();

    void SetMembers();
    void SetMember(string StrInfos,eNMQueue Name, int& dest);
    string GetMemberFromString(string Infos,eNMQueue Name);

};

#endif // NETMESSAGE_H
