#include "genericmovebehavior.h"

bool GenericMoveBehavior::stepOn(QPointer<GameObject> target) {
    // Get all the behaviors from the target and its children.
    auto behaviors = target->getAllBehaviors<Movement>();
    bool steppable = true;

    for(const auto &bh : behaviors) {
        if(bh.isNull())
            return false;

        steppable = steppable && bh->isSteppable();
    }

    // if any of them is not steppable or don't have the movement behavior, don't move.
    if(!steppable)
        return false;

    float energy = m_owner->getData(DataRole::Energy).toFloat();
    float targetEnergy = target->getData(DataRole::Energy).toFloat();

    // Makes sure the protagonist does not get stuck even if they have a bit of energy left.
    if(energy - targetEnergy < 0) {
        for(const auto &neighbor : m_owner->getAllNeighbors()) {
            if(neighbor->getData(DataRole::Energy).toFloat())
                break;
            m_owner->setData(DataRole::Energy, 0);
        }
        return false;
    }

    // Call step on all the children from the target (and the target itself).
    for(const auto &bh : behaviors) {
        bh->getSteppedOn(m_owner);
    }

    m_owner->setParent(target);
    // This event is the Qt way of notifying a parent change, the view is unaware of
    // the difference, but since there is no position stored in most objects, we cannot change it to notify.
    // The GameObject will send a DataChanged signal with the position of its parent. But will not store it.
    QEvent ev(QEvent::ParentChange);
    m_owner->event(&ev);
    m_owner->setData(DataRole::Energy, energy - targetEnergy);

    return true;
}

bool GenericMoveBehavior::stepOn(Direction direction) {
    // You can either turn or move in the direction you are looking, not both
    auto currentDirection = m_owner->getData(DataRole::Direction).value<Direction>();
    if(direction != currentDirection) {
        m_owner->setData(DataRole::Direction, QVariant::fromValue<Direction>(direction));
        return false;
    }

    auto neighbor = m_owner->getNeighbor(direction);
    if(!neighbor) {
        return false;
    }

    return stepOn(neighbor);
}
