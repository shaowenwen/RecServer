#include "topkg.h"
#include "cJSON.h"
#include<math.h>


extern zlog_category_t* g_server_cat;
using namespace std;
ToPkg::ToPkg()
{
}

//解析JSON 包头
bool ToPkg::JSONToPkgHead(cJSON* root,
                            RecHead& head)
{
        cJSON* msghead = cJSON_GetObjectItem(root, "header");
        if(msghead == NULL)
        {
            return false;
        }
        else
        {
            if(cJSON_GetObjectItem(msghead,"user") !=NULL)
            {
                //head.szUserName =cJSON_GetObjectItem(msghead,"user")->valuestring;
                sprintf((char *)head.szUserName,"%s",cJSON_GetObjectItem(msghead,"user")->valuestring);

            }
            else
            {
                return false;
            }
            if(cJSON_GetObjectItem(msghead,"passwd") !=NULL)
            {
                sprintf((char *)head.szUserPwd,"%s",cJSON_GetObjectItem(msghead,"passwd")->valuestring);
            }
            else
            {
                return false;
            }
            if(cJSON_GetObjectItem(msghead,"command") !=NULL)
            {
                sprintf((char *)head.szCommand,"%s",cJSON_GetObjectItem(msghead,"command")->valuestring);
            }
            else
            {
                return false;
            }
            if(cJSON_GetObjectItem(msghead,"IMEI") !=NULL)
            {
                sprintf((char *)head.szIMEI,"%s",cJSON_GetObjectItem(msghead,"IMEI")->valuestring);
            }
            else
            {
                return false;
            }

            zlog_debug(g_server_cat,"\nhead.user:%s,\nhead.szUserPwd:%s,\nhead.szCommand:%s,\nhead.szIMEI:%s\n",\
                       head.szUserName,\
                       head.szUserPwd,\
                       head.szCommand,\
                       head.szIMEI);
        return true;
        }
}


