#include <stdint.h>

/**
* Predict class for features vector
*/
inline int opt_predict(float *x);
inline int opt_predict(float *x) {
    float votes[3] = {4.012f, 19.257f, -23.270f};
    *(votes) += (x[0] * 7.654f) + (x[1] * 20.377f) + (x[2] * -29.736f) + (x[3] * -13.922f);
    *(votes + 1) += (x[0] * -2.597f) + (x[1] * -6.852f) + (x[2] * 10.158f) + (x[3] * -2.146f);
    *(votes + 2) += (x[0] * -5.056f) + (x[1] * -13.525f) + (x[2] * 19.577f) + (x[3] * 16.068f);
    // return argmax of votes
    return (*(votes + 1) > *(votes)) ? ((*(votes + 2) > *(votes + 1)) ? 2 : 1) : ((*(votes + 2) > *(votes)) ? 2 : 0);
}
