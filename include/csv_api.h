#ifndef CSV_API_H_
#define CSV_API_H_
#define CSVAPI_INDEX(row, col, dim) ((row) * (dim) + (col))

enum CSVAPI_RETURN_TYPE {
    RETURN_OK = 0,
    INTERNAL_ERR = 1,
    STATUS_ERR = 2,
    ARGUMENT_ERR = 3,
    IO_ERR = 4,
    MALLOC_ERR = 5,
};

/**
 * @brief 初期化処理
 * @param csv_filename:  CSVファイル名
 * @return RETURN_OK:    成功
           ARGUMENT_ERR: 引数エラー
           STATUS_ERR:   内部状態エラー
           MALLOC_ERR:   mallocエラー
*/

enum CSVAPI_RETURN_TYPE CSVAPI_initialize(const char *csv_filename);

/**
 * @brief データ読み込み処理
 * @param data:          読み込んだデータの格納先
 * @return RETURN_OK:    成功
           STATUS_ERR:   内部状態エラー
           ARGUMENT_ERR: 引数エラー
           IO_ERR:       入出力エラー
           MALLOC_ERR:   mallocエラー
           INTERNAL_ERR: 内部エラー
*/
enum CSVAPI_RETURN_TYPE CSVAPI_read_data(int **data);

/**
 * @brief データの書き込み
 * @param filename       書き込み先
 * @param data           書き込むデータ
 * @return RETURN_OK:    成功
           STATUS_ERR:   失敗
           ARGUMENT_ERR: 引数エラー
           IO_ERR:       入出力エラー
           INTERNAL_ERR: 内部エラー
*/
enum CSVAPI_RETURN_TYPE CSVAPI_write_data(const char *filename, int *data);

/**
 * @brief rowの取得
 * @param row:           rowの値
 * @return RETURN_OK:    成功
           STATUS_ERR:   内部状態エラー
           IO_ERR:       入出力エラー
*/
enum CSVAPI_RETURN_TYPE CSVAPI_get_row(int *row);

/**
 * @brief colの取得
 * @param col:           colの値
 * @return RETURN_OK:    成功
           STATUS_ERR:   内部状態エラー
           IO_ERR:       入出力エラー
*/
enum CSVAPI_RETURN_TYPE CSVAPI_get_col(int *col);

/**
 * @brief 終了処理
 * @return RETURN_OK:    成功
           STATUS_ERR:   内部状態エラー
*/
enum CSVAPI_RETURN_TYPE CSVAPI_finalize();

#endif
