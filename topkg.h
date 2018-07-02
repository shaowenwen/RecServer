#ifndef TOPKG_H
#define TOPKG_H
#include <string.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include "cJSON.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <zlog.h>
using namespace std;

typedef struct tagRecHead{
    unsigned char		szUserName[32];			//
    unsigned char		szUserPwd[33];		    //
    unsigned char		szIMEI[32];			//
    unsigned char		szCommand[32];		//

}RecHead;


typedef struct tagPackTraj{
    unsigned char latitude[16];
    unsigned char longitude[16];
    unsigned char speed[16];
    unsigned char altitude[16];
    unsigned char direction_angle[16];
    unsigned char stamp[32];

}PackTraj;



typedef struct tagPackAlarm{
    unsigned char local[16];
    unsigned char latitude[16];
    unsigned char longitude[16];
    unsigned char speed[16];
    unsigned char altitude[16];
    unsigned char direction_angle[16];
    unsigned char stamp[32];
    unsigned char notify_type[32];


}PackAlarm;

typedef unsigned short      WORD;

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
}SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;



void GetLocalTime(SYSTEMTIME* lpSystemTime);
char * stamp_to_standard(int stampTime);
void int2str(int n, char *str);
long long str2int(const char *str);
int hex_char_value(char c);
int hex_to_decimal(const char* szHex, int len);


class ToPkg
{
public:
    ToPkg();
    bool JSONToPkgHead(cJSON* root,RecHead& head);
    bool JSONToPkgTraj(cJSON* root,PackTraj *packtraj,int &arrayLen);
    bool PkgHeadToJson(cJSON* root,RecHead& rechead);
    char* ReplyJson(RecHead& rechead,char* result);
    bool JSONToPkgAlarm(cJSON* root,PackAlarm& alarm);
    char *  PkgPostoinToJson(RecHead& rechead,PackTraj& packtraj);


};

#endif // TOPKG_H
