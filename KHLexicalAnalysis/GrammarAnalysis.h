#pragma once
#ifndef _GRAMMAR_ANALYSIS 
#define _GRAMMAR_ANALYSIS

#include "DFAMachine.h"
#include "_DFA_Adapter_Token.h"

namespace KH {

	template <class _Adapter, class _Terminator>
	class GrammarAnalysis {
	
	public:
		typedef _Adapter TokenAdapter;
		typedef _Terminator Token;
		typedef DFAMachine<TokenAdapter, Token>  Machine;



		GrammarAnalysis<TokenAdapter, Token> &AddTransition(int org, int aim, TokenAdapter ta) {
			dfam.AddTransition(org, aim, 1, ta);
			return *this;
		}


	private:
		Machine dfam;

	}; //end of class GA

}; // end of namespace 


#endif