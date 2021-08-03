### Programação Concorrente - Projeto (Entrega 1)
#### Alunos: Cícero José e Cícero Samuel
<br>

#### Algoritmos a serem implementados:

##### Crivo de Eratóstenes

Iremos implementar o Crivo de Eratóstenes serial e paralelo (com MP e MPI) inserindo uma entrada de no mínimo 10.000.000 (dez milhões) para a análise de desempenho das implementações.

O algoritmo tem por objetivo encontrar todos os números primos entre $ [2, N] $, sendo $N$ um número natural definido pelo programador.

O algoritmo está descrito a seguir:

1. Escreva todos os números de 2 até N. Inicialmente, nós vamos considerá-los todos primos. 
1. Pegue o primeiro número que ainda não foi marcado como composto (número que possui mais que 2 divisores distintos) e diga que ele é primo.
1. A partir desse número primo, percorra todos os seus múltiplos até N e marque-os como compostos. Eles necessariamente são compostos pois possuem como divisores, pelo menos, o número 1, o primo atual e eles mesmos.
2. Volte para o passo 2 até chegarmos a $\sqrt{N}$.

#### Análise de desempenho:

Para a análise de desempenho será executado 15 vezes os mesmos algoritmos (serial, paralelo com MP e paralelo com MPI) na mesma máquina e nas mesmas condições.

Depois será feita a média desses valores e calculado algumas medidas de dispersão. 

Por fim, calcularemos o speedup e eficiência de cada programa paralelo.