from cgitb import lookup
import numpy as np
from collections import deque,defaultdict
import random
import math
import time


#requisites: define lookup,external and internal nodes in file
#define variables as lookup keys
lookup={}
internal={}
external={}
variables=[]


def calculate(left,right,operator):
    if operator == "add":
        return left + right
    elif operator == "sub":
        return left - right
    elif operator == "mul":
        return left * right
    elif operator == "div":
        if right == 0:
            return 1
        return left/right
    elif operator == "exp":
        return np.exp(left)

def findType(variable):
    return variable if type(variable) == str else "number"

def solve(node,lookup,attributes):
    if node:
        nodeType = findType(node.value)
        if lookup[nodeType] == "terminal":
            if type(node.value)==float:
                return node.value
            return attributes[node.value]
        left = solve(node.left,lookup,attributes)
        right = solve(node.right,lookup,attributes)
        
        return calculate(left,right,node.value)

class node:
    def __init__(self):
        self.left = None
        self.right = None
        self.parent = None
        self.value = None
        self.level = 0
        
    def fromParent(self,parent):
        self.left = None
        self.right = None
        self.parent = parent
        self.value = None
        self.level = parent.level+1
        
    def __str__(self, level=0):
        ret = "\t"*level+repr(self.value)+"\n"
        if self.left:
            ret += self.left.__str__(level+1)
        if self.right:
            ret += self.right.__str__(level+1)
        return ret
    
    def copy(self):
        n = self.recursiveCopy(None)
        return n
    
    def recursiveCopy(self,parent):
        n = node()
        n.parent = parent
        n.value = self.value
        n.level = self.level
        if self.left:
            n.left = self.left.recursiveCopy(n)
        if self.right:
            n.right = self.right.recursiveCopy(n)
        return n
    
    def grow(self,maxsize):
        
        self.value=variables[random.randint(0,len(variables)-1)]
        
        #trata criação de operador valido
        if lookup[self.value] == "op" and self.level<maxsize:
            #teste para mutação, não precisa mudar esquerda e direita se já existir
            if self.left and self.right:
                return None
            self.left=node()
            self.left.fromParent(self)
            self.right=node()
            self.right.fromParent(self)
            level1=self.left.grow(maxsize)
            level2=self.right.grow(maxsize)
            return max(level1,level2)
        
        #trata criação de operador invalido (transforma em terminal)
        elif lookup[self.value] == "op":
            self.value = list(external.keys())[random.randint(0,len(external.keys())-1)]
        
        #define valor de constantes
        if self.value == "number":
            self.value=random.uniform(-1,1)
        
        #Garante que se um operador for mutado para terminal os filhos são removidos
        self.left=None
        self.right=None
        
        return self.level
    
    def full(self, maxsize):
        if self.level < maxsize:
            self.value = list(internal.keys())[random.randint(0,len(internal.keys())-1)]
        else:
            self.value = list(external.keys())[random.randint(0,len(external.keys())-1)]
            
        #trata criação de operador
        if lookup[self.value] == "op":
            self.left=node()
            self.left.fromParent(self)
            self.right=node()
            self.right.fromParent(self)
            level1=self.left.full(maxsize)
            level2=self.right.full(maxsize)
            return max(level1,level2)
        
        #define valor de constantes
        if self.value == "number":
            self.value=random.uniform(-1,1)
        
        return self.level

