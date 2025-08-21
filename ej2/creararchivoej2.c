#include <stdio.h>
#include <string.h>

#define MAX_PROD 5
#define MAX_VENTAS 8

typedef struct {
    int cod_prod;
    char desc[21];
    float precio;
    int stock;
    int punto_pedido;
    int cant_pedido;
} Producto;

typedef struct {
    char sucursal[16];
    int cod_prod;
    int cantidad;
} Venta;

int main() {
    Producto productos[MAX_PROD] = {
        {1, "Lapicera", 50.0, 30, 10, 20},
        {2, "Cuaderno", 200.0, 15, 5, 10},
        {3, "Goma", 25.0, 8, 5, 10},
        {4, "Regla", 60.0, 12, 6, 10},
        {5, "Cartuchera", 500.0, 5, 3, 5}
    };
    Venta ventas[MAX_VENTAS] = {
        {"SucursalA", 1, 10},
        {"SucursalA", 2, 5},
        {"SucursalA", 3, 2},
        {"SucursalB", 1, 8},
        {"SucursalB", 4, 6},
        {"SucursalB", 5, 2},
        {"SucursalC", 2, 7},
        {"SucursalC", 3, 4}
    };

    FILE *fprod = fopen("productos.dat", "wb");
    for (int i = 0; i < MAX_PROD; i++)
        fwrite(&productos[i], sizeof(Producto), 1, fprod);
    fclose(fprod);

    FILE *fventas = fopen("ventas.dat", "wb");
    for (int i = 0; i < MAX_VENTAS; i++)
        fwrite(&ventas[i], sizeof(Venta), 1, fventas);
    fclose(fventas);

    printf("Archivos de prueba creados correctamente.\n");
    return 0;
}
