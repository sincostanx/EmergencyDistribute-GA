#include "stdio.h"
#include "vector"
#include "algorithm"
#define matrix vector<vector<int> >
#define super_matrix vector<vector<long long> >
#define point pair<int,int>
#define state pair<int,point>
#define f first
#define s second
#define INF 1e8
using namespace std;
class vertex{
    public:int z,r,X,T;
    public:vector<int> R;
};
class object{
    public:int b,d;
};
int donation_center,node[110],object_number,n,m;
long long global_op=1e18,penalty;
vector<object> o;
matrix graph[100],all_shortest_path[101];
super_matrix value(101);
point ans[100],actual_ans[100];
vertex sink[100][100];
vector<state> donation;
matrix graph_input(int x){
    //input data for nodes in sub graph
    scanf("%d",&node[x]);
    for(int i=1;i<=node[x];i++){
        scanf("%d %d %d",&sink[x][i].z,&sink[x][i].r,&m);
        for(int j=1;j<=m+1;j++) scanf("%d",&n),sink[x][i].R.push_back(n);
    }
    //input number of nodes and edges in subgraph
    matrix t(node[x]+1,vector<int>(node[x]+1));
    for(int i=1;i<=node[x];i++){
        for(int j=1;j<=node[x];j++){
            scanf("%d",&t[i][j]);
            if(t[i][j]==0&&i!=j) t[i][j]=INF;
        }
    }
    return t;
}
void object_input(){
    //input number of objects and their description
    scanf("%d",&object_number);
    vector<object> t(object_number+1);
    for(int i=1;i<=object_number;i++) scanf("%d %d",&t[i].b,&t[i].d);
    o=t;
}
matrix floyd_warshall(matrix d){
    //floyd-warshall algorithm for calculate all pair shortest path
    for(int k=1;k<d.size();k++){
        for(int i=1;i<d.size();i++){
            for(int j=1;j<d.size();j++) d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
        }
    }
    return d;
}
long long D(int i,int j,int t){
    return value[i][j]=sink[i][j].z+sink[i][j].r*t;
}
long long cube(long long x){
    return x*x*x;
}
void evaluate(){
    //calculate penalty for every decisions
    for(int i=1;i<=donation_center;i++){
        for(int j=1;j<=node[i];j++) value[i][j]=D(i,j,40);
    }
    for(int i=0;i<donation.size();i++){
        //printf("%d: %d\n",i,all_shortest_path[0][donation[i].s.f][ans[i].f]+all_shortest_path[ans[i].f][1][ans[i].s]);
        value[ans[i].f][ans[i].s]-=o[donation[i].s.s].b;
    }
    penalty=0;
    for(int i=1;i<=donation_center;i++){
        for(int j=1;j<=node[i];j++) penalty+=max(-1LL,cube(value[i][j]));
    }
    /*if(penalty<global_op){
        for(int i=0;i<=donation.size();i++) actual_ans[i]=ans[i];
    }*/
    global_op=min(global_op,penalty);
}
void global_optimum(int n){
    //recursive for generate all feasible decisions
    if(n>=donation.size()) evaluate();
    else{
        for(int i=1;i<=donation_center;i++){
            for(int j=1;j<=node[i];j++){
                //check for due date by using data calculated by floyd-warshall algorithm
                if(all_shortest_path[0][donation[n].s.f][i]+all_shortest_path[i][1][j]+1<=o[donation[n].s.s].d)
                    ans[n]=point(i,j),global_optimum(n+1);
            }
        }
    }
}
int main(){
    int temp,x,countx=0;
    scanf("%d",&donation_center);
    //input graph
    for(int i=1;i<=donation_center;i++) graph[i]=graph_input(i);
    //input connection between source nodes
    graph[0]=matrix(donation_center+1,vector<int>(donation_center+1));
    for(int i=1;i<=donation_center;i++){
        for(int j=1;j<=donation_center;j++){
            scanf("%d",&graph[0][i][j]);
            if(graph[0][i][j]==0&&i!=j) graph[0][i][j]=INF;
        }
    }
    object_input();
    for(int i=0;i<=donation_center;i++) all_shortest_path[i]=floyd_warshall(graph[i]);
    //input donations
    while(scanf("%d",&temp)){
        if(temp==-1) break;
        countx++;
        for(int i=1;i<=donation_center;i++){
            for(int j=1;j<=object_number;j++){
                scanf("%d",&x);
                for(int l=0;l<x;l++) donation.push_back(state(countx,point(i,j)));
            }
        }
    }
    for(int i=1;i<=donation_center;i++) value[i]=vector<long long>(node[i]+1);
    //calculate global optimum
    global_optimum(0);
    for(int i=0;i<donation.size();i++) printf("%d %d: %d\n",actual_ans[i].f,actual_ans[i].s,o[donation[i].s.s].b);
    printf("P(40) = %I64d",global_op);
}