bool ToPkg::JSONToPkgTraj(cJSON* root,PackTraj *packtraj,int &arrayLen)
{
     bool res = false;
     cJSON *arrayItem,*item,*object;
     int size ;


     cJSON* msgbody = cJSON_GetObjectItem(root, "body");
     if(msgbody == NULL)
     {
         return false;
     }
     else
     {
        arrayItem=cJSON_GetObjectItem(msgbody,"content");
        if(arrayItem!=NULL)
        {
            size=cJSON_GetArraySize(arrayItem);
            arrayLen = size;
            zlog_debug(g_server_cat,"cJSON_GetArraySize: size=%d\n",size);

            for(int i=0;i<size;i++)
            {

                object=cJSON_GetArrayItem(arrayItem,i);

                item=cJSON_GetObjectItem(object,"latitude");
                if(item!=NULL)
                {
                    sprintf((char *)packtraj[i].latitude,"%s",item->valuestring);
                    //memcpy(packtraj[i].latitude,item->valuestring,strlen(item->valuestring));
//                    printf("packtraj[i].latitude:%s\n",packtraj[i].latitude);
//                    printf("packtraj[i].latitude:%d\n",strlen(item->valuestring));
                }
                else
                {
                    zlog_error(g_server_cat,"cJSON_GetObjectItem: get latitude failed\n");
                }


                item=cJSON_GetObjectItem(object,"longitude");
                if(item!=NULL)
                {
                    sprintf((char *)packtraj[i].longitude,"%s",item->valuestring);
                   // memcpy(packtraj[i].longitude,item->valuestring,strlen(item->valuestring));
//                    printf("packtraj[i].longitude:%s\n",packtraj[i].longitude);
//                    printf("packtraj[i].latitude:%d\n",strlen(item->valuestring));
                }
                else
                {
                    zlog_error(g_server_cat,"cJSON_GetObjectItem: get longitude failed\n");
                }


                item=cJSON_GetObjectItem(object,"speed");
                if(item!=NULL)
                {
                    sprintf((char *)packtraj[i].speed,"%s",item->valuestring);
                   // memcpy(packtraj[i].speed,item->valuestring,strlen(item->valuestring));
//                    printf("packtraj[i].speed:%s\n",packtraj[i].speed);
//                    printf("packtraj[i].latitude:%d\n",strlen(item->valuestring));
                }
                else
                {
                    zlog_error(g_server_cat,"cJSON_GetObjectItem: get speed failed\n");
                }

                item=cJSON_GetObjectItem(object,"altitude");
                if(item!=NULL)
                {
                    sprintf((char *)packtraj[i].altitude,"%s",item->valuestring);
                   // memcpy(packtraj[i].altitude,item->valuestring,strlen(item->valuestring));
//                    printf("packtraj[i].altitude:%s\n",packtraj[i].altitude);
//                    printf("packtraj[i].latitude:%d\n",strlen(item->valuestring));
                }
                else
                {
                    zlog_error(g_server_cat,"cJSON_GetObjectItem: get altitude failed\n");
                }

                item=cJSON_GetObjectItem(object,"direction_angle");
                if(item!=NULL)
                {
                    sprintf((char *)packtraj[i].direction_angle,"%s",item->valuestring);
                   //  memcpy(packtraj[i].direction_angle,item->valuestring,strlen(item->valuestring));
//                     printf("packtraj[i].direction_angle:%s\n",packtraj[i].direction_angle);
//                     printf("packtraj[i].latitude:%d\n",strlen(item->valuestring));
                }
                else
                {
                    zlog_error(g_server_cat,"cJSON_GetObjectItem: get direction_angle failed\n");
                }
                item=cJSON_GetObjectItem(object,"stamp");
                if(item!=NULL)
                {
                    sprintf((char *)packtraj[i].stamp,"%s",item->valuestring);
                   // memcpy(packtraj[i].stamp,item->valuestring,strlen(item->valuestring));
//                     printf("packtraj[i].stamp:%s\n",packtraj[i].stamp);
//                     printf("packtraj[i].latitude:%d\n",strlen(item->valuestring));
                }
                else
                {
                    zlog_error(g_server_cat,"cJSON_GetObjectItem: get stamp failed\n");
                }
            }
        }

      }
     for(int i=0;i<size;i++)
            {

             zlog_debug(g_server_cat,"i=%d, latitude:%s,longitude:%s,speed:%s,altitude:%s,direction_angle=%s,stamp=%s\n",\
                        i,
                        packtraj[i].latitude,
                        packtraj[i].longitude,
                        packtraj[i].speed,
                        packtraj[i].altitude,
                        packtraj[i].direction_angle,
                        packtraj[i].stamp);
            }
        //cout<<"packtraj.stamp"<<packtraj.stamp;
            res = true;
        return res;
}

