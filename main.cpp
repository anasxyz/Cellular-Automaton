/**
 * TEAM R
 * 
 * Anas Saad (2510059)
 * Ahmed Youssef (2507690)
 * Ramin Hashemi (2508573)
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <ctime>

using namespace std;

/*
    in order to let the user change the length, I would have to
    remove the const keyword from LENGTH, doing so would result in
    problems with declaring some arrays in the code.

    i could declare these arrays dynamically with pointers and delete
    them after usage, but this would require some big changes in the code
    structure...

    so instead, i will use a second LENGTH that uses the first LENGTH
    but with an offset.
*/
const int LENGTH = 90;
int lengthOffset = 0;
int LENGTH2 = LENGTH + lengthOffset;

int genCount = 60; // number of generations
string wrapAround = "On"; // wrap around switch

int gen[LENGTH] = {0}; // main current gen array
int newGen[LENGTH] = {0}; // new - temporary gen array
int rule[8] = {0}; // rule array for binary rule representation
int ruleNumber = 30; // rule decimal number

int genSize = sizeof(gen) / sizeof(gen[0]); // size of main current gen

/*
    displays one generation at a time
*/
void displayGen(int* gen) {
    for (int i=0; i<LENGTH2; i++) {
        if (gen[i] == 0) cout << " ";
        if (gen[i] == 1) cout << "█";
    }
    cout << endl;
}

/*
    sets the middle cell in the first gen to it's alive state (1)
*/
void singleCellInit(int* gen) {
    gen[(LENGTH2/2)-1] = 1;
}

/*
    randomises first gen cells
*/
void randomCellInit(int* gen) {
    for (int i=0; i<LENGTH2; i++) {
        gen[i] = rand() % 2;
    }
}

/*
    get first gen input from user
*/
int getFirstGen(int* genArray) {
    string input;

    cout << "Enter first generation length (maximum " << LENGTH2 << "): ";
    cin >> input;

    if (input.length() > LENGTH2) {
        cout << "\nInput length is too long. Shortening to " << LENGTH2 << " characters." << endl;
        input = input.substr(0, LENGTH2); // cut off input at LENGTH length
    } else if (input.length() < LENGTH2) {
        // add zeros to end of the string until it reaches LENGTH length
        input.append(LENGTH2 - input.length(), '0');
    }

    for (int i = 0; i < LENGTH2; i++) {
        // was using "" but had no clue
        // that '' are for char comparisons but fixed now
        if (input[i] == '0') {
            genArray[i] = 0;
        } else if (input[i] == '1') {
            genArray[i] = 1;
        } else {
            cout << "Invalid input. Only 0s and 1s allowed." << endl;
            return 1;
        }
    }

    return 0;
}

/*
    fills in main generation array with user inputted array
*/
void userCellInit(int* gen1, int* gen2) {
    for(int i=0; i<LENGTH2; i++) {
        gen1[i] = gen2[i];
    }
}

/*
    changes length of CA
*/
void changeLength() {
    int newLength;
    cout << "Enter new length (maximum " << LENGTH << "): ";
    cin >> newLength;
    
    if (newLength <= 0 || newLength > LENGTH) {
        cout << "Invalid Length. Length must be between 1 and " << LENGTH << endl;
    } else {
        // change LENGTH2 and lengthOffset
        LENGTH2 = newLength;
        lengthOffset = newLength - LENGTH;
        cout << "Length changed to " << LENGTH2 << endl;
    }
}

/*
    changes number of generations
*/
void changeGenCount() {
    int newCount;
    cout << "Enter the new number of generations: ";
    cin >> newCount;

    if (newCount <= 0) {
        cout << "Invalid number of generations. Must be greater than 0." << endl;
    } else {
        genCount = newCount;
        cout << "Number of generations changed to " << genCount << endl;
    }
}

/*
    changes rule decimal number
*/
void changeRule() {
    int newRule;

    // validate rule number so it's between 0 and 255
    do {
        cout << "Enter rule number (0 - 255): ";
        cin >> newRule;

        if (newRule < 0 || newRule > 255) {
            cout << "Invalid rule number." << endl;
        }
    } while (newRule < 0 || newRule > 255);

    ruleNumber = newRule;
}

/*
    generates random rule decimal number
*/
void generateRandomRule() {
    srand(time(NULL));

    int randomRule = rand() % 256;

    ruleNumber = randomRule;

    cout << "\nGenerated Rule: " << ruleNumber << endl;
}

/*
    switches wrap around on or off
*/
void switchWrapAround() {
    if (wrapAround == "On") {
        wrapAround = "Off";
    } else if (wrapAround == "Off") {
        wrapAround = "On";
    }
}

