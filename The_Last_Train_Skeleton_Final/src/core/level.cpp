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
                     int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
                        for(int i=0; i<NUM_FLOORS; i++){
                            for(int j=0; j<ROOMS_PER_FLOOR; j++){
                                if(j<ROOMS_PER_FLOOR-1)
                                    horizontalConn[i][j] = CONN_NONE;
                                entityCounts[i][j] = 0;
                                for(int k=0; k<MAX_ENTITIES_PER_ROOM; k++){
                                    roomEntities[i][j][k] = ENT_NONE;
                                }
                            }
                        }
                        ifstream fin(filename);
                        if(!(fin.is_open())){
                            return false;
                        }
                        char line[MAX_LINE_LEN];

                        while (fin.getline(line, MAX_LINE_LEN)){
                            int len = 0;
                            while (line[len] != '\0') 
                                    len++;

                            if (len > 0 && line[len - 1] == '\r')
                                line[len - 1] = '\0';

                            int labelPos[ROOMS_PER_FLOOR];
                            int found = 0;
                            int floorNum = 0;
                            for (int i = 0; line[i] != '\0'; i++) {
                                if(line[i] == 'F' && (line[i+1] >= '0' && line[i+1]<= '9')
                                 && line[i+2] == '-' && line[i+3] == 'R' && line[i+4] >= '0'
                                  && line[i+4]<= '9'){
                                    floorNum = line[i + 1] - '0';
                                    int roomNum = line[i + 4] - '0';
                                    if (roomNum >= 1 && roomNum <= ROOMS_PER_FLOOR) {
                                        labelPos[roomNum - 1] = i;
                                        found++;
                                    }
                                }
                            }
                            if (found != ROOMS_PER_FLOOR) continue;
                            if (floorNum < 1 || floorNum > NUM_FLOORS) continue;
                            int f = floorNum - 1;

                            for (int r = 0; r < ROOMS_PER_FLOOR - 1; r++) {
                                char conn = CONN_NONE;
                                for (int p = labelPos[r] + 5; p < labelPos[r + 1]; p++) {
                                    if (line[p] != ' ') {
                                        conn = line[p];
                                        if(conn == 'L' && line[p+1] == '1' )
                                            conn = CONN_NONE;
                                        break;
                                    }
                                }
                                horizontalConn[f][r] = conn;
                            }

                            char itemLine[MAX_LINE_LEN];
                            if (!fin.getline(itemLine, MAX_LINE_LEN)) break;
                             
                            int itemLen = 0;
                            while (itemLine[itemLen] != '\0')
                             itemLen++;

                            if (itemLen > 0 && itemLine[itemLen - 1] == '\r')
                                itemLine[itemLen - 1] = '\0';

                            int dividers = 0;
                            for (int p = labelPos[0]; p < itemLen && dividers < ROOMS_PER_FLOOR; p++) {
                                char c = itemLine[p];
                                if (c == '|') { dividers++; continue; }
                                if (c == ' ') continue;

                                int room = 0;
                                for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
                                    if (labelPos[r] <= p ) 
                                        room = r;
                                }

                                if(entityCounts[f][room] < MAX_ENTITIES_PER_ROOM){
                                    roomEntities[f][room][entityCounts[f][room]] = c;
                                    entityCounts[f][room]++ ;
                                }
                            }

                            if (floorNum == 1) break;

                        }

                        fin.close();

                        if (entityCounts[0][3] < MAX_ENTITIES_PER_ROOM) {
                              roomEntities[0][3][ entityCounts[0][3] ] = ENT_ENERGY_PACK;
                            entityCounts[0][3]++;
                        }
                        return true;
                        
                    }

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
                          const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1]) {


    if (startFloor < 1 || startFloor > NUM_FLOORS) {
        return false;
    }
    if (startRoom < 1 || startRoom > ROOMS_PER_FLOOR) {
        return false;
    }


    if (startRoom == 1) {
        return true;
    }


    char left = horizontalConn[startFloor - 1][startRoom-2];
    if (left == CONN_DOOR || left == CONN_WINDOW) {
        return true;
    }


    if (startRoom < ROOMS_PER_FLOOR) {
        char right = horizontalConn[startFloor - 1][startRoom-1];
        if (right == CONN_DOOR || right == CONN_WINDOW) {
            return true;
        }
    }


    return false;
}

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