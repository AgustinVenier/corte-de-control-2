#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct {
    char sector[16];
    int duracion, tipo;
} Llamada;

typedef struct {
    int duracion, tipo;
    float costo;
} Llamada2;

void mostrarGastos(FILE*);


int main() {
    int duracion_llamada, mayor_duracion, band = 1, t1,t2,t3;
    Llamada llamada;
    Llamada2 llamada2;
    float costos[3], gastos[3] = {0, 0, 0};
    char anterior[16], mayor[16];

    FILE *arch_llamadas=fopen("llamadas.dat", "rb");
    FILE *arch_costos = fopen("costos.dat", "rb");
    FILE *arch_gastos = fopen("gastos.dat", "wb");
    
    if (arch_llamadas == NULL || arch_costos == NULL || arch_gastos == NULL)
    {
        printf("Error al abrir el archivo.\n");
        getch();
        exit(1);
    }
    fread(costos, sizeof(float), 3, arch_costos);
    fread(&llamada, sizeof(Llamada), 1, arch_llamadas);
    while(!feof(arch_llamadas))
    {
        strcpy(anterior, llamada.sector);
        t1 = t2 = t3 = duracion_llamada = 0;
        float gastos_sector[3] = {0, 0, 0};
        while(strcmp(anterior, llamada.sector) == 0 && !feof(arch_llamadas)) {
            duracion_llamada += llamada.duracion;
            if(llamada.tipo == 1)
                t1 += llamada.duracion;
            else if(llamada.tipo == 2)
                t2 += llamada.duracion;
            else if(llamada.tipo == 3) 
                t3 += llamada.duracion;
            fread(&llamada, sizeof(Llamada), 1, arch_llamadas);
        }
        printf("\n -----------------SECTOR %s-----------------\n", anterior);
        printf("Llamada de tipo 1 con duracion %d segundos\n", t1);
        printf("Llamada de tipo 2 con duracion %d segundos\n", t2);
        printf("Llamada de tipo 3 con duracion %d segundos\n", t3);
        gastos_sector[0] = costos[0] * t1;
        gastos_sector[1] = costos[1] * t2;
        gastos_sector[2] = costos[2] * t3;
        fwrite(gastos_sector, sizeof(float), 3, arch_gastos);
        if (duracion_llamada > mayor_duracion || band == 1)
        {
            mayor_duracion = duracion_llamada;
            strcpy(mayor, anterior);
            band = 0;
        }
        //punto e
        char nombre_archivo[32];
        strcpy(nombre_archivo, anterior);
        strcat(nombre_archivo, ".dat");
        FILE *f = fopen(nombre_archivo, "wb");
        if(f==NULL)
        {
            printf("Error al crear el archivo %s\n", nombre_archivo);
            getch();
            exit(1);
        }
        llamada2.duracion = duracion_llamada;
        llamada2.tipo = llamada.tipo;
        llamada2.costo = costos[llamada.tipo - 1];
        fwrite(&llamada2, sizeof(Llamada2), 1, f);
        fclose(f);
    }
    printf("El sector con mayor duracion es: %s con %d segundos\n", mayor, mayor_duracion);
    fclose(arch_costos);
    fclose(arch_llamadas);
    fclose(arch_gastos);

    // Abrir gastos.dat en modo lectura para mostrar los gastos
    arch_gastos = fopen("gastos.dat", "rb");
    if (arch_gastos != NULL) {
        mostrarGastos(arch_gastos);
        fclose(arch_gastos);
    } else {
        printf("No se pudo abrir gastos.dat para mostrar los gastos.\n");
    }
    FILE *f2 = fopen("Sistemas.dat", "rb");
    if (f2 == NULL) {
        printf("Error al abrir el archivo Sistemas.dat.\n");
        getch();
        exit(1);
    }
    fread(&llamada2, sizeof(Llamada2), 1, f2);
    while(!feof(f2))
    {
        printf("Llamada de tipo %d con duracion %d segundos y costo %.2f\n",
               llamada2.tipo, llamada2.duracion, llamada2.costo);
            fread(&llamada2, sizeof(Llamada2), 1, f2);
        }
    fclose(f2);
    return 0;
}

void mostrarGastos(FILE *ag)
{
    float gastos[3];
    fread(gastos, sizeof(float), 3, ag);
    printf("Gastos de las llamadas:\n");
    printf("Tipo 1: %.2f\n", gastos[0]);
    printf("Tipo 2: %.2f\n", gastos[1]);
    printf("Tipo 3: %.2f\n", gastos[2]);
}
