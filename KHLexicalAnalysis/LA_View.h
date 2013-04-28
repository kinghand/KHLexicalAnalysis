#pragma once
#ifndef _KH_LA_VIEW
#define _KH_LA_VIEW

#define _VIEW_DEBUG true
#define _VIEW_SHOWDETAIL false

#include"DFAMachine.h"
#include"LexicalAnalysis.h"
#include"KH_Draw_color.h"
#include"KH_DrawText.h"
#include"KH_TextList.h"

#include<map>
#include<string>
#include"iostream"
#include"iomanip" //数据用

namespace KH{

	class LA_View{
	public:
		typedef KH::Draw_color::Colorf Colorf;
		typedef KH::Draw_color::Map Map;

		LA_View() {	
			InitialColorDef();
		}

		int ShowLine(std::string str, int x, int y, Colorf offset){ //着色并显示一行
			LexicalAnalysis::Token tk;
			int length = 0;
			while(!str.empty()) {
				tk = la.GetAToken(str);
				Colorf c = colorDef.GetTypeColor(tk.GetSign()) ;
				if(!tk.IsDefined()) {
					td.SetColor(colorDef.GetOrigin());
					td.RasterPos( x + length , y );
					td<<tk.GetValue();
					//std::cout<<tk.GetValue();
					length += td.GetTextLength(tk.GetValue());
				}
				else {
					td.SetColor(c + offset);
					td.RasterPos( x + length , y );
					td<<tk.GetValue();

					if(tk.GetSign() != LexicalAnalysis::TYPEDEF::_BLOCK && _VIEW_SHOWDETAIL) //控制台调试
						std::cout<<std::setw(30)<<tk.GetValue()<<std::setw(15)<< LexicalAnalysis::TYPEDEF::GetTypeString(tk.GetSign())<<std::endl;

					length += td.GetTextLength(tk.GetValue()); //画刷右移
					//std::cout<<tk.GetValue()<<' ';
					/*td.RasterPos( 100 + length , y );
					td<<LexicalAnalysis::TYPEDEF::GetTypeString(tk.GetSign());*/
					
				}
			}
			return length;
		}

		std::string GetLineTokenType(std::string str, bool needFirst = false){
			LexicalAnalysis::Token tk;
			int length = 0;
			while(!str.empty()) {
				tk = la.GetAToken(str);
				if(needFirst)return LexicalAnalysis::TYPEDEF::GetTypeString(tk.GetSign());
			}
			return LexicalAnalysis::TYPEDEF::GetTypeString(tk.GetSign());
		}

		void ShowLine(std::string str, int x, int y){
			ShowLine(str, x, y, Colorf(0,0,0));
		}

		KH::TextDrawer& GetDrawer() {return td; }
		LexicalAnalysis& GetLA() {return la; }

	private:
		KH::Draw_color	colorDef;
		LexicalAnalysis	la;
		KH::TextDrawer	td;
		

		void InitialColorDef(){ 
			colorDef
				.AddColor(_MARK_SYMBOL_BEGIN	, _MARK_SYMBOL_END,	158, 255, 255)
				.AddColor(_IDENTIFIER	,	 255,		238,	128)
				.AddColor(_INT			,	1,	.8,	0)
				.AddColor(_FLOAT		,	1,	.8,	.3) 
				.AddColor(_PERIOD	,	255 ,157,	0)
				.AddColor(_ASSIGN		,	255 ,	157,	0)
				.AddColor(_ANNOTATION	,	0,	136,	255)
				//.AddColor(_COMMA	,	.95 ,	.9 ,	.8)
				//.AddColor(_SEMICOLON	,	 .5	,	.5,		.5)
				//.AddColor(_BLOCK		,	 .5	,	.5,		.5)
				.AddColor(_STRING		,	 58 ,	217	 ,	0)
				.AddColor(LA_RWordDef::_MARK_RESERVEDWORD_BEGIN , LA_RWordDef::_MARK_RESERVEDWORD_END , 1, 0.36, 0.77);
				;
		}

	};

};


#endif