#include "KH_DrawText.h"
#include <sstream>
#include <gl/glut.h>


KH::TextDrawer::TextDrawer()
{}

void KH::TextDrawer::SetColor(float R, float G, float B)
{
	glColor3f(R, G, B);
}

void KH::TextDrawer::SetColor(Draw_color::Colorf c)
{
	glColor3f(c.r, c.g, c.b);
}

void KH::TextDrawer::RasterPos(const int & x, const int & y)
{
	
	glRasterPos2i(x, y);
	
	
}

void KH::TextDrawer::SelectFont(int size, int charset, const char* face = "Arial") 
{ 
	fontName = std::string(face);
	fontSize = size;
	//����ָ������ 
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0, charset, 
							OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
							DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face
							); 
	//�Ѵ���������ѡ���豸������,���������     
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont); 
}

void KH::TextDrawer::DrawEngString(const char* str) {     
	static int isFirstCall = 1;     
	static GLuint lists;     
	if( isFirstCall ) { 
		// ����ǵ�һ�ε��ã�ִ�г�ʼ��         
		// Ϊÿһ��ASCII�ַ�����һ����ʾ�б�         
		isFirstCall = 0;         // ����MAX_CHAR����������ʾ�б���         
		lists = glGenLists(_DRAW_MAX_CHAR);         // ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���         
		wglUseFontBitmaps(wglGetCurrentDC(), 0, _DRAW_MAX_CHAR, lists);     
	}     // ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�     
	for(; *str!='\0'; ++str)        
		glCallList(lists + *str);
} 

void KH::TextDrawer::Out(const char * str)
{ 
	//DrawEngString(str);return;

	int len=0, i=0; wchar_t * wstring; 
	HDC hDC=wglGetCurrentDC(); 
	GLuint list=glGenLists( 1 ); //�����ַ��ĸ��� 
								// �����˫�ֽ��ַ��ģ����������ַ����������ֽڲ���һ���ַ�     
								// ����һ���ֽ���һ���ַ� 
	for( i=0; str[i]!='\0'; i++ ) {    //�ж��ַ�str[i]�Ƿ��������ֽ����    
		if( IsDBCSLeadByte( str[i] ))     
			i++;    
		len++; 
	} //������ַ�ת��Ϊ���ַ� 
	wstring=(wchar_t*)malloc((len+1)*sizeof(wchar_t)); 
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len ); 
	wstring[len]=L'\0'; //�������ַ� 
	for( i=0; i<len; i++ ) {    
		wglUseFontBitmapsW( hDC, wstring[i], 1, list );    
		glCallList( list ); 
	} //��Դ���� 
	free( wstring ); 
	glDeleteLists( list, 1 ); 
}

KH::TextDrawer & KH::operator<<(KH::TextDrawer & glos, const char & ch)
{
	std::string str;
	str.push_back(ch);
	glos.Out(str.c_str());
	return glos;
}
KH::TextDrawer & KH::operator<<(KH::TextDrawer & glos, const std::string & str)
{
	glos.Out(str.c_str());
	return glos;
}
KH::TextDrawer & KH::operator<<(KH::TextDrawer & glos, const int & d)
{
	std::ostringstream ostr;
	ostr << d;
	glos << ostr.str();
	return glos;
}
KH::TextDrawer & KH::operator<<(KH::TextDrawer & glos, const float & f)
{
	std::ostringstream ostr;
	ostr << f;
	glos << ostr.str();
	return glos;
}

KH::TextDrawer & KH::operator<<(KH::TextDrawer & glos, const double & lf)
{
	std::ostringstream ostr;
	ostr << lf;
	glos << ostr.str();
	return glos;
}


