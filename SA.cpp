#include <iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<math.h>
#include<stdlib.h>
#include<ctime>
#include<stdio.h>
#include<time.h>

using namespace std;

int numero(string num){
    stringstream ss(num);
    int retorno=0;
    ss >> retorno;
    return retorno;
}



int **arquivo(string nome){
    int n,m;
    ifstream arq(nome);
    string aux;
    arq>>aux;
    n=numero(aux);
    arq>>aux;
    m=numero(aux);
    int **tarefa=new int*[n];
    for(int i=0;i<n;i++){
        tarefa[i]=new int[m];
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            arq>>aux;
            arq>>aux;
            tarefa[i][j]=numero(aux);
        }
    }
    arq.close();
    return tarefa;
}

int alee(int low,int high){
    int k;
    double d;
    d = (double) rand () / ((double) RAND_MAX + 1);
    k = d * (high - low + 1);
    return low + k;
}


int custos(int **tarefa,int *solucao,int n,int m){
   int *tempo=new int[m];
   for(int i=0;i<m;i++){
       tempo[i]=0;
   }
   for(int i=0;i<n;i++){
       tempo[0]+=tarefa[solucao[i]][0];
       for(int j=1;j<m;j++){
           if(tempo[j-1]>tempo[j]){
               tempo[j]=tempo[j-1]+tarefa[solucao[i]][j];
           }
           else{
               tempo[j]+=tarefa[solucao[i]][j];
           }
       }
   }
   return tempo[m-1];
}

int custosnowait(int **tarefa,int *solucao,int n,int m){
    int *tempoanterior=new int[m];
    int *tempoatual=new int[m];
    for(int i=0;i<m;i++){
        tempoanterior[i]=0;
        tempoatual[i]=0;
    }
    int max=0;
    int desvio=0;
    for(int i=0;i<n;i++){
        tempoatual[0]+=tarefa[solucao[i]][0];
        for(int j=1;j<m;j++){
            desvio=tempoanterior[j]-tempoatual[j-1];
            if(desvio>max){
                max=desvio;
            }
            tempoatual[j]=tempoatual[j-1]+tarefa[solucao[i]][j];
        }
        for(int j=0;j<m;j++){
            tempoatual[j]+=max;
            tempoanterior[j]=tempoatual[j];
        }

        desvio=0;
        max=0;
    }
    return tempoatual[m-1];
}

int *perturbacao(int *solucao,int n){
    int al,primeiro,aux;
    int *auxiliar=new int[n];
    for(int i=0;i<n;i++){
        auxiliar[i]=solucao[i];
    }
    al=2;
    if(al==2){
        primeiro=alee(0,n-2);

        aux=auxiliar[primeiro];
        auxiliar[primeiro]=auxiliar[primeiro+1];
        auxiliar[primeiro+1]=aux;
        return auxiliar;

    }
    if(al==3){
        primeiro=alee(0,n-3);

        aux=auxiliar[primeiro];
        auxiliar[primeiro]=auxiliar[primeiro+2];
        auxiliar[primeiro+2]=aux;
        return auxiliar;
    }
    if(al==4){
        primeiro=alee(0,n-4);
        aux=auxiliar[primeiro];
        auxiliar[primeiro]=auxiliar[primeiro+3];
        auxiliar[primeiro+3]=aux;
        aux=auxiliar[primeiro+1];
        auxiliar[primeiro+1]=auxiliar[primeiro+2];
        auxiliar[primeiro+2]=aux;
        return auxiliar;
    }
    if(al==5){
        primeiro=alee(0,n-5);
        aux=auxiliar[primeiro];
        auxiliar[primeiro]=auxiliar[primeiro+4];
        auxiliar[primeiro+4]=aux;
        aux=auxiliar[primeiro+1];
        auxiliar[primeiro+1]=auxiliar[primeiro+3];
        auxiliar[primeiro+3]=aux;
        return auxiliar;

    }

}
double randomico(){
    double a=0;
    int aa=alee(0,10000);
    a=a+aa;
    a=a/10000;

    //cout<<" a = "<<a<<endl;
    return a;
}





