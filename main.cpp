
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <future>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <random>
#include <time.h>

using namespace std;
using namespace chrono;

const unsigned short int EDGE_COLUMN = 120, EDGE_LINE = 37, AMOUNT_ENEMIES = 18, EDGE_COLOR = 1; // indica os limites e cores da borda e a quantidade de naves inimigas

unsigned char actualStage;

typedef high_resolution_clock Clock;

typedef struct // criando struct que representará as fases do jogo
{
  unsigned char maxAmntShots;
  unsigned short int shotsFrequency;
  unsigned char gameSpeed;
  bool isCompleted;
}GameStage;

typedef struct // criando struct que representará as naves
{
  unsigned short int col;
  unsigned short int lin;
  bool isAlive = true;
  bool isShoting;

  struct Shot
  {
    unsigned short int col;
    unsigned short int lin;    
  }Shot;
}Spaceship;

GameStage gameStages[] =
{
  {2, 2000, 50},
  {3, 2000, 45},
  {3, 1500, 40},
  {3, 1000, 40},
  {3, 1000, 35},
  {3, 1000, 30},
  {3,  500, 35}
};

Spaceship Player; // criando a nave do jogador
Spaceship Enemies[AMOUNT_ENEMIES]; // criando um array de naves inimigas
mt19937 generator; //instanciando gerador de números aleatórios
text_info vActual = {0, 0, 79, 24, WHITE, WHITE, C80, 25, 80, 1, 1};

void SetCursor(unsigned short int x,unsigned short int y);
unsigned char GetKeyPressed();
void SetTextColor(int ForgC);
void SetBackgroundColor(int newColor);
int GetRandomNumber(int Min, int Max);
void StageCompletedAnimation();
void VictoryAnimation();
void DefeatAnimation();
void PlayerExplosion();
bool PlayerIsDead();
void EnemyShotAnimation();
int GetAmountOfDeadEnemies();
void ChooseEnemytoShoot(unsigned char stageIndex);
void EnemyExplosion(unsigned char i);
void ShotHitEnemy();
void PlayerShot();
void PlayerMovimentation(char key);
void ClearPlayerSpaceship();
void PrintPlayerSpaceship();
void ClearEnemy();
void PrintEnemy();
void MainStream();
void PrintFrame();
void Inicialize();
void Controls();
void PrintMenuOptions(unsigned short int previousIndex, unsigned short int arrowIndex);
void MainMenu();

// ===================================== Set Cursor Position ==========================================

void SetCursor(unsigned short int x, unsigned short int y) // posiciona o cursor de acordo com a cordenada coluna X linha
{
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x,y});
}

// ===================================== Set Background Color ==========================================

void SetBackgroundColor(int newColor)
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
    (csbi.wAttributes & 0x0f) | (newColor << 4));
  vActual.attribute = (csbi.wAttributes & 0x0f) | (newColor << 4);
}

// ===================================== Set Text Color ==========================================

void SetTextColor(int ForgC)
 {
  WORD wColor;
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
  {
    wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
    SetConsoleTextAttribute(hStdOut, wColor);
  }
  return;
}

// ===================================== Hide the console cursor ==========================================

void HideCursor()
{
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 100;
  info.bVisible = FALSE;
  SetConsoleCursorInfo(consoleHandle, &info);
}

// ================================== Get key =========================================================

unsigned char GetKeyPressed() // retorna o próximo caractere digitado.
{
  unsigned char key = 0;

  do{ key = getch(); }while(key == 0);

  return key;
}

//======================================= Get Random Number ===========================================

int GetRandomNumber(int Min, int Max)
{
  uniform_int_distribution<uint32_t> dice(Min, Max);
  return dice(generator);
}

// ================================== Stage Completed Animation ===============================================

