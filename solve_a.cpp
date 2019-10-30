/*
Сделать:
    - поменять все сортироки на использованиеи мултисета
    - для этого написать класс в качестве компратора(покапаться в истории)
    - добить до правильного ответа через тыкание не только в наибольший, а во все по порядку
    - оптимизировать O(n^3) до O(n^2*log(n))
    - PROFIT!!!
*/
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
    bool operator() (const pair<double, pair<double, double>> &a, const pair<double, pair<double, double>> &b)
    {
        // returns true if a should go before b
        return a.first > b.first;
    }
};

struct Compare2{
    bool operator() (const pair<pair<double, double>, pair<double, double>> &a, const pair<pair<double, double>, pair<double, double>> &b)
    {
        // returns true if a should go before b
        return a.first.first > b.first.first;
    }
};

vector <pair<pair<double, double>, double>> group;
multiset <pair<pair<double, double>, pair<double, double>>, Compare2> ap;
multiset <pair<double, pair<double, double>>, Compare1> a, p; // массив активов + пассивов, первым значением пары является номинал, максимальая ставка и минимальная ставка
map <int, vector<int>> groups; // словарь, у которого ключом является номер группы, а значением вектор элементов, которые входят в группу
multiset <pair<pair<double, double>, pair<double, double>>, Compare2>::iterator it_ap, tmp_it_ap;
multiset <pair<double, pair<double, double>>, Compare1>::iterator it_a, it_p, tmp_it_a, tmp_it_p;

void solve(){
    cout << "START SOLVING" << endl;
    bool flag = true;
    //cout << "I AM HERERERERE!!!" << endl;
    pair<double, pair<double, double>> tmp;
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
                    if((((*it_a).second.first - 0.15) <= (*tmp_it_p).second.second) && (((*it_a).second.second + 0.15) >= (*tmp_it_p).second.first)){
                        //cout << "NETTING IN PROGRESS" << endl;
                        tmp.first = (*it_a).first - (*tmp_it_p).first;
                        tmp.second.first = max((*it_a).second.first, (*tmp_it_p).second.first);
                        tmp.second.second = min((*it_a).second.second, (*tmp_it_p).second.second);
                        a.erase(it_a);
                        a.insert(tmp);
                        p.erase(tmp_it_p);
                        ap.clear();
                        for(tmp_it_a = a.begin(); tmp_it_a != a.end(); tmp_it_a++)
                            ap.insert({{(*tmp_it_a).first, 1}, (*tmp_it_a).second});
                        for(tmp_it_p = p.begin(); tmp_it_p != p.end(); tmp_it_p++)
                            ap.insert({{(*tmp_it_p).first, -1}, (*tmp_it_p).second});
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
                    if((((*it_p).second.first - 0.15) <= (*tmp_it_a).second.second) && (((*it_p).second.second + 0.15) >= (*tmp_it_a).second.first)){
                        //cout << "ACTIVE ELEMENT WAS FOUND .. NETTING IN PROGRESS" << endl;
                        tmp.first = (*it_p).first - (*tmp_it_a).first;
                        tmp.second.first = max((*it_p).second.first, (*tmp_it_a).second.first);
                        tmp.second.second = min((*it_p).second.second, (*tmp_it_a).second.second);
                        p.erase(it_p);
                        p.insert(tmp);
                        a.erase(tmp_it_a);
                        ap.clear();
                        for(tmp_it_a = a.begin(); tmp_it_a != a.end(); tmp_it_a++){
                            ap.insert({{(*tmp_it_a).first, 1},(*tmp_it_a).second});
                        }
                        for(tmp_it_p = p.begin(); tmp_it_p != p.end(); tmp_it_p++){
                            ap.insert({{(*tmp_it_p).first, -1},(*tmp_it_p).second});
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
    vector<pair<double, double>> data = get_credit_deposit_rate("data_mammoth_1month.csv");
    for(int i = 0; i < data.size(); i++){
        //cout << data[i] << endl;
        if(data[i].first < 0){
            ap.insert({{(-1) * data[i].first, -1}, {data[i].second, data[i].second}});
            p.insert({(-1) * data[i].first, {data[i].second, data[i].second}});
        }
        else{
            ap.insert({{data[i].first, 1}, {data[i].second, data[i].second}});
            a.insert({data[i].first, {data[i].second, data[i].second}});
        }
    }
    solve();
    double active = 0, passive = 0;
    //cout << "ACTIVE: " << endl;
    for(tmp_it_a = a.begin(); tmp_it_a != a.end(); tmp_it_a++){
        //cout << (*tmp_it_a).first << endl;
        active += (*tmp_it_a).first;
    }
    //cout << "PASSIVE: " << endl;
    for(tmp_it_p = p.begin(); tmp_it_p != p.end(); tmp_it_p++){
        //cout << (*tmp_it_p).first << endl;
        passive += (*tmp_it_p).first;
    }
    cout << "RWA: "<< 0.05 * (active + passive) + 0.2 * (fabs(active - passive));
    return 0;
}
