#include<SFML/Graphics.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/*-------------------------------------------------------------------------------------*\
|----------<- ToDo ->-------------------------------------------------------------------|
|   2 random map / Silvan                                                               |
|   4 select ändern                                                                     |
|                                                                                       |
|                                                                                       |
|                                                                                       |
|                                                                                       |
\*-------------------------------------------------------------------------------------*/

typedef enum { false, true } bool;

// Typ's of Grafics (Suface)
typedef enum { _grass, _water } surfaceTyp;

// Windows Size
#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 700

sfRenderWindow *window;
sfSprite *grassSprite;
sfSprite *selected;

// Tile Size
#define tileWidth 40
#define tileHeight 20

// Tile scale
float scale = 1;

// Tile half of Size
int h_tileWidth = tileWidth / 2;
int h_tileHeight = tileHeight / 2;


struct point_t {
    int x, y;
    bool selected;
    surfaceTyp type;
} list[1000000];

int listCount = 0;


struct point_t nullPoint = { WINDOW_WIDTH/2, WINDOW_HEIGHT/2 };




void addPointToList(int x, int y, surfaceTyp type) {
    list[listCount].x = x;
    list[listCount].y = y;
    list[listCount].type = type;
    listCount++;
}

struct point_t *getListItem(int x, int y) {
    for (int i = 0; i < listCount; i++) {
        if (list[i].x == x && list[i].y == y)
            return &list[i];
    }
    return NULL;
}



bool prozent(int prozent) {
    int random = rand() % 101;
    
    if (random <= prozent)
        return true;

    return false;
}

int randomBetween(int min, int max) {
   int zahl;

   zahl = rand() % ((max - min) + 1);

   zahl += min;

   return zahl;
} 

bool inHand = false;
struct point_t mousePosBackup = { 0, 0 };

struct point_t selectedTile = { 0, 0 };