void StageCompletedAnimation()
{
  unsigned short int lin = 5, col = 28, cont = 0;
  unsigned char colors[5];

  for (unsigned char i = 0; i < 5; i++)
    colors[i] = GetRandomNumber(1, 15);

  system("clear||cls");
  PrintFrame();

  switch(actualStage)
  {
    case 1:
      for(unsigned char i = 0; i < 15; i++)
      {
        SetTextColor(colors[cont]);
        SetCursor(col, lin); printf("$$$$$$$$\\                                $$\\");
        SetCursor(col, lin + 1); printf("$$  _____|                             $$$$ |");
        SetCursor(col, lin + 2); printf("$$ |  $$$$$$\\  $$$$$$$\\ $$$$$$\\        \\_$$ |");
        SetCursor(col, lin + 3); printf("$$$$$\\\\____$$\\$$  _____$$  __$$\\         $$ |");
        SetCursor(col, lin + 4); printf("$$  __$$$$$$$ \\$$$$$$\\ $$$$$$$$ |        $$ |");
        SetCursor(col, lin + 5); printf("$$ | $$  __$$ |\\____$$\\$$   ____|        $$ |");
        SetCursor(col, lin + 6); printf("$$ | \\$$$$$$$ $$$$$$$  \\$$$$$$$\\       $$$$$$\\");
        SetCursor(col, lin + 7); printf("\\__|  \\_______\\_______/ \\_______|      \\______|");
        SetCursor(col, lin + 10); printf(" $$$$$$\\                            $$\\         $$\\      $$\\");
        SetCursor(col, lin + 11); printf("$$  __$$\\                           $$ |        \\__|     $$ |");
        SetCursor(col, lin + 12); printf("$$ /  \\__|$$$$$$\\ $$$$$$$\\  $$$$$$$\\$$ $$\\   $$\\$$\\ $$$$$$$ |$$$$$$\\");
        SetCursor(col, lin + 13); printf("$$ |     $$  __$$\\$$  __$$\\$$  _____$$ $$ |  $$ $$ $$  __$$ |\\____$$\\");
        SetCursor(col, lin + 14); printf("$$ |     $$ /  $$ $$ |  $$ $$ /     $$ $$ |  $$ $$ $$ /  $$ |$$$$$$$ |");
        SetCursor(col, lin + 15); printf("$$ |  $$\\$$ |  $$ $$ |  $$ $$ |     $$ $$ |  $$ $$ $$ |  $$ $$  __$$ |");
        SetCursor(col, lin + 16); printf("\\$$$$$$  \\$$$$$$  $$ |  $$ \\$$$$$$$\\$$ \\$$$$$$  $$ \\$$$$$$$ \\$$$$$$$ |");
        SetCursor(col, lin + 17); printf(" \\______/ \\______/\\__|  \\__|\\_______\\__|\\______/\\__|\\_______|\\_______|");

        cont++;
        if (cont == 4)
          cont = 0;

        Sleep(200);
      }
      break;
    case 2:
      for(unsigned char i = 0; i < 15; i++)
      {
        SetTextColor(colors[cont]);
        SetCursor(col, lin); printf("$$$$$$$$\\                               $$$$$$\\");
        SetCursor(col, lin + 1); printf("$$  _____|                             $$  __$$\\");
        SetCursor(col, lin + 2); printf("$$ |  $$$$$$\\  $$$$$$$\\ $$$$$$\\        \\__/  $$ |");
        SetCursor(col, lin + 3); printf("$$$$$\\\\____$$\\$$  _____$$  __$$\\        $$$$$$  |");
        SetCursor(col, lin + 4); printf("$$  __$$$$$$$ \\$$$$$$\\ $$$$$$$$ |      $$  ____/");
        SetCursor(col, lin + 5); printf("$$ | $$  __$$ |\\____$$\\$$   ____|      $$ |");
        SetCursor(col, lin + 6); printf("$$ | \\$$$$$$$ $$$$$$$  \\$$$$$$$\\       $$$$$$$$\\");
        SetCursor(col, lin + 7); printf("\\__|  \\_______\\_______/ \\_______|      \\________|");
        SetCursor(col, lin + 10); printf(" $$$$$$\\                            $$\\         $$\\      $$\\");
        SetCursor(col, lin + 11); printf("$$  __$$\\                           $$ |        \\__|     $$ |");
        SetCursor(col, lin + 12); printf("$$ /  \\__|$$$$$$\\ $$$$$$$\\  $$$$$$$\\$$ $$\\   $$\\$$\\ $$$$$$$ |$$$$$$\\");
        SetCursor(col, lin + 13); printf("$$ |     $$  __$$\\$$  __$$\\$$  _____$$ $$ |  $$ $$ $$  __$$ |\\____$$\\");
        SetCursor(col, lin + 14); printf("$$ |     $$ /  $$ $$ |  $$ $$ /     $$ $$ |  $$ $$ $$ /  $$ |$$$$$$$ |");
        SetCursor(col, lin + 15); printf("$$ |  $$\\$$ |  $$ $$ |  $$ $$ |     $$ $$ |  $$ $$ $$ |  $$ $$  __$$ |");
        SetCursor(col, lin + 16); printf("\\$$$$$$  \\$$$$$$  $$ |  $$ \\$$$$$$$\\$$ \\$$$$$$  $$ \\$$$$$$$ \\$$$$$$$ |");
        SetCursor(col, lin + 17); printf(" \\______/ \\______/\\__|  \\__|\\_______\\__|\\______/\\__|\\_______|\\_______|");

        cont++;
        if (cont == 4)
          cont = 0;

        Sleep(200);

      }
      break;
    case 3:
      for(unsigned char i = 0; i < 15; i++)
      {
        SetTextColor(colors[cont]);
        SetCursor(col, lin); printf("$$$$$$$$\\                               $$$$$$\\");
        SetCursor(col, lin + 1); printf("$$  _____|                             $$ ___$$\\");
        SetCursor(col, lin + 2); printf("$$ |  $$$$$$\\  $$$$$$$\\ $$$$$$\\        \\_/   $$ |");
        SetCursor(col, lin + 3); printf("$$$$$\\\\____$$\\$$  _____$$  __$$\\         $$$$$ /");
        SetCursor(col, lin + 4); printf("$$  __$$$$$$$ \\$$$$$$\\ $$$$$$$$ |        \\___$$\\");
        SetCursor(col, lin + 5); printf("$$ | $$  __$$ |\\____$$\\$$   ____|      $$\\   $$ |");
        SetCursor(col, lin + 6); printf("$$ | \\$$$$$$$ $$$$$$$  \\$$$$$$$\\       \\$$$$$$  |");
        SetCursor(col, lin + 7); printf("\\__|  \\_______\\_______/ \\_______|       \\______/");
        SetCursor(col, lin + 10); printf(" $$$$$$\\                            $$\\         $$\\      $$\\");
        SetCursor(col, lin + 11); printf("$$  __$$\\                           $$ |        \\__|     $$ |");
        SetCursor(col, lin + 12); printf("$$ /  \\__|$$$$$$\\ $$$$$$$\\  $$$$$$$\\$$ $$\\   $$\\$$\\ $$$$$$$ |$$$$$$\\");
        SetCursor(col, lin + 13); printf("$$ |     $$  __$$\\$$  __$$\\$$  _____$$ $$ |  $$ $$ $$  __$$ |\\____$$\\");
        SetCursor(col, lin + 14); printf("$$ |     $$ /  $$ $$ |  $$ $$ /     $$ $$ |  $$ $$ $$ /  $$ |$$$$$$$ |");
        SetCursor(col, lin + 15); printf("$$ |  $$\\$$ |  $$ $$ |  $$ $$ |     $$ $$ |  $$ $$ $$ |  $$ $$  __$$ |");
        SetCursor(col, lin + 16); printf("\\$$$$$$  \\$$$$$$  $$ |  $$ \\$$$$$$$\\$$ \\$$$$$$  $$ \\$$$$$$$ \\$$$$$$$ |");
        SetCursor(col, lin + 17); printf(" \\______/ \\______/\\__|  \\__|\\_______\\__|\\______/\\__|\\_______|\\_______|");

        cont++;
        if (cont == 4)
          cont = 0;

        Sleep(200);
      }
      break;
    case 4:
      for(unsigned char i = 0; i < 15; i++)
      {
        SetTextColor(colors[cont]);
        SetCursor(col, lin); printf("$$$$$$$$\\                              $$\\   $$\\");
        SetCursor(col, lin + 1); printf("$$  _____|                             $$ |  $$ |");
        SetCursor(col, lin + 2); printf("$$ |  $$$$$$\\  $$$$$$$\\ $$$$$$\\        $$ |  $$ |");
        SetCursor(col, lin + 3); printf("$$$$$\\\\____$$\\$$  _____$$  __$$\\       $$$$$$$$ |");
        SetCursor(col, lin + 4); printf("$$  __$$$$$$$ \\$$$$$$\\ $$$$$$$$ |      \\_____$$ |");
        SetCursor(col, lin + 5); printf("$$ | $$  __$$ |\\____$$\\$$   ____|            $$ |");
        SetCursor(col, lin + 6); printf("$$ | \\$$$$$$$ $$$$$$$  \\$$$$$$$\\             $$ |");
        SetCursor(col, lin + 7); printf("\\__|  \\_______\\_______/ \\_______|            \\__|");
        SetCursor(col, lin + 10); printf(" $$$$$$\\                            $$\\         $$\\      $$\\");
        SetCursor(col, lin + 11); printf("$$  __$$\\                           $$ |        \\__|     $$ |");
        SetCursor(col, lin + 12); printf("$$ /  \\__|$$$$$$\\ $$$$$$$\\  $$$$$$$\\$$ $$\\   $$\\$$\\ $$$$$$$ |$$$$$$\\");
        SetCursor(col, lin + 13); printf("$$ |     $$  __$$\\$$  __$$\\$$  _____$$ $$ |  $$ $$ $$  __$$ |\\____$$\\");
        SetCursor(col, lin + 14); printf("$$ |     $$ /  $$ $$ |  $$ $$ /     $$ $$ |  $$ $$ $$ /  $$ |$$$$$$$ |");
        SetCursor(col, lin + 15); printf("$$ |  $$\\$$ |  $$ $$ |  $$ $$ |     $$ $$ |  $$ $$ $$ |  $$ $$  __$$ |");
        SetCursor(col, lin + 16); printf("\\$$$$$$  \\$$$$$$  $$ |  $$ \\$$$$$$$\\$$ \\$$$$$$  $$ \\$$$$$$$ \\$$$$$$$ |");
        SetCursor(col, lin + 17); printf(" \\______/ \\______/\\__|  \\__|\\_______\\__|\\______/\\__|\\_______|\\_______|");

        cont++;
        if (cont == 4)
          cont = 0;

        Sleep(200);
      }
      break;
    case 5:
      for(unsigned char i = 0; i < 15; i++)
      {
        SetTextColor(colors[cont]);
        SetCursor(col, lin); printf("$$$$$$$$\\                              $$$$$$$\\");
        SetCursor(col, lin + 1); printf("$$  _____|                             $$  ____|");
        SetCursor(col, lin + 2); printf("$$ |  $$$$$$\\  $$$$$$$\\ $$$$$$\\        $$ |");
        SetCursor(col, lin + 3); printf("$$$$$\\\\____$$\\$$  _____$$  __$$\\       $$$$$$$\\");
        SetCursor(col, lin + 4); printf("$$  __$$$$$$$ \\$$$$$$\\ $$$$$$$$ |      \\_____$$\\");
        SetCursor(col, lin + 5); printf("$$ | $$  __$$ |\\____$$\\$$   ____|      $$\\   $$ |");
        SetCursor(col, lin + 6); printf("$$ | \\$$$$$$$ $$$$$$$  \\$$$$$$$\\       \\$$$$$$  |");
        SetCursor(col, lin + 7); printf("\\__|  \\_______\\_______/ \\_______|       \\______/");
        SetCursor(col, lin + 10); printf(" $$$$$$\\                            $$\\         $$\\      $$\\");
        SetCursor(col, lin + 11); printf("$$  __$$\\                           $$ |        \\__|     $$ |");
        SetCursor(col, lin + 12); printf("$$ /  \\__|$$$$$$\\ $$$$$$$\\  $$$$$$$\\$$ $$\\   $$\\$$\\ $$$$$$$ |$$$$$$\\");
        SetCursor(col, lin + 13); printf("$$ |     $$  __$$\\$$  __$$\\$$  _____$$ $$ |  $$ $$ $$  __$$ |\\____$$\\");
        SetCursor(col, lin + 14); printf("$$ |     $$ /  $$ $$ |  $$ $$ /     $$ $$ |  $$ $$ $$ /  $$ |$$$$$$$ |");
        SetCursor(col, lin + 15); printf("$$ |  $$\\$$ |  $$ $$ |  $$ $$ |     $$ $$ |  $$ $$ $$ |  $$ $$  __$$ |");
        SetCursor(col, lin + 16); printf("\\$$$$$$  \\$$$$$$  $$ |  $$ \\$$$$$$$\\$$ \\$$$$$$  $$ \\$$$$$$$ \\$$$$$$$ |");
        SetCursor(col, lin + 17); printf(" \\______/ \\______/\\__|  \\__|\\_______\\__|\\______/\\__|\\_______|\\_______|");

        cont++;
        if (cont == 4)
          cont = 0;

        Sleep(200);
      }
      break;
    case 6:
      for(unsigned char i = 0; i < 15; i++)
      {
        SetTextColor(colors[cont]);
        SetCursor(col, lin); printf("$$$$$$$$\\                               $$$$$$\\");
        SetCursor(col, lin + 1); printf("$$  _____|                             $$  __$$\\");
        SetCursor(col, lin + 2); printf("$$ |  $$$$$$\\  $$$$$$$\\ $$$$$$\\        $$ /  \\__|");
        SetCursor(col, lin + 3); printf("$$$$$\\\\____$$\\$$  _____$$  __$$\\       $$$$$$$\\");
        SetCursor(col, lin + 4); printf("$$  __$$$$$$$ \\$$$$$$\\ $$$$$$$$ |      $$  __$$\\");
        SetCursor(col, lin + 5); printf("$$ | $$  __$$ |\\____$$\\$$   ____|      $$ /  $$ |");
        SetCursor(col, lin + 6); printf("$$ | \\$$$$$$$ $$$$$$$  \\$$$$$$$\\        $$$$$$  |");
        SetCursor(col, lin + 7); printf("\\__|  \\_______\\_______/ \\_______|       \\______/");
        SetCursor(col, lin + 10); printf(" $$$$$$\\                            $$\\         $$\\      $$\\");
        SetCursor(col, lin + 11); printf("$$  __$$\\                           $$ |        \\__|     $$ |");
        SetCursor(col, lin + 12); printf("$$ /  \\__|$$$$$$\\ $$$$$$$\\  $$$$$$$\\$$ $$\\   $$\\$$\\ $$$$$$$ |$$$$$$\\");
        SetCursor(col, lin + 13); printf("$$ |     $$  __$$\\$$  __$$\\$$  _____$$ $$ |  $$ $$ $$  __$$ |\\____$$\\");
        SetCursor(col, lin + 14); printf("$$ |     $$ /  $$ $$ |  $$ $$ /     $$ $$ |  $$ $$ $$ /  $$ |$$$$$$$ |");
        SetCursor(col, lin + 15); printf("$$ |  $$\\$$ |  $$ $$ |  $$ $$ |     $$ $$ |  $$ $$ $$ |  $$ $$  __$$ |");
        SetCursor(col, lin + 16); printf("\\$$$$$$  \\$$$$$$  $$ |  $$ \\$$$$$$$\\$$ \\$$$$$$  $$ \\$$$$$$$ \\$$$$$$$ |");
        SetCursor(col, lin + 17); printf(" \\______/ \\______/\\__|  \\__|\\_______\\__|\\______/\\__|\\_______|\\_______|");

        cont++;
        if (cont == 4)
          cont = 0;

        Sleep(200);
      }
      break;
  }

  SetCursor(40, 30); SetTextColor(15);
  system("PAUSE");
}

