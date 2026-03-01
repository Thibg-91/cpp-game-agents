/**
 * @file Resource.hpp
 * @brief Structure représentant une ressource avec sa quantité courante
 *        et sa capacité maximale de stockage.
 *
 * Inclut ResourceType.hpp pour respecter la hiérarchie des dépendances.
 */

#pragma once

#include "ResourceType.hpp"

/**
 * @struct Resource
 * @brief Contient l'état d'une ressource : quantité actuelle et capacité maximale.
 *
 * Invariant : 0 <= current <= maxCapacity
 * Cet invariant est maintenu par le ResourceManager via ses méthodes publiques.
 */
struct Resource
{
    int current;      ///< Quantité actuellement disponible en stockage
    int maxCapacity;  ///< Capacité maximale du stockage pour cette ressource

    /**
     * @brief Constructeur avec valeurs par défaut.
     * @param current     Quantité initiale (défaut : 0)
     * @param maxCapacity Capacité maximale initiale (défaut : 100)
     */
    explicit Resource(int current = 0, int maxCapacity = 100)
        : current(current)
        , maxCapacity(maxCapacity)
    {
    }
};
