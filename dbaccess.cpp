#include "dbaccess.h"
#include <string.h>
#include <stdio.h>


extern zlog_category_t* g_server_cat;
using namespace std;

DBAccess::DBAccess()
{
//    IsKeyConstraintFalg =false;
}

DBAccess::~DBAccess()
{
}

void DBAccess::Init(DbParameter* pDbParameter)
{
    m_strDbName = pDbParameter->servicename;
    m_strUser = pDbParameter->userID;
    m_strPassword = pDbParameter->password;
}

void DBAccess::err_handler(OCI_Error *err)
{
    zlog_error(g_server_cat,"code  : ORA-%05i\n"
                "msg   : %s\n"
                "sql   : %s\n",
                OCI_ErrorGetOCICode(err),
                OCI_ErrorGetString(err),
                OCI_GetSql(OCI_ErrorGetStatement(err))
           );
//    if(OCI_ErrorGetOCICode(err) == 00001)
//    {
//        zlog_error(g_server_cat,"ORA-00001");
//        this->IsKeyConstraintFalg = true;
//    }

}

bool DBAccess::IsKeyConstraintViolated(OCI_Error *err)
{

}

void DBAccess::Connect_DB()
{

}

//位置信息
bool DBAccess::InsertTrajData(PackTraj trajData[],RecHead &head,int &arrayLen)
{
    OCI_Connection* cn = NULL;
    OCI_Statement* st =  NULL;
    if (!OCI_Initialize(err_handler, NULL, OCI_ENV_DEFAULT))
        return false;
    cn = OCI_ConnectionCreate(m_strDbName.c_str(), m_strUser.c_str(), m_strPassword.c_str(), OCI_SESSION_DEFAULT);
    if (cn!=NULL) {
        st = OCI_StatementCreate(cn);
        char sql[]=MT("Insert into xjczc_sswzxx_TAB ")
                   MT("( ")
                   MT("UNIQUEID, DECIVEID, STAMP , LATITUDE, LONGITUDE, SPEED ,ALTITUDE, DIRECTION_ANGLE")
                   MT( ") " )
                   MT( "values ")
                   MT( "( ")
                   MT( ":a3, :b3, :c3, :d3, :e3, :f3, :g3, :h3")
                   MT(") ");
        cout<<"Intsert table:\n"<<sql<<endl;
        OCI_Prepare(st, sql);

        GetLocalTime(&stCurrent);
        char Vehrecord[32];
        sprintf(Vehrecord,"%04d%02d%02d%02d%02d%02d",stCurrent.wYear,stCurrent.wMonth,stCurrent.wDay,stCurrent.wHour,stCurrent.wMinute,stCurrent.wSecond,stCurrent.wMilliseconds);
        if( arrayLen >20 )
        {
            zlog_error(g_server_cat,"recDataNum > 20 ");
            return false;
        }
        char* tempStr[20]={"01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20"};
        zlog_debug(g_server_cat,"sizeof(trajData)/sizeof(trajData[0]):%d",arrayLen);

//        sprintf(VehrecordID,"%s%s%s",Vehrecord,(char*)head.szIMEI,tempStr[i]);
        char sz_VehrecordID[arrayLen][33]={0};
        char sz_IMEI[arrayLen][33]={0};
        float m_latitude[arrayLen];
        float m_longitude[arrayLen];
        int m_speed[arrayLen];
        int m_altitude[arrayLen];
        int m_direction_angle[arrayLen];
        OCI_Date *tstamp_time[arrayLen];


        OCI_BindArraySetSize(st, arrayLen);
        OCI_BindArrayOfStrings(st, MT(":a3"), (char*)sz_VehrecordID, 32,0);
        OCI_BindArrayOfStrings(st, MT(":b3"), (char*)sz_IMEI, 32,0);

        OCI_BindArrayOfDates(st, MT(":c3"), tstamp_time,0);

        OCI_BindArrayOfFloats(st, MT(":d3"), m_latitude,0);
        OCI_BindArrayOfFloats(st, MT(":e3"), m_longitude,0);
        OCI_BindArrayOfInts(st, MT(":f3"), m_speed,0);
        OCI_BindArrayOfInts(st, MT(":g3"), m_altitude,0);
        OCI_BindArrayOfInts(st, MT(":h3"), m_direction_angle,0);



        for(int i=0;i<arrayLen;i++)
        {

            if((atof((char*)trajData[i].latitude) == 0.0) || (atof((char*)trajData[i].longitude) == 0.0))
            {
                zlog_error(g_server_cat,"latitude:%f,longitude:%f",atof((char*)trajData[i].latitude),atof((char*)trajData[i].longitude));
                continue;
            }
            strncat(sz_VehrecordID[i],(char*)Vehrecord, strlen((char*)Vehrecord));
            strncat(sz_VehrecordID[i],(char*)head.szIMEI,strlen((char*)head.szIMEI));
            strncat(sz_VehrecordID[i],tempStr[i],strlen((char*)tempStr[0]));

            strncat(sz_IMEI[i], (char*)head.szIMEI, strlen((char*)head.szIMEI));

            m_latitude[i] = atof((char*)trajData[i].latitude);
            m_longitude[i] = atof((char*)trajData[i].longitude);
            m_speed[i] = atoi((char*)trajData[i].speed);
            m_altitude[i] = atoi((char*)trajData[i].altitude);
            m_direction_angle[i] = atoi((char*)trajData[i].direction_angle);
            tstamp_time[i] = OCI_DateCreate(cn);
            OCI_DateFromText(tstamp_time[i],(char*)trajData[i].stamp,"yyyy-mm-dd hh24:mi:ss");
        }

        if (!OCI_Execute(st)) {
                    printf("Number of DML array errors : %d\n", OCI_GetBatchErrorCount(st));

                    g_error = OCI_GetBatchError(st);

                    while (g_error) {
                        printf("Error at row %d : %s\n", OCI_ErrorGetRow(g_error), OCI_ErrorGetString(g_error));

                        g_error = OCI_GetBatchError(st);
                    }
                }


    for(int i=0;i<arrayLen;i++)
    {
        OCI_DateFree(tstamp_time[i]);
    }
        OCI_Commit(cn);

        OCI_ConnectionFree(cn);
    }
    st = NULL;
    cn = NULL;

    OCI_Cleanup();
    return true;
}


