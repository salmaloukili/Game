#ifndef HEALONSTEPBEHAVIOR_H
#define HEALONSTEPBEHAVIOR_H

#include "genericwalkablebehavior.h"

/**
 * @brief The HealOnStepBehavior class calls the heal function from the owner on the source.
 */
class HealOnStepBehavior : public GenericWalkableBehavior {
public:
    /**
     * @brief HealOnStepBehavior default constructor.
     * @param owner of the behavior.
     */
    HealOnStepBehavior(QPointer<GameObject> owner)
        : GenericWalkableBehavior(owner) {};
    HealOnStepBehavior() = delete;

public:
    /**
     * @brief getSteppedOn call the heal method from the owner's health behavior on source.
     * @param source the object stepping on the owner.
     * @return result of GenericWalkableBehavior::getSteppedOn (true)
     */
    bool getSteppedOn(const QPointer<GameObject> &source) override;
};

#endif // HEALONSTEPBEHAVIOR_H
