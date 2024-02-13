#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<utility>

using namespace std;

#define MAX_HERO_NUM 5

int C, M, N, T;
int init_hc[MAX_HERO_NUM], init_ak[MAX_HERO_NUM];
int red_enemy_num = 0;
int blue_enemy_num = 0;
int t = 0;

string formatTime(int);
string alignTime(string, int, char);

enum HeroType {
    // iceman, lion, wolf, ninja, dragon
    dragon, ninja, iceman, lion, wolf
};
enum HomeType {
    none, red, blue
};
enum FightResultType {
    draw, win, loss
};

HeroType red_order[MAX_HERO_NUM] = { iceman, lion, wolf, ninja, dragon };
HeroType blue_order[MAX_HERO_NUM] = { lion, dragon, ninja, iceman, wolf };
const string HeroName[5] = { "dragon",  "ninja","iceman","lion", "wolf" };

class Event {
public:
    int t;
    string c;
    int q;
    int city;
    int id;
    Event(int time, string content, int eventId, int cityId = 0) {
        t = time;
        c = content;
        city = cityId;
        id = eventId;
    }
    void log() {
        cout << formatTime(t) << " " << c << endl;
    }
};
bool cmp(Event e1, Event e2) {
    if (e1.city == e2.city) {
        return e1.id < e2.id;
    }
    return e1.city < e2.city;
}


class EventList {
public:
    vector<Event> list;
    void add(Event* e) {
        // cout << e->c << endl;
        list.push_back(*e);
    }

    void log() {
        sortList();
        for (Event e : list) {
            e.log();
        }
        list.clear();
    }


    void sortList() {
        sort(list.begin(), list.end(), cmp);
    }



}eventList;


class Hero {
public:
    HeroType type;
    int hc;
    int attack;
    int id;
    int loc;
    string name;
    int winCount;
    bool isDie;
    int moveCount;

    // Home* home;
    HomeType side;
    Hero(HeroType heroType, int idx, int healthCount, int init_attack, int location, HomeType home_side) {
        type = heroType;
        hc = healthCount;
        id = idx;
        attack = init_attack;
        loc = location;
        // home = belong;
        side = home_side;
        name = HeroName[heroType];
        winCount = 0;
        moveCount = 0;
        isDie = false;
    };

    void move(int location) {
        loc = location;
        moveCount++;
        if (type == iceman && (moveCount % 2 == 0)) {
            hc -= 9;
            attack += 20;
            if (hc <= 0)hc = 1;
        }
    }
};



class LogFunc {
public:
    string born(Hero*);
    string move(Hero*, int);
    string attack(Hero*, Hero*, int);
    string reattack(Hero*, Hero*, int);
    string die(Hero*, int);
    string yell(Hero*, int);
    string getElement(Hero*, int);
    string changeOwner(string, int);
    string reachEnd(Hero*);
    string loss(string);
    string homeHc(int, string);
}LogF;



class Home {
public:
    int hc;
    int loc;    // red: 0, blue: N+1
    int enemy_num;
    int current;
    int totalCount;
    HomeType side;
    HeroType createOrder[MAX_HERO_NUM];
    Hero* hero;
    vector<Hero*> heros;

    Home(int healthCount, HeroType order[], HomeType home_side) {
        hc = healthCount;
        current = 0;
        enemy_num = 0;
        totalCount = 0;
        side = home_side;
        loc = home_side == red ? 0 : N + 1;
        setOrder(order);
    }

    void setOrder(HeroType order[]) {
        for (int i = 0;i < MAX_HERO_NUM;i++) {
            createOrder[i] = order[i];
        }
    }

    void forwardCurrent() {
        current++;
        if (current == MAX_HERO_NUM) {
            current = 0;
        }
    }

    void heroLeave() {
        hero = NULL;
    }


    void setHc(int new_hc) {
        hc = new_hc;
    }

    HeroType getCurrentHero() {
        return createOrder[current];
    }

