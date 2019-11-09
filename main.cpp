
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <future>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <random>
#include <ctime>

const int FrameCol = 120, FrameLin = 37, EnemiesCol = 6, EnemiesLin = 3, AmntEnemies = 18; // indica os limites das bordas e a quantidade de naves inimigas

/*int Player.Col = 55, Player.Shot.Col = Player.Col + 4, Player.Shot.Lin = 0;
int EnemyCol[EnemiesCol] = {19, 33, 47, 61, 75, 89}, EnemyLin[EnemiesLin] = {3, 7, 11}, DeadEnemies[DeadIndex] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, EnemyShot[AmntEnemiesShot][2];*/

typedef struct
{
  int Col;
  int Lin;
  bool Is_Alive = true;
  bool Is_Shoting;

  struct Player_Shot
  {
    int Col;
    int Lin;
  }Shot;

}SpaceShip;

SpaceShip Player;
SpaceShip Enemies[AmntEnemies];
std::mt19937 generator;

void setcursor(int x, int y);
void PrintFrame();
char GetKey();
void EnemyExplosion(int i);
void Main_Stream(int Enemy);
void Clean_Enemy();
void Print_Player_Spaceship();
void Clear_Player_Spaceship();
void Player_Shot();
void Player_Movimentation(char Key);
void DefeatAnimation();

// ===================================== Set Cursor Position ==========================================

void setcursor(int x, int y) // posiciona o cursor de acordo com a cordenada coluna X linha
{
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x,y});
}

// ==================================== Print Frame ===================================================

void PrintFrame() // Imprime a borda
{
   textbackground(1);

   for(int i = 0 ; i < FrameCol / 3; ++i)
      printf("|_|");

   for(int i = 2; i < FrameLin; ++i)
   {
     printf("\n");
     printf("|_|");
     textbackground(0);

     for (int j = 0; j < FrameCol - 6; ++j)
          printf(" ");

     textbackground(1);
     printf("|_|");
   }
   printf("\n");

   for(int i = 0 ; i < FrameCol / 3; ++i)
      printf("|_|");

   textbackground(0);
}

// ================================== Get Key ========================================================

char GetKey() // retorna o próximo caractere digitado.
{
  char c = 0;

  do{ c = getch(); }while(c == 0);

  return c;
}

// ================================= Player Movimentation =========================================

void Player_Movimentation(char Key) //atira e movimenta a nave para direita ou esquerda de acordo com o caractere recebido como parâmetro.
{
   switch (Key)
   {
      case 97:
        if (Player.Col > 3)
        {
          Clear_Player_Spaceship(); // Limpa a tela no local da nave
          Player.Col -= 2; // seta a posição da nave 2 colunas para esquerda
          Print_Player_Spaceship(); // imprime a nave na nova posição
        }
        break;
      case 100:
        if (Player.Col < 108)
        {
          Clear_Player_Spaceship();
          Player.Col += 2;
          Print_Player_Spaceship();
        }
        break;
      case 32:
        if(!Player.Is_Shoting) // o usuário só pode atirar novamente quando não há outro tiro "ativo" na tela
        {
          Player.Is_Shoting = true;
          Player.Shot.Col = Player.Col + 4;
        }
        break;
   }
}

// ================================== Player Shot =============================================================

void Player_Shot() // realiza a animação do tiro da nave do jogador
{
  if (Player.Shot.Lin > 2)
  {
    if((Player.Lin - 1) > Player.Shot.Lin)
    {
      setcursor(Player.Shot.Col, Player.Shot.Lin +  1);
      printf(" ");
    }
    textcolor(14);
    setcursor(Player.Shot.Col, Player.Shot.Lin); printf("|");
    textcolor(0);

    if (Player.Shot.Lin == 3)
      setcursor(Player.Shot.Col, 3); printf(" ");

    Player.Shot.Lin--;
  }
  else
  {
    Player.Shot.Lin = Player.Lin - 1;
    Player.Is_Shoting = false;
  }
}

// ================================== Shot Hit the Target ================================================

