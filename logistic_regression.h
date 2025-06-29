#pragma once
#include <cstdarg>
#include <cstdint>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class LogisticRegression {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        float votes[3] = { 4.012632820547 ,19.257392337132 ,-23.270025157679  };
                        votes[0] += dot(x,   7.654681129234  , 20.377916077178  , -29.736245320299  , -13.922231922034 );
                        votes[1] += dot(x,   -2.597806101213  , -6.852117286904  , 10.158527217855  , -2.146585996267 );
                        votes[2] += dot(x,   -5.056875028021  , -13.525798790273  , 19.577718102444  , 16.068817918302 );
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

                protected:
                    /**
                    * Compute dot product
                    */
                    float dot(float *x, ...) {
                        va_list w;
                        va_start(w, 4);
                        float dot = 0.0;

                        for (uint16_t i = 0; i < 4; i++) {
                            const float wi = va_arg(w, double);
                            dot += x[i] * wi;
                        }

                        return dot;
                    }
                };
            }
        }
    }