    void createHero(int healthCount, int attack) {
        if (healthCount <= hc) {
            hero = new Hero(getCurrentHero(), ++totalCount, healthCount, attack, loc, side);
            setHc(hc - healthCount);
            heros.push_back(hero);
            eventList.add(new Event(t, LogF.born(hero), 1));
            forwardCurrent();
        }
    }

};
Home* Red = NULL;
Home* Blue = NULL;

class City {
public:
    int hc;
    int loc;
    HomeType lastWinSide;
    HomeType owner; // 0: none, 1: red, 2: blue
    Hero* redHero;
    Hero* blueHero;
    City* nextCity;
    City* prevCity;

    City() {
        hc = 0;
        loc = 0;
        owner = none;
        lastWinSide = none;
        redHero = NULL;
        blueHero = NULL;
    }

    void heroEnter(Hero* h) {
        if (h->side == red) {
            redHero = h;
            h->move(loc);
        }
        else {
            blueHero = h;
            h->move(loc);
        }
        eventList.add(new Event(t, LogF.move(h, loc), 1, loc));
    }
    void redHeroLeave() {
        redHero = NULL;
    }
    void blueHeroLeave() {
        blueHero = NULL;
    }
    void oneHeroGetHC() {
        if (hc <= 0) {
            return;
        }
        if (redHero == NULL && blueHero) {
            Blue->setHc(Blue->hc + hc);
            eventList.add(new Event(t, LogF.getElement(blueHero, hc), 7, loc));
            hc = 0;
        }
        if (blueHero == NULL && redHero) {
            Red->setHc(Red->hc + hc);
            eventList.add(new Event(t, LogF.getElement(redHero, hc), 7, loc));
            hc = 0;
        }
    }

    bool isRedTurn() {
        // cout << "city " << loc << " owner = " << owner << endl;
        if (owner == red) {
            return true;
        }
        else if (owner == blue) {
            return false;
        }
        else {
            return (loc % 2 == 1);
        }
    }

    Hero* fight() {
        if (redHero == NULL || blueHero == NULL) {
            return NULL;
        }
        Hero* h1 = NULL;
        Hero* h2 = NULL;
        Hero* winner = NULL;
        Hero* losser = NULL;
        bool redTurn = isRedTurn();
        // cout << redTurn << endl;
        if (redTurn) {
            h1 = redHero;
            h2 = blueHero;
        }
        else {
            h1 = blueHero;
            h2 = redHero;
        }
        int hc1 = h1->hc;
        int hc2 = h2->hc;
        int at1 = h1->attack;
        int at2 = h2->attack;
        FightResultType result = draw;
        eventList.add(new Event(t, LogF.attack(h1, h2, loc), 3, loc));
        if (hc2 <= at1) {
            result = win;
            h1->winCount++;
            winner = h1;
            eventList.add(new Event(t, LogF.die(h2, loc), 5, loc));
            h2->isDie = true;
        }
        else {
            h2->hc -= at1;
            if (h2->type != ninja) {
                eventList.add(new Event(t, LogF.reattack(h2, h1, loc), 4, loc));
                if (hc1 <= at2 / 2) {
                    result = loss;
                    winner = h2;
                    eventList.add(new Event(t, LogF.die(h1, loc), 5, loc));
                    h1->isDie = true;
                }
                else {
                    h1->hc -= at2 / 2;
                }
            }
        }

        if (h1->type == dragon && result != loss) {
            eventList.add(new Event(t, LogF.yell(h1, loc), 6, loc));
        }

        if (h1->type == wolf && result == win && h1->winCount % 2 == 0) {
            h1->hc *= 2;
            h1->attack *= 2;
        }

        if (h1->type == lion && result == loss) {
            h2->hc += hc1;
        }
        if (h2->type == lion && result == win) {
            h1->hc += hc2;
        }


        if (winner == NULL) {
            lastWinSide = none;
        }

        return winner;

    }
};

