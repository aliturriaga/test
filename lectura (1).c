

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_NUM_COLUMNS 302

// Estructura que representa una fila del archivo CSV
typedef struct {
    char rut[MAX_LINE_LENGTH];
    char nombre_completo[MAX_LINE_LENGTH];
    int edad;
    char cod_plan[MAX_LINE_LENGTH];
    char descripcion_plan[MAX_LINE_LENGTH];
    char desde[MAX_LINE_LENGTH];
    char hasta[MAX_LINE_LENGTH];
    char cod_sede[MAX_LINE_LENGTH];
    char ubicacion_sede[MAX_LINE_LENGTH];
} Row;

// Función que lee una fila del archivo CSV
Row read_row(char* line) {
    Row row;
    char* token;
    int column = 0;
    token = strtok(line, ",");
    while (token != NULL) {
        switch (column) {
            case 0:
                strcpy(row.rut, token);
                break;
            case 1:
                strcpy(row.nombre_completo, token);
                break;
            case 2:
                row.edad = atoi(token);
                break;
            case 3:
                strcpy(row.cod_plan, token);
                break;
            case 4:
                strcpy(row.descripcion_plan, token);
                break;
            case 5:
                strcpy(row.desde, token);
                break;
            case 6:
                strcpy(row.hasta, token);
                break;
            case 7:
                strcpy(row.cod_sede, token);
                break;
            case 8:
                strcpy(row.ubicacion_sede, token);
                break;
            default:
                break;
        }
        token = strtok(NULL, ",");
        column++;
    }
    return row;
}
// Función que convierte una cadena de fecha en una estructura de fecha
struct tm str_to_date(char* str) {
    struct tm date = {0};
    strptime(str, "%d/%m/%Y", &date);
    return date;
}
void write_row(Row row, FILE* file) {
    fprintf(file, "%s,%s,%d,%s,%s,%s,%s,%s,%s\n", row.rut, row.nombre_completo, row.edad, row.cod_plan, row.descripcion_plan, row.desde, row.hasta, row.cod_sede, row.ubicacion_sede);
}
// Función que verifica si un cliente ya existe en el archivo CSV
int row_exists(Row row, Row rows[], int num_rows) {
    for (int i = 0; i < num_rows; i++) {
        if (strcmp(row.rut, rows[i].rut) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() {
    FILE* file = fopen("BigMuscle_modified.csv", "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo\n");
        return 1;
    }

    Row rows[MAX_NUM_COLUMNS];
    int num_rows = 0;

    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, file); // Leer la primera línea, que contiene los nombres de las columnas

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && num_rows < MAX_NUM_COLUMNS) {
        Row row = read_row(line);
        rows[num_rows] = row;
        num_rows++;
    }
    fclose(file);

    // Mostrar el panel de opciones
    printf("1. Agregar cliente\n");
    printf("2. Eliminar cliente\n");
    printf("3. Editar datos del cliente\n");
    printf("4. Salir\n");

    // Leer la opción elegida por el usuario
    int option = 0;
    printf("Ingrese una opcion: ");
    scanf("%d", &option);

    switch (option) {
        case 1:{
    Row new_row;
    printf("Ingrese el rut: ");
    scanf("%s", new_row.rut);
    printf("Ingrese el nombre completo: ");
    scanf(" %[^\n]s", new_row.nombre_completo);
    printf("Ingrese la edad: ");
    scanf("%d", &new_row.edad);
    
    // Verificar si el codigo de plan ingresado existe en el archivo CSV
    int plan_exists = 0;
    char cod_plan[MAX_LINE_LENGTH];
    printf("Ingrese el codigo del plan: ");
    scanf("%s", cod_plan);
    for (int i = 0; i < num_rows; i++) {
        if (strcmp(cod_plan, rows[i].cod_plan) == 0) {
            plan_exists = 1;
            break;
        }
    }
    if (plan_exists) {
        strcpy(new_row.cod_plan, cod_plan);
        printf("Ingrese la descripcion del plan: ");
        scanf(" %[^\n]s", new_row.descripcion_plan);
        printf("Ingrese la fecha de inicio del plan (formato: dd/mm/yyyy): ");
        scanf("%s", new_row.desde);
        printf("Ingrese la fecha de termino del plan (formato: dd/mm/yyyy): ");
        scanf("%s", new_row.hasta);
        if (strcmp(new_row.desde,new_row.hasta) > 0) {
        printf("La fecha desde no puede ser mayor a la fecha hasta.\n");
        break;
        }
    } else {
        printf("El codigo de plan ingresado no existe en el archivo CSV\n");
        break;
    }

    // Verificar si el codigo de sede ingresado existe en el archivo CSV
    int sede_exists = 0;
    char cod_sede[MAX_LINE_LENGTH];
    printf("Ingrese el codigo de la sede: ");
    scanf("%s", cod_sede);
    for (int i = 0; i < num_rows; i++) {
        if (strcmp(cod_sede, rows[i].cod_sede) == 0) {
            sede_exists = 1;
            break;
        }
    }
    if (sede_exists) {
        strcpy(new_row.cod_sede, cod_sede);
        printf("Ingrese la ubicacion de la sede: ");
        scanf(" %[^\n]s", new_row.ubicacion_sede);
    } else {
        printf("El codigo de sede ingresado no existe en el archivo CSV\n");
        break;
    }

    // Verificar si el cliente ya existe en el archivo CSV
    if (row_exists(new_row, rows, num_rows)) {
        printf("El cliente ya existe en el archivo CSV\n");
    } else {
        // Abrir el archivo en modo "append"
        FILE* output_file = fopen("BigMuscle_modified.csv", "a");
        if (output_file == NULL) {
            printf("No se pudo abrir el archivo\n");
            return 1;
        }

        // Agregar el nuevo cliente al final del archivo
        write_row(new_row, output_file);

        fclose(output_file);

        printf("Cliente agregado exitosamente\n");
        break;
    }
}

        case 2:
    {
        char rut[MAX_LINE_LENGTH];
        printf("Ingrese el rut del cliente a eliminar: ");
        scanf("%s", rut);

        // Verificar si el rut ingresado existe en el archivo CSV
        int row_index = -1;
        for (int i = 0; i < num_rows; i++) {
            if (strcmp(rut, rows[i].rut) == 0) {
                row_index = i;
                break;
            }
        }
        if (row_index == -1) {
            printf("El cliente con el rut ingresado no existe.\n");
            break;
        }

        // Crear un archivo temporal para escribir las filas que no corresponden al cliente a eliminar
        FILE* temp_file = fopen("temp.csv", "w");
        if (temp_file == NULL) {
            printf("No se pudo crear el archivo temporal\n");
            return 1;
        }
        fprintf(temp_file, "%s", "rut,nombre_completo,edad,cod_plan,descripcion_plan,desde,hasta,cod_sede,ubicacion_sede\n");
        for (int i = 0; i < num_rows; i++) {
            if (i != row_index) {
                write_row(rows[i], temp_file);
            }
        }
        fclose(temp_file);

        // Sobrescribir el archivo original con el archivo temporal
        remove("BigMuscle_modified.csv");
        rename("temp.csv", "BigMuscle_modified.csv");

        printf("El cliente con el rut ingresado fue eliminado exitosamente.\n");
        break;
    }
    
        case 3:{
    char rut[MAX_LINE_LENGTH];
    printf("Ingrese el rut del cliente a editar: ");
    scanf("%s", rut);

    // Buscar la fila que corresponde al rut ingresado
    int row_index = -1;
    for (int i = 0; i < num_rows; i++) {
        if (strcmp(rut, rows[i].rut) == 0) {
            row_index = i;
            break;
        }
    }

    if (row_index == -1) {
        printf("El rut ingresado no existe en el archivo CSV\n");
        break;
    }

    Row *row = &rows[row_index];

    printf("1. Editar nombre completo (%s)\n", (*row).nombre_completo);
    printf("2. Editar edad (%d)\n", (*row).edad);
    printf("3. Editar codigo de plan (%s)\n", (*row).cod_plan);
    printf("4. Editar descripcion de plan (%s)\n", (*row).descripcion_plan);
    printf("5. Editar fecha de inicio de plan (%s)\n", (*row).desde);
    printf("6. Editar fecha de termino de plan (%s)\n", (*row).hasta);
    printf("7. Editar codigo de sede (%s)\n", (*row).cod_sede);
    printf("8. Editar ubicacion de sede (%s)\n", (*row).ubicacion_sede);
    printf("9. Cancelar\n");

    int edit_option = 0;
    printf("Ingrese una opcion: ");
    scanf("%d", &edit_option);

    switch (edit_option) {
        case 1:{
            printf("Ingrese el nuevo nombre completo: ");
            scanf(" %[^\n]s", (*row).nombre_completo);
            break;
        }
        case 2:{
            printf("Ingrese la nueva edad: ");
            scanf("%d", &(*row).edad);
            break;
        }
        case 3:{
            // Verificar si el codigo de plan ingresado existe en el archivo CSV
            int plan_exists = 0;
            char cod_plan[MAX_LINE_LENGTH];
            printf("Ingrese el nuevo codigo del plan: ");
            scanf("%s", cod_plan);
            for (int i = 0; i < num_rows; i++) {
                if (i != row_index && strcmp(cod_plan, rows[i].cod_plan) == 0) {
                    plan_exists = 1;
                    break;
                }
            }
            if (plan_exists) {
                strcpy((*row).cod_plan, cod_plan);
                printf("Ingrese la nueva descripcion del plan: ");
                scanf(" %[^\n]s", (*row).descripcion_plan);
                printf("Ingrese la nueva fecha de inicio del plan (formato: dd/mm/yyyy): ");
                scanf("%s", (*row).desde);
                printf("Ingrese la nueva fecha de termino del plan (formato: dd/mm/yyyy): ");
                scanf("%s", (*row).hasta);
                if (strcmp((*row).desde,(*row).hasta) >= 0) {
                printf("La fecha de termino del plan debe ser posterior a la fecha de inicio\n");
                break;
                }
                } else {
                printf("El codigo de plan ingresado no existe en el archivo CSV\n");
                }
                break;
                }
                case 4:{
                printf("Ingrese la nueva descripcion del plan: ");
                scanf(" %[^\n]s", (*row).descripcion_plan);
                break;
                }
                case 5:{
                printf("Ingrese la nueva fecha de inicio del plan (formato: dd/mm/yyyy): ");
                scanf("%s", (*row).desde);
                if (strcmp((*row).desde,(*row).hasta) >= 0) {
                printf("La fecha de termino del plan debe ser posterior a la fecha de inicio\n");
                break;
                }
                break;
                }
                case 6:{
                printf("Ingrese la nueva fecha de termino del plan (formato: dd/mm/yyyy): ");
                scanf("%s", (*row).hasta);
                if (strcmp((*row).desde,(*row).hasta) >= 0) {
                printf("La fecha de termino del plan debe ser posterior a la fecha de inicio\n");
                break;
                }
                break;
                }
                case 7:{
                printf("Ingrese el nuevo codigo de sede: ");
                scanf("%s", (*row).cod_sede);
                break;
                }
                case 8:{
                printf("Ingrese la nueva ubicacion de la sede: ");
                scanf(" %[^\n]s", (*row).ubicacion_sede);
                break;
                }
                case 9:{
                // No se hace nada, se sale del switch
                break;
                }
                default:{
                printf("Opcion invalida\n");
                break;
                }
                }
          // Actualizar el archivo CSV
FILE *fp = fopen("BigMuscle_modified.csv", "w");
fprintf(fp, "rut,nombre_completo,edad,cod_plan,descripcion_plan,desde,hasta,cod_sede,ubicacion_sede\n");
for (int i = 0; i < num_rows; i++) {
    fprintf(fp, "%s,%s,%d,%s,%s,%s,%s,%s,%s\n", rows[i].rut, rows[i].nombre_completo, rows[i].edad, rows[i].cod_plan, rows[i].descripcion_plan, rows[i].desde, rows[i].hasta, rows[i].cod_sede, rows[i].ubicacion_sede);
    
}
fclose(fp);

printf("El cliente ha sido actualizado exitosamente\n");
break;
            
        case 4:{
            printf("Adios!\n");
            return 0;
            break;
        }
        default:{
            printf("Opcion invalida\n");
            break;  }
    }
    }
    }

