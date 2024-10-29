#ifndef POISONONSTEPBEHAVIOR_H
#define POISONONSTEPBEHAVIOR_H

#include "genericwalkablebehavior.h"

/**
 * @brief The PoisonOnStepBehavior class makes the object poison the source if it has any poison.
 */
class PoisonOnStepBehavior : public GenericWalkableBehavior {
public:
    /**
     * @brief PoisonOnStepBehavior default constructor
     * @param owner of the behavior
     */
    PoisonOnStepBehavior(QPointer<GameObject> owner)
        : GenericWalkableBehavior(owner) {};
    PoisonOnStepBehavior() = delete;

public:
    /**
     * @brief getSteppedOn overrides the default walkable function and poisons if the owner has any.
     * @param source object stepping on the owner.
     * @return result of GenericWalkableBehavior::getSteppedOn
     */
    bool getSteppedOn(const QPointer<GameObject> &source) override;
};

#endif // POISONONSTEPBEHAVIOR_H
