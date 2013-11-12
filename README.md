This program is a simulation of the following rules.

Rules
-----
The board is infinite to the top and right.
Start with a stone in the bottom-left corner.
When removing a Stone, place two new stones, one above and one right of it.
Objective: remove any stone from the bottom left corner, so that the remaining space looks like a triangle:

    ?????
    x????
    xx???
    xxx??

`?` anything, stone or empty
`x` empty space

Arguments
---------
`[program name] [-b <width> <height>] [-s]`
- `-b`
    - set board size
    - `width`
        - integer greater than 0
        - Width of the board. Although the imaginary board is infinite, the computer can only handle a limited amount of it.
    - `height`
        - integer greater than 0
        - Height of the board. See `width`.
- `-s`
    - Enable printing statistics about the board, currently including the number of stones and blocking stones, absolute and in percent of the board.

