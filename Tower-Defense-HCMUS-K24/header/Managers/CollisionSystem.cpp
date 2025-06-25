//#include "CollisionSystem.h"
//
//float CollisionSystem::computeDistanceOfTwoPoint(float dX, float dY) const
//{
//	return sqrt(dX * dX  + dY * dY);
//}
//
//void CollisionSystem::updateCheck(ComponentArray<VelocityComponent>& velocityArray, ComponentArray<CircleComponent>& circleArray, ComponentArray<TowerComponent>& towerRangeArray, ComponentArray<HealthComponent>& healthArray, EntityManager& entityManager)
//{
//    for (uint32_t i = 0; i < towerRangeArray.getSize(); ++i)
//    {
//        TowerComponent& tower = towerRangeArray.getComponentArray()[i];
//
//        // For each tower, check if any enemy is within range
//        for (uint32_t j = 0; j < circleArray.getSize(); ++j)
//        {
//            CircleComponent& enemy = circleArray.getComponentArray()[j];
//
//            // Only process if the entity is an enemy and within the tower's range
//            if (enemy.tag == CircleComponent::CollisionType::Enemy)
//            {
//                float distX = enemy.x - towerRangeArray.getComponentArray()[i].x;
//                float distY = enemy.y - towerRangeArray.getComponentArray()[i].y;
//                float distance = computeDistanceOfTwoPoint(distX, distY);
//
//                // If the enemy is within the tower's attack range
//                if (distance <= tower.attackRange)
//                {
//                    // Shoot a projectile (create a new projectile entity)
//                    EntityID projectileID = entityManager.createEntity();
//
//                    // Set the projectile's position to be the same as the tower
//                    CircleComponent projectile;
//                    projectile.x = towerRangeArray.getComponentArray()[i].x;
//                    projectile.y = towerRangeArray.getComponentArray()[i].y;
//                    projectile.radius = 5.0f; // Example projectile radius
//                    projectile.tag = CircleComponent::CollisionType::Projectile;
//                    projectile.owner = towerRangeArray.getComponentArray()[i].owner; // Link the projectile to the tower
//                    circleArray.insertData(projectileID, projectile);
//
//                    // Set the projectile's velocity to move towards the enemy
//                    VelocityComponent velocity;
//                    float speed = tower.projectileSpeed;
//                    float dx = enemy.x - projectile.x;
//                    float dy = enemy.y - projectile.y;
//                    float angle = atan2(dy, dx);
//                    velocity.x = cos(angle) * speed;
//                    velocity.y = sin(angle) * speed;
//                    velocityArray.insertData(projectileID, velocity);
//
//                    // Return after firing to avoid multiple projectiles being created
//                    return;
//                }
//            }
//        }
//    }
//
//    // Continue with your existing collision check logic
//    for (uint32_t i = 0; i < circleArray.getSize(); ++i)
//    {
//        // Example: checking if a projectile hits an enemy
//        for (uint32_t j = 0; j < circleArray.getSize(); ++j)
//        {
//            if (i == j) continue; // Skip self-collision
//
//            CircleComponent& projectile = circleArray.getComponentArray()[i];
//            CircleComponent& enemy = circleArray.getComponentArray()[j];
//
//            // Check if it's a projectile hitting an enemy
//            if (projectile.tag == CircleComponent::CollisionType::Projectile &&
//                enemy.tag == CircleComponent::CollisionType::Enemy)
//            {
//                float distX = projectile.x - enemy.x;
//                float distY = projectile.y - enemy.y;
//                float distance = computeDistanceOfTwoPoint(distX, distY);
//
//                // Check if they collide (based on their radii)
//                if (distance < projectile.radius + enemy.radius)
//                {
//                    // Deal damage to the enemy
//                    if (healthArray.containData(enemy.owner))
//                    {
//                        healthArray.getData(enemy.owner).takeDamage(tower.projectileDamage); // Use tower's projectile damage
//                    }
//
//                    // Destroy the projectile
//                    entityManager.destroyEntity(projectile.owner);
//                }
//            }
//        }
//    }
//
//}&
//
//
//
//
//
//
