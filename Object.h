#ifndef TITANPLUSPLUS_OBJECT_H
#define TITANPLUSPLUS_OBJECT_H

/**
 * @file Object.h
 * @author Bryn McKerracher
 * @date 14/10/2021
 * @brief The Object class, representing a general heap-allocated object.
 */

#include <memory>

typedef std::shared_ptr<class Object> SharedObject; ///< Alias for a shared pointer to an object.

/**
 * @brief Represents a heap-allocated object.
 * @class Object
 */
struct Object {

};

#endif //TITANPLUSPLUS_OBJECT_H
