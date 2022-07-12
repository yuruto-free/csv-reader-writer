#include "csv_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define IS_NOT_INIT (0)
#define IS_INIT (1)
#define RETVAL_OK (0)
#define RETVAL_NG (1)
#define MAX_DIGIT (11)  // int型の最大桁数

struct param_t {
    char *filename;
    int row;
    int col;
};

static int is_init = (int)IS_NOT_INIT;
static struct param_t args;

// プロトタイプ宣言
/**
 * @brief データの行数と列数をカウント
 * @param fd: ファイルディスクリプタ
 * @return RETVAL_OK: 成功
           RETVAL_NG: 失敗
*/
static int row_col_counter(FILE *fd);
/**
 * @brief データの読み込み
 * @param fd:   ファイルディスクリプタ
 * @param data: 格納先のポインタ
 * @return RETVAL_OK: 成功
           RETVAL_NG: 失敗
*/
static int read_data(FILE *fd, int *data);
/**
 * @brief データの書き込み
 * @param fd:   ファイルディスクリプタ
 * @param data: 書き込むデータ
 * @return RETVAL_OK: 成功
           RETVAL_NG: 失敗
*/
static int write_data(FILE *fd, int *data);

int CSVAPI_initialize(const char *csv_filename) {
    int ret;
    int length;

    // 内部状態チェック
    if ((int)IS_INIT == is_init) {
        ret = (int)CSVAPI_STATUS_ERR;
        goto EXIT_INITIALIZE;
    }
    // 引数チェック
    if (NULL == csv_filename) {
        ret = (int)CSVAPI_ARGUMENT_ERR;
        goto EXIT_INITIALIZE;
    }
    // 内部変数を初期化
    memset(&args, 0, sizeof(struct param_t));
    // 読み込むファイル名を取得
    length        = strlen(csv_filename);
    args.filename = (char *)malloc(sizeof(char) * length);
    if (NULL == args.filename) {
        ret = CSVAPI_MALLOC_ERR;
        goto EXIT_INITIALIZE;
    }
    // ファイル名をコピー
    memcpy(args.filename, csv_filename, length);
    // 内部状態を更新
    is_init = (int)IS_INIT;
    ret     = (int)CSVAPI_RETURN_OK;

EXIT_INITIALIZE:

    return ret;
}

int CSVAPI_read_data(int **data) {
    int ret;
    int func_val;
    FILE *fd = NULL;

    // 内部状態チェック
    if ((int)IS_NOT_INIT == is_init) {
        ret = (int)CSVAPI_STATUS_ERR;
        goto EXIT_READ_DATA;
    }
    // 引数チェック
    if ((NULL == data) || (NULL != (*data))) {
        ret = (int)CSVAPI_ARGUMENT_ERR;
        goto EXIT_READ_DATA;
    }
    // ファイル読み込み準備
    fd = fopen(args.filename, "r");
    if (NULL == fd) {
        ret = (int)CSVAPI_IO_ERR;
        goto EXIT_READ_DATA;
    }
    // データカウント
    func_val = row_col_counter(fd);
    if ((int)RETVAL_OK != func_val) {
        ret = (int)CSVAPI_INTERNAL_ERR;
        goto EXIT_READ_DATA;
    }
    // malloc
    (*data) = (int *)malloc(sizeof(int) * args.row * args.col);
    if (NULL == (*data)) {
        ret = (int)CSVAPI_MALLOC_ERR;
        goto EXIT_READ_DATA;
    }
    memset(*data, 0, sizeof(int) * args.row * args.col);
    // ファイル読み込み
    fseek(fd, 0, SEEK_SET);
    func_val = read_data(fd, *data);
    if ((int)RETVAL_OK != func_val) {
        ret = (int)CSVAPI_INTERNAL_ERR;
        goto EXIT_READ_DATA;
    }
    ret = (int)CSVAPI_RETURN_OK;

EXIT_READ_DATA:
    if (NULL != fd) {
        fclose(fd);
    }

    return ret;
}

