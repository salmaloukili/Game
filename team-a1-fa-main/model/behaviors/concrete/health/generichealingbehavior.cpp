#include "generichealingbehavior.h"
#include "model/behaviors/poison.h"

int GenericHealingBehavior::heal(const QPointer<GameObject> &target) {
    int availableHealing = m_owner->getData(DataRole::Health).toInt();

    auto h_behavior = target->getBehavior<Health>();

    // Do not heal if there is no behavior to heal. Do cure the poison even if the
    // health is maxed out. If the poison enemies can move at some point this will not work.
    int healAmount = h_behavior ? h_behavior->getHealthChanged(availableHealing) : 0;
    target->setData(DataRole::PoisonLevel, Poison::SETTINGS::MIN_POISON);

    if(!healAmount) {
        return 0;
    }

    m_owner->setData(DataRole::Health, availableHealing - healAmount);

    // Destroy HP if it is empty
    if(healAmount >= availableHealing) {
        m_owner->setData(DataRole::Destroyed, true);
        delete m_owner;
    }
    return healAmount;
}
