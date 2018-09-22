#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>
#include<GL/glut.h>

double cameraAngle;
double move_X, move_Y, move_Z;
int canDrawGrid, canDrawAxis;
double planex=-2000, planey=0, planez=275,tspeed=0.0;
double cameraRadius, cameraHeight, cameraAngleDelta;
int num_texture = -1;
int count = 0,left=0,right=0;
GLuint grassimg, wallimg, ballimg, flwimg, flwimg2,soilimg;
double speed,rotateplane,frontLeft,backleft1,backleft2,noseleft,frontright, backright1, backright2, noseright;
int LoadBitmapImage(char *filename)
{
	int i, j = 0;
	FILE *l_file;
	unsigned char *l_texture;

	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	RGBTRIPLE rgb;

	num_texture++;

	if ((l_file = fopen(filename, "rb")) == NULL) return (-1);

	fread(&fileheader, sizeof(fileheader), 1, l_file);

	fseek(l_file, sizeof(fileheader), SEEK_SET);
	fread(&infoheader, sizeof(infoheader), 1, l_file);

	l_texture = (byte *)malloc(infoheader.biWidth * infoheader.biHeight * 4);
	memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);
	for (i = 0; i < infoheader.biWidth*infoheader.biHeight; i++)
	{
		fread(&rgb, sizeof(rgb), 1, l_file);

		l_texture[j + 0] = rgb.rgbtRed;
		l_texture[j + 1] = rgb.rgbtGreen;
		l_texture[j + 2] = rgb.rgbtBlue;
		l_texture[j + 3] = 255;
		j += 4;
	}
	fclose(l_file);

	glBindTexture(GL_TEXTURE_2D, num_texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

	free(l_texture);

	return (num_texture);

}
int drive,create,m=0,a=0,mnland=0,mnupper=0;
int lw1 = 0, lw2 = 0, lw3 = 0, takeoff = 0, takef = 0;
void loadImage()
{
	grassimg = LoadBitmapImage("image/grass.bmp");
	wallimg = LoadBitmapImage("image/runway.bmp");
	flwimg = LoadBitmapImage("image/flowers.bmp");
	soilimg = LoadBitmapImage("image/soil.bmp");
	ballimg = LoadBitmapImage("image/new2.bmp");
	if ( wallimg!=-1)
		printf("Load successful!!\n");
	else printf("Image loading Failed!!\n");
}
int land,text=0;
void drawBitmapText(char *string, float x, float y, float z)
{
	char *c;
	glRasterPos3f(x, y, z);

	for (c = string; *c != '\0'; c++)
	{

		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
}

void render(void)
{
	glColor3f(1, 0, 0);
	drawBitmapText("PRESS l to land ", -150, 0, 0);

}

void render1(void)
{
	glColor3f(1, 0, 0);
	drawBitmapText("Plane CRASHED!!!!!!!! ", -150, 0, 0);

}

void render2(void)
{
	glColor3f(0, 1, 0);
	drawBitmapText("Landing SUCCESSFUL!!Press S ", -150, 0, 0);

}

void render3(void)
{
	glColor3f(0, 1, 0);
	drawBitmapText("Take Off SUCCESSFUL ", -150, 0, 0);

}
void drawGridAndAxes() {

	// draw the three major AXES

	glBegin(GL_LINES);
	//X axis
	glColor3f(0, 1, 0);	//100% Green
	glVertex3f(-150, 0, 0);
	glVertex3f(150, 0, 0);

	//Y axis
	glColor3f(0, 0, 1);	//100% Blue
	glVertex3f(0, -150, 0);	// intentionally extended to -150 to 150, no big deal
	glVertex3f(0, 150, 0);

	//Z axis
	glColor3f(1, 1, 1);	//100% White
	glVertex3f(0, 0, -150);
	glVertex3f(0, 0, 150);
	glEnd();

	//some gridlines along the field
	int i;

	glColor3f(0.5, 0.5, 0.5);	//grey
	glBegin(GL_LINES);
	for (i = -10; i <= 10; i++) {

		if (i == 0)
			continue;	//SKIP the MAIN axes

						//lines parallel to Y-axis
		glVertex3f(i * 10, -100, 0);
		glVertex3f(i * 10, 100, 0);

		//lines parallel to X-axis
		glVertex3f(-100, i * 10, 0);
		glVertex3f(100, i * 10, 0);
	}
	glEnd();

}
double frontwheelval = 0;
void drawWheel(int x) {
	glPushMatrix();
//	glTranslatef(0, -25, 0);
	glPushMatrix();

	//glColor3f(0.37,.37,.37);
	glColor3f(0, 0, 0);
	glTranslatef(x, 0, 8.5);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glRotatef(-90, 0, 1, 0);
	glRotatef(tspeed, 0, 0, 1);

	glutSolidTorus(2, 6, 20, 20);
	glPopMatrix();
	glPopMatrix();




	glPushMatrix();

	glColor3f(.37, .37, .37);
	glTranslatef(x, 0, 8.5);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glRotatef(-90, 0, 1, 0);
	glRotatef(tspeed, 0, 0, 1);

	glutWireTorus(2, 2, 20, 20);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
void newWheel(double back) {
	double x;

	if (takeoff == 1) {
		double x;
		double y;
		double z;
		if (back == 0) {
			glPushMatrix();
			frontwheelval = planex + 52;
			x = planey + 40;
			if (takef == 0) {
				y = planex - 450;
				frontLeft = planex - 450;
				z = 0;
			}
			else {
				y = frontLeft;
				z = planez + 1;
			}
			if (y <= -612 && z<=2)
				lw1 = 1;
			glTranslatef(x,y,z);
			glPushMatrix();
			glTranslatef(0, 0, 8.5);
			glPushMatrix();
			glColor3f(.87, .87, .87);
			GLUquadricObj *cyl1 = gluNewQuadric();
			gluQuadricTexture(cyl1, GL_TRUE);
			gluCylinder(cyl1, 1, 1, 16, 20, 20);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glColor3f(.87, .87, .87);
			glScalef(6.5, 1, 1);
			glutSolidCube(1);
			glEnd();
			glPopMatrix();

			glPopMatrix();
			drawWheel(5);
			drawWheel(-5);
			glPopMatrix();
		}
		else if (back == 1) {
			glPushMatrix();
			x = planey + 18;
			y = planex - 385;

			if (takef == 0) {
				y = planex - 385;
				backleft1 = planex - 385;
				z = 0;
			}
			else {
				y = backleft1;
				z = planez + 1;

			}
			if (y <= -612 && z <= 2)
				lw2 = 1;
			glTranslatef(x,y,z);
			glPushMatrix();
			glTranslatef(0, 0, 8.5);
			glPushMatrix();
			glColor3f(.87, .87, .87);
			GLUquadricObj *cyl1 = gluNewQuadric();
			gluQuadricTexture(cyl1, GL_TRUE);
			gluCylinder(cyl1, 1, 1, 25, 20, 20);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glColor3f(.87, .87, .87);
			glScalef(6.5, 1, 1);
			glutSolidCube(1);
			glEnd();
			glPopMatrix();

			glPopMatrix();

			drawWheel(5);
			drawWheel(-5);
			glPopMatrix();
		}

		else {
			glPushMatrix();
			x = planey + 62;
			y = planex - 385;
			if (takef == 0) {
				y = planex - 385;
				backleft2 = planex - 385;
				z = 0;
			}
			else {
				y = backleft2;
				z = planez + 1;

			}
			if (y <= -612 && z <= 2)
				lw3 = 1;

			glTranslatef(x, y, z);
			glPushMatrix();
			glTranslatef(0, 0, 8.5);
			glPushMatrix();
			glColor3f(.87, .87, .87);
			GLUquadricObj *cyl1 = gluNewQuadric();
			gluQuadricTexture(cyl1, GL_TRUE);
			gluCylinder(cyl1, 1, 1, 25, 20, 20);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glColor3f(.87, .87, .87);
			glScalef(6.5, 1, 1);
			glutSolidCube(1);
			glEnd();
			glPopMatrix();

			glPopMatrix();

			drawWheel(5);
			drawWheel(-5);
			glPopMatrix();
		}
	}


	else if (m == 1) {
		if (back == 0) {
			glPushMatrix();
			frontwheelval = planex + 52;
			if (left == 1)
				x = frontLeft + 360;
			else if(right==1)
				x = frontright + 360;
			else 
				x = planey + 360;
			if ((x <= 190 && x >= -190) && (planex + 52 >= -600 && planex + 52 <= -32) && (planez + 3 >= -2 && planez + 3 <= 10))
				lw1 = 1;
			//printf("%d              ",lw1);
			glTranslatef(x, planex + 52, planez + 3);
			glPushMatrix();
			glTranslatef(0, 0, 8.5);
			glPushMatrix();
			glColor3f(.87, .87, .87);
			GLUquadricObj *cyl1 = gluNewQuadric();
			gluQuadricTexture(cyl1, GL_TRUE);
			gluCylinder(cyl1, 1, 1, 16, 20, 20);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glColor3f(.87, .87, .87);
			glScalef(6.5, 1, 1);
			glutSolidCube(1);
			glEnd();
			glPopMatrix();

			glPopMatrix();
			drawWheel(5);
			drawWheel(-5);
			glPopMatrix();
		}
		else if (back == 1) {
			glPushMatrix();
			if (left == 1)
				x = backleft1 + 382;
			else if (right == 1)
				x = backright1 + 382;
			else
				x = planey + 382;

			if ((x <= 190 && x >= -190) && (planex - 15 >= -600 && planex - 15 <= -32) && (planez + 3 >= -2 && planez + 3 <= 10))
				lw2 = 1;
			//printf("%d              ", lw2);
			glTranslatef(x, planex - 15, planez + 3);
			glPushMatrix();
			glTranslatef(0, 0, 8.5);
			glPushMatrix();
			glColor3f(.87, .87, .87);
			GLUquadricObj *cyl1 = gluNewQuadric();
			gluQuadricTexture(cyl1, GL_TRUE);
			gluCylinder(cyl1, 1, 1, 25, 20, 20);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glColor3f(.87, .87, .87);
			glScalef(6.5, 1, 1);
			glutSolidCube(1);
			glEnd();
			glPopMatrix();

			glPopMatrix();

			drawWheel(5);
			drawWheel(-5);
			glPopMatrix();
		}

		else {
			glPushMatrix();
			if (left == 1)
				x = backleft2 + 338;
			else if (right == 1)
				x = backright2 + 338;
			else
				x = planey + 338;
			if ((x <= 190 && x >= -190) && (planex -15 >= -600 && planex - 15 <= -32) && (planez + 3 >= -2 && planez + 3 <= 10))
				lw3 = 1;
		//	printf("%d\n", lw3);
			glTranslatef(x, planex - 15, planez + 3);
			glPushMatrix();
			glTranslatef(0, 0, 8.5);
			glPushMatrix();
			glColor3f(.87, .87, .87);
			GLUquadricObj *cyl1 = gluNewQuadric();
			gluQuadricTexture(cyl1, GL_TRUE);
			gluCylinder(cyl1, 1, 1, 25, 20, 20);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glColor3f(.87, .87, .87);
			glScalef(6.5, 1, 1);
			glutSolidCube(1);
			glEnd();
			glPopMatrix();

			glPopMatrix();

			drawWheel(5);
			drawWheel(-5);
			glPopMatrix();
		}
	}

	else if(a==1){
		if (back == 0) {
			glPushMatrix();
			frontwheelval = planex + 52;
			x = 0;
			if ((x <= 190 && x >= -190) && (planex + 52 >= -550 && planex + 52 <= -32) && (planez + 3 >= 20 && planez + 3 <= 23))
				lw1 = 1;
			glTranslatef(x, planex + 52, planez + 3);
			glPushMatrix();
			glTranslatef(0, 0, 8.5);
			glPushMatrix();
			glColor3f(.87, .87, .87);
			GLUquadricObj *cyl1 = gluNewQuadric();
			gluQuadricTexture(cyl1, GL_TRUE);
			gluCylinder(cyl1, 1, 1, 16, 20, 20);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glColor3f(.87, .87, .87);
			glScalef(6.5, 1, 1);
			glutSolidCube(1);
			glEnd();
			glPopMatrix();

			glPopMatrix();
			drawWheel(5);
			drawWheel(-5);
			glPopMatrix();
		}
		else if (back == 1) {
			glPushMatrix();
			x = -23;
			if ((x <= 190 && x >= -190) && (planex - 15 >= -550 && planex - 15 <= -32) && (planez + 3 >= 20 && planez + 3 <= 23))
				lw2 = 1;
		//	printf("%d           ", lw2);
			glTranslatef(x, planex - 15, planez + 3);
			glPushMatrix();
			glTranslatef(0, 0, 8.5);
			glPushMatrix();
			glColor3f(.87, .87, .87);
			GLUquadricObj *cyl1 = gluNewQuadric();
			gluQuadricTexture(cyl1, GL_TRUE);
			gluCylinder(cyl1, 1, 1, 25, 20, 20);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glColor3f(.87, .87, .87);
			glScalef(6.5, 1, 1);
			glutSolidCube(1);
			glEnd();
			glPopMatrix();

			glPopMatrix();

			drawWheel(5);
			drawWheel(-5);
			glPopMatrix();
		}

		else {
			glPushMatrix();
			x = 23;
			if ((x <= 190 && x >= -190) && (planex - 15 >= -550 && planex - 15 <= -32) && (planez + 3 >= 20 && planez + 3 <= 23))
				lw3 = 1;
		//	printf("%d\n", lw3);
			glTranslatef(x, planex - 15, planez + 3);
			glPushMatrix();
			glTranslatef(0, 0, 8.5);
			glPushMatrix();
			glColor3f(.87, .87, .87);
			GLUquadricObj *cyl1 = gluNewQuadric();
			gluQuadricTexture(cyl1, GL_TRUE);
			gluCylinder(cyl1, 1, 1, 25, 20, 20);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glColor3f(.87, .87, .87);
			glScalef(6.5, 1, 1);
			glutSolidCube(1);
			glEnd();
			glPopMatrix();

			glPopMatrix();

			drawWheel(5);
			drawWheel(-5);
			glPopMatrix();
		}
	}
	//************plane front wheel cylinder
	
}
void planemainBody() {

	glPushMatrix();
	glTranslatef(0, 0, 25);
	glColor3f(1, 1, 1);//must be white for texture


					   /*glEnable(GL_TEXTURE_2D);

					   glBindTexture(GL_TEXTURE_2D, grassimg);
					   glBegin(GL_QUADS);
					   glTexCoord2f(0,1);
					   glVertex3f(-300,300,0);
					   glTexCoord2f(0,0);
					   glVertex3f(-300,-300,0);
					   glTexCoord2f(1,0);
					   glVertex3f(300,-300,0);
					   glTexCoord2f(1,1);
					   glVertex3f(300,300,0);
					   glEnd();

					   glDisable(GL_TEXTURE_2D);


					   glColor3f(1,1,1);

					   glBegin(GL_QUADS);
					   glTexCoord2f(0,0);
					   glVertex3f(0,0,50);
					   glTexCoord2f(1,0);
					   glVertex3f(150,0,50);
					   glTexCoord2f(1,1);
					   glVertex3f(150,0,0);
					   glTexCoord2f(0,1);
					   glVertex3f(0,0,0);
					   glEnd();

					   glDisable(GL_TEXTURE_2D);
					   */
					   //**for 3D basket ball
					   /*glEnable(GL_TEXTURE_2D);
					   glBindTexture(GL_TEXTURE_2D,ballimg);
					   GLUquadricObj *qobj= gluNewQuadric();
					   gluQuadricTexture(qobj,GL_TRUE);
					   gluSphere(qobj,10,20,20);
					   glEnd();*/

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ballimg);
	glPushMatrix();
	glTranslatef(50, 40, 20);
	glRotatef(90, 0, 0, 1);
	glRotatef(180, 0, 1, 1);
	GLUquadricObj *cyl = gluNewQuadric();
	gluQuadricTexture(cyl, GL_TRUE);
	gluCylinder(cyl, 20, 20, 150, 20, 20);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//************plane main cyl body
	glPushMatrix();
	glTranslatef(-1, 0, 25);
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(.1, .58, 0.58, 0.58);
	glTranslatef(62.6, 37, 30);
	glRotatef(90, 0, 0, 1);
	glRotatef(-45, 1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(7, 0, 0);
	glVertex3f(7, 0, 3);
	glVertex3f(0, 0, 3);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.58, 0.58, 0.58);
	glTranslatef(61.6, 40.5, 31.2);
	glRotatef(90, 0, 0, 1);
	glRotatef(-42, 1, 0, 0);
	glScalef(.5, .3, 3);
	glutSolidCube(1);
	glPopMatrix();
	//**********************front transparent glass
	glPushMatrix();
	glTranslatef(50, 40, 45);
	glRotatef(90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);


	double equ[4];
	equ[0] = 0;
	equ[1] = 0;
	equ[2] = -1;
	equ[3] = 0;

	glClipPlane(GL_CLIP_PLANE0, equ);

	glPushMatrix();
	glEnable(GL_CLIP_PLANE0);
	glColor3f(1, 1, 1);
	glutSolidSphere(20, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(55, 40, 45);
	glRotatef(90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);


	double equ1[4];
	equ1[0] = 0;
	equ1[1] = 0;
	equ1[2] = -1;
	equ1[3] = 0;

	glClipPlane(GL_CLIP_PLANE0, equ1);

	glPushMatrix();
	glEnable(GL_CLIP_PLANE0);
	glColor3f(1, 1, 1);
	glutSolidSphere(19, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(58, 40, 45);
	glRotatef(90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);


	equ1[4];
	equ1[0] = 0;
	equ1[1] = 0;
	equ1[2] = -1;
	equ1[3] = 0;

	glClipPlane(GL_CLIP_PLANE0, equ1);

	glPushMatrix();
	glEnable(GL_CLIP_PLANE0);
	glColor3f(1, 1, 1);
	glutSolidSphere(18, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(65, 40, 45);
	glRotatef(90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);


	equ1[4];
	equ1[0] = 0;
	equ1[1] = 0;
	equ1[2] = -1;
	equ1[3] = 0;

	glClipPlane(GL_CLIP_PLANE0, equ1);

	glPushMatrix();
	glEnable(GL_CLIP_PLANE0);
	glColor3f(1, 1, 1);
	glutSolidSphere(14.5, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(73, 40, 45);
	glRotatef(90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);


	equ1[4];
	equ1[0] = 0;
	equ1[1] = 0;
	equ1[2] = -1;
	equ1[3] = 0;

	glClipPlane(GL_CLIP_PLANE0, equ1);

	glPushMatrix();
	glEnable(GL_CLIP_PLANE0);
	glColor3f(.4, .6, 1);
	glutSolidSphere(10, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	if (m == 1) {
		double x;
		glPushMatrix();
		if (left == 1)
			x = noseleft + 360;
		else if (right == 1)
			x = noseright + 360;
		else
			x = planey + 360;
		glTranslatef(x, planex + 64, planez + 44);
		//	glRotatef(rotateplane, 0, 0, 1);
		glRotatef(-90, 1, 0, 0);
		//	glRotatef(90, 0, 1, 0);

		equ[4];
		equ[0] = 0;
		equ[1] = -1;
		equ[2] = 0;
		equ[3] = -7.5;

		glClipPlane(GL_CLIP_PLANE0, equ);


		glEnable(GL_CLIP_PLANE0);

		glPushMatrix();
		glColor3f(.2, 0.2, 0.2);
		glutSolidTorus(4, 14, 30, 30);
		glPopMatrix();

		glDisable(GL_CLIP_PLANE0);
		glPopMatrix();
	}
	else if (a == 1) {
		glPushMatrix();
		glTranslatef(1, planex + 64, planez + 44);
		//	glRotatef(rotateplane, 0, 0, 1);
		glRotatef(-90, 1, 0, 0);
		//	glRotatef(90, 0, 1, 0);

		equ[4];
		equ[0] = 0;
		equ[1] = -1;
		equ[2] = 0;
		equ[3] = -7.5;

		glClipPlane(GL_CLIP_PLANE0, equ);


		glEnable(GL_CLIP_PLANE0);

		glPushMatrix();
		glColor3f(.2, 0.2, 0.2);
		glutSolidTorus(4, 14, 30, 30);
		glPopMatrix();

		glDisable(GL_CLIP_PLANE0);
		glPopMatrix();
	}
	//**************plane nose



}
void wings2() {
	glPushMatrix();
	//	glTranslatef(-2, 45, 17);
	glTranslatef(-40, 49, 54);
	glRotatef(180, 0, 1, 0);
	glRotatef(90, 0, 0, 1);
	glPushMatrix();
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ballimg);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(100, 9, 0);
	glTexCoord2f(0, 1);
	glVertex3f(100, 18, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0, 40, 0);
	glTexCoord2f(1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//**************lower

	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(0, 40, 0);
	glVertex3f(0, 0, 0);

	glVertex3f(0, 0, 3);
	glVertex3f(0, 40, 3);
	glEnd();
	glPopMatrix();
	//******************starting


	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(.4, .6, 1);
	glBegin(GL_QUADS);
	glVertex3f(100, 9, 0);
	glVertex3f(100, 18, 0);
	glVertex3f(100, 18, 3);
	glVertex3f(100, 9, 3);
	glEnd();
	glPopMatrix();
	//**********************ending
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(100, 9, 0);
	glVertex3f(100, 9, 3);
	glVertex3f(0, 0, 3);
	glEnd();
	glPopMatrix();
	//**************1st side
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 40, 0);
	glVertex3f(100, 18, 0);
	glVertex3f(100, 18, 3);
	glVertex3f(0, 40, 3);
	glEnd();
	glPopMatrix();
	//***************2nd side
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 20);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ballimg);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(100, 9, 0);
	glTexCoord2f(0, 1);
	glVertex3f(100, 18, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0, 40, 0);
	glTexCoord2f(1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();
	//****************upper
	glPushMatrix();
	glTranslatef(-25, 85, 34);
	glRotatef(-90, 0, 0, 1);
	glRotatef(180, 1, 0, 0);
	glPushMatrix();
	glColor3f(.4, .6, 1);
	glTranslatef(0, 0, 10);
	glRotatef(90, 1, 0, 0);
	GLUquadricObj *cyl = gluNewQuadric();
	gluQuadricTexture(cyl, GL_TRUE);
	gluCylinder(cyl, 8, 10, 28, 30, 30);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0, 0, 10);
	glRotatef(90, 1, 0, 0);
	GLUquadricObj *cyl1 = gluNewQuadric();
	gluQuadricTexture(cyl1, GL_TRUE);
	gluCylinder(cyl1, 9, 10.5, 28.5, 30, 30);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(.4, 0.6, 1);
	glTranslatef(0, 12, 10);
	glRotatef(90, 1, 0, 0);
	gluQuadricTexture(cyl1, GL_TRUE);
	gluCylinder(cyl1, 3, 8, 12, 30, 30);
	glEnd();
	glPopMatrix();

	glPopMatrix();


}
void wings1() {
	glPushMatrix();
	glTranslatef(-40, 35, 17);
	glRotatef(-90, 0, 0, 1);
	glPushMatrix();
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ballimg);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(100, 9, 0);
	glTexCoord2f(0, 1);
	glVertex3f(100, 18, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0, 40, 0);
	glTexCoord2f(1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//**************lower

	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(0, 40, 0);
	glVertex3f(0, 0, 0);

	glVertex3f(0, 0, 3);
	glVertex3f(0, 40, 3);
	glEnd();
	glPopMatrix();
	//******************starting


	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(.4, .6, 1);
	glBegin(GL_QUADS);
	glVertex3f(100, 9, 0);
	glVertex3f(100, 18, 0);
	glVertex3f(100, 18, 3);
	glVertex3f(100, 9, 3);
	glEnd();
	glPopMatrix();
	//**********************ending
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(100, 9, 0);
	glVertex3f(100, 9, 3);
	glVertex3f(0, 0, 3);
	glEnd();
	glPopMatrix();
	//**************1st side
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 40, 0);
	glVertex3f(100, 18, 0);
	glVertex3f(100, 18, 3);
	glVertex3f(0, 40, 3);
	glEnd();
	glPopMatrix();
	//***************2nd side
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 20);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ballimg);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(100, 9, 0);
	glTexCoord2f(0, 1);
	glVertex3f(100, 18, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0, 40, 0);
	glTexCoord2f(1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//****************upper
	glPushMatrix();
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, ballimg);
	glTranslatef(45, 15, 17);
	glRotatef(180, 1, 0, 0);
	glPushMatrix();
	glColor3f(.4, .6, 1);
	glTranslatef(0, 0, 10);
	glRotatef(90, 1, 0, 0);
	GLUquadricObj *cyl = gluNewQuadric();
	gluQuadricTexture(cyl, GL_TRUE);
	gluCylinder(cyl, 8, 10, 28, 30, 30);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, ballimg);
	glColor3f(1, 1, 1);
	glTranslatef(0, 0, 10);
	glRotatef(90, 1, 0, 0);
	GLUquadricObj *cyl1 = gluNewQuadric();
	gluQuadricTexture(cyl1, GL_TRUE);
	gluCylinder(cyl1, 9, 10.5, 28.5, 30, 30);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, ballimg);
	glColor3f(.4, .6, 1);
	glTranslatef(0, 12, 10);
	glRotatef(90, 1, 0, 0);
	gluQuadricTexture(cyl1, GL_TRUE);
	gluCylinder(cyl1, 3, 8, 12, 30, 30);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}
