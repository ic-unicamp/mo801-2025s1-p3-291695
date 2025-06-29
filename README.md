# Relatório Projeto 3
**Aluna:** Aline Santana Cordeiro

**RA:** 291695

## Introdução
O objetivo principal desse projeto é selecionar algum algoritmo de *Machine Learning* para execução na plataforma *[LiteX](https://github.com/enjoy-digital/litex)* e otimizá-lo, de forma a conseguir algum ganho de desempenho na execução da versão otimizada. Para tal, é necessário fazer as devidas medições de tempo na plataforma *LiteX* para comparar os tempos do código original e o acelerado. 
A ideia é usar o pacote *[Micromlgen](https://github.com/eloquentarduino/micromlgen)* do *Python* para gerar, automaticamente, um código em C a partir de um modelo treinado e escrito em *Python*. Esse modelo é transformado em uma função em C, que é responsável por operar os dados de entrada com pesos definidos e, até mesmo, somar votos, para fazer a classificação de uma entrada dada pelo usuário.

No meu caso, executei todo o projeto no simulador da plataforma *LiteX*, usando os registradores de tempo da própria plataforma para registro do tempo de execução da aplicação, que é uma **Regressão Logística** usando a base de dados **Íris**.

## Informações gerais sobre o uso das aplicações
### Instalação e configuração do ambiente
Antes de configurar meu ambiente de testes, criei um ambiente virtual com o *Python* onde fiz a instalação dos pacotes *scikit-learn*, *micromlgen* e *ipykernel* para a execução da aplicação escolhida para o desenvolvimento do projeto.

### Arquivos do repositório
* **logistic_regression.py:** *Script Python* que gera o modelo de Regressão Logística com a base de dados Íris.
* **logistic_regression.c:** Código fonte gerado a partir do *script Python*, com *Micromlgen*. 
* **logistic_regression.h:** Arquivo *header* gerado a partir do script Python, com *Micromlgen*.
* **litex/soc/software/demo/litex_time.c:** Código fonte com funções auxiliares para medição do tempo de execução.
* **litex/soc/software/demo/litex_time.h:** Arquivo *header* com funções auxiliares para medição do tempo de execução.
* **litex/soc/software/demo/logregr.c:** Código fonte do *baseline*, é a modificação básica do código **logistic_regression.c** de forma a funcionar corretamente no simulador.
* **litex/soc/software/demo/logregr.h:** Arquivo *header* do *baseline*.
* **litex/soc/software/demo/opt_logregr.c:** Código fonte do *baseline* otimizado.
* **litex/soc/software/demo/opt_logregr.h:** Arquivo header do *baseline* otimizado.
* **litex/soc/software/demo/Makefile:** Adições necessárias para execução do *baseline* e *baseline* otimizado no simulador do *LiteX*.
* **litex/soc/software/demo/main.c:** Adições necessárias para execução do *baseline* e *baseline* otimizado no simulador do *LiteX*.

### Execução das aplicações
Para a geração dos arquivos **logistic_regression.c** e **logistic_regression.h**:
```
python3 logistic_regression.py
```

Para compilar e abrir a simulação do *LiteX*:
```
litex_sim --integrated-main-ram-size=0x10000 --cpu-type=vexriscv --no-compile-gateware
litex_bare_metal_demo --build-path=build/sim
litex_sim --integrated-main-ram-size=0x10000 --cpu-type=vexriscv --ram-init=demo.bi
```

Para execução dos modelos no *LiteX*:
```
logregr
opt_logregr
```
![litex_sim](<Screenshot from 2025-06-29 20-24-11.png>)

## Seleção de algoritmos de Machine Learning
Para fazer a escolha do algoritmo a ser executado na plataforma *LiteX*, o primeiro passo foi buscar alguns códigos de exemplo no site do *[scikit-learn](https://scikit-learn.org/1.5/index.html)*. Fui executando testes de diferentes tipos de algoritmos e modelos e entendendo como funcionava a ferramenta *Micromlgen* e como eram os resultados obtidos. 

Primeiramente, entendi que alguns códigos não eram opções viáveis, dado que o *Micromlgen* não dava suporte para os modelos implementados. Por exemplo, alguns códigos implementavam um *pipeline* com vários tipos de modelos diferentes usados em conjunto, enquanto que outros códigos implementavam modelos apenas não suportados pelo *Micromlgen*. No github da ferramenta é listado os tipos de calculos possíveis e alguns exemplos de uso.

Dessa maneira, testei executar exemplos usando os seguintes modelos:
* *DecisionTree*;
* *RandomForest*;
* *Support Vector Machines (SVM)*;
* *Principal Component Analysis (PCA)*;
* *Logistic Regression*; e
* *Linear Regression*.

A partir dos testes feitos, gerei o código em C e fui entendendo os padrões de implementação gerados pelos diferentes tipos de modelos. 
Foi possível entender que trabalhar com algoritmos de árvore seria bastante trabalhoso, dado que, em C, é gerada uma cadeia de condicionais *(if-else)* e que seu tamanho depende do número de entradas/parâmetros usados pelo modelo, o que pode gerar códigos extensivos e com pouca opção de otimização, dado que cada condicional acaba executando uma única operação simples. 

Já o *SVM* e *PCA* são muito parecidos, mas o tamanho do código C gerado depende muito da base de dados usada para treino. Eles geram uma função privada mais contida e enxuta, porém usando operações complexas como exponencial e potência de dois e, dependendo do número de *kernels* da função pública de predição, o código pode se tornar muito extenso, com muitas chamadas à essa função privada. Seria possível tentar otimizar as inúmeras chamadas da função privada usando *inline* na declaração dela e, talvez substituir as operações complexas por operações lógicas.

Por fim, os modelos que usam regressão são extremamente parecidos entre si e geram um código mais enxuto, independente da base de dados usada. A Regressão Logística gera um código em C com complexidade computacional um pouco maior que a Regressão Linear, mas ainda assim, o padrão de ambos é bem parecido. Por isso, se torna um código mais fácil de trabalhar de forma a identificar pontos de otimização. Talvez, a versão otimizada da Regressão não seja tão expressiva quanto outros modelos podem ser, porém os pontos de otimização existem e serão usados para exploração nesse projeto.

### Casos de teste do site Scikit-Learn

* **[L1 Penalty and Sparsity in Logistic Regression](https://scikit-learn.org/stable/auto_examples/linear_model/plot_logistic_l1_l2_sparsity.html#sphx-glr-auto-examples-linear-model-plot-logistic-l1-l2-sparsity-py):** Esse exemplo faz a comparação de matrizes esparsas quando as penalidades *Lasso regression (L1)*, *Ridge Regression (L2)* e *Elastic-Net* são usados ​​para diferentes valores de C. Esse exemplo faz uso da base *[MNIST](https://www.kaggle.com/datasets/hojjatk/mnist-dataset)* e treina o modelo com Regressão Logística, porém, não ficou claro, para mim, os resultados esperados de regressão/classificação para esse modelo. Esse algoritmo separa o conjunto de números em duas classes distintas, 0-4 classificados na classe 0 e 5-9 classificados na classe 1, porém, essa verificação não é direta. Apesar de conseguir executar o binário gerado e obter uma classicação de 0 ou 1 para os números da base de dados (de 0 a 9), não fez muito sentido a obtenção desses resultados com as matrizes esparsas geradas pelo script Python, eu não sabia fazer uma comparação direta. Fiz algumas buscas sobre as funções de regressão, mas ainda assim, achei melhor trabalhar com algo que tivesse um resultado mais direto e de fácil compreensão, dado que é necessário entender os dados que estamos usando e a melhor forma de trabalhar com eles para saber de que forma podemos aplicar maiores otimizações.  

* **[Recognizing hand-written digits](https://scikit-learn.org/stable/auto_examples/classification/plot_digits_classification.html):** Na segunda tentativa, segui para a aplicação de identificação e classificação de números, ainda usando a base de dados *MNIST*. O modelo usado no exemplo é o *SVM* e o código gerado em C tinha mais de 3 mil linhas e contava com muitos vetores de *kernel* e decisão indexados em sequência chamando uma função privada para calcular os valores de entrada com os diversos pesos gerados pelo modelo. Por algum motivo, não consegui obter a classificação correta dos vetores de característica que usei de entrada ao executar o binário gerado, logo segui para a terceira opção.

* **[Logistic Regression 3-class Classifier](https://scikit-learn.org/1.5/auto_examples/linear_model/plot_iris_logistic.html):** Por fim, segui para uma Regressão Logística usando a base de dados *[Íris](https://scikit-learn.org/1.4/auto_examples/datasets/plot_iris_dataset.html)*, que teria um conjunto de dados mais reduzido e poderia ser usada de forma mais direta. Esse banco de dados contém 3 tipos de Íris, *Setosa*, *Versicolour* e *Virginica* e as características listadas no vetor são o comprimento da sépala, a largura da sépala, o comprimento da pétala e a largura da pétala. Como observado com os testes anteriores, o código C gerado é contido e consegui fazer a classificação correta dos dados usando o binário gerado e decidi continuar trabalhando com esse exemplo para execução na plataforma *LiteX* e otimização do mesmo.

## Geração do código em C

O código fonte e *header* são gerados num misto de C e C++, dado que a extensão dos arquivos é feita em C, mas o *header* importa, automaticamente, a biblioteca *cstdarg* para trabalhar com o tipo de dados **va_list** que é adicionado pelo *Micromlgen*. Para compilação correta do programa, é necessário instanciar um objeto no arquivo fonte para fazer uso da função de predição, escrita no arquivo de *header* e adicionar a biblioteca *cstdint* para reconhecer os tipos de inteiros gerados no código.

Fazendo esses pequenos ajustes, foi possível compilar o código para gerar o binário e executar alguns testes para verificar a precisão do modelo treinado e representado em código C. Para executar esses testes, ainda com o script *Python*, eu imprimi na tela alguns vetores de características e seus respectivos rótulos e os usei como entradas válidas no código C, instanciando-os no início do código para serem passados como parâmetro da função de predição.

### Portabilidade do código original na plataforma LiteX
Da mesma forma como feito no Projeto 2, integrei os códigos, versão original e otimizada, no simulador do *LiteX*, junto das demonstrações *Donut* e *Hello C*. No caso, adicionei os arquivos com o código na pasta *litex/litex/soc/software/demo* e fiz algumas alterações nos arquivos *Makefile* e *main.c* para identificar e executar corretamente os códigos de Regressão Logística.

Assim que o código é integrado no simulador, ao compilá-lo, é possível observar que as bibliotecas de C++ não são suportadas pelo simulador do *LiteX*, logo, segui fazendo as seguintes substituições:
* Substituição da biblioteca **cstdarg** pela biblioteca **stdarg.h**;
* Substituição da biblioteca **cstdint** pela biblioteca **stdint.h**;
* Remoção dos **namespaces** e **classes**;
* Criação de protótipo para as funções do *header*;
* Mudança de ordem na instanciação das funções, para que a função **dot** seja identificada pela função **predict**; e
* Remoção da estrutura **va_list** dado que a função **va_arg** assume formato *double* quando um tipo *float* é passado para ela, e o formato *double* não é aceito pelo compilador do simulador do *LiteX*. Consequentemente, remoção da biblioteca **stdarg.h**.

Após todos os ajustes, é possível adicionar as funções para medição do tempo de execução. Podemos observar que, só na adaptação do código gerado pelo *Micromlgen* para o simulador do *Litex*, algumas alterações já podem ser consideradas otimizações, como a remoção da biblioteca **stdarg.h** e a estrutura **va_list**. Provavalmente existe um custo de alocação e instanciação dessa estrutura para fazer a operação de vetores com tamanhos não definidos. No caso atual, sabemos que os vetores têm tamanho definido, dado que a base Íris contém 4 características diferentes para cada entrada. Porém, como é esta a versão que executa corretamente no simulador do *LiteX*, ele é nosso *baseline* e é com base nele que as otimizações devem ocorrer.

### Cálculo do tempo de execução
Para fazer a estimativa de tempo de execução, tentei fazer uso da solução apresentada pelo Professor em sala de aula e que está disponível no *Google Drive* da disciplina, porém, não obtive nenhum resultado. Lendo um pouco mais a respeito, entendi que a solução é válida para aplicações em *bare-metal*, logo, entendo que não é válida para o simulador. 

Assim, fiz uso da solução implementada no *Projeto 2*, em que alguns registradores de tempo são configurados e lidos para fazer a devida estimativa de ciclos. Essa estivativa é, então, convertida para milisegundos para impressão na tela. Como será necessário medir o tempo de mais de um arquivo, criei dois novos arquivos em C, um sendo o arquivo fonte e outro o header, para manter apenas as funções de cálculo de tempo. Além disso, inclui esses novos arquivos no *Makefile* para serem ligados corretamente aos demais.

## Otimização do Código e resultados obtidos
Considerando o código apresentado nos arquivos **logregr.c** e **logregr.h** como *baseline* e que esse código foi compilado com a flag de otimização **-O0**, as seguintes otimizações foram feitas de forma incremental, usando como base, a última otimização mais relevante, e o tempo de cada uma será registrada na sequência:

1. **Baseline: 0,41s**
2. **1** + *flag* de otimização **-O3**: **0,36s**
3. **2** + *inline* da função *dot* + *loop unrolling* da função *dot* + retorno direto sem declaração de variável auxiliar: **0,36s**
4. **3** + remoção da função *dot*: **0,36s**
5. **4** + *loop unrolling* da função *predict*: **0,12s**
6. **5** + utilização de números inteiros apenas: **0,13s** (porém, perde a precisão do modelo)
7. **5** + utlização de pesos e entradas normalizados: **0,12s** (porém, perde a precisão do modelo)
8. **5** + redução do número de casas decimais para 3: **0,12s**
9. **8** + *inline* da função *predict*: **0,12s**
10. **9** + remoção do vetor de votos: **0,12s**
11. **9** + ponteiros para acessar o vetor pelo endereço de cada índice: **0,12s**

Para cada caso de teste, o tempo obtido foi o resultado da média de 10 execuções da mesma aplicação e considerou a classificação de 6 entradas específicas, sendo duas de cada classe, definidas no próprio código fonte.

Podemos observar que as alterações aplicadas ao código e citadas acima foram feitas de forma incremental, porém é possível notar que as mais relevantes são a **2** e **5**, que são, o uso da *flag* de compilação para otimização **-O3** e o *loop unrolling* da função de predição com reestruturação da condição, respectivamente. Com o uso da *flag* **-O3**, o ganho foi de **12%** em comparação com o *baseline* e com o uso do *loop unrolling* com reestruturaçao da condicional, o ganho foi de **66,6%** em cima da otimização **2** e de **70%** em comparação com o *baseline*. Apesar de ser um laço pequeno, transformá-lo em duas condicionais trouxe um ótimo desempenho computacional.

Quanto às outras modificações aplicadas ao código, como *inline* ou remoção de funções auxiliares para evitar um maior número de chamadas de função, pouco surtiram efeito dado que a *flag* **-O3** já deve ter aplicado o *inline* automaticamente. Também avaliei o tipo dos dados, substituindo valores *float* por inteiros, porém essa alteração não gerou redução no tempo de execução da aplicação e ainda alterou os resultados de classificação. Também avaliei normalizar os valores referentes a pesos e entradas com a função *L2-Norm*, porém, o tempo de execução não mudou, enquanto que diminuiu a precisão na classificação do modelo. Por fim, tentei reduzir o número de números decimais para 3 casas, porém isso não fez nenhuma diferença nos resultados, nem no tempo de execução. 

Outra alteração foi em relação ao acesso dos índices do vetor, ao invés de acessar com índexes, usar o endereço base do vetor e usar deslocamento para acessar as outras posições, com ponteiros. No caso, não teve diferença, porém é esperado um desempenho melhor em casos com vetores maiores.

O código otimizado pode ser encontrado nos arquivos **opt_logregr.c** e **opt_logregr.h**

## Considerações finais
Esse projeto trouxe uma boa ideia sobre aceleração de algoritmos de *Machine Learning* ou outros algoritmos. Fica claro que existem diversas técnicas de otimização de código e que, aplicando as técnicas corretas nos trechos corretos de código, pode trazer um desempenho computacional muito grande, por mais simples que a aplicação seja. 

Imaginei que a aplicação que escolhi não teria um ganho tão bom quanto o obtido, justamente porque estava trabalhando com um algoritmo simples e enxuto que usa uma base de dados pequena com poucas características. Porém ao usar as flags corretas de otimização e ao restruturar um laço com condicional, foi possível obter 70% de aceleração se comparado com o *baseline*, o que é um resultado muito bom.

Fiquei curiosa para saber se com o código gerado pela classificação usando *SVM* e a base de dados *MNIST*, em que é gerado um código com mais de 3 mil linhas e que, em cada linha, é feita a chamada da função *dot* com as operações necesssárias para esse modelo específico, porém que não deixa de ser simple e enxuto como o usado nesse projeto, se a *flag* **-O3** já faria o *inline* por si só (eu acredito que sim, por conta da simplicidade da função *dot*), mas se o desempenho obtido somente nessa otimização seria significativo. Vai ficar para trabalhos futuros.
