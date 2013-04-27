#include "KH_TextList.h"
#include <iostream>

KH::TextList::TextList(void)
{
	Initial();
}

KH::TextList::~TextList(void)
{
}

void KH::TextList::Initial(std::string sBegin, std::string sEnd){
	LV lv;
	if(!sBegin.empty())lv.push_front(sBegin);
	lv.push_back(sEnd);
	Content.push_back(lv);
	posLine = Content.begin();
	posCloumn = posLine->begin();
	nPosInner = 0;
}

void KH::TextList::AddLine(){
	KH::TextList::LV lv;
	lv.push_back("");
	posLine ++ ;
	Content.insert(posLine,lv);
	posLine --;
	posCloumn = posLine->begin();
}

void KH::TextList::InsertString(std::string str){
	if(str.empty()) return;
	posCloumn++;
	posLine->insert(posCloumn, str);
	posCloumn--;
}
void KH::TextList::InsertLine(std::string str){
	AddLine();
	InsertString(str);
}

void KH::TextList::InsertChar(char c){
	posCloumn->insert(nPosInner,1,c);
	std::string s = *posCloumn;
	std::cout << nPosInner++;
}

KH::TextList::iterator KH::TextList::now() const{
	return posLine;
}

std::string KH::TextList::GetString(){
	return GetString(posLine);
}

std::string KH::TextList::GetString(iterator i){
	if(i != end());
	LV::iterator it = i->begin();
	std::string s = "";
	for(;it != i->end(); it++ ) {
		s += *it;
	}
	return s;
}

KH::TextList::LV KH::TextList::GetStringLine(){
	return GetStringLine(posLine);
}

KH::TextList::LV KH::TextList::GetStringLine(iterator i){
	return LV(*i);
}