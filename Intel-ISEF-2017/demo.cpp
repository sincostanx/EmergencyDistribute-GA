#include "bits/stdc++.h"
#include "conio.h"
#define state pair<int,int>
#define super_state pair<int,state>
#define chromosome vector<int>
#define chromosome_state pair<double,chromosome>
#define chromosome_pair pair<chromosome,chromosome>
#define NP 500
#define DP 5
#define PC 0.50
#define PM 0.20
#define G 1000
#define GX 100
#define K (NP*0.2)
#define REP 5
#define RM 0.5
#define f first
#define s second
using namespace std;
random_device rd;
mt19937 generator(rd());
int node,r[200],z[200],l[200],N[200],now,sum_object;
int Graph[5000][5000],dist[5000][5000],nodex;
long long sum_util;
double R[200][200];
int lock_down[15000],time_lock_down[15000];
int g,gx,T,test;
FILE *fp;
double max_fitness,prev_mft;
vector<int> b[200],d[200],pos[200];
deque<chromosome_state> population,temp_population;
chromosome solution,prev_solution;
class util{
    public: static double square(double x){
        return x*x;
    };
    public: static int random(int a,int b){
        uniform_int_distribution<> distributor(a,b);
        return distributor(generator);
    }
    public: static vector<int> appendx(vector<int> u,vector<int> v){
        for(int i=0;i<v.size();i++) u.push_back(v[i]);
        return u;
    }
    public: static double demand(int i,int t){
        return r[i]*t+z[i];
    }
};
class graph_operator{
    public: static void extension(){
        int idx=node;
        for(int i=1;i<node;i++){
            int temp=0;
            for(int j=1;j<l[i];j++) Graph[temp][idx]=Graph[idx][temp]=true,temp=idx,idx++;
            Graph[temp][i]=Graph[i][temp]=true;//,printf("connect %d %d\n",temp,i);
        }
        nodex=idx;
    };
    public: static void bfs(int x){
        queue<state> q;
        vector<bool> check(nodex,false);
        q.push(state(0,x));
        while(!q.empty()){
            state p=q.front();
            q.pop();
        //printf("now: %d %d\n",p.f,p.s);
            if(check[p.s]) continue;
            dist[x][p.s]=p.f,check[p.s]=true;
            for(int i=0;i<nodex;i++) if(Graph[p.s][i]) q.push(state(p.f+1,i));
        }
    }
    public: static void asap(){
        for(int i=0;i<nodex;i++) bfs(i);
    }
};
class input{
    public: static void graph_input(){
        scanf("%d",&node);
        for(int i=1;i<node;i++) scanf("%d %d",&r[i],&z[i]);
        for(int i=1;i<node;i++) scanf("%d",&l[i]);
        graph_operator::extension();
        graph_operator::asap();
    };
    public: static void object_input(){
        int y,z;
        scanf("%d",&N[now]);
        for(int i=0;i<N[now];i++){
            scanf("%d %d",&y,&z);
            sum_util+=y;
            b[now].push_back(y),d[now].push_back(z);
            pos[now].push_back(0);
        }
    }
};
class genetic_operator{
    public: static chromosome random_chromosome(int dimension){
        chromosome v;
        for(int i=0;i<dimension;i++) v.push_back(util::random(1,node-1));
        v.push_back(-1);
        return v;
    }
    public: static chromosome_state resize_population(){
        chromosome_state ans;
        double maxft=-1e9;
        random_shuffle(population.begin(),population.end());
        for(int i=0;i<K;i++) if(temp_population[i].f>maxft) maxft=temp_population[i].f,ans=temp_population[i];
        return ans;
    }
    public: static chromosome sub_parent_selection(){
        chromosome ans;
        double maxft=-1e9;
        random_shuffle(population.begin(),population.end());
        for(int i=0;i<K;i++) if(population[i].f>maxft) maxft=population[i].f,ans=population[i].s;
        return ans;
    }
    public: static chromosome_pair parent_selection(){
        return chromosome_pair(sub_parent_selection(),sub_parent_selection());
    }
    public: static chromosome_pair crossing_over(chromosome_pair parent){
        if(util::random(0,100)/100.0>PC) return parent;
        chromosome_pair child;
        for(int i=0;i<parent.f.size();i++){
            if(util::random(0,100)/100.0>RM) child.f.push_back(parent.f[i]);
            else child.f.push_back(parent.s[i]);
            if(util::random(0,100)/100.0>RM) child.s.push_back(parent.f[i]);
            else child.s.push_back(parent.s[i]);
        }
        return child;
    }
    public: static chromosome_pair mutation(chromosome_pair child){
        if(util::random(0,100)/100.0>PM) return child;
        int idx1=util::random(0,child.f.size()-1),idx2=util::random(0,child.s.size()-1);
        if(child.f[idx1]!=-1) child.f[idx1]=util::random(1,node-1);
        if(child.s[idx2]!=-1) child.s[idx2]=util::random(1,node-1);
        return child;
    }
};
class genetic_algorithm{
    public: static void reset(){
        max_fitness=prev_mft=-1e9,g=gx=0,solution.clear();
    };
    public: static bool termination(){
        return (g>G)||(fabs(max_fitness)<1e-10)||(gx>GX);
    };
    public: static void survivor_selection(){
        sort(population.begin(),population.end());
        for(int i=0;i<REP;i++) population.pop_front(),population.pop_front();
    }
    public: static void population_initialization(int dimension){
        chromosome temp;
        for(int i=1;i<=NP;i++){
            temp=genetic_operator::random_chromosome(dimension),population.push_back(chromosome_state(fitness(temp),temp));
       //     printf("population %d ok\n",i);
        }
    }
    public: static void population_re_initialization(int dimension){
        chromosome cx;
        temp_population.clear();
        for(int i=0;i<population.size();i++){
            for(int j=0;j<=DP;j++) temp_population.push_back(population[i]);
        }
        for(int i=0;i<temp_population.size();i++){
            cx=util::appendx(temp_population[i].s,genetic_operator::random_chromosome(dimension));
            temp_population[i]=chromosome_state(fitness(cx),cx);
        }
        population.clear();
        for(int i=0;i<NP;i++){
            population.push_back(genetic_operator::resize_population());
      //  printf("re initial population %d ok\n",i+1);
        }
    }
    public: static double obj_fn(chromosome v){
        vector<super_state> s[node];
        memset(R,0,sizeof(R));
        int i=1,j=0;
        for(int k=0;k<v.size();k++){
          //  printf("obj: %d %d %d\n",i,j,k);
            int temp=-1;
            if(v[k]==-1) i++,j=0;
            else if(lock_down[k]!=-1){
                s[lock_down[k]].push_back(super_state(time_lock_down[k],state(i,j)));
                j++;
            }
            else if(now+dist[pos[i][j]][v[k]]<=d[i][j]&&now+dist[pos[i][j]][v[k]]<=T){
                s[v[k]].push_back(super_state(now+dist[pos[i][j]][v[k]],state(i,j)));
                j++;
            }
        }
        double temp=0;
        for(int i=1;i<node;i++) sort(s[i].begin(),s[i].end());
        for(int i=1;i<node;i++){
            if(s[i].size()==0) continue;
            //printf("interval: [%d,%d)\n",1,s[i][0].f);
            for(int j=1;j<s[i][0].f;j++) R[i][j]=0;
            for(int j=1;j<s[i].size();j++){
              //  printf("interval: [%d,%d)\n",s[i][j-1].f,s[i][j].f);
                temp+=b[s[i][j-1].s.f][s[i][j-1].s.s];
                for(int k=s[i][j-1].f;k<s[i][j].f;k++) R[i][k]=min(1.,temp/util::demand(i,k));
            }
            temp+=b[s[i].back().s.f][s[i].back().s.s];
            //printf("temp now: %f\n",temp);
            //printf("interval: [%d,%d]\n",s[i].back().f,T);
            for(int k=s[i].back().f;k<=T;k++) R[i][k]=min(1.,temp/util::demand(i,k));
        }
        double ans=0;
        /*for(int t=1;t<=T;t++){
            for(int j=1;j<node;j++) printf("%.f ",R[j][t]);
            printf("\n");
        }*/
        for(int t=1;t<=T;t++){
            double mean=0,sum=0;
            for(int j=1;j<node;j++) mean+=(R[j][t])/(node-1);
            for(int j=1;j<node;j++) sum+=util::square(R[j][t]-mean);
            ans+=sum;
        }
        //printf("fitness value %.10f\n",-ans);
        return ans;
    }
    public: static double fitness(chromosome x){
        double temp=-obj_fn(x);
        if(temp>max_fitness) max_fitness=temp,solution=x;
        return temp;
    }
};
class strategy{
    public: static void decrypt(){
        vector<state> travel[1000],fin[1000];
        int i=1,j=0;
        printf("-----DISTRIBUTION STRATEGY-----\n");
        for(int k=0;k<solution.size();k++){
            if(solution[k]==-1) i++,j=0;
            else{
                if(k>=prev_solution.size()) printf("Assign donation (%d,%d) to city %d\n",i,j,solution[k]);
                else if(prev_solution[k]!=solution[k]) printf("Re-Assign donation (%d,%d) to city %d\n",i,j,solution[k]);
                if(lock_down[k]==-1){
                    travel[solution[k]].push_back(state(i,j));
                    int next_state,minx=1e9;
                    for(int u=0;u<nodex;u++){
                        if(dist[u][solution[k]]<minx&&Graph[u][pos[i][j]]) minx=dist[u][solution[k]],next_state=u;
                    }
                    //printf("move from city %d to city %d\n",pos[i][j],next_state);
                    pos[i][j]=next_state;
                    if(pos[i][j]<node&&pos[i][j]){
                        lock_down[k]=pos[i][j],time_lock_down[k]=now,sum_util-=b[i][j];
                        //printf("object %d %d reach city %d\n",i,j,pos[i][j]);
                    }
                }
                else fin[lock_down[k]].push_back(state(i,j));
                j++;
            }
        }
        printf("-----STATUS REPORT-----\n");
        for(int i=1;i<node;i++){
            printf("Node (%d,%d) (%d,%d) = %.f: [",i,l[i],r[i],z[i],util::demand(i,now));
            for(int j=0;j<fin[i].size();j++) printf("(%d,%d) ",fin[i][j].f,fin[i][j].s);
            printf(": ");
            for(int j=0;j<travel[i].size();j++) printf("(%d,%d) ",travel[i][j].f,travel[i][j].s);
            printf("]\n");
        }
        prev_solution=solution;
        double sum_demand=0;
        for(int i=1;i<node;i++) sum_demand+=util::demand(i,now);
        printf("RATE: %.10f\n",sum_util/sum_demand);
        printf("-----END REPORT DAY %d-----\n",now);
    }
};
void GA(){
    genetic_algorithm::reset();
    printf("reset ok\n");
    if(now==1) genetic_algorithm::population_initialization(N[now]);
    else genetic_algorithm::population_re_initialization(N[now]);
    printf("initial ok\nlaunch main system!\n");
    while(!genetic_algorithm::termination()){
        for(int i=0;i<REP;i++){
            chromosome_pair off_spring=genetic_operator::mutation(genetic_operator::crossing_over(genetic_operator::parent_selection()));
            population.push_back(chromosome_state(genetic_algorithm::fitness(off_spring.f),off_spring.f));
            population.push_back(chromosome_state(genetic_algorithm::fitness(off_spring.s),off_spring.s));
        }
        genetic_algorithm::survivor_selection(),g++;
        gx=((max_fitness==prev_mft)? (gx+1):0);
        prev_mft=max_fitness;
    }
    printf("end genetic algorithm for day %d\n",now);
}
int main(){
    printf("INTERVAL:\n");
    while(scanf("%d",&T)&&T){
        sum_util=0;
        memset(lock_down,-1,sizeof(lock_down));
        memset(Graph,0,sizeof(Graph));
        population.clear();
        system("cls");
        printf("DISASTER AREA DATA:\n");
        input::graph_input(),now=1,sum_object=0;
        for(;now<=T;now++){
            b[now].clear(),d[now].clear(),pos[now].clear();
            printf("LIST OF DONATIONS AT DAY %d:\n",now);
            input::object_input();
            sum_object+=N[now];
            system("cls");
            if(N[now]) GA();
            strategy::decrypt();
            kbhit();
        }
    }
}
