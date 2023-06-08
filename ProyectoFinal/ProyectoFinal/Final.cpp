/*---------------------------------------------------------*/
/* ----------------  Proyecto Final               -----------*/
/*-----------------    2023-2   ---------------------------*/
/*------------- Alumnos:Paredes Pacab Rosaura Elena 419049582 ---------------*/
/*-------------			Rodriguez Serna Guillermo Andres 315245923  */
/*-------------			Fajardo Tellez Eduardo 316053206       ---------------*/
#include <Windows.h>
#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
//Para leer archivos
#include <fstream>
#include <string.h>

// Pragma para musica
#pragma comment(lib, "winmm.lib")
bool sound = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

void getResolution(void);

/////////////////////////////////ACUARIO////////////////////////////////////////

//----------------- ACUARIO -------------------------------//
//Tiburon
float tbInitX = 600.0F;
float tbInitY = 15.0f;
float tbInitZ = -1100.0f;

glm::vec3 tbTraslation(tbInitX, tbInitY, tbInitZ);
bool sharkAnimate = false;
//Limites
int limiteInicio = 100;
int limiteNino = 400;
//rotaciones
float	rotacionCola = 0.0f;//define el giro de la cola 
float	rotacionCuerpo = 0.0f;//giro circular
float	tbRotacionCuerpoChoque = 0.0f;//giro sobre su lugar
//
bool girarColDer = true;//define hacia donde gira la cola
bool tbMov = false;//nino
float tbVarTraslation = 0.0f;//define la traslacion
int tbVueltas = 0; //define cuando deja de girar
int tbTiempoGiroChoque = 0;//define el tiempo de giro
float velocidadTraslacion = 0.0f;//define la velocidad a la que se mueve el tiburon

/**Estados del tiburon *
* Estado 0: el tiburon va de ida y regreso
* Estado 1: el tiburon gira
* Estado 2: el tibiron se dirige al niño
* Estado 3: el tiburon gira como tornado y regresa
*/
int sharkState = 0;

//Children Camina
float cWPuntoInicial = 290.0f;
float cWPuntoFinalal = 180.0f;
float cWLejania = 60.0f;
glm::vec3 cWTraslation(cWPuntoInicial, 0.0f, tbInitZ + 430);
bool tbTiburon = true;
bool cWVisible = true;
float cWCountTimeAnim = 0.0f;

int childrenState = 0;



//Pes
float psInitX = -100.0f;
float psInitY = 10.0f;
float psInitZ = -550.0f;
bool psSube = true;//define si el pes sube o baja
glm::vec3 psTraslation(psInitX, psInitY, psInitZ);
float psGiroAleta = 0.0f;
float psGiroCola = 0.0f;
bool movPes = false;
int statePes = 0;
float psRotacionCuerpo = 0.0f;
bool girarColDerPes = true;//define hacia donde gira la cola
void giroCola(int vel);
void giroColaPes(int vel);

////////////////////////FIN ACUARIO////////////////////////////////////


/////////////CARRO/////
// posiciones
//float x = 0.0f;
//float y = 0.0f;
//para el movimiento del automovil
float	movAuto_x = 300.0f,
movAuto_z = 1200.0f,
movAuto_y = 10.0f,
orienta = 180.0f,
giroCar = 0.0f,
girollantas = 0.0f,
girollantasTraceras = 0.0f,
girollantasLado = 0.0f;
int estadoAuto = 1;
bool	animacion = false;//para el carro
		




