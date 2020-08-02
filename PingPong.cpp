#include <iostream>
#include <time.h>
#include <string>
#include <cstring>
#include "mbed.h"
//#include <conio.h>
#include "stm32f413h_discovery_ts.h"
#include "stm32f413h_discovery_lcd.h"

TS_StateTypeDef TS_State = { 0 };
DigitalIn g(p5);
DigitalIn d(p6);
enum Smjer { ZAUSTAVLJENA = 0, LIJEVO = 1, LIJEVOGORE = 2, LIJEVODOLE = 3, DESNO = 4, DESNOGORE = 5, DESNODOLE = 6 };
int brzina = 30;

void glavniMeni();

class Loptica
{
private:
    int pocetniX;
    int pocetniY;
    int trenutniX;
    int trenutniY;
    Smjer smjer;

public:
    Loptica(int x, int y)
    {
        smjer = ZAUSTAVLJENA;
        pocetniX = x;
        pocetniY = y;
        trenutniX = x;
        trenutniY = y;
    }
    void vratiLopticuNaPocetak()
    {
        trenutniX = pocetniX;
        trenutniY = pocetniY;
    }

    void promijeniSmjer(Smjer s)
    {
        smjer = s;
    }
    void generisiSmjer()
    {
        smjer = (Smjer)((rand() % 6) + 1);
    }

    int dajTrenutniX()
    {
        return trenutniX;
    }
    int dajTrenutniY()
    {
        return trenutniY;
    }
    int dajPocetniX()
    {
        return pocetniX;
    }
    int dajPocetniY()
    {
        return pocetniY;
    }
    Smjer dajSmjer()
    {
        return smjer;
    }

    void pomjeriLopticu()
    {
        switch (smjer) {
        case ZAUSTAVLJENA:
            break;
        case LIJEVO:
            trenutniX-=10;
            break;
        case LIJEVOGORE:
            trenutniX-=10;
            trenutniY-=10;
            break;
        case LIJEVODOLE:
            trenutniX-=10;
            trenutniY+=10;
            break;
        case DESNO:
            trenutniX+=10;
            break;
        case DESNOGORE:
            trenutniX+=10;
            trenutniY-=10;
            break;
        case DESNODOLE:
            trenutniX+=10;
            trenutniY+=10;
            break;
        default:
            break;
        }
    }

};

class Reket
{
private:
    int pocetniX;
    int pocetniY;
    int trenutniX;
    int trenutniY;
public:
    Reket()
    {
        pocetniX = 0;
        pocetniY = 0;
        trenutniX = 0;
        trenutniY = 0;
    }
    Reket(int x, int y)
    {
        pocetniX = x;
        pocetniY = y;
        trenutniX = x;
        trenutniY = y;
    }
    void vratiReketNaPocetak()
    {
        trenutniX = pocetniX;
        trenutniY = pocetniY;
    }
    int dajTrenutniX()
    {
        return trenutniX;
    }
    
    void postaviTrenutniY(int y) {
        trenutniY = y;
    }
    
    int dajTrenutniY()
    {
        return trenutniY;
    }
    int dajPocetniX()
    {
        return pocetniX;
    }
    int dajPocetniY()
    {
        return pocetniY;
    }

    void pomjeriGore()
    {
        trenutniY-=10;
    }
    void pomjeriDolje()
    {
        trenutniY+=10;
    }

};

class GameManager
{

private:
    int sirina, visina;
    int score1, score2;
    char gore, dolje;
    bool kraj;
    int duzinaReketa;
    Loptica* loptica;
    Reket* igrac;
    Reket* computer;

public:

Reket* getIgrac() { return computer; }
    GameManager(int s, int v)
    {
        srand(time(NULL));
        kraj = false;
        gore = 'w';
        dolje = 's';
        score1 = score2 = 0;
        sirina = s;
        visina = v;
        duzinaReketa = 60;
        loptica = new Loptica(s / 2 , v / 2);
        igrac = new Reket(1, v / 2 - 40);
        computer = new Reket(s - 11, v / 2 - 40);
        
    }

    ~GameManager()
    {
        delete loptica;
        delete igrac;
        delete computer;
    }
    
    void setDuzinuReketa( int duzina){
        duzinaReketa = duzina;
    }

    void povecajScore(Reket* reket)
    {
        if (igrac == reket)
            score1++;
        else if (computer == reket)
            score2++;

        loptica->vratiLopticuNaPocetak();
        igrac->vratiReketNaPocetak();
        computer->vratiReketNaPocetak();
    }

