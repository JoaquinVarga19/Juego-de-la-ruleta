//---------------------------------------------JUEGO DE LA RULETA AMERICANA----------------------------------------------------------------
//--------------------------------------------NOMBRE Y APELLIDO: JOAQUIN VARGA (UNICO INTEGRANTE)-----------------------------------------------------------------------------------------------------
//--------------------------------------------ABAJO DEL TODO HAY DETALLES EXTRA------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#define MAX_JUGADORES 5 //ranking de los cinco mejores jugadores



//struct de las caracteristicas del jugador
typedef struct {
    char nombre[30];       // Nombre del jugador.
    int saldo;             // Saldo disponible.
    int apuesta;           // Cantidad apostada.
    int tipo_apuesta;      // Tipo de apuesta (1=Pleno, 2=Color, etc.).
    int numero_apostado;   // En caso de Pleno, el numero apostado.
    char color_apostado[6];// "rojo" o "negro" en caso de apuesta de color.
    bool es_par;           // True para pares, False para impares (par/impar).
    bool es_impar;         // True si el numero ganador es impar.-----------------------
    bool es_falta;         // True para falta, False para pasa (1-18 o 19-36).
    bool es_pasa;          // True si el numero ganador esta en 19-36.-----------------------
    int docena;            // Primera (1), Segunda (2) o Tercera (3) docena.
    int columna;           // Primera (1), Segunda (2) o Tercera (3) columna.
} Jugador;



//struct de las caracteristicas de la ruleta cuando se juega
typedef struct {
    int numero_ganador;    // Número que salió en la ruleta.
    char color_ganador[6]; // "rojo" o "negro".
    bool es_par;           // True si el número ganador es par.
    bool es_impar;         // True si el numero ganador es impar.------------
    bool es_falta;         // True si el número ganador está en 1-18.
    bool es_pasa;          // True si el numero ganador esta en 19-36.--------
    int docena_ganadora;   // Docena correspondiente al número ganador.
    int columna_ganadora;  // Columna correspondiente al número ganador.
} ResultadoRuleta;



//variables globales para el ranking de los jugadores
//Jugador jugadores[MAX_JUGADORES];
Jugador jugadores[100];
int num_jugadores = 0;



//prototipos de funciones
void ImprimirConsigna();
void ImprimirRuleta();
void RuletaClasica();
void MostrarMenuOpciones();
void Apuestas();
void SaludarJugador(Jugador *jugador);
void GenerarNumeroGanador(ResultadoRuleta *resultado); //seria como girar ruleta y verificar donde toco si color o numero que aposte
void VerificarApuesta(Jugador *jugador, ResultadoRuleta *resultado);
void GestionarDinero(Jugador *jugador, ResultadoRuleta *resultado);
int comparar_por_saldo(const void *a, const void *b);
void actualizar_ranking(Jugador *jugador); 
void MostrarRanking(Jugador jugadores[], int num_jugadores);



//funcion para saludar al jugador por su nombre
void SaludarJugador(Jugador *jugador) {
    printf("¡Hola %s, Bienvenido!\n", jugador->nombre);
    printf("\n");
}



