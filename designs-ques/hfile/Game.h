#include <iostream>  // For std::cout and std::endl
#include "Player.h"  // This includes Player.h

class Game {
public:
    void start() {
        Player player("Alice");
        std::cout << "Player: " << player.getName() << std::endl;
    }
};