// camera
Camera camera(glm::vec3(940.0f, 7.0f, -180.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotRodIzq = 0.0f,
giroMonito = 0.0f;
bool rejasMovidas = false;
float rejaIzquierdaPosZ = 0.0f;
float rejaDerechaPosZ = 0.0f;


//Variables Lobo
ifstream AniLobo;
string strL;
int indL = 1;
float	Esc_Lobo = 30.0f,
		LposX = 0.0f,
		LposY = 0.0f,
		LposZ = 0.0f,
		LgiroPFD = 0.0f,
		LgiroPFI = 0.0f,
		LgiroCola = 0.0f,
		LgiroCab = 0.0f;
float	incLposX = 0.0f,
		incLposY = 0.0f,
		incLposZ = 0.0f,
		incLgiroPFD = 0.0f,
		incLgiroPFI = 0.0f,
		incLgiroCola = 0.0f,
		incLgiroCab = 0.0f;

//Variables Jirafa
ifstream AniJirafa;
string strJ;
int indJ = 1;
float	Esc_Jirafa = 50.0f,
		JposX = 0.0f,
		JposY = 0.0f,
		JposZ = 0.0f,
		JgiroPFD = 0.0f,
		JgiroPFI = 0.0f,
		JgiroPzFD = 0.0f,
		JgiroPzFI = 0.0f,
		JgiroPTD = 0.0f,
		JgiroPTI = 0.0f,
		JgiroCola = 0.0f,
		Jcabeza = 0.0f,
		Jcuello = 0.0f,
		Jboca = 0.0f,
		Jlengua = 0.0f;
float	incJposX = 0.0f,
		incJposY = 0.0f,
		incJposZ = 0.0f,
		incJgiroPFD = 0.0f,
		incJgiroPFI = 0.0f,
		incJgiroPzFD = 0.0f,
		incJgiroPzFI = 0.0f,
		incJgiroPTD = 0.0f,
		incJgiroPTI = 0.0f,
		incJgiroCola = 0.0f,
		incJcabeza = 0.0f,
		incJcuello = 0.0f,
		incJboca = 0.0f;

#define MAX_FRAMESL 5
#define MAX_FRAMESJ 5
int i_max_steps = 60;
int i_curr_steps = 0;


typedef struct _frameLobo
{
	//Variables para GUARDAR Key Frames del Lobo
	float LposX = 0.0f;
	float LposY = 0.0f;
	float LposZ = 0.0f;
	float LgiroPFD = 0.0f;
	float LgiroPFI = 0.0f;
	float LgiroCola = 0.0f;
	float LgiroCab = 0.0f;

}FRAMEL;

typedef struct _frameJirafa
{
	//Variables para GUARDAR Key Frames de la Jirafa
	float JposX = 0.0f;
	float JposY = 0.0f;
	float JposZ = 0.0f;
	float JgiroPFD = 0.0f;
	float JgiroPFI = 0.0f;
	float JgiroPzFD = 0.0f;
	float JgiroPzFI = 0.0f;
	float JgiroPTD = 0.0f;
	float JgiroPTI = 0.0f;
	float JgiroCola = 0.0f;
	float Jcabeza = 0.0f;
	float Jcuello = 0.0f;
	float Jboca = 0.0f;

}FRAMEJ;

FRAMEL KFLobo[MAX_FRAMESL];
int FILobo = 0;			//introducir número en caso de tener Key guardados
bool playL = false;
int playIndexL = 0;

FRAMEJ KFJirafa[MAX_FRAMESJ];
int FIJirafa = 0;			//introducir número en caso de tener Key guardados
bool playJ = false;
int playIndexJ = 0;

//Funcion para guardar los frames de la animacion del lobo
void saveFrameL(void)
{
	printf("frameindex %d\n", FILobo);
	std::cout << "Frame Index = " << FILobo << std::endl;

	KFLobo[FILobo].LposX = LposX;
	KFLobo[FILobo].LposY = LposY;
	KFLobo[FILobo].LposZ = LposZ;
	KFLobo[FILobo].LgiroPFD = LgiroPFD;
	KFLobo[FILobo].LgiroPFI = LgiroPFI;
	KFLobo[FILobo].LgiroCola = LgiroCola;
	KFLobo[FILobo].LgiroCab = LgiroCab;

	//printf("frame guardado, KFLobo[%d] = %f\n", FILobo, LgiroCab);
	FILobo++;

}

//Funcion para guardar los frames de la animacion de la jirafa
void saveFrameJ(void)
{
	printf("frameindex %d\n", FIJirafa);
	std::cout << "Frame Index = " << FIJirafa << std::endl;

	KFJirafa[FIJirafa].JposX = JposX;
	KFJirafa[FIJirafa].JposY = JposY;
	KFJirafa[FIJirafa].JposZ = JposZ;
	KFJirafa[FIJirafa].JgiroPFD = JgiroPFD;
	KFJirafa[FIJirafa].JgiroPFI = JgiroPFI;
	KFJirafa[FIJirafa].JgiroPzFD = JgiroPzFD;
	KFJirafa[FIJirafa].JgiroPzFI = JgiroPzFI;
	KFJirafa[FIJirafa].JgiroPTD = JgiroPTD;
	KFJirafa[FIJirafa].JgiroPTI = JgiroPTI;
	KFJirafa[FIJirafa].JgiroCola = JgiroCola;
	KFJirafa[FIJirafa].Jcabeza = Jcabeza;
	KFJirafa[FIJirafa].Jcuello = Jcuello;
	KFJirafa[FIJirafa].Jboca = Jboca;
	printf("frame guardado, KFJirafa[%d] = %f\n", FIJirafa, Jcabeza);
	FIJirafa++;

}

//Funcion para resetear la animacion del lobo
void resetL(void)
{
	LposX = KFLobo[0].LposX;
	LposY = KFLobo[0].LposY;
	LposZ = KFLobo[0].LposZ;
	LgiroPFD = KFLobo[0].LgiroPFD;
	LgiroPFI = KFLobo[0].LgiroPFI;
	LgiroCola = KFLobo[0].LgiroCola;
	LgiroCab = KFLobo[0].LgiroCab;
}

//Funcion para resetear la animacion de la jirafa
void resetJ(void)
{
	JposX = KFJirafa[0].JposX;
	JposY = KFJirafa[0].JposY;
	JposZ = KFJirafa[0].JposZ;
	JgiroPFD = KFJirafa[0].JgiroPFD;
	JgiroPFI = KFJirafa[0].JgiroPFI;
	JgiroPzFD = KFJirafa[0].JgiroPzFD;
	JgiroPzFI = KFJirafa[0].JgiroPzFI;
	JgiroPTD = KFJirafa[0].JgiroPTD;
	JgiroPTI = KFJirafa[0].JgiroPTI;
	JgiroCola = KFJirafa[0].JgiroCola;
	Jcabeza = KFJirafa[0].Jcabeza;
	Jcuello = KFJirafa[0].Jcuello;
	Jboca = KFJirafa[0].Jboca;
}

//Funcion de interpolacion para la animacion del lobo
void interpolationL(void)
{
	incLposX = (KFLobo[playIndexL + 1].LposX - KFLobo[playIndexL].LposX) / i_max_steps;
	incLposY = (KFLobo[playIndexL + 1].LposY - KFLobo[playIndexL].LposY) / i_max_steps;
	incLposZ = (KFLobo[playIndexL + 1].LposZ - KFLobo[playIndexL].LposZ) / i_max_steps;
	incLgiroPFD = (KFLobo[playIndexL + 1].LgiroPFD - KFLobo[playIndexL].LgiroPFD) / i_max_steps;
	incLgiroPFI = (KFLobo[playIndexL + 1].LgiroPFI - KFLobo[playIndexL].LgiroPFI) / i_max_steps;
	incLgiroCola = (KFLobo[playIndexL + 1].LgiroCola - KFLobo[playIndexL].LgiroCola) / i_max_steps;
	incLgiroCab = (KFLobo[playIndexL + 1].LgiroCab - KFLobo[playIndexL].LgiroCab) / i_max_steps;
}

//Funcion de interpolacion para la animacion de la jirafa
void interpolationJ(void)
{
	incJposX = (KFJirafa[playIndexJ + 1].JposX - KFJirafa[playIndexJ].JposX) / i_max_steps;
	incJposY = (KFJirafa[playIndexJ + 1].JposY - KFJirafa[playIndexJ].JposY) / i_max_steps;
	incJposZ = (KFJirafa[playIndexJ + 1].JposZ - KFJirafa[playIndexJ].JposZ) / i_max_steps;
	incJgiroPFD = (KFJirafa[playIndexJ + 1].JgiroPFD - KFJirafa[playIndexJ].JgiroPFD) / i_max_steps;
	incJgiroPFI = (KFJirafa[playIndexJ + 1].JgiroPFI - KFJirafa[playIndexJ].JgiroPFI) / i_max_steps;
	incJgiroPzFD = (KFJirafa[playIndexJ + 1].JgiroPzFD - KFJirafa[playIndexJ].JgiroPzFD) / i_max_steps;
	incJgiroPzFI = (KFJirafa[playIndexJ + 1].JgiroPzFI - KFJirafa[playIndexJ].JgiroPzFI) / i_max_steps;
	incJgiroPTD = (KFJirafa[playIndexJ + 1].JgiroPTD - KFJirafa[playIndexJ].JgiroPTD) / i_max_steps;
	incJgiroPTI = (KFJirafa[playIndexJ + 1].JgiroPTI - KFJirafa[playIndexJ].JgiroPTI) / i_max_steps;
	incJgiroCola = (KFJirafa[playIndexJ + 1].JgiroCola - KFJirafa[playIndexJ].JgiroCola) / i_max_steps;
	incJcabeza = (KFJirafa[playIndexJ + 1].Jcabeza - KFJirafa[playIndexJ].Jcabeza) / i_max_steps;
	incJcuello = (KFJirafa[playIndexJ + 1].Jcuello - KFJirafa[playIndexJ].Jcuello) / i_max_steps;
	incJboca = (KFJirafa[playIndexJ + 1].Jboca - KFJirafa[playIndexJ].Jboca) / i_max_steps;

}

void animate(void)
{
	if (playL) // Animacion del lobo
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndexL++;
			if (playIndexL > FILobo - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndexL = 0;
				playL = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolationL();
			}
		}
		else
		{
			//Draw animation
			LposX += incLposX;
			LposY += incLposY;
			LposZ += incLposZ;
			LgiroPFD += incLgiroPFD;
			LgiroPFI += incLgiroPFI;
			LgiroCola += incLgiroCola;
			LgiroCab += incLgiroCab;

			i_curr_steps++;
		}
	}

	if (playJ) //Animacion de la jirafa
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndexJ++;
			if (playIndexJ > FIJirafa - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndexJ = 0;
				playJ = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolationJ();
			}
		}
		else
		{
			//Draw animation
			JposX += incJposX;
			JposY += incJposY;
			JposZ += incJposZ;
			JgiroPFD += incJgiroPFD;
			JgiroPFI += incJgiroPFI;
			JgiroPzFD += incJgiroPzFD;
			JgiroPzFI += incJgiroPzFI;
			JgiroPTD += incJgiroPTD;
			JgiroPTI += incJgiroPTI;
			JgiroCola += incJgiroCola;
			Jcabeza += incJcabeza;
			Jcuello += incJcuello;
			Jboca += incJboca;

			i_curr_steps++;
		}
	}

	////////////////////////////////ANIMACIÓN ACUARIO/////////////////////////////////////
	//--------- ACUARIO ----------------//
	//Tiburon
	if (sharkAnimate) {
		float velocidadGiroCola = 3.0f;
		float velTiburon = 5.0f;

		switch (sharkState) {
		case -1:
			sharkState = 0;
			tbTraslation.x = tbInitX;
			tbTraslation.y = tbInitY;
			tbTraslation.z = tbInitZ;
			rotacionCuerpo = 0;

			cWTraslation = glm::vec3(cWPuntoInicial, 0.0f, tbInitZ + 430);

			break;

		case 0:
			velocidadGiroCola = 10.0f;
			giroCola(velocidadGiroCola);

			childrenState = 0;

			tbTraslation.z += velTiburon;
			if (tbTraslation.z >= tbInitZ + 200) {
				rotacionCuerpo += 90;
				sharkState = 1;
			}

			break;
		case 1:
			velocidadGiroCola = 10.0f;
			giroCola(velocidadGiroCola);

			tbTraslation.x -= velTiburon;
			if (tbTraslation.x <= tbInitX - 200) {
				rotacionCuerpo += 90;
				sharkState = 2;
			}

			break;

		case 2:
			velocidadGiroCola = 10.0f;
			giroCola(velocidadGiroCola);

			tbTraslation.z -= velTiburon;
			if (tbTraslation.z <= tbInitZ) {
				rotacionCuerpo += 90;
				sharkState = 3;
			}
			break;

		case 3:
			velocidadGiroCola = 10.0f;
			giroCola(velocidadGiroCola);

			tbTraslation.x += velTiburon;
			if (tbTraslation.x >= tbInitX) {
				sharkState = 4;
				rotacionCuerpo += 90;
			}
			break;

		case 4:
			velocidadGiroCola = 10.0f;
			giroCola(velocidadGiroCola);

			tbTraslation.z += velTiburon;
			if (tbTraslation.z >= tbInitZ + 200) {
				rotacionCuerpo += 90;
				sharkState = 6;
			}

			break;
		case 5:
			velocidadGiroCola = 10.0f;
			giroCola(velocidadGiroCola);

			tbTraslation.x -= velTiburon;
			if (tbTraslation.x <= tbInitX - 250) {
				rotacionCuerpo += 90;
				sharkState = 6;
			}

			break;


		case 6://va hacia el nino
			velocidadGiroCola = 20.0f;
			giroCola(velocidadGiroCola);

			velocidadTraslacion = 7.0f;
			tbTraslation.x -= velocidadTraslacion;

			if (tbTraslation.x <= limiteNino) {
				sharkState = 7;
			}

			break;

		case 7://giro en su posiscion
			velocidadGiroCola = 20.0f;
			giroCola(velocidadGiroCola);

			childrenState = 1;

			tbTiempoGiroChoque += 1;
			tbRotacionCuerpoChoque += 10;

			if (tbTiempoGiroChoque > 200 && static_cast<int>(tbRotacionCuerpoChoque) % 360 == 0) {
				tbTiempoGiroChoque = 0;
				tbRotacionCuerpoChoque = 0;
				sharkState = 8;
			}

			break;

		case 8://regreso
			velocidadGiroCola = 10.0f;
			giroCola(velocidadGiroCola);

			velocidadTraslacion = 3.0f;
			tbTraslation.x += velocidadTraslacion;

			if (tbTraslation.x >= tbInitX) {
				sharkState = -1;
				tbMov = false;
			}

			break;

		default:
			break;
		}

		if (sharkAnimate) {
			switch (childrenState) {
			case 0:
				cWTraslation.z -= 1.0f;
				cWVisible = true;
				break;
			case 1:
				cWVisible = false;
				break;
			}
		}



		if (movPes) {
			float velPes = 1.0f;
			float velRotate = 20.0f;
			float velGiroCola = 5.0f;

			giroColaPes(velGiroCola);

			switch (statePes) {

			case 0:

				psTraslation.z += velPes;
				if (psTraslation.z >= psInitZ + 100) {
					statePes = 1;
					psRotacionCuerpo = 0;
				}

				break;
			case 1:
				psRotacionCuerpo += 90;
				statePes = 2;
				break;
			case 2:
				psTraslation.x += velPes;
				if (psTraslation.x >= psInitX + 80) {
					statePes = 3;
				}

				break;
			case 3:
				psRotacionCuerpo += 90;
				statePes = 4;
				break;
			case 4:
				psTraslation.z -= velPes;
				if (psTraslation.z <= psInitZ) {
					statePes = 5;
				}
				break;
			case 5:
				psRotacionCuerpo += 90;
				statePes = 6;
				break;
			case 6:
				psTraslation.x -= velPes;
				if (psTraslation.x <= psInitX) {
					statePes = 7;
				}
				break;
			case 7:
				psRotacionCuerpo += 90;
				statePes = 0;
				break;
			default:
				break;
			}
		}

	}



	//////////////////////////////FIN ANIMACIÓN ACUARIO////////////////////////////////////


	////////ANIMACION CARRO///////////

		//Vehículo (se estaciona)
	if (animacion){
		switch (estadoAuto){
			case -1:
				movAuto_x = 300.0f;
				movAuto_z = 1200.0f;
				movAuto_y = 10.0f;
				estadoAuto = 1;
				giroCar = 90;
				animacion = false;
				girollantasLado = 0;
				break;
			case 1:
				giroCar = 0;
				movAuto_z -= 2.0f;
				girollantas += 3.0f;
				girollantasTraceras += 3.0f;

				if (movAuto_z <= 900.0f) {
					estadoAuto = 2;
				}
				break;
			case 2:
				girollantasLado += 1.0f;
				if (girollantasLado >= 45) {
					estadoAuto = 3;
				}
				break;
			case 3:
				giroCar += 1.0f;
				girollantasTraceras += 3.0f;
				if (giroCar >= 90) {
					estadoAuto = 4;
				}
				break;
			case 4:
				girollantasLado -= 1.0f;
				if (girollantasLado <= 0) {
					estadoAuto = 5;
				}
				break;
			case 5:
				movAuto_x += 3.0f;
				girollantas += 3.0f;
				girollantasTraceras += 3.0f;
				if (movAuto_x >= 500) {
					estadoAuto = 0;
				}
				break;
			default:
				break;
		}

	}
	/////FIN ANIMACION CARRO////
}


