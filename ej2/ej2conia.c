#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define MAX_PROD 200

typedef struct {
    char sucursal[16];
    int cod_prod;
    int cantidad;
} Venta;

typedef struct {
    int cod_prod;
    char desc[21];
    float precio;
    int stock;
    int punto_pedido;
    int cant_pedido;
} Producto;

typedef struct {
    int cod_prod;
    int cantidad;
} Pedido;

int contadorRegistros(FILE*);

int main() {
    FILE *fprod = fopen("productos.dat", "r+b");
    if (fprod == NULL) {
        printf("No se pudo abrir productos.dat\n");
        getch();
        exit(1);
    }
    Producto productos[MAX_PROD];
    int nprod = contadorRegistros(fprod);
    fread(productos, sizeof(Producto), nprod, fprod);
    printf("Productos leidos:\n");
    for (int i = 0; i < nprod; i++) {
    printf("%d %s %.2f %d %d %d\n", productos[i].cod_prod, productos[i].desc, productos[i].precio, productos[i].stock, productos[i].punto_pedido, productos[i].cant_pedido);
}
    FILE *fventas = fopen("ventas.dat", "rb");
    if (fventas== NULL) {
        printf("No se pudo abrir ventas.dat\n");
        getch();
        exit(1);
    }

    float recaudacion_empresa = 0, max_importe = -1;
    char suc_mas_vend[16] = "";
    Venta v;
    fread(&v, sizeof(Venta), 1, fventas);
    printf("Importe total vendido por sucursal:\n");
    while (!feof(fventas)) {
        char suc_actual[16];
        strcpy(suc_actual, v.sucursal);
        float total_suc = 0;
        int cant_prod_suc = 0;

        // Corte de control por sucursal
        while (!feof(fventas) && strcmp(v.sucursal, suc_actual) == 0) {
            // Buscar producto
            int i;
            for (i = 0; i < nprod; i++) {
                if (productos[i].cod_prod == v.cod_prod) {
                    total_suc += productos[i].precio * v.cantidad;
                    productos[i].stock -= v.cantidad;
                    cant_prod_suc += v.cantidad;
                    break;
                }
            }
            fread(&v, sizeof(Venta), 1, fventas);
        }
        printf("%s: $%.2f\n", suc_actual, total_suc);
        recaudacion_empresa += total_suc;
        if (total_suc > max_importe) {
            max_importe = total_suc;
            strcpy(suc_mas_vend, suc_actual);
        }
    }

    printf("\nRecaudacion total de la empresa: $%.2f\n", recaudacion_empresa);
    printf("Sucursal que mas vendio: %s\n", suc_mas_vend);

    // Actualizar archivo de productos con nuevo stock
    rewind(fprod);
    fwrite(productos, sizeof(Producto), nprod, fprod);
    fclose(fprod);
    fclose(fventas);

    // Generar archivo de pedidos
    FILE *fped = fopen("pedidos.dat", "wb");
    if (!fped) {
        printf("No se pudo crear pedidos.dat\n");
        fclose(fventas);
        return 1;
    }
    for (int i = 0; i < nprod; i++) {
        if (productos[i].stock < productos[i].punto_pedido) {
            Pedido p;
            p.cod_prod = productos[i].cod_prod;
            p.cantidad = productos[i].cant_pedido;
            fwrite(&p, sizeof(Pedido), 1, fped);
        }
    }
    fclose(fped);
}

int contadorRegistros(FILE *ap) {
    int bytes, registros;
    fseek(ap, 0, SEEK_END);
    bytes = ftell(ap);
    registros = bytes / sizeof(Producto);
    rewind(ap);
    return registros;
}