bool ToPkg::JSONToPkgAlarm(cJSON* root, PackAlarm& alarm)
{
        cJSON* msgbody = cJSON_GetObjectItem(root, "body");
        if(msgbody == NULL)
        {
            return false;
        }
        else
        {
            if(cJSON_GetObjectItem(msgbody,"local") !=NULL)
            {
                sprintf((char *)alarm.local,"%s",cJSON_GetObjectItem(msgbody,"local")->valuestring);

            }
            else
            {
                return false;
            }
            if(cJSON_GetObjectItem(msgbody,"latitude") !=NULL)
            {
                sprintf((char *)alarm.latitude,"%s",cJSON_GetObjectItem(msgbody,"latitude")->valuestring);
            }
            else
            {
                return false;
            }
            if(cJSON_GetObjectItem(msgbody,"longitude") !=NULL)
            {
                sprintf((char *)alarm.longitude,"%s",cJSON_GetObjectItem(msgbody,"longitude")->valuestring);
            }
            else
            {
                return false;
            }
            if(cJSON_GetObjectItem(msgbody,"speed") !=NULL)
            {
                sprintf((char *)alarm.speed,"%s",cJSON_GetObjectItem(msgbody,"speed")->valuestring);
            }
            else
            {
                return false;
            }
            if(cJSON_GetObjectItem(msgbody,"altitude") !=NULL)
            {
                sprintf((char *)alarm.altitude,"%s",cJSON_GetObjectItem(msgbody,"altitude")->valuestring);
            }
            else
            {
                return false;
            }
            if(cJSON_GetObjectItem(msgbody,"direction_angle") !=NULL)
            {
                sprintf((char *)alarm.direction_angle,"%s",cJSON_GetObjectItem(msgbody,"direction_angle")->valuestring);
            }
            else
            {
                return false;
            }
            if(cJSON_GetObjectItem(msgbody,"stamp") !=NULL)
            {
                sprintf((char *)alarm.stamp,"%s",cJSON_GetObjectItem(msgbody,"stamp")->valuestring);
            }
            else
            {
                return false;
            }
            if(cJSON_GetObjectItem(msgbody,"notify_type") !=NULL)
            {
                sprintf((char *)alarm.notify_type,"%s",cJSON_GetObjectItem(msgbody,"notify_type")->valuestring);
            }
            else
            {
                return false;
            }

            zlog_debug(g_server_cat,"\nalarm.local:%s,\nalarm.latitude:%s,\nalarm.longitude:%s,\nalarm.speed:%s, \
                       \nalarm.altitude:%s,\nalarm.direction_angle:%s,\nalarm.stamp:%s,\nalarm.notify_type:%s\n",\
                       alarm.local,\
                       alarm.latitude,\
                       alarm.longitude,\
                       alarm.speed,\
                       alarm.altitude,\
                       alarm.direction_angle,\
                       alarm.stamp,\
                       alarm.notify_type);
        return true;
        }
}

bool ToPkg::PkgHeadToJson(cJSON* root,RecHead& rechead) //组装包头
{
    cJSON* msghead = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "header", msghead);
    cJSON_AddStringToObject(msghead, "user",(char*)rechead.szUserName);
    cJSON_AddStringToObject(msghead, "passwd",(char*)rechead.szUserPwd);
    cJSON_AddStringToObject(msghead, "command",(char*)rechead.szCommand);
    cJSON_AddStringToObject(msghead, "IMEI",(char*)rechead.szIMEI);
    return true;
}


char * ToPkg::ReplyJson(RecHead& rechead,char* result) //
{
    char* reqinfo;
    cJSON* root = cJSON_CreateObject();
    this->PkgHeadToJson(root,rechead);
    cJSON* msgbody = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "body", msgbody);
    cJSON_AddStringToObject(msgbody, "result", result);
    char* out_str = cJSON_Print(root);
    cJSON_Delete(root);
    //cout<<"out_str"<<out_str;
    //strncpy((char*)reqinfo,(char*)out_str,strlen(out_str));
     //cout<<"reqinfo"<<reqinfo;
    //sprintf(reqinfo,"%s",out_str);
    //cout<<"reqinfo"<<reqinfo;
    //delete out_str;
    return out_str;
}

char *  ToPkg::PkgPostoinToJson(RecHead& rechead,PackTraj& packtraj) //组装baoti
{
    cJSON* root = cJSON_CreateObject();
    this->PkgHeadToJson(root,rechead);
    cJSON* msgbody = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "body", msgbody);


    cJSON  *pJsonArry,*pJsonsub;

    pJsonArry=cJSON_CreateArray();   /*创建数组*/
    cJSON_AddItemToArray(pJsonArry,pJsonsub=cJSON_CreateObject()); /* 给创建的数组增加对对象*/


    cJSON_AddStringToObject(pJsonsub, "latitude",(char*)packtraj.latitude);
    cJSON_AddStringToObject(pJsonsub, "longitude",(char*)packtraj.longitude);
    cJSON_AddStringToObject(pJsonsub, "speed",(char*)packtraj.speed);
    cJSON_AddStringToObject(pJsonsub, "altitude",(char*)packtraj.altitude);
    cJSON_AddStringToObject(pJsonsub, "direction_angle",(char*)packtraj.direction_angle);
    cJSON_AddStringToObject(pJsonsub, "stamp",(char*)packtraj.stamp);

    cJSON_AddItemToObject(msgbody, "content",pJsonsub);
    char* out_str = cJSON_Print(root);
    cJSON_Delete(root);

    return out_str;
}