    void izgled() {
     

  
    
        //BSP_LCD_Clear(LCD_COLOR_BLACK);
        
        
            /* One or dual touch have been detected          */

            /* Get X and Y position of the first touch post calibrated */
          
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

            int lopticaX = loptica->dajTrenutniX();
            int lopticaY = loptica->dajTrenutniY();
            int igrac1X = igrac->dajTrenutniX();
            int igrac2X = computer->dajTrenutniX();
            int igrac1Y = igrac->dajTrenutniY();
            int igrac2Y = computer->dajTrenutniY();
            
            BSP_LCD_FillRect (igrac1X, igrac1Y, 10, duzinaReketa);
            BSP_LCD_FillRect (igrac2X, igrac2Y, 10, duzinaReketa);
            BSP_LCD_SetTextColor(LCD_COLOR_RED);
            BSP_LCD_FillCircle (lopticaX, lopticaY, 5);
            
            BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
            BSP_LCD_SetFont(&Font16);
            std::string s = std::to_string(score1);
            std::string s2 = std::to_string(score2);
            string str = s + " : " + s2;
            char c[str.size() + 1];
            strcpy(c, str.c_str());
            BSP_LCD_DisplayStringAt(0, 15, (uint8_t *)c, CENTER_MODE);
            
            //(int)BSP_LCD_GetXSize(),(int) BSP_LCD_GetYSize()
            int x,y;
            x = BSP_LCD_GetXSize() / 2 -8;
            y = 3;
            
            while(y < (int) BSP_LCD_GetYSize()){
                BSP_LCD_FillRect (x , y, 5 , 10);
                y+=30;
            }
            

           
        
    }

    void input()
    {
        loptica->pomjeriLopticu();
        int lopticaX = loptica->dajTrenutniX();
        int lopticaY = loptica->dajTrenutniY();
        int igracX = igrac->dajTrenutniX();
        int igracY = igrac->dajTrenutniY();
        int computerX = computer->dajTrenutniX();
        int computerY = computer->dajTrenutniY();


    
        
        if(g)
            if(igracY > 0)
                igrac->pomjeriGore();
                
        if(d)
            if (igracY + duzinaReketa < visina)
                igrac->pomjeriDolje();
   
        
                
        
        if (loptica->dajSmjer() == ZAUSTAVLJENA)
            loptica->generisiSmjer();
                

        int computerActive = rand() % 5;
        if(score2 + 3 >= score1)
            computerActive = 1;
        if (computerActive > 0)
        {
            if (loptica->dajSmjer() == DESNO)
            {
                if (lopticaY > computerY + duzinaReketa - 1)
                {
                    if (computerY + duzinaReketa - 1 < visina - 1)
                        computer->pomjeriDolje();
                }
                else if (lopticaY < computerY)
                {
                    if (computerY > 0)
                        computer->pomjeriGore();
                }
            }

            if (loptica->dajSmjer() == DESNODOLE && lopticaY > computerY + duzinaReketa - 1)
            {
                if (computerY + duzinaReketa - 1 < visina - 1)
                    computer->pomjeriDolje();
            }
            if (loptica->dajSmjer() == DESNOGORE && lopticaY < computerY)
            {
                if (computerY > 0)
                    computer->pomjeriGore();
            }
        }

    }

    void logika()
    {
        int lopticaX = loptica->dajTrenutniX();
        int lopticaY = loptica->dajTrenutniY();
        int igrac1X = igrac->dajTrenutniX();
        int computerX = computer->dajTrenutniX();
        int igrac1Y = igrac->dajTrenutniY();
        int computerY = computer->dajTrenutniY();

        for (int i = 1; i < duzinaReketa+1; i++) {
            if (lopticaX <= igrac1X + 10)
                if (lopticaY == igrac1Y + i)
                    loptica->promijeniSmjer((Smjer)((rand() % 3) + 4));
        }

        for (int i = 1; i < duzinaReketa+1; i++) {
            if (lopticaX >= computerX)
                if (lopticaY == computerY + i)
                    loptica->promijeniSmjer((Smjer)((rand() % 3) + 1));
        }

        if (lopticaY >= visina - 1)
            loptica->promijeniSmjer(loptica->dajSmjer() == DESNODOLE ? DESNOGORE : LIJEVOGORE);
        if (lopticaY == 0)
            loptica->promijeniSmjer(loptica->dajSmjer() == DESNOGORE ? DESNODOLE : LIJEVODOLE);
        if (lopticaX >= sirina - 1)
            povecajScore(igrac);
        else if (lopticaX == 0)
            povecajScore(computer);
    }
    void obrisi() {
        
        int lopticaX = loptica->dajTrenutniX();
            int lopticaY = loptica->dajTrenutniY();
            int igrac1X = igrac->dajTrenutniX();
            int igrac2X = computer->dajTrenutniX();
            int igrac1Y = igrac->dajTrenutniY();
            int igrac2Y = computer->dajTrenutniY();
            
            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
            
            BSP_LCD_FillRect (igrac1X, igrac1Y, 10, duzinaReketa);
            BSP_LCD_FillRect (igrac2X, igrac2Y, 10, duzinaReketa);
            BSP_LCD_FillCircle (lopticaX, lopticaY, 5);
    }
    
    
    void pocetniIzgled() {
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_SetFont(&Font16);
        BSP_LCD_DisplayStringAt(0, 15, (uint8_t *)"P I N G  -  P O N G", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 75, (uint8_t *)"Start", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 125, (uint8_t *)"Exit", CENTER_MODE);
        
    }
    
