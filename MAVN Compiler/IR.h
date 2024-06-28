/* Autor: Mihailo Dikanovic Datum: 07.06.2023. */

#pragma once

#include "Types.h"

using namespace std;


class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};


	Variable() : m_type(NO_TYPE), m_value(""), m_name(""), m_assignment(no_assign) {}
	Variable(string name, int pos, VariableType type) : m_type(type), m_name(name), m_value(""), m_position(pos), m_assignment(no_assign) {}

	string typeToS(VariableType vt)
	{
		switch (vt)
		{
		case MEM_VAR:
			return "MEM_VAR";
			break;
		case REG_VAR:
			return "REG_VAR";
			break;
		case NO_TYPE:
			return "NO_TYPE";
			break;
		default:
			return "Error";
			break;
		}
	}

//m_value
	void setValue(string v)
	{
		m_value = v;
	}

	string getValue()
	{
		return m_value;
	}

//m_name
	string getName()
	{
		return m_name;
	}

	void setName(std::string n)
	{
		m_name = n;
	}


//m_type
	VariableType getType()
	{
		return m_type;
	}


//m_assignment
	void setAssignment(Regs r)
	{
		m_assignment = r;
	}

	Regs getAssignment()
	{
		return m_assignment;
	}

//m_position
	int getPosition()
	{
		return m_position;
	}

private:
	int m_position;
	VariableType m_type;
	string m_name;
	string m_value;
	Regs m_assignment;
};

typedef std::list<Variable*> Variables;



class Label
{
public:
	Label(): m_name("") {}
	Label(string n): m_name(n) {}
	
	string getLabelName() 
	{
		return m_name;
	}
private:
	string m_name;
};

typedef std::list<Label*> Labels;



class Instruction
{
public:
	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
	Instruction(string lab, int pos, InstructionType type, Variables& dst, Variables& src) : m_position(pos), m_labela(lab), m_type(type), m_dst(dst), m_src(src) {}
	Instruction (string lab, int pos, int c, InstructionType type, Variables& dst, Variables& src): m_position(pos), m_labela(lab), m_type(type), m_dst(dst), m_src(src), m_konstanta(c) {}
	Instruction(string lab, int pos, string s, InstructionType type, Variables& dst, Variables& src) : m_position(pos), m_labela(lab), m_type(type), m_dst(dst), m_src(src), m_jump(s) {}
	
	string printInstruction() 
	{
		if (m_type == InstructionType::I_ADD) {
			return "add";
		}
		else if(m_type == InstructionType::I_ADDI) {
			return "addi";
		}
		else if(m_type == InstructionType::I_B) {
			return "b";
		}
		else if (m_type == InstructionType::I_BLTZ) {
			return "bltz";
		}
		else if (m_type == InstructionType::I_LA) {
			return "la";
		}
		else if (m_type == InstructionType::I_LI) {
			return "li";
		}
		else if (m_type == InstructionType::I_LW) {
			return "lw";
		}
		else if (m_type == InstructionType::I_NOP) {
			return "nop";
		}
		else if (m_type == InstructionType::I_AND) {
			return "and";
		}
		else if (m_type ==InstructionType::I_OR) {
			return "or";
		}
		else if (m_type == InstructionType::I_SLT) {
			return "slt";
		}
		else if (m_type == InstructionType::I_SUB) {
			return "sub";
		}
		else if (m_type == InstructionType::I_SW) {
			return "sw";
		}
		else
		{
			return "";
		}
	}

//labela
	string getLabela()
	{
		return m_labela;
	}

//m_type
	InstructionType getType()
	{
		return m_type;
	}
	
//m_dst
	Variables* getDst()
	{
		return &m_dst;
	}

//m_src
	Variables* getSrc()
	{
		return &m_src;
	}
	
//m_use
	Variables* getUse()
	{
		return &m_use;
	}

	void setUse(Variable* v)
	{
		m_use.push_back(v);
	}

//m_def
	Variables* getDef()
	{
		return &m_def;
	}
	
	void setDef(Variable* v)
	{
		m_def.push_back(v);
	}

//m_in
	Variables* getIn()
	{
		return &m_in;
	}
	void setIn(Variables v)
	{
		m_in = v;
	}

//m_out
	Variables* getOut()
	{
		return &m_out;
	}
	
	void setOut(Variables v)
	{
		m_out = v;
	}

//m_succ
	std::list<Instruction*>* getSucc()
	{
		return &m_succ;
	}

	void setSucc(Instruction* i)
	{
		m_succ.push_back(i);
	}

//m_pred
	std::list<Instruction*>* getPred()
	{
		return &m_pred;
	}

	void setPred(Instruction* i)
	{
		m_pred.push_back(i);
	}

//konstanta
	int returnKonstanta()
	{
		return m_konstanta;
	}

//jump
	string returnJump()
	{
		return m_jump;
	}

private:
	int m_position;
	int m_konstanta;
	
	string m_labela;
	string m_jump;
	
	InstructionType m_type;
	
	Variables m_dst;
	Variables m_src;
	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;
};

typedef std::list<Instruction*> Instructions;