void backwing2() {
	glPushMatrix();
	glTranslatef(-118, 50, 62);
	glRotatef(180, 0, 1, 0);
	glRotatef(90, 0, 0, 1);
	glRotatef(10, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ballimg);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(50, 9, 0);
	glTexCoord2f(0, 1);
	glVertex3f(50, 16, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0, 30, 0);
	glTexCoord2f(1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//**************lower

	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 30, 0);
	glVertex3f(0, 0, 0);

	glVertex3f(0, 0, 1.5);
	glVertex3f(0, 30, 1.5);
	glEnd();
	glPopMatrix();
	//******************starting


	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(.4, .6, 1);
	glBegin(GL_QUADS);
	glVertex3f(50, 9, 0);
	glVertex3f(50, 16, 0);
	glVertex3f(50, 16, 1.5);
	glVertex3f(50, 9, 1.5);
	glEnd();
	glPopMatrix();
	//**********************ending
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(50, 9, 0);
	glVertex3f(50, 9, 1.5);
	glVertex3f(0, 0, 1.5);
	glEnd();
	glPopMatrix();
	//**************1st side
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 30, 0);
	glVertex3f(50, 16, 0);
	glVertex3f(50, 16, 1.5);
	glVertex3f(0, 30, 1.5);
	glEnd();
	glPopMatrix();
	//***************2nd side
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 18.5);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ballimg);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(50, 9, 0);
	glTexCoord2f(0, 1);
	glVertex3f(50, 16, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0, 30, 0);
	glTexCoord2f(1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//****************upper

	glPopMatrix();
}

