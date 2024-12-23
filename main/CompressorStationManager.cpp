#include "CompressorStationManager.h"
#include "CompressorStation.h"
#include <iostream>
#include <string>

extern std::map<int, CompressorStation> stations;
using namespace std;

string CompressorStationManager::saveCS(const CompressorStation& stations) const {
    return stations.full_name + ";" + to_string(stations.totalShops) + ";" +
        to_string(stations.operationalShops) + ";" +
        to_string(stations.unusedOperationalShops) + ";" +
        to_string(stations.efficiency);
}

void CompressorStationManager::toggleShop(CompressorStation& stations, int id) {
    if (stations.full_name.empty() && stations.totalShops <= 0 && stations.operationalShops <= 0) {
        cout << "There is no station. Start/Stop of the workshop is impossible." << endl;
    }
    else {
        int choice;
        cout << "----------------------------" << endl;
        cout << " 1.Launch a new workshop    " << endl;
        cout << " 2.Stop the workshop        " << endl;
        cout << "----------------------------" << endl;
        choice = inputIntInRange("Choose an action: ", 1, 3);
        cin.ignore();
        log("The user selected an action: " + choice);

        switch (choice) {
        case 1:
            log("Launch a new workshop");
            if (stations.operationalShops < stations.totalShops) {
                stations.operationalShops++;
                stations.unusedOperationalShops = ((stations.totalShops - stations.operationalShops) / static_cast<double>(stations.totalShops)) * 100;
                cout << "We launched a new workshop. Now in work: " << stations.operationalShops << endl;
            }
            else {
                cout << "All workshops have already been launched" << endl;
            }
            break;
        case 2:
            log("Workshop stop");
            if (stations.operationalShops > 0) {
                stations.operationalShops--;
                stations.unusedOperationalShops = ((stations.totalShops - stations.operationalShops) / static_cast<double>(stations.totalShops)) * 100;
                cout << "The workshop was stopped. Now in work: " << stations.operationalShops << endl;
            }
            else {
                cout << "There are no working workshops to stop." << endl;
            }
            break;
        default:
            cout << "Try again." << endl;

            break;
        }

    }
}

void CompressorStationManager::addStation() {
    CompressorStation station;

    while (stations.find(nextId) != stations.end()) {
        nextId++;
    }

    station.read();
    stations[nextId] = station;

    cout << "New station added." << endl;
    log("New station added with ID: " + to_string(nextId));

    nextId++;
}

void CompressorStationManager::displayStations() const {
    if (stations.empty()) {
        cout << "There is no stations to watch." << endl;
        return;
    }
    cout << "List of stations:" << endl;
    cout << endl;
    for (const auto& station : stations) {
        cout << "ID " << station.first;
        station.second.display();
    }
    cout << endl;
    log("List of stations.");
}

void CompressorStationManager::removeStation() {
    if (stations.empty()) {
        cout << "There is no list of stations." << endl;
        return;
    }
    cout << "Enter the station ID to delete: ";
    string command;
    long id;
    getline(cin, command);
    id = numberOrDefault(command);
    if (id == -1) {
        cout << "Try again" << endl;
        return;
    }

    auto it = stations.find(id);
    if (it == stations.end()) {
        cout << "Eror.There is no station with that ID" << endl;
        return;
    }

    stations.erase(it);
    cout << "Station was deleted." << endl;
}

void CompressorStationManager::editStation() {
    if (stations.empty()) {
        cout << "The list is empty, you can't edit the station" << endl;
        return;
    }
    cout << "Add the ID of station to edit: ";
    string command;
    long id;
    getline(cin, command);
    id = numberOrDefault(command);
    if (id == -1) {
        cout << "Try again" << endl;
        return;
    }

    auto it = stations.find(id);
    if (it != stations.end()) {
        cout << "Editing station with ID " << id << endl;
        toggleShop(it->second, it->first);
        log("Updated status of the workshop of the station with ID: " + to_string(id));
    }
    else {
        cout << "Station with ID " << id << " didn't found." << endl;
    }
}

void CompressorStationManager::searchStationName() {
    string searchName;
    log("Launched search for stations by 'Name' filter.");
    cout << "Add the station name for searching (@_@): ";
    cin >> searchName;
    cin.ignore();
    log("Added name:" + searchName);

    if (stations.empty()) {
        cout << "I can't find (soriamba (-_-))" << endl;
        log("There is no names for this filter.");
    }
    else {
        cout << "Results of searching by the filter 'Name':\n";
        bool found = false;
        for (const auto& station : stations) {
            if (station.second.full_name.find(searchName) != string::npos) {
                cout << "ID " << station.first;
                station.second.display();
                log("Results of searching by the filter 'Name': " + to_string(station.first));
                found = true;
            }
        }

        if (!found) {
            cout << "There is no info" << endl;
            log("There is no info");
        }
    }
}

