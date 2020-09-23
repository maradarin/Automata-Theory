#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#define breakline() cout<<"-------------------------------------------------------------------------\n"
using namespace std;

class DFA
{
    vector<string> Q, F, Sigma;
    map<string, vector<string> > delta;

public:
    DFA() {};
    DFA(vector<string> Q, vector<string> Sigma, map<string, vector<string> > delta, int q0, vector<string> F)
    {
        this->Q = Q;
        this->Sigma = Sigma;
        this->delta = delta;
        this->F = F;
    }

    vector<string> getSigma() const
    {
        return this->Sigma;
    }

    friend istream& operator >> (istream&, DFA&);
    friend ostream& operator << (ostream&, DFA&);

    void update_set(vector<vector<string> > &, map<string, int> &, int, int &);
    void update_table(vector<string> &, map<string, int> &);
    void min_DFA(int);
};



istream& operator >> (istream& f, DFA& M)
{
    ifstream in("date1.in");
    // ifstream in("date1.in");
    int noOfStates, noOfFinalStates,nr=0,noOfSymbols;
    in>>noOfStates>>noOfFinalStates>>noOfSymbols;
    nr=noOfStates-noOfFinalStates;
    for(int i=1; i<=nr; i++)
    {
        string ch;
        in>>ch;
        M.Q.push_back(ch);                              // separam starile finale de la bun inceput
    }
    for(int i=1; i<=noOfFinalStates; i++)
    {
        string ch;
        in>>ch;
        M.F.push_back(ch);
    }
    M.Sigma.push_back("@");
    for(int i=1; i<=noOfSymbols; i++)
    {
        string ch;
        in>>ch;
        M.Sigma.push_back(ch);
    }
    for(vector<string>::iterator it=M.Q.begin(); it!=M.Q.end(); it++)
    {
        string s1,s2;
        in>>s1>>s2;
        M.delta[*it].push_back(s1);
        M.delta[*it].push_back(s2);
    }
    for(vector<string>::iterator it=M.F.begin(); it!=M.F.end(); it++)
    {
        string s1,s2;
        in>>s1>>s2;
        M.delta[*it].push_back(s1);
        M.delta[*it].push_back(s2);
    }

    return f;
}


ostream& operator << (ostream& g, DFA& M)
{
    breakline();
    cout<<"Non Terminal States : \n";
    for(int i=0; i<M.Q.size(); i++) cout<<M.Q[i]<<" ";
    cout<<endl;
    cout<<"Final States : \n";
    for(int i=0; i<M.F.size(); i++) cout<<M.F[i]<<" ";
    cout<<endl<<endl<<endl;
    for(vector<string>::iterator lit=M.Sigma.begin(); lit!=M.Sigma.end(); lit++) cout<<*lit<<"\t";
    cout<<endl;
    for(map<string, vector<string> >::iterator it=M.delta.begin(); it!=M.delta.end(); it++)
    {
        cout<<it->first<<"\t";
        for(int j=0; j<it->second.size(); j++) cout<<it->second[j]<<"\t";
        cout<<endl;
    }
    breakline();
    return g;
}



void DFA::update_set(vector<vector<string> > &set, map<string, int> &m, int max_set, int &nset)
{
    set.resize(max_set);                                                    // actualizam dimensiunea vectorului de partitii
    for(int i=0; i<nset; i++)
    {
        for(int j=0; j<set[i].size(); j++)
        {
            if(m[set[i][j]]!=i)
            {
                set[m[set[i][j]]].push_back(set[i][j]);                     // creez o noua partitie pt starea separabila
                set[i].erase(set[i].begin()+j, set[i].begin()+j+1);         // elimin starea separabila din vechea partitie
                j--;
            }
        }
    }
    nset=max_set;                                                           // nr curent de partitii devine nr maxim de partitii
}



void DFA::update_table(vector<string> &cset, map<string, int> &m)
{
    map<string, vector<string> > newdelta;
    for(map<string, vector<string> >::iterator it=delta.begin(); it!=delta.end(); it++)
    {
        newdelta[cset[m[it->first] ] ];                                              // starile concatenate din fiecare partitie sunt noile stari
        for(int j=0; j<it->second.size(); j++)
        {
            if(newdelta[cset[m[it->first]]].size()<it->second.size())
                newdelta[cset[m[it->first]]].push_back(cset[m[it->second[j]]]);
        }
    }
    delta.clear();
    delta=newdelta;

    for(int i=0; i<Q.size(); i++)                               // actualizam multimea stariloe nefinale
    {
        if((cset[m[Q[i]] ].size())>0)
        {
            string t=cset[m[Q[i]] ];
            cset[m[Q[i]] ].clear();
            Q[i]=t;
        }
        else
        {
            Q.erase(Q.begin()+i,Q.begin()+i+1);
            i--;
        }
    }

    for(int i=0; i<F.size(); i++)                               // actualizam multimea starilor finale
    {
        if((cset[m[F[i]] ].size())>1)
        {
            string t=cset[m[F[i]] ];
            cset[m[F[i]] ].clear();
            F[i]=t;
        }
        else
        {
            F.erase(F.begin()+i,F.begin()+i+1);
            i--;
        }
    }
}



void DFA::min_DFA(int sym)
{
    map<string, int> m;                 // marchez partitia (int) de care apartine fiecare stare (string)
    vector<vector<string> > set;
    vector<string> t, cset;
    int s,cnset,max_set,eqset;
    bool change=1, ok=0;

    for(int i=0; i<Q.size(); i++)       // initial toate starile neterminale fac parte din aceeasi partitie
    {
        m[Q[i]]=0;
        t.push_back(Q[i]);
    }
    set.push_back(t);
    t.clear();
    for(int i=0; i<F.size(); i++)       // starile finale formeaza alta partitie
    {
        m[F[i]]=1;
        t.push_back(F[i]);
    }
    set.push_back(t);
    t.clear();

    cnset=2;
    max_set=2;                                          // numarul maxim de partitii
    while(change)
    {
        for(s=0; s<sym; s++)
        {
            change=0;
            for(int i=0; i<cnset; i++)                  // parcurgem fiecare partitie
            {
                ok=0;
                cset=set[i];
                string p=delta[cset[0]][s];             // starea corespunzatoare primei stari din partitia pe care o evaluam si a simbolului s
                eqset=m[delta[cset[0]][s] ];            // partitia din care face parte p
                for(int j=1; j<cset.size(); j++)
                {
                    if(m[delta[cset[j]][s] ]!=eqset)    // conditie ce marcheaza starile separabile
                    {
                        change=1;
                        ok=1;
                        m[cset[j]]=max_set;             // starea separabila este adaugata in alta partitie
                        p=cset[j];
                    }
                }
                if(ok)
                {
                    max_set++;                          // creste nr partitiilor
                    ok=0;
                }
            }
            if(change)
            {
                update_set(set,m,max_set,cnset);
            }
        }
    }

    vector<string> comb;
    string temp;
    for(int i=0; i<set.size(); i++)
    {
        temp.clear();
        for(int j=0; j<set[i].size(); j++) temp+=set[i][j];         // pentru fiecare partitie in parte, concatenam starile intr-una singura
        comb.push_back(temp);                                       // noul vector de stari
    }
    set.clear();

    update_table(comb,m);                                           // actualizam mapa de tranzitii
}

int main()
{
    DFA M;
    cin>>M;
    //cout<<M;
    cout<<endl;
    M.min_DFA(M.getSigma().size()-1);
    cout<<M;
}
