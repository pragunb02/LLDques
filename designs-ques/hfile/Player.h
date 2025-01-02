#pragma once  // Prevents multiple inclusions
#include <string>

class Player {
private:
    std::string name;
public:
    Player(const std::string& name) : name(name) {}
    std::string getName() const {
        return name;
    }
};
