/**
 * @file Bullet.cpp
 * @brief 子弹类实现
 */

#include "Bullet.h"
#include "Game.h"

const COLORREF PLAYER_BULLET_COLOR = RGB(100, 255, 255);
const COLORREF ENEMY_BULLET_COLOR = RGB(255, 100, 100);

Bullet::Bullet(int x, int y, float speed, int damage, bool isPlayerBullet)
    : m_x(x)
    , m_y(y)
    , m_speed(speed)
    , m_damage(damage)
    , m_isPlayerBullet(isPlayerBullet)
{
    if (isPlayerBullet)
    {
        m_color = PLAYER_BULLET_COLOR;
    }
    else
    {
        m_color = ENEMY_BULLET_COLOR;
    }
}

Bullet::~Bullet()
{
}

void Bullet::move()
{
    if (m_isPlayerBullet)
    {
        m_y -= m_speed;
    }
    else
    {
        m_y += m_speed;
    }
}

void Bullet::update()
{
    move();
}

void Bullet::draw()
{
    setfillstyle(BS_SOLID);
    setfillcolor(m_color);
    solidrectangle(m_x, m_y, m_x + BULLET_WIDTH, m_y + BULLET_HEIGHT);
}

bool Bullet::isOutOfScreen() const
{
    if (m_isPlayerBullet)
    {
        return m_y + BULLET_HEIGHT < 0;
    }
    else
    {
        return m_y > GAME_HEIGHT;
    }
}