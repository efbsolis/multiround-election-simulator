
/**
 * Voting simulator.
 *
 * @file voting.cpp
 * @author Eamon Bohan
 * @version 1 07/05/16
 */

#include <vector>
#include <iostream>
#include <limits>
#include <stdlib.h>
using namespace std;


/* Return random int 1-x */
double randomFromRange(unsigned int range) {
    static bool first = true;
    if(first) {
        srand(time(NULL));
        first = false;
    }
    unsigned int rando = 0 + rand() % (range - 0);
    return rando;
}


/* Uses a random number to determine if a vote was successful */
bool tryVote(double upperLimit) {
    double chance = randomFromRange(100);
    if (chance <= upperLimit) {
        return true;
    } else {
        return false;
    }
}


/* Check that user's text input is correct, assigns it to referenced variable */
void validateInput(bool &userBool) {
    string trueFalse;
    cin >> trueFalse;
    if ((trueFalse == "y") || (trueFalse == "Y")) {
        userBool = true;
    }
}

void validateInput(unsigned int &userNum, unsigned int rangeMin, unsigned int rangeMax) {
    for (;;) {
        if ((cin >> userNum) && ((userNum >= rangeMin) && (userNum <= rangeMax)) ) {
            break;
        } else { // squish bug where range is 1 - 2
            cout << "Please enter an integer between " << rangeMin << "-" << rangeMax << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


/* Collect inputs from user */
void getUserInput(unsigned int &numberCandidates, unsigned int &numberVoters,
    unsigned int &votesEach, bool &votesBoostChance) {
        //add power boost variable

        cout << "Hello!\nHow many candidates are competing? Range: 2-100\n";
        validateInput(numberCandidates, 2, 100);

        cout << "How many voters are there? Range: 2-" << (numberCandidates / 2) << endl;
        validateInput(numberVoters, 2, (numberCandidates / 2));

        cout << "How many votes does each voter get? Range: 1-" << (numberVoters / 2) << endl;
        validateInput(votesEach, 2, (numberCandidates / 4));

        cout << "Does vote number increase electability? (y = yes)\n";
        validateInput(votesBoostChance);
}


/* Removes from an unsigned int vector */
void removeFromVector(vector <unsigned int> &needsRemoval, unsigned int whichIndex) {
    if ((needsRemoval.size() <= 1) || (whichIndex > needsRemoval.size() )) {
        cout << "Vector too small" << endl;
    } else {
        for (unsigned int x = whichIndex; x<needsRemoval.size()-1; x++) {
            needsRemoval[x] = needsRemoval[x+1];
            if (needsRemoval.size() <= 1) { x = 2; }
        }
        if (needsRemoval.size() >= 1) { needsRemoval.pop_back(); }
    }
}


/* Returns array length=input where numbers 1-input are shuffled */
vector<unsigned int> makeShuffleArray(unsigned int arrayLength) {
    vector <unsigned int> orderedList;
    for (unsigned int x = 0; x < arrayLength; x++) {
        orderedList.push_back(x);
    }
    vector <unsigned int> toReturn;
    for (unsigned int x = 0; x < arrayLength; x++) {
        if (orderedList.size() <= 1) {
            x = arrayLength;
            toReturn.push_back(orderedList[0]);
        } else {
            unsigned int pluckedRandom = randomFromRange(orderedList.size());
            toReturn.push_back(orderedList[pluckedRandom]);
            removeFromVector(orderedList, pluckedRandom);
        }
    }
    return toReturn;
}


/* Candidate object that keeps votes and a switch */
class candidate {
    private:
        int ID;
        int votes;
        bool justVotedOn;
        //should it include an ID, or does the array do that?
    public:
        void initialise() {
            votes = 1; // if it were 0, they would get no votes ever
            justVotedOn = false;
        }

        void setID(int name) { ID = name; }

        int getID() { return ID; }

        void addVote() {
            votes = votes + 1;
            justVotedOn = true;
        }

        int getVotes() { return votes; }

        bool wasVoted() { return justVotedOn; }

        void resetVotedOn() { justVotedOn = false; }
};

/* Object that keeps this current round and can manipulate it */
class currentRoster {
    private:
        vector<candidate> roster;
    public:
        //FLESH OUT AND MOVE BELOW METHODS THAT MANIPULATE VECTORS
        //get number of candidates
        //sort array from highest to lowest
        //return whole array
};

/* Object containing record of race so far */
class roundsRecord {
    private:
        vector<vector<candidate> > rosterArray;
    public:
        void addNewRoud(vector<candidate> newRound) {
            rosterArray.push_back(newRound);
        }
        // return the whole array to read
};


/* Returns array containing currently running candidates */
vector<candidate> createRoster(unsigned int numCandidates) {
    candidate newCandidate;
    newCandidate.initialise();
    vector<candidate> currentRound;
    for (unsigned int x = 0; x < numCandidates; x++) {
        currentRound.push_back(newCandidate);
    }
    return currentRound;
}


/* Goes through roster adding votes until count is complete or array finishes */
void votingRound(vector<candidate> &nowRunning, double powerOfEachVote,
unsigned int &votedThisRound, unsigned int votesEach) {

    vector<unsigned int> randomArray = makeShuffleArray(nowRunning.size());

    for (unsigned int x = 0; x < nowRunning.size(); x++) {
        if (votedThisRound == votesEach) { x = nowRunning.size();
        } else if (votedThisRound > votesEach) {
                x = nowRunning.size();
                cout << "Bug; votes: " << votedThisRound << endl;
        }
        if (!nowRunning[x].wasVoted()) {
            //make a better probability calculating function
            double probability = powerOfEachVote * nowRunning[x].getVotes();
            if (tryVote(probability)) {
                nowRunning[randomArray[x]].addVote();
                cout << " " << randomArray[x];
                votedThisRound++;
            }
        }
    }
}


/* Reset votedOn BOOL in nowRunning array elements for next round */
void resetRoundVotes(vector<candidate> &nowRunning) {
    for (unsigned int x = 0; x < nowRunning.size(); x++) {
        nowRunning[x].resetVotedOn();
    }
}


/* Outputs a double that increments voting power of a candidate */
double calculateVotePower(double currentVotePower, unsigned int votesSoFar) {
    double newVotePower = 0.0;
    //what to do if 0 votesSoFar?
    newVotePower = 100 / votesSoFar;
    cout << "Current vote coefficient: " << newVotePower << endl;
    return newVotePower;
}

/* Prints chart from candidate array */
void printChart(vector<candidate> nowRunning, int roundsSoFar) {
    cout << "\nVotes in round #" << roundsSoFar << endl;
    for (unsigned int x = 0; x < nowRunning.size(); x++) { //PRINT OUT VOTES
        cout << x << "\t| ";
        for (int y = 0; y < nowRunning[x].getVotes() - 1; y++) {
            cout << "*";
        }
        cout << endl;
    }
}

struct node {
    candidate candidato;
    node *next;
};

bool isEmpty(node *head) {
    if (head == NULL) {
        return true;
    } else {
        return false;
    }
}

void createNextRound(vector<candidate> &nowRunning, double attritionRate) {

}

int main() {

    //inputs
    unsigned int numberCandidates = 16; // default 2
    unsigned int numberVoters = 8; // default 1
    unsigned int votesEach = 4; // default 1
    double attritionRate = 0.5; //percentage eliminated per round
    bool votesBoostChance = true; // default false
    getUserInput(numberCandidates, numberVoters, votesEach, votesBoostChance);
    vector<candidate> nowRunning = createRoster(numberCandidates);

    //tally
    unsigned int roundsSoFar = 0; // is this being used yet>
    unsigned int votesSoFar = 0; // roundsSoFar * votesEach
    double powerOfEachVote = 0.05; // 100 / votesSoFar (all election??)
    //create record vector<vector<candidate> > recordedRace;

    if (votesBoostChance) {
        //WHILE NUMBER OF CANDIDATES LEFT > 1 //add to recordedRace

        cout << "Round #" << roundsSoFar+1 << endl << endl;

        for (unsigned int x = 0; x < numberVoters; x++) {
            cout << "Voter #" << x+1 << " : ";

            unsigned int votesByCandidateSoFar = 0;
            bool allVotesIn = false;

            while (!allVotesIn) {
                votingRound(nowRunning, powerOfEachVote, votesByCandidateSoFar, votesEach);
                votesSoFar =+ votesByCandidateSoFar;

                if (votesByCandidateSoFar >= votesEach) {
                    allVotesIn = true;
                }
            }

            cout << " (" << votesByCandidateSoFar << ")\n";

            //calculate new voting power; reset voted button for candidates
            powerOfEachVote = 100.0 / votesSoFar;
            //CHECK to see if line above even makes sense

            resetRoundVotes(nowRunning);
            votesByCandidateSoFar = 0; //reset for next voter to use
        }

        roundsSoFar++;

        printChart(nowRunning, roundsSoFar);

        createNextRound(nowRunning, attritionRate);

    }

    return 0;
}
