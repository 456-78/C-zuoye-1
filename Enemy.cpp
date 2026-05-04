/**
 * @file Enemy.cpp
 * @brief 敌机类实现
 */

#include "Enemy.h"
#include "Game.h"

Enemy::Enemy(EnemyType type)
    : m_type(type)
    , m_speedMultiplier(1.0f)
    , m_shootTimer (0)
    ,m_shootCooldown (120)
{
    switch (m_type)
    {
    case EnemyType::SMALL:
        m_width = SMALL_ENEMY_WIDTH;
        m_height = SMALL_ENEMY_HEIGHT;
        m_hp = SMALL_ENEMY_HP;
        m_speed = SMALL_ENEMY_SPEED;
        m_score = SMALL_ENEMY_SCORE;
        m_damage = SMALL_ENEMY_DAMAGE;
        break;

    case EnemyType::MEDIUM:
        m_width = MEDIUM_ENEMY_WIDTH;
        m_height = MEDIUM_ENEMY_HEIGHT;
        m_hp = MEDIUM_ENEMY_HP;
        m_speed = MEDIUM_ENEMY_SPEED;
        m_score = MEDIUM_ENEMY_SCORE;
        m_damage = MEDIUM_ENEMY_DAMAGE;
        break;

    case EnemyType::LARGE:
        m_width = LARGE_ENEMY_WIDTH;
        m_height = LARGE_ENEMY_HEIGHT;
        m_hp = LARGE_ENEMY_HP;
        m_speed = LARGE_ENEMY_SPEED;
        m_score = LARGE_ENEMY_SCORE;
        m_damage = LARGE_ENEMY_DAMAGE;
        break;
    }

    m_x = rand() % (GAME_WIDTH - m_width);
    m_y = -m_height;
}

Enemy::~Enemy()
{
}

void Enemy::move(float speedMultiplier)
{
    m_y += (int)(m_speed * speedMultiplier);
}

void Enemy::update()
{
    move(m_speedMultiplier);
    if (m_shootTimer > 0) {
        m_shootTimer--;
    }
}

void Enemy::draw()
{
    switch (m_type)
    {
    case EnemyType::SMALL:
        drawSmallEnemy();
        break;
    case EnemyType::MEDIUM:
        drawMediumEnemy();
        break;
    case EnemyType::LARGE:
        drawLargeEnemy();
        break;
    }
}

void Enemy::drawSmallEnemy()
{
    setfillstyle(BS_SOLID);

    setfillcolor(RGB(255, 100, 100));
    POINT triangle[] = {
        {m_x + m_width / 2, m_y + m_height},
        {m_x, m_y},
        {m_x + m_width, m_y}
    };
    solidpolygon(triangle, 3);
}

void Enemy::drawMediumEnemy()
{
    setfillstyle(BS_SOLID);

    setfillcolor(RGB(180, 100, 200));
    solidrectangle(m_x + 12, m_y + 5, m_x + 28, m_y + 35);

    setfillcolor(RGB(150, 80, 180));
    solidellipse(m_x + 14, m_y + 30, m_x + 26, m_y + 40);

    setfillcolor(RGB(160, 90, 180));
    POINT leftWing[] = {
        {m_x + 12, m_y + 15},
        {m_x - 5, m_y + 28},
        {m_x + 12, m_y + 25}
    };
    solidpolygon(leftWing, 3);

    POINT rightWing[] = {
        {m_x + 28, m_y + 15},
        {m_x + 45, m_y + 28},
        {m_x + 28, m_y + 25}
    };
    solidpolygon(rightWing, 3);

    solidrectangle(m_x + 15, m_y, m_x + 25, m_y + 10);
}

void Enemy::drawLargeEnemy()
{
    setfillstyle(BS_SOLID);

    setfillcolor(RGB(80, 120, 200));
    solidrectangle(m_x + 15, m_y + 10, m_x + 40, m_y + 45);

    setfillcolor(RGB(60, 100, 180));
    solidrectangle(m_x + 5, m_y + 20, m_x + 15, m_y + 40);
    solidrectangle(m_x + 40, m_y + 20, m_x + 50, m_y + 40);

    setfillcolor(RGB(50, 90, 170));
    solidellipse(m_x + 20, m_y + 40, m_x + 35, m_y + 55);

    setfillcolor(RGB(70, 110, 190));
    solidrectangle(m_x + 18, m_y, m_x + 32, m_y + 15);
}

void Enemy::takeDamage(int damage)
{
    m_hp -= damage;
    if (m_hp < 0)
    {
        m_hp = 0;
    }
}

bool Enemy::isOutOfScreen() const
{
    return m_y > GAME_HEIGHT;
}
bool Enemy::canShoot()
{
    if (m_shootTimer <= 0)
    {
        m_shootTimer = m_shootCooldown;
        return true;
    }
    return false;
}

void Enemy::resetShootTimer()
{
    m_shootTimer = m_shootCooldown;
}