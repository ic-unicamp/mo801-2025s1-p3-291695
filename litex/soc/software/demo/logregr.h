#pragma once
#include <stdint.h>

/**
* Compute dot product
*/
float dot(float *x, float *w);
float dot(float *x, float *w) {
    float dot = 0.0;

    for (uint16_t i = 0; i < 4; i++) {
        dot += x[i] * w[i];
    }

    return dot;
}

/**
* Predict class for features vector
*/
int predict(float *x);
int predict(float *x) {
    float w1[] = {7.654681129234  , 20.377916077178  , -29.736245320299  , -13.922231922034};
    float w2[] = {-2.597806101213  , -6.852117286904  , 10.158527217855  , -2.146585996267};
    float w3[] = {-5.056875028021  , -13.525798790273  , 19.577718102444  , 16.068817918302};
    float votes[3] = { 4.012632820547 ,19.257392337132 ,-23.270025157679  };
    votes[0] += dot(x, w1);
    votes[1] += dot(x, w2);
    votes[2] += dot(x, w3);
    // return argmax of votes
    uint8_t classIdx = 0;
    float maxVotes = votes[0];

    for (uint8_t i = 1; i < 3; i++) {
        if (votes[i] > maxVotes) {
            classIdx = i;
            maxVotes = votes[i];
        }
    }

    return classIdx;
}
