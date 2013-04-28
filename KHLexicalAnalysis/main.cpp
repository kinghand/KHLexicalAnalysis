#include<gl/glut.h>
//#include <Windows.h>
#include <fstream>

#include "KH_DrawText.h"
#include "KH_TextList.h"
#include "DFAM_Range.h"
#include "LA_View.h"
#include "GrammarAnalysis.h"

GLvoid init(GLvoid)
{
}

GLint Height		(0);
GLint Width			(0);
GLint LineHeight	(0x10);
GLint LineNum		(0);
GLint FontSize		(0x10);
GLint CenterHeight  (0);
GLint SignWidth		(0x0C);
char  SignCha		('>');

void ResetHeight(GLint H) { Height = H; LineNum = Height / LineHeight; CenterHeight =(LineHeight * (LineNum/2)) ;}


std::string MainString = "Kinghand: a";

KH::LA_View lav;
KH::TextList tl;
float r = .75,g = 0.99 ,b= 0.8;
int line = 1;

void display(void)
{
	

	int length;
	lav.GetDrawer().SelectFont(FontSize,100,"Menlo");
	
	glClearColor(.0,.133,.251,0);
	//glClearColor(1,1,1,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	
	lav.GetDrawer().SetColor(1.0, 1.0, 1.0);
	lav.ShowLine(tl.strConsole,5,5);

	lav.ShowLine(":" + lav.GetLineTokenType(tl.strConsole), Width - 150, 5, KH::LA_View::Colorf(1,1,1));

	lav.GetDrawer().SetColor(1,g,1);
	lav.GetDrawer().RasterPos( 0 , Height - CenterHeight );
	lav.GetDrawer()<<SignCha;
	length = lav.ShowLine(tl.GetString(),SignWidth , Height - CenterHeight ,KH::LA_View::Colorf(0.3,0.4,0.3)); //
	lav.GetDrawer().SetColor(1,1,1);
	lav.GetDrawer().RasterPos(SignWidth + length , Height - CenterHeight);
	lav.GetDrawer()<<"|";

	system("cls");
	std::cout << std::endl << "##########################################################" << std::endl;
	//int i = 8;
	//printf("%d %d %d %d %d %d ;\n",++i,i++,--i,i--,i++,i--);
	// i = 7;
	//  
	//printf("%d %d %d %d %d %d ;\n",++i,i++,--i,i--,i++,i--);
	

	KH::TextList::iterator itUp = tl.now();
	for(int i = 1; i * LineHeight < CenterHeight; i++){
		if(itUp == tl.begin())break;
		else itUp--;
		lav.ShowLine(tl.GetString(itUp),SignWidth , Height - CenterHeight + i * LineHeight ,KH::LA_View::Colorf(-i*0.005,-i*0.001,-i*0.002));
	}
	itUp = tl.now();
	for(int i = 1; i * LineHeight < CenterHeight; i++){
		if(itUp == tl.end())break;
		else itUp++;
		if(itUp == tl.end())break;

		lav.ShowLine(tl.GetString(itUp),SignWidth , Height - CenterHeight - i * LineHeight ,KH::LA_View::Colorf(-i*0.005,-i*0.001,-i*0.002));
	}

	/*glLoadIdentity();
	glBegin(GL_TRIANGLES);
          glVertex3f(-0.5,-0.5,0.0);
          glVertex3f(0.5,0.0,0.0);
          glVertex3f(0.0,0.5,0.0);
    glEnd();*/

	glFlush();

	
	glutSwapBuffers();
}

void reshape(const int width, const int height)
{
	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	ResetHeight(height);
	Width = width;

	
}

void processSpecialKeys(int key,int x,int y)
{
    switch(key){
	case GLUT_KEY_F1             :break;
	case GLUT_KEY_F2             :break;
	case GLUT_KEY_F3             :break;
	case GLUT_KEY_F4             :break;
	case GLUT_KEY_F5             :break;
	case GLUT_KEY_F6             :break;
	case GLUT_KEY_F7             :break;
	case GLUT_KEY_F8             :break;
	case GLUT_KEY_F9             :break;
	case GLUT_KEY_F10            :break;
	case GLUT_KEY_F11            :break;  
	case GLUT_KEY_F12            :break;  
	case GLUT_KEY_LEFT           :break;  
	case GLUT_KEY_RIGHT          :break; 
	case GLUT_KEY_UP             :
		tl.handleMoveUp();
		break; 
	case GLUT_KEY_DOWN           :
		tl.handleMoveDown();
		break; 
	case GLUT_KEY_PAGE_UP        :
		LineHeight += 1;
		FontSize += 1;
		break; 
	case GLUT_KEY_PAGE_DOWN      :
		LineHeight -= 1;
		FontSize -= 1;
		break; 
	case GLUT_KEY_HOME           :break; 
	case GLUT_KEY_END            :break; 
	case GLUT_KEY_INSERT         :break; 
	default:break;
	}
	glutPostRedisplay();
} 

