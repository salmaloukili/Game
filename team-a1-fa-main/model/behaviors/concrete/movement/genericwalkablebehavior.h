#ifndef GENERICWALKABLEBEHAVIOR_H
#define GENERICWALKABLEBEHAVIOR_H

#include <model/behaviors/movement.h>
/**
 * @brief The GenericWalkableBehavior class simply returns true on the function isSteppable.
 */
class GenericWalkableBehavior : public Movement {
public:
    /**
     * @brief GenericWalkableBehavior default constructor.
     * @param owner of the behavior.
     */
    GenericWalkableBehavior(QPointer<GameObject> owner)
        : Movement(owner) {};
    GenericWalkableBehavior() = delete;

public:
    /**
     * @brief isSteppable simply return true to mark the owner as steppable.
     * @return true.
     */
    bool isSteppable() override { return true; };
};

#endif // GENERICWALKABLEBEHAVIOR_H
