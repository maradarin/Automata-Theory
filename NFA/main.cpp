#include <iostream>
#include <map>
#include <string>
#include <set>
#include <fstream>
using namespace std;

class NFA
{
	set<int> Q, F;
	set<char> Sigma;
	set<int> q0;
	map<pair<int, char>, set<int> > delta;

public:
	NFA() {
	    set<int> z;
	    z.insert(0);
	    this->q0=z;
	    //q0.insert(0);
	}
	NFA(set<int> Q, set<char> Sigma, map<pair<int, char>, set<int> > delta, set<int> q0, set<int> F)
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

	friend istream& operator >> (istream&, NFA&);

	bool intersection(set<int>, set<int>);
	set<int> deltaStar(set<int>, string);
};

bool NFA::intersection(set<int> q, set<int> F)
{
    if (q.size() > F.size()) return intersection(F, q);
    for (set<int>::iterator i = q.begin(); i != q.end(); i++)
        if (F.find(*i) != F.end()) return true;
    return false;
}

set<int> NFA::deltaStar(set<int> q, string w)
{
    int n=w.length(),contor=0;
    set<int> stfinale;
    for(set<int>::iterator j=delta[ {*q.begin(),w[0]}].begin(); j!=delta[ {*q.begin(),w[0]}].end() && delta[{*q.begin(),w[0]}].size()>0; j++)
        stfinale.insert(*j);
    n--;
    if(n==0) return stfinale;
    while(n)
    {
        set<int> aux;
        for(set<int>::iterator i=stfinale.begin(); i!=stfinale.end(); i++)
        {
            for(set<int>::iterator j=delta[ {*i,w[contor+1]}].begin(); j!=delta[ {*i,w[contor+1]}].end() && delta[{*i,w[contor+1]}].size()>0; j++)
                aux.insert(*j);
        }
        n--;
        contor++;
        stfinale.clear();
        for(set<int>::iterator i=aux.begin(); i!=aux.end(); i++) stfinale.insert(*i);
        aux.clear();
    }
    return stfinale;
}

istream& operator >> (istream& f, NFA& M)
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
		f >> ch;
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
	NFA M;

	ifstream fin("nfa.txt");
	//ifstream fin("nfa1.txt");
	fin >> M;
	fin.close();

	set<int> lastState = M.deltaStar(M.getInitialState(), "bwemewjjeewefwjjnewfnj");

	if (M.intersection(lastState,M.getF())==true)
	{
		cout << "Cuvant acceptat";
	}
	else
	{
		cout << "Cuvant respins";
	}

	return 0;
}
