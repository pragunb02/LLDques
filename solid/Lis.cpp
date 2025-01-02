#include <iostream>
#include <string>

// BAD EXAMPLE - Violating LSP
class CoffeeMachine {
protected:
    int temperature = 85; // in Celsius
public:
    virtual void brew() {
        std::cout << "Brewing at " << temperature << "°C\n";
    }
    
    virtual void setTemperature(int temp) {
        if (temp < 65) {
            throw std::runtime_error("Coffee must be hot!");
        }
        temperature = temp;
    }
};

// This violates LSP because it changes the expected behavior
class IcedCoffeeMachine : public CoffeeMachine {
public:
    void setTemperature(int temp) override {
        // Violates parent's expectations - accepts cold temperatures!
        temperature = temp;
    }
    
    void brew() override {
        std::cout << "Brewing at " << temperature << "°C (Cold!)\n";
    }
};

// GOOD EXAMPLE - Following LSP
class Beverage {
public:
    virtual void prepare() = 0;
    virtual ~Beverage() = default;
};

class HotCoffee : public Beverage {
    int temperature = 85;
public:
    void prepare() override {
        std::cout << "Preparing hot coffee at " << temperature << "°C\n";
    }
};

class IcedCoffee : public Beverage {
    int temperature = 4;
public:
    void prepare() override {
        std::cout << "Preparing iced coffee at " << temperature << "°C\n";
    }
};

// Function to serve drinks
void serveDrink(Beverage& drink) {
    std::cout << "Server: \"Here's your drink!\"\n";
    drink.prepare();
}

int main() {
    std::cout << "BAD EXAMPLE:\n";
    std::cout << "------------\n";
    
    CoffeeMachine regular;
    IcedCoffeeMachine iced;
    
    try {
        std::cout << "Regular coffee machine:\n";
        regular.setTemperature(85);
        regular.brew();
        
        std::cout << "\nIced coffee machine:\n";
        iced.setTemperature(4);  // This should fail but doesn't!
        iced.brew();
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    
    std::cout << "\nGOOD EXAMPLE:\n";
    std::cout << "-------------\n";
    
    HotCoffee hotCoffee;
    IcedCoffee icedCoffee;
    
    std::cout << "Serving hot coffee:\n";
    serveDrink(hotCoffee);
    
    std::cout << "\nServing iced coffee:\n";
    serveDrink(icedCoffee);
    
    return 0;
}