// ================================== Victory Animation ===============================================

void VictoryAnimation()
{
  unsigned short int lin = 5, col = 30, cont = 0;
  unsigned char colors[] = {3, 14, 6, 11, 9};

  system("clear||cls");
  PrintFrame();

  for(unsigned char i = 0; i < 15; i++)
  {
    SetTextColor(colors[cont]);
    SetCursor(col, lin); printf("                                       $\\");
    SetCursor(col, lin + 1); printf("                                      $$$\\");
    SetCursor(col, lin + 2); printf("                                     $$ $$\\");
    SetCursor(col, lin + 3); printf("      $$\\    $$\\                    $$  \\$$\\");
    SetCursor(col, lin + 4); printf("      $$ |   $$ |                   \\__/ \\__|");
    SetCursor(col, lin + 5); printf("      $$ |   $$ |$$$$$$\\  $$$$$$$\\ $$$$$$\\");
    SetCursor(col, lin + 6); printf("      \\$$\\  $$  $$  __$$\\$$  _____$$  __$$\\");
    SetCursor(col, lin + 7); printf("       \\$$\\$$  /$$ /  $$ $$ /     $$$$$$$$ |");
    SetCursor(col, lin + 8); printf("        \\$$$  / $$ |  $$ $$ |     $$   ____|");
    SetCursor(col, lin + 9); printf("$$\\    $$\\$  /  \\$$$$$$  \\$$$$$$$\\\\$$$$$$$\\                   $$\\");
    SetCursor(col, lin + 10); printf("$$ |   $$ \\_/    \\______/ \\_______|\\_______|                  $$ |");
    SetCursor(col, lin + 11); printf("$$ |   $$ |$$$$$$\\ $$$$$$$\\  $$$$$$$\\ $$$$$$\\ $$\\   $$\\       $$ |");
    SetCursor(col, lin + 12); printf("\\$$\\  $$  $$  __$$\\$$  __$$\\$$  _____$$  __$$\\$$ |  $$ |      $$ |");
    SetCursor(col, lin + 13); printf(" \\$$\\$$  /$$$$$$$$ $$ |  $$ $$ /     $$$$$$$$ $$ |  $$ |      \\__|");
    SetCursor(col, lin + 14); printf("  \\$$$  / $$   ____$$ |  $$ $$ |     $$   ____$$ |  $$ |          ");
    SetCursor(col, lin + 15); printf("   \\$  /  \\$$$$$$$\\$$ |  $$ \\$$$$$$$\\\\$$$$$$$\\\\$$$$$$  |      $$\\ ");
    SetCursor(col, lin + 16); printf("    \\_/    \\_______\\__|  \\__|\\_______|\\_______|\\______/       \\__|");

    cont++;
    if (cont == 4)
      cont = 0;

    Sleep(200);
  }
  SetCursor(40, 30); SetTextColor(15);
  system("PAUSE");
}

