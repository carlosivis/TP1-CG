#include <stdio.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SFML/Audio.hpp>
//Compilation flags g++ pong.c -lGL -lGLU -lglut -lSOIL -lsfml-audio;
//Defines screen size 
#define sizex 500
#define sizey 500
//Game state flag, the mother of all "magic numbers". Changes game behaviour depending on the flag. 0 = Game, 1 = Pause , 2 = Main Menu , 3 = Credits , 4 = 4 player mode, 5 = Instructions screen
int gamestate = 2;
//Possible gamestates. Numbers are in order of implementation. Doesn't really matter so long as the numbers are different though.
#define Stage01 0
#define Stage02 4
#define Pause 1
#define Main_Menu 2
#define Credits 3
#define Instructions 5
#define Ball_Enter_Animation 6
//Stores the previous state of the game. Required in order to return from the pause menu.
int previousstate = 2;
//Background flag. Similar in function to the gamestate variable.
int backgroundstate = 1;
//Possible background states. Also in order of implementation.
#define Phantasm_Texture 0
#define Main_Menu_Texture 1
#define Credits_Texture 2
#define Instructions_Texture 3
#define Stage02_Texture 4
//Ball flag.
int ballstate = 0;
//Ball position
int ballx = 0;
int bally = 0;
//Ball speed vector
int ballspeedx = 1;
int ballspeedy = 1;
//Ball speed modifier, makes the ball faster
float ballspeedmod = 10;
//Ball rotation angle
float ballrotateangle = 0;
//Top player cursor position
int bar1x = 0;
int bar1y = 500;
//Bottom player cursor position
int bar2x = 0;
int bar2y = -490;
//Left player cursor position
int bar3x = -350;
int bar3y = 0;
//Right player cursor position
int bar4x = 340;
int bar4y = 0;
//Top player score
int score1 = 0;
//Bottom player score
int score2 = 0;
//Global keyboard vector
int keyboard [127];
//Texture IDs. I've no idea how to work with textures, so this is mostly copied from the teacher's example code. The texture loader is a carbon copy, in fact. Our characters have very simple animations, so each frame is saved individually. Wasteful, I know.
GLuint idRanBall;
GLuint idRanSlide;
GLuint idRanNeutral;
GLuint idRanStart;
GLuint idBackPhantasm;
GLuint idMenu01;
GLuint idCredits;
GLuint idInstructions;
GLuint idNumbers[10];
GLuint idCrown;
GLuint idCatBall;
GLuint idBackStage02;
// Sons
sf::Music barImpact;
sf::Music zoneImpact;
sf::Music stage01Song;
sf::Music stage02Song;

