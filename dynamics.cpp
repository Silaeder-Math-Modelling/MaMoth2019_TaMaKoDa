#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <algorithm>
#include "assert.h"
#include "time.h"
#include <string>

#include "data_getter.cpp"
using namespace std;

template<class T1, class T2>
ostream &operator<<(ostream &out, const pair<T1, T2> &p)
{
    out << p.first << ' ' << p.second;
    return out;
}

template<class T>
ostream &operator<<(ostream &out, const set<T> &v)
{
    if(v.empty())
        return out;
    out << *v.begin();
    for(auto it = next(v.begin()); it != v.end(); ++it)
    {
        out << ' ' << *it;
    }
    return out;
}

template<class T1, class T2, class T3>
ostream &operator<<(ostream &out, const trio<T1, T2, T3> &t)
{
    out << t.first << ' ' << t.second << ' ' << t.third;
    return out;
}


const double EPS = 1e-9;

bool lt(double a, double b)
{
    return b - a > EPS;
}
bool eq(double a, double b)
{
    return abs(b - a) < EPS;
}
bool le(double a, double b)
{
    return lt(a, b) || eq(a, b);
}
bool gt(double a, double b)
{
    return !le(a, b);
}
bool ge(double a, double b)
{
    return gt(a, b) || eq(a, b);
}
bool ne(double a, double b)
{
    return !eq(a, b);
}


using Contract = pair<double, double>;
// first is value (номинал), second is rate
using Fragmentation = vector<vector<int>>;

vector<Contract> contracts;
vector<vector<Fragmentation>> dp;

double rwa(const Fragmentation &groups)
{
    double main_passive, main_active;
    main_passive = main_active = 0;
    for(const vector<int> &group : groups)
    {
        double group_passive, group_active;
        group_passive = group_active = 0;
        for(const int &i : group)
        {
            if(contracts[i].first > 0)
                group_active += contracts[i].first;
            else
                group_passive += -contracts[i].first;
        }
        if(group_passive > group_active)
            main_passive += group_passive - group_active;
        else
            main_active += group_active - group_passive;
    }
    return (main_active + main_passive) * 0.05 + abs(main_active - main_passive) * 0.2;
}

template<class T>
vector<set<T>> super_union(const vector<vector<set<T>>> &v)
{
    vector<set<T>> ans;
    for(const auto &i : v)
    {
        ans.insert(ans.end(), i.begin(), i.end());
    }
    return ans;
}

class Timer
{
public:
    Timer(string message)
    {
        this->timer = clock();
        this->message = move(message);
    }

    ~Timer()
    {
        cerr << fixed << this->message << ": " << (double)(clock() - this->timer) / CLOCKS_PER_SEC << '\n';
    }
private:
    string message;
    clock_t timer;
};

inline Fragmentation get_dp(int i, int j)
{
    if(!dp[i][j].empty())
        return dp[i][j];
    assert(i != j);

    Fragmentation best_fragmentation;
    double best_rwa = numeric_limits<double>::max();
    for(int snake1 = i; snake1 <= j; ++snake1)
    {
        for(int snake2 = snake1; snake2 <= j; ++snake2)
        {
            vector<Fragmentation> got_groups(0);
            got_groups.reserve(3);
            if(snake1 != i)
            {
                got_groups.push_back(get_dp(i, snake1 - 1));
            }
            if(snake1 != snake2)
            {
                got_groups.push_back(get_dp(snake1, snake2 - 1));
            }
            if(snake2 != j)
            {
                got_groups.push_back(get_dp(snake2, j - 1));
            }
            for(Fragmentation &groups : got_groups)
            {
                groups.push_back({j});
                double cur_rwa = rwa(groups);
                if((eq(cur_rwa, best_rwa) && (best_fragmentation.size() < groups.size())) || (lt(cur_rwa, best_rwa)))
                {
                    best_rwa = cur_rwa;
                    best_fragmentation = groups;
                }
                groups.pop_back();
                for(vector<int> &group : groups)
                {
                    double smallest_rate = contracts[*group.begin()].second;
                    if(le(contracts[j].second - smallest_rate, 0.15))
                    {
                        group.push_back(j);
                        double cur_rwa = rwa(groups);
                        if((eq(cur_rwa, best_rwa) && (best_fragmentation.size() < groups.size())) || (lt(cur_rwa, best_rwa)))
                        {
                            best_rwa = cur_rwa;
                            best_fragmentation = groups;
                        }
                        group.pop_back();
                    }
                }
            }
        }
    }
    dp[i][j] = move(best_fragmentation);
    return dp[i][j];
}

double solve()
{
    sort(contracts.begin(), contracts.end(), [](Contract a, Contract b) { return a.second < b.second; });
    vector<Contract> all_contracts = move(contracts);
    vector<vector<Contract>> devided_contracts = {{all_contracts[0]}};
    for(int i = 1; i < all_contracts.size(); ++i)
    {
        if(gt(abs(all_contracts[i].second - devided_contracts.back().back().second), 0.15))
        {
            devided_contracts.emplace_back();
        }
        devided_contracts.back().push_back(all_contracts[i]);
    }

    Fragmentation ans(0);
    int sum = 0;

    for(int _i = 0; _i < devided_contracts.size(); ++_i)
    {
        Timer c("Plank " + to_string(_i));
        contracts = move(devided_contracts[_i]);

        dp.assign(contracts.size(), vector<Fragmentation>(contracts.size()));
        for(int i = 0; i < contracts.size(); ++i)
        {
            for(int j = 0; j < contracts.size(); ++j)
            {
                if(i != j)
                    dp[i][j] = {};
                else
                    dp[i][j] = {{i}};
            }
        }

        Fragmentation part_ans = get_dp(0, contracts.size() - 1);
        for(const vector<int> &group : part_ans)
        {
            ans.emplace_back(group.size());
            for(int &i : ans.back())
            {
                i += sum;
            }
        }

        devided_contracts[_i] = move(contracts);
        sum += devided_contracts[_i].size();
    }
    contracts = move(all_contracts);
    return rwa(ans);
}

int main()
{
    //contracts = {{1, 1}};
    //contracts = {{100, 0.1}, {-700, 0.13}, {200, 0.16}};
    //contracts = {{-107, 0.25}, {982, 0.33}, {511, 0.38}, {382, 0.47}, {-100, 0.62}, {1000, 0.7}};
    //contracts = {{100, 0.1}, {100, 0.15}, {-30, 0.25}, {-69, 0.3}};
    //contracts = {{100, 0.1}, {-54, 0.25}, {54, 0.4}};
    //contracts = {{100, 0.1}, {-59, 0.15}, {-110, 0.25}, {69, 0.3}};
    //contracts = {{100, 0.1}, {-50, 0.15}, {-110, 0.25}, {60, 0.3}, {100, 0.5}, {-50, 0.65},  {50, 0.8}};
    contracts = get_credit_deposit_rate("dannye_mamont_1mes.csv");
    contracts.resize(100);
    {
        Timer t1("Full solution time");
        cout << solve() << endl;
    }
}
