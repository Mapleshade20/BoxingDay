# BoxingDay

The C++ implementation of Human Resource Machine game.

## Milestones

The project will have 3 major releases.

- Alpha implements the fundamental game logic. We set up a main module that reads preset `game data`, receives `instructions` given by the player and evaluates if the outbox sequence matches the desired one. (**This module shall pass the OJ.**)
- Beta implements a basic command-line UI framework, including in-game elements display and keyboard interactions.
- 1.0 finalizes the gameplay experience. It records `player data` of each level, stores all data into files and restores it after a program rerun.

<details>

<summary>Implementation Details</summary>

## Implementation Details

### Data Files

`game data` is built-in for the first four levels, and open to customization for other levels. It consists of a inbox number sequence, a desired outbox number sequence, level title, level number, level description, available instructions, and available tile amount.

`player data` is the steps and instructions used when a level finishes. It's recorded and displayed in the menu (display to be done in Beta).

The two types of data is stored in two separate `toml` files. By modifying the existing files or creating new ones, we can make changes to the levels playable.

### Game Interactions

Display module consists of `inbox`, `outbox`, `memory` (tiles), `instructions` (boxes), `register` (the agent) and the game menu.

Keyboard controls a virtual cursor over buttons and `instructions`, and triggers actions when receiving certain keys. The set-alias feature may also allow player to type in a string to rename the tiles.

</details>

## Todos

- [ ] Read game data from `toml` files
- [ ] Scan instructions from stdin and generate an `array` that contains each instruction as a `struct`
- [ ] Run player's instructions and return status as each iteration goes (useful for UI development)