GLuint loadtexture (const char* file){
	GLuint texture = SOIL_load_OGL_texture(
                           file,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
	if (texture == 0)
		printf ("ERROR : '%s'\n",SOIL_last_result());
	return texture;
}

void setup(){
	glClearColor (0, 0, 0, 1);//Sets background colour to a solid black.
	glEnable (GL_BLEND);//Aparently enables the use of transparency.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0;i<127;i++)//Initializes all keyboard positions as 0s. Helps against garbage memory screwing up the program.
		keyboard[i]=0;
	//Where all textures are loaded. Is it supposed to be done like this ? God knows.
	//Menu elements
	idMenu01 = loadtexture("MenuPlaceholder.png");
	idCredits = loadtexture("CreditsPlaceholder.png");
	idInstructions = loadtexture("InstructionsPlaceholder.png");
	//Ball 1 : Yakumo Ran
	idRanStart = loadtexture("Sprites1.png");
	idRanSlide = loadtexture("Sprites2.png");
	idRanBall = loadtexture("Sprites3.png");
	idRanNeutral = loadtexture("Sprites4.png");

	//Ball 2: Cat
	idCatBall = loadtexture("carBall.png");

	//Stage Backgrounds
	idBackPhantasm = loadtexture("BackFinal1.png");
	idBackStage02 = loadtexture("back2.png");

	//Numbers
	idNumbers[0] = loadtexture("N0.png");
	idNumbers[1] = loadtexture("N1.png");
	idNumbers[2] = loadtexture("N2.png");
	idNumbers[3] = loadtexture("N3.png");
	idNumbers[4] = loadtexture("N4.png");
	idNumbers[5] = loadtexture("N5.png");
	idNumbers[6] = loadtexture("N6.png");
	idNumbers[7] = loadtexture("N7.png");
	idNumbers[8] = loadtexture("N8.png");
	idNumbers[9] = loadtexture("N9.png");
	//Winning player crown
	idCrown = loadtexture("Chen_Spread.png");
}

void drawbackground(){
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	switch (backgroundstate){ //Selects proper background texture depending on the gamemode
		case Phantasm_Texture : glBindTexture(GL_TEXTURE_2D,idBackPhantasm);break;
		case Main_Menu_Texture : glBindTexture(GL_TEXTURE_2D,idMenu01);break;
		case Credits_Texture : glBindTexture(GL_TEXTURE_2D,idCredits);break;
		case Instructions_Texture : glBindTexture(GL_TEXTURE_2D,idInstructions);break;
		case Stage02_Texture : glBindTexture(GL_TEXTURE_2D,idBackStage02);break;
	}
	glBegin(GL_POLYGON);
		glTexCoord2f(0,0);
		glVertex2f(-350,-500);
				
		glTexCoord2f(1,0);
		glVertex2f(350,-500);

		glTexCoord2f(1,1);
		glVertex2f(350,500);

		glTexCoord2f(0,1);
		glVertex2f(-350,500);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawbar(int x,int y){
	glColor3f (1,1,1);
	glBegin(GL_POLYGON);
		glVertex2f(x-70,y);
		glVertex2f(x-70,y-10);
		glVertex2f(x+70,y-10);
		glVertex2f(x+70,y);
	glEnd();
}

void drawbarvert(int x, int y){
	glColor3f (1,1,1);
	glBegin(GL_POLYGON);
		glVertex2f(x,y-70);
		glVertex2f(x+10,y-70);
		glVertex2f(x+10,y+70);
		glVertex2f(x,y+70);
	glEnd();
}

void drawscoreaux(){
	glBegin(GL_POLYGON);
		glTexCoord2f(0,0);
		glVertex2f(-20,-30);
		glTexCoord2f(1,0);
		glVertex2f(20,-30);
		glTexCoord2f(1,1);
		glVertex2f(20,30);
		glTexCoord2f(0,1);
		glVertex2f(-20,30);
	glEnd();
}

void drawscore(){
	glColor3f (1,1,1);

	if(score1>=10){
	glPushMatrix();
	glTranslatef(80,0,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, idNumbers[score1/10]);
	drawscoreaux();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();}

	glPushMatrix();
	glTranslatef(120,0,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, idNumbers[score1%10]);
	drawscoreaux();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-80,0,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, idNumbers[score2%10]);
	drawscoreaux();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	if(score2>=10){
	glPushMatrix();
	glTranslatef(-120,0,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, idNumbers[score2/10]);
	drawscoreaux();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();}

	if (score1>score2){
		glPushMatrix();
		glTranslatef(100,60,0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, idCrown);
		drawscoreaux();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

		if (score2>score1){
		glPushMatrix();
		glTranslatef(-100,60,0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, idCrown);
		drawscoreaux();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

}

void drawball(){
	glColor3f(1,1,1);
	if(gamestate==Ball_Enter_Animation){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, idRanSlide);
		glPushMatrix();
			glTranslatef(ballx,bally,0);
			glBegin(GL_POLYGON);
				glTexCoord2f(0,0);
				glVertex2f(-30,-30);
				
				glTexCoord2f(1,0);
				glVertex2f(30,-30);

				glTexCoord2f(1,1);
				glVertex2f(30,30);

				glTexCoord2f(0,1);
				glVertex2f(-30,30);
			glEnd();
			glPopMatrix();
	}
	if ((gamestate == Stage01)||(gamestate==Stage02)){//Draws the ball when the gamestate = 0 (Gameplay)
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, idRanBall);
		glPushMatrix();
			glTranslatef(ballx,bally,0);
			glRotatef(ballrotateangle,0,0,1);
			ballrotateangle = ballrotateangle + 30;
			if (ballrotateangle>360)
				ballrotateangle = ballrotateangle - 360;
			glBegin(GL_POLYGON);
				glTexCoord2f(0,0);
				glVertex2f(-30,-30);
				
				glTexCoord2f(1,0);
				glVertex2f(30,-30);

				glTexCoord2f(1,1);
				glVertex2f(30,30);

				glTexCoord2f(0,1);
				glVertex2f(-30,30);
			glEnd();
			glPopMatrix();
		}
	if ((gamestate == Stage01)||(gamestate==Stage02)||(gamestate==Pause)){//Same as above but for gamestate = 1 (Paused). The difference is that the ball's rotation speed isn't updated here, meaning the ball maintains its angle when the game is paused
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, idRanBall);
		glPushMatrix();
			glTranslatef(ballx,bally,0);
			glRotatef(ballrotateangle,0,0,1);
			glBegin(GL_POLYGON);
				glTexCoord2f(0,0);
				glVertex2f(-30,-30);
				
				glTexCoord2f(1,0);
				glVertex2f(30,-30);

				glTexCoord2f(1,1);
				glVertex2f(30,30);

				glTexCoord2f(0,1);
				glVertex2f(-30,30);
			glEnd();
			glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);
}

