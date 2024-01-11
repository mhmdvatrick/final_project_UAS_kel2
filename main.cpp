#include <sl.h>
#include <random>
#include <vector>
#include <cmath>
#include <string>
#include "entities.h" // Pastikan file header ini berisi definisi class Fish, Boom, dan Koin

// backsound game
void backsound() {
    int Music = slLoadWAV("sound1baru.wav");
    slSoundLoop(Music);
}

// sound effect boom
void soundeffectboom() {
    int bgSound = slLoadWAV("sound boom.wav");
    slSoundPlay(bgSound);
}

// sound effect koin
void soundeffectkoin() {
    int bgSound = slLoadWAV("coin.wav");
    slSoundPlay(bgSound);
}

void ensureFishInScreen(Fish& fish) {
 
    const int minX = 50;
    const int maxX = 1870;
    const int minY = 80;
    const int maxY = 1030;

   
    if (fish.posX < minX) fish.posX = minX;
    if (fish.posX > maxX) fish.posX = maxX;
    if (fish.posY < minY) fish.posY = minY;
    if (fish.posY > maxY) fish.posY = maxY;
}

bool isCollisiontBoom(const Fish& fish, const Boom& boom) {
    double dx = fish.posX - boom.posX;
    double dy = fish.posY - boom.posY;
    double distance = sqrt(pow(dx, 2.0) + pow(dy, 2));
    return (distance <= (boom.radius + fish.radius));
}

bool isCollisiontKoin(const Fish& fish, const Koin& koins) {
    double dx = fish.posX - koins.posX;
    double dy = fish.posY - koins.posY;
    double distance = sqrt(pow(dx, 2.0) + pow(dy, 2));
    return (distance <= (koins.radius + fish.radius));
}

int main() {
    slWindow(1920, 1080, "Let's Eat The Fish", false);

    // Memuat aset
    int imgOcean = slLoadTexture("Ocean.png");
    int imgFish = slLoadTexture("Fish1.png");
    int imgFish2 = slLoadTexture("Fishbalik.png");
    int imgBoom = slLoadTexture("Boom.png");
    int imgKoin = slLoadTexture("Koin.png");
    int font = slLoadFont("font.TTF");

    // fungsi random 
    std::random_device rd;

    // koordinat ikan agar di tengah
    Fish ikan{ 960, 540, 40 };

    Boom boomtuh{ 10, 1085, 90 };
    std::vector<Boom> booms;

    Koin kointuh{ 10, 1085, 40 };
    std::vector<Koin> koins;

    slSetFont(font, 36);
    int nyawa = 3;
    int score = 0;
    bool gameOver = false;

    // perulangan boom jika sampai ujung bawah
    for (int i = 0; i < 3; ++i) {
        double x = 1920 + i * 128;
        double y = (rd() % 3 + 1) * 100;
        booms.push_back({ x, y, 15 });

        backsound();
    }

    // perulangan koin jika sampai ujung bawah
    for (int j = 0; j < 5; ++j) {
        double x = 1920 + j * 128;
        double y = (rd() % 3 + 1) * 100;
        koins.push_back({ x, y, 15 });
    }

    while (!slShouldClose()) {
        slSprite(imgOcean, 960, 540, 1920, 1080);

        // fungsi dalam game
        if (!gameOver) {
            slSprite(imgFish, ikan.posX, ikan.posY, 100, 80);

            if (slGetKey(SL_KEY_DOWN))
                ikan.posY -= 5;
            if (slGetKey(SL_KEY_UP))
                ikan.posY += 5;
            if (slGetKey(SL_KEY_LEFT)) {
                ikan.posX -= 5;
                slSprite(imgFish2, ikan.posX, ikan.posY, 100, 80);
            }
            if (slGetKey(SL_KEY_RIGHT))
                ikan.posX += 5;

            ensureFishInScreen(ikan);

            for (const Boom& g : booms)
                slSprite(imgBoom, g.posX, g.posY, 64, 64);

            for (const Koin& g : koins)
                slSprite(imgKoin, g.posX, g.posY, 64, 64);

            //perulangan boom dan fungsi boom 
            for (Boom& g : booms) {
                g.posY -= 5; // kecepatan jatuh boom
                if (g.posY <= 0) {
                    g.posY = 1000;
                    g.posX = (rd() % 18 + 1) * 100;
                }

                if (isCollisiontBoom(ikan, g)) {
                    soundeffectboom();

                    nyawa = nyawa - 1;
                    g.posY = 1000;
                    g.posX = (rd() % 18 + 1) * 100;

                    if (nyawa <= 0)
                    {
                        gameOver = true;
                    }
                }
            }

            for (Koin& k : koins) {
                k.posY -= 5; // kecepatan jatuh koin
                if (k.posY <= 0) {
                    k.posY = 1000;
                    k.posX = (rd() % 18 + 1) * 100;
                }

                if (isCollisiontKoin(ikan, k)) {
                    soundeffectkoin();
                    k.posY = 1000;
                    k.posX = (rd() % 18 + 1) * 100;

                    score++; // Update score apabila terkena koin
                }
            }

            // text in game
            slSetTextAlign(SL_ALIGN_RIGHT);
            slText(335, 950, std::to_string(nyawa).c_str());
            slText(280, 950, "Healt = ");

            slSetTextAlign(SL_ALIGN_CENTER);
            slText(320, 900, std::to_string(score).c_str());
            slText(173, 900, "Score = ");

        }

        // game over untuk melanjutkan
        else if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT)) {
                score = 0;
                nyawa = 3;
                gameOver = false;
                ikan.posY = 540;
                ikan.posX = 960;

                for (int i = 0; i < booms.size(); ++i)
                {
                    booms[i].posY = 1080 + i * 128;
                    booms[i].posX = (rd() % 19 + 1) * 100;
                }

                for (int i = 0; i < koins.size(); ++i)
                {
                    koins[i].posY = 1080 + i * 128;
                    koins[i].posX = (rd() % 19 + 1) * 100;
                }
            }

        // text game over
        else {
            slSetTextAlign(SL_ALIGN_CENTER);
            slText(960, 540, "Game Over");
            slText(930, 450, "score = ");
            slText(1080, 450, std::to_string(score).c_str());
        }

        slRender();

    }

    slClose();

    return 0;
}
