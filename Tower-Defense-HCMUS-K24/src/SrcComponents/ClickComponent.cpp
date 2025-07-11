#include "../../header/Components/ClickComponent.h"
#include "../../header/Managers/EntityManager.h"
#include "../../header/Managers/World.h"


bool ClickComponent::tryClick(Vector2f mousePos, EntityID entityId, World& world)
{
    if (area.contains(mousePos) && onClick)
    {
        onClick(entityId, world);
        return true;
    }
    return false;
}