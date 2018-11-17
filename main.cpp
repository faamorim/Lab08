#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;
// Global variable that sets if places should be printed by default or not
bool globalPrintPlaces = false;
// weird mess of forward declarations...
bool load(const string& fileName, map<string, set<string>>& peoplePlaces, set<string>& allPlaces);
bool visited(const pair<string, set<string>>& personPlaces, const string& place);
int visited(const pair<string, set<string>>& personPlaces, const set<string>& places);
void removePersonThat(map<string, set<string>>& peoplePlaces, bool visit, const string& place);
void removePersonThat(map<string, set<string>>& peoplePlaces, bool visit, const set<string>& places, bool printRemovals = false, bool printPlaces = globalPrintPlaces);
void print(const map<string, set<string>>& peoplePlaces, bool printPlaces = globalPrintPlaces);
void print(const pair<string, set<string>>& personPlaces, bool printPlaces = globalPrintPlaces);

// Main metho
int main() {
    globalPrintPlaces = false;
    map<string, set<string>> peoplePlaces;
    set<string> allPlaces;
    load("peoplePlaces.txt", peoplePlaces, allPlaces);
    cout << "Everybody in the city: " << endl;
    print(peoplePlaces);

    cout << endl << "Visited both Krusty-Burger and Tavern: " << endl;
    map<string, set<string>> B{peoplePlaces};
    removePersonThat(B, false, "Krusty-Burger");
    removePersonThat(B, false, "Tavern");
    print(B);

    cout << endl << "Did NOT visit Krusty-Burger and Home: " << endl;
    map<string, set<string>> C{peoplePlaces};
    removePersonThat(C, true, "Krusty-Burger");
    removePersonThat(C, true, "Home");
    print(C);

    cout << endl << "Visited Krusty-Burger and School but did NOT visit Tavern and Home: " << endl;
    map<string, set<string>> D{peoplePlaces};
    removePersonThat(D, false, "Krusty-Burger");
    removePersonThat(D, false, "School");
    removePersonThat(D, true, "Tavern");
    removePersonThat(D, true, "Home");
    print(D);

    cout << endl << "Removing visited all locations: " << endl;
    map<string, set<string>> E{peoplePlaces};
    removePersonThat(E, true, allPlaces, true);
    cout << endl << "Everybody in the city after removing people who visited all location: " << endl;
    print(E);

    return 0;
}

// Prints everyone from the given people map, and prints their visited places if bool is set
void print(const map<string, set<string>>& peoplePlaces, bool printPlaces){
    for_each(peoplePlaces.begin(),peoplePlaces.end(), [&printPlaces](pair<string, set<string>> personPlaces){
        print(personPlaces, printPlaces);});
}

// Prints the given person, and print their visited places if bool is set
void print(const pair<string, set<string>>& personPlaces, bool printPlaces){
    cout << personPlaces.first;
    if(printPlaces){
        cout << " - ";
        for_each(personPlaces.second.begin(), personPlaces.second.end(), [](string s){cout << s << " ";});
    }
    cout << endl;
}

// Loads the given filename saving people and their visited places into a map, and all the places into a set
bool load(const string& fileName, map<string, set<string>>& peoplePlaces, set<string>& allPlaces) {
    ifstream file = ifstream(fileName, ios_base::in);
    if(!file.is_open()) {
        cerr << "Unable to open file to load.";
        return false;
    }
    else {
        string line;
        while(getline(file, line)){
            set<string> places;
            stringstream lineStream;
            string name;
            lineStream << line;
            lineStream >> name;
            string place;
            while(lineStream >> place) {
                places.insert(place);
                allPlaces.insert(place);
            }
            peoplePlaces.insert(pair<string, set<string>>(name, places));
        }
        file.close();
        if(file.is_open()) {
            cerr << "Unable to close file after load.";
            return false;
        }
    }
    return true;
}

// Removes from the given people map everyone that visited or not (determined as second argument) the given place
void removePersonThat(map<string, set<string>>& peoplePlaces, bool visit, const string& place){
    for(auto itr = peoplePlaces.begin(); itr != peoplePlaces.end();){
        if(visited(*itr, place) == visit){
            itr = peoplePlaces.erase(itr);
        }
        else {
            ++itr;
        }
    }
}

// Removes from the given people map everyone that visited or not (determined as second argument)
// the given set of places (boolean flags to print can be set)
void removePersonThat(map<string, set<string>>& peoplePlaces, bool visit, const set<string>& places, bool printRemovals, bool printPlaces){
    for(auto itr = peoplePlaces.begin(); itr != peoplePlaces.end();){
        if(visited(*itr, places) == (visit ? places.size() : 0)){
            if(printRemovals) {
                cout << " -";
                print(*itr, printPlaces);
            }
            itr = peoplePlaces.erase(itr);
        }
        else {
            ++itr;
        }
    }
}

// Returns true if the person visited the given place
bool visited(const pair<string, set<string>>& personPlaces, const string& place){
    return personPlaces.second.find(place) != personPlaces.second.end();
}

// Returns the number of places in a set that a person visited
int visited(const pair<string, set<string>>& personPlaces, const set<string>& places){
    int visitCount = 0;
    for_each(places.begin(), places.end(), [personPlaces, &visitCount](const string& place){
        if(visited(personPlaces, place))visitCount++;});
    return visitCount;
}