void backwing1() {
	glPushMatrix();
	glTranslatef(-118, 23, 28);
	glRotatef(-90, 0, 0, 1);
	glRotatef(-10, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ballimg);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(50, 9, 0);
	glTexCoord2f(0, 1);
	glVertex3f(50, 16, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0, 30, 0);
	glTexCoord2f(1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//**************lower

	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 30, 0);
	glVertex3f(0, 0, 0);

	glVertex3f(0, 0, 1.5);
	glVertex3f(0, 30, 1.5);
	glEnd();
	glPopMatrix();
	//******************starting


	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(.4, .6, 1);
	glBegin(GL_QUADS);
	glVertex3f(50, 9, 0);
	glVertex3f(50, 16, 0);
	glVertex3f(50, 16, 1.5);
	glVertex3f(50, 9, 1.5);
	glEnd();
	glPopMatrix();
	//**********************ending
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(50, 9, 0);
	glVertex3f(50, 9, 1.5);
	glVertex3f(0, 0, 1.5);
	glEnd();
	glPopMatrix();
	//**************1st side
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 30, 0);
	glVertex3f(50, 16, 0);
	glVertex3f(50, 16, 1.5);
	glVertex3f(0, 30, 1.5);
	glEnd();
	glPopMatrix();
	//***************2nd side
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 18.5);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ballimg);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(50, 9, 0);
	glTexCoord2f(0, 1);
	glVertex3f(50, 16, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0, 30, 0);
	glTexCoord2f(1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//****************upper

	glPopMatrix();
}
void planeBack() {
	glPushMatrix();
	glTranslatef(-92, 50, 45);
	glRotatef(180, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);
	glPushMatrix();
	glTranslatef(10, 0, 10);
	glRotatef(90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);


	double equ1[4];
	equ1[0] = 0;
	equ1[1] = 0;
	equ1[2] = -1;
	equ1[3] = 7.5;

	glClipPlane(GL_CLIP_PLANE0, equ1);

	glPushMatrix();
	glEnable(GL_CLIP_PLANE0);
	glColor3f(1, 1, 1);
	glutSolidSphere(20.1, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(17, 0, 10);
	glRotatef(90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);


	equ1[4];
	equ1[0] = 0;
	equ1[1] = 0;
	equ1[2] = -1;
	equ1[3] = 4.5;

	glClipPlane(GL_CLIP_PLANE0, equ1);

	glPushMatrix();
	glEnable(GL_CLIP_PLANE0);
	glColor3f(.4, .6, 1);
	glutSolidSphere(17, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(28.5, 0, 13);
	glRotatef(90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);


	equ1[4];
	equ1[0] = 0;
	equ1[1] = 0;
	equ1[2] = -1;
	equ1[3] = 0;

	glClipPlane(GL_CLIP_PLANE0, equ1);

	glPushMatrix();
	glEnable(GL_CLIP_PLANE0);
	glColor3f(.4, .6, 1);
	glutSolidSphere(7, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();


}
void upperWing() {
	glPushMatrix();
	glTranslatef(-80, 55, 64);
	//	glRotatef(180, 0, 1, 0);
	glRotatef(90, 0, 0, 1);
	glRotatef(-90, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ballimg);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(50, 9, 0);
	glTexCoord2f(0, 1);
	glVertex3f(50, 16, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0, 30, 0);
	glTexCoord2f(1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//**************lower

	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 30, 0);
	glVertex3f(0, 0, 0);

	glVertex3f(0, 0, .8);
	glVertex3f(0, 30, .8);
	glEnd();
	glPopMatrix();
	//******************starting


	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(.4, .6, 1);
	glBegin(GL_QUADS);
	glVertex3f(50, 9, 0);
	glVertex3f(50, 16, 0);
	glVertex3f(50, 16, .8);
	glVertex3f(50, 9, .8);
	glEnd();
	glPopMatrix();
	//**********************ending
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(50, 9, 0);
	glVertex3f(50, 9, .8);
	glVertex3f(0, 0, .8);
	glEnd();
	glPopMatrix();
	//**************1st side
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 30, 0);
	glVertex3f(50, 16, 0);
	glVertex3f(50, 16, .8);
	glVertex3f(0, 30, .8);
	glEnd();
	glPopMatrix();
	//***************2nd side
	glPushMatrix();
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 0, 17.8);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ballimg);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(50, 9, 0);
	glTexCoord2f(0, 1);
	glVertex3f(50, 16, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0, 30, 0);
	glTexCoord2f(1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//****************upper

	glPopMatrix();
}
void plane(double x, double y, double z,double r) {
	glPushMatrix();
	glRotatef(r, 0, 0, 1);
	glTranslatef(x, y, z);
	wings1();
	upperWing();
	wings2();
	backwing1();
	backwing2();
	planeBack();
	planemainBody();
	glPopMatrix();
	glPushMatrix();
	newWheel(0);
	newWheel(1);
	newWheel(2);
	glPopMatrix();
}
void loadTexture() {
	glPushMatrix();
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, flwimg);
	glBegin(GL_QUADS);
	glVertex3f(-1600, -2000, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-800, -2000, 0);
	glTexCoord2f(1, 0);
	glVertex3f(-800, -30, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-1600, -30, 0);
	glTexCoord2f(0, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, flwimg);
	glBegin(GL_QUADS);
	glVertex3f(500, -2000, 0);
	glTexCoord2f(0, 0);
	glVertex3f(1000, -2000, 0);
	glTexCoord2f(1, 0);
	glVertex3f(1000, -30, 0);
	glTexCoord2f(1, 1);
	glVertex3f(500, -30, 0);
	glTexCoord2f(0, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();



	glPushMatrix();
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, flwimg);
	glBegin(GL_QUADS);
	glVertex3f(-400, -610, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-200, -610, 0);
	glTexCoord2f(1, 0);
	glVertex3f(-200, -30, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-400, -30, 0);
	glTexCoord2f(0, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	glPushMatrix();
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wallimg);
	glBegin(GL_QUADS);
	glVertex3f(-800, -610, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-400, -610, 0);
	glTexCoord2f(1, 0);
	glVertex3f(-400, -30, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-800, -30, 0);
	glTexCoord2f(0, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	glPushMatrix();
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wallimg);
	glBegin(GL_QUADS);
	glVertex3f(-200, -610, 0);
	glTexCoord2f(0, 0);
	glVertex3f(200, -610, 0);
	glTexCoord2f(1, 0);
	glVertex3f(200, -30, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-200, -30, 0);
	glTexCoord2f(0, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();



	glPushMatrix();
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, soilimg);
	glBegin(GL_QUADS);
	glVertex3f(200, -2000, 0);
	glTexCoord2f(0, 0);
	glVertex3f(500, -2000, 0);
	glTexCoord2f(1, 0);
	glVertex3f(500, -30, 0);
	glTexCoord2f(1, 1);
	glVertex3f(200, -30, 0);
	glTexCoord2f(0, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, grassimg);
	glBegin(GL_QUADS);
	glVertex3f(-800, -2000, 0);
	glTexCoord2f(0, 0);
	glVertex3f(200, -2000, 0);
	glTexCoord2f(1, 0);
	glVertex3f(200, -610, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-800, -610, 0);
	glTexCoord2f(0, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}
void fullhouse(int n,int x) {
	int i = 0;
	double r = 0;
	
	for (i = 0; i < n; i++) {
	
		
		glPushMatrix();
		glRotatef(-90, 0, 0, 1);
		glTranslatef(x, 250, r);
		r = r + 60;
		glPushMatrix();
		glColor3f(.2, .3, .6);
		glTranslatef(0, 0, 30);
		glScalef(80, 100, 60);
		glutSolidCube(1);
		glPopMatrix();
		if (i == 0) {
			glPushMatrix();
			glTranslatef(0, -50.2, 30);
			glColor3f(1, .6, .2);
			glScalef(18, 1, 35);
			glutSolidCube(1);
			glPopMatrix();
		}
		glPushMatrix();
		glTranslatef(-25, -50.2, 38);
		glColor3f(.2, 0.2, 0.2);
		glScalef(20, 1, 25);
		glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(25, -50.2, 38);
		glColor3f(.2, 0.2, 0.2);
		glScalef(20, 1, 25);
		glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40.5, -25, 38);
		glColor3f(.2, 0.2, 0.2);
		glScalef(1, 20, 25);
		glutSolidCube(1);
		glPopMatrix();


		glPushMatrix();
		glTranslatef(-40.5, -25, 38);
		glColor3f(.2, 0.2, 0.2);
		glScalef(1, 20, 25);
		glutSolidCube(1);
		glPopMatrix();


		glPushMatrix();
		glTranslatef(40.5, 25, 38);
		glColor3f(.2, 0.2, 0.2);
		glScalef(1, 20, 25);
		glutSolidCube(1);
		glPopMatrix();


		glPushMatrix();
		glTranslatef(-40.5, 25, 38);
		glColor3f(.2, 0.2, 0.2);
		glScalef(1, 20, 25);
		glutSolidCube(1);
		glPopMatrix();

		glPopMatrix();
	}
	
}
void display() {
	//codes for Models, Camera

	//clear the display
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear buffers to preset values

															/***************************
															/ set-up camera (view) here
															****************************/
															//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);		//specify which matrix is the current matrix

									//initialize the matrix
	glLoadIdentity();				//replace the current matrix with the identity matrix [Diagonals have 1, others have 0]

									//now give three info
									//1. where is the camera (viewer)?
									//2. where is the camera looking?
									//3. Which direction is the camera's UP direction?

									//gluLookAt(0,-150,20,	0,0,0,	0,0,1);
	gluLookAt(cameraRadius*sin(cameraAngle), -cameraRadius*cos(cameraAngle), cameraHeight, 0, 0, 0, 0, 0, 1);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/**************************************************
	/ Grid and axes Lines(You can remove them if u want)
	***************************************************/
//	drawGridAndAxes();

	/****************************
	/ Add your objects from here
	****************************/
	if (create == 1) {
		plane(planex, planey, planez, rotateplane);
	}

	loadTexture();
	fullhouse(3,750);
	fullhouse(2, 1150);
	fullhouse(1, 550);
	fullhouse(6, 1550);
	if (text == 1)
		render();

	if (text == 3)
		render1();

	if (text == 4)
		render2();
	if (text == 6)
		render3();
	
	/*glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-300, -25, 125);
	glScalef(1300, 2, 125);
	glutSolidCube(2);
	glPopMatrix();
	*///ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate() {
	//codes for any changes in Models, Camera
	speed += .5;
	tspeed -= .5;
	if (a == 1) {
		if (create == 1 && drive == 1)
		{
			count++;
			if (count % 50 == 0) {
				cameraAngle = cameraAngle + .0001;
			}

			if (lw1 == 1 && lw2 == 1 && lw3 == 1)
			{
				text = 4;
				if (land != 5) {
					planex = planex + .07;
					cameraRadius = cameraRadius - .07;
				}
				if (land == 5) {
					text = 0;
				}
			}
			else {

				if (land == 1) {
					if (frontwheelval <= -600) {
						text = 0;
						planex = planex + .07;
						planez = planez - .04;
						cameraHeight = cameraHeight - .04;
						cameraRadius = cameraRadius - .07;
					}
					else {
						text = 0;
						planex = planex + .07;
						cameraRadius = cameraRadius - .07;
					}
				}
				else
				{
					if (planex <= -1050 && planex >= -1100)
					{
						text = 1;
					}
					else {
						text = 0;
						planex = planex + .07;
						cameraRadius = cameraRadius - .07;
					}
				}
			}
		}
	}


	if (takef == 1) {

		planex = planex + .07;
		if (planex >= 1500)
			text = 6;
		cameraRadius = cameraRadius + .07;
		if(cameraAngle<=-.6)
		cameraAngle = cameraAngle + .0001;
		frontLeft = frontLeft - .07;
		backleft1 = backleft1 - .07;
		backleft2 = backleft2 - .07;
		if (lw1 == 1 && lw2 == 1 && lw3 == 1)
		{
			text = 3;
			create = 0;
		}
		else {
			if (mnupper == 1) {
				planez = planez + 1;
				cameraHeight = cameraHeight + 1;
				mnupper = 0;
			}
		}

	}

	if (m == 1) {
		if (create == 1 && drive == 1)
		{
			if (lw1 == 1 && lw2 == 1 && lw3 == 1)
			{
				text = 4;
				if (land != 5) {
					planex = planex + .07;
					cameraRadius = cameraRadius - .07;
				}
				
			}
			else {
				planex = planex + .07;
				cameraRadius = cameraRadius - .07;
				if (mnland == 1) {
					planez = planez - 1;
					cameraHeight = cameraHeight - .05;
					mnland = 0;
				}

				if (mnupper == 1) {
					planez = planez + 1;
					cameraHeight = cameraHeight + .05;
					mnupper = 0;
				}

				if (planez <= 330 && planey <= -140 && (planex <= -1550 && planex >= -1560)) {
					text = 3;
					create = 0;
				}

				if (planez <= 190 && planey <= -140 && (planex <= -750 && planex >= -760)) {
					text = 3;
					create = 0;
				}

				if (planez <= 130 && planey <= -140 && (planex <= -1150 && planex >= -1160)) {
					text = 3;
					create = 0;
				}
			}
		}

	}
	//	cameraAngle += cameraAngleDelta;	// camera will rotate at 0.002 radians per frame.

	//codes for any changes in Models

	//MISSING SOMETHING? -- YES: add the following
	glutPostRedisplay();	//this will call the display AGAIN

}