void Shot_Hit_Enemy()  // verifica se o tiro atingiu anlguma nave inimiga
{
  for (int i = 0; i < AmntEnemies; ++i) // percorre o array de inimigos
  {
    if ((Player.Shot.Lin == Enemies[i].Lin + 1) || (Player.Shot.Lin == Enemies[i].Lin  + 2 )) // se o tiro estiver na mesma linha que o inimigo...
      for (int j = 0; j < 10; ++j) // percorre todas as 10 colunas que constituem a nave inimiga
        if ((Player.Shot.Col == (Enemies[i].Col + j)) && Enemies[i].Is_Alive) // e estiver na mesma coluna que a nave inimiga (que deve estar "viva")...
        {
          textcolor(0);
          setcursor(Player.Shot.Col, Player.Shot.Lin + 1);printf(" "); // remove da tela o tiro
          EnemyExplosion(i); // executa a animação de explosão
          Player.Shot.Lin = Player.Lin - 1;
          Player.Is_Shoting = false;
          Enemies[i].Is_Alive = false;  // marca a nave inimiga  como "morta".
        }
  }
}

// ================================== Enemy Explosion ===================================================

void EnemyExplosion(int i) // realiza a animação da explosão das naves inimigas
{
  textcolor(4);
  setcursor(Enemies[i].Col, Enemies[i].Lin); printf("   \\ | /  ");
  setcursor(Enemies[i].Col, Enemies[i].Lin + 1); printf("  ---*---  ");
  setcursor(Enemies[i].Col, Enemies[i].Lin + 2); printf("   / | \\  ");

  Sleep(60);
  textcolor(0);
  setcursor(Enemies[i].Col, Enemies[i].Lin); printf("          ");
  setcursor(Enemies[i].Col, Enemies[i].Lin + 1); printf("           ");
  setcursor(Enemies[i].Col, Enemies[i].Lin + 2); printf("         ");

  Sleep(60);
  textcolor(12);
  setcursor(Enemies[i].Col, Enemies[i].Lin);printf("     ^  ");
  setcursor(Enemies[i].Col, Enemies[i].Lin + 1); printf("   --*--  ");
  setcursor(Enemies[i].Col, Enemies[i].Lin + 2); printf("     v  ");

  Sleep(60);
  textcolor(0);
  setcursor(Enemies[i].Col, Enemies[i].Lin);printf("        ");
  setcursor(Enemies[i].Col, Enemies[i].Lin + 1); printf("          ");
  setcursor(Enemies[i].Col, Enemies[i].Lin + 2); printf("        ");
}

//======================================= Get Random Number ===================================================================

int GetRandomNumber(int Min, int Max)
{
  std::uniform_int_distribution<uint32_t> dice(Min, Max);
  return dice(generator);
}

// ================================== Choose Enemy to Shoot ====================================================

void Choose_Enemy_to_Shoot()
{
  int DeadEnemies = 0, Index = 0, ShotCount = 0;

  do{

    do{
      Index = GetRandomNumber(0, AmntEnemies-1);

      if(!Enemies[Index].Is_Shoting && Enemies[Index].Is_Alive)
      {
        Enemies[Index].Is_Shoting = true;
        Enemies[Index].Shot.Col = Enemies[Index].Col + 5;
        ShotCount++;
      }
    }while(ShotCount <= 2);

    ShotCount = 0;

    for (int i = 0; i < AmntEnemies; ++i)
      if(!Enemies[i].Is_Alive)
        DeadEnemies++;

    Sleep(4000);
  } while (DeadEnemies != AmntEnemies);
}

// ================================== Enemy Shot ==============================================================

void EnemyShot(int i) // realiza a animação dos tiro da nave inimiga
{

  if (Enemies[i].Shot.Lin + 1 < FrameLin)
  {
    if(Enemies[i].Shot.Lin > 14)
    {
      setcursor(Enemies[i].Shot.Col, Enemies[i].Shot.Lin -  1);
      printf(" ");
    }
    textcolor(13);
    setcursor(Enemies[i].Shot.Col, Enemies[i].Shot.Lin); printf("*");
    textcolor(0);
    Enemies[i].Shot.Lin++;
  }
  else
  {
    setcursor(Enemies[i].Shot.Col, Enemies[i].Shot.Lin - 1); printf(" ");
    Enemies[i].Shot.Lin = 14;
    Enemies[i].Is_Shoting = false;
  }
}

// ================================== Player Explosion ===================================================

