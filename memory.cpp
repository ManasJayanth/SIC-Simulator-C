#include "memory.h"
#include <cstdlib>
#include <iostream>

using namespace std;

MEMORY_SUBSYSTEM::MEMORY_SUBSYSTEM()
{
  size = 0;
  baseptr = NULL;
}

MEMORY_SUBSYSTEM::~MEMORY_SUBSYSTEM()
{
  delete[] baseptr;
}

void MEMORY_SUBSYSTEM::InitialiseMemory(int start_add)
{
  SIC_Start_Address = start_add;
  size = 0x8000;
  baseptr = new char[size];
  if(baseptr == NULL)
  {
      cout << "Error : MEMORY_SUBSYSTEM::InitialiseMemory() couldn't allocate memory\n";
  }
}

void MEMORY_SUBSYSTEM::LoadtoMemory(char *line,int StartAddress,int Length)
{
    int i = 9,num = 0,BytesRead = 0,index = StartAddress;
    while(BytesRead < Length)
    {
        char ch = line[i++];
        if(ch >= 'A')
        {
            ch -= 7;
        }
        ch -= 48;
        num = ch << 4;

        ch = line[i++];
        if(ch >= 'A')
        {
            ch -= 7;
        }
        ch -= 48;
        num += ch;
        BytesRead++;
        StoreByte(index++,(char)num);
    }
}

int MEMORY_SUBSYSTEM::FetchWord(int address)
{
    bool g = false;
    int num1 = 0,num2 = 0;

    for(int i = 0; i < 3; ++i)
    {
        char ch = baseptr[address++];
        num2 = (int)ch;
        num2 = num2 & 0x000000FF;
        num1 = num1 | num2;
        num1 = num1 << 8;
    }
    num1 = num1 >> 8;
    if(num1 & 0x800000)
    {
        num1 = num1 | 0xFF000000;
    }
    return num1;
}

char MEMORY_SUBSYSTEM::FetchByte(int address)
{
    return baseptr[address];
}

void MEMORY_SUBSYSTEM::StoreWord(int address,int data)
{
    char ch;
    for(int i = 2; i >= 0; --i)
    {
        ch = data & 0xFF;
        baseptr[address + i] = ch;
        data = data >> 8;
    }
}

void MEMORY_SUBSYSTEM::StoreByte(int address,char data)
{
    baseptr[address] = data;
}
