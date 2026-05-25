#ifndef AI_LEARNING_H
#define AI_LEARNING_H

#include <vector>
#include <algorithm>
#include <cmath>

struct ShotRecord {
    int angle;
    double power;
    int damageDealt;
    bool hit;
    int turnNumber;
};

class AILearning {
    std::vector<ShotRecord> history;
    int totalRounds;
    int wins;
    int losses;

    double avgPlayerAngle;
    double avgPlayerPower;
    int playerShotsFired;
    int playerShotsHit;

public:
    AILearning() : totalRounds(0), wins(0), losses(0),
                   avgPlayerAngle(45.0), avgPlayerPower(0.6),
                   playerShotsFired(0), playerShotsHit(0) {}

    void recordPlayerShot(int angle, double power, int damage, bool didHit) {
        ShotRecord record;
        record.angle = angle;
        record.power = power;
        record.damageDealt = damage;
        record.hit = didHit;
        record.turnNumber = totalRounds;
        history.push_back(record);

        playerShotsFired++;
        if (didHit) playerShotsHit++;

        updatePlayerStats();
    }

    void updatePlayerStats() {
        if (history.empty()) return;

        double totalAngle = 0, totalPower = 0;
        int count = 0;
        int recentCount = std::min((int)history.size(), 10);

        for (int i = history.size() - recentCount; i < (int)history.size(); i++) {
            totalAngle += history[i].angle;
            totalPower += history[i].power;
            count++;
        }

        if (count > 0) {
            avgPlayerAngle = totalAngle / count;
            avgPlayerPower = totalPower / count;
        }
    }

    void onRoundEnd(bool aiWon) {
        totalRounds++;
        if (aiWon) wins++;
        else losses++;
    }

    double getPlayerAccuracy() const {
        if (playerShotsFired == 0) return 0.0;
        return (double)playerShotsHit / playerShotsFired;
    }

    int getDifficultyLevel() const {
        double accuracy = getPlayerAccuracy();

        if (totalRounds < 3) return 1;
        if (accuracy > 0.7 && totalRounds > 5) return 3;
        if (accuracy > 0.4 && totalRounds > 2) return 2;
        return 1;
    }

    int predictBestAngle(int playerAngle, double playerPower) {
        int level = getDifficultyLevel();
        int baseAngle = playerAngle;
        int basePower = (int)(playerPower * 100);

        int minAngle = baseAngle - 15;
        int maxAngle = baseAngle + 10;
        int minPower = basePower - 15;
        int maxPower = basePower + 15;

        switch (level) {
            case 1:
                minAngle = baseAngle - 25;
                maxAngle = baseAngle + 20;
                minPower = basePower - 25;
                maxPower = basePower + 25;
                break;
            case 3:
                minAngle = baseAngle - 5;
                maxAngle = baseAngle + 5;
                minPower = basePower - 5;
                maxPower = basePower + 5;
                break;
        }

        if (minAngle < -45) minAngle = -45;
        if (maxAngle > 90) maxAngle = 90;
        if (minPower < 0) minPower = 0;
        if (maxPower > 100) maxPower = 100;

        return minAngle + (rand() % (maxAngle - minAngle + 1));
    }

    int predictBestPower(double playerPower) {
        int level = getDifficultyLevel();
        int basePower = (int)(playerPower * 100);

        int minPower = basePower - 15;
        int maxPower = basePower + 15;

        switch (level) {
            case 1:
                minPower = basePower - 25;
                maxPower = basePower + 25;
                break;
            case 3:
                minPower = basePower - 5;
                maxPower = basePower + 5;
                break;
        }

        if (minPower < 0) minPower = 0;
        if (maxPower > 100) maxPower = 100;

        return minPower + (rand() % (maxPower - minPower + 1));
    }

    double getAdaptiveAccuracy() {
        int level = getDifficultyLevel();
        switch (level) {
            case 1: return 0.3 + (rand() % 20) / 100.0;
            case 2: return 0.5 + (rand() % 20) / 100.0;
            case 3: return 0.7 + (rand() % 20) / 100.0;
            default: return 0.5;
        }
    }

    int getTotalRounds() const { return totalRounds; }
    int getWins() const { return wins; }
    int getLosses() const { return losses; }
};

inline AILearning g_aiLearning;

#endif // AI_LEARNING_H