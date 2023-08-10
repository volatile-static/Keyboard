
#include "matrix.h"
#include "wait.h"

static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS, 
                   col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

void matrix_init_custom(void) {
    for (uint8_t i = 1; i < MATRIX_ROWS; ++i)
        setPinOutput(row_pins[i]);
    for (uint8_t i = 0; i < MATRIX_COLS; ++i)
        setPinInputHigh(col_pins[i]);
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool changed = false;
    for (int i = 0; i < MATRIX_ROWS; ++i) {
        if (i > 0) 
            writePinHigh(row_pins[i - 1]);
        writePinLow(row_pins[i]);
        wait_us(6);

        matrix_row_t cols = 0;
        for (uint8_t i = 0; i < MATRIX_COLS; ++i)
            cols |= !readPin(col_pins[i]) << i;
            
        changed |= cols != current_matrix[i];
        current_matrix[i] = cols;
    }
    writePinHigh(row_pins[MATRIX_ROWS - 1]);
    return changed;
}