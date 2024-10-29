#include "healonstepbehavior.h"

#include <model/behaviors/health.h>

bool HealOnStepBehavior::getSteppedOn(const QPointer<GameObject> &source) {
    m_owner->getBehavior<Health>()->heal(source);
    return GenericWalkableBehavior::getSteppedOn(source);
}