// ========================================== Defeat Animation ===========================================

void DefeatAnimation()
{
  unsigned short int lin = 5, col = 30;
  unsigned char cont = 0;
  unsigned char colors[] = {6, 3, 11, 7, 12};

  system("clear||cls");
  PrintFrame();

  for(unsigned char i = 0; i < 15; i++)
  {
    SetTextColor(colors[cont]);
    SetCursor(col, lin); printf("                                       $\\");
    SetCursor(col, lin + 1); printf("                                      $$$\\");
    SetCursor(col, lin + 2); printf("                                     $$ $$\\");
    SetCursor(col, lin + 3); printf("      $$\\    $$\\                    $$  \\$$\\");
    SetCursor(col, lin + 4); printf("      $$ |   $$ |                   \\__/ \\__|");
    SetCursor(col, lin + 5); printf("      $$ |   $$ |$$$$$$\\  $$$$$$$\\ $$$$$$\\");
    SetCursor(col, lin + 6); printf("      \\$$\\  $$  $$  __$$\\$$  _____$$  __$$\\");
    SetCursor(col, lin + 7); printf("       \\$$\\$$  /$$ /  $$ $$ /     $$$$$$$$ |");
    SetCursor(col, lin + 8); printf("        \\$$$  / $$ |  $$ $$ |     $$   ____|");
    SetCursor(col, lin + 9); printf("         \\$  /  \\$$$$$$  \\$$$$$$$\\\\$$$$$$$\\                      ");
    SetCursor(col, lin + 10); printf("          \\_/    \\______/ \\_______|\\_______|                     ");
    SetCursor(col, lin + 11); printf("$$$$$$$\\                         $$\\                         $$\\");
    SetCursor(col, lin + 12); printf("$$  __$$\\                        $$ |                        $$ |");
    SetCursor(col, lin + 13); printf("$$ |  $$ |$$$$$$\\  $$$$$$\\  $$$$$$$ |$$$$$$\\ $$\\   $$\\       $$ |");
    SetCursor(col, lin + 14); printf("$$$$$$$  $$  __$$\\$$  __$$\\$$  __$$ $$  __$$\\$$ |  $$ |      $$ |");
    SetCursor(col, lin + 15); printf("$$  ____/$$$$$$$$ $$ |  \\__$$ /  $$ $$$$$$$$ $$ |  $$ |      \\__|");
    SetCursor(col, lin + 16); printf("$$ |     $$   ____$$ |     $$ |  $$ $$   ____$$ |  $$ |          ");
    SetCursor(col, lin + 17); printf("$$ |     \\$$$$$$$\\$$ |     \\$$$$$$$ \\$$$$$$$\\\\$$$$$$  |      $$\\ ");
    SetCursor(col, lin + 18); printf("\\__|      \\_______\\__|      \\_______|\\_______|\\______/       \\__|");

    cont++;
    if (cont == 5)
      cont = 0;

    Sleep(200);
  }
    SetCursor(40, 30); SetTextColor(15);
    system("PAUSE");
}

