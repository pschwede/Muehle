#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

//Variablen

// Moegliche Mühlen auf einem Mühle-Brett
char muehlen[16][3] = {{'A','B','C'},{'D','E','F'},{'G','H','I'},
  {'J','K','L'},{'M','N','O'},{'P','Q','R'},{'S','T','U'},{'V','W','X'},
  {'A','J','V'},{'D','K','S'},{'G','L','P'},{'B','E','H'},{'Q','T','W'},
  {'I','M','R'},{'F','N','U'},{'C','O','X'}};
// Mögliche Züge; Verbindungen zwischen Positionen
char moegliche_wege[32][2] = {{'A','B'},{'B','C'},{'C','O'},{'O','X'},
  {'X','W'},{'W','V'},{'V','J'},{'J','A'},{'D','E'},{'E','F'},
  {'F','N'},{'N','U'},{'U','T'},{'T','S'},{'S','K'},{'K','D'},
  {'G','H'},{'H','I'},{'I','M'},{'M','R'},{'R','Q'},{'Q','P'},
  {'P','L'},{'L','G'},{'B','E'},{'E','H'},{'M','N'},{'N','O'},
  {'J','K'},{'K','L'},{'Q','T'},{'T','W'}};
char board[24];

unsigned current_player;
unsigned unplaced_pieces[2];
char newline;
int matches;
int treffer;
char weggefunden[4];

//Funktionsaufrufe


//Funktionen

char int2sym(int player) {
  switch(player) {
    case 0: return '0';
    case 1: return '1';
    default: return '.';
  }
}
char int2char(int random_integer) {
  char umwandlung = random_integer+65;
  return umwandlung;
}

int char2int(char random_character) {
  int umwandlung = random_character-65;
  return umwandlung;
}

char random_piece_of_color(char col) {
    char pos;
    int random_integer, gesetzt = 0;
    
    while (gesetzt == 0) {
        random_integer = random()%23;
        
        if(board[random_integer] == '.') {
            pos = int2char(random_integer);
            printf("Setze stein auf %c, zuvor war hier ein %c\n", pos, board[random_integer]);
            gesetzt = 1;
        }
    }
    return pos;
}

char steinesetzen() {
    return random_piece_of_color('.');
}

char killstein() {
  char enemy_color = int2sym((current_player+1)%2);
  return random_piece_of_color(enemy_color);
}


int muehlen_check(char* piece_kill) {
  char piece_put = -1;

  char k[4] = "   ";
  for (int i = 0; i<16; i++) {
    for (int j = 0; j<3; j++) {
      k[j] = board[char2int(muehlen[i][j])];
    }
    if (current_player == 0) {
      if (strcmp(k,".11") == 0) {
        piece_put = muehlen[i][0];
      } else if (strcmp(k,"1.1") == 0) {
        piece_put = muehlen[i][1];
      } else if (strcmp(k,"11.") == 0) {
        piece_put = muehlen[i][2];
      } else if (strcmp(k,".00") == 0) {
        piece_put = muehlen[i][0];
        *piece_kill = killstein();
      } else if (strcmp(k,"0.0") == 0) {
        piece_put = muehlen[i][1];
        *piece_kill = killstein();
      } else if (strcmp(k,"00.") == 0) {
        piece_put = muehlen[i][2];
        *piece_kill = killstein();
      }
    } else {
      if (strcmp(k,".00") == 0) {
        piece_put = muehlen[i][0];
      } else if (strcmp(k,"0.0") == 0) {
        piece_put = muehlen[i][1];
      } else if (strcmp(k,"00.") == 0) {
        piece_put = muehlen[i][2];
      } else if (strcmp(k,".11") == 0) {
        piece_put = muehlen[i][0];
        *piece_kill = killstein();
      } else if (strcmp(k,"1.1") == 0) {
        piece_put = muehlen[i][1];
        *piece_kill = killstein();
      } else if (strcmp(k,"11.") == 0) {
        piece_put = muehlen[i][2];
        *piece_kill = killstein();
      }
    }
  }
  if(piece_put >= 0)
    printf("Muehle gefunden (%c) bei {%c, %c, %c}\n", piece_put, k[0], k[1], k[2]);
  if(*piece_kill != ' ')
    printf("Nehme weg: %c\n", *piece_kill);

  return piece_put;
}

void steineziehen(char* piece_move, char* piece_put) {
  char from, to;
  bool no_hit = true;

  while(no_hit) { // solange kein feld vom player besetzt...
    from = int2char(random()%23);
    printf("Ich bin Player %c und ich tippe auf... %c\n", int2sym(current_player), from);
    if(board[char2int(from)] == int2sym(current_player)) { // feld vom player besetzt
      printf("Auf %c liegt ein Stein von mir (%c)\n", from, int2sym(current_player));
      // durchsuche board nach möglichen zielpositionen
      for(int i=0;i<32;i++){
        for (int j=0; j<2; j++) {
          to = moegliche_wege[i][(j+1)%2];
          if(moegliche_wege[i][j] == from && board[char2int(to)] == '.') {
            // beweglichen stein und zielposition gefunden
            *piece_move = from;
            *piece_put = to;
            printf("Verschiebe von %c nach %c\n", from, to);
            no_hit = false;
          }
        }
      }
    }
  }
}

int main(void)
{
  char piece_move, piece_put, piece_kill;

  // Unix-Pipes Ã¶ffnen
  /* current game state is delivered via file descriptor 3 */
  FILE *state_input = fdopen(3, "r");
  /* we output our move via file descriptor 4 */
  FILE *move_output = fdopen(4, "w");

  if (!state_input || !move_output) {
    fprintf(stderr, "error accessing file descriptors\n");
    exit(1);
  }
  srandom(time(NULL));


  while(1) { 
    //Variablen

    //Spielzustaende und Board holen
    matches = fscanf(state_input, "%u %u %u%c", &current_player,
        &unplaced_pieces[0], &unplaced_pieces[1], &newline);
    if(matches !=  4) {
      fprintf(stderr, "error while reading the game state\n");
      break;
    }

    matches = fscanf(state_input, "%24c%c", board, &newline);
    if(matches !=  2) {
      fprintf(stderr, "error while reading the board state\n");
      break;
    }

    printf("Boardposition ABCDEFGHIJKLMNOPQRSTUVWX\n              ");
    for(int i = 0; i<24; i++) {
      printf("%c", board[i]);
    }
    printf("\n");

    // Zuerst auf moegliche Muehlen reagieren und ansonsten zufällig Stein setzen
    if(unplaced_pieces[current_player]>0) {
      printf("mache muehlencheck...\n");
      piece_move = piece_kill = ' ';
      piece_put = muehlen_check(&piece_kill);

      if(piece_put < 0) { // keine Mühle gefunden
        printf("keine Muehle gefunden...setze stein\n");
        piece_put = steinesetzen();
      }
    } else {
      //Alle Steine gesetzt...jetzt werden moegliche Wege gesucht
      steineziehen(&piece_move, &piece_put); 
      piece_kill = ' ';
    }
    printf("Player %i does '%c%c%c'\n", current_player, piece_move, piece_put, piece_kill);
    //Rueckgabe aller Werte an den MCP
    fprintf(move_output, "%c%c%c\n", piece_move, piece_put, piece_kill); 
    fflush(move_output);
  }	
  return 0;
}
