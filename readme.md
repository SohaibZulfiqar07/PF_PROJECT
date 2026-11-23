# THE LAST TRAIN

A deterministic, text-based rescue and exploration game written in C++17 (no OOP classes).
Course project for **CS-1002: Programming Fundamentals (AI & DS)**.

**Authors:** `<Name 1> (<RollNo1>)`, `<Name 2> (<RollNo2>)`
**Release tag:** `v1.0.<RollNo>`

---

## About the game

You are trapped in a 5-floor building (4 rooms per floor, 20 rooms in total). Before your
**Energy** or **Time** runs out, you must:

1. Rescue the **injured passenger** (needs the Medical Kit).
2. Secure the **stranded cat** (needs the Cat Carrier).
3. Collect the three **clue digits**, unlock the cabinet with code **427**, and take the **Train Key**.
4. Install the **Fuse** at the Control Panel and collect the **Lift Pass** to restore Lift 2.
5. Prepare the train and board it at **F5-R2**.

If Energy or Time reaches 0 before you finish, the mission fails. Invalid actions cost nothing.

Maximum score: **520 points**.

---

## Build

Requires `g++` with C++17 support.

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/core/*.cpp -o last_train
```

Alternatively, with CMake:

```bash
cmake -S . -B build
cmake --build build
```

## Run

Run from the **project root**, because the program opens files by relative path
(`data/building.txt`, `data/initial-state.txt`).

```bash
./last_train
```

On Windows (MinGW/PowerShell): `.\last_train.exe`

---

## Project structure

```
.
├── CMakeLists.txt
├── README.md
├── include/            Header files
│   ├── config.h        Constants: sizes, action costs, bonuses, scores
│   ├── state.h         Connection types, entity symbols, objective indices
│   ├── level.h         Map and initial-state loading
│   ├── physics.h       Movement rules, inventory and resource helpers
│   ├── switches.h      Lifts, fuse, cabinet code
│   ├── simulator.h     Game loop and command handlers
│   ├── logger.h        Save, load and summary report
│   ├── viewer_iface.h  Terminal rendering
│   └── util.h          Small helpers (clamp, trim, compare)
├── src/core/           Source files matching the headers (+ main_core.cpp)
├── data/               building.txt, initial-state.txt, test input files
├── out/                savegame.txt, game_summary.txt (generated)
└── docs/               Project_Diagram.jpg, Flowchart.png, ConsoleUI.png
```

## Modules

| File | Responsibility |
|---|---|
| `util.cpp` | Clamping, whitespace trimming, string comparison, screen helpers |
| `level.cpp` | Parses `building.txt` and `initial-state.txt`; entity names; start-room validation |
| `physics.cpp` | Inventory add/remove/check, resource and score updates, connectivity and traversal rules |
| `switches.cpp` | Lift access rules, fuse installation, cabinet code check |
| `viewer_iface_core.cpp` | Dashboard, room details, ASCII building, objectives checklist |
| `logger.cpp` | Writes and reads `savegame.txt`; writes `game_summary.txt` |
| `simulator.cpp` | Main command loop, command handlers, win/lose conditions |
| `main_core.cpp` | Program entry point and initialisation |

The code uses plain arrays, C-strings and reference parameters instead of classes.

---

## Game rules

### Resource costs

| Action | Energy | Time |
|---|---|---|
| Move through a door or stairs | -2 | -1 |
| Move through a window | -3 | -2 |
| Lift journey (Lift 1 or Lift 2) | -4 | -3 |
| Inspect a room | -1 | -1 |
| Pick up an item | -1 | -1 |
| Use an item / puzzle interaction | -1 | -1 |

Pickup bonuses: Energy Pack **+25 Energy**, Emergency Toolkit **+15 Energy**,
Observation Token **+15 Time**. Energy and Time are clamped between 0 and 100.

### Connections

| Type | Rule |
|---|---|
| Door (`D`) | Always passable |
| Window (`W`) | Always passable (higher cost) |
| Locked door (`L`) | F3-R1 and F3-R2 only; needs the **Master Key** |
| Solid wall (`#`) | Impassable (F4-R2 and F4-R3) |
| Staircases | F1-R1 and F2-R1, F2-R1 and F3-R1, F4-R1 and F5-R1 |
| Lift 1 | Floors 1 and 2 (R2/R3), always available |
| Lift 2 | F2-R4, F4-R4 and F5-R4; needs the Fuse installed and the Lift Pass, or up to 3 emergency trips with the Emergency Access Card |

### Scoring

| Objective | Points |
|---|---|
| Master Key collected | +10 |
| Each clue digit (4, 2, 7) | +10 each |
| Cabinet unlocked (code 427) | +30 |
| Train Key acquired | +10 |
| Passenger rescued | +100 |
| Cat secured | +100 |
| Fuse installed | +20 |
| Lift Pass collected | +20 |
| Train prepared and boarded | +100 |
| Mission victory bonus | +100 |
| **Total** | **520** |

---

## Commands

> Update this table to match the commands your `runSimulation` actually accepts.

| Command | Description |
|---|---|
| `move <floor> <room>` | Move to a connected room |
| `inspect` | Search the current room (reveals clue digits) |
| `collect <item>` | Pick up an item in the room |
| `use <item>` | Use an item or interact with a fixture |
| `unlock <code>` | Enter the cabinet code |
| `save` | Save progress to `out/savegame.txt` |
| `quit` | End the session |

---

## Data and output files

| File | Purpose |
|---|---|
| `data/building.txt` | Static ASCII map. Read at launch, never modified |
| `data/initial-state.txt` | Starting values (Energy 100, Time 50, Score 0, empty inventory, all objectives false) |
| `out/savegame.txt` | Saved snapshot: position, resources, inventory, emergency trips, objective flags, room contents |
| `out/game_summary.txt` | Final report: score, remaining resources, objective checklist |

---

## Verifying determinism

Given the same initial state and the same sequence of commands, the program produces
identical output every time. To check:

```bash
# 1. Run with a prepared command file, saving the output
./last_train < data/test_input_1.txt > run1.txt
cp out/game_summary.txt summary1.txt

# 2. Run again with the same input
./last_train < data/test_input_1.txt > run2.txt
cp out/game_summary.txt summary2.txt

# 3. Compare. No output means identical
diff run1.txt run2.txt
diff summary1.txt summary2.txt
```

No timestamps or random values are used anywhere, so the runs match exactly.

---

## Implementation status

> Keep this table updated as you go, and delete it before final submission.

| Module | Status |
|---|---|
| `util.cpp` | Done |
| `level.cpp` | In progress (`loadInitialState` done) |
| `physics.cpp` | Not started |
| `switches.cpp` | Not started |
| `viewer_iface_core.cpp` | Not started |
| `logger.cpp` | Not started |
| `simulator.cpp` | Not started |

---

## Notes

- Floors and rooms are **1-based** for the player and **0-based** inside arrays
  (`F3-R2` is `roomEntities[2][1]`).
- All paths are relative. Run from the project root.
- The program compiles without warnings under `-Wall -Wextra -std=c++17`.