
#ifndef RISCV_SIM_CPU_H
#define RISCV_SIM_CPU_H

#include "Memory.h"
#include "Decoder.h"
#include "RegisterFile.h"
#include "CsrFile.h"
#include "Executor.h"

class Cpu
{
public:
    Cpu(Memory& mem)
        : _mem(mem)
    {

    }

    void ProcessInstruction()
    {
        /* YOUR CODE HERE */         
        Word word = _mem.Request(_ip);
        InstructionPtr instrPtr = _decoder.Decode(word);
        _rf.Read(instrPtr);
        _csrf.Read(instrPtr);
        _exe.Execute(instrPtr, _ip); 
        _mem.Request(instrPtr);
        _rf.Write(instrPtr);
        _csrf.Write(instrPtr);
        _csrf.InstructionExecuted(); 
        _ip = instrPtr->_nextIp;
        
    }

    void Reset(Word ip)
    {
        _csrf.Reset();
        _ip = ip;
    }

    std::optional<CpuToHostData> GetMessage()
    {
        return _csrf.GetMessage();
    }

private:
    Reg32 _ip;
    Decoder _decoder;
    RegisterFile _rf;
    CsrFile _csrf;
    Executor _exe;
    Memory& _mem;
};


#endif //RISCV_SIM_CPU_H
