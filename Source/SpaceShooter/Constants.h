// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Math/UnrealMathUtility.h"
#include <array>
#include <map>

// BrainCloud app constants
static const FString kBrainCloudServer = "https://api.braincloudservers.com/dispatcherv2";
static const FString kBrainCloudAppID = ""; // App ID is empty on purpose
static const FString kBrainCloudAppSecret = "";  // App secret is empty on purpose
static const FString kBrainCloudAppName = "Space Shooter (Unreal)";
static const FString kBrainCloudAppVersion = "1.0.0";
static const FString kBrainCloudMainLeaderboardID = "Main";
static constexpr int kBrainCloudDefaultMinHighScoreIndex = 0;
static constexpr int kBrainCloudDefaultMaxHighScoreIndex = 9;
static const FString kBrainCloudLevelDataIndexedID = "Level";

static const FString kBrainCloudStatGamesPlayed = "GamesCompleted";
static const FString kBrainCloudStatEnemiesKilled = "EnemiesKilled";
static const FString kBrainCloudStatAsteroidDestroyed = "AsteroidsDestroyed";
static const std::map<FString, FString> kBrainCloudStatisticDescriptions{   {kBrainCloudStatGamesPlayed, "Games completed"},
                                                                            {kBrainCloudStatEnemiesKilled, "Enemies killed"},
                                                                            {kBrainCloudStatAsteroidDestroyed, "Asteroids destroyed"}
};

static const FString kBrainCloudAchievementLast30 = "Survive30";
static const FString kBrainCloudAchievementLast60 = "Survive60";
static const FString kBrainCloudAchievementBeatLevel1 = "BeatLevel1";
static const FString kBrainCloudUserDataEntityType = "UserProgress";

static const FString kBrainCloudEmailIdentity = "Email";


// General constants
static constexpr float kScreenWidth = 1280.0f;
static constexpr float kScreenHeight = 720.0f;
static constexpr float kOffScreenSpawnBuffer = 128.0f;
static constexpr float kBackgroundSpeed = 250.0f;
static constexpr float kEndOfGameDisplayDuration = 2.5f;
static constexpr float kLevelDisplayDuration = 2.5f;
static constexpr int kHordeModeLevelOne = 0;
static constexpr int kHordeModeLevelTwo = 1;
static constexpr int kHordeModeLevelThree = 2;
static constexpr int kHordeModeLevelBoss = 3;
static constexpr float kConnectingDialogDotsInterval = 0.75f;
static constexpr int kMaxAchievementElements = 8;
static constexpr int kMaxStatisticElements = 8;
static constexpr int kMaxLeaderboardElements = 10;



// Object pool constants
static constexpr int kAsteroidPoolSize = 50;
static constexpr int kEnemyPoolSize = 20;
static constexpr int kExplosionPoolSize = 25;
static constexpr int kLaserPoolSize = 75;
static constexpr int kLaserImpactPoolSize = kLaserPoolSize;
static constexpr int kMissilePoolSize = 25;
static constexpr int kShipWingPoolSize = (kEnemyPoolSize + 1) * 2;
static constexpr int kPickupPoolSize = 4;


// Depth constants
static constexpr float kExplosionLayer = 9.0f;
static constexpr float kLaserImpactLayer = 8.0f;
static constexpr float kAsteroidLayer = 7.0f;
static constexpr float kShipLayer = 6.0f;
static constexpr float kShipWingLayer = 5.5f;
static constexpr float kEnemyLayer = 5.0f;
static constexpr float kBossLayer = 4.0f;
static constexpr float kLaserLayer = 2.0f;
static constexpr float kMissileLayer = 1.5f;
static constexpr float kPickupLayer = 1.0f;


// HUD constants
static constexpr int kHudHeight = 48;
static constexpr float kHudSmallBuffer = 2.0f;
static constexpr float kHudBigBuffer = 6.0f;
static constexpr float kHudDangerFlashIncrement = PI;
static constexpr float kHudHighScoreMovementSpeed = 150.0f;


// Ship constants
static constexpr int kShipInitialHealth = 4;
static constexpr int kShipInitialShieldHealth = 4;
static constexpr float kShipInvincibilityDuration = 0.75f;
static constexpr float kShipInvincibleAlpha = 0.65f;
static constexpr float kShipOffScreenSpawnX = -100.0f;
static constexpr float kShipSpawnX = 100.0f;

