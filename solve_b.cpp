#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>
#include <set>
#include "data_getter.cpp"
#define eps 1e-9

using namespace std;

struct Compare1{
    bool operator() (const pair<pair<double, double>, pair<pair<double, double>, pair<double, double>>> &a, const pair<pair<double, double>, pair<pair<double, double>, pair<double, double>>> &b)
    {
        // returns true if a should go before b
        return a.first.first > b.first.first;
    }
};

struct Compare2{
    bool operator() (const pair<pair<double, double>, pair<pair<double, double>, pair<double, double>>> &a, const pair<pair<double, double>, pair<pair<double, double>, pair<double, double>>> &b)
    {
        // returns true if a should go before b
        return a.first.first > b.first.first;
    }
};

int amount_of_groups = 0;
vector <pair<double, double>> link;
vector <vector<int>> gr;
vector <bool> used;
vector <int> group;
multiset <pair<pair<double, double>, pair<pair<double, double>, pair<double, double>>>, Compare2> ap;
multiset <pair<pair<double, double>, pair<pair<double, double>, pair<double, double>>>, Compare1> a, p; // массив активов + пассивов, первым значением пары является номинал, максимальая ставка и минимальная ставка
multiset <pair<pair<double, double>, pair<pair<double, double>, pair<double, double>>>, Compare2>::iterator it_ap, tmp_it_ap;
multiset <pair<pair<double, double>, pair<pair<double, double>, pair<double, double>>>, Compare1>::iterator it_a, it_p, tmp_it_a, tmp_it_p;

void dfs(int v, int g)
{
    group[v] = g;
    if(used[v])
        return;
    used[v] = true;
    for (int i : gr[v])
            dfs(i, g);
    return;
}

void solve(){
    cout << "START SOLVING" << endl;
    bool flag = true;
    //cout << "I AM HERERERERE!!!" << endl;
    pair<pair<double, double>, pair<pair<double, double>, pair<double, double>>> tmp;
    while(flag){
        //cout << "0" << endl;
        flag = false;
        /*cout << "ACTIVE & PASSIVE: " << endl;
        for(tmp_it_ap = ap.begin(); tmp_it_ap != ap.end(); tmp_it_ap++){
            cout << (*tmp_it_ap).first.second << " " << (*tmp_it_ap).first.first << endl;
        }
        cout << "ACTIVE: " << endl;
        for(tmp_it_a = a.begin(); tmp_it_a != a.end(); tmp_it_a++){
            cout << (*tmp_it_a).first << endl;
        }
        cout << "PASSIVE: " << endl;
        for(tmp_it_p = p.begin(); tmp_it_p != p.end(); tmp_it_p++){
            cout << (*tmp_it_p).first << endl;
        }*/
        //cout << "1" << endl;
        it_a = a.begin();
        it_p = p.begin();
        //cout << "1I AM HERERERERE!!!" << endl;
        for(it_ap = ap.begin(); it_ap != ap.end(); it_ap++){
            if((*it_ap).first.second > 0){
                //cout << "I2 AM HERERERERE!!!" << endl;
                for(tmp_it_p = it_p; tmp_it_p != p.end(); tmp_it_p++){
                    //cout << "FINDING ITEM FOR NETING IN PASSIVE" << endl;
                    if((((*it_a).second.first.first - 0.15) <= (*tmp_it_p).second.first.second) && (((*it_a).second.first.second + 0.15) >= (*tmp_it_p).second.first.first)){
                        //cout << "NETTING IN PROGRESS" << endl;
                        tmp.first.first = (*it_a).first.first - (*tmp_it_p).first.first;
                        tmp.first.second = (*it_a).first.second;
                        link.push_back({(*it_a).first.second, (*tmp_it_p).first.second});
                        tmp.second.first.first = max((*it_a).second.first.first, (*tmp_it_p).second.first.first);
                        tmp.second.first.second = min((*it_a).second.first.second, (*tmp_it_p).second.first.second);
                        a.erase(it_a);
                        a.insert(tmp);
                        p.erase(tmp_it_p);
                        ap.clear();
                        for(tmp_it_a = a.begin(); tmp_it_a != a.end(); tmp_it_a++)
                            ap.insert({{(*tmp_it_a).first.first, 1}, (*tmp_it_a).second});
                        for(tmp_it_p = p.begin(); tmp_it_p != p.end(); tmp_it_p++)
                            ap.insert({{(*tmp_it_p).first.first, -1}, (*tmp_it_p).second});
                        //cout << "BREAK" << endl;
                        flag = true;
                        //cout << "NETTING COMPLITED SUCCESFUly" << endl;
                        break;
                    }
                    //cout << "NO ITEMS FOUNDED FOR NETTING" << endl;
                }
                //cout << "ALREADY HERE" << endl;
                it_a++;
            }
            else{
                //cout << "I3 AM HERERERERE!!!" << endl;
                for(tmp_it_a = it_a; tmp_it_a != a.end(); tmp_it_a++){
                    //cout << "FINDING ELEMENT IN ACTIVE FOR NETTING" << endl;
                    if((((*it_p).second.first.first - 0.15) <= (*tmp_it_a).second.first.second) && (((*it_p).second.first.second + 0.15) >= (*tmp_it_a).second.first.first)){
                        //cout << "ACTIVE ELEMENT WAS FOUND .. NETTING IN PROGRESS" << endl;
                        tmp.first.first = (*it_p).first.first - (*tmp_it_a).first.first;
                        tmp.first.second = (*it_p).first.second;
                        link.push_back({(*it_p).first.second, (*tmp_it_a).first.second});
                        tmp.second.first.first = max((*it_p).second.first.first, (*tmp_it_a).second.first.first);
                        tmp.second.first.second = min((*it_p).second.first.second, (*tmp_it_a).second.first.second);
                        p.erase(it_p);
                        p.insert(tmp);
                        a.erase(tmp_it_a);
                        ap.clear();
                        for(tmp_it_a = a.begin(); tmp_it_a != a.end(); tmp_it_a++){
                            ap.insert({{(*tmp_it_a).first.first, 1},(*tmp_it_a).second});
                        }
                        for(tmp_it_p = p.begin(); tmp_it_p != p.end(); tmp_it_p++){
                            ap.insert({{(*tmp_it_p).first.first, -1},(*tmp_it_p).second});
                        }
                        //cout << "BREAK" << endl;
                        flag = true;
                        //cout << "NETTING COMLITE UHUHUHU" << endl;
                        break;
                    }
                }
                /*cerr << "xxxxxxxxxx" << endl;
                for(tmp_it_ap = ap.begin(); tmp_it_ap != ap.end(); tmp_it_ap++){
                    cout << (*tmp_it_ap).first.second << " " << (*tmp_it_ap).first.first << endl;
                }*/
                it_p++;
                //cerr << "GET OUT OF HERE" << endl;
            }
            //cout << "CHECK THE FLAG  SIR" << endl;
            if(flag){
                //cout << "AHHAHAHAHA" << endl;
                break;
            }
        }
        //cout << "FLAG IS: " << flag << endl;

    }
    cout << "END SOLVING" << endl;
}