// ================================== Player Explosion ===================================================

void PlayerExplosion() // realiza a animação da explosão das naves inimigas
{
  for (unsigned char i = 0; i < 3; i++)
  {
    SetTextColor(14);
    SetCursor(Player.col, Player.lin - 1); printf("        ");
    SetCursor(Player.col, Player.lin); printf("    ^   ");
    SetCursor(Player.col, Player.lin + 1); printf("  --*-- ");
    SetCursor(Player.col, Player.lin + 2); printf("    v   ");
    SetCursor(Player.col, Player.lin + 3); printf("        ");

    Sleep(80);
    SetTextColor(0);
    SetCursor(Player.col, Player.lin - 1); printf("        ");
    SetCursor(Player.col, Player.lin); printf("        ");
    SetCursor(Player.col, Player.lin + 1); printf("        ");
    SetCursor(Player.col, Player.lin + 2); printf("        ");
    SetCursor(Player.col, Player.lin + 3); printf("        ");

    Sleep(80);
    SetTextColor(12);
    SetCursor(Player.col, Player.lin - 1); printf(" \\  ^  /");
    SetCursor(Player.col, Player.lin); printf("  \\ | / ");
    SetCursor(Player.col, Player.lin + 1); printf(" ---*---");
    SetCursor(Player.col, Player.lin + 2); printf("  / |\\  ");
    SetCursor(Player.col, Player.lin + 3); printf(" /  v \\ ");

  }

  Sleep(80);
  SetTextColor(12);
  SetCursor(Player.col, Player.lin - 1); printf("         ");
  SetCursor(Player.col, Player.lin); printf("         ");
  SetCursor(Player.col, Player.lin + 1); printf("         ");
  SetCursor(Player.col, Player.lin + 2); printf("         ");
  SetCursor(Player.col, Player.lin + 3); printf("         ");
  Sleep(500);
}

// ================================== Shot Hit the Player ===============================================

bool PlayerIsDead()  // verifica se o jogador perdeu
{
  for (unsigned char i = 0; i < AMOUNT_ENEMIES; ++i) // percorre o array de inimigos
  {
    if ((Enemies[i].Shot.lin == Player.lin + 1) || (Enemies[i].Shot.lin == Player.lin + 2) || (Enemies[i].Shot.lin == Player.lin + 3) || (Enemies[i].Shot.lin == Player.lin + 4)) // se o tiro estiver na mesma linha que o inimigo...
      for (int j = 0; j < 8; ++j) // percorre todas as 10 colunas que constituem a nave inimiga
        if (Enemies[i].Shot.col == (Player.col + j)) // e estiver na mesma coluna que a nave inimiga (que deve estar "viva")...
        {
          SetCursor(Enemies[i].Shot.col, Enemies[i].Shot.lin + 1);printf(" "); // remove da tela o tiro
          PlayerExplosion(); // executa a animação de explosão
          Player.Shot.lin = Player.lin - 1;
          Enemies[i].isShoting = false;
          Player.isAlive = false;  // marca a nave inimiga  como "morta".
          return true;
        }
  }
  return false;
}

// ================================== Enemy Shot =======================================================

void EnemyShotAnimation() // realiza a animação dos tiros das naves inimigas
{
  for (unsigned char i = 0; i < AMOUNT_ENEMIES; ++i)
    if(Enemies[i].isShoting)
    {
      if(Enemies[i].Shot.lin == 0)
        Enemies[i].Shot.lin = Enemies[i].lin + 4;

      if (Enemies[i].Shot.lin + 1 < EDGE_LINE)
      {
        SetCursor(Enemies[i].Shot.col, Enemies[i].Shot.lin -  1);
        printf(" ");
        SetTextColor(4);
        SetCursor(Enemies[i].Shot.col, Enemies[i].Shot.lin); printf("*");
        Enemies[i].Shot.lin++;
      }
      else
      {
        SetCursor(Enemies[i].Shot.col, Enemies[i].Shot.lin - 1); printf(" ");
        Enemies[i].isShoting = false;
        Enemies[i].Shot.lin = 0;
      }
    }
}

//=========================== Get amount of enemies killed ========================================

int GetAmountOfDeadEnemies()
{
  unsigned char deadEnemies = 0;

  for (unsigned char i = 0; i < AMOUNT_ENEMIES; ++i)
    if(Enemies[i].isAlive == false)
      deadEnemies++;

  return deadEnemies;
}

// ================================== Choose Enemy to Shoot ==========================================

void ChooseEnemytoShoot(unsigned char stageIndex)
{
  unsigned char index = 0, shotCount = 0;
  bool isOver;

  do{
    Sleep(gameStages[actualStage].shotsFrequency);
    shotCount = 0;

    do{
      index = GetRandomNumber(0, AMOUNT_ENEMIES-1);

      if(Enemies[index].isShoting == false && Enemies[index].isAlive)
      {
        Enemies[index].isShoting = true;
        Enemies[index].Shot.col = Enemies[index].col + 5;
        shotCount++;
      }

      isOver = gameStages[stageIndex].isCompleted || Player.isAlive == false;

    }while(shotCount <= gameStages[actualStage].maxAmntShots - 1 && isOver == false);

  } while (isOver == false);

}

// ================================== Enemy Explosion =============================================

void EnemyExplosion(unsigned char i) // realiza a animação da explosão das naves inimigas
{
  SetTextColor(14);
  SetCursor(Enemies[i].col, Enemies[i].lin); printf("   \\ | /  ");
  SetCursor(Enemies[i].col, Enemies[i].lin + 1); printf("  ---*---  ");
  SetCursor(Enemies[i].col, Enemies[i].lin + 2); printf("   / | \\  ");

  Sleep(60);
  SetCursor(Enemies[i].col, Enemies[i].lin); printf("          ");
  SetCursor(Enemies[i].col, Enemies[i].lin + 1); printf("           ");
  SetCursor(Enemies[i].col, Enemies[i].lin + 2); printf("         ");

  Sleep(60);
  SetTextColor(12);
  SetCursor(Enemies[i].col, Enemies[i].lin);printf("     ^  ");
  SetCursor(Enemies[i].col, Enemies[i].lin + 1); printf("   --*--  ");
  SetCursor(Enemies[i].col, Enemies[i].lin + 2); printf("     v  ");

  Sleep(60);
  SetCursor(Enemies[i].col, Enemies[i].lin);printf("        ");
  SetCursor(Enemies[i].col, Enemies[i].lin + 1); printf("          ");
  SetCursor(Enemies[i].col, Enemies[i].lin + 2); printf("        ");
}