static constexpr float kShipAcceleration = 30.0f;
static constexpr float kShipDrag = 0.8f;
static constexpr float kShipMaxSpeed = 900.0f;
static constexpr float kShipMinX = 100.0f;
static constexpr float kShipMaxOffsetX = 200.0f;

static constexpr float kShipTurnTilt = 0.34906f;        // 20 degrees
static constexpr float kShipGunOffset = 10.0f;
static constexpr float kShipGunAngleTilt = 0.0209436f;  // 1.2 degrees

static constexpr float kShipCircleCollisionRadius = 20.0f;
static constexpr float kShipEdgeCollisionAngle = 2.094395f;  // 120 degrees
static constexpr float kShipEdgeCollisionMagnitude = 42.0f;


// Asteroid constants
static constexpr float kAsteroidMinSpawnRadians = 2.792f;  // 160 degrees
static constexpr float kAsteroidMaxSpawnRadians = 3.49f;   // 200 degrees
static constexpr float kAsteroidMinSpeed = 100.0f;
static constexpr float kAsteroidMaxSpeed = 300.0f;
static constexpr float kAsteroidMinAngularVelocity = PI * 0.5f;         // 90 degrees per second
static constexpr float kAsteroidMaxAngularVelocity = PI + PI * 0.5f;    // 270 degrees per second
static constexpr float kAsteroidMinSpawnTime = 0.8f;
static constexpr float kAsteroidMaxSpawnTime = 2.5f;
static constexpr float kAsteroidMinSpawnCount = 1;
static constexpr float kAsteroidMaxSpawnCount = 5;
static constexpr float kAsteroidExplosionMinSpeed = 150.0f;
static constexpr float kAsteroidExplosionMaxSpeed = 325.0f;
static constexpr float kAsteroidExplosionFadeOutTime = 1.0f;
static constexpr float kAsteroidExplosionFadeDelay = 0.25f;

static constexpr int kNumAsteroidSizes = 4;
static constexpr std::array<int,kNumAsteroidSizes> kNumAsteroidVariations = {3, 2, 2, 2};
static const std::array<FString,kNumAsteroidVariations[0]> kBigAsteroidAtlasKeys = {"AsteroidBig-1", "AsteroidBig-2", "AsteroidBig-3"};
static const std::array<FString,kNumAsteroidVariations[1]> kMediumAsteroidAtlasKeys = {"AsteroidMedium-1", "AsteroidMedium-2"};
static const std::array<FString,kNumAsteroidVariations[2]> kSmallAsteroidAtlasKeys = {"AsteroidSmall-1", "AsteroidSmall-2"};
static const std::array<FString,kNumAsteroidVariations[3]> kTinyAsteroidAtlasKeys = {"AsteroidTiny-1", "AsteroidTiny-2"};

static constexpr std::array<int,kNumAsteroidSizes> kAsteroidHealth = {2, 1, 0, 0};
static constexpr std::array<int,kNumAsteroidSizes> kAsteroidAttackDamage = {2, 1, 0, 0};


// Enemy constants
static constexpr float kEnemyMinSpeed = 200.0f;
static constexpr float kEnemyMaxSpeed = 300.0f;
static constexpr float kEnemyMinSpawnAngle = 2.96701f;  // 170 Degrees;
static constexpr float kEnemyMaxSpawnAngle = 3.31607f;  // 190 Degrees;
static constexpr float kEnemyLaserMinDelay = 0.75f;
static constexpr float kEnemyLaserMaxDelay = 2.0f;
static constexpr float kEnemyMinSpawnTime = 1.5f;
static constexpr float kEnemyMaxSpawnTime = 3.5f;
static constexpr int kEnemyMinSpawnCount = 1;
static constexpr int kEnemyMaxSpawnCount = 3;
static constexpr float kEnemyThreeFireDelay = 0.35f;
static constexpr float kEnemyFourFireDelay = 0.1f;
static constexpr float kEnemyFourFiringDuration = 1.25f;
static constexpr float kEnemyFourFiringCooldown = 0.5f;
static constexpr float kEnemyFiveFiringMinCooldown = 1.75f;
static constexpr float kEnemyFiveFiringMaxCooldown = 3.0f;
static constexpr float kEnemyFiveMissileMinDelay = 0.8f;
static constexpr float kEnemyFiveMissileMaxDelay = 1.4f;
static constexpr float kEnemyFiveLaserMinDelay = 0.4f;
static constexpr float kEnemyFiveLaserMaxDelay = 1.2f;

