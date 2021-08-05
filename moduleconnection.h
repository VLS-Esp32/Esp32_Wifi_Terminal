///////////////////////////////
// VLS TCP stream class header
// v1.0.1 03.07.2021
// Created by VLS
///////////////////////////////

#ifndef MODULECONNECTION_H
#define MODULECONNECTION_H
#include <unistd.h>
#include <thread>
#include <sys/types.h>

#ifdef __WIN32__
# include <winsock2.h>
#else
# include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif


using namespace std;

class ModuleConnection
{

public:

 int SetConnection(string IpAdress, uint16_t port);
 int Disconnect();
 string IpAdress();

 int Available();  //IO methods

 bool Write(string message);
 bool Write(char message);
 bool Write(int message);
 bool Write(float message);

 uint8_t Peek();
 uint8_t Read();
 string ReadString();
 bool IsConnected();

private:
static void _listener(ModuleConnection * p);
void _MoveBuffer();
char Readbuffer[1024];
int _IncomingBufferCurrentPosition=0;
bool _ReadingIsBlocked=false;
int sock;
string _ModuleIpAdress;
bool connected =false;
};

#endif // MODULECONNECTION_H
