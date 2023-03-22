// ACO.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include "headers/ACO.hpp"
#include <ctime>

int main(int argc, char *argv[])
{
	if(argc<7)
		return -1;
    int ants, itr;
	string database;
    float p, alfa, beta,q;
	ants=stoi(argv[1]);
	itr=stoi(argv[2]);
	p = stod(argv[3]);
	q = stod(argv[4]);
	alfa = stod(argv[5]);
	beta = stod(argv[6]);
	database = argv[7];
    ACO a(ants,itr,p,q,alfa,beta);
    srand(static_cast <unsigned> (time(0)));
    a.setup(database);
    Ant best = a.run();
    return 0;
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar:
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
