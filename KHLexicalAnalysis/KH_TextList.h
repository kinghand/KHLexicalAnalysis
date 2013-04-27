#pragma once
#ifndef _KH_DRAW_TEXT_LIST
#define _KH_DRAW_TEXT_LIST

#include<string>
#include<list>

namespace KH 
{
	class TextList
	{
	public:
		typedef std::list<std::list<std::string>> LLV;
		typedef std::list<std::string> LV;
		typedef std::string ValueType;
		typedef LLV::iterator iterator;

		TextList(void);
		~TextList(void);

		//Set;
		void Initial(std::string sBegin = "",std::string sEnd = "");
		void AddLine();
		void InsertString(std::string str);
		void InsertLine(std::string str = "");
		void InsertChar(char c);
		//Erase
		void EraseString() { 
			if(posCloumn != posLine->begin() && posCloumn != posLine->end() && !posCloumn->empty()){
				LV::iterator it = posCloumn;
				it ++;
				posLine->erase(posCloumn); 
				if(it == posLine->end())it --;
				posCloumn = it;
			}
			else
			CheckLine();
		}
		//Get iterator;
		iterator now() const;
		iterator begin(){ return Content.begin(); }
		iterator end(){ return Content.end(); }
		void handleMoveUp() { 
			if(posLine != Content.begin()){
				posLine --; 
				ResetCloumn();
			}
		}
		void handleMoveDown() { 
			iterator itl = posLine; itl ++;
			if(itl != Content.end()) posLine ++; 
			ResetCloumn();
		}
		//GetString;
		std::string GetString();
		std::string GetString(iterator i);
		LV GetStringLine();
		LV GetStringLine(iterator i);
		std::string strConsole;

		
	private:
		LLV		Content;
		LLV::iterator	posLine;
		LV::iterator	posCloumn;
		ValueType::size_type	nPosInner;
		//
		void CheckLine(){
			if(Content.size() > 1 && posLine->size() <= 1 && posLine != Content.begin()) {
				iterator i = posLine;
				i++;
				Content.erase(posLine);
				i--;
				posLine = i;

			}
			ResetCloumn();
		}

		void ResetCloumn() {
			LV::iterator it = posLine->end();
			it --;
			posCloumn = it; 
		}
	};

};

#endif

