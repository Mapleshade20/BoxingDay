# BoxingDay

The C++ implementation of Human Resource Machine game.

## Milestones

The project is having 3 major releases.

- Alpha implements the fundamental game logic. We set up a main module that reads preset `game info`, receives `instructions` given by the player and evaluates if the outbox sequence matches the desired one. (**This module shall pass the OJ.**)
- Beta implements a basic command-line UI framework, including in-game elements display and keyboard interactions.
- Gamma finalizes the gameplay experience. It records `player info` of each level, stores all data into files and restores it after a program rerun.

<details>

<summary>Implementation Details</summary>

## Implementation Details

`game info` is built-in for the first four levels, and open to customization for other levels. It consists of a inbox number sequence, a desired outbox number sequence, level name, level number, level description, available instructions, and available tile amount.

`player info` is the steps and instructions used when a level finishes. It's recorded and displayed in the menu (display to be done in Beta).

Display module consists of `inbox`, `outbox`, `memory` (tiles), `instructions` (boxes), `register` (the agent) and the game menu.

Keyboard controls a virtual cursor over buttons and `instructions`, and triggers actions when receiving certain keys. The set-alias feature may also allow player to type in a string to rename the tiles.

</details>

## Todos

- [ ] Read game info from files
- [ ] Scan instructions from stdin
- [ ] Run player's instructions and return status as each iteration goes (useful for UI development)
