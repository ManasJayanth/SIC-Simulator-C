#include <iostream>
#include "memory.h"

using namespace std;

CPU_SUBSYSTEM::CPU_SUBSYSTEM(MEMORY_SUBSYSTEM *ptr)
{
    Mptr = ptr;
    A = X = L = PC = SW = 0;
}

CPU_SUBSYSTEM::~CPU_SUBSYSTEM()
{

}

void CPU_SUBSYSTEM::InitialisePC(int PCval)
{
    PC = PCval;
}

void CPU_SUBSYSTEM::MemoryDump()
{
    cout << "Memory :\n";
    for(int i = 0x1039; i < 0x1049; i++)
    {
        cout << (int*)Mptr->baseptr[i] << ' ';
        if(i % 8 == 0)
        {
            cout << endl;
        }
    }
}

int CPU_SUBSYSTEM::ExecuteProgram()
{
    /**** Fetch Decode Execute cycles ****/
    int address,opcode;

    do
    {
        //MemoryDump();
        //RegisterDump();
        int instruction = Mptr->FetchWord(PC);
        //cout << "Executing Instruction : " << (int*)instruction << "\n";
        PC += 3;

        DecodeInstruction(instruction,opcode,address);

    }while(ExecuteInstruction(opcode,address) != EXIT_PROGRAM);

    return A;
}

void CPU_SUBSYSTEM::RegisterDump()
{
    cout << "\n==============================================\n";
    cout << "\nA : " << A;
    cout << "\tX : " << X;
    cout << "\nL : " << L;
    cout << "\tPC : " << (int*)PC;
    cout << "\nSW : " << SW;
    cout << "\n==============================================\n";
    cout << "Press any key...\n";
    cin.get();
}

void CPU_SUBSYSTEM::DecodeInstruction(int instruction,int& opcode,int& address)
{
    bool INDEXED = false;
    if(instruction & 0x8000)
    {
        INDEXED = true;
    }
    address = instruction & 0x7FFF;
    opcode = instruction & 0xFF0000;
    opcode = opcode >> 16;
    if(INDEXED)
    {
        address += X;
    }
}
