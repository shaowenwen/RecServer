#ifndef APPMANAGER_H
#define APPMANAGER_H

#include "zlog.h"
#include <string.h>
#include <stdlib.h>
#include "dbaccess.h"
#include <iostream>
#include <czmq.h>

#include "topkg.h"
#include "dbaccess.h"
#include "configpara.h"

class appManager;

class appManager
{
public:
    appManager();
    void init();
    static appManager* instance();
    void zlog_InitStep();
    bool verifyHeaders(char * recMsg,RecHead &headers);
    bool saveTrajData(char * recMsgbody,RecHead &head);
    bool saveAlarmData(char * recMsgbody,RecHead& head);

    ToPkg *pkg ;
    zsock_t *push;
    static appManager* appMan;

private:

    DBAccess dbaccessor;
    RecHead head;
    PackTraj Traj[20];
    PackAlarm alarm;
};

#endif // APPMANAGER_H