int CSVAPI_write_data(const char *filename, int *data) {
    int ret;
    int func_val;
    FILE *fd;

    // 内部状態チェック
    if ((int)IS_NOT_INIT == is_init) {
        ret = (int)CSVAPI_STATUS_ERR;
        goto EXIT_WRITE_DATA;
    }
    // 引数チェック
    if (NULL == data) {
        ret = (int)CSVAPI_ARGUMENT_ERR;
        goto EXIT_WRITE_DATA;
    }
    // ファイル書き込み準備
    fd = fopen(filename, "w");
    if (NULL == fd) {
        ret = (int)CSVAPI_IO_ERR;
        goto EXIT_WRITE_DATA;
    }
    // 書き込み処理
    func_val = write_data(fd, data);
    fclose(fd);

    if ((int)RETVAL_OK != func_val) {
        ret = (int)CSVAPI_INTERNAL_ERR;
        goto EXIT_WRITE_DATA;
    }
    ret = (int)CSVAPI_RETURN_OK;

EXIT_WRITE_DATA:

    return ret;
}

int CSVAPI_get_row(int *row) {
    int ret;

    if (NULL == row) {
        ret = (int)CSVAPI_ARGUMENT_ERR;
        goto EXIT_GET_ROW;
    }
    *row = args.row;
    ret  = (int)CSVAPI_RETURN_OK;

EXIT_GET_ROW:

    return ret;
}

int CSVAPI_get_col(int *col) {
    int ret;

    if (NULL == col) {
        ret = (int)CSVAPI_ARGUMENT_ERR;
        goto EXIT_GET_COL;
    }
    *col = args.col;
    ret  = (int)CSVAPI_RETURN_OK;

EXIT_GET_COL:

    return ret;
}

int CSVAPI_finalize() {
    int ret;

    // 内部状態チェック
    if ((int)IS_INIT == is_init) {
        // 領域解放
        if (NULL != args.filename) {
            free(args.filename);
        }
        // 内部変数を初期化
        memset(&args, 0, sizeof(struct param_t));
        // 内部状態を更新
        is_init = (int)IS_NOT_INIT;
        ret     = (int)CSVAPI_RETURN_OK;
    } else {
        ret = (int)CSVAPI_STATUS_ERR;
    }

    return ret;
}

static int row_col_counter(FILE *fd) {
    int ret = (int)RETVAL_NG;
    int row, col;
    char val;

    if (NULL == fd) {
        goto EXIT_ROW_COL_COUNTER;
    }
    row = 0;
    col = 0;

    // 順に読み込む
    while (EOF != fscanf(fd, "%c", &val)) {
        // カンマの場合
        if (',' == val) {
            col++;
        }
        // 改行コードの場合
        if ('\n' == val) {
            col++;
            row++;
        }
    }
    args.row = row;
    args.col = col / row;
    ret      = (int)RETVAL_OK;

EXIT_ROW_COL_COUNTER:

    return ret;
}

static int read_data(FILE *fd, int *data) {
    int ret = (int)RETVAL_NG;
    int max_row, max_col;
    int row, col;
    int count;
    char val;
    char input[MAX_DIGIT];

    if ((NULL == fd) || (NULL == data)) {
        goto EXIT_READ_DATA;
    }
    max_row = args.row;
    max_col = args.col;

    for (row = 0; row < max_row; row++) {
        for (col = 0; col < max_col; col++) {
            count    = 0;
            input[0] = '\0';

            while (EOF != fscanf(fd, "%c", &val)) {
                // カンマもしくは改行コードの場合ループを抜ける
                if ((',' == val) || ('\n' == val)) {
                    break;
                }

                if (((int)'0' <= (int)val) && ((int)val <= (int)'9')) {
                    input[count++] = val;
                }
            }
            input[count]                          = '\0';
            data[CSVAPI_INDEX(row, col, max_col)] = atoi(input);
        }
    }
    ret = (int)RETVAL_OK;

EXIT_READ_DATA:

    return ret;
}

static int write_data(FILE *fd, int *data) {
    int ret = (int)RETVAL_NG;
    int max_row, max_col;
    int row, col;

    if ((NULL == fd) || (NULL == data)) {
        goto EXIT_READ_DATA;
    }
    max_row = args.row;
    max_col = args.col;

    for (row = 0; row < max_row; row++) {
        for (col = 0; col < max_col - 1; col++) {
            fprintf(fd, "%d,", data[CSVAPI_INDEX(row, col, max_col)]);
        }
        fprintf(fd, "%d\n", data[CSVAPI_INDEX(row, max_col - 1, max_col)]);
    }
    ret = (int)RETVAL_OK;

EXIT_READ_DATA:

    return ret;
}
