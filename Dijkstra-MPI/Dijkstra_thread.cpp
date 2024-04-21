#include<iostream>
#include<stdlib.h>
#include<pthread.h>
#include<climits>
#include<unistd.h>
#define N 4

using namespace std;

struct stFin {
    int st;
    int fin;
};
struct g {
    int val, flag = 0;
};

int no_nodes, st_pt;
int graph[4][4] = { {0,1,5,4},{1,0,2,6},{5,2,0,3},{4,6,3,0} };
int resGraph[N][N];

void updateRes(int k, int cur, g com[][N])
{
    for (int i = 0; i < no_nodes; i++)
    {
        resGraph[k][i] = com[cur][i].val;
    }
}

void init(g com[][N])
{
    for (int i = 0; i < no_nodes; i++)
    {
        for (int j = 0; j < no_nodes; j++)
        {
            com[i][j].val = INT_MAX - 100;
            com[i][j].flag = 0;
        }
        com[0][i].val = INT_MAX - 100;
    }
}

int minimum(int a, int b)
{
    return (a > b ? b : a);
}

int minimum_index(int k, g com[][N])
{
    int minval, min_index = -1;
    minval = 10000;
    for (int i = 0; i < no_nodes; i++)
    {
        if (com[k][i].val < minval && com[k][i].flag == 0)
        {
            minval = com[k][i].val;
            min_index = i;
        }
    }
    return min_index;
}

void inc_flag(int k, g com[][N])
{
    for (int i = 0; i < N; i++)
    {
        com[i][k].flag = 1;
    }
}

bool checkFlag(g com[][N])
{
    for (int i = 0; i < no_nodes; i++)
    {
        if (com[0][i].flag == 0)
            return true;
    }
    return false;
}

void* shortestPath(void* varg)
{
    int* st_pt1 = (int*)varg;
    int stptq = *st_pt1;
    int cur = 0;
    g com[N][N];
    init(com);

    com[0][stptq].val = 0;

    do {
        cur++;
        for (int j = 0; j < no_nodes; j++)
        {
            com[cur][j].val = minimum(com[cur - 1][j].val, com[cur - 1][stptq].val + graph[stptq][j]);
            if (com[cur][j].val != com[cur - 1][j].val)
            {
                for (int i = 0; i < N; i++)
                    com[i][j].flag = 0;
            }
        }
        inc_flag(stptq, com);
        stptq = minimum_index(cur, com);

    } while (checkFlag(com));

    // Cre?te flagul pentru nodul curent la sfâr?itul calculului.
    inc_flag(stptq, com);

    // Actualizeaz? matricea rezultatului pentru nodul curent.
    updateRes(stptq, cur, com);

    // Returneaz? matricea rezultatului pentru nodul curent.
    pthread_exit(NULL);
}

void dijikstra(int no)
{
    no_nodes = no;
    pthread_t tid[no_nodes];

    cout << "\nEnter info into graph : " << endl;
    // inputinfo(); Nu este nevoie de aceast? func?ie în implementarea pe fire de execu?ie.

    cout << "\nGraph : " << endl;
    for (int i = 0; i < no_nodes; i++) {
        pthread_create(&tid[i], NULL, shortestPath, &i);
    }

    for (int i = 0; i < no_nodes; i++) {
        pthread_join(tid[i], NULL);
    }
}

void print(int g[N][N])
{
    char st = 'A';
    for (int i = 0; i < no_nodes; i++)
    {
        char lt = 'A';
        if (i == 0) {
            cout << "\t";
            for (int k = 0; k < no_nodes; k++)
            {
                cout << lt++ << "\t";
            }
            cout << endl;
        }
        for (int j = 0; j < no_nodes; j++)
        {
            if (j == 0)
            {
                cout << st++ << "\t";
            }
            if (g[i][j] != INT_MAX - 100)
                cout << g[i][j] << "\t";
            else
                cout << "%\t";
        }
        cout << endl;
    }
}

int main()
{
    int no;
    cout << "\nEnter no of vertices : ";
    cin >> no;
    dijikstra(no);
    cout << "\nResult:" << endl;
    print(resGraph);
}
