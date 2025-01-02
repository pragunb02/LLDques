#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <memory>
#include <algorithm>

// Forward declarations
class Board;
class Player;
class Game;

// Singleton Board Class
class Board {
private:
    static std::shared_ptr<Board> instance;
    int size;
    std::map<int, int> snakes;
    std::map<int, int> ladders;
    
    Board(int boardSize) {
        size = boardSize;
    }

public:
    static std::shared_ptr<Board> getInstance(int size) {
        if (instance == nullptr) {
            instance = std::shared_ptr<Board>(new Board(size));
        }
        return instance;
    }

    void addSnake(int start, int end) {
        snakes[start] = end;
    }
    
    void addLadder(int start, int end) {
        ladders[start] = end;
    }
    
    int getSize() {
        return size;
    }
    
    int getNextPosition(int currentPos) {
        if (snakes.find(currentPos) != snakes.end()) {
            std::cout << "Snake bite! Moving from " << currentPos << " to " << snakes[currentPos] << std::endl;
            return snakes[currentPos];
        }
        if (ladders.find(currentPos) != ladders.end()) {
            std::cout << "Ladder found! Moving from " << currentPos << " to " << ladders[currentPos] << std::endl;
            return ladders[currentPos];
        }
        return currentPos;
    }
};

std::shared_ptr<Board> Board::instance = nullptr;

// Observer Interface
class Observer {
public:
    virtual void update(std::string message) = 0;
    virtual ~Observer() = default;
};

// Player Class implementing Observer
class Player : public Observer {
private:
    std::string name;
    int position;

public:
    Player(std::string playerName) {
        name = playerName;
        position = 0;
    }
    
    void update(std::string message) override {
        std::cout << name << ": " << message << std::endl;
    }
    
    std::string getName() {
        return name;
    }
    
    int getPosition() {
        return position;
    }
    
    void setPosition(int newPosition) {
        position = newPosition;
    }
};

// Dice Strategy Interface
class DiceStrategy {
public:
    virtual int rollDice() = 0;
    virtual ~DiceStrategy() = default;
};

// Normal Dice Implementation
class NormalDice : public DiceStrategy {
private:
    std::random_device rd;
    std::mt19937 gen;

public:
    NormalDice() {
        gen = std::mt19937(rd());
    }
    
    int rollDice() override {
        std::uniform_int_distribution<> dis(1, 6);
        return dis(gen);
    }
};

// Command Interface
class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

// Move Command Implementation
class MoveCommand : public Command {
private:
    std::shared_ptr<Player> player;
    int steps;
    std::shared_ptr<Board> board;

public:
    MoveCommand(std::shared_ptr<Player> p, int s, std::shared_ptr<Board> b) {
        player = p;
        steps = s;
        board = b;
    }
    
    void execute() override {
        int currentPos = player->getPosition();
        int newPosition = currentPos + steps;
        
        if (newPosition > board->getSize()) {
            newPosition = currentPos;
        } else {
            newPosition = board->getNextPosition(newPosition);
        }
        
        player->setPosition(newPosition);
    }
};

// Game Class
class Game {
private:
    std::vector<std::shared_ptr<Player>> players;
    int currentPlayerIndex;
    std::shared_ptr<Board> board;
    std::shared_ptr<DiceStrategy> dice;

public:
    Game(std::shared_ptr<Board> b, std::shared_ptr<DiceStrategy> d) {
        board = b;
        dice = d;
        currentPlayerIndex = 0;
    }
    
    void addPlayer(std::shared_ptr<Player> player) {
        players.push_back(player);
    }
    
    void notifyPlayers(std::string message) {
        for (auto player : players) {
            player->update(message);
        }
    }
    
    std::shared_ptr<Player> getCurrentPlayer() {
        return players[currentPlayerIndex];
    }
    
    void nextTurn() {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    }
    
    void playGame() {
        while (true) {
            std::shared_ptr<Player> currentPlayer = getCurrentPlayer();
            int diceRoll = dice->rollDice();
            
            std::cout << "\n" << currentPlayer->getName() << "'s turn. Dice roll: " << diceRoll << std::endl;
            
            auto moveCmd = std::make_shared<MoveCommand>(currentPlayer, diceRoll, board);
            moveCmd->execute();
            
            std::string message = "Moved to position " + std::to_string(currentPlayer->getPosition());
            notifyPlayers(message);
            
            if (currentPlayer->getPosition() == board->getSize()) {
                std::cout << "\n🎉 " << currentPlayer->getName() << " wins! 🎉" << std::endl;
                break;
            }
            
            nextTurn();
            
            // Add small delay to make the game more readable
            std::cout << "Press Enter to continue...";
            std::cin.get();
        }
    }
};

int main() {
    // Initialize board with size 100
    auto board = Board::getInstance(100);
    
    // Add snakes
    board->addSnake(16, 6);
    board->addSnake(47, 26);
    board->addSnake(49, 11);
    board->addSnake(56, 53);
    board->addSnake(62, 19);
    board->addSnake(87, 24);
    board->addSnake(93, 73);
    board->addSnake(95, 75);
    board->addSnake(98, 78);
    
    // Add ladders
    board->addLadder(1, 38);
    board->addLadder(4, 14);
    board->addLadder(9, 31);
    board->addLadder(21, 42);
    board->addLadder(28, 84);
    board->addLadder(36, 44);
    board->addLadder(51, 67);
    board->addLadder(71, 91);
    board->addLadder(80, 100);
    
    // Create game with normal dice
    auto dice = std::make_shared<NormalDice>();
    auto game = std::make_shared<Game>(board, dice);
    
    // Add players
    auto player1 = std::make_shared<Player>("Player 1");
    auto player2 = std::make_shared<Player>("Player 2");
    game->addPlayer(player1);
    game->addPlayer(player2);
    
    // Start the game
    std::cout << "🎲 Starting Snake and Ladder Game 🎲\n" << std::endl;
    game->playGame();
    
    return 0;
}