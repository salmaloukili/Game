#ifndef GENERICMOVEBEHAVIOR_H
#define GENERICMOVEBEHAVIOR_H

#include <model/behaviors/movement.h>
/**
 * @brief The GenericMoveBehavior class handles movement actions.
 */
class GenericMoveBehavior : public Movement {
public:
    /**
     * @brief GenericMoveBehavior default constructor
     * @param owner the owner of the behavior
     */
    GenericMoveBehavior(QPointer<GameObject> owner)
        : Movement(owner) {};
    GenericMoveBehavior() = delete;
    /**
     * @brief stepOn moves on top of a target.
     * The stepOn function checks if everything in the target has a Movement behavior,
     * and if it does it checks that everything is steppable. If either is false,
     * it will not step on the target.
     * @param target to step on.
     * @return true if it was able to.
     */
    bool stepOn(QPointer<GameObject> target) override;
    /**
     * @brief stepOn overrides the stepOn function on a direction.
     * @param direction relative to the owner's position.
     * @return true if it is able to.
     */
    bool stepOn(Direction direction) override;
};

#endif // GENERICMOVEBEHAVIOR_H
