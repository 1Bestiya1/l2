#include "PipeManager.h"
#include "Pipe.h"
#include <vector>
#include <iostream>
#include <map>

extern std::map<int, Pipe> pipes;
using namespace std;

string PipeManager::savePipe(const Pipe& pipe) const {
    return pipe.full_name + ";" + to_string(pipe.length) + ";" + to_string(pipe.diameter) + ";" + to_string(pipe.underRepair);
}

void PipeManager::toggleRepair(Pipe& pipe, int id) {
    if (pipe.length > 0) {
        pipe.underRepair = !pipe.underRepair;
        cout << "ID: " << id << " - IN REMONT changed on: " << (pipe.underRepair ? "Yes" : "No") << endl;
        log("Status of the pipe with ID " + to_string(id) + string(pipe.underRepair ? " In remont" : " At work (!_!)"));
    }
    else {
        cout << "There is no pipe" << endl;
    }
}

void PipeManager::addPipe() {
    Pipe newPipe;
    newPipe.read();

    while (pipes.find(nextId) != pipes.end()) {
        nextId++;
    }

    pipes[nextId] = newPipe;

    cout << "Pipe added with ID: " << nextId << endl;
    log("Pipe added with ID: " + to_string(nextId));

    nextId++;
}

void PipeManager::editPipes() {
    if (pipes.empty()) {
        cout << "The list of the pipes is empty. U can't edit" << endl;
        return;
    }
    cout << "Add ID of the pipes for editing, WITH ',' (or 'pls' for editing all pipes): ";
    string input;
    getline(cin, input);
    stringstream ss(input);
    string idStr;
    vector<int> ids;
    vector<int> nonExistentIds;

    if (input == "pls") {
        for (auto& pipePair : pipes) {
            toggleRepair(pipePair.second, pipePair.first);
        }
        cout << "All pipes were updated" << endl;
        log("All pipes were updated(status 'in remont').");
    }
    else {
        while (getline(ss, idStr, ',')) {
            try {
                int id = stoi(idStr);
                ids.push_back(id);
            }
            catch (const invalid_argument&) {
                cout << "Error. Let's do this one more time!" << endl;
                return;
            }
            catch (const out_of_range&) {
                cout << "Error. Value is out of range" << endl;
                return;
            }
        }

        bool edited = false;
        for (int id : ids) {
            auto it = pipes.find(id);
            if (it != pipes.end()) {
                toggleRepair(it->second, id);
                edited = true;
            }
            else {
                nonExistentIds.push_back(id);
            }
        }

        if (edited) {
            cout << "Status was updated." << endl;
        }

        if (!nonExistentIds.empty()) {
            cout << "Pipe(s) with ID: ";
            for (size_t i = 0; i < nonExistentIds.size(); ++i) {
                cout << nonExistentIds[i];
                if (i < nonExistentIds.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "doesn't exist" << endl;
        }
    }
}

void PipeManager::deletePipes() {
    if (pipes.empty()) {
        cout << "The list of pipes is empty. U can't delete (^_^)" << endl;
        return;
    }
    cout << "Add the ID of the pipes, using ',' (or 'pls' to delete all pipes): ";
    string input;
    getline(cin, input);

    if (input == "pls") {
        pipes.clear();
        cout << "All pipes were deleted" << endl;
        log("All pipes were deleted. The list is empty");
        return;
    }

    stringstream ss(input);
    string idStr;
    vector<int> ids;
    vector<int> nonExistentIds;

    while (getline(ss, idStr, ',')) {
        try {
            int id = stoi(idStr);
            ids.push_back(id);
        }
        catch (const invalid_argument&) {
            cout << "Error: '" << idStr << "' try again. I'm already tired (!_!)" << endl;
            log("Error: '" + idStr + "' not right number");
            return;
        }
        catch (const out_of_range&) {
            cout << "Error: Value '" << idStr << "' out of range." << endl;
            log("Error: Value '" + idStr + "' out of range.");
            return;
        }
    }

    for (int id : ids) {
        auto it = pipes.find(id);

        if (it != pipes.end()) {
            pipes.erase(it);
            cout << "Pipe with ID " << id << " was deleted." << endl;
            log("Pipe was deleted. It's ID: " + to_string(id));
        }
        else {
            nonExistentIds.push_back(id);
        }
    }

    if (!nonExistentIds.empty()) {
        cout << "Pipe(s) with ID: ";
        for (size_t i = 0; i < nonExistentIds.size(); ++i) {
            cout << nonExistentIds[i];
            if (i < nonExistentIds.size() - 1) {
                cout << ", ";
            }
        }
        cout << " doesn't exist" << endl;
    }
}

void PipeManager::displayAllPipes() {
    if (pipes.empty()) {
        cout << "There is no pipes" << endl;
    }
    else {
        cout << "the list of the pipes:" << endl;
        cout << endl;
        for (const auto& pipe : pipes) {
            cout << "ID " << pipe.first;
            pipe.second.display();
        }
        cout << endl;
    }
    log("The list of the pipes");
}

void PipeManager::editPipesFound(const vector<int>& foundIds) {
    cout << "Add the ID of the pipe for editing, using ',' (or 'pls' for editing all pipes): ";
    string input;
    getline(cin, input);
    stringstream ss(input);
    string idStr;
    vector<int> ids;
    vector<int> nonExistentIds;

    if (input == "pls") {
        // Обновляем только найденные трубы
        for (int id : foundIds) {
            auto it = pipes.find(id);
            if (it != pipes.end()) {
                toggleRepair(it->second, id);
            }
        }
        cout << "All found pipes were updated" << endl;
        log("All found pipes were updated (status 'in remont')");
    }
    else {
        while (getline(ss, idStr, ',')) {
            try {
                int id = stoi(idStr);
                ids.push_back(id);
            }
            catch (const invalid_argument&) {
                cout << "Error: I was made for loving u baby. Try again" << endl;
                return;
            }
            catch (const out_of_range&) {
                cout << "Error: out of range" << endl;
                return;
            }
        }

        bool edited = false;
        for (int id : ids) {
            if (find(foundIds.begin(), foundIds.end(), id) != foundIds.end()) {
                auto it = pipes.find(id);
                if (it != pipes.end()) {
                    toggleRepair(it->second, id);
                    edited = true;
                }
            }
            else {
                nonExistentIds.push_back(id);
            }
        }

        if (edited) {
            cout << "Status was updated" << endl;
        }

        if (!nonExistentIds.empty()) {
            cout << "Pipe(s) with ID: ";
            for (size_t i = 0; i < nonExistentIds.size(); ++i) {
                cout << nonExistentIds[i];
                if (i < nonExistentIds.size() - 1) {
                    cout << ", ";
                }
            }
            cout << " doesn't exist." << endl;
        }
    }
}

void PipeManager::deletePipesFound(const vector<int>& foundIds) {
    cout << "Add the ID of the pipes for deleting, using ',' (or 'pls' for deleting all pipes): ";
    string input;
    getline(cin, input);

    if (input == "pls") {
        for (int id : foundIds) {
            pipes.erase(id);
            cout << "Pipe with ID " << id << " was deleted." << endl;
            log("Pipe was deleted with ID: " + to_string(id));
        }
        cout << "All found pipes were deleted" << endl;
        log("All found pipes were deleted");
        return;
    }

    stringstream ss(input);
    string idStr;
    vector<int> ids;
    vector<int> nonExistentIds;

    while (getline(ss, idStr, ',')) {
        try {
            int id = stoi(idStr);
            if (find(foundIds.begin(), foundIds.end(), id) != foundIds.end()) {
                ids.push_back(id);
            }
            else {
                nonExistentIds.push_back(id);
            }
        }
        catch (const invalid_argument&) {
            cout << "Error: '" << idStr << "' missundersood. Try again" << endl;
            log("Error: '" + idStr + "' not true value");
            return;
        }
        catch (const out_of_range&) {
            cout << "Error:  '" << idStr << "' Don't be that way. out of range" << endl;
            log("Error: '" + idStr + "' out of range");
            return;
        }
    }

    for (int id : ids) {
        auto it = pipes.find(id);
        if (it != pipes.end()) {
            pipes.erase(it);
            cout << "Pipe with ID " << id << " If I love u was the promise. was deleted." << endl;
            log("Pipe was deleted with ID: " + to_string(id));
        }
    }

    if (!nonExistentIds.empty()) {
        cout << "Pipe(s) with ID: ";
        for (size_t i = 0; i < nonExistentIds.size(); ++i) {
            cout << nonExistentIds[i];
            if (i < nonExistentIds.size() - 1) {
                cout << ", ";
            }
        }
        cout << " doesn't exist." << endl;
    }
}

void PipeManager::displayPipeMenuFound() {
    cout << "-----------------------------------------------" << endl;
    cout << "                   Menu list:                  " << endl;
    cout << " 1. Delete the pipes                           " << endl;
    cout << " 2. Edit the pipes                             " << endl;
    cout << " 0. Back (Okay, I'll show you what's in my bag)" << endl;
    cout << "-----------------------------------------------" << endl;
}

void PipeManager::handleFoundPipeMenu(const vector<int>& foundPipeIds) {
    int choice;
    while (true) {
        displayPipeMenuFound();
        choice = inputIntInRange("Choose the action: ", 0, 2);
        log("Action: " + to_string(choice));

        switch (choice) {
        case 1:
            deletePipesFound(foundPipeIds);
            break;
        case 2:
            editPipesFound(foundPipeIds);
            break;
        case 0:
            log("Filter of the pipes");
            return;
        default:
            cout << "Incorrect choice. Try again" << endl;
            log("Incorrect choice: " + choice);
            break;
        }
    }
}

void PipeManager::searchPipesName() {
    string searchName;
    log("Searching for pipes using filter 'NAME'");
    cout << "Add the name for searching pipes: ";
    cin >> searchName;
    cin.ignore();
    log("Searching for pipes using name:" + searchName);

    if (pipes.empty()) {
        cout << "I can't find..maybe it's my bad.." << endl;
        log("There is no information about pipes using this tag");
    }
    else {
        cout << "Results of the searching:\n";
        vector<int> foundPipeIds;
        bool found = false;

        for (const auto& pipe : pipes) {
            if (pipe.second.full_name.find(searchName) != string::npos) {
                cout << "ID " << pipe.first;
                pipe.second.display();
                log("Results of the searching, using filter 'NAME', their ID: " + to_string(pipe.first));
                foundPipeIds.push_back(pipe.first);
                found = true;
            }
        }
        if (!found) {
            cout << "There is no info, using this tag (&_&)" << endl;
            log("There is no info, using this tag");
            return;
        }
        else {
            handleFoundPipeMenu(foundPipeIds);
        }
    }
}

void PipeManager::searchPipesRepair() {
    bool repairStatus;
    log("Searching pipes using 'status in remont'");
    cout << "Searching pipes 'status in remont' (1 - Yes, 0 - And(No)):";
    repairStatus = inputIntInRange(" ", 0, 1);
    log("Searching pipes using 'status in remont': Status (1 - Yes, 0 - No):" + to_string(repairStatus));

    if (pipes.empty()) {
        cout << "There is no info using this tag" << endl;
        log("There is no info using this tag");
        return;
    }
    cout << "Results of searching:\n";

    vector<int> foundPipeIds;

    for (const auto& pipe : pipes) {
        if (pipe.second.underRepair == repairStatus) {
            cout << "ID " << pipe.first;
            pipe.second.display();
            log("Results searching with tag 'status in remont', ID: " + to_string(pipe.first));
            foundPipeIds.push_back(pipe.first);

        }
    }
    handleFoundPipeMenu(foundPipeIds);
    if (foundPipeIds.empty()) {
        cout << "There is no info (%_%)" << endl;
        log("There is no info using tag 'status in remont'");
        return;
    }
}

void PipeManager::searchPipeMenu() {
    cout << "-----------------------------" << endl;
    cout << "           God's Menu:       " << endl;
    cout << " 1. Search using 'NAME'      " << endl;
    cout << " 2. Search using 'STATUS'    " << endl;
    cout << " 0. Dive back in time (Back) " << endl;
    cout << "-----------------------------" << endl;
}

void PipeManager::searchPipeSwitch() {
    if (pipes.empty()) {
        cout << "The list is emty. Search can't be done" << endl;
        return;
    }
    int choice;
    while (true) {
        searchPipeMenu();
        choice = inputIntInRange("Choose the action: ", 0, 5);
        log("Action: " + to_string(choice));

        switch (choice) {
        case 1:
            searchPipesName();
            break;
        case 2:
            searchPipesRepair();
            break;
        case 0:
            PipeSwitch();
            log("Action 'Pipe control'");
        default:
            cout << "Incorrect choice. Try again (~_~)" << endl;
            log("Incorrect choice: " + choice);
            break;
        }
    }
}

void PipeManager::displayPipeMenu() {
    cout << "---------------------------" << endl;
    cout << "            Menu:          " << endl;
    cout << " 1. Add the pipe           " << endl;
    cout << " 2. Edit the pipe          " << endl;
    cout << " 3. Delete the pipe        " << endl;
    cout << " 4. List of the pipes      " << endl;
    cout << " 5. Search by the filter   " << endl;
    cout << " 0. Back                   " << endl;
    cout << "---------------------------" << endl;
}

void PipeManager::PipeSwitch() {
    void runProgram();
    int choice;
    while (true) {
        displayPipeMenu();
        choice = inputIntInRange("Choose the action: ", 0, 5);
        log("Action: " + to_string(choice));

        switch (choice) {
        case 1:
            addPipe();
            break;
        case 2:
            editPipes();
            break;
        case 3:
            deletePipes();
            break;
        case 4:
            displayAllPipes();
            break;
        case 5:
            searchPipeSwitch();
            log("Action 'Pipe filter'");
            break;
        case 0:
            log("Action 'Base menu'");
            runProgram();
        default:
            cout << "Incorrect choice. Chupapimuniania, try again " << endl;
            log("Incorrect choice: " + choice);
            break;
        }
    }
}