# Notes


## Smart Pointers
All QObjects in the implementation are using QPointers as their "smart" pointer, this is nothing more than a guarded raw pointer and can be used interchangeably. Otherwise, it is exactly the same as a raw pointer. The drawback is they can only be used for QObjects (which make the bulk of the application). For all other classes that do not extend QObject (the renderers, most behaviors) a QSharedPointer is used. It is a Qt equivalent to std::shared_ptr but more compatible with Qt out of the box.

The memory management is very simple when using raw/QPointers thanks to the parent/child relationship inherent to QObjects. They are responsible of deleting their children.


