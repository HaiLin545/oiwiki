#include<iostream>
#include<map>
#include<queue>
using namespace std;

typedef  string PigTypes;
typedef  string CardTypes;
int MAX_HP = 4;
int n = 0;
int m = 0;
queue<CardTypes>cardHeap{};

struct Pig {
    PigTypes type;
    int hp = 4;
    bool equip = false;
    map<CardTypes, int> cards{};
};


void round(Pig* pig) {

}

int main() {

    cin >> n >> m;
    Pig* pigs = new Pig[n];
    CardTypes card;
    for (int i = 0;i < n;i++) {
        cin >> pigs[i].type;
        for (int j = 0;j < 4;j++) {
            cin >> card;
            if (pigs[i].cards.count(card) == 0) {
                pigs[i].cards[card] = 1;
            }
            else {
                pigs[i].cards[card]++;
            }
        }
    }

    for (int i = 0;i < m;i++) {
        cin >> card;
        cardHeap.push(card);
    }

    cout << pigs[0].type << endl;
    cout << cardHeap.front();


    return 0;
}
