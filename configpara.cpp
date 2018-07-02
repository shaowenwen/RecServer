#include "configpara.h"
#include <iostream>
using namespace std;


ConfigPara::ConfigPara()
{

}

/** @brief 读取解析某路径的xml文件

@param path xml文件路径
*/
void ConfigPara::ReadOptionsParam(const std::string& path)
{
    // 导入文件错误, 退出
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(path.c_str());
    if (error != tinyxml2::XMLError::XML_SUCCESS) return;

    tinyxml2::XMLElement* pFirst = doc.FirstChildElement("AgentOptions");

    sz_ServingIP = pFirst->FirstChildElement("ServingIP")->GetText();

    m_ServingPort = pFirst->FirstChildElement("ServingPort")->IntText();
    //DBPara
    tinyxml2::XMLElement* pDBPara = pFirst->FirstChildElement("DBPara");


    DBPar.m_DBused = pDBPara->FirstChildElement("DBused")->IntText();
    DBPar.m_User = pDBPara->FirstChildElement("User")->GetText();
    DBPar.m_Passwd = pDBPara->FirstChildElement("Passwd")->GetText();
    DBPar.m_SID = pDBPara->FirstChildElement("SID")->GetText();

    tinyxml2::XMLElement* pkafkaPara = pFirst->FirstChildElement("kafkaPara");

    kafkaPar.m_kafkaused = pkafkaPara->FirstChildElement("kafkaused")->IntText();
    kafkaPar.brokers = pkafkaPara->FirstChildElement("kafkaServer")->GetText();
    kafkaPar.topic = pkafkaPara->FirstChildElement("topic1")->GetText();

    tinyxml2::XMLElement* phttpServerPar = pFirst->FirstChildElement("httpServerPar");
    httpPar.port1 = phttpServerPar->FirstChildElement("port1")->IntText();
    httpPar.port2 = phttpServerPar->FirstChildElement("port2")->IntText();
    httpPar.threadnum = phttpServerPar->FirstChildElement("threadnum")->IntText();

    tinyxml2::XMLElement* pudpServerPar = pFirst->FirstChildElement("udpServerPar");
    udpPar.port1 = pudpServerPar->FirstChildElement("port1")->IntText();
    udpPar.port2 = pudpServerPar->FirstChildElement("port2")->IntText();
    udpPar.threadnum = pudpServerPar->FirstChildElement("threadnum")->IntText();

    tinyxml2::XMLElement* pProxyAddresses = pFirst->FirstChildElement("ProxyAddresses");

    tinyxml2::XMLElement* pProxyAddress = pProxyAddresses->FirstChildElement("ProxyAddress");

    while (pProxyAddress != NULL)
    {
        string p = pProxyAddress->GetText();
        if (!(p.empty()))
        {
            m_lanetoftpmap.insert(LaneToFTPMap::value_type(p,p));

        }
        pProxyAddress = pProxyAddress->NextSiblingElement("ProxyAddress");
    }

}

int ConfigPara::GetServingPort()
{
    return m_ServingPort;
}

string ConfigPara::GetServingIP()
{
    return sz_ServingIP;
}

int ConfigPara::GetDBused()
{
    return DBPar.m_DBused;
}

int ConfigPara::GetHttpPort1()
{
    return httpPar.port1;
}
int ConfigPara::GetHttpPort2()
{
    return httpPar.port2;
}
int ConfigPara::GetHttpThreadNum()
{
    return httpPar.threadnum;
}

int ConfigPara::GetUdpPort1()
{
    return udpPar.port1;
}
int ConfigPara::GetUdpPort2()
{
    return udpPar.port2;
}
int ConfigPara::GetUdpThreadNum()
{
    return udpPar.threadnum;
}

string ConfigPara::GetDBUser()
{
    return DBPar.m_User;
}

string ConfigPara::GetDBPasswd()
{
    return DBPar.m_Passwd;
}

string ConfigPara::GetDBSID()
{
    return DBPar.m_SID;
}

int ConfigPara::GetKafkaServerused()
{
    return kafkaPar.m_kafkaused;
}

string ConfigPara::GetKafkaServerIP()
{
    return kafkaPar.brokers;
}

string ConfigPara::GetKafkaTopic()
{
    return kafkaPar.topic;
}

LaneToFTPMap* ConfigPara::GetNASConfig()
{
    return &m_lanetoftpmap;
}