// ================================ Shot Hit Some Enemy ======================================

void ShotHitEnemy()  // verifica se o tiro atingiu anlguma nave inimiga
{
  for (unsigned char i = 0; i < AMOUNT_ENEMIES; ++i) // percorre o array de inimigos
  {
    if ((Player.Shot.lin == Enemies[i].lin + 1) || (Player.Shot.lin == Enemies[i].lin  + 2 )) // se o tiro estiver na mesma linha que o inimigo...
      for (int j = 0; j < 10; ++j) // percorre todas as 10 colunas que constituem a nave inimiga
        if ((Player.Shot.col == (Enemies[i].col + j)) && Enemies[i].isAlive) // e estiver na mesma coluna que a nave inimiga (que deve estar "viva")...
        {
          SetCursor(Player.Shot.col, Player.Shot.lin + 1);printf(" "); // remove da tela o tiro
          EnemyExplosion(i); // executa a animação de explosão
          Player.Shot.lin = Player.lin - 1;
          Player.isShoting = false;
          Enemies[i].isAlive = false;  // marca a nave inimiga  como "morta".
        }
  }
}

// ================================== Player Shot ==========================================

void PlayerShot() // realiza a animação do tiro da nave do jogador
{
  if (Player.Shot.lin > 2)
  {
    if((Player.lin - 1) > Player.Shot.lin)
    {
      SetCursor(Player.Shot.col, Player.Shot.lin +  1);
      printf(" ");
    }
    SetTextColor(14);
    SetCursor(Player.Shot.col, Player.Shot.lin); printf("|");

    if (Player.Shot.lin == 3)
    {SetCursor(Player.Shot.col, 3); printf(" ");}

    Player.Shot.lin--;
  }
  else
  {
    Player.Shot.lin = Player.lin - 1;
    Player.isShoting = false;
  }
}

// ========================== Player Movimentation =======================================

void PlayerMovimentation(char key) //atira e movimenta a nave para direita ou esquerda de acordo com o caractere recebido como parâmetro.
{
  if(key == 224)
    key = getch();


  if( (key == 'a' || key == 97) || (key == 'A' || key == 65) || key == 75)
  {
    if (Player.col > 3)
    {
      ClearPlayerSpaceship(); // Limpa a tela no local da nave
      Player.col -= 2; // seta a posição da nave 2 colunas para esquerda
      PrintPlayerSpaceship(); // imprime a nave na nova posição
    }
  }
  else if( (key == 'd' || key == 100) || (key == 'D' || key == 68) || key == 77)
  {
    if (Player.col < 108)
    {
      ClearPlayerSpaceship();
      Player.col += 2;
      PrintPlayerSpaceship();
    }
  }
  else if( key == ' ' || key == 32)
  {
    if(!Player.isShoting) // o usuário só pode atirar novamente quando não há outro tiro "ativo" na tela
    {
      Player.isShoting = true;
      Player.Shot.col = Player.col + 4;
    }
  }
  else if(key == VK_ESCAPE)
    MainMenu();
}

// ======================= Clean Spaceship ============================================

void ClearPlayerSpaceship()
{
  SetCursor(Player.col, Player.lin); printf("     ");
  SetCursor(Player.col, Player.lin + 1); printf("      ");
  SetCursor(Player.col, Player.lin + 2); printf("        ");
  SetCursor(Player.col, Player.lin + 3); printf("        ");
}

// ========================= Print Spaceship ==========================================

void PrintPlayerSpaceship()
{
  SetTextColor(10);
  SetCursor(Player.col, Player.lin); printf("   /\\");
  SetCursor(Player.col, Player.lin + 1); printf("  |  |");
  SetCursor(Player.col, Player.lin + 2); printf(" /|/\\|\\");
  SetCursor(Player.col, Player.lin + 3); printf("/_||||_\\");
}

// ============================ Clean Enemy ===========================================

void ClearEnemy()
{
  for (unsigned char i = 0; i < AMOUNT_ENEMIES; ++i)
  {
    SetCursor(Enemies[i].col, Enemies[i].lin); printf("       ");
    SetCursor(Enemies[i].col, Enemies[i].lin + 1); printf("          ");
    SetCursor(Enemies[i].col, Enemies[i].lin + 2); printf("          ");
  }
}

// ============================ Clean Enemy ===========================================

void PrintEnemy()
{
  SetTextColor(11);
  for (unsigned char i = 0; i < AMOUNT_ENEMIES; ++i)
    if (Enemies[i].isAlive)
    {
      SetCursor(Enemies[i].col, Enemies[i].lin); printf("   ____");
      SetCursor(Enemies[i].col, Enemies[i].lin + 1); printf("<|(____)|>");
      SetCursor(Enemies[i].col, Enemies[i].lin + 2); printf("  //  \\\\");
    }
}

// ==================================== Print Frame ================================

void PrintFrame() // Imprime a borda
{
   SetBackgroundColor(EDGE_COLOR);

  for(unsigned char i = 0 ; i < EDGE_COLUMN / 3; ++i)
    printf("|_|");

  for(unsigned char i = 2; i < EDGE_LINE; ++i)
  {
    printf("\n");
    printf("|_|");
    SetBackgroundColor(0);

    for (int j = 0; j < EDGE_COLUMN - 6; ++j)
      printf(" ");

    SetBackgroundColor(EDGE_COLOR);
    printf("|_|");
  }
  printf("\n");

  for(unsigned char i = 0 ; i < EDGE_COLUMN / 3; ++i)
    printf("|_|");

  SetBackgroundColor(0);
}

// ================================= Inicialize =========================================

void Inicialize()
{
  Player.isAlive = true;
  Player.isShoting = false;
  Player.col = (EDGE_COLUMN - 6)/2 - 4; // inicializando as posições da nave do jogador
  Player.lin = EDGE_LINE - 6;
  Player.Shot.lin = Player.lin - 1;

  Enemies[0].col = 19;
  for (unsigned char i = 0; i < AMOUNT_ENEMIES; ++i) // inicializando as posições das naves inimigas
  {
    if (i < 6)
    {
      Enemies[i].lin = 3;
      if(i > 0)
        Enemies[i].col = Enemies[i-1].col + 14;
    }
    else
    {
      Enemies[i].lin = Enemies[i-6].lin + 4;
      Enemies[i].col = Enemies[i-6].col;
    }
    Enemies[i].isShoting = false;
    Enemies[i].isAlive = true;
    Enemies[i].Shot.lin = 0;
  }
}

