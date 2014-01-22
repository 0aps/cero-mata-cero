#include <iostream>
#include <windows.h>

/*
    Cero mata cero de nxn (sin conio.h :D)
    Tenía un año sin programar nada, así estoy qué deprimente.
    Me tomó todo el día hacer el código y aún así tiene un montón de bugs y
    cosas por arreglar, como las obstrucciones con las diagonales, y el
    encerrado de un jugador. Además es demasiado ineficiente y los métodos
    de las clases pueden implementarse mejor.

    Algún día, lo terminaré.

    @aps
    /19/01/2014

*/


using namespace std;

class jugador{

//private: para ver como no puedo acceder desde la funcion de la otra clase :d
    public:

    int x, y, apos_x, apos_y;
    char s;

        jugador(int a, int b, char c): x(a), y(b), s(c) {};
};

class plantilla{

    int p1, p2, n, **cuadricula;

    public:
        plantilla(int, jugador, jugador);

        void dibujar(jugador, jugador);
        int mover(jugador*);
        int verificar(int);

        void cargar(int);
        void limpiar();
        void goxy(int, int);
        int GetCh();

};

int plantilla::GetCh ()
{
	INPUT_RECORD irInputRecord;
    DWORD dwEventsRead, modo;
	HANDLE ih = GetStdHandle( STD_INPUT_HANDLE );

	// Desactivar la escritura
	SetConsoleMode ( ih, modo & ~( ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT ) );

    ReadConsoleInputA (ih, &irInputRecord, 1, &dwEventsRead);
    ReadConsoleInputA (ih, &irInputRecord , 1, &dwEventsRead);

	SetConsoleMode ( ih, modo ); // Volver al estado anterior

	return irInputRecord.Event.KeyEvent.wVirtualKeyCode;
}

void plantilla::goxy(int x, int y)
{

        COORD coord;
        coord.X = x; coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void plantilla::limpiar()
{
    HANDLE hOut = GetStdHandle ( STD_OUTPUT_HANDLE );
    COORD coord = { 0, 0 };
    DWORD dwcount;
    CONSOLE_SCREEN_BUFFER_INFO csb_i;
    GetConsoleScreenBufferInfo ( hOut, &csb_i );
    FillConsoleOutputCharacter ( hOut, ' ',
        csb_i.dwSize.X * csb_i.dwSize.Y, coord, &dwcount );
    SetConsoleCursorPosition ( hOut, coord );
}

void plantilla::cargar(int x)
{
    HANDLE hOut;
    hOut=GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT DisplayArea = {0, 0, 0, 0};
    DisplayArea.Bottom = 1;
    DisplayArea.Right = x;
    SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);

    cout << "\t\tCargando ..." << endl;

        //Barra de carga
    for(int i=0;i<=x;i++)
    {
        cout << "Û";
        Sleep(250);
    }
    limpiar();

    //Volver al estado anterior
    DisplayArea.Bottom = 20;
    DisplayArea.Right  = 50;
    SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);
}

// 220 y 284 son numeros amigos :D
plantilla::plantilla(int a, jugador p1, jugador p2) : n(a) {

       //filas //columnas
    int p(a+2),q(2*a+1);

    cuadricula = new int*[p];
        for(int i=0; i< p; i++)
             cuadricula[i] = new int[q];

    for(int i=0; i < p; i++)
            for(int j = 0; j < q; j++)
                            cuadricula[i][j] = 0;

    //linea horizontal superior
    for(int i=0; i < q; i++)
            cuadricula[0][i] = 220;

    //linea horizontal inferior
    for(int i=0; i < q; i++)
            cuadricula[p-1][i] = 220;

    //linea vertical izquierda
    for(int i=0; i < p; i++)
            cuadricula[i][0] = 220;

    //linea vertical derecha
    for(int i=0; i < p; i++)
            cuadricula[i][q-1] = 220;

    //lineas verticales paralelas
        for(int i=1; i<=a; i++)
                for(int j=2; j<= q-2; j+=2)
                    cuadricula[i][j] = 284;

    //posicion inicial del jugador 1
        cuadricula[p1.x][p1.y] = int(p1.s);

    //posicion inicial del jugador 2
        cuadricula[p2.x][p2.y] = int(p2.s);

}

void plantilla::dibujar(jugador p1, jugador p2)
{
      int p(n+2), q(2*n+1);

      goxy(0,0);
      for(int i=0; i <p; i++)
        {
            for(int j=0; j < q; j++)
            {
                if(cuadricula[i][j] == 220)
                    cout << "#";
                else if(cuadricula[i][j] == 284)
                    cout << "|";
                else if(cuadricula[i][j] == int(p1.s) || cuadricula[i][j] == int(p1.s)+1)
                   cout << p1.s;
                else if(cuadricula[i][j] == int(p2.s) || cuadricula[i][j] == int(p2.s)+1)
                    cout << p2.s;
                else
                    cout << " ";
            }
            cout << endl;
        }

}