//funcion para girar la ruleta
void GenerarNumeroGanador(ResultadoRuleta *resultado){

    //verificar para los plenos
    resultado->numero_ganador = rand() % 37; //numero ganador

    //ahora verificamos el color (negro, rojo, o verde si es 0)
    if (resultado->numero_ganador == 0) //verde
    {
        strcpy(resultado->color_ganador, "verde");
    }
    else if (resultado->numero_ganador % 2 == 0) //negro
    { 
        strcpy(resultado->color_ganador, "negro");
    }
    else{ //rojo
        strcpy(resultado->color_ganador, "rojo");
    }
    
    //ahora verificamos por los pares/impares
    resultado->es_par = (resultado->numero_ganador % 2 == 0); //si el numero al dividirlo por 2 da resto 0 es par
    resultado->es_impar = (resultado->numero_ganador % 2 == 1); //viceversa

    //ahora verificamos para el tipo de apuesta falta/pasa
    resultado->es_falta = (resultado->numero_ganador >= 1 && resultado->numero_ganador <= 18); //falta
    resultado->es_pasa = (resultado->numero_ganador >= 19 && resultado->numero_ganador <= 36); //pasa

    //verificamos para las docenas 1,2,3
    if (resultado->numero_ganador >= 1 && resultado->numero_ganador <= 12) //primer docena
    {
        resultado->docena_ganadora = 1; //primer docena 1-12
    }
    else if (resultado->numero_ganador >= 13 && resultado->numero_ganador <= 24) //segunda docena
    {
        resultado->docena_ganadora = 2; //segunda docena 13-24
    }
    else if (resultado->numero_ganador >= 25 && resultado->numero_ganador <= 36) //tercera docena
    {
        resultado->docena_ganadora = 3; //tercera docena 25-36
    }
    else {
        resultado->docena_ganadora = -1; //en el caso que toque 0 (verde)
    }

    //verificamos si se apuesta por columnas (1era, 2da y 3era)
    int mod = resultado->numero_ganador % 3; //esto se debe a que los numeros por columna van de 3 en 3
    if (mod == 1)
    {
        resultado->columna_ganadora = 1; //primer columna 1,4,7,10,13,16,19,22,25,28,31,34
    }
    else if (mod == 2)
    {
        resultado->columna_ganadora = 2; //segunda columna 2,5,8,11,14,17,20,23,26,29,32,35
    }
    else if (mod == 0 && resultado->numero_ganador != 0) //tercera columna
    {
        resultado->columna_ganadora = 3;  //tercera columna 3,6,9,12,15,18,21,24,27,30,33,36
    }
    else {
        resultado->columna_ganadora = -1; //por si el numero es 0 (verde)
    }
}



