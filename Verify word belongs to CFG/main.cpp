#include <iostream>
#include <cstring>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

string cuv = "aaabbb";      //cuvantul de verificat

struct reg                  //regula
{
    char et;                //eticheta (neterminal)
    vector<string> prod;    //vector de productii
};

vector<reg> citireFisier()
{
    vector<reg> reguli(2); //vectorul care va fi transmis in programul principal, cu 2 reguli

    reguli[0].et='S';
    reguli[0].prod.push_back("AbAbAbA");
    reguli[1].et='A';
    reguli[1].prod.push_back("aA");
    reguli[1].prod.push_back("bA");
    reguli[1].prod.push_back("@");
    // {w din {a,b}* | w contine min 3 b}

    return reguli;
}

void afisareGramatica(const vector<reg>& a)
{
    for (int i=0; i<a.size(); i++)
    {
        cout << a[i].et << "->";
        for(int j=0; j<a[i].prod.size(); j++)
            cout << a[i].prod[j] << " ";
        cout << endl;
    }
}

int gasesteIndex(const vector<reg>& a, const char& x)
{
    for(int i = 0; i < a.size(); i++)
        if(a[i].et == x)
            return i;

    return -1;
}

bool verifica(const string& a, const string& b)
{
    //b = cuvantul nostru
    //a = cuvantul verificat
    if(a.size() > b.size() + 1)
        return false;

    for(int k = 0; k < b.size(); k++)
        if( a[k] != b[k] )
            return isupper(a[k]) != 0;

    return true;
}

bool apartine(const vector<reg>& gram, const char& start)
{
    int index;
    index = gasesteIndex(gram, start);
    queue<string> stiva;
    string a, auxstr;
    int iaux;
    bool gasit = false;

    if(index != -1)
    {
        for (int i=0; i< gram[index].prod.size(); i++)
            stiva.push(gram[index].prod[i]);

        while(!stiva.empty())
        {
            a = stiva.front();
            stiva.pop();

            for(int i = 0; i < a.size(); i++)           //parcurgem toate literele din cuvantul actual
            {
                if(isupper(a[i]))                       //daca este neterminal
                {
                    gasit = true;
                    iaux = gasesteIndex(gram, a[i]);    //extrage din gramatica regula cu acest neterminal

                    for(int j = 0; j < gram[iaux].prod.size(); j++)  //pentru fiecare productie din regula
                    {
                        auxstr = a;
                        if(gram[iaux].prod[j] == "@")   //daca este LAMBDA atunci se elimina neterminalul
                            auxstr.replace(i, 1, "");
                        else                            //daca nu este lambda se inlocuieste cu productia
                            auxstr.replace(i, 1, gram[iaux].prod[j]);

                        //cout << a << " : ";         // se arata de unde se pleaca

                        if(verifica(auxstr, cuv))   // si unde se ajunge
                        {
                            //cout << auxstr << " ";
                            stiva.push(auxstr);     // noua productie se adauga in stiva si se continua algoritmul
                        }
                        if(auxstr == cuv)           //daca am gasit cuvantul, terminam
                            return true;
                        //cout << endl;

                    }
                    //cout << endl;
                }
            }
        }
    }
    return false;                                   //nu am gasit cuvantul
}

int main()
{

    char start = 'S';
    vector<reg> gram = citireFisier();              // gramatica noastra
    afisareGramatica(gram);
    cout << endl;

    if(apartine(gram, start))
        cout << "\nMerge";
    else
        cout << "\nNu merge";

    return 0;
}
