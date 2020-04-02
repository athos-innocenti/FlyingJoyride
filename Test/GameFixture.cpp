//
// Created by Athos Innocenti on 27/11/2018.
//

#include "gtest/gtest.h"
#include "../Game.h"

class GameTest : public ::testing::Test {
protected:
    Game game;
    Window window;
    Enemy testEnemy;
    Block testBlock;
    Character testPlayer;

    sf::CircleShape testEnemyBullet;
    sf::CircleShape testBullet;

    std::vector<sf::CircleShape> bullets;
    std::vector<Block> blocks;
    std::vector<Enemy> enemies;
    std::vector<sf::CircleShape> enemyBullets;

    sf::Texture testPlayerTexture;

    BlockFactory factoryB;
    EnemyFactory factoryE;

    int maxY = game.getMaxY();
};

TEST_F(GameTest, testSettingConst) {
    ASSERT_EQ(1.7f, game.getBulletSpeed());
    ASSERT_EQ(63.0f, game.getLevelGround());
    ASSERT_EQ(1.3f, game.getShootTime());
    ASSERT_EQ(0.120f, game.getRateIncreaser());
    ASSERT_EQ(0.1f, game.getSpeedIncreaser());
}

TEST_F(GameTest, testAssignment) {
    ASSERT_EQ(sf::Vector2f(0.7, 0.8), game.getSpeed());
    ASSERT_TRUE(game.getCreationRate() <= 1.4f);
    ASSERT_TRUE(game.random(maxY) >= 0 && game.random(maxY) <= game.getMaxY());
}

TEST_F(GameTest, testDeletingBlocks) {
    testBlock.setPosition(-100, 50);
    blocks.emplace_back(testBlock);
    // testBlock ha la minima X per cui dovrà essere eliminato
    ASSERT_TRUE(testBlock.getPosition().x < 0);
    blocks.erase(blocks.begin());
    ASSERT_TRUE(blocks.size() == 0);
}

TEST_F(GameTest, testDeletingEnemies) {
    testEnemy.setPosition(-100, 50);
    enemies.emplace_back(testEnemy);
    // testEnemy ha la minima X per cui dovrà essere eliminato
    ASSERT_TRUE(testEnemy.getPosition().x < 0);
    enemies.erase(enemies.begin());
    ASSERT_TRUE(enemies.size() == 0);
}

TEST_F(GameTest, testDeletingEnemiesBullets) {
    testEnemyBullet.setPosition(-100, 50);
    enemyBullets.emplace_back(testEnemyBullet);
    // testEnemyBullet ha la minima X per cui dovrà essere eliminato
    ASSERT_TRUE(testEnemyBullet.getPosition().x < 0);
    enemyBullets.erase(enemyBullets.begin());
    ASSERT_TRUE(enemyBullets.size() == 0);
}

TEST_F(GameTest, testDeletingBullets) {
    testBullet.setPosition(-100, 50);
    bullets.emplace_back(testBullet);
    // testBullet ha la minima X per cui dovrà essere eliminato
    ASSERT_TRUE(testBullet.getPosition().x < 0);
    bullets.erase(bullets.begin());
    ASSERT_TRUE(bullets.size() == 0);
}

TEST_F(GameTest, testMovingPlayer) {
    float lg = game.getLevelGround();
    game.movePlayer();
    ASSERT_TRUE(testPlayer.getPlayerPosition().y >= 0 &&
                testPlayer.getPlayerPosition().y <= (window.getWindowSize().y - lg));
}

TEST_F(GameTest, testBullet) {
    game.createBullet();
    ASSERT_EQ(sf::Color::Black, game.getBullets()[0].getFillColor());
    ASSERT_EQ(10, game.getBullets()[0].getRadius());
}

TEST_F(GameTest, testCollisionPlayerNormalBlock) {
    std::unique_ptr<Block> testBlock = factoryB.createBlock(BlockType::NormalBlock);
    testBlock->setPosition(sf::Vector2f(game.getWindowSize().x, game.random(maxY)));
    testPlayerTexture.loadFromFile("frame-1.png");
    testPlayer.setPlayerTexture(testPlayerTexture);
    testPlayer.setPlayerPosition(testBlock->getPosition().x,testBlock->getPosition().y);
    ASSERT_TRUE(testBlock->getGlobalBounds().intersects(testPlayer.getBound()));
}

TEST_F(GameTest, testNotCollisionPlayerNormalBlock) {
    std::unique_ptr<Block> testBlock = factoryB.createBlock(BlockType::NormalBlock);
    testBlock->setPosition(sf::Vector2f(2 * game.getWindowSize().x, game.random(maxY)));
    testPlayerTexture.loadFromFile("frame-1.png");
    testPlayer.setPlayerTexture(testPlayerTexture);
    testPlayer.setPlayerPosition(game.getWindowSize().x, 2 * game.random(maxY));
    ASSERT_FALSE(testBlock->getGlobalBounds().intersects(testPlayer.getBound()));
}

