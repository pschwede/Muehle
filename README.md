Nine Men's Morris
=================

Your assignment is to develop a computer player for the Nine Men's Morris game.
The code archive you get includes everything you need to get started. It should
compile and run on most Linux distributions and on Mac OS X, although we only
officially support the Ubuntu configuration in the computer pool. If you want
to work on your own machine, you need a reasonably modern C compiler and
environment with C99 and UNIX 03 support. With gcc or LLVM's clang you should
be fine.

The game rules for Nine Men's Morris can be found in Wikipedia:
http://en.wikipedia.org/wiki/Nine_Men%27s_Morris
http://de.wikipedia.org/wiki/M%C3%BChle_(Spiel)

Since some of the rules are vague, here are some clarifications which are
mandatory within this assignment and the tournament:
* If you have a mill, you may remove one of your opponent's pieces from the
  board. If all of your opponent's pieces are part of a formed mill, you may
  remove any of those pieces. Otherwise, you may only remove pieces that are
  not part of a formed mill.
* Even if you close two mills in one move, you may only remove one piece.
* Once you are reduced to three pieces, your pieces may fly. This means you
  can move them to any empty spot on the board.
* The game ends in a draw if the current game state already appeared earlier.
* The game ends with the opposing player winning if the current player is
  unable to make a valid move.

The code archive you received includes a Makefile to compile everything. The
command "make help" tells you about the supported build targets. A plain "make"
will compile everything.

Once compiled, you get three binaries:
* The Master Control Program (MCP) allows two computer programs to play against
  each other. The MCP orchestrates the game by sending the current game state
  to the players and receiving their moves. The MCP makes sure the rules are
  obeyed and the players behave.
* An example player, which allows you to play the game yourself with keyboard
  input.
* An empty file my_player.c where you should develop your game playing
  algorithm. Look at the example player to learn how you receive the game state
  from the MCP and how to send your own move back. You may copy code from the
  example player, otherwise all player code has to be written by you. You may
  not use code from the MCP. (That's why the interesting bits of the MCP are
  obfuscated by delivering them in assembler code.)

Use "make run" to start your player against the example player, so you can play
the game against the computer.

The official check whether you pass the assignment and also the tournament is
performed under a time and memory limit for your player. You may use 1GB of
memory and up to 60 seconds of think time for one move (multiple threads are
allowed, fork()ing is not). These limits are enforced by the MCP. You can test
your player under these conditions using "make fight".

If you implement a long-running algorithm in your player, you may react to the
SIGXCPU signal. The MCP sends this signal when the 60 second think timer has
expired. If you react to this signal, you get one extra second to report your
move to the MCP before it SIGKILLs you.