//funcion para jugar a la ruleta y elegir el tipo de juego
void VerificarApuesta(Jugador *jugador, ResultadoRuleta *resultado){

    //imprimir el tipo de apuestas
    Apuestas();
    printf("Ingrese una opcion de apuesta: ");
    scanf("%d", &jugador->tipo_apuesta); // Leer el tipo de apuesta

    //opciones de apuestas
    switch (jugador->tipo_apuesta)
    {
        //apostar pleno
    case 1:

    printf("Elegiste apostar pleno\n");
    //preguntar al usuario a que numero quiere apostar
    printf("Ingrese un numero del 0-36\n");
    scanf("%d", &jugador->numero_apostado);

    //validar que el usuario haya ingresado un numero correspondiente
    while (jugador->numero_apostado < 0 || jugador->numero_apostado > 36)
    {
        printf("Numero incorrecto!!. Ingrese un numero entre 0-36\n");
        scanf("%d", &jugador->numero_apostado);
    }

    //verificar si el jugador gano o perdio
    if (jugador->numero_apostado == resultado->numero_ganador)
    {
        printf("Felicidades, multiplicas tu saldo x35!!. el numero es %d\n", resultado->numero_ganador);
        jugador->saldo += jugador->apuesta * 35; //se multiplica el saldo x35 al apostar pleno
    }
    else {
        printf("Perdiste. salio %d!!\n", resultado->numero_ganador);
        jugador->saldo -= jugador->apuesta; //se resta lo apostado
    }
    break;
    
    //apostar color
    case 2:

    printf("Elegiste apostar por color.\n");
    //preguntar al usuario si quiere apostar negro o rojo
    printf("Ingrese el color (rojo o negro): ");
    scanf("%5s", jugador->color_apostado);


    if (strcmp(jugador->color_apostado, resultado->color_ganador) == 0) //compara el color apostado con el ganador
    {
        printf("Felicidades, se multiplica tu apostado x2 y salio %d!!\n", resultado->numero_ganador);
        jugador->saldo += jugador->apuesta * 2; //multiplicar apostado x2
    }
    else {
        printf("Perdiste!!. salio %d\n", resultado->numero_ganador);
        jugador->saldo -= jugador->apuesta;  //se resta el saldo con la apuesta
    }
    break;
    
    //apostar par/impar
    case 3:

    printf("Elegiste apostar par/impar.\n");
    //preguntar al usuario si quiere apostar par o impar
    printf("Ingrese si quiere apostar 'par' o 'impar': ");
    char apuesta_par_impar[6];
    scanf("%s", apuesta_par_impar);

    //convertimos la entrada del jugador para evitar mayusculas
    for (int i = 0; i < apuesta_par_impar[i] != '\0'; i++)
    {
        apuesta_par_impar[i] == tolower(apuesta_par_impar[i]); //evitar mayusculas
    }

    //verificamos si la apuesta es 'par' o 'impar'
    if (strcmp(apuesta_par_impar, "par") == 0)
    {
        jugador->es_par = true;
        jugador->es_impar = false;
    }
    else if (strcmp(apuesta_par_impar, "impar") == 0)
    {
        jugador->es_par = false;
        jugador->es_impar = true;
    }
    else
    {
    printf("Apuesta invalida. Porfavor ingrese PAR o IMPAR correctamente\n");
    break;
    }

    //verificamos las condiciones por si sale par o impar
    if (jugador->es_par && resultado->es_par || jugador->es_impar && resultado->es_impar) //si se cumple que el numero al que se aposto y que toca es impar o par
    {
        printf("Felicidades, se multiplica tu apostado x2. el numero es: %d!!\n", resultado->numero_ganador);
        jugador->saldo += jugador->apuesta * 2; //multiplicar apostado x2
    }
    else {
        printf("Perdiste. salio %d!!\n", resultado->numero_ganador);
        jugador->saldo -= jugador->apuesta; 
    }
    break;

    //Falta o pasa 1-18 o 19-36
    case 4:

    printf("Elegiste jugar Falta-pasa\n");
    printf("Ingrese una opcion entre falta (1-18) o pasa (19-36): ");
    char falta_o_pasa[6];
    scanf("%s", falta_o_pasa);

    //convertimos las entradas para evitar mayusculas
    for (int i = 0; i < falta_o_pasa[i] != '\0' ; i++)
    {
        falta_o_pasa[i] == tolower(falta_o_pasa[i]);
    }

    //ahora vefificamos que el usuario ingrese bien por pantalla "FALTA" o "PASA"
    if (strcmp(falta_o_pasa, "falta") == 0)
    {
        jugador->es_falta = true;
        jugador->es_pasa = false;
    }
    else if (strcmp(falta_o_pasa, "pasa") == 0)
    {
        jugador->es_pasa = true;
        jugador->es_falta = false;
    }
    else{
        printf("Apuesta invalida. Porfavor ingrese FALTA o PASA correctamente\n");
    }

    //verificamos las condiciones por si sale falta primero
    if (jugador->es_falta && resultado->numero_ganador >= 1 && resultado->numero_ganador <= 18)
    {
        printf("Felicidades, ganaste por FALTA!!. Se multiplica tu apostado x2. El numero es: %d", resultado->numero_ganador);
        jugador->saldo += jugador->apuesta * 2; //se multiplica el saldo x2
    }
    //ahora verificamos por segundo si sale pasa
    else if (jugador->es_pasa && resultado->numero_ganador >= 19 && resultado->numero_ganador <= 36)
    {
        printf("Felicidades, ganaste por PASA!!. Se multiplica tu apostado x2. El numero es: %d", resultado->numero_ganador);
        jugador->saldo += jugador->apuesta * 2; //se multiplica el saldo x2
    }
    //por si sale FALTA o PASA segun lo que haya apostado
    else {
       printf("Perdiste!!. Toco el numero %d", resultado->numero_ganador);
       jugador->saldo -= jugador->apuesta; //se resta saldo - apostado
    }
    break;

    //Docena (Primera, Segunda o Tercera)
    case 5:
    printf("Elegiste jugar por docena\n");
    printf("Ingrese a que docena quiere apostar (1.primera, 2.segunda o 3.tercera): ");
    scanf("%d", &jugador->docena);

    //verificar que el jugador ingrese una opcion valida
    while (jugador->docena < 1 || jugador->docena > 3)
    {
        printf("Opcion invalida. Porfavor ingrese una opcion valida\n");
        scanf("%d", &jugador->docena);
    }

    //verificar si el jugador gano a su docena correspondiente
    if (jugador->docena == resultado->docena_ganadora)
    {
        printf("Felicidades!! Ganaste a la docena %d y el numero ganador es %d\n", jugador->docena, resultado->numero_ganador);
        jugador->saldo += jugador->apuesta * 2; 
    }
    else {
        printf("Lo siento, perdiste. La docena ganadora fue la %d y el numero ganador es %d\n", resultado->docena_ganadora, resultado->numero_ganador);
        jugador->saldo -= jugador->apuesta;
    }
    break;

    //Apuesta por columnas, 1era, 2da y 3era
    case 6:
    printf("Elegiste jugar por columnas\n");
    printf("Ingrese a que columna quiere apostar (1.primera, 2.segunda o 3.tercera): ");
    scanf("%d", &jugador->columna);

    //verificar que el jugador ingrese una opcion valida
    while (jugador->columna < 1 || jugador->columna > 3)
    {
        printf("Opcion invalida. Porfavor ingrese una opcion valida\n");
        scanf("%d", &jugador->columna);
    }

    //verificar si el jugador aposto a una columna correspondiente
    if (jugador->columna == resultado->columna_ganadora)
    {
        printf("Felicidades!! Ganaste a la COLUMNA %d y el numero ganador es %d\n", jugador->columna, resultado->numero_ganador);
        jugador->saldo += jugador->apuesta * 2;
    }
    else {
        printf("Lo siento, perdiste. La COLUMNA ganadora fue la %d y el numero ganador es %d\n", resultado->columna_ganadora, resultado->numero_ganador);
        jugador->saldo -= jugador->apuesta;
    }
    break;

    //En otro caso.......    
    default:
    printf("Opcion invalida!!\n");
    break;
    }
    actualizar_ranking(jugador);
}