void CompressorStationManager::searchStationUnused() {
    double unusedOperationalShopsCheck = 0.0;
    long aroundUnusedOperationalShops;
    string command;
    log("Launched search for the stations by 'Unused workshops' filter");
    cout << "Enter the percentage of idle workshops: ";
    getline(cin, command);
    aroundUnusedOperationalShops = numberOrDefault(command);
    if (aroundUnusedOperationalShops == -1) {
        cout << "Try again" << endl;
        return;
    }

    log("Search for stations 'Unused workshops': Percentage entered: " + to_string(unusedOperationalShopsCheck));


    cout << "-----------------------------" << endl;
    cout << "              Menu:          " << endl;
    cout << " 1. = %        OMG           " << endl;
    cout << " 2. > %          MAMI        " << endl;
    cout << " 3. < %            CHUPAPI   " << endl;
    cout << "-----------------------------" << endl;
    int choice;
    choice = inputIntInRange("Choose an action: ", 1, 3);
    log("Choose an action: " + to_string(choice));

    if (stations.empty()) {
        cout << "There is no info" << endl;
        log("There is no info");
        return;
    }
    cout << "Results of the searching:\n";

    for (const auto& station : stations) {
        aroundUnusedOperationalShops = station.second.unusedOperationalShops;

        bool conditionMet = false;
        switch (choice) {
        case 1:
            conditionMet = (aroundUnusedOperationalShops == unusedOperationalShopsCheck);
            log("= %");
            break;
        case 2:
            conditionMet = (aroundUnusedOperationalShops > unusedOperationalShopsCheck);
            log("> %");
            break;
        case 3:
            conditionMet = (aroundUnusedOperationalShops < unusedOperationalShopsCheck);
            log("< %");
            break;
        default:
            cout << "Incorrect choice" << endl;
            return;
        }

        if (conditionMet) {
            cout << "ID " << station.first;
            station.second.display();
            log("Results of the searching: " + to_string(station.first));
        }
    }
}

void CompressorStationManager::searchStationMenu() {
    cout << "---------------------------------------" << endl;
    cout << "        Menu(Choose me please):       " << endl;
    cout << " 1. Filter NAME                       " << endl;
    cout << " 2. Filter percentage unused workshops" << endl;
    cout << " 0. U spin me right round (Back)      " << endl;
    cout << "---------------------------------------" << endl;
}

void CompressorStationManager::searchStationSwitch() {
    if (stations.empty()) {
        cout << "The list is empty. i can't start searching (UwU)" << endl;
        return;
    }
    int choice;
    while (true) {
        searchStationMenu();
        choice = inputIntInRange("Choose the action: ", 0, 2);
        log("Action: " + to_string(choice));

        switch (choice) {
        case 1:
            searchStationName();
            break;
        case 2:
            searchStationUnused();
            break;
        case 0:
            CompressorStationSwitch();
            log("Menu station manager");
        default:
            cout << "Durachok. MB try again" << endl;
            log("Incorrect choice: " + choice);
            break;
        }
    }
}

void CompressorStationManager::displayCompressorStationMenu() {
    cout << "-----------------------------" << endl;
    cout << "            Menu:          " << endl;
    cout << " 1. Add the station        " << endl;
    cout << " 2. Edit the station       " << endl;
    cout << " 3. Delete the station     " << endl;
    cout << " 4. Check the stations list" << endl;
    cout << " 5. Searching by filter    " << endl;
    cout << " 0. Dive back in time! (Back)" << endl;
    cout << "-----------------------------" << endl;

}

void CompressorStationManager::CompressorStationSwitch() {
    void runProgram();
    int choice;

    while (true) {
        displayCompressorStationMenu();
        choice = inputIntInRange("I choose myself (Choose the action): ", 0, 5);
        log("Action (: " + to_string(choice));

        switch (choice) {
        case 1:
            addStation();
            break;
        case 2:
            editStation();
            break;
        case 3:
            removeStation();
            break;
        case 4:
            displayStations();
            break;
        case 5:
            log("Action - filter");
            searchStationSwitch();
            break;
        case 0:
            log("Base menu");
            runProgram();
        default:
            cout << "Try again sobaka-babaka" << endl;
            log("Incorrect choice: " + choice);
            break;
        }
    }
}