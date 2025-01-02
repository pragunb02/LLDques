#include<bits/stdc++.h>
using namespace std;

class Player {
private:
    const string name; // iMMUTABLE
    int currPos = 0;
public:
    Player(const string& name) : name(name) { // use const
        // this->name = name;
    }
    //not used
    // void setName(const string& name) {
    //     this->name = name;
    // }

    //   read-only
    string getName() const {
        return this->name;
    }

    int getPos() const {
        return this->currPos;
    }

    void setPos(const int Pos) {
        currPos = Pos;
    }

    // iska dekhata hu use
    ~Player() = default;
};




// Open for extension
class IObstacle {
public:
    virtual int getstart() const = 0;
    virtual int getend() const = 0;
    virtual string gettype() const = 0;
    // destructoer idk why or why not right now let see on requirement
    virtual ~IObstacle() = default; //  Virtual destructor for polymorphic deletion
};

class SnakeObstacle : public IObstacle {
private:
    int start;
    int end;
public:
    // private
    SnakeObstacle(int start, int end) {
        this->start = start;
        this->end = end;
    }
    int getstart() const override {
        return this->start;
    }
    int getend() const override {
        return this->end;
    }
    string gettype() const override {
        return "Snake";
    }

};

class LadderObstacle : public IObstacle {
private:
    int start;
    int end;
public:
    // private
    LadderObstacle(int start, int end) {
        this->start = start;
        this->end = end;
    }
    int getstart() const override {
        return this->start;
    }
    int getend() const override {
        return this->end;
    }
    string gettype() const override {
        return "Ladder";
    }

};


// class GameNetwork(){
// private:
//     GameConfig GConfig;
//     vector<Player>Players;
//     queue<Player>turn;
// public:
//     GameNetwork(GameConfig GConfig,vector<Player>Players,queue<Player>turn){
//         this->GConfig=GConfig;
//         this->Player=Player;
//         this->turn=turn;
//     }


// };

class GameNetwork {
private:
    map<int, int>snakes;
    map<int, int>ladder;
public:
    vector<IObstacle*>obstacles;
    int size;
    // size*size;
private:
    GameNetwork(vector<IObstacle*>obstacles, map<int, int>snakes, map<int, int>ladder, int size) {
        this->snakes = snakes;
        this->ladder = ladder;
        this->obstacles = obstacles;
        this->size = size;
    }
public:
    // GameNetwork(GameConfig GConfig,vector<Player>Players,queue<Player>turn){
    //     this->GConfig=GConfig;
    //     this->Player=Player;
    //     this->turn=turn;
    // }
    // innser nested class
    int getsize() const {
        return this->size;
    }
    class GameNetworkBuilder {
    public:
        map<int, int>snakes;
        map<int, int>ladder;
        vector<IObstacle*>obstacles;
        // The pointer can point to any derived class object (like SnakeObstacle or LadderObstacle) thanks to polymorphism
        int size;
        GameNetworkBuilder& setSize(int size) {
            this->size = size;
            return *this;
        }
        GameNetworkBuilder& addSnake(int start, int end) {
            snakes[start] = end;
            obstacles.push_back(new SnakeObstacle(start, end));
            return *this;
        }
        GameNetworkBuilder& addLadder(int start, int end) {
            ladder[start] = end;
            obstacles.push_back(new LadderObstacle(start, end));
            return *this;
        }
        GameNetwork build() {
            return GameNetwork(obstacles, snakes, ladder, size);
        }

    };
    // friend class GameNetworkBuilder;
    void printGameNetwork() const {
        cout << "Game Network (Size: " << size << "x" << size << "):\n";
        cout << "Snakes:\n";
        for (const auto& snake : snakes) {
            cout << "  Start: " << snake.first << " -> End: " << snake.second << "\n";
        }
        cout << "Ladders:\n";
        for (const auto& ladders : ladder) {
            cout << "  Start: " << ladders.first << " -> End: " << ladders.second << "\n";
        }
        cout << "Obstacles:" << endl;
        for (const auto& obs : obstacles) {
            if (obs->gettype() == "Ladder") {
                cout << "Ladder " << obs->getstart() << " " << obs->getend() << " " << endl;
            }
            else {
                cout << "Snake " << obs->getstart() << " " << obs->getend() << " " << endl;
            }
        }
    }

};