void init() {
	//codes for initialization
	speed = 0.0;
	loadImage();
	move_X = 0;
	move_Y = 0;
	move_Z = 0;
	canDrawGrid = 1;
	canDrawAxis = 1;
	rotateplane = 90;
	cameraAngleDelta = .001;
	drive = 0;
    create = 0;
	land = 0;
	m = 0;
	a = 1;
	cameraAngle = .072;	//angle in radian
	cameraRadius = 2150;
	cameraHeight = 330;

	//clear the screen
	glClearColor(0, 0, 0, 0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	/*
	gluPerspective() — set up a perspective projection matrix

	fovy -         Specifies the field of view angle, in degrees, in the y direction.
	aspect ratio - Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
	zNear -        Specifies the distance from the viewer to the near clipping plane (always positive).
	zFar  -        Specifies the distance from the viewer to the far clipping plane (always positive).
	*/

	gluPerspective(70, 1, 0.1, 10000.0);

}

void keyboardListener(unsigned char key, int x, int y) {
	switch (key) {

	case '1':
		if (m == 1) {
			left = 1;
			frontLeft = frontLeft - .35;
			backleft1 = backleft1 - .35;
			backleft2 = backleft2 - .35;
			noseleft = noseleft - .35;
			planey = planey + .35;
			frontright = frontright - .35;
			backright1 = backright1 - .35;
			backright2 = backright2 - .35;
			noseright = noseright - .35;
			
		}
		break;

	case '2':
		if (m == 1) {
			right = 1;
			frontright = frontright + .35;
			backright1 = backright1 + .35;
			backright2 = backright2 + .35;
			noseright = noseright + .35;
			planey = planey - .35;
			frontLeft = frontLeft + .35;
			backleft1 = backleft1 + .35;
			backleft2 = backleft2 + .35;
			noseleft = noseleft + .35;

		}
		break;

	case '3':
		move_Y += 1;
		break;
	case '4':
		move_Y -= 1;
		break;

	case '5':
		move_Z += 1;
		break;
	case 'q':
		if (m == 0 && a == 0)
		{
			takef = 1;
			
		}
		break;

	case 't':
		takeoff = 1;
		takef = 0;
		text = 0;
		lw1 = 0;
		lw2 = 0;
		lw3 = 0;
		m = 0;
		a = 0;
		rotateplane = -90;
		planex = 200;
		frontLeft = planex;
		planey = -650;
		planez = 0;
		cameraAngle = -1.2;	//angle in radian
		cameraRadius = 1200;
		cameraHeight = 150;
		create = 1;
		break;
	case 'm':

		if (m == 0)
		{
			m = 1;
			a = 0;
			takef = 0;
			takeoff = 0;
			create = 1;
			cameraAngle = .042;
			cameraRadius = 2150;
			cameraHeight = 330;
			planex = -2000;
			planey = -200;
			frontLeft = planey;
			backleft1 = planey;
			backleft2 = planey;
			noseleft = planey;
			frontright = planey;
			backright1 = planey;
			backright2 = planey;
			noseright = planey;
			planez = 275;
			drive = 0;

		}

		break;

	case 'a':
		if (m == 0)
		{
			a = 1;
			m = 0;
			takef = 0;
			takeoff = 0;

		}
		break;

	case 'l':
		land = 1;
		break;
	case 's':
		if (m == 1||a==1||takeoff==1)
		{
			text = 0;
			land = 5;
			if (takeoff == 1)
			{
				takef = 0;
				takeoff = 0;
				create = 0;
			}
			
		}
		break;

	case 'p':
			mnland = 1;
			mnupper = 0;
		break;

	case 'o':
		if (m == 1 || takef == 1) {
			mnland = 0;
			mnupper = 1;
		}
		break;



	case 'd':
		if (drive == 0)
			if (m == 1 || a == 1)
			{
				drive = 1;
			}
		break;

	case 'e':
		exit(0);
		break;


	case 'c':
		if (create == 0) {
			
			takeoff = 0;
			takef = 0;
			cameraAngle = .042;
			cameraRadius = 2150;
			cameraHeight = 330;
			planex = -2000;
			planey = -41;
			frontLeft = planey;
			planez = 275;
			drive = 0;
			create = 1;
		}
		break;

	default:
		break;
	}
}

void specialKeyListener(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:		//down arrow key
		cameraRadius += 10;
		break;
	case GLUT_KEY_UP:		// up arrow key
		if (cameraRadius > 10)
			cameraRadius -= 10;
		break;

	case GLUT_KEY_RIGHT:
		cameraAngle += 0.01;
		break;
	case GLUT_KEY_LEFT:
		cameraAngle -= 0.01;
		break;

	case GLUT_KEY_PAGE_UP:
		cameraHeight += 10;
		break;
	case GLUT_KEY_PAGE_DOWN:
		cameraHeight -= 10;
		break;

	case GLUT_KEY_INSERT:
		break;

	case GLUT_KEY_HOME:
		cameraAngle = 0;
		cameraRadius = 150;
		cameraHeight = 50;
		break;
	case GLUT_KEY_END:

		break;

	default:
		break;
	}
}

