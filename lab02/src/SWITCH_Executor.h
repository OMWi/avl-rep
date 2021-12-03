
#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"
#include <iostream>

class Executor
{
public:
    void Execute(InstructionPtr& instr, Word ip)
    {
        /* YOUR CODE HERE */
        
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

        switch (instr->_aluFunc) {
            case AluFunc::Add:
                result = operand1 + operand2;
                break;
            case AluFunc::Sub:
                result = operand1 - operand2;
                break;
            case AluFunc::And:
                result = operand1 & operand2;
                break;
            case AluFunc::Or:
                result = operand1 | operand2;
                break;
            case AluFunc::Xor:
                result = operand1 ^ operand2;
                break;
            case AluFunc::Slt:
                result = (SignedWord)operand1 < (SignedWord) operand2;
                break;
            case AluFunc::Sltu:
                result = operand1 < operand2;
                break;
            case AluFunc::Sll:
                result = operand1 << (operand2 % 32);
                break;
            case AluFunc::Srl:
                result = operand1 >> (operand2 % 32);
                break;
            case AluFunc::Sra:
                result = (SignedWord)operand1 >> (operand2 % 32);
                break;
        }
        
        if ((instr->_type == IType::Ld) || (instr->_type == IType::St)) {
            instr->_addr = result;
        }

        switch (instr->_type) {
            case IType::Csrr:
                instr->_data = instr->_csrVal;
                break;
            case IType::Csrw:
                instr->_data = instr->_src1Val;
                break;
            case IType::St:
                instr->_data = instr->_src2Val;
                break;
            case IType::J:
            case IType::Jr:
                instr->_data = ip + 4;
                break;
            case IType::Auipc:
                instr->_data = ip + *(instr->_imm);
                break;
            default:
                instr->_data = result;
                break;
        }

        bool transition;

        switch (instr->_brFunc) {
            case BrFunc::Eq:
                transition = operand1 == operand2;
                break;
            case BrFunc::Neq:
                transition = operand1 != operand2;
                break;
            case BrFunc::Lt:
                transition = (SignedWord)operand1 < (SignedWord)operand2;
                break;
            case BrFunc::Ltu:
                transition = operand1 < operand2;
                break;
            case BrFunc::Ge:
                transition = (SignedWord)operand1 > (SignedWord)operand2;
                break;
            case BrFunc::Geu:
                transition = operand1 > operand2;
                break;
            case BrFunc::AT:
                transition = true;
                break;
            case BrFunc::NT:
                transition = false;
                break;                
        }

        Word nextIp;
        switch (instr->_type) {
            case IType::Br:
            case IType::J:
                nextIp = ip + *(instr->_imm);
                break;
            case IType::Jr:
                nextIp = instr->_src1Val + *(instr->_imm);
                break;
        }
        if (transition) {
            instr->_nextIp = nextIp;
        }
        else {
            instr->_nextIp = ip + 4;
        }        
    }

private:
    /* YOUR CODE HERE */

};

#endif // RISCV_SIM_EXECUTOR_H