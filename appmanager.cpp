#include "appmanager.h"

extern zlog_category_t* g_server_cat;
extern ConfigPara g_conf;
appManager* appManager::appMan = new appManager();
appManager::appManager()
{

}

void appManager::init()
{
    zlog_InitStep();
    DbParameter par;
    par.servicename ="192.168.44.13:1521/jxtest";
    par.userID ="shaow";
    par.password = "123456";
    dbaccessor.Init(&par);

}

appManager* appManager::instance() {
    return appManager::appMan;
}

void appManager::zlog_InitStep()
{
    int ret = zlog_init("../conf/RecData_log.conf");
    if (ret != 0)
    {
        printf("Initlize zlog faield\n");
        exit(EXIT_FAILURE);
    }

    g_server_cat = zlog_get_category("server_cat_server");
    if (!g_server_cat)
    {
       printf("zlog_get_category failed\n");
        exit(EXIT_FAILURE);
    }
     g_conf.ReadOptionsParam("../conf/aoptions.xml");
}

bool appManager::verifyHeaders(char * recMsg,RecHead &headers)
{
    cJSON* root = cJSON_Parse(recMsg);
    if (root == NULL)
    {
        zlog_error(g_server_cat," parse verifyHeaders json failed \n");
        return false;
    }
    else
    {
          //取包头数据
          memset(&head,0,sizeof(RecHead));
          if (!pkg->JSONToPkgHead(root, head))
          {
             zlog_error(g_server_cat," get Headers json data failed \n");
             return false;
          }
          memcpy(&headers,&head,sizeof(RecHead));
          if(strcmp((char *)head.szCommand,"track") == 0)
          {
              saveTrajData(recMsg,head);
          }else if(strcmp((char *)head.szCommand,"get_notify") == 0)
          {
              cout << "get_notify" <<endl;
              saveAlarmData(recMsg,head);
          }
          return true;
    }

}




bool appManager::saveTrajData(char * recMsgbody,RecHead &head)
{
    cJSON* root = cJSON_Parse(recMsgbody);
    if (root == NULL)
    {
        zlog_error(g_server_cat," parse trajmsgbody json failed \n");
    }
    else
    {
        //取数据
        int size;
        memset(Traj,0,sizeof(PackTraj));
        if (!pkg->JSONToPkgTraj(root, Traj,size))
        {
             zlog_error(g_server_cat," get trajmsgbody json data failed \n");
        }
        dbaccessor.InsertTrajTempData(Traj,head,size);
        dbaccessor.InsertTrajData(Traj,head,size);
        dbaccessor.UpdateTrajTempData(Traj,head,size);
    }

}

bool appManager::saveAlarmData(char * recMsgbody,RecHead &head)
{
    cJSON* root = cJSON_Parse(recMsgbody);
    if (root == NULL)
    {
        zlog_error(g_server_cat," parse alarmmsgbody json failed \n");
    }
    else
    {
        //取数据
        //memset(alarm,0,sizeof(PackTraj));
        if (!pkg->JSONToPkgAlarm(root, alarm))
        {
             zlog_error(g_server_cat," get alarmmsgbody json data failed \n");
        }

        dbaccessor.InsertAlarmData(alarm,head);
    }

}
