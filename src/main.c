#include <stdio.h>
#include <stdlib.h>
#include "csv_api.h"
#define OUTPUT_CSV_FILENAME ("output.csv")

static void print_console(int *data);
static void output_return_type(enum CSVAPI_RETURN_TYPE ret) {
    if (RETURN_OK == ret) {
        printf("RETURN_OK\n");
    }
    else if (INTERNAL_ERR == ret) {
        printf("INTERNAL_ERR\n");
    }
    else if (STATUS_ERR == ret) {
        printf("STATUS_ERR\n");
    }
    else if (ARGUMENT_ERR == ret) {
        printf("ARGUMENT_ERR\n");
    }
    else if (IO_ERR == ret) {
        printf("IO_ERR\n");
    }
    else if (MALLOC_ERR == ret) {
        printf("MALLOC_ERR\n");
    }
    else {
        printf("none\n");
    }
}

int main(int argc, char **argv) {
    enum CSVAPI_RETURN_TYPE ret;
    int *data = NULL;

    if (argc == 2) {
        // 初期化
        ret = CSVAPI_initialize(argv[1]);
        if (RETURN_OK != ret) {
            output_return_type(ret);
            fprintf(stderr, "Error: CSVAPI_initalize\n");
            goto EXIT_MAIN;
        }
        // CSVファイルの読み込み
        ret = CSVAPI_read_data(&data);
        if (RETURN_OK != ret) {
            output_return_type(ret);
            fprintf(stderr, "Error: CSVAPI_read_data\n");
            goto EXIT_MAIN;
        }
        // データ出力
        print_console(data);
        // ファイルへの書き込み
        ret = CSVAPI_write_data(OUTPUT_CSV_FILENAME, data);
        if (RETURN_OK != ret) {
            output_return_type(ret);
            fprintf(stderr, "Error: CSVAPI_write_data\n");
            goto EXIT_MAIN;
        }
        // 終了処理
        ret = CSVAPI_finalize();
        if (RETURN_OK != ret) {
            output_return_type(ret);
            fprintf(stderr, "Error: CSVAPI_finalize\n");
            goto EXIT_MAIN;
        }
    }
    else {
        fprintf(stderr, "Usage: %s csv-filename\n", argv[0]);
    }

EXIT_MAIN:
    if (NULL != data) {
        free(data);
    }

    return 0;
}

static void print_console(int *data) {
    int row, col;
    int max_row, max_col;

    CSVAPI_get_row(&max_row);
    CSVAPI_get_col(&max_col);

    for (row = 0; row < max_row; row++) {
        for (col = 0; col < max_col - 1; col++) {
            printf("%d ", data[CSVAPI_INDEX(row, col, max_col)]);
        }
        printf("%d\n", data[CSVAPI_INDEX(row, max_col - 1, max_col)]);
    }
}