void resetball (){
	ballx = 0;
	bally = 0;
	ballspeedmod = 10;
}

void gameloop(int valor){
	//Menus
	if(keyboard[27]){
		switch(gamestate){
			case Stage02:{
				previousstate = gamestate;
				gamestate = 1;}
				break;
			case Stage01:{
				previousstate = gamestate;
				gamestate = 1;}//Changes gamestate to 1 (Paused state) when ESC is pressed
				break;
			case Pause:{
				gamestate = previousstate;
				if(previousstate==Stage01)
				    stage01Song.play();
				if(previousstate==Stage02)
				    stage02Song.play();
				}//Continues game when the game is paused and the ESC key is pressed
				break;
		}
		keyboard[27] = 0;//Setting ESC to 0 here ensures that the ESC key toggles the pause menu
	}	

	if ((gamestate==Pause)&&(keyboard[120]))//Exits game when the game is paused and the x key is pressed
		exit(0);

	if (gamestate==Pause)
		if(keyboard[100]){
            if(previousstate==Stage01)
                stage01Song.pause();
            if(previousstate==Stage02)
                stage02Song.pause();
			gamestate = Main_Menu;
			resetball();
			score1 = 0;
			score2 = score1;
			backgroundstate = Main_Menu_Texture;
		}

	if (gamestate==Ball_Enter_Animation){
		ballx = ballx + 10;
		if (ballx>0)
			gamestate = Stage01;
	}

	if (gamestate==Main_Menu){
		drawbackground();
		if (keyboard[122]){//Key that plays the game
			ballx = -400;
			gamestate = Ball_Enter_Animation;
			backgroundstate = Phantasm_Texture;}
		if (keyboard[120]){//Key that leads to the credits screen.
			gamestate = Credits;
			backgroundstate = Credits_Texture;}
		if (keyboard[105]){//Key that leads to the instructions screen.
			gamestate = Instructions;
			backgroundstate = Instructions_Texture;
		}
		if (keyboard[99])//Key to exit from the menu.
			exit(0);
	}

	if (gamestate==Credits)
		if (keyboard[122]){
			gamestate = Main_Menu;
			backgroundstate = Main_Menu_Texture;
			keyboard[122]=0;
		}

	if (gamestate==Instructions)
		if (keyboard[122]){
			gamestate = Main_Menu;
			backgroundstate = Main_Menu_Texture;
			keyboard[122]=0;
		}

	//Game logic
	if ((gamestate==Stage01)||(gamestate==Stage02)){
		if (keyboard[111]==1)//Moves the player's cursors
			bar1x = bar1x -10;
		if (keyboard[112]==1)
			bar1x = bar1x +10;
		if (keyboard[113]==1)
			bar2x = bar2x -10;
		if (keyboard[119]==1)
			bar2x = bar2x +10;

		if (gamestate==Stage02){
			if (keyboard[109]==1)//Moves the player's cursors
				bar4y = bar4y -20;
			if (keyboard[107]==1)
				bar4y = bar4y +20;
			if (keyboard[122]==1)
				bar3y = bar3y -20;
			if (keyboard[97]==1)
				bar3y = bar3y +20;
		}
		
		ballx = ballx + ballspeedx*ballspeedmod;//Moves the ball in the x axis
		bally = bally + ballspeedy*ballspeedmod;//Moves the ball in the y axis
		//All logic below here is based off of them juicy magical numbers because it was made on prototype phase. Alas, I am too lazy to change the code so please don't change the window size.
		if (gamestate == Stage01){
			if (ballx>sizex-180){//Reflects the ball off the sides
				ballspeedx = -1;
				zoneImpact.play();
			}
			if (ballx<-sizex+180){
				ballspeedx = 1;
				zoneImpact.play();
			}
		}



		if(bar1x<-280)//Limits the movement of the player's cursors to stay within the screen
			bar1x=-280;
		if(bar1x>280)
			bar1x=280;
		if(bar2x<-280)
			bar2x=-280;
		if(bar2x>280)
			bar2x=280;

		if(bar3y>430)
			bar3y = 430;
		if(bar3y<-430)
			bar3y = -430;
		if(bar4y>430)
			bar4y = 430;
		if(bar4y<-430)
			bar4y = -430;

		if(ballspeedy==1){
			if((bally>=480)&&(bally<=500)&&((ballx>bar1x-90)&&(ballx<bar1x+90))){//Reflects the ball if it hits the top bar
				ballspeedy = -1;
				barImpact.play();
				ballspeedmod = ballspeedmod*1.05;//Accelerates the ball whenever it reflects off the racket by 5%
			}
		}
		else{
			if((bally<=-480)&&(bally>=-500)&&((ballx>bar2x-90)&&(ballx<bar2x+90))){//Same as above but for the bottom bar
				ballspeedy = 1;
				barImpact.play();
				ballspeedmod = ballspeedmod*1.05;
			}
		}

		if(gamestate==Stage02){
			if (ballspeedx==1){
				if((ballx>=340)&&(ballx<=350)&&((bally>bar4y-90)&&(bally<bar4y+90))){
					ballspeedx = -1;
					barImpact.play();
					ballspeedmod = ballspeedmod*1.05;
				}
			}
			else{
				if((ballx<=-340)&&(ballx>=-350)&&((bally>bar3y-90)&&(bally<bar3y+90))){
					ballspeedx = 1;
					barImpact.play();
					ballspeedmod = ballspeedmod*1.05;
				}
			}

		}

		if (bally>500){//Increases the score
			score2++;
			resetball();
		}
		if (bally<-500){
			score1++;
			resetball();
		}
		if (gamestate==Stage02){
			if(ballx>370){
				score2++;
				resetball();
			}
			if(ballx<-370){
				score1++;
				resetball();
			}
		}
		if((gamestate==Stage01)&&((score1>=12)||(score2>=12))){//Advances from stage 1 to 2
			gamestate = Stage02;
			stage01Song.stop();
			stage02Song.play();
			score1 = 0;
			score2 = score1;
			resetball();
		}
		}	
	glutPostRedisplay();
	glutTimerFunc(33,gameloop,0);
}

