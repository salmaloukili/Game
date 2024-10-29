#include "newlevelonstep.h"
bool NewLevelOnStep::getSteppedOn(const QPointer<GameObject> &source) {
    ObjectType srcType = source->getData(DataRole::Type).value<ObjectType>();

    if(srcType == ObjectType::Protagonist) {
        // Does not do anything special, simply checks if the protagonist was the source
        // and then changes its own direction to be the same as before.
        // This encodes enough info to know where to go next. If the door is Up, it goes one level up.
        m_owner->setData(DataRole::Direction, m_owner->getData(DataRole::Direction));
    }

    return true;
}