//报警信息
bool DBAccess::InsertAlarmData(PackAlarm alarmData,RecHead &head)
{
    OCI_Date *tstamp_time;
    OCI_Connection* cn = NULL;
    OCI_Statement* st =  NULL;

    if (!OCI_Initialize(err_handler, NULL, OCI_ENV_DEFAULT))
        return false;
    cn = OCI_ConnectionCreate(m_strDbName.c_str(), m_strUser.c_str(), m_strPassword.c_str(), OCI_SESSION_DEFAULT);
    if (cn!=NULL) {
        st = OCI_StatementCreate(cn);
        char sql[]=MT("Insert into xjczc_ssbjxx_TAB ")
                   MT("( ")
                   MT("UNIQUEID, DECIVEID, STAMP , LOCAL, LATITUDE , LONGITUDE, SPEED ,ALTITUDE, DIRECTION_ANGLE ,NOTIFY_TYPE")
                   MT( ") " )
                   MT( "values ")
                   MT( "( ")
                   MT( ":a0, :b0, :c0, :d0, :e0, :f0, :g0, :h0, :i0, :j0")
                   MT(") ");
        cout<<"Intsert table:\n"<<sql<<endl;
        OCI_Prepare(st, sql);

        if((atof((char*)alarmData.latitude) == 0.0 )|| (atof((char*)alarmData.longitude) == 0.0))
        {
            zlog_info(g_server_cat,"latitude:%f,longitude:%f",atof((char*)alarmData.latitude),atof((char*)alarmData.longitude));
        }
        else
        {
            GetLocalTime(&stCurrent);
            char Vehrecord[32];
            char VehrecordID[32];
            sprintf(Vehrecord,"%04d%02d%02d%02d%02d%02d",stCurrent.wYear,stCurrent.wMonth,stCurrent.wDay,stCurrent.wHour,stCurrent.wMinute,stCurrent.wSecond,stCurrent.wMilliseconds);
            sprintf(VehrecordID,"%s%s",Vehrecord,(char*)head.szIMEI);

            tstamp_time = OCI_DateCreate(cn);
            OCI_DateFromText(tstamp_time,(char*)alarmData.stamp,"yyyy-mm-dd hh24:mi:ss");

            OCI_BindString(st, MT(":a0"), const_cast<char *>((char*)VehrecordID), strlen((char*)VehrecordID));
            OCI_BindString(st, MT(":b0"), const_cast<char *>((char*)head.szIMEI), strlen((char*)head.szIMEI));

            OCI_BindDate(st, MT(":c0"), tstamp_time);

            float m_latitude = atof((char*)alarmData.latitude);
            float m_longitude = atof((char*)alarmData.longitude);
            int m_speed = atoi((char*)alarmData.speed);
            int m_altitude = atoi((char*)alarmData.altitude);
            int m_direction_angle = atoi((char*)alarmData.direction_angle);


            OCI_BindString(st, MT(":d0"), const_cast<char *>((char*)alarmData.local), strlen((char*)alarmData.local));
            OCI_BindFloat(st, MT(":e0"), &m_latitude);
            OCI_BindFloat(st, MT(":f0"), &m_longitude);
            OCI_BindInt(st, MT(":g0"), &m_speed);
            OCI_BindInt(st, MT(":h0"), &m_altitude);
            OCI_BindInt(st, MT(":i0"), &m_direction_angle);
            OCI_BindString(st, MT(":j0"),  const_cast<char *>((char*)alarmData.notify_type), strlen((char*)alarmData.notify_type));

            OCI_Execute(st);
            OCI_DateFree(tstamp_time);
            OCI_Commit(cn);
        }
        st = NULL;
        cn = NULL;

        OCI_ConnectionFree(cn);
    }

    OCI_Cleanup();
    return true;
}