//Funcion para gestionar el dinero de las apuestas como retirar y recargar, aca se usan archivos
void GestionarDinero(Jugador *jugador, ResultadoRuleta *resultado) {
   
    FILE *GestionDinero; //vamos a manejar archivos para la gestion del dinero
    char linea[100]; 
    char historial[1000];
    int saldo_inicial, saldo_actual, total_apostado, total_perdido;
    int opcion;
    int cant_a_rec, cant_a_retirar;

    //abrimos el archivo en modo escritura y lectura
    GestionDinero = fopen("Gestion_dinero.txt", "a+");

    //primera condicion para el archivo por si no existe
    if (GestionDinero == NULL)
    {
        perror("Error al abrir el archivo");
        return;
    }

    //empezamos a operar con el archivo de manera tal que busquemos al jugador
    while (fgets(linea, 100, GestionDinero) != NULL)
    {
      char *nombre = strtok(linea, ";");
      if (strcmp(nombre, jugador->nombre) == 0)
      {
        //si se encuentra al jugador se leen los datos
        sscanf(linea, "%*s: %d; %d; %d; %d[^\n]", &saldo_actual, &saldo_inicial, &total_apostado, &total_perdido);
        break;
      }
    }

    //si no se encuentra al jugador creamos uno
    if (feof(GestionDinero))
    {
        printf("Jugador no encontrado. Creando uno nuevo....\n");
        fprintf(GestionDinero, "%s,1000,1000,0,0,\n", jugador->nombre);
        saldo_inicial = 1000;
        saldo_actual; 
        total_apostado = 0;
        total_perdido = 0;
    }

    //Menu de opciones
    printf("Saldo actual: %d\n", jugador->saldo);
    printf("1. Recargar saldo\n");
    printf("2. Retirar dinero\n");
    printf("3. Salir\n");
    printf("\n");
    printf("Ingrese una opcion: ");
    scanf(" %d", &opcion);

    switch (opcion)
    {
    case 1: //recargar saldo
    printf("Ingrese la cantidad a recargar: ");
    scanf("%d", &cant_a_rec);

    //validamos si la cantidad a recargar es positiva
    if (cant_a_rec > 0)
    {
        jugador->saldo += cant_a_rec;
        printf("Perfecto. saldo recargado, ahora tu saldo es: %d\n", jugador->saldo);

        //ahora actualizamos el archivo
        fseek(GestionDinero, 0, SEEK_END); //ir al final del archivo
        fprintf(GestionDinero, "%s,%s,%d,%s\n", jugador->nombre, "Recarga", cant_a_rec);
    }   
    else {
        printf("La cantidad debe ser positiva\n");
    }
    break;

    case 2: //retirar dinero

    printf("Ingrese la cantidad a retirar: ");
    scanf("%d", &cant_a_retirar);

    //ahora validamos que el retiro sea menor al saldo
    if (cant_a_retirar <= jugador->saldo)
    {
    
        jugador->saldo -= cant_a_retirar; //se resta la cantidad a retirar con el saldo quedando el saldo que te queda en la ruleta

        if (saldo_actual < 0) { 
            saldo_actual = 0;  //se verifica si el saldo es positivo
        }

        printf("Perfecto. saldo retirado, ahora tu saldo es: %d\n", jugador->saldo);

        //agregamos al historial
        strcat(historial, ",retiro_");
        char cantidad[10];
        sprintf(cantidad, "%d", cant_a_retirar);
        strcat(historial, cantidad);

        //actualizamos el archivo
        fseek(GestionDinero, 0, SEEK_END); //ir al final del archivo
        fprintf(GestionDinero, "%s,%s,%d,%s\n", jugador->nombre, "Retiro", cant_a_retirar);
    }
    else {
         printf("Saldo insuficiente. No puede retirar esa cantidad\n");
    }
    break;    

    //en otro caso.....
    default:
    printf("Opcion no valida");
        break;
    }

    // Cerrar el archivo
    fclose(GestionDinero);
}



