#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED
#define EXIT_PROGRAM 0x4C00
#include <cstdlib>

class MEMORY_SUBSYSTEM;

class CPU_SUBSYSTEM
{
    private:
    int A,X,L,PC,SW,temp;
    MEMORY_SUBSYSTEM *Mptr;
    CPU_SUBSYSTEM();    //No CPU is created without a memory

    public:
    CPU_SUBSYSTEM(MEMORY_SUBSYSTEM *ptr = NULL);
    ~CPU_SUBSYSTEM();
    void InitialisePC(int PCval);
    int ExecuteProgram();
    void DecodeInstruction(int instruction,int& opcode,int& address);
    int ExecuteInstruction(int opcode,int address);
    void RegisterDump();
    void MemoryDump();
};

class MEMORY_SUBSYSTEM
{
  private:
  char *baseptr;
  int size;
  int SIC_Start_Address;

  public :
  MEMORY_SUBSYSTEM();
  ~MEMORY_SUBSYSTEM();
  void InitialiseMemory(int start_add);
  void LoadtoMemory(char *line,int StartAddress,int Length);
  int FetchWord(int address);
  void StoreWord(int address,int data);
  char FetchByte(int address);
  void StoreByte(int address,char data);
  friend class CPU_SUBSYSTEM;
};

char stdinputch();

#endif // MEMORY_H_INCLUDED