int plantilla::mover(jugador *pl)
{
        int ch = GetCh();

    	//Arriba
    	if (ch == VK_UP)
    	{
			pl->apos_x = pl->x - 1;
			if (!cuadricula[pl->apos_x][pl->y])              //si la anterior esta libre
			{                                                //y en la que estyo tambien, pongo en la que estoy libre
                if(cuadricula[pl->x][pl->y] != int(pl->s)+1) //actualizo la nueva posicion
			    	cuadricula[pl->x][pl->y] = 0;            //la coloco en la cuadricula

				pl->x = pl->apos_x;
				cuadricula[pl->apos_x][pl->y]=int(pl->s);
			}
		}

		 //Abajo
		if (ch == VK_DOWN)
		{
            pl->apos_x = pl->x + 1;
			if (!cuadricula[pl->apos_x][pl->y])
			{
				if(cuadricula[pl->x][pl->y] != int(pl->s)+1)
			    	cuadricula[pl->x][pl->y] = 0;

				    pl->x = pl->apos_x;
				    cuadricula[pl->apos_x][pl->y]= int(pl->s);

			}
        }


		//Izquierda
		if (ch == VK_LEFT)
		{
			pl->apos_y = pl->y - 2;
			if (!cuadricula[pl->x][pl->apos_y])
			{
			    if(cuadricula[pl->x][pl->y] != int(pl->s)+1)
			    	cuadricula[pl->x][pl->y] = 0;

				pl->y = pl->apos_y;
				cuadricula[pl->x][pl->y] = int(pl->s);
			}
		}

		//Derecha
		if (ch == VK_RIGHT)
		{
			pl->apos_y = pl->y + 2;
             if (!cuadricula[pl->x][pl->apos_y])
			{
                if(cuadricula[pl->x][pl->y] != int(pl->s)+1)
			    	cuadricula[pl->x][pl->y] = 0;

                pl->y = pl->apos_y;
				cuadricula[pl->x][pl->y] = int(pl->s);
			}
		}


		if( ch == VK_RETURN)
		{
        	cuadricula[pl->x][pl->y] = int(pl->s)+1;
        }

        return ch;
}

int plantilla::verificar(int s)
{
    int x, ganar(0);

    //vertical
    for(int i=1; i <= n; i++)
        {
            x = 2*(i)-1;
            for(int j=1; j<=n; j++)
                if(cuadricula[j][x] == s || cuadricula[j][x] == s+1)
                            ganar ++;
            if( ganar == n)
                    return 0;

            ganar = 0;
        }

    //horizontal
    for(int i=1; i <= n; i++)
        {
            for(int j=1; j<=n; j++)
            {
                x = 2*(j)-1;
                if(cuadricula[i][x] == s || cuadricula[i][x] == s+1)
                          ganar ++;
            }

            if( ganar == n)
                    return 0;

            ganar = 0;
        }

    //Diagonal D
    for(int i=1, j=1; i <= n; i++,j++)
        {
                x = 2*(i)-1;
                if(cuadricula[j][x] == s || cuadricula[j][x] == s+1)
                          ganar ++;

            if( ganar == n)
                    return 0;

        }
     ganar = 0;

    //Diagonal I
    for(int i=1, j=1; i <= n; i++,j++)
        {
                x = -2*(j)+2*n+1;
                if(cuadricula[j][x] == s || cuadricula[j][x] == s+1)
                          ganar ++;

            if( ganar == n)
                    return 0;

        }


    return 1;
}


int main()
{
    int t;
    cin >> t;

    jugador *p1 = new jugador(2,3,'x');
    jugador *p2 = new jugador(1,1, 'o');

    plantilla m3x3(t, *p1, *p2);

    m3x3.cargar(50);

    while(!GetAsyncKeyState(VK_ESCAPE))
    {
               while( m3x3.mover(p1) != VK_RETURN)
               {
                   m3x3.dibujar(*p1, *p2);

                   cout << "\n\n\nJugador: " << p1->s << endl;
                   cout << "x: " << p1->x << "\ny: " << p1->y << endl;

               }

               if( !m3x3.verificar(int(p1->s)) )
                {
                    cout << "\n\n" << p1->s << " gano. " << endl;
                    break;
                }

               while( m3x3.mover(p2) != VK_RETURN)
               {
                    m3x3.dibujar(*p1, *p2);

                    cout << "\n\n\nJugador: " << p2->s << endl;
                    cout << "x: " << p2->x << "\ny: " << p2->y << endl;
               }

               if( !m3x3.verificar(int(p2->s)) )
                {
                    cout << "\n\n" << p2->s << " gano. " << endl;
                    break;
                }

    }

    cin.get();
    return 0;
}