//临时位置信息
bool DBAccess::InsertTrajTempData(PackTraj trajData[],RecHead &head,int &arrayLen)
{
    OCI_Date *tstamp_time;
    OCI_Connection* cn = NULL;
    OCI_Statement* st =  NULL;

    if (!OCI_Initialize(err_handler, NULL, OCI_ENV_DEFAULT))
        return false;
    cn = OCI_ConnectionCreate(m_strDbName.c_str(), m_strUser.c_str(), m_strPassword.c_str(), OCI_SESSION_DEFAULT);
    if (cn!=NULL) {
        st = OCI_StatementCreate(cn);
        char sql[]=MT("Insert into xjczc_sswzxx_TAB_temp ")
                   MT("( ")
                   MT("UNIQUEID, DECIVEID, STAMP , LATITUDE, LONGITUDE, SPEED ,ALTITUDE, DIRECTION_ANGLE")
                   MT( ") " )
                   MT( "values ")
                   MT( "( ")
                   MT( ":a1, :b1, :c1, :d1, :e1, :f1, :g1, :h1")
                   MT(") ");
        cout<<"Intsert table:\n"<<sql<<endl;
        OCI_Prepare(st, sql);

        GetLocalTime(&stCurrent);
        char Vehrecord[32];
        char VehrecordID[32];
        sprintf(Vehrecord,"%04d%02d%02d%02d%02d%02d",stCurrent.wYear,stCurrent.wMonth,stCurrent.wDay,stCurrent.wHour,stCurrent.wMinute,stCurrent.wSecond,stCurrent.wMilliseconds);

        tstamp_time = OCI_DateCreate(cn);
        OCI_DateFromText(tstamp_time,(char*)trajData[arrayLen -1].stamp,"yyyy-mm-dd hh24:mi:ss");

        OCI_BindString(st, MT(":a1"), const_cast<char *>((char*)Vehrecord), strlen((char*)Vehrecord));
        OCI_BindString(st, MT(":b1"), const_cast<char *>((char*)head.szIMEI), strlen((char*)head.szIMEI));

        OCI_BindDate(st, MT(":c1"), tstamp_time);

        float m_latitude = atof((char*)trajData[arrayLen -1].latitude);
        float m_longitude = atof((char*)trajData[arrayLen -1].longitude);
        int m_speed = atoi((char*)trajData[arrayLen -1].speed);
        int m_altitude = atoi((char*)trajData[arrayLen -1].altitude);
        int m_direction_angle = atoi((char*)trajData[arrayLen -1].direction_angle);


        OCI_BindFloat(st, MT(":d1"), &m_latitude);
        OCI_BindFloat(st, MT(":e1"), &m_longitude);
        OCI_BindInt(st, MT(":f1"), &m_speed);
        OCI_BindInt(st, MT(":g1"), &m_altitude);
        OCI_BindInt(st, MT(":h1"), &m_direction_angle);

        OCI_Execute(st);
        OCI_DateFree(tstamp_time);
        OCI_Commit(cn);
        OCI_ConnectionFree(cn);
    }
    st = NULL;
    cn = NULL;
    OCI_Cleanup();
    return true;
}


