#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <cmath>
#include <random>

using namespace std;

struct Node
{
    public:
        Node(Node *padre,vector<int> disp , int move, vector<int> check);
        ~Node();
        double wins;
        double visited;
        int played;
        vector<int> dispo;
        vector<int> checked;
        vector< Node *> children;
        Node *parent;
};

Node::Node(Node *padre, vector<int> disp, int move, vector<int> check):visited(1), wins(0)
{
    parent = padre;
    dispo = disp;
    played = move;
    checked = check;
}

Node *selection(Node &padre)
{
    double maxi = 0;
    Node *the_good_one = NULL;
    for (size_t i = 0; i < padre.children.size(); ++i)
    {
        double n = padre.children[i]->visited;
        double w = padre.children[i]->wins;
        double value = double( w/n + sqrt(2)*sqrt(log(padre.visited)/n) );
        if (value >= maxi)
        {
            maxi = value;
            the_good_one = padre.children[i];
        }
    }
    return the_good_one;
}

bool alreadyExists(vector<int> array, int value)
{
       for (int i = 0; i < array.size(); i++)
           if (array[i] == value)
               return true;
       return false;
}

vector<int> reachable(Node *cur)
{
    if(cur->dispo.size()==0)
    {
        vector<int> reach;
        for (int i=0; i<81; i++) reach.push_back(i);
        return reach;
    }
    vector<int> reach;
    int j = cur->played - 9*(int)(cur->played/9);
    if (cur->checked[j]==0)
    {
        for (int i=9*j; i<9*(j+1); i++)
        {
            if (!alreadyExists(cur->dispo, i))
            {
                reach.push_back(i);
            }
        }
    }
    if (cur->checked[j]!=0)
    {
        for (int l=0; l<9; l++)
        {
            if (cur->checked[l]==0)
            {
                for (int i=9*l; i<9*(l+1); i++)
                {
                    if (!alreadyExists(cur->dispo, i))
                    {
                        reach.push_back(i);
                    }
                }
            }
        }
    }
    return reach;
}

vector<vector <int>> winners(int i)
{
    int add = 9*i;
    vector<vector<int>> winners;
    winners.push_back({0+add, 1+add, 2+add});
    winners.push_back({3+add, 4+add, 5+add});
    winners.push_back({6+add, 7+add, 8+add});
    winners.push_back({0+add, 3+add, 6+add});
    winners.push_back({1+add, 4+add, 7+add});
    winners.push_back({2+add, 5+add, 8+add});
    winners.push_back({0+add, 4+add, 8+add});
    winners.push_back({2+add, 4+add, 6+add});
    return winners;
}

bool including(vector<int> grande_liste, vector<int> petite_liste)
{
    int count = 0;
    for (int i: petite_liste)
    {
        for (int j: grande_liste)
        {
            if (i == j)
            {
                count += 1;
            }
        }
    }
    if (count == petite_liste.size()) return true;
    else return false;
}

int sub_verify(Node *cur, int i)
{
    vector<int> coups_j1;
    vector<int> coups_j2;
    for (int l=0; l<cur->dispo.size(); l++)
    {
        if (l%2==0)
        {
            coups_j1.push_back(cur->dispo[l]);
        }
        else
        {
            coups_j2.push_back(cur->dispo[l]);
        }
    }
    for(vector<int> winner: winners(i))
    {
        if (including(coups_j1, winner))
        {
            return 1;
        }
        if (including(coups_j2, winner))
        {
            return -1;
        }
    }
    vector<int> full;
    for (int j=9*i; j<9*(i+1); j++) full.push_back(j);
    if (including(coups_j1, full))
    {
        return 2;
    }
    return 0;
}

void update_checked(Node *cur)
{
    for(int i=0; i<9; i++)
    {
        if (cur->checked[i]==0)
        {
            cur->checked[i] = sub_verify(cur, i);
        }
    }
}

vector<int> verify(Node *cur)
{
    vector<int> a = cur->checked;
    if ( ( (a[0]==1) && (a[1]==1) && (a[2]==1) ) || ( (a[3]==1) && (a[4]==1) && (a[5]==1) ) || ( (a[6]==1) && (a[7]==1) && (a[8]==1) ) || ( (a[0]==1) && (a[3]==1) && (a[6]==1) ) || ( (a[1]==1) && (a[4]==1) && (a[7]==1) ) || ( (a[2]==1) && (a[5]==1) && (a[8]==1) ) || ( (a[0]==1) && (a[4]==1) && (a[8]==1) ) || ( (a[2]==1) && (a[4]==1) && (a[6]==1) ) )
    {
        return {0, 1};
    }
    if ( ( (a[0]==-1) && (a[1]==-1) && (a[2]==-1) ) || ( (a[3]==-1) && (a[4]==-1) && (a[5]==-1) ) || ( (a[6]==-1) && (a[7]==-1) && (a[8]==-1) ) || ( (a[0]==-1) && (a[3]==-1) && (a[6]==-1) ) || ( (a[1]==-1) && (a[4]==-1) && (a[7]==-1) ) || ( (a[2]==-1) && (a[5]==-1) && (a[8]==-1) ) || ( (a[0]==-1) && (a[4]==-1) && (a[8]==-1) ) || ( (a[2]==-1) && (a[4]==-1) && (a[6]==-1) ) )
    {
        return {0, -1};
    }
    for (int i=0; i<9; i++)
    {
        if (a[i] == 0)
        {
            return {1, 0};
        }
    }
    return {0, 0};
}

