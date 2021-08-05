#include <iostream>
#include "modulesearch.h"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <QString>
#include <QStringList>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#ifdef __WIN32__
# include <winsock2.h>
#include <IpHlpAPI.h>
extern int inet_aton(const char *cp, struct in_addr *inp);
#define CLOSE_SOCKET closesocket(sock);
#define SOCKETCLEANUP WSACleanup();
#else
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define CLOSE_SOCKET close(sock);
#define SOCKETCLEANUP
#endif

using namespace std;


int ModuleSearch::SearchModules(string *arr){

    string mask = GetLocalMachineIPMask();
    int FoundModulesCount=0;

    if(mask == ""){
        return -1;
    }

    char msg[21] = "11111111111111111111"; //3283

#ifdef __WIN32__
    WSADATA ws;
    if(FAILED (WSAStartup (MAKEWORD( 1,1 ), &ws)))
       {
         cout << "Windows WSA startup failed!" << endl;
       }
#endif

     for(int i=1; i<255; i++){

        int sock;
        struct sockaddr_in addr;


        sock = socket(AF_INET, SOCK_DGRAM, 0);

           if(sock < 0)
           {
               cout << "Can't create socket: " << endl;
           }

           addr.sin_family = AF_INET;
           addr.sin_port = htons(3283);

           string IpAdress = "192.168."+ mask + "." + to_string(i);

           char ip_array[IpAdress.length()];

           strcpy(ip_array, IpAdress.c_str());

           int errhnd = inet_aton(ip_array, &addr.sin_addr);

           if(errhnd==0){ cout << "Wrong ip?!" << endl;} //If ip convertation crashed

           sendto(sock, msg, 21, 0, (struct sockaddr *)&addr, sizeof(addr));

           CLOSE_SOCKET
    }


        int sock;
        struct sockaddr_in addr;
        char buf[256];
        int bytes_read;

        sock = socket(AF_INET, SOCK_DGRAM, 0);

        if(sock < 0)
        {
            return -1;
        }

        addr.sin_family = AF_INET;
        addr.sin_port = htons(3283);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            cout << "Can't bind listening socket" << endl;
        }

 #ifdef __WIN32__
        DWORD dw = 2000;
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,(char*)&dw,sizeof(dw)) < 0) { // Set socket timeout
           cout << "Error! Timeout not include, try restart app" << endl;
           return 0;
        }
#else
        struct timeval tv;  //Set timeout
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv,sizeof(tv)) < 0) { // Set socket timeout
           cout << "Error! Timeout not include, try restart app" << endl;
           return 0;
        }
#endif

        while(true)
        {
            bytes_read = recvfrom(sock, buf, 256, 0, NULL, NULL);

            if(bytes_read <= 0){
              cout << "UDP listener end" << endl;
              break;
            }

           if(buf[0] == 'I' && buf[10] == '2'){

                    arr[FoundModulesCount]= string(buf, bytes_read);
                    FoundModulesCount++;
           }

     }


     CLOSE_SOCKET
     SOCKETCLEANUP

     return FoundModulesCount;
}



#ifdef __WIN32__
string ModuleSearch::GetLocalMachineIPMask()
{
    string FindedLocalIpMask="";
    ULONG sz = 0;
    PMIB_IPADDRTABLE tbl = NULL;
    DWORD ret = ::GetIpAddrTable(tbl, &sz, TRUE);

    if(NO_ERROR != ret && ERROR_INSUFFICIENT_BUFFER != ret)
    {
     cout << "Critical error! Ip table not extracted!" << endl;
     return FindedLocalIpMask;
    }

    tbl = (PMIB_IPADDRTABLE) GlobalAlloc(GHND, sz);

    if(NULL == tbl)
    {
      cout << "Critical error! Ip table not extracted!" << endl;
      return FindedLocalIpMask;
    }

    ret = ::GetIpAddrTable(tbl, &sz, TRUE);

    if(NO_ERROR != ret)
    {
        cout << "Critical error! Ip table not extracted!" << endl;
        GlobalFree(tbl);
        return FindedLocalIpMask;

    }

    for(size_t i = 0; i<tbl->dwNumEntries; ++i)
    {
        in_addr addr;
        addr.S_un.S_addr = tbl->table[i].dwAddr;

        char* ch = ::inet_ntoa(addr);

        if(NULL == ch)
        {
            cout << "Ip not extracted!" << endl;
            continue;
        }

        QString Editable= QString(ch);    //Checing adress via QString
        QStringList AdressBytes = Editable.split(".");

        if(AdressBytes.value(0) == "192" && AdressBytes.value(1) == "168"){
         FindedLocalIpMask = AdressBytes.value(2).toStdString();

         cout << "Needed adress finded!" << endl;
        }

    }

    GlobalFree(tbl);
    return FindedLocalIpMask;
}

#else
string ModuleSearch::GetLocalMachineIPMask(){

       struct ifaddrs * ifAddrStruct=NULL;
       struct ifaddrs * ifa=NULL;
       void * tmpAddrPtr=NULL;

       string FindedLocalIpMask="";

       getifaddrs(&ifAddrStruct);

       cout << "Try get local IP from adress family"<< endl;

       for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {

           if (ifa ->ifa_addr->sa_family==AF_INET) {

               tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
               char addressBuffer[INET_ADDRSTRLEN];
               inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

               cout << "Finded interface ip: "<< addressBuffer << endl;

               QString Editable= QString(addressBuffer);    //Checing adress via QString
               QStringList AdressBytes = Editable.split(".");

               if(AdressBytes.value(0) == "192" && AdressBytes.value(1) == "168"){
                FindedLocalIpMask = AdressBytes.value(2).toStdString();

                cout << "Needed adress finded!" << endl;
               }

            }
       }

       if (ifAddrStruct!=NULL){
           freeifaddrs(ifAddrStruct);//remember to free ifAddrStruct
       }

        return FindedLocalIpMask;



}

#endif