typedef pair<Hero*, City*> WinnerType;
bool winnerCmp(WinnerType p1, WinnerType p2) {
    Hero* h1 = p1.first;
    Hero* h2 = p2.first;
    // return h1->loc > h2->loc;
    if (h1->side != h2->side) {
        return (h1->side == red);
    }
    else {
        return h1->side == red ? (h1->loc > h2->loc) : (h1->loc < h2->loc);
    }
}


/*
1
99 2 1000
10 20 50 50  30
20 50 50 50  50
Sample:
2
99 2 1000
10 20 50 50  30
20 50 50 50  50
1
10 2 1000
1 2 1 2 1
1 2 1 2 1
20 20 20 20 20
*/



void Game(int M, int N, int T, int init_hc[], int init_ak[]) {
    Red = new Home(M, red_order, red);
    Blue = new Home(M, blue_order, blue);
    City citys[N + 1];
    for (int i = 1;i <= N;i++)citys[i].loc = i;
    bool isGameOver = false;
    t = 0;
    red_enemy_num = 0;
    blue_enemy_num = 0;
    vector<WinnerType> winners;
    while (!isGameOver && t <= T) {
        switch (t % 60)
        {
        case 0:
            Red->createHero(init_hc[Red->getCurrentHero()], init_ak[Red->getCurrentHero()]);
            Blue->createHero(init_hc[Blue->getCurrentHero()], init_ak[Blue->getCurrentHero()]);
            break;
        case 10:
            for (Hero* hero : Red->heros) {
                if (hero->isDie) {
                    continue;
                }
                int currentLoc = hero->loc;
                if (currentLoc == 0) {
                    citys[currentLoc + 1].heroEnter(hero);
                    Red->heroLeave();
                }
                else if (currentLoc < N) {
                    citys[currentLoc + 1].heroEnter(hero);
                    citys[currentLoc].redHeroLeave();
                }
                else if (currentLoc == N) {
                    blue_enemy_num++;
                    hero->move(N + 1);
                    eventList.add(new Event(t, LogF.reachEnd(hero), 0, N + 1));
                    citys[currentLoc].redHeroLeave();
                    if (blue_enemy_num == 2) {
                        eventList.add(new Event(t, LogF.loss("blue"), 10, N + 1));
                        isGameOver = true;
                    }
                }
            }
            for (Hero* hero : Blue->heros) {
                if (hero->isDie) {
                    continue;
                }
                int currentLoc = hero->loc;
                if (currentLoc == N + 1) {
                    citys[currentLoc - 1].heroEnter(hero);
                    Blue->heroLeave();
                }
                else if (currentLoc > 1) {
                    citys[currentLoc - 1].heroEnter(hero);
                    citys[currentLoc].blueHeroLeave();
                }
                else if (currentLoc == 1) {
                    red_enemy_num++;
                    hero->move(0);
                    eventList.add(new Event(t, LogF.reachEnd(hero), 9, 0));
                    citys[currentLoc].blueHeroLeave();
                    if (red_enemy_num == 2) {
                        eventList.add(new Event(t, LogF.loss("red"), 10, 0));
                        isGameOver = true;
                    }
                }
            }
            break;
        case 20:
            for (int i = 1;i <= N;i++) {
                citys[i].hc += 10;
            }
            break;
        case 30:
            for (int i = 1;i <= N;i++) {
                citys[i].oneHeroGetHC();
            }
            break;
        case 40:
            for (int i = 1;i <= N;i++) {
                Hero* winner = citys[i].fight();
                if (winner != NULL) {
                    winners.push_back(make_pair(winner, &citys[i]));
                }

            }
            sort(winners.begin(), winners.end(), winnerCmp);
            for (pair<Hero*, City*> p : winners) {
                Hero* winner = p.first;
                City* city = p.second;
                if (city->owner != winner->side && city->lastWinSide == winner->side) {
                    city->owner = winner->side;
                    eventList.add(new Event(t, LogF.changeOwner(city->owner == red ? "red" : "blue", city->loc), 8, city->loc));
                }
                city->lastWinSide = winner->side;
                winner == city->redHero ? city->blueHeroLeave() : city->redHeroLeave();
                Home* winnerHome = winner->side == red ? Red : Blue;
                if (winnerHome->hc >= 8) {
                    winnerHome->setHc(winnerHome->hc - 8);
                    winner->hc += 8;
                }
            }
            for (pair<Hero*, City*> p : winners) {
                Hero* winner = p.first;
                City* city = p.second;
                if (winner->side == red) {
                    city->blueHero = NULL;
                }
                else {
                    city->redHero = NULL;
                }
                if (city->hc > 0) {
                    Home* winnerHome = winner->side == red ? Red : Blue;
                    winnerHome->setHc(winnerHome->hc + city->hc);
                    eventList.add(new Event(t, LogF.getElement(winner, city->hc), 7, city->loc));
                    city->hc = 0;
                }

            }
            // cout << "fight end" << winners.size() << endl;
            winners.clear();
            break;
        case 50:
            eventList.add(new Event(t, LogF.homeHc(Red->hc, "red"), 11, 0));
            eventList.add(new Event(t, LogF.homeHc(Blue->hc, "blue"), 11, 0));
            // isGameOver = true;
            break;
        default:
            break;
        }
        eventList.log();
        t += 10;
    }


}