void escfunc (){
	drawbackground();
	drawscore();
	drawball();
	glColor4f(1,1,1,0.7);
	glBegin(GL_POLYGON);
		glVertex2f(-200,-200);
		glVertex2f(200,-200);
		glVertex2f(200,200);
		glVertex2f(-200,200);
	glEnd();
	drawbar(bar1x,bar1y);
	drawbar(bar2x,bar2y);
		if(previousstate==4){
			drawbarvert(bar3x,bar3y);
			drawbarvert(bar4x,bar4y);
		}
}

void drawscene(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,1,1);

	switch (gamestate){//Detects the game mode and draws accordingly.
		case Pause : {
			escfunc();
			stage01Song.pause();
			stage02Song.pause();
		}
			break;
		case Stage01 :{
			drawbackground();
			drawscore();
			drawball();
			drawbar(bar1x,bar1y);
			drawbar(bar2x,bar2y);
		}break;
		case Main_Menu : drawbackground();break;
		case Credits : drawbackground();break;
		case Instructions : drawbackground();break;
		case Stage02 : {
			drawbackground();
			drawscore();
			drawball();
			drawbar(bar1x,bar1y);
			drawbar(bar2x,bar2y);
			drawbarvert(bar3x,bar3y);
			drawbarvert(bar4x,bar4y);
		}break;
		case Ball_Enter_Animation : {
			drawbackground();
			drawscore();
			drawball();
			drawbar(bar1x,bar1y);
			drawbar(bar2x,bar2y);
		}break;
	}

	glFlush();
}


void keypress (unsigned char key, int x, int y){
	keyboard[key] = 1;
}

void keyrelease (unsigned char key, int x, int y){
	keyboard[key] = 0;
}

void redimensionada(int width, int height) {
   glViewport(0, 0, width, height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-sizex+150, sizex-150, -sizey, sizey, -1, 1);
   //Sets up the size of the screen. This makes the screen 300 pixels taller than it is wide.
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char** argv) {
    if(!stage02Song.openFromFile("nyan.wav")){
        printf("Erro\n");
    }
	if(!stage01Song.openFromFile("Foxy.wav")){
		printf("Erro\n");
	}
	if(!barImpact.openFromFile("impactBars.wav")){
		printf("Erro\n");
	}
	if(!zoneImpact.openFromFile("impactZone.wav")){
		printf("Erro\n");
	}
	stage01Song.play();
   glutInit(&argc, argv);
   glutInitContextVersion(1, 1);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   //Stuff not worth looking at (yet)
   glutInitWindowSize(700, 1000);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Pong Prototype");
   //Self explanatory names
   glutDisplayFunc(drawscene);
   glutKeyboardFunc(keypress);
   glutKeyboardUpFunc(keyrelease);
   glutReshapeFunc(redimensionada);
   //No idea how reshaping works so I'll just copy the teacher's code
   setup();
   glutTimerFunc(33,gameloop,0);
   glutMainLoop();
   //The main loop proper, receives no parameters (I think)
   }
