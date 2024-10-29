#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <QPointer>
#include <QSharedPointer>

// Foward declaration of GameObject
class GameObject;

/**
 * @brief The Behavior class is a marker interface (abstract class) that all the behaviors have to extend.
 * This is not a pure interface since it does store one reference to the owner of the behavior.
 */
class Behavior {
public:
    /**
     * @brief Behavior default constructor.
     * @param owner the owner of the behavior.
     */
    Behavior(QPointer<GameObject> owner)
        : m_owner(owner) {};

    ///@{
    /// Interface without default constructor
    virtual ~Behavior() = 0;
    Behavior() = delete;
    Behavior &operator=(const Behavior &) {
        return *this;
    };
    ///@}
protected:
    /**
     * @brief m_owner the GameObject this behavior belongs to.
     */
    QPointer<GameObject> m_owner;
};
#endif // BEHAVIOR_H
