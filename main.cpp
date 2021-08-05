#include "mainwindow.h"
#include "moduleconnection.h"
#include <QApplication>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <thread>
#include "modulesearch.h"

#ifdef WIN32
extern int inet_aton(const char *cp, struct in_addr *inp)
{
    if (sizeof(cp) == 0 || sizeof(cp) == 0)
    {
        return 0;
    }

    unsigned long addr = inet_addr(cp);
    if (addr == INADDR_NONE)
    {
        return 0;
    }

     inp->s_addr = addr;
   }

#endif

using namespace std;


ModuleConnection Module;
uint8_t DropConnectionReason=0;



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow wind(nullptr);
    wind.show();



    return a.exec();
    //return 0;
}