void MCTS(Node &root, int T)
{
    for (int t=0; t<T; t++)
    {
        //selection
        Node *current = &root;
        int first_play = root.played;
        while(current->children.size()!=0)
        {
            current->visited += 1;
            current = selection(*current);
        }

        //expansion
        Node *next;
        if (verify(current)[0] == 1)
        {
            if (current->played == -1)
            {
                next = new Node(current, {40}, 40, current->checked);
                next->visited +=1;
                current->children.push_back(next);
            }

            else
            {
                vector<int> reach = reachable(current);
                int indice = rand()%reach.size();
                int j_etoile = reach[indice];
                next = new Node(current, current->dispo, j_etoile, current->checked);
                next->visited += 1;
                current->children.push_back(next);
                next->dispo.push_back(j_etoile);
                update_checked(next);
                int c = 1;
                for (int j: reach)
                {
                    if (j!=j_etoile)
                    {
                        auto *news = new Node(current, current->dispo, j, current->checked);
                        current->children.push_back(news);
                        news->dispo.push_back(j);
                        update_checked(news);
                        c += 1;
                    }
                }
            }
        }
        else
        {
            next = current;
        }
        //simulation
        Node *current2 = next;
        int indice;
        while (verify(current2)[0] == 1)
        {
            vector<int> reach = reachable(current2);
            if (reach.size()==0) break;
            indice = rand()%reach.size();
            int j_star = reach[indice];
            current2 = new Node(current2, current2->dispo, j_star, current2->checked);
            current2->dispo.push_back(j_star);
            update_checked(current2);
        }
        //back_propagation
        int result = verify(current2)[1];
        Node *current3 = next;
        while (current3->played != first_play)
        {
            if (result == +1)
            {
                current3->wins += 1;
            }
            current3 = current3->parent;
        }
        
        if (result == +1)
        {
            current3->wins += 1;
        }
    }

}

void und_to_deuxd(int entier)
{
    int entier_bis = entier - 9*(int)(entier/9);
    int entier_tus = entier - 9*(entier_bis);
    vector<vector <int>> to_add = { {0,0}, {0,3}, {0,6}, {3,0}, {3,3}, {3,6}, {6,0}, {6,3}, {6,6}};
    cout << (int)entier_bis/3 + to_add[(int)(entier/9)][0]  << " " << entier_bis%3 + to_add[(int)(entier/9)][1] << endl;
}

int deux_to_un(int i, int j)
{
    return(3*i+ j);
}

int  deuxd_to_und(int i, int j)
{
    int a = deux_to_un(int(i/3), int(j/3));
    return(3*(i%3)+ j%3 + 9*a);
}

Node *selection_mechant(Node &current, int opponent_row,  int opponent_col)
{
    if ((opponent_row==-1) && (opponent_col==-1))
    {
        return &current;
    }
    else
    {
        int played = deuxd_to_und(opponent_row, opponent_col);
        for (Node *child : current.children)
        {
            if (child->played == played)
            {
                return child;
            }
        }
        Node *child = new Node(&current, current.dispo, played, current.checked);
        child->dispo.push_back(played);
        update_checked(child);
        current.children.push_back(child);
        return child;
    }
}

Node *selection_gentil(Node &current)
{
    double maxi = 0;
    Node *the_good_one = NULL;
    for (Node *child : current.children)
    {
        double n = child->visited;
        double w = child->wins;
        if (w/n >= maxi)
        {
            maxi = w/n;
            the_good_one = child;
        }
    }
        return the_good_one;
}

Node *selection_gentil_min(Node &current)
{
    double mini = 1000.0;
    Node *the_good_one = current.children[0];
    for (Node *child : current.children)
    {
        double n = child->visited;
        double w = child->wins;
        if (w/n < mini)
        {
            mini = w/n;
            the_good_one = child;
        }
    }
        return the_good_one;
}

int main()
{
    // game loop
    vector<int> check (9,0);
    vector<int> disp = {};
    Node current(NULL, disp, -1, check);    
    MCTS(current, 30);
    while (1)
    {
        int opponentRow;
        int opponentCol;
        cin >> opponentRow >> opponentCol; cin.ignore();
        int validActionCount;
        cin >> validActionCount; cin.ignore();
        for (int i = 0; i < validActionCount; i++)
        {
            int row;
            int col;
            cin >> row >> col; cin.ignore();
        }
        current = *selection_mechant(current, opponentRow, opponentCol);
        if ((current.dispo.size())%2 ==0)
        {
            MCTS(current, 6);
            current = *selection_gentil(current);
        }
        else
        {
            MCTS(current, 6);
            current = *selection_gentil_min(current);
        }
        und_to_deuxd(current.played);
    }
}
