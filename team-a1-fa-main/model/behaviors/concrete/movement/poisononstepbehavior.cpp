#include "poisononstepbehavior.h"

#include <model/behaviors/poison.h>

bool PoisonOnStepBehavior::getSteppedOn(const QPointer<GameObject> &source) {
    // Mostly for tiles, since they keep poison and pass it on to the protagonist.
    m_owner->getBehavior<Poison>()->poison(source);
    return GenericWalkableBehavior::getSteppedOn(source);
}
