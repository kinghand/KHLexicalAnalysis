#pragma once 
#ifndef _KH_DFA_INTERFACE
#define _KH_DFA_INTERFACE

#include "_DFA.h"
#include "_DFA_Adapter_Range.h"
#include<stdarg.h>

namespace KH{
	
	template<class _Adapter,class _Terminator>
	class DFAMachine{
		typedef _Adapter Adapter;
		typedef _Terminator Terminator;
		typedef State<_Adapter,_Terminator> _State;

	public:

		DFAMachine(){ 
			dfa = DFA<_Adapter,_Terminator>(_Adapter());
			//pDfa = new DFA<_Adapter,_Terminator>(_Adapter());
		}

		~DFAMachine(){ 
			if(pDfa != NULL)
				delete pDfa;
		}


		DFAMachine(_Adapter a){ 
			dfa = DFA<_Adapter,_Terminator>(a);
			//pDfa = new DFA<_Adapter,_Terminator>(a);
		}

		DFAMachine(int numofstate,int numoffinal, ...){ //��ʼ��

			std::vector<DFA_Transition<Adapter,_State>> transitions;

			this->numOfState = numofstate ;

			/*	����ָ�������� State	*/
			State<_Adapter,_Terminator>* pState;	
			for(int i = 0; i < numOfState; i++) {
				sList.push_back(State<_Adapter,_Terminator>());
			}

			/*	���ָ����final state	*/ 
			va_list argp; 
			int para = 0;
			va_start(argp,numoffinal);
			for(int i=0; i < numoffinal; i++)
			{
				para = va_arg(argp,int);
				if(para>=0 && para < numofstate) {
					spListFinal.push_back(&sList[para]);
				}
				else throw 0;
			}

			/* ���� DFA */
			dfa = DFA<_Adapter,_Terminator>(sList,spListFinal,transitions);
			/*if(pDfa != NULL)
				delete pDfa;*/
			//pDfa = new DFA<_Adapter,_Terminator>(sList,spListFinal,transitions);
		}

		/*	ΪDFA�������ת������	*/
		DFAMachine& AddTransition(int nState, int nNextState, int nAdapter, ...) {
			va_list argp; 
			_Adapter para = 0;
			va_start(argp,nAdapter);
			for(int i = 0; i < nAdapter; i ++)
			{
				para = va_arg(argp,_Adapter);
				dfa.AddTransition(nState, nNextState, para);
				//pDfa->AddTransition(nState, nNextState, para);
			}
			return (*this);
		}

		/*DFAMachine& AddTransition(int nState, int nNextState, _Adapter a) {
			AddTransition(1,1,1,a);
			return (*this);
		}*/

		/* ���� */
		void Reset() { 
			dfa.Reset(); 
			//pDfa->Reset();
		}

		/* ����Ƿ�Ϊ��̬	*/
		bool CheckFinal(){
			//return pDfa->CheckFinal();
			return dfa.CheckFinal();
		}

		bool CheckNULL(){
			//return pDfa->CheckNULL();
			return dfa.CheckNULL();
		}

		bool InputTerminator(Terminator t){
			//return pDfa->InputTerminator(t);
			return dfa.InputTerminator(t);
			
		}

		friend DFAMachine& operator<<(DFAMachine& dm, Terminator t){
			dm.InputTerminator(t);
			return dm;
		}

	private :
		DFA<_Adapter,_Terminator> *pDfa;
		DFA<_Adapter,_Terminator> dfa;
		std::vector<_State> sList;
		std::vector<_State*> spListFinal;

		int numOfState;
	};

	/* Adapter define */

	RangeAdapter aDigit('0','9');
	RangeAdapter aLower('a','z');
	RangeAdapter aUpper('A','Z');
	RangeAdapter aPlus('+');
	RangeAdapter aMinus('-');
	RangeAdapter aTimes('*');
	RangeAdapter aSlash('/');
	RangeAdapter aBackSlash('\\');
	RangeAdapter aLParen('(');
	RangeAdapter aRParen(')');
	RangeAdapter aEqual('=');
	RangeAdapter aComma(',');
	RangeAdapter aPeriod('.');
	RangeAdapter aColon(':');
	RangeAdapter aSemicolon(';');
	RangeAdapter aUnderline('_');
	RangeAdapter aQuote('"');
	RangeAdapter aSharp('#');
	RangeAdapter aBlockSpace(' ');
	RangeAdapter aBlockNewline('\r');
	RangeAdapter aBlockEnter('\n');
	RangeAdapter aBlockTable('\t');
	RangeAdapter aAllChar(0,127);

	typedef DFAMachine<RangeAdapter,char> DFARange;
};


#endif