bool DBAccess::UpdateTrajTempData(PackTraj trajData[],RecHead &head,int &arrayLen)
{
    OCI_Date *tstamp_time;
    OCI_Connection* cn = NULL;
    OCI_Statement* st =  NULL;

    if (!OCI_Initialize(err_handler, NULL, OCI_ENV_DEFAULT))
        return false;
    cn = OCI_ConnectionCreate(m_strDbName.c_str(), m_strUser.c_str(), m_strPassword.c_str(), OCI_SESSION_DEFAULT);
    if (cn!=NULL) {
        st = OCI_StatementCreate(cn);
        char sql[]=("update xjczc_sswzxx_TAB_temp set UNIQUEID = :a2, STAMP =:b2 , LATITUDE = :c2, LONGITUDE = :d2, SPEED = :e2 ,ALTITUDE =:f2, DIRECTION_ANGLE =:g2 where DECIVEID = :h2 ");
        cout<<"Intsert table:\n"<<sql<<endl;
        OCI_Prepare(st, sql);

        GetLocalTime(&stCurrent);
        char Vehrecord[32];
        sprintf(Vehrecord,"%04d%02d%02d%02d%02d%02d",stCurrent.wYear,stCurrent.wMonth,stCurrent.wDay,stCurrent.wHour,stCurrent.wMinute,stCurrent.wSecond,stCurrent.wMilliseconds);

        tstamp_time = OCI_DateCreate(cn);
        OCI_DateFromText(tstamp_time,(char*)trajData[arrayLen -1].stamp,"yyyy-mm-dd hh24:mi:ss");

        OCI_BindString(st, MT(":a2"), const_cast<char *>((char*)Vehrecord), strlen((char*)Vehrecord));
        OCI_BindString(st, MT(":h2"), const_cast<char *>((char*)head.szIMEI), strlen((char*)head.szIMEI));

        OCI_BindDate(st, MT(":b2"), tstamp_time);

        float m_latitude = atof((char*)trajData[arrayLen -1].latitude);
        float m_longitude = atof((char*)trajData[arrayLen -1].longitude);
        int m_speed = atoi((char*)trajData[arrayLen -1].speed);
        int m_altitude = atoi((char*)trajData[arrayLen -1].altitude);
        int m_direction_angle = atoi((char*)trajData[arrayLen -1].direction_angle);


        OCI_BindFloat(st, MT(":c2"), &m_latitude);
        OCI_BindFloat(st, MT(":d2"), &m_longitude);
        OCI_BindInt(st, MT(":e2"), &m_speed);
        OCI_BindInt(st, MT(":f2"), &m_altitude);
        OCI_BindInt(st, MT(":g2"), &m_direction_angle);

        OCI_Execute(st);
        OCI_DateFree(tstamp_time);
        OCI_Commit(cn);
    }
    st = NULL;
    cn = NULL;
    OCI_ConnectionFree(cn);
    OCI_Cleanup();
    return true;
}




