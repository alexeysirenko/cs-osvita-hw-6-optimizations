#include "vec.h"


data_t dotproduct(vec_ptr u, vec_ptr v) {

    // call vec_length once bc compiler does not know 
    // whether it has side effects, so makes calls each time
    long len = vec_length(u);
    data_t *u_data = get_vec_start(u);
    data_t *v_data = get_vec_start(v);

    data_t sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;

    // 4 core-parallel summations
    long i = 0;
    for (; i < len - 3; i += 4) {
        sum0 += u_data[i]     * v_data[i];
        sum1 += u_data[i + 1] * v_data[i + 1];
        sum2 += u_data[i + 2] * v_data[i + 2];
        sum3 += u_data[i + 3] * v_data[i + 3];
    }

    // collect the rest of the values that are not % 4
    for (; i < len; i++) {
        sum0 += u_data[i] * v_data[i];
    }

    return sum0 + sum1 + sum2 + sum3;
}
