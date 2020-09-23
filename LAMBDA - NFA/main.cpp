#include <iostream>
#include <map>
#include <string>
#include <set>
#include <fstream>
using namespace std;

class LNFA
{
    set<int> Q, F;
    set<char> Sigma;
    set<int> q0;
    map<pair<int, char>, set<int> > delta;

public:
    LNFA()
    {
        set<int> z;
        z.insert(0);
        this->q0=z;
        //q0.insert(0);
    }
    LNFA(set<int> Q, set<char> Sigma, map<pair<int, char>, set<int> > delta, set<int> q0, set<int> F)
    {
        this->Q = Q;
        this->Sigma = Sigma;
        this->delta = delta;
        this->q0 = q0;
        this->F = F;
    }

    set<int> getQ() const { return this->Q; }
    set<int> getF() const { return this->F; }
    set<char> getSigma() const { return this->Sigma; }
    set<int> getInitialState() const { return this->q0; }
    map<pair<int, char>, set<int> > getDelta() const { return this->delta; }

    friend istream& operator >> (istream&, LNFA&);

    bool intersection(set<int>, set<int>);
    set<int> lambda(int);
    void deltaStar(set<int>, string);
};

bool LNFA::intersection(set<int> q, set<int> F)
{
    if (q.size() > F.size()) return intersection(F, q);
    for (set<int>::iterator i = q.begin(); i != q.end(); i++)
        if (F.find(*i) != F.end()) return true;
    return false;
}


set<int> stfinale,inchidere,rez,total;

void LNFA::deltaStar(set<int> q, string w)
{
    if(w.length()==0)
    {
        for(set<int>::iterator j=stfinale.begin(); j!=stfinale.end(); j++)
            rez=lambda(*j);
    }
    else if(w.length()>0)
    {
        inchidere.clear();
        for(set<int>::iterator j=q.begin(); j!=q.end(); j++)
            inchidere=lambda(*j);
        stfinale.clear();
        for(set<int>::iterator i=inchidere.begin(); i!=inchidere.end(); i++)
            {
                total.insert(*i);
                for(set<int>::iterator j=delta[ {*i,w[0]}].begin(); j!=delta[ {*i,w[0]}].end() && delta[ {*i,w[0]}].size()>0; j++)
                stfinale.insert(*j);
            }
        if(stfinale.size()==0 && total.size()<Q.size()) deltaStar(inchidere,w);
        else if(stfinale.size()==0 && total.size()==Q.size()) return;
        else deltaStar(stfinale,w.substr(1, w.length() - 1));
    }
}



set<int> LNFA::lambda(int q)
{
    set<int> sol;
    sol.insert(q);
    for(set<int>::iterator i=delta[ {q,'.'}].begin(); i!=delta[ {q,'.'}].end() && delta[ {q,'.'}].size()>0; i++)
        sol.insert(*i);
    return sol;
}

istream& operator >> (istream& f, LNFA& M)
{
    int noOfStates;
    f >> noOfStates;
    for (int i = 0; i < noOfStates; ++i)
    {
        int q;
        f >> q;
        M.Q.insert(q);
    }

    int noOfLetters;
    f >> noOfLetters;
    for (int i = 0; i < noOfLetters; ++i)
    {
        char ch;
        f >> ch ;
        M.Sigma.insert(ch);
    }

    int noOfTransitions;
    f >> noOfTransitions;
    while(noOfTransitions!=0)
    {
        char ch;
        int i,nr,st;
        f>>i>>ch>>nr;
        for(int j=1;j<=nr;j++) {f>>st; M.delta[ {i,ch}].insert(st);}
        noOfTransitions=noOfTransitions-nr;
    }

    int x;
    f >> x;
    M.q0.insert(x);

    int noOfFinalStates;
    f >> noOfFinalStates;
    for (int i = 0; i < noOfFinalStates; ++i)
    {
        int q;
        f >> q;
        M.F.insert(q);
    }

    return f;
}

int main()
{
    LNFA M;
    ifstream fin("lnfa.txt");
    fin >> M;
    fin.close();

    M.deltaStar(M.getInitialState(),"ababbbb");

    if (M.intersection(rez,M.getF())==true)
    {
    	cout << "Cuvant acceptat";
    }
    else
    {
    	cout << "Cuvant respins";
    }

    return 0;
}
