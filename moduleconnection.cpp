///////////////////////////////
// VLS TCP stream class
// v1.0.1 03.07.2021
// Created by VLS
///////////////////////////////

#include "moduleconnection.h"
#include <iostream>
#include <unistd.h>
#include <thread>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

#ifdef __WIN32__
# include <winsock2.h>
extern int inet_aton(const char *cp, struct in_addr *inp);
#define CLOSE_SOCKET closesocket(sock);
#define SOCKETCLEANUP WSACleanup();
#else
# include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define CLOSE_SOCKET close(sock);
#define SOCKETCLEANUP
#endif

using namespace std;

int ModuleConnection::SetConnection(string IpAdress, uint16_t port){
    if(!connected){

#ifdef __WIN32__
    WSADATA ws;
    if(FAILED (WSAStartup (MAKEWORD( 1,1 ), &ws)))
       {
         cout << "Windows WSA startup failed!" << endl;
       }
#endif

    sock = socket(AF_INET, SOCK_STREAM, 0); //Declaring socket
    struct sockaddr_in addr; //Binding adress

        if(sock < 0)
        {
            cout << "Socket creation critical error!" << endl;   // :(
            return 1;
        }

        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        char ip_array[IpAdress.length()];

        strcpy(ip_array, IpAdress.c_str());

        int errhnd = inet_aton(ip_array, &addr.sin_addr);

        if(errhnd==0){ cout << "What is this ip?!" << endl; return 2;} //If ip conversion crashed

            if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            {
                cout << "Socket connection critical error!" << endl;   // :(
                return 3;
            }

            _ModuleIpAdress=IpAdress;
            connected=true;
            thread Listening(_listener, this);
            Listening.detach(); //Maybe

            cout << "Connection installed" << endl;

            return 0;

    }else{
        return 4;
    }
}

string ModuleConnection::IpAdress(){
    return _ModuleIpAdress;
}

void ModuleConnection::_listener(ModuleConnection * p){
    char rbuff[64];
    while(p->connected){   

    int received = recv(p->sock, rbuff, 63, 0); //Read bytes from socket, or reading connection error

    if(!p->connected){break;}

    if(received == -1){   //Handle result
      p->connected=false;
      p->Disconnect();
      cout << "Fatal socket error!"<<endl;
      break; //Oops, Error!
    }else if(p->_IncomingBufferCurrentPosition+received < 1023){ //foolproof

     while(p->_ReadingIsBlocked);           //Starting normal handle
     p->_ReadingIsBlocked=true;

     for(int i=0;i<received;i++){

         p->Readbuffer[p->_IncomingBufferCurrentPosition+i]=rbuff[i];
        // p->_IncomingBufferCurrentPosition++; // Not now
     }

     p->_IncomingBufferCurrentPosition+=received;
     p->_ReadingIsBlocked=false;
    }

  }
}

bool ModuleConnection::IsConnected(){
    return connected;
}

int ModuleConnection::Available(){
   // cout<< _IncomingBufferCurrentPosition<<endl; //Zlo debug
    return _IncomingBufferCurrentPosition;
}

void ModuleConnection::_MoveBuffer(){

    for(int i=0;i<_IncomingBufferCurrentPosition;i++){
      Readbuffer[i]=Readbuffer[i+1];
    }

}

int ModuleConnection::Disconnect(){

   while(_ReadingIsBlocked);
   _ReadingIsBlocked=true;
   connected=false;

   for(int i=0; i<512;i++){ //Renew bufer
       Readbuffer[i]=255;
   }

   CLOSE_SOCKET
   SOCKETCLEANUP
   _ReadingIsBlocked=false;

   return 1;
}

/////////////////////////////////////////////////////////////IO METODS///////////////////////////////////////////////
uint8_t ModuleConnection::Read(){
    while(_ReadingIsBlocked);

    if(_IncomingBufferCurrentPosition>0){

       _ReadingIsBlocked=true;
       uint8_t Executed=Readbuffer[0];
       _MoveBuffer();
       _IncomingBufferCurrentPosition--;
       _ReadingIsBlocked=false;

    return Executed;
    }else{
        return 255;
    }
}

uint8_t ModuleConnection::Peek(){
    while(_ReadingIsBlocked);

    if(_IncomingBufferCurrentPosition>0){

       uint8_t Executed=Readbuffer[0];

    return Executed;
    }else{
        return 255;
    }
}


string ModuleConnection::ReadString(){
    while(_ReadingIsBlocked);
    if(_IncomingBufferCurrentPosition>0){

        _ReadingIsBlocked=true;
        string Executed;

        for(int i=0;i<_IncomingBufferCurrentPosition;i++){
            Executed+=Readbuffer[i];
        }
        _IncomingBufferCurrentPosition=0;
        _ReadingIsBlocked=false;

     return Executed;
    }else{
        return "";
    }
}

bool ModuleConnection::Write(string message){

    char char_array[message.length()+1];

           for(int i=0;i<message.length();i++){
               char_array[i]=message[i];
           }

       int state = send(sock, char_array, (sizeof(char_array)-1), 0);

       if(state == -1){
           cout << "Error! Socket disconnected!" << endl;
           Disconnect();
          return false;
       }

     return true;
}

bool ModuleConnection::Write(int message){

       int state= Write(to_string(message));

       if(state == -1){
          return false;
       }

     return true;
}

bool ModuleConnection::Write(float message){

       int state= Write(to_string(message));

       if(state == -1){
          return false;
       }

     return true;
}

bool ModuleConnection::Write(char message){

       int state= Write(to_string(message));

       if(state == -1){
          return false;
       }

     return true;
}