// ================================== Main Stream ====================================

void MainStream(unsigned char amntStages)
{
  bool toRight = true, ready;

  auto future = async(launch::async, GetKeyPressed);
  auto f = async(launch::async, ChooseEnemytoShoot, actualStage);

  system("CLEAR || CLS");
  PrintFrame();
  PrintPlayerSpaceship();

  do
  {
    Sleep(gameStages[actualStage].gameSpeed);
    ClearEnemy();

    if (Enemies[AMOUNT_ENEMIES-1].col < 106 && toRight)
    {
      if (Enemies[AMOUNT_ENEMIES-1].col == 105)
        toRight = false;

      for(unsigned char i = 0; i < AMOUNT_ENEMIES; i++)
        Enemies[i].col++;
    }
    else
    {
      if (Enemies[0].col == 5)
        toRight = true;

      for(unsigned char i = 0; i < AMOUNT_ENEMIES; i++)
        Enemies[i].col--;
    }

    PrintEnemy();
    ready = future.wait_for(chrono::seconds(0)) == future_status::ready;

    if (ready)
    {
      auto fut = async(launch::async, PlayerMovimentation, future.get());
      future = async(launch::async, GetKeyPressed);
    }

    if (Player.isShoting)
    {
      PlayerShot();
      ShotHitEnemy();
    }

  auto ft = async(launch::async, EnemyShotAnimation);
  gameStages[actualStage].isCompleted = GetAmountOfDeadEnemies() == AMOUNT_ENEMIES;

  }while(PlayerIsDead() == false && gameStages[actualStage].isCompleted == false);

  if(gameStages[actualStage].isCompleted)
  {
    actualStage++;

    if(actualStage != amntStages)
    {
      StageCompletedAnimation();
      Inicialize();
      MainStream(amntStages);
    }
    else
    {
      VictoryAnimation();
      MainMenu();
    }
  }
  else if(Player.isAlive == false)
  {
    DefeatAnimation();
    MainMenu();
  }
}

// =============================== Instructions ========================================

void Instructions()
{
  unsigned short col = 15, lin = 10;
  unsigned char cont = 0, colors[] = {2, 10, 9, 3, 7};

  system("CLEAR||CLS");
  PrintFrame();

  SetTextColor(11);
  SetCursor(col + 15, lin + 5 ); printf("Utilize as teclas direcionais para controlar a nave aliada");
  SetCursor(col + 15, lin + 6); printf("e \"espa%co\" para atirar.", 135);
  SetCursor(col + 15, lin + 7); printf("Desvie dos tiros e destrua todos os inimigos para avan%car",135);
  SetCursor(col + 15, lin + 8); printf("em sua jornada.");

  SetCursor(40, 31);
  SetTextColor(15);
  printf("Precione qualquer tecla para sair ...");

  do{
      SetTextColor(colors[cont]);
      SetCursor(col + 15, lin - 7); printf("    ____              __                       _  _");
      SetCursor(col + 15, lin - 6); printf("   /  _/____   _____ / /_ _____ __  __ _____ _//\\//  ___   _____");
      SetCursor(col + 15, lin - 5); printf("   / / / __ \\ / ___// __// ___// / / // ___// __ \\ / _ \\ / ___/");
      SetCursor(col + 15, lin - 4); printf(" _/ / / / / /(__  )/ /_ / /   / /_/ // /__ / /_/ //  __/(__  )");
      SetCursor(col + 15, lin - 3); printf("/___//_/ /_//____/ \\__//_/    \\__,_/ \\___/ \\____/ \\___//____/");
      SetCursor(col + 15, lin - 2); printf("                                     /_)");
      Sleep(300);

      cont++;
      if (cont ==  (*(&colors + 1) - colors)) //caso o vetor de core tenha sido inteiramente percorrido, o indice volta ao começo
        cont = 0;;

  }while (!kbhit());
  getch();
}

// ================================= Controls =========================================

void Controls()
{
  unsigned short col = 15, lin = 10;
  unsigned char cont = 0, colors[] = {3, 13, 9, 12, 6};

  system("CLEAR||CLS");
  PrintFrame();

  SetTextColor(7);
  SetCursor(col, lin); printf("       ____ ");
  SetCursor(col, lin + 1); printf("      ||w ||");
  SetCursor(col, lin + 2); printf("      ||__||      ");
  SetCursor(col, lin + 3); printf(" ____ |/__\\| ____ ");
  SetCursor(col, lin + 4); printf("||a ||||s ||||d ||");
  SetCursor(col, lin + 5); printf("||__||||__||||__||");
  SetCursor(col, lin + 6); printf("|/__\\||/__\\||/__\\|");

  SetCursor(col + 70, lin); printf("       ____ ");
  SetCursor(col + 70, lin + 1); printf("      ||^ ||");
  SetCursor(col + 70, lin + 2); printf("      ||__||      ");
  SetCursor(col + 70, lin + 3); printf(" ____ |/__\\| ____ ");
  SetCursor(col + 70, lin + 4); printf("||< ||||v ||||> ||");
  SetCursor(col + 70, lin + 5); printf("||__||||__||||__||");
  SetCursor(col + 70, lin + 6); printf("|/__\\||/__\\||/__\\|");

  SetCursor(col + 35, lin + 8); printf(" _________________ ");
  SetCursor(col + 35, lin + 9); printf("||   __________  ||");
  SetCursor(col + 35, lin + 10); printf("||_______________||");
  SetCursor(col + 35, lin + 11); printf("|/_______________\\|");

  SetTextColor(14);
  //SetCursor(col + 22, lin + 12); printf("ATIRAR");
  SetCursor(col + 22, lin + 5); printf("<---------- DIREITA");
  SetCursor(col + 48, lin + 5); printf("ESQUERDA -------->");

  SetCursor(col + 44, lin + 13); printf("^");
  SetCursor(col + 44, lin + 14); printf("|");
  SetCursor(col + 41, lin + 16); printf("ATIRAR");

  SetCursor(col + 3, lin + 11); printf("_ ESQUERDA");
  SetCursor(col + 2, lin + 8); printf("^");
  for (unsigned char i = 0; i < 3; i++)
  {
    SetCursor(col + 2, lin + 9 + i); printf("|");
  }

  SetCursor(col + 76, lin + 11); printf("DIREITA _");
  SetCursor(col + 85, lin + 8); printf("^");
  for (unsigned char i = 0; i < 3; i++)
  {
    SetCursor(col + 85, lin + 9 + i); printf("|");
  }

  SetCursor(40, 31);
  SetTextColor(15);
  printf("Precione qualquer tecla para sair ...");

  do{
      SetTextColor(colors[cont]);
      SetCursor(col + 18, lin - 7); printf("   ______               __                __");
      SetCursor(col + 18, lin - 6); printf("  / ____/____   ____   / /_ _____ ____   / /___   _____");
      SetCursor(col + 18, lin - 5); printf(" / /    / __ \\ / __ \\ / __// ___// __ \\ / // _ \\ / ___/");
      SetCursor(col + 18, lin - 4); printf("/ /___ / /_/ // / / // /_ / /   / /_/ // //  __/(__  ) ");
      SetCursor(col + 18, lin - 3); printf("\\____/ \\____//_/ /_/ \\__//_/    \\____//_/ \\___//____/");
      Sleep(300);

      cont++;
      if (cont ==  (*(&colors + 1) - colors)) //caso o vetor de core tenha sido inteiramente percorrido, o indice volta ao começo
        cont = 0;;

  }while (!kbhit());
  getch();
}