int main(void) {

    srand(time(NULL));

    // Textanzeige
    sfText *cellXText = sfText_create();
    sfText_setFillColor(cellXText, sfRed);
    sfText_setColor(cellXText, sfBlack);
    sfText_setFont(cellXText, sfFont_createFromFile("Fonts/arial.ttf"  ));
    sfText_setCharacterSize(cellXText, 20);

    sfText *cellYText = sfText_create();
    sfText_setFillColor(cellYText, sfRed);
    sfText_setColor(cellYText, sfBlack);
    sfText_setFont(cellYText, sfFont_createFromFile("Fonts/arial.ttf"  ));
    sfText_setCharacterSize(cellYText, 20);
    sfVector2f poss = { 0, 50 };
    sfText_setPosition(cellYText, poss);




    // window
    sfVideoMode mode = { WINDOW_WIDTH, WINDOW_HEIGHT, 32 };
    window = sfRenderWindow_create(mode, "Kekcivilisation", sfDefaultStyle, NULL);
    

    // tiles
    grassSprite = sfSprite_create();
    sfSprite_setTexture(grassSprite, sfTexture_createFromFile("Images/tiles.png", 0), 0);

    // selectet
    selected = sfSprite_create();
    sfSprite_setTexture(selected, sfTexture_createFromFile("Images/selected.png", 0), 0);
    

    // Random Map with simple noise

    srand(time(NULL));

    sfVector2i worldSize = { 5, 5 };

    double worldNoise[worldSize.y][worldSize.x]; 
    double temp_worldNoise[worldSize.y][worldSize.x];

    // random zahl in den ecken
    temp_worldNoise[0][0] = randomBetween(-20, 20);
    temp_worldNoise[0][worldSize.x - 1] = randomBetween(-20, 20);
    temp_worldNoise[worldSize.y - 1][0] = randomBetween(-20, 20);
    temp_worldNoise[worldSize.y - 1][worldSize.x - 1] = randomBetween(-20, 20);

    // zwischen oben 
    temp_worldNoise[0][1] = temp_worldNoise[0][0] * 0.75 + temp_worldNoise[0][4] * 0.25;
    temp_worldNoise[0][2] = temp_worldNoise[0][0] * 0.50 + temp_worldNoise[0][4] * 0.50;
    temp_worldNoise[0][3] = temp_worldNoise[0][0] * 0.25 + temp_worldNoise[0][4] * 0.75;

    // zwischen unten
    temp_worldNoise[4][1] = temp_worldNoise[4][0] * 0.75 + temp_worldNoise[4][4] * 0.25;
    temp_worldNoise[4][2] = temp_worldNoise[4][0] * 0.50 + temp_worldNoise[4][4] * 0.50;
    temp_worldNoise[4][3] = temp_worldNoise[4][0] * 0.25 + temp_worldNoise[4][4] * 0.75;

    // zwischen seite rechts
    temp_worldNoise[1][0] = temp_worldNoise[0][0] * 0.75 + temp_worldNoise[4][0] * 0.25;
    temp_worldNoise[2][0] = temp_worldNoise[0][0] * 0.50 + temp_worldNoise[4][0] * 0.50;
    temp_worldNoise[3][0] = temp_worldNoise[0][0] * 0.25 + temp_worldNoise[4][0] * 0.75;
    
    // zwischen seite links
    temp_worldNoise[1][4] = temp_worldNoise[0][4] * 0.75 + temp_worldNoise[4][4] * 0.25;
    temp_worldNoise[2][4] = temp_worldNoise[0][4] * 0.50 + temp_worldNoise[4][4] * 0.50;
    temp_worldNoise[3][4] = temp_worldNoise[0][4] * 0.25 + temp_worldNoise[4][4] * 0.75;

    // mittelsteine
    temp_worldNoise[1][1] = temp_worldNoise[1][0] * 0.75 + temp_worldNoise[1][4] * 0.25;
    temp_worldNoise[1][2] = temp_worldNoise[1][0] * 0.50 + temp_worldNoise[1][4] * 0.50;
    temp_worldNoise[1][3] = temp_worldNoise[1][0] * 0.25 + temp_worldNoise[1][4] * 0.75;

    // mittelsteine 2
    temp_worldNoise[2][1] = temp_worldNoise[2][0] * 0.75 + temp_worldNoise[2][4] * 0.25;
    temp_worldNoise[2][2] = temp_worldNoise[2][0] * 0.50 + temp_worldNoise[2][4] * 0.50;
    temp_worldNoise[2][3] = temp_worldNoise[2][0] * 0.25 + temp_worldNoise[2][4] * 0.75;

    // mittelsteine 3
    temp_worldNoise[3][1] = temp_worldNoise[3][0] * 0.75 + temp_worldNoise[3][4] * 0.25;
    temp_worldNoise[3][2] = temp_worldNoise[3][0] * 0.50 + temp_worldNoise[3][4] * 0.50;
    temp_worldNoise[3][3] = temp_worldNoise[3][0] * 0.25 + temp_worldNoise[3][4] * 0.75;
   
    for (int i = 0; i < worldSize.y; i++) {
        for (int j = 0; j < worldSize.x; j++) {
            worldNoise[i][j] = temp_worldNoise[i][j];
            temp_worldNoise[i][j] = 0;
        }
    }

    // 2 schritt

    temp_worldNoise[0][0] = randomBetween(-10, 10);
    temp_worldNoise[0][2] = randomBetween(-10, 10);
    temp_worldNoise[0][worldSize.x - 1] = randomBetween(-10, 10);

    temp_worldNoise[2][0] = randomBetween(-10, 10);
    temp_worldNoise[2][2] = randomBetween(-10, 10);
    temp_worldNoise[2][worldSize.x - 1] = randomBetween(-10, 10);

    temp_worldNoise[worldSize.y - 1][0] = randomBetween(-10, 10);
    temp_worldNoise[worldSize.y - 1][2] = randomBetween(-10, 10);
    temp_worldNoise[worldSize.y - 1][worldSize.x - 1] = randomBetween(-10, 10);

    // column runter
    temp_worldNoise[0][1] = 0.5 * (temp_worldNoise[0][0] + temp_worldNoise[0][2]);
    temp_worldNoise[1][1] = 0.5 * (temp_worldNoise[1][0] + temp_worldNoise[1][2]);
    temp_worldNoise[2][1] = 0.5 * (temp_worldNoise[2][0] + temp_worldNoise[2][2]);
    temp_worldNoise[3][1] = 0.5 * (temp_worldNoise[3][0] + temp_worldNoise[3][2]);
    temp_worldNoise[4][1] = 0.5 * (temp_worldNoise[4][0] + temp_worldNoise[4][2]);

    temp_worldNoise[0][3] = 0.5 * (temp_worldNoise[0][2] + temp_worldNoise[0][4]);
    temp_worldNoise[1][3] = 0.5 * (temp_worldNoise[1][2] + temp_worldNoise[1][4]);
    temp_worldNoise[2][3] = 0.5 * (temp_worldNoise[2][2] + temp_worldNoise[2][4]);
    temp_worldNoise[3][3] = 0.5 * (temp_worldNoise[3][2] + temp_worldNoise[3][4]);
    temp_worldNoise[4][3] = 0.5 * (temp_worldNoise[4][2] + temp_worldNoise[4][4]);

    // rows
    temp_worldNoise[1][0] = 0.5 * (temp_worldNoise[0][0] + temp_worldNoise[2][0]);
    temp_worldNoise[1][2] = 0.5 * (temp_worldNoise[0][2] + temp_worldNoise[2][2]);
    temp_worldNoise[1][4] = 0.5 * (temp_worldNoise[0][4] + temp_worldNoise[2][4]);

    temp_worldNoise[3][0] = 0.5 * (temp_worldNoise[2][0] + temp_worldNoise[4][0]);
    temp_worldNoise[3][2] = 0.5 * (temp_worldNoise[2][2] + temp_worldNoise[4][2]);
    temp_worldNoise[3][4] = 0.5 * (temp_worldNoise[2][4] + temp_worldNoise[4][4]);


    for (int i = 0; i < worldSize.y; i++) {
        for (int j = 0; j < worldSize.x; j++) {
            worldNoise[i][j] += temp_worldNoise[i][j];
            temp_worldNoise[i][j] = 0;
        }
    }

    // 3 schritt
    for (int i = 0; i < worldSize.y; i++) {
        for (int j = 0; j < worldSize.x; j++) {
            worldNoise[i][j] += randomBetween(-5, 5);
        }
    }

    double lowest;

    for (int i = 0; i < worldSize.y; i++) {
        for (int j = 0; j < worldSize.x; j++) {
           if (worldNoise[i][j] < lowest)
               lowest = worldNoise[i][j];
        }
    }

    for (int i = 0; i < worldSize.y; i++) {
        for (int j = 0; j < worldSize.x; j++) {
            worldNoise[i][j] -= lowest;
        }
    }

    double highest;

    for (int i = 0; i < worldSize.y; i++) {
        for (int j = 0; j < worldSize.x; j++) {
           if (worldNoise[i][j] > highest)
               highest = worldNoise[i][j];
        }
    }

    for (int i = 0; i < worldSize.y; i++) {
        for (int j = 0; j < worldSize.x; j++) {
            worldNoise[i][j] /= highest;
        }
    }


    

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            surfaceTyp typ;
            if (worldNoise[i][j] > 0.7)
                typ = _grass;
            else
                typ = _water;
            addPointToList(j, i, typ);
        }
    }


    



    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            switch (event.type) {
                case sfEvtClosed:
                    sfRenderWindow_close(window);
                    break;
                case sfEvtMouseButtonPressed:
                    inHand = true;
                    break;
                case sfEvtMouseButtonReleased:
                    inHand = false;
            }
            if (event.type == sfEvtMouseMoved) {
                // move the world
                int posX = event.mouseMove.x;
                int posY = event.mouseMove.y;
                if (inHand == true) {
                    int diffX = posX-mousePosBackup.x;
                    int diffY = posY-mousePosBackup.y;
                    nullPoint.x = nullPoint.x + diffX;
                    nullPoint.y = nullPoint.y + diffY;
                }
                mousePosBackup.x = posX;
                mousePosBackup.y = posY;
                        


                // select
                int offsetX = 0, offsetY = 0;

                int temp_tileWidth = tileWidth * scale;
                int temp_h_tileWidth = h_tileWidth * scale;
                int temp_h_tileHeight = h_tileHeight * scale;
                

                selectedTile.x = (posX - nullPoint.x) / temp_tileWidth;
                selectedTile.y = (posY - nullPoint.y) / temp_h_tileHeight;

                offsetX = (posX - nullPoint.x) % temp_tileWidth;
                offsetY = (posY - nullPoint.y) % temp_h_tileHeight;

                switch (selectedTile.y % 2) {
                    case 0:
                        if (offsetX <= (offsetY - temp_h_tileHeight) / -0.5) {
                            selectedTile.x--;
                            selectedTile.y--;
                        } 
                        if (offsetX - temp_h_tileWidth >= offsetY / 0.5) {
                            selectedTile.y--;
                        }
                        break;
                    case 1:
                        switch (offsetX / temp_h_tileWidth) {
                            int eins, zwei;
                            case 0:
                                if (offsetX <= offsetY / 0.5)
                                    selectedTile.x--;
                                if (offsetX >= offsetY / 0.5)
                                    selectedTile.y--;
                                break;
                            case 1:
                                eins = offsetX - temp_h_tileWidth;
                                zwei = (offsetY - temp_h_tileHeight) / -0.5;

                                if (offsetX - temp_h_tileWidth <= (offsetY - temp_h_tileHeight) / -0.5) {
                                    selectedTile.y--;
                                }
                                if (offsetX - temp_h_tileWidth >= (offsetY - temp_h_tileHeight) / -0.5) {
                                }
                                break;
                        } 
                        break;
                }




                char cellX_Text[25];
                char cellY_Text[25];

                sprintf(cellX_Text, "CellX: %d OffsetX: %d", selectedTile.x, offsetX); 
                sprintf(cellY_Text, "CellY: %d OffsetY: %d", selectedTile.y, offsetY); 
                

                sfText_setString(cellXText, cellX_Text);
                sfText_setString(cellYText, cellY_Text);


            }
            if (event.type == sfEvtMouseWheelMoved) {

                int mouse_x = event.mouseWheel.x;
                int mouse_y = event.mouseWheel.y;

                int delta = event.mouseWheel.delta;

                if (scale == 1 && delta == -1) {
                    continue;
                }
                scale += delta;


                switch (delta) {
                    case 1:
                        nullPoint.x -= ((mouse_x - nullPoint.x) * scale) / (scale - 1) - (mouse_x - nullPoint.x);
                        nullPoint.y -= ((mouse_y - nullPoint.y) * scale) / (scale - 1) - (mouse_y - nullPoint.y);
                        break;
                    case -1:
                        nullPoint.x -= ((mouse_x - nullPoint.x) * scale) / (scale + 1) - (mouse_x - nullPoint.x);
                        nullPoint.y -= ((mouse_y - nullPoint.y) * scale) / (scale + 1) - (mouse_y - nullPoint.y);
                        break;
                }
            }
        }

        sfRenderWindow_clear(window, sfWhite);
        for (int i=0; i<listCount; i++) {
            sfVector2f vec = { nullPoint.x + scale * (list[i].x * tileWidth + list[i].y % 2 * h_tileWidth), nullPoint.y + scale * (list[i].y * h_tileHeight) };
            sfIntRect rect = { list[i].type * tileWidth, 0, tileWidth, tileHeight };
            sfSprite_setTextureRect(grassSprite, rect);

            sfVector2f scaleS = { scale, scale };
            sfSprite_setScale(grassSprite, scaleS);

            sfSprite_setPosition(grassSprite, vec);
            sfRenderWindow_drawSprite(window, grassSprite, NULL);
        }

        // selected
        sfVector2f vecSel = { nullPoint.x + scale * (selectedTile.x * tileWidth + selectedTile.y % 2 * h_tileWidth), nullPoint.y + scale * (selectedTile.y * h_tileHeight) };
        sfVector2f scaleSelected = { scale, scale };
        sfSprite_setScale(selected, scaleSelected);
        sfSprite_setPosition(selected, vecSel);
        sfRenderWindow_drawSprite(window, selected, NULL);
       
        // select Rect 
        sfRenderWindow_drawText(window, cellXText, NULL);
        sfRenderWindow_drawText(window, cellYText, NULL);


        sfRenderWindow_display(window);

    }
}

