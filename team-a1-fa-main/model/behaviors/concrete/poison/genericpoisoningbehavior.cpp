#include "genericpoisoningbehavior.h"

#include <QRandomGenerator>

int GenericPoisoningBehavior::poison(const QPointer<GameObject> &target) {
    auto behaviors = target->getAllBehaviors<Poison>();
    int poisonAdminisered = 0;

    for(const auto &behavior : behaviors) {
        if(behavior) {
            int currentLevel = m_owner->getData(DataRole::PoisonLevel).toInt();

            if(currentLevel <= 0) {
                continue;
            }
            // This makes it more fun, otherwise the player just mops up all the poison in the tiles.
            int poisonAmount = QRandomGenerator::global()->bounded(
              Poison::SETTINGS::MIN_POISON_PER_ACTION, Poison::SETTINGS::MAX_POISON_PER_ACTION);

            int poisonedAmount = currentLevel > poisonAmount ? poisonAmount : currentLevel;
            poisonedAmount = behavior->getPoisoned(poisonedAmount);
            poisonAdminisered += poisonedAmount;

            m_owner->setData(DataRole::PoisonLevel, QVariant(currentLevel - poisonedAmount));
        }
    }
    return poisonAdminisered;
}