// ========================== Print Menu Options =====================================

void PrintMenuOptions(unsigned short int previousIndex, unsigned short int arrowIndex)
{
  unsigned short int arrow = arrowIndex * 2 + 20;
  unsigned short int whereToClean = previousIndex * 2 + 20;

  SetCursor(50, whereToClean); printf("  ");

  SetTextColor(9);
  SetCursor(50, arrow); printf("|>");
  SetTextColor(2);
  SetCursor(53,22); printf(" JOGAR");
  SetCursor(53,24); printf(" CONTROLES");
  SetCursor(53,26); printf(" INSTRU%c%cES", 128, 229);
  SetCursor(53,28); printf(" SAIR");
  SetTextColor(3);
  SetCursor(43,34); printf("Produzido por Valdecir Raimundo");
}

// ============================== Main Menu ==========================================

void MainMenu()
{
  unsigned short int lin = 5, col = 22;
  unsigned char cont = 0, menuIndex = 1, optionsAmount = 4, key = 0, amntStages = (*(&gameStages + 1) - gameStages);
  unsigned char colors[] = {3, 13, 9, 14, 7};

  actualStage = 0;

  for (unsigned char i = 0; i < amntStages; ++i)
    gameStages[i].isCompleted = false;

  system("CLEAR || CLS");
  Inicialize();
  PrintFrame();
  PrintMenuOptions(optionsAmount, menuIndex);

  do{
    do{
        SetTextColor(colors[cont]);
        SetCursor(col, lin); printf("     _______..______      ___       ______  _______       ");
        SetCursor(col, lin + 1); printf("    /       ||   _  \\    /   \\     /      ||   ____|");
        SetCursor(col, lin + 2); printf("   |   (----`|  |_)  |  /  ^  \\   |  ,----'|  |__");
        SetCursor(col, lin + 3); printf("    \\   \\    |   ___/  /  /_\\  \\  |  |     |   __|");
        SetCursor(col, lin + 4); printf(".----)   |   |  |     /  _____  \\ |  `----.|  |____");
        SetCursor(col, lin + 5); printf("|____.__/  __. ____  /_____  ____\\ \\____________________ .______          _______.");
        SetCursor(col, lin + 6); printf("|  | |  \\ |  | \\   \\  /   / /   \\     |       \\ |   ____||   _  \\        /       |");
        SetCursor(col, lin + 7); printf("|  | |   \\|  |  \\   \\/   / /  ^  \\    |  .--.  ||  |__   |  |_)  |      |   (----`");
        SetCursor(col, lin + 8); printf("|  | |  . `  |   \\      / /  /_\\  \\   |  |  |  ||   __|  |      /        \\   \\ ");
        SetCursor(col, lin + 9); printf("|  | |  |\\   |    \\    / /  _____  \\  |  '--'  ||  |____ |  |\\  \\----.----)   |");
        SetCursor(col, lin + 10); printf("|__| |__| \\__|     \\__/ /__/     \\__\\ |_______/ |_______|| _| `._____|_______/");
        Sleep(200);

        cont++;
        if (cont ==  (*(&colors + 1) - colors)) //caso o vetor de core tenha sido inteiramente percorrido, o indice volta ao começo
          cont = 0;;

    }while (!kbhit());

    key = getch();
    if(key == 224)
      key = getch();

    if (key == 'w' || key == 'W' || key == 72)
    {
      if(menuIndex == 1 )
      {
        menuIndex = optionsAmount;
        PrintMenuOptions(1, menuIndex);
      }
      else
      {
        menuIndex--;
        PrintMenuOptions(menuIndex + 1, menuIndex);
      }
    }
    else if (key == 's' || key == 'S' || key == 80)
    {
      if(menuIndex == optionsAmount )
      {
        menuIndex = 1;
        PrintMenuOptions(optionsAmount, menuIndex);
      }
      else
      {
        menuIndex++;
        PrintMenuOptions(menuIndex - 1, menuIndex);
      }
    }
    else if(key == 13 || ((key == 'd' || key == 100) || (key == 'D' || key == 68) || key == 77))
    {
      switch (menuIndex)
      {
        case 1:
          MainStream(amntStages);
          break;
        case 2:
          Controls();
          system("CLEAR || CLS");
          PrintFrame();
          PrintMenuOptions(optionsAmount, menuIndex);
          break;
        case 3:
          Instructions();
          system("CLEAR || CLS");
          PrintFrame();
          PrintMenuOptions(optionsAmount, menuIndex);
          break;
        case 4:
          key = VK_ESCAPE;
      }
    }

  }while(key != VK_ESCAPE);

  system("clear||cls");
  SetTextColor(9);
  SetCursor(47, 16); printf("Obrigado por jogar !!!");
  SetCursor(41, 17); printf("Precione qualquer tecla para sair ...");
  SetTextColor(0);
  exit(0);
}

// ============================== Main ==========================================

int main()
{
  system("MODE con cols=120 lines=37"); // define o tamanho do terminal
  HideCursor(); // deixa o cursor invisível
  generator.seed((unsigned int)time(0)); //utilizando o valor retornado pela função time(0) para inicializar o gerador de números aleatórios.

  MainMenu();

  return 0;
}
