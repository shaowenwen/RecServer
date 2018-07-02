#ifndef CONFIGPARA_H
#define CONFIGPARA_H
#include "tinyxml2.h"
#include <string>
#include <map>
#include <vector>

using namespace std;
typedef  struct _CDBPara
{
   int m_DBused;
   string m_User;
   string m_Passwd;
   string m_SID;

}DBPara;

typedef struct _CKafkaParameter
{
    int             m_kafkaused;
    std::string		brokers;
    std::string     topic;
}KafkaPara;

typedef struct _CHttpParameter
{
    int	 port1;
    int  port2;
    int  threadnum;
}HttpPara;

typedef struct _CUdpParameter
{
    int	 port1;
    int  port2;
    int  threadnum;
}UdpPara;

typedef std::map<string,string >	LaneToFTPMap;
typedef std::map<string,string >::iterator	LaneToFTPIterator;

class ConfigPara
{
public:
    ConfigPara();

    void ReadOptionsParam(const std::string& path);

    int GetServingPort();

    string GetServingIP();

    int GetDBused();

    int GetHttpPort1();

    int GetHttpPort2();

    int GetHttpThreadNum();

    int GetUdpPort1();

    int GetUdpPort2();

    int GetUdpThreadNum();

    string GetDBSID();

    string GetDBUser();

    string GetDBPasswd();

    string GetKafkaServerIP();

    string GetKafkaTopic();

    int GetKafkaServerused();

    LaneToFTPMap* GetNASConfig();
private:
    DBPara DBPar;
    KafkaPara  kafkaPar;
    HttpPara httpPar;
    UdpPara udpPar;

    int m_ServingPort;
    string sz_ServingIP;
    LaneToFTPMap	m_lanetoftpmap;

};

#endif // CONFIGPARA_H
