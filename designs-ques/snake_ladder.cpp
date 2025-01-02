#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <queue>
#include <random>
#include <ctime>

// Singleton for Dice
class Dice {
private:
    int sides;
    Dice(int sides) : sides(sides) {}
    static std::shared_ptr<Dice> instance;

public:
    static std::shared_ptr<Dice> getInstance(int sides = 6) {
        if (!instance) {
            instance = std::shared_ptr<Dice>(new Dice(sides));
        }
        return instance;
    }

    int roll() {
        static std::mt19937 rng(std::time(nullptr));
        std::uniform_int_distribution<int> dist(1, sides);
        return dist(rng);
    }
};
std::shared_ptr<Dice> Dice::instance = nullptr;

// Strategy Pattern for Player Movement
class MovementStrategy {
public:
    virtual int move(int currentPosition, int roll, const std::unordered_map<int, int>& snakes, const std::unordered_map<int, int>& ladders) = 0;
    virtual ~MovementStrategy() = default;
};

class NormalMovement : public MovementStrategy {
public:
    int move(int currentPosition, int roll, const std::unordered_map<int, int>& snakes, const std::unordered_map<int, int>& ladders) override {
        int newPosition = currentPosition + roll;
        if (snakes.count(newPosition)) {
            return snakes.at(newPosition);
        }
        if (ladders.count(newPosition)) {
            return ladders.at(newPosition);
        }
        return newPosition;
    }
};

// Builder Pattern for Game Configuration
class GameConfig {
public:
    int boardSize;
    std::unordered_map<int, int> snakes;
    std::unordered_map<int, int> ladders;

    GameConfig(int size, const std::unordered_map<int, int>& snakes, const std::unordered_map<int, int>& ladders)
        : boardSize(size), snakes(snakes), ladders(ladders) {}
};

class GameConfigBuilder {
private:
    int boardSize;
    std::unordered_map<int, int> snakes;
    std::unordered_map<int, int> ladders;

public:
    GameConfigBuilder& setBoardSize(int size) {
        boardSize = size;
        return *this;
    }

    GameConfigBuilder& addSnake(int head, int tail) {
        snakes[head] = tail;
        return *this;
    }

    GameConfigBuilder& addLadder(int start, int end) {
        ladders[start] = end;
        return *this;
    }

    GameConfig build() {
        return GameConfig(boardSize, snakes, ladders);
    }
};

// Player Class
class Player {
private:
    std::string name;
    int position;

public:
    Player(const std::string& name) : name(name), position(0) {}

    const std::string& getName() const { return name; }

    int getPosition() const { return position; }

    void setPosition(int newPosition) { position = newPosition; }
};

// Game Class
class Game {
private:
    GameConfig config;
    std::vector<Player> players;
    std::queue<Player*> turnQueue;
    std::shared_ptr<MovementStrategy> movementStrategy;

public:
    Game(const GameConfig& config, const std::vector<std::string>& playerNames, std::shared_ptr<MovementStrategy> strategy)
        : config(config), movementStrategy(strategy) {
        for (const auto& name : playerNames) {
            players.emplace_back(name);
        }
        for (auto& player : players) {
            turnQueue.push(&player);
        }
    }

    void play() {
        auto dice = Dice::getInstance();

        while (true) {
            Player* currentPlayer = turnQueue.front();
            turnQueue.pop();

            int roll = dice->roll();
            int newPosition = movementStrategy->move(currentPlayer->getPosition(), roll, config.snakes, config.ladders);

            if (newPosition > config.boardSize) {
                newPosition = currentPlayer->getPosition();
            }

            currentPlayer->setPosition(newPosition);

            std::cout << currentPlayer->getName() << " rolled a " << roll << " and moved to position " << newPosition << "\n";

            if (newPosition == config.boardSize) {
                std::cout << currentPlayer->getName() << " wins the game!\n";
                break;
            }

            turnQueue.push(currentPlayer);
        }
    }
};

// Main Function
int main() {
    GameConfigBuilder builder;
    auto config = builder.setBoardSize(100)
                    .addSnake(99, 10)
                    .addSnake(95, 50)
                    .addLadder(2, 25)
                    .addLadder(40, 90)
                    .build();

    std::vector<std::string> playerNames = {"Alice", "Bob"};
    auto strategy = std::make_shared<NormalMovement>();
    Game game(config, playerNames, strategy);
    game.play();

    return 0;
}