void GetLocalTime(SYSTEMTIME* lpSystemTime)
{
    struct timeval tv;
    gettimeofday (&tv , NULL);

    tm tm_now ;
    localtime_r(&tv.tv_sec,&tm_now);

    lpSystemTime->wYear = tm_now.tm_year+1900;			//年份
    lpSystemTime->wMonth = tm_now.tm_mon+1;				//月 tm[0-11] sys[1-12]
    lpSystemTime->wDay = tm_now.tm_mday;						//日
    lpSystemTime->wDayOfWeek = (tm_now.tm_wday+1)%7 ;		// tm一星期的日数, 从星期一算起, 范围为0-6 sys从星期日算起
    lpSystemTime->wHour =tm_now.tm_hour;					//小时
    lpSystemTime->wMinute = tm_now.tm_min;				//分钟
    lpSystemTime->wSecond = tm_now.tm_sec;				//秒
    lpSystemTime->wMilliseconds = tv.tv_usec/1000;			//毫秒
}
char * stamp_to_standard(int stampTime)
{
        time_t tick = (time_t)stampTime;
        struct tm tm;
        char s[100];

        //tick = time(NULL);
        tm = *localtime(&tick);
        strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
        return s;
}

void int2str(int n, char *str)
{
    char buf[10] = "";
    int i = 0;
    int len = 0;
    int temp = n < 0 ? -n: n;  // temp为n的绝对值

    if (str == NULL)
    {
        return;
    }
    while(temp)
    {
        buf[i++] = (temp % 10) + '0';  //把temp的每一位上的数存入buf
        temp = temp / 10;
    }

    len = n < 0 ? ++i: i;  //如果n是负数，则多需要一位来存储负号
    str[i] = 0;            //末尾是结束符0
    while(1)
    {
        i--;
        if (buf[len-i-1] ==0)
        {
            break;
        }
        str[i] = buf[len-i-1];  //把buf数组里的字符拷到字符串
    }
    if (i == 0 )
    {
        str[i] = '-';          //如果是负数，添加一个负号
    }
}


long long str2int(const char *str)
{
    long long temp = 0;
    const char *ptr = str;  //ptr保存str字符串开头

    if (*str == '-' || *str == '+')  //如果第一个字符是正负号，
    {                      //则移到下一个字符
        str++;
    }
    while(*str != 0)
    {
        if ((*str < '0') || (*str > '9'))  //如果当前字符不是数字
        {                       //则退出循环
            break;
        }
        temp = temp * 10 + (*str - '0'); //如果当前字符是数字则计算数值
        str++;      //移到下一个字符
    }
    if (*ptr == '-')     //如果字符串是以“-”开头，则转换成其相反数
    {
        temp = -temp;
    }

    return temp;
}

int hex_char_value(char c)
{
    if(c >= '0' && c <= '9')
        return c - '0';
    else if(c >= 'a' && c <= 'f')
        return (c - 'a' + 10);
    else if(c >= 'A' && c <= 'F')
        return (c - 'A' + 10);
    //assert(0);
    return 0;
}
int hex_to_decimal(const char* szHex, int len)
{
    int result = 0;
    for(int i = 0; i < len; i++)
    {
        result += (int)pow((float)16, (int)len-i-1) * hex_char_value(szHex[i]);
    }
    return result;
}


