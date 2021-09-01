#include "stdio.h"
#include "vector"
#include "set"
#include "algorithm"
#define point pair<int,int>
#define matrix vector<vector<int> >
#define super_matrix vector<vector<long long> >
#define vector_matrix vector<vector<vector<int> > >
#define double_matrix pair<matrix,matrix>
#define super_decision pair<int,decision>
#define f first
#define s second
#define INF 1e9
using namespace std;
class vertex{
    public: int z,r,X,T;
    public: vector<int> R;
};
class object{
    public: int b,d;
    public: object(int b=0,int d=0):
        b(b),d(d){};
};
class super_object{
    public: int time_pass,sum_b;
    public: vector<point> item;
    public: super_object(int sum_b=0,vector<point> item=vector<point>(),int time_pass=0):
        sum_b(sum_b),item(item),time_pass(time_pass){};
};
class vehicle{
    public: int now;
    public: super_object ob;
    public: vector<point> schedule;
    public: vector<int> prefix_sum;
    public: vehicle(super_object ob=super_object(),vector<point> schedule=vector<point>(),vector<int> prefix_sum=vector<int>(),int now=0):
        ob(ob),schedule(schedule),prefix_sum(prefix_sum),now(now){};
};
class decision{
    public: int type;
    public: super_object ob;
    public: point p,q;
    public: long long between;
    public: decision(super_object ob=super_object(0,vector<point>()),int type=0,point p=point(0,0),point q=point(0,0),long long between=0):
        ob(ob),type(type),p(p),q(q),between(between){};
    public: bool operator <(decision X) const{
        return ob.sum_b>X.ob.sum_b;
    }
};
int donation_center,node[110],object_number,n,m;
int actual_sink[100][100];
long long take_out,dist,max_now;
super_matrix value(110,vector<long long>(110));
vector<int> ans[100][100];
matrix graph[100];
vector_matrix path[100];
double_matrix all_shortest_path[101];
vertex sink[100][100];
vector<object> o;
vector<vehicle> travel;
multiset<super_decision> incoming[101][101];
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
void input(){
    //main function for receive graph and object
    scanf("%d",&donation_center);
    for(int i=1;i<=donation_center;i++) graph[i]=graph_input(i);
    graph[0]=matrix(donation_center+1,vector<int>(donation_center+1));
    for(int i=1;i<=donation_center;i++){
        for(int j=1;j<=donation_center;j++){
            scanf("%d",&graph[0][i][j]);
            if(graph[0][i][j]==0&&i!=j) graph[0][i][j]=INF;
        }
    }
    object_input();
}
double_matrix floyd_warshall(matrix d){
    //floyd-warshall algorithm and keep track of paths
    matrix next(d.size(),vector<int>(d.size(),-1));
    for(int i=1;i<d.size();i++){
        for(int j=1;j<d.size();j++) if(i!=j) next[i][j]=j;
    }
    for(int k=1;k<d.size();k++){
        for(int i=1;i<d.size();i++){
            for(int j=1;j<d.size();j++){
                if(d[i][k]+d[k][j]<d[i][j]) d[i][j]=d[i][k]+d[k][j],next[i][j]=next[i][k];
            }
        }
    }
    return double_matrix(d,next);
}
vector<int> findpath(int k,int i,int j){
    //find shortest path for each pair of nodes
    if(all_shortest_path[k].s[i][j]==-1) return vector<int>();
    vector<int> path;
    path.push_back(i);
    while(i!=j) i=all_shortest_path[k].s[i][j],path.push_back(i);
    return path;
}
void preprocess(){
    //main function for find exact all pair shortest path
    node[0]=donation_center;
    for(int i=0;i<=donation_center;i++) all_shortest_path[i]=floyd_warshall(graph[i]);
    for(int k=0;k<=donation_center;k++){
        path[k]=vector_matrix(node[k]+1,matrix(node[k]+1));
        for(int i=1;i<=node[k];i++){
            for(int j=1;j<=node[k];j++){
                if(i==j) path[k][i][j].push_back(i);
                else path[k][i][j]=findpath(k,i,j);
            }
        }
    }
}
long long square(long long x){
    return x*x;
}
long long new_distance(point x,point y){
    //support calculate distance between node j in sub grah i to destination
    if(x.f==y.f) return all_shortest_path[x.f].f[x.s][y.s];
    else return all_shortest_path[0].f[x.f][y.f];
}
long long get_distance(int i,int j,decision choice){
    //calculate distance between node j in sub grah i to destination
    if(!choice.type){
        if(choice.p.f==i) return all_shortest_path[i].f[choice.p.s][j];
        return all_shortest_path[choice.p.f].f[choice.p.s][1]+all_shortest_path[0].f[choice.p.f][i]+all_shortest_path[i].f[1][j];
    }
    else{
        long long x,y;
        if(choice.p.f==i) x=all_shortest_path[i].f[choice.p.s][j];
        else x=all_shortest_path[choice.p.f].f[choice.p.s][1]+all_shortest_path[0].f[choice.p.f][i]+all_shortest_path[i].f[1][j];
        if(choice.q.f==i) y=all_shortest_path[i].f[choice.q.s][j];
        else y=all_shortest_path[choice.q.f].f[choice.q.s][1]+all_shortest_path[0].f[choice.q.f][i]+all_shortest_path[i].f[1][j];
        return min(x+new_distance(choice.p,choice.q)-choice.between,y+choice.between);
    }
}
long long D(int i,int j,int t){
    //demand calculation function
    return sink[i][j].z+sink[i][j].r*t-sink[i][j].X-actual_sink[i][j];
}
long long A(int i,int j,int t,int y){
    return max(0LL,D(i,j,t)-take_out);
}
double g(int i,int j,int t,int d,int k){
    return (square(A(i,j,t+d,k))-square(max(0LL,A(i,j,t+d,k)-k)))/(double)(d+1);
}
double f(int i,int j,int t){
    double value=0;
    take_out=0;
    for(multiset<super_decision>::iterator it=incoming[i][j].begin();it!=incoming[i][j].end();it++)
        value+=g(i,j,t,get_distance(i,j,(*it).s),(*it).s.ob.sum_b),take_out+=(*it).s.ob.sum_b;
    return value;
}
long long evaluate(){
    //calculate objective function
    for(int i=1;i<=donation_center;i++){
        for(int j=1;j<=node[i];j++) value[i][j]+=sink[i][j].z+sink[i][j].r*10000;
    }
    long long penalty=0;
    for(int i=1;i<=donation_center;i++){
        for(int j=1;j<=node[i];j++) penalty+=square(max(0LL,value[i][j]));
    }
    return penalty;
}
void update(int x){
    //update transportation state
    travel[x].ob.time_pass++;
    while(travel[x].ob.time_pass>travel[x].ob.item.back().f&&!(travel[x].ob.item.empty())) travel[x].ob.sum_b-=travel[x].ob.item.back().s,travel[x].ob.item.pop_back();
    while(travel[x].ob.time_pass>travel[x].prefix_sum[travel[x].now]) travel[x].now++;
}
vector<vehicle> update_transportation(){
    //main function for update transportation state
    vector<vehicle> z;
    for(int i=0;i<travel.size();i++){
        update(i);
        if(travel[i].ob.time_pass<travel[i].prefix_sum.back()) z.push_back(travel[i]);
        else{
            value[travel[i].schedule.back().f][travel[i].schedule.back().s]-=travel[i].ob.sum_b;
            ans[travel[i].schedule.back().f][travel[i].schedule.back().s].push_back(travel[i].ob.sum_b);
            actual_sink[travel[i].schedule.back().f][travel[i].schedule.back().s]+=travel[i].ob.sum_b;
        }
    }
    return z;
}
vector<decision> create_choice(int temp){
    //create list of donations
    int x;
    vector<decision> donation;
    for(int i=0;i<travel.size();i++){
        if(travel[i].ob.time_pass==travel[i].prefix_sum[travel[i].now])
            donation.push_back(decision(travel[i].ob,0,travel[i].schedule[travel[i].now]));
        else
            donation.push_back(decision(travel[i].ob,1,travel[i].schedule[travel[i].now-1],travel[i].schedule[travel[i].now],travel[i].prefix_sum[travel[i].now]-travel[i].ob.time_pass));
    }
    if(temp!=-1){
        for(int i=1;i<=donation_center;i++){
            for(int j=1;j<=object_number;j++){
                scanf("%d",&x);
                for(int l=0;l<x;l++){
                    super_object dummy;
                    dummy.item.push_back(point(o[j].d,o[j].b)),dummy.sum_b=o[j].b;
                    donation.push_back(decision(dummy,0,point(i,1)));
                }
            }
        }
    }
    return donation;
}
point choose_maximum(decision x,int countx){
    //assign destination for a donation
    max_now=-1e18;
    point choose;
    decision choice;
    for(int i=1;i<=donation_center;i++){
        for(int j=1;j<=node[i];j++){
            choice=x;
            dist=get_distance(i,j,choice);
            while(dist+choice.ob.time_pass>choice.ob.item.back().f) choice.ob.sum_b-=choice.ob.item.back().s,choice.ob.item.pop_back();
            if(choice.ob.item.size()==0) continue;
            incoming[i][j].insert(super_decision(dist+choice.ob.time_pass,choice));
            long long dummyx=f(i,j,countx);
            if(dummyx>max_now) max_now=dummyx,choose=point(i,j);
            incoming[i][j].erase(incoming[i][j].find(super_decision(dist+choice.ob.time_pass,choice)));
        }
    }
    return choose;
}
vector<point> pathing(int i,int j,point p,point q){
    //find (shortest) path to destination
    vector<point> dummy_1;
    if(q!=point(-1,-1)) dummy_1.push_back(q);
    if(p.f!=i){
        for(int l=0;l<path[p.f][p.s][1].size();l++) dummy_1.push_back(point(p.f,path[p.f][p.s][1][l]));
            for(int l=0;l<path[0][p.f][i].size()-1;l++){
                if(l==0) continue;
                dummy_1.push_back(point(path[0][p.f][i][l],1));
            }
        for(int l=0;l<path[i][1][j].size();l++) dummy_1.push_back(point(i,path[i][1][j][l]));
    }
    else for(int l=0;l<path[i][p.s][j].size();l++) dummy_1.push_back(point(i,path[i][p.s][j][l]));
    return dummy_1;
}
vector<vehicle> recalculate_transportation(int countx){
    //update transportation state (again)
    vector<vehicle> travel;
    for(int i=1;i<=donation_center;i++){
        for(int j=1;j<=node[i];j++){
            for(multiset<super_decision>::iterator it=incoming[i][j].begin();it!=incoming[i][j].end();it++){
                decision choice=(*it).s;
                vector<point> dummy_1;
                vector<int> dummy_2;
                long long x,y;
                if(!choice.type){
                    dummy_1=pathing(i,j,choice.p,point(-1,-1));
                    dummy_2.push_back(0+choice.ob.time_pass);
                }
                else{
                    if(choice.p.f==i) x=all_shortest_path[i].f[choice.p.s][j];
                    else x=all_shortest_path[choice.p.f].f[choice.p.s][1]+all_shortest_path[0].f[choice.p.f][i]+all_shortest_path[i].f[1][j];
                    if(choice.q.f==i) y=all_shortest_path[i].f[choice.q.s][j];
                    else y=all_shortest_path[choice.q.f].f[choice.q.s][1]+all_shortest_path[0].f[choice.q.f][i]+all_shortest_path[i].f[1][j];
                    if(x+new_distance(choice.p,choice.q)-choice.between<y+choice.between){
                        dummy_1=pathing(i,j,choice.p,choice.q);
                        dummy_2.push_back(-choice.between+choice.ob.time_pass);
                    }
                    else{
                        dummy_1=pathing(i,j,choice.q,choice.p);
                        dummy_2.push_back(-(new_distance(choice.p,choice.q)-choice.between)+choice.ob.time_pass);
                    }
                }
                for(int l=0;l<dummy_1.size()-1;l++){
                    if(dummy_1[l].f==dummy_1[l+1].f) dummy_2.push_back(dummy_2.back()+graph[dummy_1[l].f][dummy_1[l].s][dummy_1[l+1].s]);
                    else dummy_2.push_back(dummy_2.back()+graph[0][dummy_1[l].f][dummy_1[l+1].f]);
                }
                travel.push_back(vehicle(choice.ob,dummy_1,dummy_2,0));
            }
        }
    }
    return travel;
}
int main(){
    int countx=0,temp;
    input(),preprocess();
    while(true){
        travel=update_transportation();
        if(temp!=-1) scanf("%d",&temp);
        if(temp==-1&&travel.empty()) break;
        //create sorted list of donations
        vector<decision> donation=create_choice(temp);
        sort(donation.begin(),donation.end());
        //clear value
        for(int i=1;i<=donation_center;i++){
            for(int j=1;j<=node[i];j++) incoming[i][j].clear(),sink[i][j].X=0;
        }
        //assign destination
        for(int k=0;k<donation.size();k++){
            point choose=choose_maximum(donation[k],countx);
            dist=get_distance(choose.f,choose.s,donation[k]);
            incoming[choose.f][choose.s].insert(super_decision(dist+donation[k].ob.time_pass,donation[k]));
            sink[choose.f][choose.s].X+=donation[k].ob.sum_b;
        }
        travel=recalculate_transportation(countx++);
    }
    printf("%I64d",evaluate());
}
