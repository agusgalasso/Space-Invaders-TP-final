#include <iostream>
#include <conio2.h>
#include <windows.h>
using namespace std;

#define MAX_BALAS 5
#define MAX_ENEMIGOS 12
#define ANCHO 40
#define ALTO 20


class Entidad {
protected:
	int x, y;
	char simbolo;
public:
	Entidad(int _x=0, int _y=0, char _s='?') {
		x=_x; y=_y; simbolo=_s;
	}
	int tenerX(){ return x; }
	int tenerY(){ return y; }
	char tenerSimbolo(){ return simbolo; }
	void ponerPos(int _x,int _y){ x=_x; y=_y; }
};


class Nave : public Entidad {
private:
	int vidas;
public:
	Nave(int _x,int _y) : Entidad(_x,_y,'A') {
		vidas=3;
	}
	void mover(char tecla){
		if(tecla=='a' && x>1) x--;
		if(tecla=='d' && x<ANCHO-2) x++;
	}
		int tenerVidas(){ return vidas; }
		void perderVida(){ vidas--; }
};


class Enemigo : public Entidad {
public:
	bool vivo;
	int fila;
	Enemigo(){ vivo=false; fila=0; }
	Enemigo(int _x,int _y,int _fila) : Entidad(_x,_y,'M') {
		vivo=true;
		fila=_fila;
	}
};


class Bala : public Entidad {
private:
	bool activa;
	int dir;
public:
	Bala(){ activa=false; dir=-1; simbolo='|'; }
	void disparar(int _x,int _y,int _dir){
		x=_x; y=_y; dir=_dir; activa=true;
	}
		void mover(){
			if(activa){
				y+=dir;
				if(y<0) activa=false;
			}
		}
			bool estaActiva(){ return activa; }
			void desactivar(){ activa=false; }
};


class Juego {
private:
	Nave jugador;
	Enemigo enemigos[MAX_ENEMIGOS];
	Bala balas[MAX_BALAS];
	int numEnemigos;
	int direccionEnemigos;
public:
	Juego() : jugador(ANCHO/2,ALTO-2) {
		numEnemigos=MAX_ENEMIGOS;
		direccionEnemigos=1;
		
		int fila = 0;
		for(int i=0;i<MAX_ENEMIGOS;i++){
			int col = i % 4;
			if(i>0 && i%4==0) fila++;
			enemigos[i]=Enemigo(4+col*7, 2+fila*2, fila);
		}
	}

	
//Hasta acá fue lo primero que hice, puse las clases 
	
	
	void dibujar(){
		clrscr();
		
		textcolor(GREEN);
		gotoxy(jugador.tenerX(), jugador.tenerY());
		cout<<jugador.tenerSimbolo();
		
		for(int i=0;i<MAX_ENEMIGOS;i++){
			if(enemigos[i].vivo){
				switch(enemigos[i].fila){
				case 0: textcolor(RED); break;
				case 1: textcolor(MAGENTA); break;
				case 2: textcolor(BLUE); break;
				}
				gotoxy(enemigos[i].tenerX(), enemigos[i].tenerY());
				cout<<enemigos[i].tenerSimbolo();
			}
		}
		
		textcolor(YELLOW);
		for(int i=0;i<MAX_BALAS;i++){
			if(balas[i].estaActiva()){
				gotoxy(balas[i].tenerX(), balas[i].tenerY());
				cout<<balas[i].tenerSimbolo();
			}
		}
		
		textcolor(WHITE);
		gotoxy(1,ALTO);
		cout<<"Vidas: "<<jugador.tenerVidas()<<"   Enemigos restantes: "<<numEnemigos<<"   ";
	}


//Hasta acá es la segunda parte de lo que hice, puse algunos colores, mientras escribo esto, todos los enemigos son rojos, si llego a
//cambiarlo más adelante lo voy a especificar al final


		
		void loop(){
			char tecla=0;
			while(jugador.tenerVidas()>0 && numEnemigos>0){
				if(kbhit()){
					tecla=getch();
					jugador.mover(tecla);
					if(tecla==' '){
						for(int i=0;i<MAX_BALAS;i++){
							if(!balas[i].estaActiva()){
								balas[i].disparar(jugador.tenerX(),jugador.tenerY()-1,-1);
								break;
							}
						}
					}
				}
				
				for(int i=0;i<MAX_BALAS;i++) balas[i].mover();
				
				for(int i=0;i<MAX_ENEMIGOS;i++){
					if(enemigos[i].vivo) enemigos[i].ponerPos(enemigos[i].tenerX()+direccionEnemigos, enemigos[i].tenerY());
				}
				for(int i=0;i<MAX_ENEMIGOS;i++){
					if(enemigos[i].vivo){
						if(enemigos[i].tenerX()<=1 || enemigos[i].tenerX()>=ANCHO-1){
							direccionEnemigos*=-1;
							for(int j=0;j<MAX_ENEMIGOS;j++){
								enemigos[j].ponerPos(enemigos[j].tenerX(),enemigos[j].tenerY()+1);
								if(enemigos[j].tenerY()>=jugador.tenerY()){
									jugador.perderVida();
								}
							}
							break;
						}
					}
				}

//Hastá acá es lo tercero que hice, empecé algunos de los comandos if y for (y un while)


				for(int i=0;i<MAX_BALAS;i++){
					if(balas[i].estaActiva()){
						for(int j=0;j<MAX_ENEMIGOS;j++){
							if(enemigos[j].vivo &&
							   balas[i].tenerX()==enemigos[j].tenerX() &&
							   balas[i].tenerY()==enemigos[j].tenerY()){
								enemigos[j].vivo=false;
								balas[i].desactivar();
								numEnemigos--;
							}
						}
					}
				}
				
				dibujar();
				Sleep(100);
			}
			
			clrscr();
			gotoxy(ANCHO/2-5,ALTO/2);
			textcolor(LIGHTGREEN);
			if(jugador.tenerVidas()>0) cout<<"GANASTE!";
			else cout<<"PERDISTE...";
		}
};

//Acá terminé lo que me restaba el código
//Al final sí cambié los colores de los enemigos

int main(){
	Juego juego;
	juego.loop();
	return 0;
}
