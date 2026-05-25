#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <raylib.h>

class Background {
    Texture bgFar, bgMid, bgNear, ground, clouds;
    float cloudOffset;
    float parallaxFar, parallaxMid;

public:
    Background() : cloudOffset(0), parallaxFar(0), parallaxMid(0) {
        bgFar = LoadTexture("assets/bg1.png");
        bgMid = LoadTexture("assets/bg2.png");
        bgNear = LoadTexture("assets/bg22.png");
        ground = LoadTexture("assets/island.png");
        clouds = LoadTexture("assets/woodie_menu3.png");
    }

    void update(float playerOffset) {
        parallaxFar = playerOffset * 0.1f;
        parallaxMid = playerOffset * 0.3f;
        cloudOffset += 0.2f;
        if (cloudOffset > SCREEN_WIDTH) cloudOffset = 0;
    }

    void draw() {
        Rectangle source = {0, 0, (float)bgFar.width, (float)bgFar.height};
        Rectangle destFar = {-parallaxFar, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
        DrawTexturePro(bgFar, source, destFar, {0, 0}, 0, WHITE);

        Rectangle sourceMid = {0, 0, (float)bgMid.width, (float)bgMid.height};
        Rectangle destMid = {-parallaxMid, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
        DrawTexturePro(bgMid, sourceMid, destMid, {0, 0}, 0, WHITE);

        Rectangle sourceNear = {0, 0, (float)bgNear.width, (float)bgNear.height};
        Rectangle destNear = {0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
        DrawTexturePro(bgNear, sourceNear, destNear, {0, 0}, 0, WHITE);

        float groundY = SCREEN_HEIGHT - 200;
        Rectangle groundSrc = {0, 0, (float)ground.width, (float)ground.height};
        float groundScale = (float)SCREEN_WIDTH / ground.width;
        Rectangle groundDest = {0, groundY, (float)SCREEN_WIDTH, (float)ground.height * groundScale * 0.5f};
        DrawTexturePro(ground, groundSrc, groundDest, {0, 0}, 0, WHITE);
    }

    ~Background() {
        UnloadTexture(bgFar);
        UnloadTexture(bgMid);
        UnloadTexture(bgNear);
        UnloadTexture(ground);
    }
};

#endif // BACKGROUND_H