#include "level.h"
#include "util.h"
#include <fstream>
#include <cstdlib>
#include <iostream>
using namespace std;

// Modular implementation using primitive arrays for map parsing and verification


bool loadBuildingMap(const char* filename,
                     char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                     char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                     int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]);

bool loadInitialState(const char* filename,
                      int& playerFloor, int& playerRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft){

                        (void)playerFloor;
                        (void)playerRoom;
                        (void)inventory;

                        energy = INITIAL_ENERGY;
                        time = INITIAL_TIME;
                        score = INITIAL_SCORE;

                        inventoryCount = 0;
                        emergencyTripsLeft = 0;

                        for(int i=0; i<NUM_OBJECTIVES; i++){
                            objectives[i] = 0;
                        }

                        ifstream fin(filename);
                            if(!fin.is_open()){
                                return false;
                            }
                        char line[MAX_LINE_LEN];

                    while (fin.getline(line, MAX_LINE_LEN)) {
                           int colon = 0;
                            while (line[colon] != '\0' && line[colon] != ':') {
                                colon++;
                                }
                            if (line[colon] == '\0') continue;
                            if (colon >= MAX_NAME_LEN) continue;

                            char key[MAX_NAME_LEN];
                            for (int i = 0; i < colon; i++) {

                            key[i] = line[i];
                            }

                            key[colon] = '\0';

                            

                            char value[MAX_LINE_LEN];
                            int k = 0;

                            for(int j = colon+1; line[j]!='\0';j++){
                                value[k] = line [j];
                                k++;
                                
                            }
                            value[k] ='\0';
                            
                           trimWhitespace(key);
                            trimWhitespace(value);


                        if (stringEquals(key, "INITIAL_ENERGY")) {
                            energy = atoi(value);
                        } else if (stringEquals(key, "INITIAL_TIME")) {
                            time = atoi(value);
                        } else if (stringEquals(key, "INITIAL_SCORE")) {
                            score = atoi(value);
                        } else if (stringEquals(key, "CABINET_UNLOCKED")
                                   && stringEquals(value, "YES")) {
                            objectives[OBJ_CABINET_UNLOCKED] = true;
                        }
                        else if (stringEquals(key, "TRAIN_KEY_COLLECTED")
                                   && stringEquals(value, "YES")) {
                            objectives[OBJ_TRAIN_KEY] = true;
                        }
                        else if (stringEquals(key, "FUSE_INSTALLED")
                                   && stringEquals(value, "YES")) {
                            objectives[OBJ_FUSE_INSTALLED] = true;
                        }
                        else if (stringEquals(key, "LIFT_PASS_COLLECTED")
                                   && stringEquals(value, "YES")) {
                            objectives[OBJ_LIFT_PASS] = true;
                        }
                        else if (stringEquals(key, "PASSENGER_RESCUED")
                                   && stringEquals(value, "YES")) {
                            objectives[OBJ_PASSENGER_RESCUED] = true;
                        }
                        else if (stringEquals(key, "CAT_RESCUED")
                                   && stringEquals(value, "YES")) {
                            objectives[OBJ_CAT_SECURED] = true;
                        }
                        else if (stringEquals(key, "TRAIN_PREPARED")
                                   && stringEquals(value, "YES")) {
                            objectives[OBJ_TRAIN_PREPARED] = true;
                        }
                    }
                        
                    energy = clampInt(energy, 0, MAX_ENERGY);
                    time = clampInt(time, 0, MAX_TIME);

                        fin.close();

                        return true;

}

bool validateStartingRoom(int startFloor, int startRoom,
                          const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1]);

const char* getEntityName(char entitySymbol){
     switch (entitySymbol) {
        case ENT_MASTER_KEY:
            return "Master Key";
        case ENT_ENERGY_PACK:
            return "Energy Pack";
        case ENT_EMERGENCY_TOOLKIT:
            return "Emergency Toolkit";
        case ENT_CLUE_4:
            return "Clue Digit 4";
        case ENT_CLUE_2:
            return "Clue Digit 2";
        case ENT_CLUE_7:
            return "Clue Digit 7";
        case ENT_FUSE:
            return "Fuse";
        case ENT_CAT:
            return "Cat";
        case ENT_CONTROL_PANEL:
            return "Control Panel";
        case ENT_LIFT_PASS:
            return "Lift Pass";
        case ENT_PASSENGER:
            return "Injured Passenger";
        case ENT_MEDICAL_KIT:
            return "Medical Kit";
        case ENT_EMERGENCY_CARD:
            return "Emergency Access Card";
        case ENT_CABINET_TRAIN_KEY:
            return "Cabinet";
        case ENT_CAT_CARRIER:
            return "Cat Carrier";
        case ENT_OBSERVATION_TOKEN:
            return "Observation Token";
        case ENT_NONE:
            return "Empty";
        default:
            return "Unknown";
    }
}