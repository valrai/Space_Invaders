
#include <stdio.h>
#include <stdlib.h>
#include <conio.c>
#include <windows.h>
#include <time.h>
#include <future>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <random>
#include <ctime>

const  unsigned short int EDGE_COLUMN = 120, EDGE_LINE = 37, AMOUNT_ENEMIES = 18; // indica os limites das bordas e a quantidade de naves inimigas

unsigned char actualStage;
bool keepSearching;

typedef struct // criando struct que representará as fases do jogo
{
  unsigned char maxAmntShots;
  unsigned short int shotsFrequency;
  unsigned char gameSpeed;
  bool stageIsCompleted;

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
  {2, 2000, 50, false},
  {3, 2000, 45, false},
  {3, 1500, 40, false},
  {3, 1000, 40, false},
  {3, 1000, 35, false},
  {3, 1000, 30, false},
  {3,  800, 30, false}
};

Spaceship Player; // criando a nave do jogador
Spaceship Enemies[AMOUNT_ENEMIES]; // criando um array de naves inimigas
std::mt19937 generator; //instanciando gerador de números aleatórios

void SetCursor(unsigned short int x,unsigned short int y);
unsigned char GetKeyPressed();
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
void PlayerMovimentation(unsigned char key);
void ClearPlayerSpaceship();
void PrintPlayerSpaceship();
void ClearEnemy();
void PrintEnemy();
void MainStream();
void PrintMenuOptions(unsigned short int previousIndex, unsigned short int arrowIndex);
void PrintFrame();
void Inicialize();
void MainMenu();

// ===================================== Set Cursor Position ==========================================

void SetCursor(unsigned short int x, unsigned short int y) // posiciona o cursor de acordo com a cordenada coluna X linha
{
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x,y});
}

// ================================== Get key =========================================================

unsigned char GetKeyPressed() // retorna o próximo caractere digitado.
{
  unsigned char key = 0;

  do{ key = getch(); }while(key == 0 && keepSearching);

  return key;
}

//======================================= Get Random Number ===========================================

int GetRandomNumber(int Min, int Max)
{
  std::uniform_int_distribution<uint32_t> dice(Min, Max);
  return dice(generator);
}

// ================================== Stage Completed Animation ===============================================

