#include <stdio.h>

typedef struct {
    int canal;
    char programa[36]; // 35 + 1 para null-terminator
    float rating;
} Rating;

typedef struct {
    int canal;
    float promedio;
} CanalPromedio;

int main() {
    FILE *frating = fopen("Rating.dat", "rb");
    if (!frating) {
        printf("No se pudo abrir Rating.dat\n");
        return 1;
    }

    FILE *fprom = fopen("Promedios.dat", "wb");
    if (!fprom) {
        printf("No se pudo crear Promedios.dat\n");
        fclose(frating);
        return 1;
    }

    Rating reg;
    int canal_actual, canal_mas_visto = -1;
    float suma_canal, max_rating = -1;
    int cant_canal;

    // Para informar canales con menos de 15 puntos
    printf("Canales con menos de 15 puntos en total:\n");

    int leido = fread(&reg, sizeof(Rating), 1, frating);
    while (leido) {
        canal_actual = reg.canal;
        suma_canal = 0;
        cant_canal = 0;

        // Corte de control por canal
        while (leido && reg.canal == canal_actual) {
            suma_canal += reg.rating;
            cant_canal++;
            leido = fread(&reg, sizeof(Rating), 1, frating);
        }

        if (suma_canal < 15)
            printf("Canal %d (total: %.2f)\n", canal_actual, suma_canal);

        if (suma_canal > max_rating) {
            max_rating = suma_canal;
            canal_mas_visto = canal_actual;
        }

        // Escribir promedio en archivo
        CanalPromedio cp;
        cp.canal = canal_actual;
        cp.promedio = suma_canal / cant_canal;
        fwrite(&cp, sizeof(CanalPromedio), 1, fprom);
    }

    printf("\nCanal mas visto: %d (total: %.2f)\n", canal_mas_visto, max_rating);

    fclose(frating);
    fclose(fprom);
    return 0;
}
