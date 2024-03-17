#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>

using namespace std;

#define fi first
#define se second
#define all(x) x.begin(),x.end()
#define sz(x) (int)x.size()

mt19937 gen(91238243529344534);

vector<pair<pair<int, int>, pair<int, int>>>squares{
{{0, 0}, {2, 2}},{{0, 3}, {2, 5}},{{0, 6}, {2, 8}},
{{3, 0}, {5, 2}},{{3, 3}, {5, 5}},{{3, 6}, {5, 8}},
{{6, 0}, {8, 2}},{{6, 3}, {8, 5}},{{6, 6}, {8, 8}},
};

vector<vector<int>>field(9, vector<int>(9));
bool in[9][9];
int ver[9][10];
int hor[9][10];

int f(){
    int res = 0;
    for (int i = 0; i < 9; i++){
        vector<int>temp(field[i]);
        sort(all(temp));
        res += 9 - (unique(all(temp)) - temp.begin());
    }
    for (int i = 0; i < 9; i++){
        vector<int>temp;
        for (int j = 0; j < 9; j++){
            temp.push_back(field[j][i]);
        }
        sort(all(temp));
        res += 9 - (unique(all(temp)) - temp.begin());
    }
    return res;
}

int main() {
    ifstream fin("sudoku.txt");
    ofstream fout("result.txt");
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            char h;
            fin >> h;
            if (h != '*'){
                in[i][j] = 1;
                field[i][j] = (h - '0');
            }
        }
    }
    vector<vector<pair<int, int>>>klet(9);
    int k = 0;
    for (auto[st, fn] : squares){
        set<int>s{1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (int i = st.fi; i <= fn.fi; i++){
            for(int j = st.se; j <= fn.se; j++){
                if (in[i][j])s.erase(field[i][j]);
            }
        }
        for (int i = st.fi; i <= fn.fi; i++){
            for(int j = st.se; j <= fn.se; j++){
                if (!in[i][j]){
                    klet[k].push_back({i, j});
                    field[i][j] = *s.begin();
                    s.erase(s.begin());
                }
            }
        }
        k++;
    }

    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            hor[i][field[i][j]]++;
            ver[j][field[i][j]]++;
        }
    }
    double t = 1;
    uniform_real_distribution<double>unif(0, 1);
    int fv = f();
    int r = 0;
    cout << "The temperature and the total number of repetitions in rows and columns every 1e6 iterations" << endl;
    while(fv != 0){
        t *= 0.99999999;
        int sq = gen() % 9;
        int pepe = (gen() % sz(klet[sq]));
        int pepe2 = (pepe + (gen() % (sz(klet[sq]) - 1)) + 1) % sz(klet[sq]);
        auto a = klet[sq][pepe];
        auto b = klet[sq][pepe2];
        int fu = fv;
        if (hor[a.fi][field[a.fi][a.se]] > 1)fu--;
        if (ver[a.se][field[a.fi][a.se]] > 1)fu--;
        if (hor[b.fi][field[b.fi][b.se]] > 1)fu--;
        if (ver[b.se][field[b.fi][b.se]] > 1)fu--;


        hor[a.fi][field[a.fi][a.se]]--;
        ver[a.se][field[a.fi][a.se]]--;
        hor[b.fi][field[b.fi][b.se]]--;
        ver[b.se][field[b.fi][b.se]]--;

        if (hor[b.fi][field[a.fi][a.se]] > 0)fu++;
        if (ver[b.se][field[a.fi][a.se]] > 0)fu++;
        if (hor[a.fi][field[b.fi][b.se]] > 0)fu++;
        if (ver[a.se][field[b.fi][b.se]] > 0)fu++;

        if (fu < fv || unif(gen) < exp((fv - fu) / t)){
            fv = fu;
            hor[b.fi][field[a.fi][a.se]]++;
            ver[b.se][field[a.fi][a.se]]++;
            hor[a.fi][field[b.fi][b.se]]++;
            ver[a.se][field[b.fi][b.se]]++;
            swap(field[a.fi][a.se], field[b.fi][b.se]);
        }
        else{
            hor[a.fi][field[a.fi][a.se]]++;
            ver[a.se][field[a.fi][a.se]]++;
            hor[b.fi][field[b.fi][b.se]]++;
            ver[b.se][field[b.fi][b.se]]++;
        }
        if (!r)cout << t << ' ' << fv << endl;
        r = (r + 1) % 1000000;
    }
    cout << t << ' ' << fv << endl;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            fout << field[i][j] << ' ';
        }
        fout << endl;
    }
    cout << "Done!" << endl;
    system("PAUSE");
    return 0;
}