TEST_F(GameTest, testCollisionPlayerPowerUpBlock) {
    std::unique_ptr<Block> testBlock = factoryB.createBlock(BlockType::PowerUpBlock);
    testBlock->setPosition(sf::Vector2f(game.getWindowSize().x, game.random(maxY)));
    testPlayerTexture.loadFromFile("frame-1.png");
    testPlayer.setPlayerTexture(testPlayerTexture);
    testPlayer.setPlayerPosition(testBlock->getPosition().x,testBlock->getPosition().y);
    ASSERT_TRUE(testBlock->getGlobalBounds().intersects(testPlayer.getBound()));
}

TEST_F(GameTest, testNotCollisionPlayerPowerUpBlock) {
    std::unique_ptr<Block> testBlock = factoryB.createBlock(BlockType::PowerUpBlock);
    testBlock->setPosition(sf::Vector2f(game.getWindowSize().x, game.random(maxY)));
    testPlayerTexture.loadFromFile("frame-1.png");
    testPlayer.setPlayerTexture(testPlayerTexture);
    testPlayer.setPlayerPosition(testBlock->getPosition().x,testBlock->getPosition().y);
    ASSERT_TRUE(testBlock->getGlobalBounds().intersects(testPlayer.getBound()));
}

TEST_F(GameTest, testCollisionPlayerShootingEnemy) {
    std::unique_ptr<Enemy> testEnemy = factoryE.createEnemy(EnemyType::ShootingEnemy);
    testEnemy->setPosition(sf::Vector2f(game.getWindowSize().x, game.random(maxY)));
    testPlayerTexture.loadFromFile("frame-1.png");
    testPlayer.setPlayerTexture(testPlayerTexture);
    testPlayer.setPlayerPosition(testEnemy->getPosition().x,testEnemy->getPosition().y);
    ASSERT_TRUE(testEnemy->getGlobalBounds().intersects(testPlayer.getBound()));
}

TEST_F(GameTest, testNotCollisionPlayerShootingEnemy) {
    std::unique_ptr<Enemy> testEnemy = factoryE.createEnemy(EnemyType::ShootingEnemy);
    testEnemy->setPosition(sf::Vector2f(2 * game.getWindowSize().x, game.random(maxY)));
    testPlayerTexture.loadFromFile("frame-1.png");
    testPlayer.setPlayerTexture(testPlayerTexture);
    testPlayer.setPlayerPosition(game.getWindowSize().x, 2 * game.random(maxY));
    ASSERT_FALSE(testEnemy->getGlobalBounds().intersects(testPlayer.getBound()));
}

TEST_F(GameTest, testCollisionPlayerFlyingEnemy) {
    std::unique_ptr<Enemy> testEnemy = factoryE.createEnemy(EnemyType::FlyingEnemy);
    testEnemy->setPosition(sf::Vector2f(game.getWindowSize().x, game.random(maxY)));
    testPlayerTexture.loadFromFile("frame-1.png");
    testPlayer.setPlayerTexture(testPlayerTexture);
    testPlayer.setPlayerPosition(testEnemy->getPosition().x,testEnemy->getPosition().y);
    ASSERT_TRUE(testEnemy->getGlobalBounds().intersects(testPlayer.getBound()));
}

TEST_F(GameTest, testNotCollisionPlayerFlyingEnemy) {
    std::unique_ptr<Enemy> testEnemy = factoryE.createEnemy(EnemyType::FlyingEnemy);
    testEnemy->setPosition(sf::Vector2f(2 * game.getWindowSize().x, game.random(maxY)));
    testPlayerTexture.loadFromFile("frame-1.png");
    testPlayer.setPlayerTexture(testPlayerTexture);
    testPlayer.setPlayerPosition(game.getWindowSize().x, 2 * game.random(maxY));
    ASSERT_FALSE(testEnemy->getGlobalBounds().intersects(testPlayer.getBound()));
}

TEST_F(GameTest, testCollisionBulletShootingEnemy) {
    std::unique_ptr<Enemy> testEnemy = factoryE.createEnemy(EnemyType::ShootingEnemy);
    testEnemy->setPosition(sf::Vector2f(game.getWindowSize().x, game.random(maxY)));
    testBullet.setRadius(10);
    testBullet.setPosition(testEnemy->getPosition().x,testEnemy->getPosition().y);
    ASSERT_TRUE(testEnemy->getGlobalBounds().intersects(testBullet.getGlobalBounds()));
}

TEST_F(GameTest, testNotCollisionBulletShootingEnemy) {
    std::unique_ptr<Enemy> testEnemy = factoryE.createEnemy(EnemyType::ShootingEnemy);
    testEnemy->setPosition(sf::Vector2f(2 * game.getWindowSize().x, 2 * game.random(maxY)));
    testBullet.setRadius(10);
    testBullet.setPosition(game.getWindowSize().x, game.random(maxY));
    ASSERT_FALSE(testEnemy->getGlobalBounds().intersects(testBullet.getGlobalBounds()));
}