void PlayerExplosion() // realiza a animação da explosão das naves inimigas
{
 textcolor(4);
 setcursor(Player.Col, Player.Lin - 1); printf("        ");
 setcursor(Player.Col, Player.Lin); printf("    ^   ");
 setcursor(Player.Col, Player.Lin + 1); printf("  --*-- ");
 setcursor(Player.Col, Player.Lin + 2); printf("    v   ");
 setcursor(Player.Col, Player.Lin + 3); printf("        ");

 Sleep(80);
 textcolor(0);
 setcursor(Player.Col, Player.Lin - 1); printf("        ");
 setcursor(Player.Col, Player.Lin); printf("        ");
 setcursor(Player.Col, Player.Lin + 1); printf("        ");
 setcursor(Player.Col, Player.Lin + 2); printf("        ");
 setcursor(Player.Col, Player.Lin + 3); printf("        ");

 Sleep(80);
 textcolor(12);
 setcursor(Player.Col, Player.Lin - 1); printf(" \\  ^  /");
 setcursor(Player.Col, Player.Lin); printf("  \\ | / ");
 setcursor(Player.Col, Player.Lin + 1); printf(" ---*---");
 setcursor(Player.Col, Player.Lin + 2); printf("  / |\\  ");
 setcursor(Player.Col, Player.Lin + 3); printf(" /  v \\ ");

 Sleep(80);
 textcolor(0);
 setcursor(Player.Col, Player.Lin - 1); printf("        ");
 setcursor(Player.Col, Player.Lin); printf("        ");
 setcursor(Player.Col, Player.Lin + 1); printf("        ");
 setcursor(Player.Col, Player.Lin + 2); printf("        ");
 setcursor(Player.Col, Player.Lin + 3); printf("        ");
}

// ================================== Shot Hit the Player ===============================================

bool Shot_Hit_Player()  // verifica se o tiro atingiu anlguma nave inimiga
{
  for (int i = 0; i < AmntEnemies; ++i) // percorre o array de inimigos
  {
    if ((Enemies[i].Shot.Lin == Player.Lin + 1) || (Enemies[i].Shot.Lin == Player.Lin + 2) || (Enemies[i].Shot.Lin == Player.Lin + 3) || (Enemies[i].Shot.Lin == Player.Lin + 4)) // se o tiro estiver na mesma linha que o inimigo...
      for (int j = 0; j < 8; ++j) // percorre todas as 10 colunas que constituem a nave inimiga
        if (Enemies[i].Shot.Col == (Player.Col + j)) // e estiver na mesma coluna que a nave inimiga (que deve estar "viva")...
        {
          textcolor(0)
;          setcursor(Enemies[i].Shot.Col, Enemies[i].Shot.Lin + 1);printf(" "); // remove da tela o tiro
          PlayerExplosion(); // executa a animação de explosão
          Player.Shot.Lin = Player.Lin - 1;
          Enemies[i].Is_Shoting = false;
          Player.Is_Alive = false;  // marca a nave inimiga  como "morta".
          return true;
        }
  }
  return false;
}

// ================================== Main Stream ===================================================

void Main_Stream()
{
  bool toRight = true, Ready;
  char key;

  auto future = std::async(std::launch::async, GetKey);
  auto f = std::async(std::launch::async, Choose_Enemy_to_Shoot);

  do
  {
    Sleep(50);
    Clean_Enemy();
    textcolor(11);

    if (Enemies[AmntEnemies-1].Col < 106 && toRight)
    {
      if (Enemies[AmntEnemies-1].Col == 105)
        toRight = false;

      for(int i = 0; i < AmntEnemies; i++)
        Enemies[i].Col++;
    }
    else
    {
      if (Enemies[0].Col == 5)
        toRight = true;

      for(int i = 0; i < AmntEnemies; i++)
        Enemies[i].Col--;
    }

    for (int i = 0; i < AmntEnemies; ++i)
      if (Enemies[i].Is_Alive)
      {
        setcursor(Enemies[i].Col, Enemies[i].Lin);printf("   ____");
        setcursor(Enemies[i].Col, Enemies[i].Lin + 1);printf("<|(____)|>");
        setcursor(Enemies[i].Col, Enemies[i].Lin + 2);printf("  //  \\\\");
      }

    Ready = future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;

    if (Ready)
    {
      key = future.get();
      Player_Movimentation(key);
      future = std::async(std::launch::async, GetKey);
    }

    if (Player.Is_Shoting)
    {
      Player_Shot();
      std::async(std::launch::async, Shot_Hit_Enemy);
    }

    for (int i = 0; i < AmntEnemies; ++i)
      if(Enemies[i].Is_Shoting)
         std::async(std::launch::async, EnemyShot, i);

  }while(key != 27 && Shot_Hit_Player() == false);

  DefeatAnimation();

}

// ================================== Clean Enemy ==================================================

void Clean_Enemy()
{
  textcolor(0);
  for (int i = 0; i < AmntEnemies; ++i)
  {
    setcursor(Enemies[i].Col, Enemies[i].Lin); printf("       ");
    setcursor(Enemies[i].Col, Enemies[i].Lin + 1); printf("          ");
    setcursor(Enemies[i].Col, Enemies[i].Lin + 2); printf("          ");
  }
}

