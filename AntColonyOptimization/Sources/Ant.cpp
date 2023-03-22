#include "../headers/Ant.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

Ant::Ant()
{
	cost = 0;
}

bool Ant::buildSolution(vector<vector<edge>> &adjacency, int u, int v, float alfa, float beta)
{
	solution = vector<int>();
	cost = 0;
	solution.push_back(u);
	//enquanto não alcançar o destino
	while (solution.back()!=v)
	{
		vector<edge> validEdges = getValidEdges(adjacency,solution.back());

		//se nao houverem arestas validas para seguir, repare solucao
		if (validEdges.empty())
		{
			break;
			return repairSolution(adjacency, v);
		}
		float denominador = calculateSummation(validEdges,alfa,beta);

		//escolhe probabilisticamente qual aresta seguir
		choose(validEdges, denominador, alfa, beta);
	}
	return true;
}

void Ant::deposit(vector<vector<edge>>& adjacency,float weight)
{
	int u, v;
	for (int i = 1; i < solution.size(); i++)
	{
		//aresta (u,v) atual a ser atualizada
		u = solution[i - 1];
		v = solution[i];
		//encontra a aresta (u,v)
		auto it = find(adjacency[u].begin(), adjacency[u].end(), edge(v));
		//atualiza feromonio
		if(it != adjacency[u].end())
			it->feromonio += weight * cost;
	}
}

int Ant::getCost()
{
	return cost;
}

vector<int> Ant::getSolution()
{
	return solution;
}

//calcula denominador da fração de calculo de probabilidades
float Ant::calculateSummation(vector<edge>& valid, float alfa, float beta)
{
	float cumulativeValue = 0;
	//calcula o fator de normalização para o calculo de probabilidades
	for (auto& it : valid)
	{
		//considera arestas validas
		cumulativeValue += pow(it.cost,beta) * pow(it.feromonio,alfa);
	}
	return cumulativeValue;
}

//escolhe uma aresta para seguir
void Ant::choose(vector<edge>& valid, float denominador, float alfa, float beta)
{
	float cumulativeProbability = 0;
	//gera o numero aleatorio
	float choice = randomNumber();
	for (auto& it : valid) 
	{
		//define probabilidade de escolher aresta valida
		cumulativeProbability += (pow(it.cost,beta) * pow(it.feromonio,alfa)) / denominador;

		//escolhe aresta a seguir
		if (choice <= cumulativeProbability)
		{
			solution.push_back(it.destination);
			cost += it.cost;
			return;
		}
	}
}

//repara solução fazendo backtrack até uma valida
bool Ant::repairSolution(vector<vector<edge>>& adjacency,int end)
{
	edge lastEdge = edge(end);
	//enquanto for possível backtrack
	while (!solution.empty())
	{
		int currentNode = solution.back();
		auto it = find(adjacency[currentNode].begin(), adjacency[currentNode].end(), lastEdge);
		//caso encontre uma aresta válida para o fim, coloque ela e finalize a solução
		if (it != adjacency[currentNode].end())
		{
			solution.push_back(end);
			cost += it->cost;
			return true;
		}
		//caso contrário, remova o vertice e a respectiva aresta da solução
		else
		{
			solution.pop_back();
			edge removed = edge(currentNode);
			auto it = find(adjacency[solution.back()].begin(), adjacency[solution.back()].end(), removed);
			cost -= it->cost;
		}
	}
	//impossível reparar solução(não há solução válida de start para end)
	return false;
}

vector<edge> Ant::getValidEdges(vector<vector<edge>>& adjacency, int row)
{
	vector<edge> valid;
	for (auto& it : adjacency[row])
	{
		//adiciona arestas validas
		if (find(solution.begin(), solution.end(), it.destination) == solution.end())
			valid.push_back(it);
	}
	return valid;
}
