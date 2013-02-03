#include <iostream>
#include <fstream>
#include <cstring>
#include "memory.h"

#define MAX_RECORD_LENGTH 69
#define HEADER_RECORD_LENGTH 19
#define TEXT_RECORD_LENGTH 69
#define END_RECORD_LENGTH 7

using namespace std;

void ReadHeaderRecord(char *line,char *progname,int &strtadd,int &objlen)
{
    int i,ctr = 0;
    for(i = 1; i < 7; ++i) //!= 7 because index starts from 0 not 1
    {
        progname[ctr++] = line[i];
    }
    progname[ctr] = '\0';

    int num = 0;
    while(i < 13)
    {
        char ch = line[i++];
        if(ch >= 'A')
        {
            ch -= 7;
        }

        ch -= 48;
        num += (int) ch;
        num = num << 4;
    }
    num = num >> 4;
    strtadd = num;

    num = 0;
    while(i < 19)
    {
        char ch = line[i++];
        if(ch >= 'A')
        {
            ch -= 7;
        }

        ch -= 48;
        num += (int) ch;
        num = num << 4;
    }
    num = num >> 4;
    objlen = num;
}

void ReadTextRecord(char *line,int &RecordStartAdd,int &RecordLength)
{
    int i = 1,num = 0;
    while(i < 7)
    {
        char ch = line[i++];
        if(ch >= 'A')
        {
            ch -= 7;
        }

        ch -= 48;
        num += (int) ch;
        num = num << 4;
    }
    num = num >> 4;
    RecordStartAdd = num;
    num = 0;
    while(i < 9)
    {
        char ch = line[i++];
        if(ch >= 'A')
        {
            ch -= 7;
        }

        ch -= 48;
        num += (int) ch;
        num = num << 4;
    }
    num = num >> 4;
    RecordLength = num;
}

void ReadEndRecord(char *line,int &FirstExecIns)
{
    int i = 1,num = 0;
    while(i < 7)
    {
        char ch = line[i++];
        if(ch >= 'A')
        {
            ch -= 7;
        }

        ch -= 48;
        num += (int) ch;
        num = num << 4;
    }
    FirstExecIns = num >> 4;
}
int main(int argc,char *argv[])
{
    char ProgramName[7];
    int StartingAddress = 0,ObjcodeLength = 0,RecordStartAddress = 0,RecordLength = 0,FirstExecIns = 0;
    fstream obj;
    char line[MAX_RECORD_LENGTH + 1]; //+1 for NULL character
    MEMORY_SUBSYSTEM virMemory;
    CPU_SUBSYSTEM virCPU(&virMemory);
    char filename[256];

    if(argc == 2)
    {
        strcpy(filename,argv[1]);
    }

    else
    {
        strcpy(filename,"object.obj");
    }

    obj.open(filename,ios::in);
    if(!obj)
    {
        cerr << "Error opening file : " << filename << "\n";
        return -1;
    }



    obj.getline(line,70);
    ReadHeaderRecord(line,ProgramName,StartingAddress,ObjcodeLength);
    virMemory.InitialiseMemory(StartingAddress);

    obj.getline(line,70);
    while(line[0] != 'E')
    {
        ReadTextRecord(line,RecordStartAddress,RecordLength);
        virMemory.LoadtoMemory(line,RecordStartAddress,RecordLength);
        obj.getline(line,70);
    }

    ReadEndRecord(line,FirstExecIns);
    virCPU.InitialisePC(FirstExecIns);
    virCPU.ExecuteProgram();

    cin.get();
    return 0;
}
