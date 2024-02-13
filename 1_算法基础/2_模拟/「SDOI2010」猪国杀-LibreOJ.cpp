/*
坑点很多，列出一些：
1. 出了某个牌以后，可能导致前面的牌又可以用了，比如装了Z，前面的K可以用
2. 使用锦囊牌后，从使用者开始（包括使用者）逆时针判断是否有人使用J，比如MP用了N，MP可以给ZP用J帮它挡
3. 当前出牌的猪可能会die了
4. 所有牌用完以后，一直摸都是最后一张牌（离谱，看别人题解得到的）
*/
#include<iostream>
#include<string>
#include<queue>
using namespace std;

class Card {
public:
    Card(string t) {
        type = t;
    }
    string type;
    Card* next = NULL;
    Card* pre = NULL;
};

class Pig {
public:
    Pig(string _type) {
        type = _type;
    }
    void addCard(string card) {
        Card* newCard = new Card(card);
        if (cardHead == NULL) {
            cardHead = newCard;
            cardTail = newCard;
        }
        else {
            cardTail->next = newCard;
            newCard->pre = cardTail;
            cardTail = newCard;
        }
    }
    bool hasCard(string card) {
        Card* cur = cardHead;
        while (cur != NULL) {
            if (cur->type == card)return true;
            cur = cur->next;
        }
        return false;
    }
    Card* findCard(string card) {
        Card* cur = cardHead;
        while (cur != NULL) {
            if (cur->type == card)return cur;
            cur = cur->next;
        }
        return NULL;
    }
    bool removeCard(string card) {
        Card* c = findCard(card);
        if (c != NULL) {
            if (c == cardHead && c == cardTail) {
                cardHead = cardHead->next = cardHead->pre = NULL;
                cardTail = cardTail->next = cardTail->pre = NULL;
            }
            else if (c == cardHead) {
                cardHead = c->next;
                cardHead->pre = NULL;
            }
            else if (c == cardTail) {
                cardTail = c->pre;
                cardTail->next = NULL;
            }
            else {
                c->pre->next = c->next;
                c->next->pre = c->pre;
            }
            return true;
        }
        return false;
    }
    void setState() {
        usedF = false;
    }
    void removeAllCard() {
        if (cardHead == NULL)return;
        Card* cur = cardHead;
        Card* next = NULL;
        while (cur != NULL) {
            next = cur->next;
            cur = next;
        }
        cardHead = cardHead->next = cardHead->pre = NULL;
        cardTail = cardTail->next = cardTail->pre = NULL;
        equip = false;
    }
    virtual bool is_J_playable(Pig*, bool) = 0;
    virtual bool is_K_playable() = 0;
    virtual Pig* is_F_playable() = 0;
    int hp = 4;
    int id = 0;
    bool equip = false;
    bool dead = false;
    int attitude = 0; // 未表明0，类反1、跳反2、 跳忠3
    bool usedF = false;
    Pig* nextPig = NULL;
    string type;
    Card* cardHead = NULL;
    Card* cardTail = NULL;
};

class MP : public Pig {
public:
    MP() : Pig("MP") {
        attitude = 3;
    }
    bool is_J_playable(Pig* to, bool isGood) { // 
        if (!hasCard("J")) {
            return false;
        }
        return isGood ? to->attitude == 1 || to->attitude == 2 : to->attitude == 3;
    }
    bool is_K_playable() {
        return nextPig->attitude == 1 || nextPig->attitude == 2;
    }
    Pig* is_F_playable() {
        Pig* p = nextPig;
        while (p != this) {
            if (p->attitude == 1 || p->attitude == 2) {
                return p;
            }
            p = p->nextPig;
        }
        return NULL;
    }
};
class ZP : public Pig {
public:
    ZP() : Pig("ZP") {}
    bool is_J_playable(Pig* to, bool isGood) {
        if (!hasCard("J")) {
            return false;
        }
        return isGood ? to->attitude == 2 : to->attitude == 3;

    }
    bool is_K_playable() {
        return nextPig->attitude == 2;
    }
    Pig* is_F_playable() {
        Pig* p = nextPig;
        while (p != this) {
            if (p->attitude == 2) {
                return p;
            }
            p = p->nextPig;
        }
        return NULL;
    }

};
class FP : public Pig {
public:
    FP() : Pig("FP") {}
    bool is_J_playable(Pig* to, bool isGood) { // 无懈需要抵消的类型
        if (!hasCard("J")) {
            return false;
        }
        return isGood ? to->attitude == 3 : to->attitude == 2;
    }
    bool is_K_playable() {
        return nextPig->attitude == 3;
    }
    Pig* is_F_playable() {
        Pig* p = nextPig;
        while (p != this) {
            if (p->type == "MP") {
                return p;
            }
            p = p->nextPig;
        }
        return NULL;
    }
};

