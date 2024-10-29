#include "genericpoisonablebehavior.h"
#include "model/behaviors/health.h"

void GenericPoisonableBehavior::poisonEffect() {
    QVariant poisonLevel = m_owner->getData(DataRole::PoisonLevel);
    auto behavior = m_owner->getBehavior<Health>();
    // Probably better not to do this every tick, the player is the only poisonable obj.
    if(!(poisonLevel.isValid() && behavior && poisonLevel.toInt())) {
        return;
    }

    // It becomes a race against time to see if you can reach a health pack before dying.
    int newPoison = poisonLevel.toInt() - Settings.DAMAGE_PER_TICK;
    if(newPoison > Settings.MIN_POISON) {
        behavior->getHealthChanged(-Settings.DAMAGE_PER_TICK);
        m_owner->setData(DataRole::PoisonLevel, newPoison);

    } else {
        m_owner->setData(DataRole::PoisonLevel, 0);
    }
}
