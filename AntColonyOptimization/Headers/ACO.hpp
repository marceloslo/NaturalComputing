#ifndef ACO_H
#define ACO_H
#include "Ant.hpp"
#include "Utils.hpp"
#include <vector>
#include <string>

using namespace std;

class ACO
{
public:
	/***
	* Inicializa o algoritmo onde:
	* ants = numero de formigas
	* itr = numero de iteracoes
	* t0 = feromonio inicial
	* p = taxa de evaporacao
	* q = taxa de liberação de feromonio
	* alfa = importancia do feromonio
	* beta = importancia da heuristica
	***/
	ACO(int ants,int itr,float p,float q,float alfa,float beta);

	//le arquivo e gera a matriz de adjacencia
	void setup(string filename);


	//executa o algoritmo de ant colony e retorna a melhor formiga
	Ant run();


private:
	void updatePheromones(Ant itrbest);
	Ant findBest();
	void construct(int ants, int itr, float p, float q,float alfa, float beta,float minT,float maxT);
	string logIteration(int itr,Ant best);

	vector<Ant> ants;
	vector<vector<edge>> adjacency;
	int numberOfIterations, numberOfAnts;
	float evaporation,alfa,beta,q;
	float minFeromonio, maxFeromonio;
	bool log=true;
};


#endif // !ACO_H