/////////////METODOS PARA ANIMACION DE ACUARIO//////////////////////

void giroCola(int vel) {
	if (girarColDer) {
		if (giroMonito > 30.0f) {
			girarColDer = !girarColDer;
		}
		else {
			giroMonito += vel;
		}
	}
	else {
		if (giroMonito < -30.0f) {
			girarColDer = !girarColDer;
		}
		else {
			giroMonito -= vel;
		}
	}
}

void giroColaPes(int vel) {
	if (girarColDer) {
		if (psGiroCola > 20.0f) {
			girarColDerPes = !girarColDer;
		}
		else {
			psGiroCola += vel;
		}
	}
	else {
		if (psGiroCola < -20.0f) {
			girarColDerPes = !girarColDer;
		}
		else {
			psGiroCola -= vel;
		}
	}


	if (psSube) {
		if (psTraslation.y >= psInitY + 10.f) {
			psSube = !psSube;
		}
		else {
			psTraslation.y += 0.2f;
		}
	}
	else {
		if (psTraslation.y <= psInitY) {
			psSube = !psSube;
		}
		else {
			psTraslation.y -= 0.2f;
		}
	}

}


///////////FIN METODOS PARA ANIMACION ACUARIO/////////////////////////


void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void music() {
	if (sound) {
		bool played = PlaySound(L"Ambiente.wav", NULL, SND_LOOP | SND_ASYNC);
		sound = false;
	}
}



