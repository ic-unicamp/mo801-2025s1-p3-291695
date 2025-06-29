
#include "logistic_regression.h"
#include <stdio.h>

int main() {
    Eloquent::ML::Port::LogisticRegression logistic_regression;

    float class00[] = { 5.1, 3.5, 1.4, 0.2 };
    float class01[] = { 4.6, 3.6, 1.0, 0.2 };
    float class10[] = { 7.0, 3.2, 4.7, 1.4 };
    float class11[] = { 6.3, 2.3, 4.4, 1.3 };
    float class20[] = { 6.9, 3.2, 5.7, 2.3 };
    float class21[] = { 6.8, 3.2, 5.9, 2.3 };
  
  printf("Predição: %d\n", logistic_regression.predict(class00));
  printf("Predição: %d\n", logistic_regression.predict(class01));
  printf("Predição: %d\n", logistic_regression.predict(class10));
  printf("Predição: %d\n", logistic_regression.predict(class11));
  printf("Predição: %d\n", logistic_regression.predict(class20));
  printf("Predição: %d\n", logistic_regression.predict(class21));
  return 0;
}
