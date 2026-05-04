/**
 * @file Bullet.h
 * @brief 子弹类 - 子弹移动、绘制
 
 */

#ifndef BULLET_H
#define BULLET_H

#pragma once

#include <graphics.h>

 // 子弹参数配置
const int BULLET_WIDTH = 6;
const int BULLET_HEIGHT = 15;

class Bullet
{
public:
    Bullet(int x, int y, float speed, int damage, bool isPlayerBullet = true);
    ~Bullet();

    void move();
    void update();
    void draw();

    int getX() const { return m_x; }
    int getY() const { return m_y; }
    int getWidth() const { return BULLET_WIDTH; }
    int getHeight() const { return BULLET_HEIGHT; }
    int getDamage() const { return m_damage; }
    bool isPlayerBullet() const { return m_isPlayerBullet; }
    bool isOutOfScreen() const;

private:
    int m_x;
    int m_y;
    float m_speed;
    int m_damage;
    bool m_isPlayerBullet;
    COLORREF m_color;
};

#endif // BULLET_H#pragma once