class Game {
private:
    vector<Pig*> pigs;
    queue<string> cardHeap{};
    const int INIT_CARDS_NUM = 4;
    const int MAX_HP = 4;
    int numPigs = 0;
    int numCardsInHeap = 0;
    int numFP = 0;
    bool gameOver = false;

public:
    Game() {}
    void getCardFromHeap(Pig* p, int num) {
        for (int i = 0;i < num && cardHeap.size()>0;i++) {
            p->addCard(cardHeap.front());
            if (cardHeap.size() > 1)cardHeap.pop();
        }
    }
    void pigDied(Pig* diedPig) {
        diedPig->dead = true;
        Pig* p = diedPig->nextPig;
        while (p->nextPig != diedPig) {
            p = p->nextPig;
        }
        p->nextPig = diedPig->nextPig;
    }
    void setGameOver() {
        gameOver = true;
    }
    void getHurt(Pig* attacker, Pig* p, int delta_hp, string card) {
        p->hp -= delta_hp;
        while (p->hp <= 0 && p->removeCard("P")) {
            p->hp++;
        }
        bool isDied = p->hp <= 0;
        if (p->type == "MP") {
            if (attacker->attitude <= 1 && (card == "N" || card == "W")) { // 没跳身份则且用N或W造成伤害，则为类反猪
                attacker->attitude = 1;
            }
            if (isDied) {
                setGameOver();
            }
        }
        else if (p->type == "ZP") {
            if (isDied && attacker->type == "MP") {
                attacker->removeAllCard();
            }
        }
        else {
            if (isDied) {
                numFP--;
                if (numFP == 0) {
                    setGameOver();
                }
                else {
                    getCardFromHeap(attacker, 3);
                }
            }
        }
        if (isDied) {
            pigDied(p);
        }
    }
    // 使用万箭齐发/南猪入侵
    void playAllEffect(Pig* pig, string card, string escape) {
        pig->removeCard(card);
        Pig* p = pig->nextPig;
        while (p != pig) {
            // 无懈
            Pig* h = checkJ(pig, p, false);
            if (h != NULL && playJ(h, p, true)) {
                p = p->nextPig;
                continue;
            }
            if (!p->removeCard(escape)) {
                getHurt(pig, p, 1, card);
            }
            if (gameOver)break;
            p = p->nextPig;
        }
    }

    Pig* checkJ(Pig* from, Pig* to, bool isGood) { //from对to使用了锦囊牌，isGood献殷情, 表敌意
        if (to->attitude <= 1)return NULL;
        Pig* p = from;
        do {
            if (p->is_J_playable(to, isGood)) {
                return p;
            }
            p = p->nextPig;
        } while (p != from);
        return NULL;
    }

