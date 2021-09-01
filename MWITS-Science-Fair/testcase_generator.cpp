#include "stdio.h"
#include "string.h"
#include "vector"
#include "random"
#include "chrono"
#include "algorithm"
#define vertex pair<int,int>
#define f first
#define s second
#define TIME_LIMIT 10
#define OBJECT_LIMIT 10
#define POSSIBLE_GRAPH 42
#define WEIGHT_LIMIT 5
#define DAY_LIMIT 20
#define BENEFIT_LIMIT 100
#define NUMBER_OBJECT_LIMIT 8
#define RATE_LIMIT 100
using namespace std;
int graph_type,object_type,donation_center;
int countx,now;
FILE *fp,*it;
void subgraph_initialize(int n,int m){
    //declare variables
    vector<int> u;
    vector<vertex> v;
    int g[n+1][n+1]={},choose,k;
    unsigned seed=chrono::system_clock::now().time_since_epoch().count();
    minstd_rand0 generator(seed);
    //generate randomly spanning tree
    for(int i=1;i<=n;i++) u.push_back(i);
    random_shuffle(u.begin(),u.end());
    for(int i=1;i<u.size();i++) choose=generator()%i,g[u[i]][u[choose]]=g[u[choose]][u[i]]=generator()%WEIGHT_LIMIT+1,m--;
    //generate n nodes connected graph with m edges
    for(int i=1;i<=n;i++) for(int j=1;j<i;j++) if(!g[i][j]) v.push_back(vertex(i,j));
    random_shuffle(v.begin(),v.end());
    int i=0;
    while(m--) g[v[i].f][v[i].s]=g[v[i].s][v[i].f]=generator()%WEIGHT_LIMIT+1,i++;
    //generate input
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++) fprintf(fp,"%d ",g[i][j]);
        fprintf(fp,"\n");
    }
}
void graph_initialize(){
    //declare variables
    unsigned seed=chrono::system_clock::now().time_since_epoch().count();
    minstd_rand0 generator(seed);
    int graph_type=generator()%POSSIBLE_GRAPH+1,k;
    char t[]="input",x[100];
    vector<int> v;
    //open integer partition file and gather data
    sprintf(x,"%d",graph_type),strcat(t,x);
    printf("open: %s\n",t);
    it=fopen(t,"r");
    while(fscanf(it,"%d",&k)==1) v.push_back(k);
    donation_center=v.size();
    //generate input
    fprintf(fp,"%d\n",donation_center);
    //generate connected subgraph
    for(int i=0;i<v.size();i++){
        fprintf(fp,"%d\n",v[i]);
        //generate sink nodes data
        for(int j=1;j<=v[i];j++){
            fprintf(fp,"%d %d ",generator()%RATE_LIMIT,generator()%RATE_LIMIT+1);
            k=generator()%3,fprintf(fp,"%d ",k);
            for(int l=0;l<=k;l++) fprintf(fp,"%d ",generator()%WEIGHT_LIMIT);
            fprintf(fp,"\n");
        }
        //generate connected graph
        if(v[i]==1) fprintf(fp,"0\n");
        else if(v[i]==2) fprintf(fp,"0 1\n1 0\n");
        else subgraph_initialize(v[i],generator()%((v[i]*v[i]-3*v[i]+2)/2)+(v[i]-1));
    }
    if(donation_center==1) fprintf(fp,"0\n");
    else if(donation_center==2) fprintf(fp,"0 1\n1 0\n");
    else subgraph_initialize(donation_center,generator()%((donation_center*donation_center-3*donation_center+2)/2)+(donation_center-1));
    fclose(it);
}
void object_initialize(){
    unsigned seed=chrono::system_clock::now().time_since_epoch().count();
    minstd_rand0 generator(seed);
    object_type=generator()%OBJECT_LIMIT+1;
    fprintf(fp,"%d\n",object_type);//fprintf(fp,"%d\n",object_type);
    for(int i=1;i<=object_type;i++) fprintf(fp,"%d %d\n",generator()%BENEFIT_LIMIT+1,generator()%DAY_LIMIT+1);//fprintf(fp,"%d %d\n",generator()%100+1,generator()%20+1);
}
void random_input(){
    fprintf(fp,"%d\n",1);
    unsigned seed=chrono::system_clock::now().time_since_epoch().count();
    int k;
    minstd_rand0 generator(seed);
    for(int i=1;i<=donation_center;i++){
        for(int j=1;j<=object_type;j++){
            if(now==0) fprintf(fp,"%d ",0);
            else k=generator()&1,fprintf(fp,"%d ",k),now-=k;
        }
        fprintf(fp,"\n");//fprintf(fp,"\n");
    }
}
int main(){
    for(int i=1;i<=200000;i++){
        char t[]="testcase",x[100];
        sprintf(x,"%d",i),strcat(t,x);
        fp=fopen(t,"w");
        graph_initialize();
        object_initialize();
        unsigned seed=chrono::system_clock::now().time_since_epoch().count();
        minstd_rand0 generator(seed);
        now=NUMBER_OBJECT_LIMIT;
        do random_input(),countx++;
        while((countx<=TIME_LIMIT||generator()%(TIME_LIMIT>>1)!=0)||now>0);
        fprintf(fp,"-1"),fclose(fp);
        printf("TEST CASE %d COMPLETED\n",i);
    }
}
