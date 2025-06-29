# -----------------------------------------------------------------------------
# https://scikit-learn.org/1.5/auto_examples/linear_model/plot_iris_logistic.html
# Code source: Gaël Varoquaux
# Modified for documentation by Jaques Grobler
# License: BSD 3 clause

from sklearn import datasets
from sklearn.linear_model import LogisticRegression

# import some data to play with
iris = datasets.load_iris()
X = iris.data
Y = iris.target

# Create an instance of Logistic Regression Classifier and fit the data.
logreg = LogisticRegression(C=1e5)
logreg.fit(X, Y)

# Print coefficients used into C code input
print(X[0], Y[0])
print(X[22], Y[22])
print(X[50], Y[50])
print(X[87], Y[87])
print(X[120], Y[120])
print(X[143], Y[143])

# -----------------------------------------------------------------------------
from micromlgen import port

# Exporta o modelo logreg treinado para código C (TinyML)
with open('logistic_regression.h', 'w') as f:
  f.write(port(logreg))
print("Modelo exportado para logistic_regression.h")

c_code = """
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
  
  printf("Predição: %d\\n", logistic_regression.predict(class00));
  printf("Predição: %d\\n", logistic_regression.predict(class01));
  printf("Predição: %d\\n", logistic_regression.predict(class10));
  printf("Predição: %d\\n", logistic_regression.predict(class11));
  printf("Predição: %d\\n", logistic_regression.predict(class20));
  printf("Predição: %d\\n", logistic_regression.predict(class21));
  return 0;
}
"""
with open('logistic_regression.c', 'w') as f:
  f.write(c_code)
print("Código C salvo em logistic_regression.c")

# -----------------------------------------------------------------------------