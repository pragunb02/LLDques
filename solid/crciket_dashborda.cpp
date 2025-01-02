#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <memory>
using namespace std;

// Class to represent a Player
class Player
{
    string name;
    int score;
    int ballsFaced;
    int fours;
    int sixes;

public:
    Player(string name) : name(move(name)), score(0), ballsFaced(0), fours(0), sixes(0) {}

    void updateScore(int runs) {
        score += runs;
        if (runs == 4) fours++;
        if (runs == 6) sixes++;
    }

    void ballFaced() { ballsFaced++; }

    string getName() const { return name; }

    void display(bool isOnStrike) const {
        cout << name << (isOnStrike ? "*" : " ") << " \t" << score << " \t" << fours << " \t" << sixes << " \t" << ballsFaced << endl;
    }

    int getScore() const { return score; }
};

// Class to represent a Team
class Team {
    string name;
    vector<shared_ptr<Player>> players;
    int totalScore;
    int totalWickets;
    int currentPlayerIndex;

public:
    Team(string name, vector<string> playerNames) : name(move(name)), totalScore(0), totalWickets(0), currentPlayerIndex(0) {
        for (const auto& playerName : playerNames) {
            players.emplace_back(make_shared<Player>(playerName));
        }
    }

    shared_ptr<Player> getOnStrike() { return players[currentPlayerIndex]; }

    shared_ptr<Player> getOffStrike() { return players[currentPlayerIndex + 1]; }

    void updateScore(int runs) {
        totalScore += runs;
        getOnStrike()->updateScore(runs);
    }

    void updateBallFaced() { getOnStrike()->ballFaced(); }

    void wicketLost() {
        totalWickets++;
        currentPlayerIndex++;
    }

    void rotateStrike() { swap(players[currentPlayerIndex], players[currentPlayerIndex + 1]); }

    void displayScorecard() {
        cout << "Scorecard for " << name << ":\n";
        cout << "Player Name \tScore \t4s \t6s \tBalls\n";
        for (const auto& player : players) {
            bool isOnStrike = (player == getOnStrike());
            player->display(isOnStrike);
        }
        cout << "Total: " << totalScore << "/" << totalWickets << endl;
    }

    int getTotalScore() const { return totalScore; }

    int getTotalWickets() const { return totalWickets; }
};

// Class to represent a Match
class Match {
    unique_ptr<Team> team1;
    unique_ptr<Team> team2;
    int overs;
    int currentOver;

public:
    Match(string team1Name, string team2Name, vector<string> team1Players, vector<string> team2Players, int overs) : overs(overs), currentOver(0) {
        team1 = make_unique<Team>(team1Name, team1Players);
        team2 = make_unique<Team>(team2Name, team2Players);
    }

    void playOver(Team& battingTeam) {
        cout << "Enter over details:\n";
        for (int i = 0; i < 6; ++i) {
            string ball;
            cin >> ball;
            if (ball == "W") {
                battingTeam.wicketLost();
            }
            else if (ball == "Wd" || ball == "Nb") {
                battingTeam.updateScore(1); // Extra
            }
            else {
                int runs = stoi(ball);
                battingTeam.updateScore(runs);
                battingTeam.updateBallFaced();
                if (runs % 2 == 1) {
                    battingTeam.rotateStrike();
                }
            }
        }
        battingTeam.rotateStrike(); // Change strike at the end of the over
    }

    void startMatch() {
        cout << "Team 1 Batting:\n";
        for (currentOver = 1; currentOver <= overs; ++currentOver) {
            playOver(*team1);
            team1->displayScorecard();
        }

        cout << "Team 2 Batting:\n";
        for (currentOver = 1; currentOver <= overs; ++currentOver) {
            playOver(*team2);
            team2->displayScorecard();
        }

        determineWinner();
    }

    void determineWinner() {
        int team1Score = team1->getTotalScore();
        int team2Score = team2->getTotalScore();

        cout << "Result: ";
        if (team1Score > team2Score) {
            cout << "Team 1 won by " << team1Score - team2Score << " runs\n";
        }
        else if (team1Score < team2Score) {
            cout << "Team 2 won by " << 10 - team2->getTotalWickets() << " wickets\n";
        }
        else {
            cout << "The match is a tie!\n";
        }
    }
};

int main() {
    int playersPerTeam, overs;
    cout << "Enter number of players per team: ";
    cin >> playersPerTeam;
    cout << "Enter number of overs: ";
    cin >> overs;

    vector<string> team1Players(playersPerTeam), team2Players(playersPerTeam);

    cout << "Enter batting order for Team 1:\n";
    for (auto& player : team1Players) cin >> player;

    cout << "Enter batting order for Team 2:\n";
    for (auto& player : team2Players) cin >> player;

    Match match("Team 1", "Team 2", team1Players, team2Players, overs);
    match.startMatch();

    return 0;
}
