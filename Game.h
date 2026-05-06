/**
 * @file Game.h
 * @brief 游戏管理类 - 负责游戏主循环、状态管理和碰撞检测

 */

#ifndef GAME_H
#define GAME_H

#pragma once

#include <graphics.h>
#include <time.h>
#include <vector>

 // 前置声明
class Player;
class Enemy;
class Bullet;

// 游戏窗口大小
const int GAME_WIDTH = 480;
const int GAME_HEIGHT = 700;

// 游戏状态枚举
enum class GameState {
    MENU,       // 开始菜单
    PLAYING,    // 游戏中
    PAUSED,     // 暂停
    GAMEOVER    // 游戏结束
};

// 游戏难度等级
enum class Difficulty {
    EASY,       // 简单
    NORMAL,     // 普通
    HARD        // 困难
};

// 敌机类型枚举 - 改为前置声明，实际定义在 Enemy.h 中
enum class EnemyType;  // 前置声明，避免重复定义

class Game
{
public:
    Game();
    ~Game();

    void run();

    static int getWidth() { return GAME_WIDTH; }
    static int getHeight() { return GAME_HEIGHT; }

private:
    void init();
    void gameLoop();
    void handleInput();
    void update();
    void render();
    void drawMenu();
    void drawGameOver();
    void drawPaused();
    void drawUI();
    void checkCollisions();
    void spawnEnemies();
    void resetGame();
    void applyDifficulty();

private:
    HWND m_hwnd;
    GameState m_state;
    Difficulty m_difficulty;

    Player* m_player;
    std::vector<Enemy*> m_enemies;
    std::vector<Bullet*> m_bullets;

    int m_score;
    int m_highScore;
    int m_enemySpawnTimer;
    int m_frameCount;

    bool m_keyStates[256] = {0};
    int m_enemySpawnInterval;
    float m_enemySpeedMultiplier;
};

#endif // GAME_H