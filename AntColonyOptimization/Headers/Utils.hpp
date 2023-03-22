#pragma once
#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>

//modela uma aresta como possuindo custo, feromonio e destino
//o vertice de onde a aresta sai é facilmente deduzido pela matriz de adjacencia
class edge
{
public:
	edge(int u, int c,float f) {
		cost = c;
		destination = u;
		feromonio = f;
	};
	edge(int u) {
		cost = 1;
		destination = u;
		feromonio = 0;
	}
	bool operator==(const edge& other) const {
		return other.destination == this->destination;
	};
	int cost, destination;
	float feromonio;
};

//gera um numero de 0 a 1
inline float randomNumber()
{
	return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}

#endif // !UTILS_H