int main(){
    //noome do arquivo
    string nome;
    cout<<"digite o nome do arquivo : ";
    cin>>nome;
    int tipo;//1 se for fsp e 2 se for nfsp
     cout<<"digite o tipo (1 se for fsp ou 2 se for nfsp)   :   ";
     cin>>tipo;
    double inicio,fim;
    if(tipo==1){
    int **tarefa=arquivo(nome);
    int n,m;
    ifstream arq(nome);
    string aux;
    arq>>aux;
    n=numero(aux);
    arq>>aux;
    m=numero(aux);
    arq.close();
    inicio=clock();
    int *solucao=new int[n];
    int *auxiliar;
    for(int i=0;i<n;i++){
        solucao[i]=i;
    }
    for(int i=0;i<n;i++){
        cout<<solucao[i]+1<<" ";
    }
    cout<<endl<<"custo inicial = "<<custos(tarefa,solucao,n,m);

    cout<<endl<<endl;
    double tfinal=0.001;
    double temp=0.1;
    double alfa=0.98;
    double delta;
    int pode,l=100,P=1000;
    int x,y;
    int *melhor=new int[n];
    for(int i=0;i<n;i++){
        melhor[i]=i;
    }
    do{
        x=1;
        pode=0;
        do{
            auxiliar=perturbacao(solucao,n);
            delta=custos(tarefa,auxiliar,n,m)-custos(tarefa,solucao,n,m);
            if(delta<=0||exp(-1*delta/temp)>randomico()){
                for(int i=0;i<n;i++){
                    solucao[i]=auxiliar[i];
                }
                if(custos(tarefa,solucao,n,m)<custos(tarefa,melhor,n,m)){
                    for(int i=0;i<n;i++){
                        melhor[i]=solucao[i];
                    }
                }
                pode++;
            }
            x++;
        }while(pode<=l||x<P);
        y++;
        temp*=alfa;
    }while(temp>tfinal);
    fim=clock();
    for(int i=0;i<n;i++){
        cout<<melhor[i]+1<<" ";
    }
    ofstream arqu("solucao.txt");
    for(int i=0;i<n;i++){
        stringstream numero;
        numero<<melhor[i];
        arqu<<numero.str()<<" ";
    }
    arqu<<endl;
    stringstream ti;
    ti<<tipo;
    arqu<<ti.str();
    arqu<<endl<<nome;
    arqu.close();






    cout<<endl<<"custo final = "<<custos(tarefa,melhor,n,m);
    cout<<endl;
    cout<<"tempo = "<<(fim-inicio)/1000000<<" segundos"<<endl;
    }

    if(tipo==2){
    int **tarefa=arquivo(nome);


    int n,m;
    ifstream arq(nome);
    string aux;
    arq>>aux;
    n=numero(aux);
    arq>>aux;
    m=numero(aux);
    arq.close();
    inicio=clock();
    int *solucao=new int[n];
    int *auxiliar;
    for(int i=0;i<n;i++){
        solucao[i]=i;
    }
    for(int i=0;i<n;i++){
        cout<<solucao[i]+1<<" ";
    }
    cout<<endl<<"custo inicial = "<<custosnowait(tarefa,solucao,n,m);

    cout<<endl<<endl;
    double tfinal=0.01;
    double temp=0.1;
    double alfa=0.3;
    double delta;
    int pode,l=(n*3)/10,P=(n*6)/10;
    int x,y;
    int *melhor=new int[n];
    for(int i=0;i<n;i++){
        melhor[i]=i;
    }
    do{
        x=1;
        pode=0;
        do{
            auxiliar=perturbacao(solucao,n);
            delta=custosnowait(tarefa,auxiliar,n,m)-custosnowait(tarefa,solucao,n,m);
            if(delta<=0||exp(-1*delta/temp)>randomico()){
                for(int i=0;i<n;i++){
                    solucao[i]=auxiliar[i];
                }
                if(custosnowait(tarefa,solucao,n,m)<custosnowait(tarefa,melhor,n,m)){
                    for(int i=0;i<n;i++){
                        melhor[i]=solucao[i];
                    }
                }
                pode++;
            }
            x++;
        }while(pode<=l||x<P);
        y++;
        temp*=alfa;
    }while(temp>tfinal);
    fim=clock();
    for(int i=0;i<n;i++){
        cout<<melhor[i]+1<<" ";
    }
    ofstream arqui("solucao.txt");
    for(int i=0;i<n;i++){
        stringstream numero;
        numero<<melhor[i];
        arqui<<numero.str()<<" ";
    }
    arqui<<endl;
    stringstream ti;
    ti<<tipo;
    arqui<<ti.str();
    arqui<<endl<<nome;
    arqui.close();

    cout<<endl<<"custo final = "<<custosnowait(tarefa,melhor,n,m);
    cout<<endl<<"tempo = "<<(fim-inicio)/1000000<<" segundos"<<endl;
    }








    return 0;
}