int main() {
    cin >> C;
    for (int c = 0;c < C;c++) {
        cin >> M >> N >> T;
        for (int i = 0;i < MAX_HERO_NUM;i++)cin >> init_hc[i];
        for (int i = 0;i < MAX_HERO_NUM;i++)cin >> init_ak[i];
        cout << "Case:" << c + 1 << endl;
        Game(M, N, T, init_hc, init_ak);
    }

    return 0;
}

string alignTime(string x, int len, char c) {
    if (x.size() < len) {
        x = string(len - x.size(), c) + x;
    }
    return x;
}

string formatTime(int minute) {
    string hour = to_string(minute / 60);
    string minu = to_string(minute % 60);
    return alignTime(hour, 3, '0') + ":" + alignTime(minu, 2, '0');
}

string heroIdentifier(Hero* hero) {
    string side = hero->side == red ? "red" : "blue";
    return side + " " + hero->name + " " + to_string(hero->id);
}

string heroStatus(Hero* hero) {
    return "with " + to_string(hero->hc) + " elements and force " + to_string(hero->attack);
}

string LogFunc::born(Hero* hero) {
    return heroIdentifier(hero) + " born";
}

string LogFunc::move(Hero* hero, int cityId) {
    return heroIdentifier(hero) + " marched to city " + to_string(cityId) + " " + heroStatus(hero);
}

string LogFunc::attack(Hero* h1, Hero* h2, int cityId) {
    return heroIdentifier(h1) + " attacked " + heroIdentifier(h2) + " in city " + to_string(cityId) + " " + heroStatus(h1);
}

string LogFunc::reattack(Hero* h1, Hero* h2, int cityId) {
    return heroIdentifier(h1) + " fought back against " + heroIdentifier(h2) + " in city " + to_string(cityId);
}

string LogFunc::getElement(Hero* hero, int hc) {
    return heroIdentifier(hero) + " earned " + to_string(hc) + " elements for his headquarter";
}

string LogFunc::homeHc(int hc, string side) {
    return to_string(hc) + " elements in " + side + " headquarter";
}

string LogFunc::die(Hero* hero, int cityId) {
    return heroIdentifier(hero) + " was killed in city " + to_string(cityId);
}

string LogFunc::yell(Hero* hero, int cityId) {
    return heroIdentifier(hero) + " yelled in city " + to_string(cityId);
}

string LogFunc::changeOwner(string side, int cityId) {
    return side + " flag raised in city " + to_string(cityId);
}

string LogFunc::loss(string side) {
    return side + " headquarter was taken";
}

string LogFunc::reachEnd(Hero* h) {
    return heroIdentifier(h) + " reached " + (h->side == red ? "blue " : "red ") + "headquarter " + heroStatus(h);
}