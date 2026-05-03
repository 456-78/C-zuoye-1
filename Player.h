#pragma once
/**
 * @file Player.h
 * @brief 玩家飞机类 - 玩家控制、移动、射击
 */

#ifndef PLAYER_H
#define PLAYER_H

#pragma once

#include <graphics.h>

 // 玩家参数配置
const int PLAYER_WIDTH = 40;//玩家宽度
const int PLAYER_HEIGHT = 40;
const int PLAYER_SPEED = 8;
const int PLAYER_BULLET_SPEED = 10;//子弹速度
const int PLAYER_BULLET_DAMAGE = 1;//子弹伤害
const int PLAYER_MAX_HP = 10;//玩家生命值
const int PLAYER_SHOOT_COOLDOWN = 10;//射击冷却时间

class Player
{
public:
    Player();
    ~Player();

    void move(int dx, int dy);
    void update();
    void draw();

    int getX() const { return m_x; }
    int getY() const { return m_y; }
    int getWidth() const { return PLAYER_WIDTH; }
    int getHeight() const { return PLAYER_HEIGHT; }
    int getHP() const { return m_hp; }
    void setHP(int hp) { m_hp = hp; }
    void takeDamage(int damage);
    
    bool canShoot() const { return m_shootCooldown <= 0; }
    void startCooldown() { m_shootCooldown = PLAYER_SHOOT_COOLDOWN; }

private:
    void clampPosition();

private:
    int m_x;
    int m_y;
    int m_hp;
    int m_shootCooldown;
};

#endif // PLAYER_H