//funcion para comparar el saldo entre dos jugadores
int comparar_por_saldo(const void *a, const void *b) {
    const Jugador *jugador1 = (const Jugador *)a;
    const Jugador *jugador2 = (const Jugador *)b;
    return jugador1->saldo - jugador2->saldo; //ordenar de mayor a menor saldo del jugador 1 - saldo del jugador 2
}

//funcion para actualizar el ranking
void actualizar_ranking(Jugador *jugador) {
    //agregar al jugador en el ranking si aun no esta
    if (num_jugadores < MAX_JUGADORES)
    {
        jugadores[num_jugadores] = *jugador;
        num_jugadores++;
    }
    
    else {
        //por si el arreglo esta lleno, se reemplaza al jugador con menor saldo 
        for (int i = 0; i < MAX_JUGADORES; i++)
        {
            if (jugadores[i].saldo < jugador->saldo)
            {
                jugadores[i] = *jugador;
                break;
            }   
        }
    }
    qsort(jugadores, num_jugadores, sizeof(Jugador), comparar_por_saldo); //usamos qsort (Quicksort)  para ordenar por saldo

}



//mostrar ranking de los mejores cinco (5) jugadores (segun lo que apostaron y ganaron)
void MostrarRanking(Jugador jugadores[], int num_jugadores) {
    printf("Mostrando ranking de jugadores:\n");
    for (int i = 0; i < num_jugadores; i++)
    {
        printf("%d. %s - Saldo: %d\n", i + 1, jugadores[i].nombre, jugadores[i].saldo); //nombre;saldo
    }
}

//programa principal
int main() {

    srand(time(NULL)); //generador de numeros aleatorios

    ResultadoRuleta resultado;
    Jugador jugador; //esta variable sirve para poder manejarme mejor con las variables del struct, es el registro
    printf("Ingrese su nombre por favor: ");
    scanf("%29s", jugador.nombre);
    jugador.saldo = 1000; //saldo inicial de 1000
    SaludarJugador(&jugador);

    //imprimimos la consigna con la ruleta
    ImprimirConsigna();
    ImprimirRuleta();
    RuletaClasica();

    int opcion; //opcion del menu

    do
    {
     MostrarMenuOpciones();
     printf("Ingrese una opcion de menu: ");
     scanf("%d", &opcion);

     switch (opcion)
     {
        //Hacer apuesta y jugar
     case 1:
     printf("Saldo actual: %d\n", jugador.saldo);
     printf("Ingrese el saldo que quiera apostar: ");
     scanf("%d", &jugador.apuesta);

     //verificar que el jugador tenga saldo para jugar
     if (jugador.apuesta > jugador.saldo)
     {
        printf("No tiene saldo suficiente, si no intente con un saldo menor\n");
     }
     else { //si tengo saldo y para apostar se llaman a las dos siguientes funciones y se me actualiza el saldo
        GenerarNumeroGanador(&resultado);
        VerificarApuesta(&jugador, &resultado);
        printf("Saldo restante: %d\n", jugador.saldo);
     }
        break;
        
        //Gestionar dinero de las apuestas
        case 2:
        GestionarDinero(&jugador, &resultado);
        break;

        //Mostrar ranking de jugadores
        case 3:
        if (num_jugadores > 0) {
            MostrarRanking(jugadores, num_jugadores);
        } 
        else {
            printf("Aun no hay jugadores en el ranking.\n");
            }
        break;
        //salir del programa
        case 4:
        printf("¡¡Hasta luego!!");
        break;

     default:
     printf("Opcion no valida. Por favor, intente de nuevo.\n");
        break;
     }

    } while (opcion != 4);
    
    return 0;
}



