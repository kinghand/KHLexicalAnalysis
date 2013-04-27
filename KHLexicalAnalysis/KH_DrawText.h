#pragma once 
#ifndef _KH_DRAW_TEXT
#define _KH_DRAW_TEXT

#define _AFXDLL

#include <stdlib.h>
#include <string>
#include <afxwin.h>
#include <Windows.h>
#include "GdiPlus.h"
#include "GdiplusBase.h"
#include "GdiPlusPath.h"
#include "KH_Draw_color.h"

namespace KH 
{
	const int _DRAW_MAX_CHAR = 128 ;

	class TextDrawer{
	public:
		TextDrawer();
		void RasterPos(const int & X, const int & Y);	//�趨λ��
		void SelectFont(int Size, int, const char* FontName);		//�趨����
		void SelectFont(int size, std::string FontName);
		void SetColor(float R, float G, float B);
		void SetColor(Draw_color::Colorf c);
		void Out(const char *);					//����ַ���
		void DrawEngString(const char*);		//ֻ���Ӣ��
		int GetTextLength(std::string str){
			wchar_t w;
			HDC hDC = wglGetCurrentDC();
			// ת��Ϊ���ַ�
			int count = 0;
			if(!str.empty()){
				std::string::iterator i = str.begin();
				for(;i!=str.end();i++){
					MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, &(*i), 2, &w, 1);
					int width;
					GetCharWidth32W(hDC, w, w, &width);    // ȡ���ַ��Ŀ��
					count += width;
				}
			}
			return count;
		}

		friend TextDrawer& operator<<(TextDrawer &, const char &);
		friend TextDrawer& operator<<(TextDrawer &, const std::string &);
		friend TextDrawer& operator<<(TextDrawer &, const int &);
		friend TextDrawer& operator<<(TextDrawer &, const float &);
		friend TextDrawer& operator<<(TextDrawer &, const double &);

	private:
		TextDrawer(const TextDrawer &);
		TextDrawer & operator =(const TextDrawer &);
		std::string fontName;
		int  fontSize;
	};	
};

#endif