#include <iostream>
#include <czmq.h>
#include "zlog.h"
#include "ocilib.h"
#include "dbaccess.h"
#include "configpara.h"
#include "appmanager.h"

using namespace std;

zlog_category_t* g_server_cat;
ConfigPara g_conf;


int main(int, char**)
{
    //appman
    appManager  *appMan = appManager::instance();
    appMan->init();
   char endpoint[32];
   sprintf((char*)endpoint,"tcp://*:%d",g_conf.GetServingPort());
   cout << "--------------------------------------\n"<<endpoint<<endl;
   zsock_t *pull = zsock_new_pull (endpoint);
   RecHead headers;

   while (1) {
       char *string= zstr_recv (pull);
       if(!(appMan->verifyHeaders(string, headers)))
        {
           zlog_error(g_server_cat," rely  json failed \n");
        }
       else
       {
           zlog_info(g_server_cat,"rely  json success \n");
       }
       zstr_free (&string);
   }
   zsock_destroy(&pull);
}