class symbolicTree:
    def __init__(self):
        self.maxsize = 7
        self.root = node()
        self.root.level=1
        self.height = 0
        self.fitness = np.inf
    
    def copy(self):
        newTree = symbolicTree()
        newTree.maxsize = self.maxsize
        newTree.height = self.height
        newTree.root = self.root.copy()
        return newTree
        
    def __str__(self):
        s="Altura máxima: "+str(self.maxsize)+"\n"
        s+="Altura: "+str(self.height)+"\n"
        s+= self.root.__str__()
        return s
    
    def __repr__(self):
        return self.__str__()
    
    #na criação o tamanho máximo dado pode ser diferente do tamanho maximo da arvore
    def grow(self,msize):
        self.height=self.root.grow(msize)
    def full(self,msize):
        self.height = self.root.full(msize)
        
    def evaluate(self,data):
        y=np.zeros(len(data))
        estimation=np.zeros(len(data))
        for i,attributes in enumerate(data):
            y[i]=attributes["Y"]
            estimation[i]=solve(self.root,lookup,attributes)
        Y_ = np.mean(y) 
        NMRSE = np.sqrt( np.sum((y-estimation)**2) / np.sum((y-Y_)**2) )
        self.fitness = NMRSE
        return NMRSE
    
    def chooseNode(self):
        nodes = []
        nodes.append(self.root)
        i=0
        while(i < len(nodes)):
            nxt = nodes[i]
            if nxt.left:
                nodes.append(nxt.left)
            if nxt.right:
                nodes.append(nxt.right)
            i+=1
        return nodes[math.floor(random.uniform(0,len(nodes)))]
    
    
    def mutation(self):
        descendant = self.copy()
        node = descendant.chooseNode()
        node.grow(descendant.maxsize)
        descendant.recalculateHeight()
        return descendant
    
    def elitistMutation(self, data):
        descendant = self.mutation()
        descendant.evaluate(data)
        if descendant.fitness<self.fitness:
            return descendant
        return self
        
    def crossover(self,othertree):
        descendant1=self.copy()
        descendant2=othertree.copy()
        mynode = descendant1.chooseNode()
        othernode = descendant2.chooseNode()
        
        if descendant1.fits(mynode,othernode) and descendant2.fits(othernode,mynode):
            descendant1.swap(descendant2,mynode,othernode)
            descendant1.recalculateHeight()
            descendant2.recalculateHeight()
            return descendant1,descendant2
        
        #crossover falhou
        else:
            return None,None
    
    def elitistCrossover(self, othertree, data):
        descendant1,descendant2 = self.crossover(othertree)
        #crossover falhou
        if not descendant1:
            return None,None
        
        descendant1.evaluate(data)
        descendant2.evaluate(data)
        
        ret = []
        #checa se algum filho é melhor que o pai
        if descendant1.fitness < self.fitness and descendant1.fitness < othertree.fitness:
            ret.append(descendant1)
        if descendant1.fitness < self.fitness and descendant1.fitness < othertree.fitness:
            ret.append(descendant2)

        #completa o retorno com os pais(em ordem de fitness) caso nenhum filho seja melhor
        if len(ret) == 0:
            if self.fitness<othertree.fitness:
                ret.append(self)
                ret.append(othertree)
            else:
                ret.append(othertree)
                ret.append(self)

        elif len(ret) == 1:
            if self.fitness<othertree.fitness:
                ret.append(self)
            else:
                ret.append(othertree)

        #retorna os melhores individuos
        return ret
    
    def fits(self,mynode,othernode):
        nodes = []
        nodes.append(othernode)
        i=0
        maxlevel=0
        while(i<len(nodes)):
            nxt=nodes[i]
            if nxt.level>maxlevel:
                maxlevel=nxt.level
            if nxt.left:
                nodes.append(nxt.left)
            if nxt.right:
                nodes.append(nxt.right)
            i+=1
        treesize = maxlevel - othernode.level
        if mynode.level + treesize <= self.maxsize:
            return True
        else:
            return False
    
    
    def swap(self,othertree,mynode,othernode):
        myparent = mynode.parent
        otherparent = othernode.parent
        if not myparent:
            self.root = othernode
            self.root.parent= None
            self.root.level = 1
        if not otherparent:
            othertree.root = mynode
            othertree.root.parent=None
            othertree.root.level =1 
            
        if myparent:
            if mynode is myparent.left:
                myparent.left=othernode
                othernode.parent=myparent
            else:
                myparent.right=othernode
                othernode.parent=myparent
                
        if otherparent:
            if othernode is otherparent.left:
                otherparent.left=mynode
                mynode.parent = otherparent
            else:
                otherparent.right=mynode
                mynode.parent = otherparent
    
    
    def recalculateHeight(self):
        self.root.level=1
        self.height = 1
        queue = deque()
        queue.append(self.root)
        while(queue):
            nxt = queue.popleft()
            if nxt.left:
                nxt.left.level=nxt.level+1
                queue.append(nxt.left)
            if nxt.right:
                nxt.right.level=nxt.level+1
                queue.append(nxt.right)
            if nxt.level>self.height:
                self.height=nxt.level
    

