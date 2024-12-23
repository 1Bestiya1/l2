#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Pipe.h"
#include "CompressorStation.h"
#include "utils.h"
#include "PipeManager.h"
#include "CompressorStationManager.h"
#include <locale>

using namespace std;

map<int, Pipe> pipes;
map<int, CompressorStation> stations;

void setupLocale() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
}

void saveToFile(const map<int, Pipe>& pipes, const map<int, CompressorStation>& stations, CompressorStationManager& cmanager, PipeManager& pmanager) {
    string filename;
    cout << "Enter a file name to save stations and pipes: ";
    cin >> filename;
    cin.ignore();
    ofstream ofs(filename + ".txt");
    log("Save to file operation started: " + filename + ".txt");

    if (ofs) {
        for (const auto& station : stations) {
            ofs << "station " << station.first << ": " << cmanager.saveCS(station.second) << endl;
        }

        for (const auto& pipe : pipes) {
            ofs << "pipe " << pipe.first << ": " << pmanager.savePipe(pipe.second) << endl;
        }

        ofs.close();
        cout << "Station and pipe data are saved in " << filename << ".txt" << endl;
        log("Successfully saved to file: " + filename + ".txt");
    }
    else {
        cerr << "Error opening file to save." << endl;
        log("Error opening file to save: " + filename + ".txt");
    }
}

void loadFromFile(map<int, Pipe>& pipes, map<int, CompressorStation>& stations) {
    string filename;
    cout << "Enter a file name to download stations and pipes (Let is snow): ";
    cin >> filename;
    cin.ignore();
    log("Try to download from file: " + filename + ".txt");

    ifstream ifs(filename + ".txt");
    if (ifs) {
        string line;
        while (getline(ifs, line)) {
            try {
                if (line.substr(0, 2) == "Station") {
                    size_t pos = line.find(':');
                    if (pos == string::npos) throw runtime_error("Invalid string format for station");

                    int stationKey = stoi(line.substr(3, pos - 3));
                    CompressorStation station;
                    station.load(line.substr(pos + 1));

                    if (stations.find(stationKey) == stations.end()) {
                        stations[stationKey] = station;
                        log("Loaded station with ID: " + to_string(stationKey));
                    }
                    else {
                        cerr << "Error. Station with ID " << stationKey << " is already exist." << endl;
                        log("Error. Station with ID " + to_string(stationKey) + " is already exist.");
                    }
                }
                else if (line.substr(0, 4) == "pipe") {

                    size_t pos = line.find(':');
                    if (pos == string::npos) throw runtime_error("Invalid string format for pipe");

                    int pipeKey = stoi(line.substr(5, pos - 5));
                    Pipe pipe;
                    pipe.load(line.substr(pos + 1));

                    if (pipes.find(pipeKey) == pipes.end()) {
                        pipes[pipeKey] = pipe;
                        log("Pipe loaded with ID: " + to_string(pipeKey));
                    }
                    else {
                        cerr << "Pipe with ID " << pipeKey << " is already exist." << endl;
                        log("Error: Pipe with ID " + to_string(pipeKey) + " is already exist.");
                    }
                }
            }
            catch (const exception& e) {
                cerr << "Downloading error: " << e.what() << endl;
            }
        }
        ifs.close();
    }
    else {
        cerr << "Error opening file: " << filename << ".txt" << endl;
    }
}

void runProgram() {
    PipeManager pManager;
    CompressorStationManager sManager;
    int choice;

    while (true) {
        cout << "-------------------------------" << endl;
        cout << "            Menu:              " << endl;
        cout << " 1. Pipes                WE    " << endl;
        cout << " 2. Stations            WISH   " << endl;
        cout << " 3. View all objects    YOU    " << endl;
        cout << " 4. Save                 A     " << endl;
        cout << " 5. Download           MARRY   " << endl;
        cout << " 0. Exit             CHRISTMAS!" << endl;
        cout << "-------------------------------" << endl;
        choice = inputIntInRange("Select an action: ", 0, 5);
        log("Action selected:" + to_string(choice));


        switch (choice) {
        case 1:
            pManager.PipeSwitch();
            break;
        case 2:
            sManager.CompressorStationSwitch();
            break;
        case 3:
            pManager.displayAllPipes();
            sManager.displayStations();
            log("All pipes and stations have been displayed.");
            break;
        case 4:
            saveToFile(pipes, stations, sManager, pManager);
            break;
        case 5:
            loadFromFile(pipes, stations);
            break;
        case 0:
            cout << "AND A HAPPY NEW YEAR!" << endl;
            log("Exit the program.");
            exit(1);
        default:
            cout << "Incorrect choice, try again. (I got a hangover. Ain't my fault..)" << endl;
            log("Incorrect choice: " + choice);
            break;
        }
    }
}

int main() {
    setupLocale();
    log("SUCCESS!");
    runProgram();
    return 0;
}