template<class T1, class T2>
ostream &operator<<(ostream &out, const pair<T1, T2> &p){
    out << p.first << ' ' << p.second;
    return out;
}

int main()
{
    vector<trio<double, double, int>> data = get_credit_deposit_rate_maturity("data_mammoth_3months.csv");
    for(int i = 0; i < data.size(); i++){
        //cout << i << ": " << data[i] << endl;
        if(data[i].first < 0){
            ap.insert({{(-1) * data[i].first, -1}, {{data[i].second, data[i].second}, {data[i].third, data[i].third}}});
            p.insert({{(-1) * data[i].first, i}, {{data[i].second, data[i].second}, {data[i].third, data[i].third}}});
        }
        else{
            ap.insert({{data[i].first, 1}, {{data[i].second, data[i].second}, {data[i].third, data[i].third}}});
            a.insert({{data[i].first, i}, {{data[i].second, data[i].second}, {data[i].third, data[i].third}}});
        }
    }
    solve();
    //cout << "BEGGINING NEXT STAGE" << endl;
    int n = data.size();
    gr.resize(data.size());
    group.resize(data.size());
    for(int i = 0; i < link.size(); i++){
        //cout << link[i] << endl;
    }
    //cout << "MMMMMM" << endl;
    for(int i = 0; i < link.size(); i++){
        //cout << ".";
        gr[link[i].first].push_back(link[i].second);
        gr[link[i].second].push_back(link[i].first);
        //cout << ":";
    }
    //cout << "I'AM OVER HERE" << endl;
    used.resize(data.size());
    used.assign(data.size(), false);
    for(int i = 0; i < n; i++){
        if(!used[i])
            dfs(i, ++amount_of_groups);
    }
    double active = 0, passive = 0, pos = 0;
    //cout << "ACTIVE: " << endl;
    for(tmp_it_a = a.begin(); tmp_it_a != a.end(); tmp_it_a++){
        //cout << (*tmp_it_a).first << endl;
        active += (*tmp_it_a).first.first;
    }
    //cout << "PASSIVE: " << endl;
    for(tmp_it_p = p.begin(); tmp_it_p != p.end(); tmp_it_p++){
        //cout << (*tmp_it_p).first << endl;
        passive += (*tmp_it_p).first.first;
    }
    cout << "RWA: "<< 0.05 * (active + passive) + 0.2 * (fabs(active - passive)) << endl;
    cout << "AMOUNT OF GROUPS: " << amount_of_groups << endl;
    for(int i = 0; i < n; i++){
        cout << i << ": " << group[i] << '\n';
    }
    return 0;
}
