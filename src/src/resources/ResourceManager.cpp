/**
 * @file ResourceManager.cpp
 * @brief Implémentation du gestionnaire central des ressources.
 */

#include "ResourceManager.hpp"

#include <algorithm>  // std::min
#include <iostream>   // std::cout
#include <stdexcept>  // std::out_of_range (lancé par map::at si clé absente)

// ---------------------------------------------------------------------------
// Constructeur
// ---------------------------------------------------------------------------

ResourceManager::ResourceManager()
{
    initResources();
}

// ---------------------------------------------------------------------------
// Initialisation
// ---------------------------------------------------------------------------

void ResourceManager::initResources()
{
    // Réinitialise (ou crée) les trois ressources avec les valeurs par défaut du jeu.
    resources_[ResourceType::WOOD]  = Resource(0, 200);
    resources_[ResourceType::STONE] = Resource(0, 150);
    resources_[ResourceType::GOLD]  = Resource(0, 100);
}

// ---------------------------------------------------------------------------
// Ajout de ressource
// ---------------------------------------------------------------------------

void ResourceManager::addResource(ResourceType type, int amount)
{
    // Ignorer silencieusement les quantités nulles ou négatives.
    if (amount <= 0)
        return;

    Resource& res = resources_.at(type);

    // Plafonner au maximum pour ne jamais dépasser maxCapacity.
    res.current = std::min(res.current + amount, res.maxCapacity);
}

// ---------------------------------------------------------------------------
// Consommation de ressource
// ---------------------------------------------------------------------------

bool ResourceManager::consumeResource(ResourceType type, int amount)
{
    // Une consommation nulle ou négative est toujours considérée réussie.
    if (amount <= 0)
        return true;

    // Vérifier la disponibilité avant toute modification.
    if (!hasEnough(type, amount))
        return false;

    resources_.at(type).current -= amount;
    return true;
}

// ---------------------------------------------------------------------------
// Accesseurs (const)
// ---------------------------------------------------------------------------

int ResourceManager::getAmount(ResourceType type) const
{
    // Utilisation de map::at() en contexte const pour garantir la const-correctness
    // (contrairement à operator[] qui pourrait insérer une entrée).
    return resources_.at(type).current;
}

int ResourceManager::getMaxCapacity(ResourceType type) const
{
    return resources_.at(type).maxCapacity;
}

bool ResourceManager::hasEnough(ResourceType type, int amount) const
{
    return resources_.at(type).current >= amount;
}

bool ResourceManager::isFull(ResourceType type) const
{
    const Resource& res = resources_.at(type);
    return res.current == res.maxCapacity;
}

// ---------------------------------------------------------------------------
// Modification de capacité
// ---------------------------------------------------------------------------

void ResourceManager::setMaxCapacity(ResourceType type, int newMax)
{
    Resource& res = resources_.at(type);
    res.maxCapacity = newMax;

    // Clamp current si la nouvelle capacité est inférieure à la quantité actuelle.
    // Maintient l'invariant : 0 <= current <= maxCapacity.
    res.current = std::min(res.current, res.maxCapacity);
}

// ---------------------------------------------------------------------------
// Affichage debug
// ---------------------------------------------------------------------------

void ResourceManager::printStatus() const
{
    std::cout << "[DEBUG] === ResourceManager Status ===\n";

    for (const auto& [type, res] : resources_)
    {
        // Conversion de l'enum en chaîne lisible pour l'affichage.
        std::string typeName;
        switch (type)
        {
            case ResourceType::WOOD:  typeName = "WOOD";  break;
            case ResourceType::STONE: typeName = "STONE"; break;
            case ResourceType::GOLD:  typeName = "GOLD";  break;
            default:                  typeName = "UNKNOWN"; break;
        }

        std::cout << "[DEBUG] " << typeName << ": "
                  << res.current << "/" << res.maxCapacity << "\n";
    }

    std::cout << "[DEBUG] =====================================\n";
}
