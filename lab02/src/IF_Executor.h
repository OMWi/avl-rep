
#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"
#include <iostream>

class Executor
{
public:
    void Execute(InstructionPtr& instr, Word ip)
    {    
        Word operand1, operand2, result;
        if (instr->_src1) {
            operand1 = instr->_src1Val;
        }
        if (instr->_imm) {
            operand2 = *(instr->_imm);
        }
        else if (instr->_src2) {
            operand2 = instr->_src2Val;
        }

        if (instr->_aluFunc == AluFunc::Add) 
            result = operand1 + operand2;
        else if (instr->_aluFunc == AluFunc::Sub) 
            result = operand1 - operand2;
        else if (instr->_aluFunc == AluFunc::And) 
            result = operand1 & operand2;
        else if (instr->_aluFunc == AluFunc::Or) 
            result = operand1 | operand2;
        else if (instr->_aluFunc == AluFunc::Xor)
            result = operand1 ^ operand2;
        else if (instr->_aluFunc == AluFunc::Slt)
            result = (SignedWord)operand1 < (SignedWord)operand2;
        else if (instr->_aluFunc == AluFunc::Sltu)
            result = operand1 < operand2;
        else if (instr->_aluFunc == AluFunc::Sll) 
            result = operand1 << (operand2 % 32);
        else if (instr->_aluFunc == AluFunc::Srl)
            result = operand1 >> (operand2 % 32);
        else if (instr->_aluFunc == AluFunc::Sra)
            result = (SignedWord)operand1 >> (operand2 % 32);
        
        if ((instr->_type == IType::Ld) || (instr->_type == IType::St)) {
            instr->_addr = result;
        }

        if (instr->_type == IType::Csrr)
            instr->_data = instr->_csrVal;
        else if (instr->_type == IType::Csrw)
            instr->_data = instr->_src1Val;
        else if (instr->_type == IType::St)
            instr->_data = instr->_src2Val;
        else if (instr->_type == IType::J || instr->_type == IType::Jr)
            instr->_data = ip + 4;
        else if (instr->_type == IType::Auipc)
            instr->_data = ip + *(instr->_imm);
        else 
            instr->_data = result;

        bool transition;

        if (instr->_brFunc == BrFunc::Eq)
            transition = instr->_src1Val == instr->_src2Val;
        else if (instr->_brFunc == BrFunc::Neq)
            transition = instr->_src1Val != instr->_src2Val;
        else if (instr->_brFunc == BrFunc::Lt) 
            transition = (SignedWord)instr->_src1Val < (SignedWord)instr->_src2Val;
        else if (instr->_brFunc == BrFunc::Ltu)
            transition = instr->_src1Val < instr->_src2Val;
        else if (instr->_brFunc == BrFunc::Ge)
            transition = (SignedWord)instr->_src1Val >= (SignedWord)instr->_src2Val;
        else if (instr->_brFunc == BrFunc::Geu)
            transition = instr->_src1Val >= instr->_src2Val;
        else if (instr->_brFunc == BrFunc::AT)
            transition = true;
        else if (instr->_brFunc == BrFunc::NT)
            transition = false;

        Word nextIp;
  
        if (transition) {
            if (instr->_type == IType::Br || instr->_type == IType::J)
                instr->_nextIp = ip + *(instr->_imm);
            else if (instr->_type == IType::Jr)
                instr->_nextIp = *(instr->_imm) + instr->_src1Val;
        }
        else {
            instr->_nextIp = ip + 4;
        }
    }

private:

};

#endif // RISCV_SIM_EXECUTOR_H
