#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <cctype>
#include <map>
#include <iterator>
#include <chrono>
using namespace std;
using namespace std::chrono;


//read textfile and store each lists to each vector
void readfile(string marketfile, string listfile, vector<string> &mword, vector<string> &lword){

    ifstream mFile(marketfile);
    ifstream lFile(listfile);

    string str;
    while(getline(mFile, str)){
        mword.push_back(str);
    }

    while(getline(lFile,str)){
        lword.push_back(str);
    }

}


//check if name in the list exists in market.
//if name doesn't exist in the list, then return false. Otherwise, return true
bool notExist(vector<string> problem, map<string, int> marketList){

        map<string, int>::iterator it;
        string split;
        int pos;

        for(int i = 0; i<problem.size(); i++){
            bool find = false;
            split = problem.at(i);
            pos = split.find(" ");
            for(it = marketList.begin(); it != marketList.end(); it++){
                if(it->first == problem.at(i).substr(0,pos)){
                    find = true;
                    
                }
            }
            if(find == false){
                cout << problem.at(i) << " doesn't exist in the market " << endl;
                return false;
            }
        }

        return true;
   

}

//generate all the possible combination
//ex) if there is 3 cards in the list, then this function make 8(2^3) combinations, pusing back to 2d vector
void comb(vector<string>& list_card,vector<string>& sublist, vector<vector<string>>& all_combination, int start){

    all_combination.push_back(sublist);

    for(int i = start; i< list_card.size(); i++){
        sublist.push_back(list_card[i]);

        comb(list_card, sublist, all_combination, i + 1);

        sublist.pop_back();

    }
}

//recieve card list from each problem and make all the possible combinations with comb function
//return all the combinations, which is 2d vector, from list
vector<vector<string>> combination(vector<string>& list_card){


    vector<string> combination;
    vector<vector<string>> all_combination;

    int start = 0;
    comb(list_card, combination, all_combination, start);
    
    return all_combination;
}

//receving all the combination and list from market
//compute max profit
//if the the price of cards from combination is less or eqaul to the maximum amount we can spend,
//calcualte the profit by difference with market price and the price of cards in the list
//if the profit is higher than the previous max profit, then assign to the maxprofit
//return the maxprofit with the list of cards
int ComputeMaxProfit(int weight, vector<vector<string>> combination, map<string, int> market_list, vector<string> &maxprofit_list){

    map<string, int>::iterator it;
    vector<string> list;
    vector<string> max_list;
    int maxProfit, consideration, profit, marketprice = 0;
    string split;
    int pos = 0;


    int i, j = 0;
     
    for (i = 0; i < combination.size(); i++) {
        consideration = 0;
        marketprice = 0;
        for (j = 0; j < combination[i].size(); j++){
            split = combination[i][j];
            pos = split.find(" ");
            consideration += stoi(split.substr(pos, split.length()-1));
            marketprice += market_list.find(split.substr(0,pos))->second;
            list.push_back(split);
        }
        if(consideration <= weight){
            profit = marketprice - consideration;
            if(maxProfit < profit){
                maxProfit = profit;
                max_list.clear();
                max_list.insert(max_list.begin(), list.begin(), list.end());
            
            }
        }
        list.clear();
    } 
    maxprofit_list.insert(maxprofit_list.begin(), max_list.begin(), max_list.end());
    
    return maxProfit;
    
}

   

int main(int argc, char* argv[]){

    const char *optstring = "m:p:";
    string market_file, price_file;
    vector<string> mword;
    vector<string> lword;
    map<string, int> price_list;
    map<string, int> market_list;
    int o;

    
    while((o = getopt(argc,argv,optstring)) != -1){
        switch(o){
            case 'm':
                market_file = argv[2];
                break;
            
            case 'p' :
                price_file = argv[4];
                break;
            
            case '?':
                printf("error optopt: %c\n", optopt);
                printf("error opterr: %c\n", opterr);
                break;
        }
    }

    readfile(market_file, price_file, mword, lword);

    
    string split;
    string name;
    string price;
    int pos;
    int i = 0;
    int card_num = 0;
    int weight = 0;

    for(int j = 0; j<mword.size(); j++){
        split = mword.at(j);
        pos = split.find(" ");
        if(!isdigit(mword.at(j)[0])){
            name = mword.at(j).substr(0, pos);
            price = mword.at(j).substr(pos+1,mword.at(j).length() -1);
            market_list.insert(pair<string, int>(name,stoi(price)));
        }
    }

    vector<string> single_problem;
    vector<string> maxprofit_list;
    ofstream output;
    output.open("output.txt");
    //int count = 1;


    //run the list till the end
    high_resolution_clock::time_point start = high_resolution_clock::now();
    while(i != lword.size()){



        split = lword.at(i);
        pos = split.find(" ");

        if(!isdigit(lword.at(i)[0])){
            single_problem.push_back(lword.at(i));
        }else{
            card_num = stoi(lword.at(i).substr(0,pos));
            weight = stoi(lword.at(i).substr(pos+1,lword.at(i).length()-1));
        }

        if(card_num == single_problem.size()){
            if(notExist(single_problem, market_list) == true){
                vector<vector<string>> lists = combination(single_problem);
                int profit = ComputeMaxProfit(weight, lists, market_list, maxprofit_list);
                high_resolution_clock::time_point end = high_resolution_clock::now();
                duration<double> time = duration_cast<duration<double>>(end - start);
                output << card_num << " " << profit << "$ " << maxprofit_list.size() << " " << time.count() << "seconds" << endl;
                single_problem.clear();
                maxprofit_list.clear();
                card_num = 0;
                weight = 0;
                start = high_resolution_clock::now();
            }else{
                break;
            }
        }
        i++;

    }
}