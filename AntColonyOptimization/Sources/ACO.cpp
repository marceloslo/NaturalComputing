#include "../Headers/ACO.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <float.h>

ACO::ACO(int ants, int itr, float p,float q,float alfa, float beta)
{
	this->construct(ants, itr, p,q, alfa, beta,1,FLT_MAX);
}

void ACO::setup(string filename)
{
	ifstream in(filename);
	int u, v, cost;
	// le o arquivo no formato u v custo, onde (u,v) é aresta
	while (in >> u >> v >> cost)
	{
		if (u > adjacency.size())
		{
			while(adjacency.size() < u)
				adjacency.push_back(vector<edge>());
		}
		adjacency[u - 1].push_back(edge(v-1, cost,minFeromonio));
	}
	//seta o feromonio maximo como o máximo custo possível para um caminho teoricamente
	maxFeromonio = adjacency.size() * 10;
	//seta o feromonio minimo como o minimo custo possivel para um caminho teoricamente
	minFeromonio = 1;
	//coloca o feromonio inicial = feromonio maximo
	for (int i = 0; i < adjacency.size(); i++)
	{
		for (auto& it : adjacency[i])
		{
			it.feromonio = maxFeromonio;
		}
	}
	return;
}

Ant ACO::run()
{
	string logging;
	int start = 0, end = adjacency.size()-1;
	Ant best;
	for (int iterations = 0; iterations < numberOfIterations; iterations++)
	{
		bool couldFindSolution = false;
		//formigas criam soluções
		for (auto& ant : ants)
		{
			bool success=ant.buildSolution(adjacency, start, end, alfa, beta);
			couldFindSolution = success || couldFindSolution;
		}
		//se não houver solução valida, encerre algoritmo
		if (!couldFindSolution)
		{
			break;
		}
		//atualiza melhor solução
		Ant iterationBest;
		iterationBest = findBest();
		if (iterationBest.getCost() > best.getCost())
			best = iterationBest;
		//atualiza feromonios
		//updatePheromones(best);
		updatePheromones(iterationBest);
		
		if (log)
		{
			logging += logIteration(iterations,best);
		}
	}
	cout << logging <<endl;
	return best;
}

//atualiza feromônios na adjacencia
void ACO::updatePheromones(Ant itrbest)
{
	//faz a evaporação de feromonio em todas arestas
	for (int row = 0; row < adjacency.size(); row++)
	{
		for (auto& it : adjacency[row])
		{
			it.feromonio = it.feromonio * (1-evaporation);
		}
	}
	//faz deposição de feromonios(apenas a melhor formiga da iteração)
	itrbest.deposit(adjacency, q);

	// trata feromonios fora dos limites de feromonio
	for (int row = 0; row < adjacency.size(); row++)
	{
		for (auto& it : adjacency[row])
		{
			//impede feromonio menor que o minimo
			if (it.feromonio < minFeromonio && it.feromonio >=0)
				it.feromonio = minFeromonio;
			//impede feromonio maior que o máximo e overflow
			else if (it.feromonio > maxFeromonio || it.feromonio < 0)
				it.feromonio = maxFeromonio;
		}
	}
}

Ant ACO::findBest()
{
	Ant best;
	int maxcost = 0;
	// encontra formiga com menor custo
	for (auto& ant : ants)
	{
		if (ant.getCost() > maxcost)
		{
			best = ant;
			maxcost = ant.getCost();
		}
	}
	return best;
}

void ACO::construct(int ants, int itr, float p,float q, float alfa, float beta,float minT,float maxT)
{
	numberOfAnts = ants;
	numberOfIterations = itr;
	this->ants = vector<Ant>(numberOfAnts);
	evaporation = p;
	this->alfa = alfa;
	this->beta = beta;
	minFeromonio = minT;
	maxFeromonio = maxT;
	this->q = q;
}

//log format = iteration,best,worst,mean,mean pathsize,global best\n
string ACO::logIteration(int itr,Ant best)
{
	float mean=0, meanPathsize=0;
	int bst=0, worst=INT_MAX, cost,best_path=0;
	//gerando estatísticas
	for (auto& ant : ants)
	{
		cost = ant.getCost();
		mean += cost;
		meanPathsize += ant.getSolution().size();
		if (cost >= bst)
		{
			bst = cost;
			best_path = ant.getSolution().size();
		}
		if (cost <= worst)
		{
			worst = cost;
		}
	}
	mean = mean / numberOfAnts;
	meanPathsize = meanPathsize / numberOfAnts;

	//formatando string
	string aux = to_string(itr) + ',' + to_string(bst) + ',' + to_string(worst)+ ',' + to_string(mean)+','
                +to_string(best_path)+',' + to_string(meanPathsize) +',' + to_string(best.getCost()) + ','
                +to_string(numberOfAnts)+','+to_string(numberOfIterations)+','+to_string(evaporation)+','+to_string(q)+','+to_string(alfa)+','+to_string(beta)+"\n";

	return aux;
}
