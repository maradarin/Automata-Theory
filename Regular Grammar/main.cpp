#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Derivare
{
    vector<pair<char,string> > prod;                                            // vector de productii
public:
    friend istream& operator>>(istream&, Derivare&);
    bool verifica(string, char);
};

bool Derivare::verifica(string cuvant, char stare)
{
    if (cuvant.length() == 0)
    {
        for (int i = 0; i < prod.size(); i++)
            if (prod[i].first == stare && prod[i].second=="?")
                return true;                                                    // acceptarea cuvantului vid
        return false;
    }

    bool ok = false;
    for (int i = 0; i < prod.size(); i++)
        if (prod[i].first == stare)
        {
            string res = prod[i].second;
            if (res.size() == 1)
            {
                if (res[0]>='a')
                {
                    if (cuvant.compare(res) == 0)                               // cazul in care avem doar un terminal
                        ok = true;
                }
                else ok = (verifica(cuvant, res[0])) ? true : ok;               // altfel verificam pt lambda
            }
            else
            {
                if (res[0]>='a' && cuvant[0] == res[0])                              // daca prima litera din cuvant coincide cu terminalul
                    ok = (verifica(cuvant.substr(1), res[1])) ? true : ok;           // se avanseaza la urmatoarea litera din cuvant si se preia neterminalul
            }
        }

    return ok;
}

void print(vector<pair<char,string> > d)
{
    for (int i = 0; i < d.size(); i++)
        cout << d[i].first << " " << d[i].second << endl;
}

istream& operator>>(istream& in, Derivare& D)
{
	vector<Derivare> input;
    int n;
    in >> n;

    for (int i = 0; i < n; i++)
    {
        char neterminal;
        in >> neterminal;
        string res;
        in >> res;
        D.prod.push_back(make_pair(neterminal,res));
    }
    return in;
}

int main()
{
    ifstream file("date.in");
    // cuvinte de forma a^(2n)*b^(3k) cu n,k>=0
    Derivare d;
    file>>d;

    if (d.verifica("", 'S')) cout << "True!" << endl;
    else cout << "False.." << endl;

    if (d.verifica("aabbb", 'S')) cout << "True!" << endl;
    else cout << "False.." << endl;

    if (d.verifica("abb", 'S')) cout << "True!" << endl;
    else cout << "False.." << endl;

    return 0;
}