/*
    asks for rule in decimal for then converts into binary
*/
void ruleDecToBin(int* rule, int ruleNumber) {
    unsigned long long number;

    number = ruleNumber;

    string binBits;

    // convert number to binary
    while (number > 0)
    {
        binBits.append(to_string(number % 2));
        number /= 2;
    }

    // reverse string 
    reverse(binBits.begin(), binBits.end());

    // add the leading zeros to complete 8 bit length
    while (binBits.length() < 8)
    {
        binBits.insert(0, "0");
    }
    
    // fill rule array with binary characters
    for (int i=0; i<8; i++) {
        rule[i] = binBits[i] - '0';
    }

}

/*
    applies whatever rule user chooses

    for example: 
    rule: 30
    rule[8] = {0, 0, 0, 1, 1, 1, 1, 0}
*/
void applyRule(int* gen1, int* gen2, int* rule) {
    int leftCell;
    int middleCell;
    int rightCell;
    
    for (int i = 0; i < LENGTH2; i++) {
        // determine neighbouring cells indices
        if (wrapAround == "On") {
            leftCell = gen1[(i + LENGTH2 - 1) % LENGTH2];
            middleCell = gen1[i];
            rightCell = gen1[(i + 1) % LENGTH2];
        } else if (wrapAround == "Off") {
            leftCell = gen1[i-1];
            middleCell = gen1[i];
            rightCell = gen1[i+1];
        }

        // apply chosen rule based on the neighbouring cells combinations
        if (leftCell == 1 && middleCell == 1 && rightCell == 1) {
            gen2[i] = rule[0];
        } else if (leftCell == 0 && middleCell == 1 && rightCell == 1) {
            gen2[i] = rule[1];
        } else if (leftCell == 1 && middleCell == 0 && rightCell == 1) {
            gen2[i] = rule[2];
        } else if (leftCell == 0 && middleCell == 0 && rightCell == 1) {
            gen2[i] = rule[3];
        } else if (leftCell == 1 && middleCell == 1 && rightCell == 0) {
            gen2[i] = rule[4];
        } else if (leftCell == 0 && middleCell == 1 && rightCell == 0) {
            gen2[i] = rule[5];
        } else if (leftCell == 1 && middleCell == 0 && rightCell == 0) {
            gen2[i] = rule[6];
        } else if (leftCell == 0 && middleCell == 0 && rightCell == 0) {
            gen2[i] = rule[7];
        }
    }

    // fill main gen with temporary gen's items
    for (int k=0; k<LENGTH2; k++) {
        gen1[k] = gen2[k];
    }
}

void saveTofile() {
    // CONTINUE HERE
}

/*
    displays cellular automata
*/
void displayCA() {
    for (int i=0; i<genCount; i++) {
        displayGen(gen); // display current gen
        applyRule(gen, newGen, rule); // apply chosen rule to current gen
    }

    // reset generation for consistent output
    memset(gen, 0, sizeof(gen));
}


int main() {
    // For the random number generator
    srand(time(0));

    int choice;
    do {
        cout << endl;
        cout << "                >>> Menu <<<\n" << endl;

        cout << "--- MAKE SURE TERMINAL IS IN FULLSCREEN ---\n" << endl;

        cout << "> Length (max 90): " << LENGTH2 << "\n";
        cout << "> Generations: " << genCount << "\n";
        cout << "> Rule: " << ruleNumber << "\n";
        cout << "> Wrap Around: " << wrapAround << "\n" << endl;

        cout << "1. Single Initial Cell\n";
        cout << "2. Random Initial Cells\n";
        cout << "3. Choose Initial Cells\n" << endl;

        cout << "4. Generate Random Rule\n";
        cout << "5. Change Rule\n";
        cout << "6. Change Length of First Generation\n";
        cout << "7. Change Number of Generations\n";
        cout << "8. Switch Wrap Around\n" << endl;

        cout << "9. Save last to file";
        cout << "10. Exit\n" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                singleCellInit(gen);
                ruleDecToBin(rule, ruleNumber);
                displayCA();
                break;
            case 2:
                randomCellInit(gen);
                ruleDecToBin(rule, ruleNumber);
                displayCA();
                break;
            case 3:
                int firstGenArray[LENGTH];

                if (getFirstGen(firstGenArray) == 0) {
                    userCellInit(gen, firstGenArray);
                    ruleDecToBin(rule, ruleNumber);
                    displayCA();
                }
                break;
            case 4:
                generateRandomRule();
                break;
            case 5:
                changeRule();
                break;
            case 6:
                changeLength();
                break;
            case 7:
                changeGenCount();
                break;
            case 8:
                switchWrapAround();
                break;
            case 9:
                // put saveToFile() function here when done please
                break;
            case 10:
                cout << "Exiting Program..." << endl;
                return 0;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    } while (choice != 10);

    return 0;
}