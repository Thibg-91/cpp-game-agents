/**
 * @file ResourceType.hpp
 * @brief Enumération fortement typée définissant les types de ressources du jeu.
 *
 * Utilise enum class (C++11/17) pour éviter les collisions de noms
 * et forcer le typage explicite lors de l'utilisation.
 */

#pragma once

/**
 * @enum ResourceType
 * @brief Les trois types de ressources disponibles dans le jeu de gestion 2D.
 */
enum class ResourceType
{
    WOOD,   ///< Bois : ressource de base pour la construction
    STONE,  ///< Pierre : ressource intermédiaire pour les bâtiments avancés
    GOLD    ///< Or : ressource rare pour les améliorations et le commerce
};