    bool playJ(Pig* p1, Pig* p2, bool isGood) { // p2受到攻击/无懈，p1使用无懈帮p2挡，p1的行为类型
        p1->removeCard("J");
        if (isGood) {
            p1->attitude = p2->attitude == 3 ? 3 : 2;
        }
        else {
            p1->attitude = p2->attitude == 2 ? 3 : 2;
        }
        // 无懈
        Pig* h = checkJ(p1, p2, isGood);
        if (h == NULL) {
            return true;
        }
        bool isBlock = playJ(h, p2, !isGood);
        return !isBlock;
    }
    // p1对p2使用决斗
    void playF(Pig* p1, Pig* p2) {
        p1->removeCard("F");
        if (p2->attitude != 1) {
            p1->attitude = p2->attitude == 3 ? 2 : 3;
        }
        // 无懈
        Pig* h = checkJ(p1, p2, false);
        if (h != NULL && playJ(h, p2, true)) {
            return;
        }
        if (p2->type == "ZP" && p1->type == "MP") {
            getHurt(p1, p2, 1, "F");
            return;
        }
        // fight
        Pig* cur = p2;
        while (cur->removeCard("K")) {
            cur = cur == p1 ? p2 : p1;
        }
        getHurt(cur == p1 ? p2 : p1, cur, 1, "F");
    }

    void playK(Pig* p1, Pig* p2) {
        p1->removeCard("K");
        p1->usedF = true;
        if (p2->attitude != 1) {
            p1->attitude = p2->attitude == 3 ? 2 : 3;
        }
        if (!p2->removeCard("D")) {
            getHurt(p1, p2, 1, "K");
        }
    }

    bool playCard(Pig* p, string card) {
        if (card == "K") {
            if ((!p->usedF || p->equip) && p->is_K_playable()) {
                playK(p, p->nextPig);
                return true;
            }
        }
        else if (card == "P") {
            if (p->hp < MAX_HP && p->removeCard("P")) {
                p->hp++;
                return true;
            }
        }
        else if (card == "Z") {
            p->removeCard("Z");
            p->equip = true;
            return true;
        }
        // 以下是锦囊牌
        else if (card == "F") {
            Pig* f = p->is_F_playable();
            if (f != NULL) {
                playF(p, f);
                return true;
            }
        }
        else if (card == "N") {
            playAllEffect(p, "N", "K");
            return true;
        }
        else if (card == "W") {
            playAllEffect(p, "W", "D");
            return true;
        }
        return false;
    }
    void pigPlay(Pig* p) {
        // 摸牌阶段
        getCardFromHeap(p, 2);
        // 出牌阶段
        Card* card = p->cardHead;
        while (card != NULL) {
            if (playCard(p, card->type)) {
                card = p->cardHead;
            }
            else {
                card = card->next;
            }
            if (p->dead || gameOver) {
                break;
            }
        }
        p->setState();
    }

    void printResult() {
        if (pigs[0]->dead) {
            cout << "FP" << endl;
        }
        else {
            cout << "MP" << endl;
        }
        for (Pig* p : pigs) {
            if (p->dead) {
                cout << "DEAD" << endl;
            }
            else {
                printCards(p);
            }
        }
    }
    void printCards(Pig* p) {
        Card* c = p->cardHead;
        while (c != NULL) {
            cout << c->type << " ";
            c = c->next;
        }
        cout << endl;
    }

    void run() {
        Pig* p = pigs[0];
        while (!gameOver && cardHeap.size() > 0) {
            pigPlay(p);
            p = p->nextPig;
        }
        printResult();
    }

    void inputPigs() {
        int n, m;
        cin >> n >> m;
        numPigs = n;
        numCardsInHeap = m;
        string card, type;
        Pig* pig;
        for (int i = 0;i < n;i++) {
            cin >> type;
            if (type == "MP") {
                pig = new MP();
            }
            else if (type == "ZP") {
                pig = new ZP();
            }
            else {
                pig = new FP();
                numFP++;
            }
            pig->id = i;
            pigs.push_back(pig);
            for (int j = 0;j < INIT_CARDS_NUM;j++) {
                cin >> card;
                pigs[i]->addCard(card);
            }
        }
        for (int i = 1;i < numPigs;i++) {
            pigs[i - 1]->nextPig = pigs[i];
        }
        pigs[n - 1]->nextPig = pigs[0];
    }

    void inputCardHeap() {
        string card;
        for (int i = 0;i < numCardsInHeap;i++) {
            cin >> card;
            cardHeap.push(card);
        }
    }

};

int main() {
    Game game;
    game.inputPigs();
    game.inputCardHeap();
    game.run();
    return 0;
}