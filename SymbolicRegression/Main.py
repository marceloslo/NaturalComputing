from distutils.core import setup
import numpy as np
import pandas as pd
import sys
import random
import matplotlib.pyplot as plt
import RegressaoSimbolica
from RegressaoSimbolica import SymbolicRegression,solve

def readCmd():
    parameters = sys.argv
    pop = int(parameters[1])
    gen = int(parameters[2])
    cross = float(parameters[3])
    mut = float(parameters[4])
    tourn = int(parameters[5])
    elitism = bool(parameters[6])
    df = parameters[7]
    test = parameters[8]
    try:
        seed = int(parameters[9])
    except:
        seed = random.randint(0,20000)
    return pop,gen,cross,mut,tourn,df,elitism,seed,test

def Setup(db):
    df = pd.read_csv(db,header=None)
    labels = ["X"+str(i) for i in range(len(df.columns))]
    labels[-1] = "Y"
    df.columns = labels

    #internal tree nodes (operators)
    RegressaoSimbolica.internal = {
            "sub":"op",
            "add":"op",
            #"div":"op",
            "mul":"op"
    }

    #external tree nodes(terminals)
    RegressaoSimbolica.external={
        "number":"terminal"
    }

    for i in df.columns[:-1]:
        RegressaoSimbolica.external[i]="terminal"

    RegressaoSimbolica.lookup = {**RegressaoSimbolica.internal,**RegressaoSimbolica.external}    

    RegressaoSimbolica.variables=list(RegressaoSimbolica.lookup.keys())
    df = df.to_dict("records")
    return df

def SetupTest(db):
    df = pd.read_csv(db,header=None)
    labels = ["X"+str(i) for i in range(len(df.columns))]
    labels[-1] = "Y"
    df.columns = labels
    #df=df.sort_values(by='Y')
    df = df.to_dict('records')
    return df

def plotPrediction(tree,lookup,test,label):
    plt.figure(figsize=(16,6))
    x = [i for i in range(len(test))]
    y = [k['Y'] for k in test]
    y_ = [solve(tree.root,lookup,k) for k in test]
    plt.plot(x,y,label="data")
    plt.plot(x,y_,label="prediction")
    plt.legend()
    plt.xlabel('data point')
    plt.ylabel('Y')
    plt.title(label + ' Prediction')
    plt.show()

if __name__=="__main__":
    pop,gen,cross,mut,tourn,db,elitism,seed,test = readCmd()
    random.seed(seed)
    #setup training data
    df = Setup(db)
    S = SymbolicRegression(pop,gen,cross,mut,tourn,df,elitism)
    best,lastpop = S.geneticAlgorithm()
    print("Best train Fitness:"+str(best.evaluate(df)))
    df = SetupTest(test)
    plotPrediction(best,RegressaoSimbolica.lookup,df,test)
    print("Best test Fitness:"+str(best.evaluate(df)))
    