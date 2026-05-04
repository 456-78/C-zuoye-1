/**
 * @file Enemy.h
 * @brief 敌机类 - 敌机类型、移动、碰撞

 */

#ifndef ENEMY_H
#define ENEMY_H

#pragma once
#include <graphics.h>
#include <cstdlib>

 // 小型敌机属性
const int SMALL_ENEMY_WIDTH = 30;
const int SMALL_ENEMY_HEIGHT = 30;
const int SMALL_ENEMY_HP = 1;
const int SMALL_ENEMY_SPEED =3;    
const int SMALL_ENEMY_SCORE = 10;
const int SMALL_ENEMY_DAMAGE = 1;

// 普通敌机属性
const int MEDIUM_ENEMY_WIDTH = 40;
const int MEDIUM_ENEMY_HEIGHT = 40;
const int MEDIUM_ENEMY_HP = 3;
const int MEDIUM_ENEMY_SPEED = 4;   
const int MEDIUM_ENEMY_SCORE = 20;
const int MEDIUM_ENEMY_DAMAGE = 2;

// 大型敌机属性
const int LARGE_ENEMY_WIDTH = 60;
const int LARGE_ENEMY_HEIGHT = 60;
const int LARGE_ENEMY_HP = 5;
const int LARGE_ENEMY_SPEED = 5;    
const int LARGE_ENEMY_SCORE = 50;
const int LARGE_ENEMY_DAMAGE = 3;

enum class EnemyType {
    SMALL,
    MEDIUM,
    LARGE
};

class Enemy
{
public:
    Enemy(EnemyType type);
    ~Enemy();

    void move(float speedMultiplier = 1.0f);
    void update();
    void draw();

    int getX() const { return m_x; }
    int getY() const { return m_y; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getHP() const { return m_hp; }
    void setHP(int hp) { m_hp = hp; }
    void takeDamage(int damage);
    EnemyType getType() const { return m_type; }
    int getScore() const { return m_score; }
    bool isOutOfScreen() const;
    void setSpeedMultiplier(float mult) { m_speedMultiplier = mult; }
    bool canShoot();
    void resetShootTimer();
    int getBulletX() const { return m_x + m_width / 2 - 3; }  // 子弹出生X
    int getBulletY() const { return m_y + m_height; }           // 子弹出生Y

private:
    void drawSmallEnemy();
    void drawMediumEnemy();
    void drawLargeEnemy();

private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_hp;
    float m_speed;
    EnemyType m_type;
    int m_score;
    int m_damage;
    float m_speedMultiplier;
    int m_shootTimer;
    int m_shootCooldown;
};

#endif // ENEMY_H