// ================================== Print SpaceShip ==============================================

void Print_Player_Spaceship()
{
   textcolor(10);
   setcursor(Player.Col, Player.Lin); printf("   /\\");
   setcursor(Player.Col, Player.Lin + 1); printf("  |  |");
   setcursor(Player.Col, Player.Lin + 2); printf(" /|/\\|\\");
   setcursor(Player.Col, Player.Lin + 3); printf("/_||||_\\");
}

// ================================== Clean SpaceShip =============================================

void Clear_Player_Spaceship()
{
  textcolor(0);
  setcursor(Player.Col, Player.Lin); printf("     ");
  setcursor(Player.Col, Player.Lin + 1); printf("      ");
  setcursor(Player.Col, Player.Lin + 2); printf("        ");
  setcursor(Player.Col, Player.Lin + 3); printf("        ");
}

// ================================== Defeat Animation ===================================================

void DefeatAnimation()
{
  int Lin = 12, Col = 8, Cont = 0;
  int Colors[] = {5, 12, 11, 6, 3};

  system("clear||cls");
  PrintFrame();

  for(int i = 0; i<30; i++)
  {
    textcolor(Colors[Cont]);
    setcursor(Col, Lin);  printf(" __     __                                   _______                          __                      __ ");
    setcursor(Col, Lin + 1);  printf("|  \\   |  \\                                 |       \\                        |  \\                    |  \\");
    setcursor(Col, Lin + 2); printf("| $$   | $$ ______   _______  ______        | $$$$$$$\\ ______   ______   ____| $$ ______  __    __   | $$");
    setcursor(Col, Lin + 3); printf("| $$   | $$/      \\ /       \\/      \\       | $$__/ $$/      \\ /      \\ /      $$/      \\|  \\  |  \\  | $$");
    setcursor(Col, Lin + 4); printf(" \\$$\\ /  $|  $$$$$$|  $$$$$$|  $$$$$$\\      | $$    $|  $$$$$$|  $$$$$$|  $$$$$$|  $$$$$$| $$  | $$  | $$");
    setcursor(Col, Lin + 5); printf("  \\$$\\  $$| $$  | $| $$     | $$    $$      | $$$$$$$| $$    $| $$   \\$| $$  | $| $$    $| $$  | $$   \\$$");
    setcursor(Col, Lin + 6); printf("   \\$$ $$ | $$__/ $| $$_____| $$$$$$$$      | $$     | $$$$$$$| $$     | $$__| $| $$$$$$$| $$__/ $$   __ ");
    setcursor(Col, Lin + 7); printf("    \\$$$   \\$$    $$\\$$     \\\\$$     \\      | $$      \\$$     | $$      \\$$    $$\\$$     \\\\$$    $$  |  \\");
    setcursor(Col, Lin + 8); printf("     \\$     \\$$$$$$  \\$$$$$$$ \\$$$$$$$       \\$$       \\$$$$$$$\\$$       \\$$$$$$$ \\$$$$$$$ \\$$$$$$    \\$$");

    Cont++;
    if (Cont == 4)
      Cont = 0;

    Sleep(200);
  }
    setcursor(40, 30);
    textcolor(15);
    system("PAUSE");
}
// ================================= Main =========================================================

int main()
{
  system("MODE con cols=138 lines=38"); // define o tamanho do terminal
  _setcursortype(_NOCURSOR); // deixa o cursor invisível

  Player.Col = (FrameCol - 6)/2 - 4; // inicializando as posições da nave do jogador
  Player.Lin = FrameLin - 6;
  Player.Shot.Lin = Player.Lin - 1;

  Enemies[0].Col = 19;
  for (int i = 0; i < AmntEnemies; ++i) // inicializando as posições das naves inimigas
  {
    if (i<6)
    {
      Enemies[i].Lin = 3;
      if(i>0)
        Enemies[i].Col = Enemies[i-1].Col + 14;
    }
    else
    {
      Enemies[i].Lin = Enemies[i-6].Lin + 4;
      Enemies[i].Col = Enemies[i-6].Col;
    }
    Enemies[i].Shot.Lin = 14;
  }
  generator.seed((unsigned int)std::time(0));
  PrintFrame();
  Print_Player_Spaceship();
  DefeatAnimation();
  Main_Stream();

  system("clear||cls");
  printf("Fim de Jogo\n");
  getch();

  return 0;
}