class Candidates {
    queue<Player*>Turns;
    vector<string>players;
public:
    Candidates() {

    }
    queue<Player*> prepareList() {
        cout << "How Many Players Playing: " << endl;
        int num; cin >> num;
        cout << "Write Their Name One-by-One With Their Turn Number:" << endl;
        string name;
        players.resize(num);
        int priority = 0;
        while (num--) {
            cin >> name;
            cin >> priority;
            players[priority - 1] = name;
        }
        prepareTurns();
        return Turns;
    }
    void prepareTurns() {
        for (const auto& player : players) {
            Player* obj = new Player(player);
            Turns.push(obj);
        }
    }


};
class IRollStrategy {
public:
    virtual int rollDice() const = 0;
    virtual ~IRollStrategy() = default;
};

class NormalStrategy : public IRollStrategy {
private:
    int side;
public:
    NormalStrategy(int side) : side(side) {

    }
    int rollDice() const override { /// const overide imp
        return (rand() % side) + 1;
    }
};

class PlayGround {
private:
    GameNetwork Game;
    queue<Player*>turn;
    IRollStrategy* dice;
public:
    PlayGround(GameNetwork Game, queue<Player*>turn, IRollStrategy* dice) :Game(Game) {
        // this->Game=Game;
        this->dice = dice;
        this->turn = turn;
    }
    void startGame() {
        int winPos = 0;
        while (turn.size()) {
            auto topPlayer = turn.front();
            turn.pop();
            int diceNumer = dice->rollDice();
            int currPos = topPlayer->getPos();
            int newPos = move(diceNumer, currPos);
            if (newPos > Game.getsize()) {
                newPos = currPos;
            }
            cout << "Dice Rolled for Player:" << topPlayer->getName() << " " << "Move From" << currPos << "->" << newPos << endl;
            for (int i = 0;i < 10000000;i++) {

            }
            topPlayer->setPos(newPos);
            if (newPos == Game.getsize()) {
                cout << "Hurrah " << topPlayer->getName() << " finished game at " << winPos++ << endl;
            }
            else {
                turn.push(topPlayer);
            }

        }
    }
    int move(int dice, int pos) {
        // check if any obstace is starting at pos
        int newPos = dice + pos;
        vector<IObstacle*>obstacles = Game.obstacles;
        for (const auto& obs : obstacles) {
            if (newPos == obs->getstart() and obs->gettype() == "Snake") {
                cout << "Oops Snake bit you" << endl;
                newPos = obs->getend();
                break;
            }
            if (newPos == obs->getstart() and obs->gettype() == "Ladder") {
                cout << "Wow Ladder Again for you" << endl;
                newPos = obs->getend();
                break;
            }
        }
        return newPos;
    }

};


int main() {
    /*
    Player-Class
    Dice-Class (Strategy)->run time polymorphism Interface
    GameCongif - Class (Builder)->

    */
    // Player* p1 = new Player();  // why pointer
    // cout << (p1->getName()) << endl;
    // // p1->setName("Alice");
    // cout << (p1->getName()) << endl;
    // Player* p2 = new Player();
    // cout << (p2->getName()) << endl;
    // // p2->setName("Bob");
    // cout << (p2->getName()) << endl;
    // delete p1;
    // delete p2;

    GameNetwork Game = GameNetwork::GameNetworkBuilder().setSize(100).addLadder(0, 100).addLadder(2, 100).addSnake(10, 2).addLadder(9, 100).build();
    // // immutale game object and rebuild
    // auto GameBuilderInstance=GameNetwork::GameNetworkBuilder();
    // GameBuilderInstance.addSnake(20,10);
    // Game=GameBuilderInstance.build();

    Game.printGameNetwork();
    Candidates list;
    queue<Player*>turn = list.prepareList();
    IRollStrategy* dice = new NormalStrategy(6);
    PlayGround Play(Game, turn, dice);
    Play.startGame();

}

//  smart pointer
// pointer deletion