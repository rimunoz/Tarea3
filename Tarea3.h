//DECLARE STRUCTURES
struct container{ int w; int h; int p; int s; };

struct particle{
 // 4 estados   4 direcciones
 int  *x;
 int  *y;
 char *s;
 int  *d;
};

//Synonyms for data
typedef struct container Container;
typedef struct particle Particle;

void printContainer(Container C);
void printParticula(Particle P, int size);
void imprimirContainer(Container C, Particle P, int iter);
char getElement(int i, int j, Particle P,  Container C);
struct particle  generarParticulas(Container C);
void leerArgumentosEntrada(struct container *C , int argc , char *argv[]);
int moveDuplicates(Container C, Particle P);
void checkDuplicates(Container C, Particle P);
struct particle obtenerDireccion(Container C, Particle P);
void moverParticulas(Container C, Particle P, Particle P_next);
void comprobarColisiones(Container C, Particle P);
void desbloquearParticulas(Container C, Particle P);