void ImprimirConsigna() {
 // Colores ANSI (puedes probar con diferentes combinaciones)
    char* color1 = "\033[1;31m"; // Rojo
    char* color2 = "\033[1;32m"; // Verde
    char* color3 = "\033[1;33m"; // Amarillo
    char* color4 = "\033[1;34m"; // Azul
    char* color5 = "\033[1;35m"; // Magenta
    char* reset = "\033[0m"; // Restablecer color

    // Imprimir el texto con colores alternados
    printf("%s****   *  *   *      ****   *****   *****    %s\n", color1, reset);
    printf("%s*   *  *  *   *      *        *     *   *    %s\n", color2, reset);
    printf("%s***    *  *   *      ***      *      ***     %s\n", color3, reset);
    printf("%s*  *   *  *   *      *        *     *   *    %s\n", color4, reset);
    printf("%s*   *   **    ****   ****     *     *   *    %s\n", color5, reset);
    printf("\n");
}



//Funcion para imprimir la tabla de la ruleta
void ImprimirRuleta(){
    // Colores ANSI
    char* verde = "\033[1;32m";  // Verde
    char* rojo = "\033[1;31m";   // Rojo
    char* negro = "\033[1;30m"; // Negro
    char* reset = "\033[0m";    // Restablecer color

    printf("     |%s7%s|%s28%s|%s12%s|%s35%s|%s3%s|%s26%s|%s  %s0%s  %s|%s32%s|%s15%s|%s19%s|%s4%s|%s21%s|%s2%s|\n",
           rojo, reset, negro, reset, rojo, reset, negro, reset, rojo, reset, negro, reset,
           verde, reset, negro, reset, rojo, reset, negro, reset, rojo, reset, negro, reset);
    printf("    |%s29%s|                                     |%s25%s|\n", negro, reset, negro, reset);
    printf("   |%s18%s|                                       |%s17%s|\n", rojo, reset, negro, reset);
    printf("  |%s22%s|                LA RULETA                |%s34%s|\n", negro, reset, negro, reset);
    printf("   |%s9 %s|                                       |%s6 %s|\n", rojo, reset, negro, reset);
    printf("    |%s31%s|                                     |%s27%s|\n", negro, reset, negro, reset);
    printf("     |%s14%s|%s20%s|%s1 %s|%s33%s|%s16%s|%s24%s|%s5 %s|%s10%s|%s23%s|%s8 %s|%s30%s|%s11%s|%s36%s|%s13%s|\n",
           negro, reset, rojo, reset, negro, reset, rojo, reset, negro, reset, rojo, reset,
           negro, reset, rojo, reset, negro, reset, rojo, reset, negro, reset, rojo, reset);
    printf("\n");
}



//imprimir la tabla de la ruleta clasica
void RuletaClasica() {
    char* verde = "\033[1;32m";  // Verde
    char* rojo2 = "\033[1;31m";   // Rojo
    char* negro2 = "\033[1;30m"; // Negro
    char* reset = "\033[0m";    // Restablecer color
    printf("\t\t\t /|%s3%s|%s6%s|%s9%s|%s12%s|%s15%s|%s18%s|%s21%s|%s24%s|%s27%s|%s30%s|%s33%s|%s36%s|2 a 1|\n", rojo2, reset, negro2, reset, rojo2, reset, rojo2, reset, negro2, reset, rojo2, reset, rojo2, reset, negro2, reset, rojo2, reset, rojo2, reset, negro2, reset, rojo2, reset);

    printf("\t\t\t|%s0%s|%s2%s|%s5%s|%s8%s|%s11%s|%s14%s|%s17%s|%s20%s|%s23%s|%s26%s|%s29%s|%s32%s|%s35%s|2 a 1|\n", verde, reset, negro2, reset, rojo2, reset, negro2, reset, negro2, reset, rojo2, reset, negro2, reset, negro2, reset, rojo2, reset, negro2, reset, negro2, reset, rojo2, reset, negro2, reset);
    printf("\t\t\t \\|%s1%s|%s4%s|%s7%s|%s10%s|%s13%s|%s16%s|%s19%s|%s22%s|%s25%s|%s28%s|%s31%s|%s34%s|2 a 1|\n", rojo2, reset, negro2, reset, rojo2, reset, negro2, reset, negro2, reset, rojo2, reset, rojo2, reset, negro2, reset, rojo2, reset, negro2, reset, negro2, reset, rojo2, reset);
    printf("\t\t\t  | 1ra 12 |  2da  12  |  3ra  12  |\n");
    printf("\t\t\t  |1 a 18| PAR |%s %s|%s %s| IMPAR |19 a 36|", rojo2, reset, negro2, reset);
}




