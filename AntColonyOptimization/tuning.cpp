// ACO.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include "headers/ACO.hpp"
#include <ctime>
#include <vector>
#include <fstream>

using namespace std;

bool file_exists(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

float testParameter(int ants, int itr,float p,float q,float alfa,float beta,vector<int> seeds,string filename)
{
    Ant Ibest;
    float meanBestCost=0;
    for(int j=0;j<seeds.size();j++)
    {
        srand(seeds[j]);
        ACO aco(ants,itr,p,q,alfa,beta);
        aco.setup(filename);
        Ibest = aco.run();
        meanBestCost+=Ibest.getCost();
    }
    meanBestCost = meanBestCost/seeds.size();
    return meanBestCost;
}

int main(int argc, char *argv[])
{
    string filename = (argc>1) ? argv[1]: "entrada1.txt";
    vector<int> ants = {5,10,15,20,30};
    vector<int> itr = {100,50,200,300};
    vector<float> p = {0.1,0.2,0.05,0.5};
    vector<float> q = {0.1,0.05,0.2,0.5};
    vector<float> alfa = {1,0.5,2,5};
    vector<float> beta = {5,2,1};
    vector<int> seeds = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
    int i;
    int bestAnt=ants[0],bestItr=itr[0];
    float bestP=p[0],bestQ=q[0],bestAlfa=alfa[0],bestBeta=beta[0];
    float meanBestCost,bestMeanCost=0;
    cout<<"iteration,best,worst,mean,best pathsize,mean pathsize,global best,number of ants,number of iterations,evaporacao,quantidade de feromonio,alfa,beta\n";
    //test ant size
    for(i=0;i<ants.size();i++)
    {
        meanBestCost=testParameter(ants[i],bestItr,bestP,bestQ,bestAlfa,bestBeta,seeds,filename);
        if(meanBestCost > bestMeanCost)
        {
            bestAnt=ants[i];
            bestMeanCost=meanBestCost;
        }
    }
	
    //test evaporation
    bestMeanCost=0;
    for(i=0;i<p.size();i++)
    {
        meanBestCost=testParameter(bestAnt,bestItr,p[i],bestQ,bestAlfa,bestBeta,seeds,filename);
        if(meanBestCost > bestMeanCost)
        {
            bestP=p[i];
            bestMeanCost=meanBestCost;
        }
    }

    //test quantity of pheromones
    bestMeanCost=0;
    for(i=0;i<q.size();i++)
    {
        meanBestCost=testParameter(bestAnt,bestItr,bestP,q[i],bestAlfa,bestBeta,seeds,filename);
        if(meanBestCost > bestMeanCost)
        {
            bestQ=q[i];
            bestMeanCost=meanBestCost;
        }
    }
	
	//test alfa
    bestMeanCost=0;
    for(i=0;i<alfa.size();i++)
    {
        meanBestCost=testParameter(bestAnt,bestItr,bestP,bestQ,alfa[i],bestBeta,seeds,filename);
        if(meanBestCost > bestMeanCost)
        {
            bestAlfa=alfa[i];
            bestMeanCost=meanBestCost;
        }
    }

    //test beta
    bestMeanCost=0;
    for(i=0;i<beta.size();i++)
    {
        meanBestCost=testParameter(bestAnt,bestItr,bestP,bestQ,bestAlfa,beta[i],seeds,filename);
        if(meanBestCost > bestMeanCost)
        {
            bestBeta=beta[i];
            bestMeanCost=meanBestCost;
        }
    }
	
    //test number of iterations
    bestMeanCost=0;
    for(i=0;i<itr.size();i++)
    {
        meanBestCost=testParameter(bestAnt,itr[i],bestP,bestQ,bestAlfa,bestBeta,seeds,filename);
        if(meanBestCost > bestMeanCost)
        {
            bestItr=itr[i];
            bestMeanCost=meanBestCost;
        }
    }
	testParameter(bestAnt,bestItr,bestP,bestQ,bestAlfa,bestBeta,seeds,filename);
    string parameters;
    //save best parameters in file
    if (!file_exists("Outputs/best_parameters.txt"))
    {
        parameters = "database,number of ants,number of iterations,evaporacao,quantidade de feromonio,alfa,beta\n";
    }
    parameters+=filename+','+to_string(bestAnt)+','+to_string(bestItr)+','+to_string(bestP)+','+to_string(bestQ)+','+to_string(bestAlfa)+','+to_string(bestBeta)+"\n";
    ofstream ParameterFile("Outputs/best_parameters.txt", ios_base::app);
    ParameterFile << parameters;
    ParameterFile.close();
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
