#ifndef NEWLEVELONSTEP_H
#define NEWLEVELONSTEP_H

#include "genericwalkablebehavior.h"

/**
 * @brief The NewLevelOnStep class handles the stepping on objects that move the protagonist to a new level.
 */
class NewLevelOnStep : public GenericWalkableBehavior {
public:
    /**
     * @brief NewLevelOnStep default constructor
     * @param owner of the behavior
     */
    NewLevelOnStep(QPointer<GameObject> owner)
        : GenericWalkableBehavior(owner) {};
    NewLevelOnStep() = delete;
    /**
     * @brief getSteppedOn if the object stepping on the owner is a protagonist, go to a new level.
     * @param source the object stepping on the owner.
     * @return true
     */
    bool getSteppedOn(const QPointer<GameObject> &source) override;
};

#endif // NEWLEVELONSTEP_H