static constexpr int kNumEnemyTypes = 5;
static const std::array<FString,kNumEnemyTypes> kEnemyAtlasKeys = { "EnemyShip-1", "EnemyShip-2", "EnemyShip-3", "EnemyShip-4", "EnemyShip-5" };
static const std::array<FString,kNumEnemyTypes> kEnemyTypes = { "Enemy-1", "Enemy-2", "Enemy-3", "Enemy-4", "Enemy-5" };
static const std::array<FString,kNumEnemyTypes> kEnemyWingLeftAtlasKeys = { "EnemyShipLeftWing-1", "EnemyShipLeftWing-2", "EnemyShipLeftWing-3", "EnemyShipLeftWing-4", "EnemyShipLeftWing-5" };
static const std::array<FString,kNumEnemyTypes> kEnemyWingRightAtlasKeys = { "EnemyShipRightWing-1", "EnemyShipRightWing-2", "EnemyShipRightWing-3", "EnemyShipRightWing-4", "EnemyShipRightWing-5" };

static constexpr float kEnemyGunOffset1 = 16.0f;
static constexpr float kEnemyGunOffset2 = 6.0f;
static constexpr float kEnemyGunOffset3 = 18.0f;
static constexpr float kEnemyGunOffset4_1 = 21.0f;
static constexpr float kEnemyGunOffset4_2 = 27.0f;
static constexpr float kEnemyGunOffset5 = 25.0f;


// Ship wing constants
static constexpr float kShipWingExplosionMinSpeed = 175.0f;
static constexpr float kShipWingExplosionMaxSpeed = 250.0f;
static constexpr float kShipWingExplosionFadeOutTime = 1.0f;
static constexpr float kShipWingExplosionOffset = 5.0f;
static constexpr float kShipWingMinAngularVelocity = PI * 0.25f;         // 45 degrees per second
static constexpr float kShipWingMaxAngularVelocity = PI * 0.5f;  // 90 degrees per second


// Laser constants
static constexpr float kLaserSpeed = 1200.0f;
static constexpr float kLaserImpactLifetime = 0.05f;


// Missile constants
static constexpr float kMissileSmallSpeed = 1000.0f;
static constexpr float kMissileBigSpeed = 750.0f;
static constexpr int kMissileSmallAttackDamage = 2;
static constexpr int kMissileBigAttackDamage = 4;


// Boss constants
static const FString kBossFrontLeftWingAtlasKey = "BossFrontLeftWing";
static const FString kBossFrontRightWingAtlasKey = "BossFrontRightWing";
static const FString kBossMiddleLeftWingAtlasKey = "BossMiddleLeftWing";
static const FString kBossMiddleRightWingAtlasKey = "BossMiddleRightWing";
static const FString kBossBackLeftWingAtlasKey = "BossBackLeftWing";
static const FString kBossBackRightWingAtlasKey = "BossBackRightWing";
static constexpr float kBossWingExplosionSpeed = 500.0f;
static constexpr float kBossWingExplosionOffset = 40.0f;
static constexpr float kBossOffScreenSpawnX = 1450.0f;
static constexpr float kBossSpawnX = 950.0f;
static constexpr float kBossMinX = 450.0f;
static constexpr float kBossMovementMinDelay = 0.05f;
static constexpr float kBossMovementMaxDelay = 0.2f;
static constexpr float kBossMovementMinRange = 200.0f;
static constexpr float kBossMovementMaxRange = 600.0f;
static constexpr float kBossSmallMissileMinDelay = 0.6f;
static constexpr float kBossSmallMissileMaxDelay = 1.2f;
static constexpr float kBossBigMissileMinDelay = 1.2f;
static constexpr float kBossBigMissileMaxDelay = 2.0f;
static constexpr float kBossSpeed = 450.0f;
static constexpr int kBossHealth = 40;
static constexpr int kBossAttackDamage = 10;

static constexpr float kBossGunOffset1 = 64.0f;
static constexpr float kBossGunOffset2 = 56.0f;

// Pickup constants
static constexpr float kPickupLifetime = 5.0f;
static constexpr float kPickupFadeOutTime = 0.5f;

