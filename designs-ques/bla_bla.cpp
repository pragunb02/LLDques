#include<bits/stdc++.h>
using namespace std;

// USER - REGISTERY BUILDER PATTERN USE CLASS
class User {
private:
    string name;
    int liceneNum = -1;
    User(string name, int liceneNum) {
        this->name = name;
        this->liceneNum = liceneNum;
    }
public:
    string getName() const {
        return this->name;
    }
    class Userbuilder {
    private:
        string name;
        int liceneNum = -1;
    public:
        Userbuilder& addName(string name) {
            this->name = name;
            return *this;
            // returring references
        }
        Userbuilder& addLno(int liceneNum) {
            this->liceneNum = liceneNum;
            return *this;
        }
        User build() {
            return User(name, liceneNum);
        }
    };
};

// Vechile Interface for PATTERN IDK?????????
class IVechile {
public:
    virtual string getType() const = 0;
    virtual int getVehicleNumber() const = 0;
}

class TwoSeater : public IVechile {
private:
    int vehicleNumber;
public:
    TwoSeater(int vehicleNumber) {
        this->vehicleNumber = vehicleNumber;
    }
    string getType() const {
        return "Two";
    }
    int getVehicleNumber() const {
        return vehicleNumber
    }

}

class FourSeater : public IVechile {
private:
    int vehicleNumber;
public:
    FourSeater(int vehicleNumber) {
        this->vehicleNumber = vehicleNumber;
    }
    string getType() const {
        return "Four";
    }
    int getVehicleNumber() const {
        return vehicleNumber
    }
}

// Ride Class 
class Ride {
priavte:
    string startCity;
    string endCity;
    string owner;
public:
    Ride(string& startCity, string& endCity, string& owner) {
        this->startCity = startCity;
        this->endCity = endCity;
        this->owner = owner;
    }
}

// Matching Strategy Interface for matching the rides
class MatchingStrategy{
    virtual 
};

class NormalMatching : public MatchingStrategy{

}

class UniqueMatching : public MatchingStrategy{
    
}

// // manages our rides // RideController
// class RideManager{
//     vector<Rides>ridesAvaliable;
//     void createRide(int startCity,int endCity,int ){
//          ridesAvaliable.push_back(Rides(startCity,endCity));
//     }
// }

class UserManagerService{
private:
  map<int,User>regUser;
public:
    UserManagerService.add(string &name,int &Lno){
    User user = User::Userbuilder().addName().addLno(Lno).build();
    for()
    regUser[id++]=user;
   }
};

class carPoolServiceClientLayer{
    UserManagerService user;
    User user;
    void addVechile(){
        for(const auto&user:registeredUser){
            if(){
                VechileAvaliable.push();
            }
        }
    }
    void regsiteredUser(string &name,int Lno=-1){
        user.add(name,Lno);
    }
    void createRide(){
        // pick first vechile if vichjel is their by user

        Rides.push();
    }
    void findRide(){
        // user is registred
        // itertate ride fina and ride match s==staRT e==end
    }
}

int main() {
    Car Pooling 
    User user = User::Userbuilder().addName("Pragun").addLno(123).build();
    cout << (user.getName()) << endl;
}



// g++ bla_bla.cpp -o bla_bla && ./bla_bla