//#pragma once
//
//#include <cstdint>
//using EntityID = uint32_t;
//
//struct TowerComponent
//{
//	float fireRate;
//	float projectileSpeed;
//	float projectileDamage;  
//	float attackRange;       
//	float lastShotTimer;  // calculate fireRate in real time
//
//	TowerComponent() : fireRate(0.0f), projectileSpeed(0.0f), projectileDamage(0.0f), attackRange(0.0f), lastShotTimer(0.0f) {}
//
//	TowerComponent(float tRate, float tSpeed, float tDamage, float tRange)
//		: fireRate(tRate), projectileSpeed(tSpeed), projectileDamage(tDamage), attackRange(tRange), lastShotTimer(tRate > 0.0f ? (1.0f / tRate) : 0.0f) {}
//	
//};