void mouseListener(int button, int state, int x, int y) {	//x, y is the x-y of the screen (2D)
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {		// 2 times?? in ONE click? -- solution is checking DOWN or UP
			cameraAngleDelta = -cameraAngleDelta;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		//........
		break;

	case GLUT_MIDDLE_BUTTON:
		//........
		break;

	default:
		break;
	}
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);							//initialize the GLUT library

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);

	/*
	glutInitDisplayMode - inits display mode
	GLUT_DOUBLE - allows for display on the double buffer window
	GLUT_RGBA - shows color (Red, green, blue) and an alpha
	GLUT_DEPTH - allows for depth buffer
	*/
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

	glutCreateWindow("Some Title");

	printf("Camera Control\n");
	printf("_____________\n");
	printf("Zoom In-Out: UP and DOWN arrow\n");
	printf("Camera Rotate: LEFT and RIGHT arrow\n");
	printf("Up-Down: PAGEUP and PAGEDOWN\n");
	printf("Reset Camera: HOME\n");
	printf("CREATE PLANE: c\n");
	printf("AUTO LANDING: a\n");
	printf("MANUAL LANDING: m\n");
	printf("DRIVE FRONT: d\n");
	printf("PLANE DOWN: p\n");
	printf("PLANE UP: o\n");
	printf("PLANE LEFT: 1\n");
	printf("PLANE RIGHT: 2\n");
	printf("LANDING: l\n");
	printf("TAKEOFF: t\n");
	printf("TAKEOFF FRONT: q\n");
	printf("EXIT: e\n");

	init();						//codes for initialization

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
