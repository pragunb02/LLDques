// State Design Pattern

// IdleState -> AcceptingCoinState -> ProductSelectionState -> DispensingState -> IdleState

// IdleState has pressInsertCoinButton change state to AcceptingCoinState

// AcceptingCoinState has insertCoin() then pressSelectProductButton() to change state to ProductSelectionState

// ProductSelectionState has selectProduct() to change state to DispensingState

// DispensingState has collectProduct() to change state to IdleState

// AcceptingCoinState and ProductSelectionState has cancelRequest() that return coins and changes state to IdleState

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class VendingMachine;

class Coin {
protected:
    int value;
public:
    int getValue() const { return value; }
};

class OneRupee : public Coin {
public:
    OneRupee() { value = 1; }
};

class FiveRupees : public Coin {
public:
    FiveRupees() { value = 5; }
};

class TenRupees : public Coin {
public:
    TenRupees() { value = 10; }
};

class Item {
public:
    string type;
    int price;

    int getPrice() const { return price; }
};

class Coke : public Item {
public:
    Coke() {
        type = "Coke";
        price = 5;
    }
};

class Soda : public Item {
public:
    Soda() {
        type = "Soda";
        price = 10;
    }
};

class State {
public:
    virtual void pressInsertCoinButton(VendingMachine *machine) {
        cout << "Method not supported" << endl;
    }
    virtual void insertCoin(VendingMachine *machine, Coin *coin) {
        cout << "Method not supported" << endl;
    }
    virtual void pressSelectProductButton(VendingMachine *machine) {
        cout << "Method not supported" << endl;
    }
    virtual void selectProduct(VendingMachine *machine, int code) {
        cout << "Method not supported" << endl;
    }
    virtual Item* collectProduct(VendingMachine *machine, int code) {
        cout << "Method not supported" << endl;
        return nullptr;
    }
    virtual vector<Coin*> cancelRequest(VendingMachine *machine) {
        cout << "Method not supported" << endl;
        return {};
    }
    virtual int getChange(int returnChangeMoney) {
        cout << "Method not supported" << endl;
        return 0;
    }
    virtual ~State() = default;
};

class VendingMachine {
private:
    vector<Coin*> coinList;
    State *vendingMachineState;
    map<int, Item*> inventory;

public:
    VendingMachine();
    State* getVendingMachineState();
    void setVendingMachineState(State *state);
    Item* getItemFromInventory(int code);
    vector<Coin*> getCoinList();
    void addCoinToCoinList(Coin *coin);
    void resetCoinList();
};

class IdleState : public State {
public:
    IdleState() {
        cout << "Vending Machine is in IdleState" << endl;
    }
    void pressInsertCoinButton(VendingMachine *machine) override;
};

class AcceptingCoinState : public State {
public:
    AcceptingCoinState() {
        cout << "Vending Machine is in AcceptingCoinState" << endl;
    }
    void insertCoin(VendingMachine *machine, Coin *coin) override;
    void pressSelectProductButton(VendingMachine *machine) override;
    vector<Coin*> cancelRequest(VendingMachine *machine) override;
};

class ProductSelectionState : public State {
public:
    ProductSelectionState() {
        cout << "Vending Machine is in ProductSelectionState" << endl;
    }
    void selectProduct(VendingMachine *machine, int code) override;
    int getChange(int amount) override;
    vector<Coin*> cancelRequest(VendingMachine *machine) override;
};

class DispensingState : public State {
public:
    DispensingState(VendingMachine *machine, int code);
    Item* collectProduct(VendingMachine *machine, int code) override;
};

// Implementation of VendingMachine methods
VendingMachine::VendingMachine() {
    coinList.clear();
    vendingMachineState = new IdleState();

    // Hard coding inventory for simplicity
    for (int i = 0; i < 6; i++) {
        if (i < 3) {
            inventory[i] = new Coke();
        } else {
            inventory[i] = new Soda();
        }
    }
}

State* VendingMachine::getVendingMachineState() {
    return vendingMachineState;
}

void VendingMachine::setVendingMachineState(State *state) {
    delete vendingMachineState;
    vendingMachineState = state;
}

Item* VendingMachine::getItemFromInventory(int code) {
    return inventory[code];
}

vector<Coin*> VendingMachine::getCoinList() {
    return coinList;
}

void VendingMachine::addCoinToCoinList(Coin *coin) {
    coinList.push_back(coin);
}

void VendingMachine::resetCoinList() {
    for (auto coin : coinList) {
        delete coin;
    }
    coinList.clear();
}

// Implementation of State methods
void IdleState::pressInsertCoinButton(VendingMachine *machine) {
    machine->setVendingMachineState(new AcceptingCoinState());
}

void AcceptingCoinState::insertCoin(VendingMachine *machine, Coin *coin) {
    cout << "Accepting Coin " << coin->getValue() << endl;
    machine->addCoinToCoinList(coin);
}

void AcceptingCoinState::pressSelectProductButton(VendingMachine *machine) {
    machine->setVendingMachineState(new ProductSelectionState());
}

vector<Coin*> AcceptingCoinState::cancelRequest(VendingMachine *machine) {
    cout << "Cancel Request and Returning the money" << endl;
    vector<Coin*> returnCoins = machine->getCoinList();
    machine->setVendingMachineState(new IdleState());
    machine->resetCoinList();
    return returnCoins;
}

void ProductSelectionState::selectProduct(VendingMachine *machine, int code) {
    Item *item = machine->getItemFromInventory(code);
    int amountPaidByCustomer = 0;
    for (auto& coin : machine->getCoinList()) {
        amountPaidByCustomer += coin->getValue();
    }

    if (amountPaidByCustomer < item->getPrice()) {
        cancelRequest(machine);
        cout << "Insufficient Money" << endl;
    } else {
        if (amountPaidByCustomer > item->getPrice()) {
            int changeReturned = getChange(amountPaidByCustomer - item->getPrice());
            cout << "Amount of extra change returned is: " << changeReturned << endl;
        }
        machine->setVendingMachineState(new DispensingState(machine, code));
    }
}

int ProductSelectionState::getChange(int amount) {
    return amount;
}

vector<Coin*> ProductSelectionState::cancelRequest(VendingMachine *machine) {
    cout << "Cancel Request and Returning the money" << endl;
    vector<Coin*> returnCoins = machine->getCoinList();
    machine->setVendingMachineState(new IdleState());
    machine->resetCoinList();
    return returnCoins;
}

DispensingState::DispensingState(VendingMachine *machine, int code) {
    cout << "Vending Machine is in DispensingState" << endl;
    collectProduct(machine, code);
}

Item* DispensingState::collectProduct(VendingMachine *machine, int code) {
    Item *item = machine->getItemFromInventory(code);
    cout << "Collecting Item: " << item->type << endl;
    machine->setVendingMachineState(new IdleState());
    return item;
}

// Main function for testing
int main() {
    VendingMachine *machine = new VendingMachine();
    
    machine->getVendingMachineState()->pressInsertCoinButton(machine);
    machine->getVendingMachineState()->insertCoin(machine, new FiveRupees());
    machine->getVendingMachineState()->insertCoin(machine, new FiveRupees());
    machine->getVendingMachineState()->pressSelectProductButton(machine);
    machine->getVendingMachineState()->selectProduct(machine, 0);  // Selecting Coke

    delete machine;
    return 0;
}