#ifndef ANT_H
#define ANT_H
#include <vector>
#include "Utils.hpp"

using namespace std;

class Ant
{
public:
	Ant();
	//gera uma solucao que vai de u a v de acordo com os feromonios e custos da adjacencia
	//retorna true se bem sucedida e false caso contrário
	bool buildSolution(vector<vector<edge>> &adjacency, int u, int v, float alfa, float beta);

	//deposita feromonios na matriz de adjacencia de acordo com a solucao atual e um peso
	void deposit(vector<vector<edge>>& adjacency,float weight);

	int getCost();

	vector<int> getSolution();

private:
	float calculateSummation(vector<edge>& valid, float alfa, float beta);
	void choose(vector<edge>& valid, float denominador,float alfa,float beta);
	bool repairSolution(vector<vector<edge>>& adjacency,int end);
	vector<edge> getValidEdges(vector<vector<edge>>& adjacency, int row);

	vector<int> solution;
	int cost;
};



#endif //ANT_H