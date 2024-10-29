#include "counterattackbehavior.h"

int CounterAttackBehavior::getAttacked(const QPointer<GameObject> &target, int strenght) {
    // This is neat, the opposite angle is the angle + 180,
    // modulo 360 makes sure it is always smaller than 360 deg.
    int direction = target->getData(DataRole::Direction).toInt();
    int newDirection = (180 + direction) % 360;

    // Move to where you got attacked and counter attack.
    if(direction - newDirection) {
        m_owner->setData(DataRole::Direction, newDirection);
    }
    GenericAttackBehavior::attack(target);

    // Process the attack. This does mean enemies attack first when hit by an enemy but it is not noticeable.
    return GenericAttackBehavior::getAttacked(target, strenght);
}
