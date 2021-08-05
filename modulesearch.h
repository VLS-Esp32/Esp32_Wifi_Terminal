#ifndef MODULESEARCH_H
#define MODULESEARCH_H

#include <iostream>
#include <string.h>


using namespace std;



class ModuleSearch
{
public:

    int SearchModules(string *arr);

private:


string GetLocalMachineIPMask();
};

#endif // MODULESEARCH_H
