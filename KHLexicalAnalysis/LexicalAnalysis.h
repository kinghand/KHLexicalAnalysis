#pragma once
#ifndef _DFA_LEXICAL_ANALYSIS
#define _DFA_LEXICAL_ANALYSIS

#include"DFAMachine.h"
#include"LexicalAnalysisDefine.h"
#include"string"
#include<stdarg.h>

namespace KH{

	template <class _ValueType = std::string, class _SignType = std::int> 
	class _Token {
	public:
		typedef _SignType SignType;
		typedef _ValueType ValueType;

		//define 
		static const _SignType DefSign = 0; 
		static const _Token DefToken;

		//set
		_Token() { sign = DefSign;}
		_Token(_ValueType v, _SignType s) {
			SetToken(v, s);
		}
		void SetToken(_ValueType v, _SignType s) {
			sign = s;
			value = v;
		}

		//get
		_ValueType GetValue() const { return value; }
		_SignType GetSign() const { return sign; }

		bool operator==(_Token t) { 
			if(sign == t.GetSign() && value == t.GetValue() )	return true; 
			else return false;
		}

		bool IsSign0() { //Ϊ��ʱ
			return sign == _Token::DefSign;
		}

		bool IsDefined() { //�Ƿ��ж���
			return LEXICAL_ANALYSIS_DEFINE::IsTypeDefined(sign);
		}
	private:
		_SignType sign;
		_ValueType value;
	};

	template<class _SignType = int, class _DFAMachine = KH::DFARange>
	class TokenAnalysis {
	public:
		typedef _SignType SignType;
		typedef _DFAMachine DFAMachine;

		TokenAnalysis() { ; }
		TokenAnalysis(_SignType st, _DFAMachine &d) {
			pDFAM = &d;
			sign = st;
		}

		//push
		void SetMachine(_DFAMachine &d) { pDFAM = &d; }
		void SetSign(_SignType st) { sign = st; }
		//pop
		_SignType GetSign() { return sign; }
		_DFAMachine* GetPMachine() { return pDFAM; }

		/*~TokenAnalysis(){
			free(pDFAM);
		}*/

	private:
		_DFAMachine* pDFAM; //!!!!!!!!!!!!
		_SignType sign;
	};

	class LexicalAnalysis { //������
	public:
		typedef _Token<std::string, int> Token;
		typedef TokenAnalysis<int, KH::DFARange> TAIR;
		typedef LEXICAL_ANALYSIS_DEFINE TYPEDEF;
		typedef KH::_ReservedWord<std::string, int>  ReservedWord;
		typedef LA_RWordDef Def;

		LexicalAnalysis(){
			VTAIR.clear();
			InitialReservedWord();
		}

		//Set
		friend LexicalAnalysis& operator<<(LexicalAnalysis& la, TAIR ta) {
			la.GetTAIRList()->push_back(ta);
			return la;
		}

		LexicalAnalysis& AddTair(int sign, TAIR::DFAMachine &dfam) {	//ֱ������tair
			TAIR tair(sign, dfam);
			return (*this)<<tair;
		}

		//Get
		inline std::vector<TAIR>* GetTAIRList() {
			return &VTAIR;
		}

		Token GetAToken( std::string &strInput) {//��ȡtoken��
			std::string::iterator it = strInput.begin();
			std::vector<TAIR>::iterator itTAIR = VTAIR.begin();
			std::string sResult = "";
			int imax = 0;
			TAIR::SignType sign = Token::DefSign ;

			for(; itTAIR != VTAIR.end(); itTAIR ++) {
				int i = 0;
				it = strInput.begin();
				std::string sTemp = "";
				while(it != strInput.end()){
					sTemp += *it;
					if( itTAIR-> GetPMachine()->InputTerminator(*it) && !itTAIR->GetPMachine()->CheckNULL()) { //������ܲ�δ����DFA�����������
						if( i >= imax) { //iʼ�ձȵ�ǰ��С1
							sResult = sTemp;
							sign = itTAIR->GetSign();
							imax = i + 1;
							//continue;
						}
					}
				
					it ++;
					i++;
				}
			}
			Token t;
			if(!strInput.empty() && sResult.empty()){
				std::string sttr;
				sttr.push_back(*strInput.begin());
				t = Token(sttr,sign);
			}
			else 
				t = Token(sResult, sign);

			itTAIR = VTAIR.begin();
			for(; itTAIR != VTAIR.end(); itTAIR ++) {
				itTAIR->GetPMachine()->Reset();
			}

			if(!t.IsDefined())strInput.erase(strInput.begin()); //����Ҳ�������ɾ��һλ
			else {
				it = strInput.begin(); //����ɾ��imaxλ
				for(int i = 0; i < imax; i ++) {
					it ++;
				}
				strInput.erase(strInput.begin(), it);
			}
		
			if(t.GetSign() == LA_Def::_IDENTIFIER) {
				int signWord = reservedWord.Find(t.GetValue());
				std::string s = t.GetValue(); 
				if(signWord != 0) {
					if(Def::IsDefined(signWord))
						t.SetToken( s, signWord); //�ж���ʱ��¼sign
					else
						t.SetToken( s, LA_Def::_RESERVEDWORD);
				}
			}

			return t; //ͨ��.Token::IsUndefine()�����
		}

		Token GetFirstToken(std::string s) { //��ȫȡ�÷�ʽ�������޸������stringֵ
			return GetAToken(s);
		}

	private:
		std::vector<TAIR> VTAIR;
		ReservedWord	reservedWord;

		void InitialReservedWord(){ 
			reservedWord
				.Add("begin",Def::_BEGIN)	.Add("call",Def::_CALL)	.Add("const",Def::_CONST)	.Add("do",Def::_DO)
				.Add("end",Def::_END)		.Add("if",Def::_IF)		.Add("odd",Def::_ODD)		.Add("procedure",Def::_PROC)
				.Add("read",Def::_READ)		.Add("then",Def::_THEN)	.Add("var",Def::_VAR)		.Add("while",Def::_WHILE)
				.Add("write",Def::_WRITE);		
		}

	};

};

#endif


