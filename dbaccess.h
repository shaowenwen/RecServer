#ifndef DBACCESS_H
#define DBACCESS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "ocilib.h"

#include "zlog.h"
#include "topkg.h"

typedef struct _DbParameter
{
    int             used;
    std::string		userID;
    std::string		password;
    std::string     servicename;
    std::string		brokers;
    std::string     topic;
}DbParameter;


class DBAccess
{
public:
    DBAccess();
    virtual ~DBAccess();
////    int get_dbstat();
    bool CheckConnection();
    static void err_handler(OCI_Error *err);
    void Connect_DB();
    void Disconn_DB();

    bool IsKeyConstraintViolated(OCI_Error *err);

    void Init(DbParameter* pDbParameter);
    bool InsertTrajData(PackTraj trajData[],RecHead &head,int &arrayLen);
    bool InsertAlarmData(PackAlarm alarmData,RecHead &head);
    bool InsertTrajTempData(PackTraj trajData[],RecHead &head,int &arrayLen);
    bool UpdateTrajTempData(PackTraj trajData[],RecHead &head,int &arrayLen);

    OCI_Error     *g_error;


private:
    bool                m_bConnected;


    std::string         m_strUser;
    std::string         m_strPassword;
    std::string         m_strDbName;
    SYSTEMTIME stCurrent;

};

#endif // DBACCESS_H