class SymbolicRegression:
    def __init__(self, numberOfIndividuals, generations, crossoverProb, mutationProb, tournamentSize,df,elitist=False):
        self.size = numberOfIndividuals
        self.generations = generations
        self.mutationProb = mutationProb
        self.crossoverProb = crossoverProb
        self.tournamentSize = tournamentSize
        self.data = df
        self.population = [symbolicTree() for x in range(numberOfIndividuals)]
        self.elitist = elitist
        self.maxSolutionSize = 7
        
    def tournament(self):
        choices = random.sample(list(range(0,self.size)),self.tournamentSize)
        best = choices[0]
        error = self.population[best].fitness
        for i in choices:
            newerror = self.population[i].fitness
            if newerror<=error:
                error = newerror
                best = i
        return self.population[best]
    
    def geneticOperator(self,ind):
        prob = random.uniform(0,1)
        if prob <= self.crossoverProb:
            #seleciona outro pai
            otherind = self.tournament()
            while otherind is ind:
                otherind = self.tournament()
            
            descendant1=None
            descendant2=None
            #faz crossover até dar certo
            if not self.elitist:
                while not descendant1:
                    descendant1,descendant2=ind.crossover(otherind)
                descendant1.evaluate(self.data)
                descendant2.evaluate(self.data)
            else:
                while not descendant1:
                    descendant1,descendant2=ind.elitistCrossover(otherind,self.data)
            
            ret=[descendant1,descendant2]
            return ret

        
        elif prob <= self.crossoverProb + self.mutationProb:
            if not self.elitist:
                descendant = ind.mutation()
                descendant.evaluate(self.data)
            else:
                descendant = ind.elitistMutation(self.data)
            return [descendant]
        
        #retorna o proprio individuo se nenhum operador for escolhido
        return [ind]
    
    def newPopulation(self):
        newPop = []
        #enquanto não forem criados N novos individuos
        while(len(newPop) < self.size):
            #seleciona
            ind = self.tournament()
            #e aplica operador genético
            newind = self.geneticOperator(ind)
            for i in newind:
                if len(newPop)<self.size:
                    newPop.append(i)
        self.population = newPop  
    
    def HalfHalf(self):
        interval = 1
        limit = int(self.size/self.maxSolutionSize)
        boundary = limit/2
        division = limit
        for i in range(self.size):
            if i > division:
                interval+=1
                division = limit * interval
                if interval > self.maxSolutionSize:
                    interval = self.maxSolutionSize
            if i < division-boundary:
                self.population[i].full(interval)
            else:
                self.population[i].grow(interval)
            self.population[i].evaluate(self.data)
        
    
    def geneticAlgorithm(self):
        stats = []
        #população ramped half-half
        self.HalfHalf()
        #população inicial
       # for i in self.population:
         #   i.randomize()
         #   i.evaluate(self.data)
            
        currentGen=0
        while(currentGen < self.generations):
            #a fase especifica de avaliação de fitness pode ser ignorada,
            #pois precisa-se de avaliar a fitness na criação do individuo devido ao operador elitista
            
            #fase de seleção + operadores geneticos + nova população
            self.newPopulation()
            
            currentGen+=1
            stats.append(self.population)
        
        #select best
        minfitness=np.inf
        argmin=0
        for i in self.population:
            if i.fitness < minfitness:
                minfitness = i.fitness
                argmin = i
        return argmin,stats