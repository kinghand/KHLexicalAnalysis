#pragma once
#ifndef _KH_DFA
#define _KH_DFA

#include <vector>


namespace KH{
	
	//Ҫ��boost::shared_ptr�𣿻������˰�..

	template<class _Terminator, class _Adapter, class _State>
	class Transition{ //����֧��State������࣬���ڲ��߼���д
	public:
		_State* pNextState;
		_Adapter acceptedAdapter; //����ĳ�ָ��Ͳ����ˣ�����
		
		Transition(_Adapter &a, _State *pNext){
			acceptedAdapter = a;
			pNextState = pNext ;
		}

		_State* operator+(_Terminator t){
			if(acceptedAdapter == t){
				return pNextState;
			}
			else return NULL;
		}
	};

	template<class _Adapter, class _Terminator>
	class State{ //������ʾ״̬����
	public:
		typedef _Adapter Adapter;
		typedef _Terminator Terminator;
		std::vector<Transition<_Terminator,_Adapter,State>> transitions;

		~State(){
			transitions.swap(std::vector<Transition<_Terminator,_Adapter,State>>() );
		}

		State* operator+(_Terminator t){
			State* nextState;
			std::vector<Transition<_Terminator, Adapter, State>>::iterator it;
			if(!transitions.empty())
				it = transitions.begin();
			else
				return NULL;
			for(;it != transitions.end(); it++){
				nextState = *it + t;
				if(NULL != nextState) return nextState;
			}
			return NULL;
		}

		void AcceptTransition(_Adapter &a, State *pNext){ //����ת���ĺ���
			Transition<_Terminator,_Adapter,State> *pTAS = new Transition<_Terminator,_Adapter,State>(a,pNext);
			transitions.push_back(*pTAS); 
		}
	};

	template<class _Adapter, class _State>
	class DFA_Transition{ //��������ת����������
	public:
		typedef _Adapter Adapter;
		typedef _State State;
		DFA_Transition();
		DFA_Transition(State *pOrg, _Adapter &adapter, State *pNext) {
			this->pState = pOrg;
			this->adapter = adapter;
			this->pNextState = pNext;
		}

		void TransitionToState(){
			this->pState->AcceptTransition(adapter, pNextState);
		}

	private:
		_State *pState;
		_State *pNextState;
		_Adapter adapter;
	};

	template<class _Adapter,class _Terminator, class _State = State<_Adapter,_Terminator>>
	class DFA{ // DFA������
	public:
		typedef _Terminator Terminator;
		typedef _State State;
		typedef _Adapter Adapter;

		DFA() {
			pStates = new std::vector<_State>();
			pStates->push_back(_State());
			pStates->push_back(_State());
			pFinalStates = new std::vector<_State*>();
			pFinalStates->push_back(&(*pStates)[1]);
			AddTransition(0,0,_Adapter());
			pCurrentState = &(*pStates)[0];
		}

		DFA(_Adapter &a) {
			//pAdapterOnly = _Adapter(a);//������ģʽ
			pStates = new std::vector<_State>();
			pStates->push_back(_State());
			pStates->push_back(_State());
			pFinalStates = new std::vector<_State*>();
			pFinalStates->push_back(&(*pStates)[1]);
			AddTransition(0,1,a);
			pCurrentState = &(*pStates)[0];
		}

		DFA(std::vector<_State> &sList, 
			//std::vector<_Adapter> &tList, 
			std::vector<_State*> &sListFinal, 
			std::vector<DFA_Transition<_Adapter,_State>> &transitions
			)
		{	//ע������ʵ���϶�������ָ�룬����������ⲿ�ͷŵ�����ͷҲ��û��

			//pAdapterOnly = NULL;

			pCurrentState = &sList[0];
			pStates = &sList;
			//pTerminators = &tList;
			pFinalStates = &sListFinal;

			for(std::vector<DFA_Transition<_Adapter,_State>>::iterator it = transitions.begin(); it != transitions.end(); it ++) {
				it->TransitionToState();
			}//�����������transitionת�Ƶ�state��

			transitions.swap(std::vector<DFA_Transition<_Adapter,_State>>() ); //��������DFA_transition
		}

		//Get
		bool CheckTerminator(_Adapter a, _Terminator t) const {
			return a == t;
		}

		bool CheckFinal(){ //������ģʽ��û������
			for(std::vector<_State*>::iterator iS = pFinalStates->begin(); iS != pFinalStates->end(); iS ++) {
				if(*iS == pCurrentState)return true;
			}
			return false;
		}

		bool CheckNULL(){
			return pCurrentState == NULL;
		}

		int CheckStateNum() const{
			return pStates->size();
		}

		_State* GetCurrentState() const {
			return pCurrentState;
		}

		//Push
		void AddTransition(int nState, int nNextState, _Adapter &adapter){//����һ��ת������

			if(nState > (int)pStates->size() || nNextState > (int)pStates->size() ) {
				throw 0;
				return;
			}

			DFA_Transition<_Adapter,_State> trans( &(*pStates)[nState], adapter, &(*pStates)[nNextState] );
			trans.TransitionToState();
		}

		bool InputTerminator(_Terminator t) {
			//if(pAdapterOnly != NULL) { //������ģʽ�У�ֱ���ж�
			//	return *pAdapterOnly == t;
			//}

			if(pCurrentState != NULL){
				pCurrentState = *pCurrentState + t;
				return CheckFinal();
			}
			return false;
		};

		bool operator<<( _Terminator &t) {
			return InputTerminator(t);
		}

		void Reset() { 
			/*if(pAdapterOnly == NULL)*/
			pCurrentState = &(*pStates)[0]; 
		}

		/*~DFA() {
			free(pCurrentState);
			free(pStates);
			free(pFinalStates);
		}*/

	private:
		State*	pCurrentState;
		std::vector<_State>*		pStates;
		//std::vector<_Adapter>*	pTerminators;
		std::vector<_State*>*		pFinalStates;
		//_Adapter* pAdapterOnly; //��ΪNULLʱ�����õ�����ģʽ
	};

};

#endif