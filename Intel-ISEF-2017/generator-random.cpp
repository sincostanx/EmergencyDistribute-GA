#include "bits/stdc++.h"
#define DAY_LIMIT 1000
#define EDGE_LIMIT 4
#define NODE_LIMIT 100
#define NUM_OBJ_LIMIT 100
#define RATE_LIMIT 1000
#define TIME_LIMIT 30
#define UTIL_LIMIT 1000
using namespace std;
random_device rd;
mt19937 generator(rd());
int random(int a,int b){
    uniform_int_distribution<> distributor(a,b);
    return distributor(generator);
}
void graph_initialize(){
    int n=random(2,NODE_LIMIT);
    printf("%d\n",n);
    for(int i=2;i<=n;i++) printf("%d %d\n",random(1,RATE_LIMIT),random(0,RATE_LIMIT));
    for(int i=2;i<=n;i++) printf("%d ",random(1,EDGE_LIMIT));
    printf("\n");
}
void random_input(int x){
    int n=random(1,NUM_OBJ_LIMIT);
    printf("%d\n",n);
    for(int i=1;i<=n;i++) printf("%d %d\n",random(1,UTIL_LIMIT),random(x,DAY_LIMIT));
}
int main(){
    //freopen("small_test_data.txt","w",stdout);
    int test;
    scanf("%d",&test);
    while(test--){
        int T=random(1,TIME_LIMIT);
        printf("%d\n",T);
        graph_initialize();
        for(int k=1;k<=T;k++) random_input(k);
    }
    printf("0");
}