int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model Escenario("resources/objects/Escenario/Escenario.obj");
	Model Parada("resources/objects/Bus/BusStop.obj");
	Model Basurero("resources/objects/Objetos/classic bin.obj");
	Model Banca("resources/objects/Objetos/classic park bench.obj");
	Model LamparaTwoHead1("resources/objects/Objetos/classic dual light.obj");
	Model LamparaTwoHeadC("resources/objects/Objetos/classic dual light.obj");
	Model RejaIzquieda("resources/objects/RejaEntrada/RejaIzquieda.obj");
	Model RejaDerecha("resources/objects/RejaEntrada/RejaDerecha.obj");
	Model Osito("resources/objects/Osito/Osito.obj");
	Model arbol("resources/objects/arbol/arbol.obj");

	//Modelo Lobo
	Model loboCabeza("resources/objects/Lobo/Lcabeza.obj");
	Model loboCuerpo("resources/objects/Lobo/Lcuerpo.obj");
	Model loboCola("resources/objects/Lobo/Lcola.obj");
	Model loboPataFD("resources/objects/Lobo/Lpatafd.obj");
	Model loboPataFI("resources/objects/Lobo/Lpatafi.obj");
	Model loboPataTD("resources/objects/Lobo/Lpatatd.obj");
	Model loboPataTI("resources/objects/Lobo/Lpatati.obj");

	//Modelo Jirafa
	Model JirCabeza("resources/objects/Jirafa/Jcabeza.obj");
	Model JirBoca("resources/objects/Jirafa/Jboca.obj");
	Model JirCuello("resources/objects/Jirafa/Jcuello.obj");
	Model JirCuerpo("resources/objects/Jirafa/Jcuerpo.obj");
	Model JirCola("resources/objects/Jirafa/Jcola.obj");
	Model JirPataFD("resources/objects/Jirafa/Jpatafd.obj");
	Model JirPataFI("resources/objects/Jirafa/Jpatafi.obj");
	Model JirPezunaFD("resources/objects/Jirafa/Jpezunafd.obj");
	Model JirPezunaFI("resources/objects/Jirafa/Jpezunafi.obj");
	Model JirPataTD("resources/objects/Jirafa/Jpatatd.obj");
	Model JirPataTI("resources/objects/Jirafa/Jpatati.obj");

	//Personas
	Model hombre("resources/objects/Personas/hombre.obj");
	Model mujer("resources/objects/Personas/mujer.obj");
	Model chico("resources/objects/Personas/chico.obj");

	////////////BOCHIDO////////////
	Model Carro("resources/objects/Bocho/Bocho.obj");//Carroceria del bocho
	Model llanta("resources/objects/Bocho/Rueda.obj");//ruedas 
	//////FIN BOCHIDO/////////////

	///////////DECLARO MODELOS PARA ACUARIO//////////////
	//------------------ ACUARIO --------------------//
	Model tbTorzo("resources/objects/TiburonC/tbTorzo.obj");
	Model tbTorzoBajo("resources/objects/TiburonC/tbTorzoBajo.obj");
	Model tbCola("resources/objects/TiburonC/tbCola.obj");
	Model tbMandibula("resources/objects/TiburonC/tbMandibula.obj");

	//--Partes del Pes--//
	Model psCuerpo("resources/objects/Pes/psFTorzo.obj");
	Model psCola("resources/objects/Pes/psFCola.obj");
	Model psAletaDer("resources/objects/Pes/psFAD.obj");
	Model psAletaIzq("resources/objects/Pes/psFAI.obj");

	//Mixamo
	ModelAnim tbNinoAsustado("resources/objects/NinoAsustado/ninoAsustado.dae");
	tbNinoAsustado.initShaders(animShader.ID);
	ModelAnim tbNinoCamina("resources/objects/NinoCamina/ninoCamina.dae");
	tbNinoCamina.initShaders(animShader.ID);
	//----------------------------------------------------------//


	/////////FIN DECLARO MODELOS PARA ACUARIO///////////


	//Inicialización de KeyFrames para la animacion del lobo
	AniLobo.open("Animaciones/AniLobo.txt", ios::in);
	saveFrameL();
	while (indL < MAX_FRAMESL && !AniLobo.eof()) {
		getline(AniLobo, strL);
		LposX = stof(strL);
		getline(AniLobo, strL);
		LposY = stof(strL);
		getline(AniLobo, strL);
		LposZ = stof(strL);
		getline(AniLobo, strL);
		LgiroPFD = stof(strL);
		getline(AniLobo, strL);
		LgiroPFI = stof(strL);
		getline(AniLobo, strL);
		LgiroCola = stof(strL);
		getline(AniLobo, strL);
		LgiroCab = stof(strL);
		saveFrameL();
		indL++;
	}
	AniLobo.close();

	//Inicialización de KeyFrames para la animacion de la jirafa
	AniJirafa.open("Animaciones/AniJirafa.txt", ios::in);
	saveFrameJ();
	while (indJ < MAX_FRAMESJ && !AniJirafa.eof()) {
		getline(AniJirafa, strJ);
		JposX = stof(strJ);
		getline(AniJirafa, strJ);
		JposY = stof(strJ);
		getline(AniJirafa, strJ);
		JposZ = stof(strJ);
		getline(AniJirafa, strJ);
		JgiroPFD = stof(strJ);
		getline(AniJirafa, strJ);
		JgiroPFI = stof(strJ);
		getline(AniJirafa, strJ);
		JgiroPzFD = stof(strJ);
		getline(AniJirafa, strJ);
		JgiroPzFI = stof(strJ);
		getline(AniJirafa, strJ);
		JgiroPTD = stof(strJ);
		getline(AniJirafa, strJ);
		JgiroPTI = stof(strJ);
		getline(AniJirafa, strJ);
		JgiroCola = stof(strJ);
		getline(AniJirafa, strJ);
		Jcabeza = stof(strJ);
		getline(AniJirafa, strJ);
		Jcuello = stof(strJ);
		getline(AniJirafa, strJ);
		Jboca = stof(strJ);
		saveFrameJ();
		indJ++;
	}
	AniJirafa.close();

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();
		// Reproducir música
		music();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// don't forget to enable shader before setting uniforms
		staticShader.use();

		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);




		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

		//Matrices Lobo
		glm::mat4 tmpL = glm::mat4(1.0f);
		glm::mat4 tmpLPFD = glm::mat4(1.0f);
		glm::mat4 tmpLPFI = glm::mat4(1.0f);
		glm::mat4 tmpLPTD = glm::mat4(1.0f);
		glm::mat4 tmpLPTI = glm::mat4(1.0f);
		glm::mat4 tmpLCola = glm::mat4(1.0f);
		glm::mat4 tmpLCab = glm::mat4(1.0f);

		//Matrices Jirafa
		glm::mat4 tmpJ = glm::mat4(1.0f);
		glm::mat4 tmpJPFD = glm::mat4(1.0f);
		glm::mat4 tmpJPFI = glm::mat4(1.0f);
		glm::mat4 tmpJPzFD = glm::mat4(1.0f);
		glm::mat4 tmpJPzFI = glm::mat4(1.0f);
		glm::mat4 tmpJPTD = glm::mat4(1.0f);
		glm::mat4 tmpJPTI = glm::mat4(1.0f);
		glm::mat4 tmpJCola = glm::mat4(1.0f);
		glm::mat4 tmpJCab = glm::mat4(1.0f);
		glm::mat4 tmpJBoca = glm::mat4(1.0f);
		glm::mat4 tmpJCuello = glm::mat4(1.0f);

		// -------------------------------------------------------------------------------------------------------------------------
		// Lobo
		// -------------------------------------------------------------------------------------------------------------------------

		//Cuerpo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 55.0f, -1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmpL = model = glm::translate(model, glm::vec3(LposX, LposY, LposZ));
		model = glm::scale(tmpL, glm::vec3(Esc_Lobo, Esc_Lobo, Esc_Lobo));
		staticShader.setMat4("model", model);
		//loboCuerpo.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmpL, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(LgiroCab), glm::vec3(1.0f, 0.0f, 0.0f));
		tmpLCab = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(Esc_Lobo, Esc_Lobo, Esc_Lobo));
		staticShader.setMat4("model", model);
		//loboCabeza.Draw(staticShader);

		//Pata Frontal Derecha
		model = glm::translate(tmpL, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(LgiroPFD), glm::vec3(1.0f, 0.0f, 0.0f));
		tmpLPFD = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpLPFD, glm::vec3(Esc_Lobo, Esc_Lobo, Esc_Lobo));
		staticShader.setMat4("model", model);
		//loboPataFD.Draw(staticShader);

		//Pata Frontal Izquierda
		model = glm::translate(tmpL, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(LgiroPFI), glm::vec3(1.0f, 0.0f, 0.0f));
		tmpLPFI = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpLPFI, glm::vec3(Esc_Lobo, Esc_Lobo, Esc_Lobo));
		staticShader.setMat4("model", model);
		//loboPataFI.Draw(staticShader);

		//Pata Trasera Derecha
		model = glm::translate(tmpL, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(LgiroPFI), glm::vec3(1.0f, 0.0f, 0.0f));
		tmpLPTD = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpLPTD, glm::vec3(Esc_Lobo, Esc_Lobo, Esc_Lobo));
		staticShader.setMat4("model", model);
		//loboPataTD.Draw(staticShader);

		//Pata Trasera Izquierda
		model = glm::translate(tmpL, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(LgiroPFD), glm::vec3(1.0f, 0.0f, 0.0f));
		tmpLPTI = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpLPTI, glm::vec3(Esc_Lobo, Esc_Lobo, Esc_Lobo));
		staticShader.setMat4("model", model);
		//loboPataTI.Draw(staticShader);

		//Cola
		model = glm::translate(tmpL, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(LgiroCola), glm::vec3(0.0f, 1.0f, 0.0f));
		tmpLCola = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpLCola, glm::vec3(Esc_Lobo, Esc_Lobo, Esc_Lobo));
		staticShader.setMat4("model", model);
		//loboCola.Draw(staticShader);


		////////////////////
			// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(movAuto_x, movAuto_y, movAuto_z));
		model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(giroCar), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		staticShader.setMat4("model", model);
		Carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(15.0f, -2.0f, 30.0f));
		model = glm::rotate(model, glm::radians(girollantas), glm::vec3(1.0f, 0.0f, 0.0f));//giro de las llantas
		model = glm::rotate(model, glm::radians(girollantasLado), glm::vec3(0.0f, 0.0f, 1.0f));//giro de las llantas
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-15.0f, -2.0f, 30.0f));
		model = glm::rotate(model, glm::radians(girollantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(girollantasLado), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-15.0f, -2.0f, -20.0f));
		model = glm::rotate(model, glm::radians(girollantasTraceras), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(15.0f, -2.0f, -20.0f));
		model = glm::rotate(model, glm::radians(girollantasTraceras), glm::vec3(1.0f, 0.0f, 0.0f));//giro de las llantas
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);//Izq trase   


		/////////////////FIN CARRO//////////


		// -------------------------------------------------------------------------------------------------------------------------
		// Jirafa
		// -------------------------------------------------------------------------------------------------------------------------

		//Cuerpo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, 65.0f, 0.0f));
		model = glm::translate(model, glm::vec3(JposX, JposY, JposZ));
		tmpJ = model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(tmpJ, glm::vec3(Esc_Jirafa, Esc_Jirafa, Esc_Jirafa));
		staticShader.setMat4("model", model);
		//JirCuerpo.Draw(staticShader);

		//Cuello
		model = glm::translate(tmpJ, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpJCuello = model = glm::rotate(model, glm::radians(Jcuello), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(tmpJCuello, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpJCuello, glm::vec3(Esc_Jirafa, Esc_Jirafa, Esc_Jirafa));
		staticShader.setMat4("model", model);
		//JirCuello.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmpJCuello, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpJCab = model = glm::rotate(model, glm::radians(Jcabeza), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(tmpJCab, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpJCab, glm::vec3(Esc_Jirafa, Esc_Jirafa, Esc_Jirafa));
		staticShader.setMat4("model", model);
		//JirCabeza.Draw(staticShader);

		//Boca
		model = glm::translate(tmpJCab, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpJBoca = model = glm::rotate(model, glm::radians(Jboca), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(tmpJBoca, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpJBoca, glm::vec3(Esc_Jirafa, Esc_Jirafa, Esc_Jirafa));
		staticShader.setMat4("model", model);
		//JirBoca.Draw(staticShader);

		//Cola
		model = glm::translate(tmpJ, glm::vec3(0.0, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(JgiroCola), glm::vec3(0.0f, 0.0f, 1.0f));
		tmpJCola = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpJCola, glm::vec3(Esc_Jirafa, Esc_Jirafa, Esc_Jirafa));
		staticShader.setMat4("model", model);
		//JirCola.Draw(staticShader);

		//Pata Trasera Derecha
		model = glm::translate(tmpJ, glm::vec3(0.0, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(JgiroPTD), glm::vec3(1.0f, 0.0f, 0.0f));
		tmpJPTD = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpJPTD, glm::vec3(Esc_Jirafa, Esc_Jirafa, Esc_Jirafa));
		staticShader.setMat4("model", model);
		//JirPataTD.Draw(staticShader);

		//Pata Trasera Izquierda
		model = glm::translate(tmpJ, glm::vec3(0.0, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(JgiroPTI), glm::vec3(1.0f, 0.0f, 0.0f));
		tmpJPTI = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpJPTI, glm::vec3(Esc_Jirafa, Esc_Jirafa, Esc_Jirafa));
		staticShader.setMat4("model", model);
		//JirPataTI.Draw(staticShader);

		//Pata Frontal Derecha
		model = glm::translate(tmpJ, glm::vec3(0.0, 0.0f, 0.0f));
		tmpJPFD = model = glm::rotate(model, glm::radians(JgiroPFD), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpJPFD, glm::vec3(Esc_Jirafa, Esc_Jirafa, Esc_Jirafa));
		staticShader.setMat4("model", model);
		//JirPataFD.Draw(staticShader);

		//Pata Frontal Izquierda
		model = glm::translate(tmpJ, glm::vec3(0.0, 0.0f, 0.0f));
		tmpJPFI = model = glm::rotate(model, glm::radians(JgiroPFI), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpJPFI, glm::vec3(Esc_Jirafa, Esc_Jirafa, Esc_Jirafa));
		staticShader.setMat4("model", model);
		//JirPataFI.Draw(staticShader);

		//Pata Pezuña Derecha
		model = glm::translate(tmpJPFD, glm::vec3(0.0, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(JgiroPzFD), glm::vec3(1.0f, 0.0f, 0.0f));
		tmpJPzFD = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpJPzFD, glm::vec3(Esc_Jirafa, Esc_Jirafa, Esc_Jirafa));
		staticShader.setMat4("model", model);
		//JirPezunaFD.Draw(staticShader);

		//Pata Pezuña Izquierda
		model = glm::translate(tmpJPFI, glm::vec3(0.0, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(JgiroPzFI), glm::vec3(1.0f, 0.0f, 0.0f));
		tmpJPzFI = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(tmpJPzFI, glm::vec3(Esc_Jirafa, Esc_Jirafa, Esc_Jirafa));
		staticShader.setMat4("model", model);
		//JirPezunaFI.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personas
		// -------------------------------------------------------------------------------------------------------------------------

		//Hombre
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 5.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//hombre.Draw(staticShader);

		//Mujer
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(280.0f, 5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//mujer.Draw(staticShader);

		//Chico
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(110.0f, 5.0f, -950.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//chico.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);



		// -------------------------------------------------------------------------------------------------------------------------
		// Chilren Walk
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), cWTraslation);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); //ya no escalamos
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);

		if (sharkAnimate) {
			if (cWVisible) {
				//tbNinoCamina.Draw(animShader);
			}
		}


		// -------------------------------------------------------------------------------------------------------------------------
		// Chilren Screem
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(cWPuntoInicial, 0.0f, tbInitZ + 200));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); //ya no escalamos
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);

		if (sharkAnimate) {
			if (!cWVisible) {
				//tbNinoAsustado.Draw(animShader);
			}
		}



		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-100.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-100.0f, -1.75f, -950.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(850.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(850.0f, -1.75f, -950.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
		

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		Escenario.Draw(staticShader);

		// Arbol
		model = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, 55.0f, -100.0f));
		model = glm::scale(model, glm::vec3(60.0f));
		staticShader.setMat4("model", model);
		//arbol.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, rejaIzquierdaPosZ));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		RejaIzquieda.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, rejaDerechaPosZ));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		RejaDerecha.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(230.0f, 0.75f, -90.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		LamparaTwoHead1.Draw(staticShader);

		//Centro!!!!! 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(230.0f, 0.75f, -350.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		LamparaTwoHeadC.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(230.0f, 0.75f, -650.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		LamparaTwoHead1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(230.0f, 0.75f, -995.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		LamparaTwoHead1.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, -50.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//Osito.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, -10.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		Parada.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(765.0f, 0.75f, -420.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		LamparaTwoHead1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(765.0f, 0.75f, -220.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		LamparaTwoHead1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(980.0f, 0.75f, -220.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		LamparaTwoHead1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(980.0f, 0.75f, -420.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		LamparaTwoHead1.Draw(staticShader);

		//últimos faros
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(765.0f, 0.75f, -960.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		LamparaTwoHead1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(980.0f, 0.75f, -960.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		LamparaTwoHead1.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------


		//---------------- ACUARIO --------------------------//
	// -------------------------------------------------------------------------------------------------------------------------
// Tiburon en cachos
// -------------------------------------------------------------------------------------------------------------------------
		glm::mat4 tmp1 = glm::mat4(1.0f);

		tmp = model = glm::translate(glm::mat4(1.0f), glm::vec3(tbTraslation));
		tmp = glm::rotate(tmp, glm::radians(-rotacionCuerpo), glm::vec3(0.0f, 1.0f, 0.0));
		tmp = glm::scale(tmp, glm::vec3(4.0f, 4.0f, 4.0f));
		tmp = tmp1 = glm::rotate(tmp, glm::radians(-tbRotacionCuerpoChoque), glm::vec3(0.0f, 0.0f, 1.0));
		staticShader.setMat4("model", tmp);
		//tbTorzo.Draw(staticShader);

		//torzoBajo
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		tmp = glm::rotate(tmp, glm::radians(giroMonito), glm::vec3(0.0f, 1.0, 0.0));
		staticShader.setMat4("model", tmp);
		//tbTorzoBajo.Draw(staticShader);

		//Cola
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroMonito / 2), glm::vec3(0.0f, 1.0, 0.0));
		staticShader.setMat4("model", model);
		//tbCola.Draw(staticShader);

		//Mandibula
		tmp = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", tmp);
		//tbMandibula.Draw(staticShader);
		////////////////////////////////////////////////////////////////////////////////////////////////

		// -----------------------------------------------------------------------------------------------------
		// Pes
		// ----------------------------------------------------------------------------------------------------
		//torzo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 15.0f, 0.0f));
		model = glm::translate(glm::mat4(1.0f), psTraslation);
		model = glm::translate(model, psTraslation);
		tmp1 = tmp = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		tmp = tmp1 = glm::rotate(tmp, glm::radians(psRotacionCuerpo), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", tmp);
		//psCuerpo.Draw(staticShader);

		//cola
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		tmp = glm::rotate(tmp, glm::radians(psGiroCola), glm::vec3(0.0f, 1.0, 0.0));
		staticShader.setMat4("model", tmp);
		//psCola.Draw(staticShader);

		//aleta der
		model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(psGiroAleta), glm::vec3(0.0f, 1.0, 0.0));
		staticShader.setMat4("model", model);
		//psAletaDer.Draw(staticShader);

		//aleta izq
		model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(psGiroAleta), glm::vec3(0.0f, 1.0, 0.0));
		staticShader.setMat4("model", model);
		//psAletaIzq.Draw(staticShader);

		///////TERMINA ACUARIO//////////////

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//Carro animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		animacion ^= true;
		if (estadoAuto == 0) {
			estadoAuto = -1;
		}
	}


	//--------------- ACUARIO  ----------------------//
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		sharkAnimate = !sharkAnimate;
		movPes = !movPes;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		tbTraslation.x = tbInitX;
		tbTraslation.y = tbInitY;
		tbTraslation.z = tbInitZ;
		sharkState = -1;

	}
	//To Configure Model
	// Verificar si se ha presionado la tecla "o"
	
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		// Invertir el estado de las rejas
		rejasMovidas = !rejasMovidas;

		// Si las rejas se han movido, actualizar las posiciones
		if (rejasMovidas)
		{
			// Mover RejaDerecha 80 unidades en z negativa
			rejaDerechaPosZ -= 80.0f;

			// Mover RejaIzquierda 80 unidades en z positiva
			rejaIzquierdaPosZ += 80.0f;
		}
		else
		{
			// Volver las rejas a su posición original
			rejaDerechaPosZ += 80.0f;
			rejaIzquierdaPosZ -= 80.0f;
		}
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (playL == false && (FILobo > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetL();
			//First Interpolation				
			interpolationL();

			playL = true;
			playIndexL = 0;
			i_curr_steps = 0;
		}
		else
		{
			playL = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		if (playJ == false && (FIJirafa > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetJ();
			//First Interpolation				
			interpolationJ();

			playJ = true;
			playIndexJ = 0;
			i_curr_steps = 0;
		}
		else
		{
			playJ = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}
}

	//To play KeyFrame animation 




// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}