void processNormalKeys(unsigned char key,int x,int y) 
{ 
	switch(key){
	case 8: //backspace
		if(tl.strConsole.empty()) {
			tl.EraseString();
		}
		else
			tl.strConsole.pop_back();
		break;
	case 32: //space
		tl.InsertString(tl.strConsole + " ") ; tl.strConsole = "";   break;
	case 13: //enter
		if(!tl.strConsole.empty())tl.InsertString(tl.strConsole + " ");
		/*else tl.InsertString(tl.strConsole) ;*/
		tl.InsertLine() ;
		tl.strConsole = "";
		break;
	case 27: //esc
		exit(0);break;
	default: tl.strConsole += key;
	}
	
	glutPostRedisplay();
} 

int main(int argc, char *argv[])
{
	//File input
	std::ifstream fin("LanguageTest.txt",std::ios::in);
	std::string strf;
	char buff[256];
	while(!fin.eof() ) {
		fin.getline(buff,256,'\n');
		strf = std::string(buff);
		tl.InsertLine(strf);
	}

	/*tl.InsertString("Hello ");
	tl.InsertLine("This is a test program .");
	tl.InsertLine("an ");
	tl.InsertString("Editor! ");
	tl.InsertLine("Number: ");
	tl.InsertString("int ");
	tl.InsertString("123 ");
	tl.InsertString("float ");
	tl.InsertString("-1233.23 ");
	tl.InsertLine("Have ");
	tl.InsertString("Fun ");
	tl.InsertString(" \"^_^\" ");*/

	KH::DFAM_Range_Initial();

	lav.GetLA() 
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_BLOCK,		KH::dBlock)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_IDENTIFIER,	KH::dIdentifier)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_STRING,		KH::dString)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_ANNOTATION,	KH::dAnnotation)

		.AddTair(KH::LexicalAnalysis::TYPEDEF::_MINUS,	KH::dMinus)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_PLUS,	KH::dPlus)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_TIMES,	KH::dTimes)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_SLASH,	KH::dSlash)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_PERIOD,	KH::dPeriod)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_COMMA,	KH::dComma)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_COLON,	KH::dColon)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_SEMICOLON,	KH::dSemiColon)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_LPAREN,	KH::dLParen)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_RPAREN,	KH::dRParen)
		
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_QUOTE,	KH::dQuote)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_EQUAL,	KH::dEqual)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_IDENCITAL,	KH::dIdencital)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_ASSIGN,	KH::dAssign)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_ASSIGN,	KH::dAssignNorm)

		.AddTair(KH::LexicalAnalysis::TYPEDEF::_INT,	KH::dInt)
		.AddTair(KH::LexicalAnalysis::TYPEDEF::_FLOAT,	KH::dFloat)


		//.AddTair(KH::LexicalAnalysis::TYPEDEF::_AND,dASD)
				;

	

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow(argv[0]);
	
	//init();
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

//void renderScene(void)
//{
//          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//          glLoadIdentity();
//          glBegin(GL_TRIANGLES);
//          glVertex3f(-0.5,-0.5,0.0);
//          glVertex3f(0.5,0.0,0.0);
//          glVertex3f(0.0,0.5,0.0);
//          glEnd();
//          glutSwapBuffers();
//}
//
// 
//int main(int argc, char* argv[])
//{
//          glutInit(&argc, (char**) argv);
//          glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
//          glutInitWindowPosition(100,100);
//          glutInitWindowSize(800,600);
//          glutCreateWindow("Hello OpenGL");
//          //glutDisplayFunc(renderScene);
//		  glutDisplayFunc(display);
//		  //glutReshapeFunc(reshape);
//
//          glutMainLoop();//enters the GLUT event processing loop.
//          return 0;
//}