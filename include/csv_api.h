#ifndef CSV_API_H_
#define CSV_API_H_
#define CSVAPI_INDEX(row, col, dim) ((row) * (dim) + (col))
#define CSVAPI_RETURN_OK (0)
#define CSVAPI_INTERNAL_ERR (1)
#define CSVAPI_STATUS_ERR (2)
#define CSVAPI_ARGUMENT_ERR (3)
#define CSVAPI_IO_ERR (4)
#define CSVAPI_MALLOC_ERR (5)

/**
 * @brief 初期化処理
 * @param csv_filename:  CSVファイル名
 * @return CSVAPI_RETURN_OK:    成功
           CSVAPI_ARGUMENT_ERR: 引数エラー
           CSVAPI_STATUS_ERR:   内部状態エラー
           CSVAPI_MALLOC_ERR:   mallocエラー
*/

int CSVAPI_initialize(const char *csv_filename);

/**
 * @brief データ読み込み処理
 * @param data:          読み込んだデータの格納先
 * @return CSVAPI_RETURN_OK:    成功
           CSVAPI_STATUS_ERR:   内部状態エラー
           CSVAPI_ARGUMENT_ERR: 引数エラー
           CSVAPI_IO_ERR:       入出力エラー
           CSVAPI_MALLOC_ERR:   mallocエラー
           CSVAPI_INTERNAL_ERR: 内部エラー
*/
int CSVAPI_read_data(int **data);

/**
 * @brief データの書き込み
 * @param filename       書き込み先
 * @param data           書き込むデータ
 * @return CSVAPI_RETURN_OK:    成功
           CSVAPI_STATUS_ERR:   失敗
           CSVAPI_ARGUMENT_ERR: 引数エラー
           CSVAPI_IO_ERR:       入出力エラー
           CSVAPI_INTERNAL_ERR: 内部エラー
*/
int CSVAPI_write_data(const char *filename, int *data);

/**
 * @brief rowの取得
 * @param row:           rowの値
 * @return CSVAPI_RETURN_OK:    成功
           CSVAPI_STATUS_ERR:   内部状態エラー
           CSVAPI_IO_ERR:       入出力エラー
*/
int CSVAPI_get_row(int *row);

/**
 * @brief colの取得
 * @param col:           colの値
 * @return CSVAPI_RETURN_OK:    成功
           CSVAPI_STATUS_ERR:   内部状態エラー
           CSVAPI_IO_ERR:       入出力エラー
*/
int CSVAPI_get_col(int *col);

/**
 * @brief 終了処理
 * @return CSVAPI_RETURN_OK:    成功
           CSVAPI_STATUS_ERR:   内部状態エラー
*/
int CSVAPI_finalize();

#endif
