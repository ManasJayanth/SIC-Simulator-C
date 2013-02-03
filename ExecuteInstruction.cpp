#include <iostream>
#include "memory.h"

using namespace std;

int CPU_SUBSYSTEM::ExecuteInstruction(int opcode,int address)
{
    int WordData,tmpch;
    char ByteData;

    switch(opcode)
    {
        case 0x18 :
        /*** ADD ***/
        WordData = Mptr->FetchWord(address);
        A += WordData;
        break;

        case 0x40:
        /*** AND ***/
        WordData = Mptr->FetchWord(address);
        A = A & WordData;
        break;

        case 0x28:
        WordData = Mptr->FetchWord(address);
        temp = A - WordData;
        if(temp < 0)
        {
            SW = SW | 0x40;
            SW = SW & 0xFFFF7F;
        }
        else if(temp == 0)
        {
            SW = SW & 0xFFFF3F;
        }
        else
        {
            SW = SW | 0x80;
            SW = SW & 0xFFFFBF;
        }
        /***
        CC = 00 -> Equal
        CC = 01 -> Less than
        CC = 10 -> Greater than
        CC = 11 -> Not used
        ***/
        break;

        case 0x24:
        /*** DIV ***/
        WordData = Mptr->FetchWord(address);
        if(WordData == 0)
        {
            //cout << "Division by zero error";
            return -1;
        }
        A /= WordData;
        break;

        case 0x3C:
        /*** J ***/
        PC = address;
        break;

        case 0x30:
        /*** JEQ ***/
        if (!(SW & 0xC0))
        {
            PC = address;
        }
        break;

        case 0x34:
        /*** JGT ***/
        temp = (SW & 0xC0) >> 6;
        if(temp == 2)
        {
            PC = address;
        }
        break;

        case 0x38:
        /*** JLT ***/
        temp = (SW & 0xC0) >> 6;
        if(temp == 1)
        {
            PC = address;
        }
        break;

        case 0x48:
        /*** JSUB ***/
        L = PC;
        PC = address;
        break;

        case 0x00:
        /*** LDA ***/
        WordData = Mptr->FetchWord(address);
        A = WordData;
        break;

        case 0x50:
        /*** LDCH ***/
        ByteData = Mptr->FetchByte(address);
        tmpch = (int)ByteData;
        tmpch = tmpch & 0xFF;
        A = A & 0xFFFFFF00;
        A = A | tmpch;
        break;

        case 0x08:
        /*** LDL ***/
        WordData = Mptr->FetchWord(address);
        L = WordData;
        break;

        case 0x04:
        /*** LDX ***/
        WordData = Mptr->FetchWord(address);
        X = WordData;
        break;

        case 0x20:
        /*** MUL ***/
        WordData = Mptr->FetchWord(address);
        A *= WordData;
        break;

        case 0x44:
        /*** OR ***/
        WordData = Mptr->FetchWord(address);
        A = A | WordData;
        break;

        case 0xD8:
        /*** RD ***/
        ByteData = Mptr->FetchByte(address);
        if(ByteData == '\xF1')
        {
            //STD INPUT
            //DeviceSW = DeviceSW & 0xFFFF3F; //Marking Device as busy
            //OR make DEVICE = BUSY

            ByteData = stdinputch();
            if(ByteData == '\n')
            ByteData = 0;
            tmpch = (int)ByteData;
            tmpch = tmpch & 0xFF;
            A = A & 0xFFFFFF00;
            A = A | tmpch;

           // DeviceSW = DeviceSW | 0x40;     //Marking Available again
           // DeviceSW = DeviceSW & 0xFFFF7F;
           // OR make DEVICE = AVAILABLE
        }
        break;

        case 0x4C:
        /*** RSUB **/
        if(L == 0)  //End
        {
            return EXIT_PROGRAM;
        }
        else
        PC = L;
        break;

        case 0x0C:
        /*** STA ***/
        Mptr->StoreWord(address,A);
        break;

        case 0x54:
        /*** STCH ***/
        tmpch = A & 0xFF;
        ByteData = (char)tmpch;
        Mptr->StoreByte(address,ByteData);
        break;

        case 0x14:
        /*** STL ***/
        Mptr->StoreWord(address,L);
        break;

        case 0x10:
        /*** STX ***/
        Mptr->StoreWord(address,X);
        break;

        case 0x1C:
        /*** SUB ***/
        WordData = Mptr->FetchWord(address);
        A -= WordData;
        break;

        case 0xE0:
        /*** TD ***/
        /************
        Test of device is busy
        ************/
        // if device is available
        SW = SW | 0x40;
        SW = SW & 0xFFFF7F; //CC is <

        /*
        Else
        SW = SW & 0xFFFF3F;
        */
        break;

        case 0x2C:
        /*** TIX ***/
        WordData = Mptr->FetchWord(address);
        temp = ++X - WordData;
        if(temp < 0)
        {
            SW = SW | 0x40;
            SW = SW & 0xFFFF7F;
        }
        else if(temp == 0)
        {
            SW = SW & 0xFFFF3F;
        }
        else
        {
            SW = SW | 0x80;
            SW = SW & 0xFFFFBF;
        }

        break;

        case 0xDC:
        /*** WD ***/
        ByteData = Mptr->FetchByte(address);
        if(ByteData == '\x05')
        {
            //STD INPUT
            //DeviceSW = DeviceSW & 0xFFFF3F; //Marking Device as busy
            //OR make DEVICE = BUSY

            ByteData = (char) A;
            cout.put(ByteData);
           // DeviceSW = DeviceSW | 0x40;     //Marking Available again
           // DeviceSW = DeviceSW & 0xFFFF7F;
           // OR make DEVICE = AVAILABLE
        }
        break;

        default:
        return -1;
    }

    return 0;
}
