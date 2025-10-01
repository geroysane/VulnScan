#ifndef IREPOSITORY_H
#define IREPOSITORY_H

#include <QList>
#include <optional>

/**
 * @interface IRepository
 * @brief Generic repository pattern interface
 *
 * This interface implements the Repository pattern for data persistence.
 * It provides CRUD operations for entities of type T.
 *
 * @tparam T The entity type managed by this repository
 */
template<typename T>
class IRepository {
public:
    virtual ~IRepository() = default;

    /**
     * @brief Saves an entity to the repository
     * @param entity The entity to save
     */
    virtual void save(const T& entity) = 0;

    /**
     * @brief Finds an entity by its ID
     * @param id The entity ID
     * @return Optional containing the entity if found, empty otherwise
     */
    virtual std::optional<T> findById(int id) = 0;

    /**
     * @brief Retrieves all entities from the repository
     * @return List of all entities
     */
    virtual QList<T> findAll() = 0;

    /**
     * @brief Removes an entity by its ID
     * @param id The entity ID to remove
     */
    virtual void remove(int id) = 0;
};

#endif // IREPOSITORY_H