void StageCompletedAnimation()
{
  unsigned short int lin = 5, col = 28, cont = 0;
  unsigned char colors[] = {3, 13, 9, 14, 7};

  system("clear||cls");
  PrintFrame();

  switch(actualStage)
  {
    case 1:
      for(unsigned char i = 0; i < 15; i++)
      {
        textcolor(colors[cont]);
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
        textcolor(colors[cont]);
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
        textcolor(colors[cont]);
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
        textcolor(colors[cont]);
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
        textcolor(colors[cont]);
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
        textcolor(colors[cont]);
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

  SetCursor(40, 30); textcolor(15);
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
    textcolor(colors[cont]);
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
  SetCursor(40, 30); textcolor(15);
  system("PAUSE");
}

// ========================================== Defeat Animation ===========================================

void DefeatAnimation()
{
  unsigned short int lin = 5, col = 30;
  unsigned char cont = 0;
  unsigned char colors[] = {5, 12, 11, 6, 3};

  system("clear||cls");
  PrintFrame();

  for(unsigned char i = 0; i < 15; i++)
  {
    textcolor(colors[cont]);
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
    if (cont == 4)
      cont = 0;

    Sleep(200);
  }
    SetCursor(40, 30); textcolor(15);
    system("PAUSE");
}

// ================================== Player Explosion ===================================================

void PlayerExplosion() // realiza a animação da explosão das naves inimigas
{
 textcolor(4);
 SetCursor(Player.col, Player.lin - 1); printf("        ");
 SetCursor(Player.col, Player.lin); printf("    ^   ");
 SetCursor(Player.col, Player.lin + 1); printf("  --*-- ");
 SetCursor(Player.col, Player.lin + 2); printf("    v   ");
 SetCursor(Player.col, Player.lin + 3); printf("        ");

 Sleep(80);
 SetCursor(Player.col, Player.lin - 1); printf("        ");
 SetCursor(Player.col, Player.lin); printf("        ");
 SetCursor(Player.col, Player.lin + 1); printf("        ");
 SetCursor(Player.col, Player.lin + 2); printf("        ");
 SetCursor(Player.col, Player.lin + 3); printf("        ");

 Sleep(80);
 textcolor(12);
 SetCursor(Player.col, Player.lin - 1); printf(" \\  ^  /");
 SetCursor(Player.col, Player.lin); printf("  \\ | / ");
 SetCursor(Player.col, Player.lin + 1); printf(" ---*---");
 SetCursor(Player.col, Player.lin + 2); printf("  / |\\  ");
 SetCursor(Player.col, Player.lin + 3); printf(" /  v \\ ");

 Sleep(80);
 SetCursor(Player.col, Player.lin - 1); printf("        ");
 SetCursor(Player.col, Player.lin); printf("        ");
 SetCursor(Player.col, Player.lin + 1); printf("        ");
 SetCursor(Player.col, Player.lin + 2); printf("        ");
 SetCursor(Player.col, Player.lin + 3); printf("        ");
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
      if (Enemies[i].Shot.lin + 1 < EDGE_LINE)
      {
        if(Enemies[i].Shot.lin > 14)
        {
          SetCursor(Enemies[i].Shot.col, Enemies[i].Shot.lin -  1);
          printf(" ");
        }
        textcolor(13);
        SetCursor(Enemies[i].Shot.col, Enemies[i].Shot.lin); printf("*");

        Enemies[i].Shot.lin++;
      }
      else
      {
        SetCursor(Enemies[i].Shot.col, Enemies[i].Shot.lin - 1); printf(" ");
        Enemies[i].Shot.lin = 14;
        Enemies[i].isShoting = false;
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

      isOver = gameStages[stageIndex].stageIsCompleted || Player.isAlive == false;

    }while(shotCount <= gameStages[actualStage].maxAmntShots - 1 && isOver == false);

  } while (isOver == false);

}

// ================================== Enemy Explosion =============================================

void EnemyExplosion(unsigned char i) // realiza a animação da explosão das naves inimigas
{
  textcolor(4);
  SetCursor(Enemies[i].col, Enemies[i].lin); printf("   \\ | /  ");
  SetCursor(Enemies[i].col, Enemies[i].lin + 1); printf("  ---*---  ");
  SetCursor(Enemies[i].col, Enemies[i].lin + 2); printf("   / | \\  ");

  Sleep(60);
  SetCursor(Enemies[i].col, Enemies[i].lin); printf("          ");
  SetCursor(Enemies[i].col, Enemies[i].lin + 1); printf("           ");
  SetCursor(Enemies[i].col, Enemies[i].lin + 2); printf("         ");

  Sleep(60);
  textcolor(12);
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
    textcolor(14);
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

void PlayerMovimentation(unsigned char key) //atira e movimenta a nave para direita ou esquerda de acordo com o caractere recebido como parâmetro.
{
  if( (key == 'a' || key == 97) || (key == 'A' || key == 65))
  {
    if (Player.col > 3)
    {
      ClearPlayerSpaceship(); // Limpa a tela no local da nave
      Player.col -= 2; // seta a posição da nave 2 colunas para esquerda
      PrintPlayerSpaceship(); // imprime a nave na nova posição
    }
  }
  else if( (key == 'd' || key == 100) || (key == 'D' || key == 68))
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
  textcolor(10);
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
  for (unsigned char i = 0; i < AMOUNT_ENEMIES; ++i)
    if (Enemies[i].isAlive)
    {
      SetCursor(Enemies[i].col, Enemies[i].lin); printf("   ____");
      SetCursor(Enemies[i].col, Enemies[i].lin + 1); printf("<|(____)|>");
      SetCursor(Enemies[i].col, Enemies[i].lin + 2); printf("  //  \\\\");
    }
}

// ========================== Print Menu Options =====================================

void PrintMenuOptions(unsigned short int previousIndex, unsigned short int arrowIndex)
{
  unsigned short int arrow = arrowIndex * 2 + 20;
  unsigned short int whereToClean = previousIndex * 2 + 20;

  SetCursor(50, whereToClean); printf("  ");

  textcolor(9);
  SetCursor(50, arrow); printf("|>");
  textcolor(2);
  SetCursor(53,22); printf(" JOGAR");
  SetCursor(53,24); printf(" CONTROLES");
  SetCursor(53,26); printf(" INSTRUCOES");
  SetCursor(53,28); printf(" SAIR");
  textcolor(3);
  SetCursor(43,34); printf("Produzido por Valdecir Raimundo");
}

// ==================================== Print Frame ================================

void PrintFrame() // Imprime a borda
{
  textbackground(1);

  for(unsigned char i = 0 ; i < EDGE_COLUMN / 3; ++i)
    printf("|_|");

  for(unsigned char i = 2; i < EDGE_LINE; ++i)
  {
    printf("\n");
    printf("|_|");
    textbackground(0);

    for (int j = 0; j < EDGE_COLUMN - 6; ++j)
      printf(" ");

    textbackground(1);
    printf("|_|");
  }
  printf("\n");

  for(unsigned char i = 0 ; i < EDGE_COLUMN / 3; ++i)
    printf("|_|");

  textbackground(0);
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
    Enemies[i].Shot.lin = 14;
    Enemies[i].isShoting = false;
    Enemies[i].isAlive = true;
  }
}

// ================================== Main Stream ====================================

void MainStream(unsigned char amntStages)
{

  bool toRight = true, ready;

  keepSearching = true;

  auto future = std::async(std::launch::async, GetKeyPressed);
  auto f = std::async(std::launch::async, ChooseEnemytoShoot, actualStage);

  system("CLEAR || CLS");
  PrintFrame();
  PrintPlayerSpaceship();

  do
  {
    Sleep(gameStages[actualStage].gameSpeed);
    ClearEnemy();
    textcolor(11);

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
    ready = future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;

    if (ready)
    {
      auto fut = std::async(std::launch::async, PlayerMovimentation, future.get());
      future = std::async(std::launch::async, GetKeyPressed);
    }

    if (Player.isShoting)
    {
      PlayerShot();
      ShotHitEnemy();
    }

  auto ft = std::async(std::launch::async, EnemyShotAnimation);
  gameStages[actualStage].stageIsCompleted = GetAmountOfDeadEnemies() == AMOUNT_ENEMIES;

  }while(PlayerIsDead() == false && gameStages[actualStage].stageIsCompleted == false);

  keepSearching = false;
  if(gameStages[actualStage].stageIsCompleted)
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

// ============================== Main Menu ==========================================

void MainMenu()
{
  unsigned short int lin = 5, col = 22;
  unsigned char cont = 0, menuIndex = 1, optionsAmount = 4, key = 0, amntStages = (*(&gameStages + 1) - gameStages);
  unsigned char colors[] = {3, 13, 9, 14, 7};

  actualStage = 0;
  keepSearching = true;

  for (unsigned char i = 0; i < amntStages; ++i)
    gameStages[i].stageIsCompleted = false;

  system("CLEAR || CLS");
  Inicialize();
  PrintFrame();
  PrintMenuOptions(optionsAmount, menuIndex);

  do{
    do{
        textcolor(colors[cont]);
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

    if (key == 'w' || key == 'W')
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
    else if (key == 's' || key == 'S' )
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
    else if(key == 13)
    {
      switch (menuIndex)
      {
        case 1:
          MainStream(amntStages);
          break;
        case 2:
          break;
        case 3:
          break;
        case 4:
          system("clear||cls");
          SetCursor(50, 15); printf("Obrigado por jogar !!!");
          SetCursor(40, 17); printf("Precione qualquer tecla para continuar ...");
          textcolor(0);
          exit(0);
          break;
      }
    }

  }while(key != VK_ESCAPE);
}

// ============================== Main ==========================================

int main()
{
  system("MODE con cols=138 lines=38"); // define o tamanho do terminal
  _setcursortype(_NOCURSOR); // deixa o cursor invisível
  generator.seed((unsigned int)std::time(0)); //utilizando o valor retornado pela função time(0) para inicializar o gerador de números aleatórios.

  MainMenu();

  return 0;
}
