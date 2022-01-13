import numpy as np
import copy
import random
import sys

class Tree:
    def __init__(self, parent, dispo, played):
        self.parent = parent
        self.children = []
        self.dispo = copy.deepcopy(dispo)
        self.wins = 0
        self.visited = 1
        self.played = played
        self.checked = [0 for i in range(9)]

def selection(padre):
    maxi = 0
    the_good_one = None
    for child in padre.children:
        n = child.visited
        w = child.wins 
        value = w/n+np.sqrt(2)*np.sqrt(np.log(padre.visited)/n)
        if value >= maxi:
            maxi = value
            the_good_one = child
    return the_good_one

def reachable(cur):
    if len(cur.dispo) == 0:
        return [i for i in range(81)]
    reachable = []
    j = cur.played-9*(cur.played//9)     
    if cur.checked[j] == 0:
        for i in range(9*j,9*(j+1)): 
            if i not in cur.dispo:
                reachable.append(i)
    else:
        for l in range(9):
            if cur.checked[l] == 0:
                for i in range(9*l,9*(l+1)):
                    if i not in cur.dispo:
                        reachable.append(i)                   
    return reachable        

def reachable2(cur):
    if len(cur.dispo) == 0:
        return 0
    j = cur.played-9*(cur.played//9)     
    if cur.checked[j] == 0:
        for i in range(9*j,9*(j+1)): 
            if i not in cur.dispo:
                return(i)
    else:
        for l in range(9):
            if cur.checked[l] == 0:
                for i in range(9*l,9*(l+1)):
                    if i not in cur.dispo:
                        return(i)                          

def winners(i):
    add = 9*i
    return([ [0+add,1+add,2+add], [3+add,4+add,5+add], [6+add,7+add,8+add], [0+add,3+add,6+add], [1+add,4+add,7+add], [2+add,5+add,8+add], [0+add,4+add,8+add], [2+add,4+add,6+add] ])

def sub_verify(cur, i):
    coups_j1 = [elt for idx, elt in enumerate(cur.dispo) if idx % 2 == 0]
    coups_j2 = [elt for idx, elt in enumerate(cur.dispo) if idx % 2 != 0]
    for winner in winners(i):
        if set(winner).issubset(set(coups_j1)):
            return 1
        if set(winner).issubset(set(coups_j2)): 
            return -1
    if set([a for a in range(9*i,9*(i+1))]).issubset(set(cur.dispo)):
        return 2
    else: 
        return 0    

def update_checked(cur):
    for i in range(9):
        if cur.checked[i] == 0:
            cur.checked[i] = sub_verify(cur, i)

def verify(cur):
    a = cur.checked
    if ( ( (a[0]==1) and (a[1]==1) and (a[2]==1) ) or ( (a[3]==1) and (a[4]==1) and (a[5]==1) ) or ( (a[6]==1) and (a[7]==1) and (a[8]==1) ) or ( (a[0]==1) and (a[3]==1) and (a[6]==1) ) or ( (a[1]==1) and (a[4]==1) and (a[7]==1) ) or ( (a[2]==1) and (a[5]==1) and (a[8]==1) ) or ( (a[0]==1) and (a[4]==1) and (a[8]==1) ) or ( (a[2]==1) and (a[4]==1) and (a[6]==1) ) ):
        return((None, 1))
    if ( ( (a[0]==-1) and (a[1]==-1) and (a[2]==-1) ) or ( (a[3]==-1) and (a[4]==-1) and (a[5]==-1) ) or ( (a[6]==-1) and (a[7]==-1) and (a[8]==-1) ) or ( (a[0]==-1) and (a[3]==-1) and (a[6]==-1) ) or ( (a[1]==-1) and (a[4]==-1) and (a[7]==-1) ) or ( (a[2]==-1) and (a[5]==-1) and (a[8]==-1) ) or ( (a[0]==-1) and (a[4]==-1) and (a[8]==-1) ) or ( (a[2]==-1) and (a[4]==-1) and (a[6]==-1) ) ):
        return((None, -1))
    for i in range(9):
        if cur.checked[i] == 0:
            return((True, 0)) 
    return((None, 0))        


def MCTS(root, T):
    for t in range(T):
        #selection
        first_play = root.played
        current = root
        while (len(current.children)!=0) :
            current.visited += 1
            current = selection(current)
        #expansion
        if ((verify(current)[0] is not None)):
            if current.parent == None:
                chosen = Tree(current, current.dispo + [40], 40)
                current.children.append(chosen)
                update_checked(chosen)
                chosen.visited += 1
            else:
                reach = reachable(current)
                j_etoile = random.choice(reach)
                chosen = Tree(current, current.dispo + [j_etoile], j_etoile)
                current.children.append(chosen)
                update_checked(chosen)
                chosen.visited += 1
                if len(reach)>2:
                    reach= reach[:2]
                for j in reach:
                    if (j!=j_etoile):
                        not_chosen = Tree(current, current.dispo + [j], j)
                        current.children.append(not_chosen)
                        update_checked(not_chosen)
                        not_chosen.visited += 0
        else:
            chosen = current            
        #simulation
        current2 = chosen
        while (verify(current2)[0] is not None):
            j_star = reachable2(current2)
            current2 = Tree(current2, current2.dispo+[j_star], j_star)
            update_checked(current2)
            
        #back_propagation
        current3 = chosen
        resultat = verify(current2)[1]
        while (current3.played != first_play):
            if (resultat == +1):
                current3.wins += 1 
            current3 = current3.parent 


def und_to_deuxd(entier):
    entier_bis = entier - 9*(entier//9)
    entier_tus = entier - 9*(entier_bis)
    to_add = [(0,0), (0,3), (0,6), (3,0), (3,3), (3,6), (6,0), (6,3), (6,6)]
    print(str(entier_bis//3 + to_add[entier//9][0]) + ' ' + str(entier_bis%3 + to_add[entier//9][1]))

def deux_to_un(i, j):
    return(3*i+ j)

def deuxd_to_und(i, j):
    a = deux_to_un(i//3, j//3)   
    return(3*(i%3)+ j%3 + 9*a)

def selection_mechant(current, opponent_row, opponent_col):
    if (opponent_row,  opponent_col) == (-1, -1):
        return current
    else:
        played = deuxd_to_und(opponent_row, opponent_col)
        for child in current.children:
            if child.played == played:
                return child      
        child = Tree(current, current.dispo+ [played], played)
        current.children.append(child)
        update_checked(child)   
        return child


def selection_gentil(current):
    if len(current.children) == 0:
        print("Error machine")    
    else: 
        maxi = 0   
        the_good_one = current.children[0]
        for child in current.children:
            n = child.visited
            w = child.wins
            if n!= 0: 
                if w/n >= maxi:
                    maxi = w/n
                    the_good_one = child
        return the_good_one

def selection_gentil_min(current):
    if len(current.children) == 0:
        print("Error machine")    
    else: 
        mini = 1000   
        the_good_one = current.children[0]
        for child in current.children:
            n = child.visited
            w = child.wins
            if n!= 0: 
                if w/n < mini:
                    mini = w/n
                    the_good_one = child
        return the_good_one

        
current = Tree(None, [], None)

MCTS(current, 50)

while True:
    opponent_row, opponent_col = [int(i) for i in input().split()]
    validActionCount = int(input())
    for i in range(validActionCount):
        row, col = [int(j) for j in input().split()]    
    current = selection_mechant(current, opponent_row, opponent_col)    
    if (len(current.dispo)%2 == 0):
        MCTS(current, 10)
        current = selection_gentil(current)
    else:
        MCTS(current, 10)
        current = selection_gentil_min(current)
    und_to_deuxd(current.played)