//menu de opciones
void MostrarMenuOpciones() {
    printf("\n");
	printf("MENU DE OPCIONES:\n");
	printf("1.Hacer apuesta y jugar\n");
	printf("2.Gestionar dinero de las apuestas\n");
	printf("3.Ver ranking\n");
	printf("4.Salir del programa\n");
	printf("\n");
}



//mostrar tipo de apuestas disponibles
void Apuestas(){
    printf("\n");
    printf("OPCIONES DE APUESTAS:\n");
    printf("1.Plenos: un solo numero. (si gana se multiplica por 35 lo apostado)\n");
    printf("2.Color: apostas a un color rojo o negro. (multiplica por 2 lo apostado)\n");
    printf("3.Par/Impar: apostar a pares o impares. (multiplica por 2 lo apostado)\n");
    printf("4.Falta o pasa: apostar de 1-18 o 19-36. (multiplica por 2 lo apostado)\n");
    printf("5.Docena: primera, segunda y tercera 1-12, 13-24 y 25-36 (multiplica por 2 lo apostado)\n");
    printf("6.Columna: 1era, 2da y 3era. (multiplica por 2 lo apostado)\n");
    printf("\n");
}

/*INSTRUCCIONES DE COMO JUGAR PASO POR PASO:

-EL PROGRAMA PIDE NOMBRE DEL USUARIO, LO CUAL PEDIRA EL NOMBRE.

-DESPUES DE ESO EL PROGRAMA IMPRIME LA BIENVENIDA CON SU NOMBRE TODO DECORADO.

-SE LE MOSTRARA UN MENU DE OPCIONES, COMO HACER APUESTA, GESTIONAR DINERO, VER RANKING Y SALIR DEL PROGRAMA.

-AL HACER APUESTA EL USUARIO EMPIEZA CON 1000 PESOS INICIALES.

-DE AHI EL PROGRAMA LE PIDE CUANTO QUIERE APOSTAR.

-DESPUES TIENE OPCIONES DE APUESTA COMO; PLENOS, COLOR, PAR/IMPAR, FALTA/PASA, DOCENA Y COLUMNA.

-SI EL USUARIO GANA SE LE ACUMULA EL SALDO CON SU GANANCIA SINO SE LE RESTA.

-DESPUES DEVUELTA SE LE MUESTRA EL MENU DE OPCIONES POR SI QUIERE SEGUIR APOSTANDO, GESTIONAR DINERO, VER RANKING O SALIR 
DEL PROGRAMA.

-AL GESTIONAR DINERO DE LAS APUESTAS EL USUARIO TIENE TRES (3) OPCIONES: RECARGAR SALDO, RETIRAR SALDO O SALIR. AL RECARGAR
SE TE SUMA EL SALDO ACTUAL MAS LO RECARGADO, Y SI RETIRAS SE TE QUEDA SALDO 0 YA QUE ESO SE GUARDA EN EL ARCHIVO (LO RETIRADO).
(LO MISMO CON LO RECARGADO).
-AL VER RANKING NO FUI MUY EXIGENTE, SOLO MUESTRA EL NOMBRE Y EL SALDO, QUISE PONER UN RANKING DE LOS CINCO (5) JUGADORES
CON MEJOR SALDO PERO NO SALIO. UPS:(.

-POR ULTIMO SALIR.

-SE LE DEDICO CASI DOS (2) SEMANAS AL TRABAJO, ENTRE ELLAS, 4 HORAS POR DIA.

-ULTIMO: ANTE CUALQUIER INQUIETUD O CONSULTA AVISARME ASI PUEDO MEJORARLE ALGUNAS COSAS.

-LO QUE SI ALGO ASI SE IMPRIME EL ARCHIVO: (SI SE USA CYGWIN CAPAS IMPRIMA CARACTERES RAROS PERO AUN ASI PUEDO EXPLICARLO):
juan,1000,1000,0,0,
juan,1000,1000,0,0,
juan,Recarga,1000,(null)
juan,1000,1000,0,0,
juan,Recarga,300,(null)
 
ESTE ESTA EJECUTADO EN VISUAL.

FIN:)
*/

/*
cd trabajointeg/juego
gcc -o ruletamain ruletamain.c
ls -la 
./ruletamain
*/