    void levelIzgled() {
        BSP_LCD_Clear(LCD_COLOR_BLACK);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_SetFont(&Font16);
        BSP_LCD_DisplayStringAt(0, 15, (uint8_t *)"Choose a level: ", CENTER_MODE);
        BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
        BSP_LCD_DisplayStringAt(0, 75, (uint8_t *)"Easy", CENTER_MODE);
        BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
        BSP_LCD_DisplayStringAt(0, 125, (uint8_t *)"Medium", CENTER_MODE);
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)"Hard", CENTER_MODE);
    }
    
    void gameOverIzgled() {
        BSP_LCD_SetFont(&Font16);
        BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
        BSP_LCD_DisplayStringAt(0, 125, (uint8_t *)"G A M E  O V E R", CENTER_MODE);
        wait(3);
        
        glavniMeni();
    }
    
    void winIzgled() {
        BSP_LCD_SetFont(&Font16);
        BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
        BSP_LCD_DisplayStringAt(0, 125, (uint8_t *)"Y O U  W I N !", CENTER_MODE);
        wait(3);
    
        glavniMeni();
    }

    void Run() {
        BSP_LCD_Clear(LCD_COLOR_BLACK);
        izgled();
        while (!kraj) {
            obrisi();
            input();
            logika();
            izgled();
            if(score1==11 || score2==11) kraj = true;
            wait_ms(brzina);

        }
        BSP_LCD_Clear(LCD_COLOR_BLACK);
        if(score1 == 11) winIzgled();
        else gameOverIzgled();
    }

};


void inicijalizacijaDispleja() {
    BSP_LCD_Init();

    /* Touchscreen initialization */
    if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) == TS_ERROR) {
        printf("BSP_TS_Init error\n");
    }
    BSP_TS_GetState(&TS_State);
     /* Clear the LCD */
    BSP_LCD_Clear(LCD_COLOR_BLACK);

    /* Set Touchscreen Demo1 description */
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font16);
}

void glavniMeni(){
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    bool start = false;
   GameManager gm((int)BSP_LCD_GetXSize(),(int) BSP_LCD_GetYSize());
    gm.pocetniIzgled();
    while (1) {
        BSP_TS_GetState(&TS_State);
        if(TS_State.touchDetected) {
            /* One or dual touch have been detected          */

            /* Get X and Y position of the first touch post calibrated */
            uint16_t x1 = TS_State.touchX[0];
            uint16_t y1 = TS_State.touchY[0];

            BSP_LCD_SetTextColor(LCD_COLOR_RED);
            BSP_LCD_FillCircle(x1, y1, 5);
            if(x1>=70 && x1<=170 && y1>=110 && y1<=140){
                BSP_LCD_Clear(LCD_COLOR_BLACK);
                return;
            }
            if(x1>=70 && x1<=170 && y1>=60 && y1<=90){
               start = true;
               break;
            }
            wait_ms(10);
        }
    }
    if(start) gm.levelIzgled();
    wait_ms(30);
     while (1) {
        BSP_TS_GetState(&TS_State);
        if(TS_State.touchDetected) {
            /* One or dual touch have been detected          */

            /* Get X and Y position of the first touch post calibrated */
            uint16_t x1 = TS_State.touchX[0];
            uint16_t y1 = TS_State.touchY[0];

            BSP_LCD_SetTextColor(LCD_COLOR_RED);
            BSP_LCD_FillCircle(x1, y1, 5);
            if(x1>=70 && x1<=170 && y1>=60 && y1<=90){
               gm.setDuzinuReketa(80);
               brzina = 300;
               gm.Run();
               return;
            }
            if(x1>=70 && x1<=170 && y1>=110 && y1<=140){
               gm.setDuzinuReketa(60);
               brzina = 170; 
               gm.Run();
               return;
            }
            if(x1>=70 && x1<=170 && y1>=160 && y1<=190){
               gm.setDuzinuReketa(40);
               brzina = 120;
               gm.Run();
               return;
            }
            wait_ms(10);
        }
    } 
}



int main()
{
    inicijalizacijaDispleja();
    glavniMeni();
    return 0;
    
}