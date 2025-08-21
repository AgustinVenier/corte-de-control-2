#include <stdio.h>
#include <string.h>

#define N 9

typedef struct {
	char sector[16];
	int duracion;
	int tipo;
} Llamada;

int main() {
		// Generar archivo costos.dat
		FILE *fc = fopen("costos.dat", "wb");
		if (!fc) {
			printf("No se pudo crear el archivo costos.dat.\n");
			return 1;
		}
		float costos[3] = {0.5, 1.2, 2.0}; // ejemplo: local, larga distancia, celular
		fwrite(costos, sizeof(float), 3, fc);
		fclose(fc);
		printf("Archivo costos.dat generado correctamente.\n");

		// Generar archivo llamadas.dat
		FILE *f = fopen("llamadas.dat", "wb");
		if (!f) {
			printf("No se pudo crear el archivo llamadas.dat.\n");
			return 1;
		}
	Llamada llamadas[N] = {
		{"RecursosHumanos", 60, 1},
		{"RecursosHumanos", 180, 2},
		{"RecursosHumanos", 45, 3},
		{"Sistemas", 300, 1},
		{"Sistemas", 150, 2},
		{"Sistemas", 75, 3},
		{"Ventas", 120, 1},
		{"Ventas", 200, 2},
		{"Ventas", 90, 3}
	};
	fwrite(llamadas, sizeof(Llamada), N, f);
	fclose(f);
	printf("Archivo Llamadas.dat generado correctamente.\n");
	return 0;
}
