/**
 * @file ResourceManager.hpp
 * @brief Déclaration du gestionnaire central des ressources du jeu.
 *
 * Le ResourceManager encapsule une map<ResourceType, Resource> et expose
 * toutes les opérations nécessaires : ajout, consommation, vérification
 * et configuration des capacités de stockage.
 *
 * Hiérarchie des inclusions :
 *   ResourceManager.hpp → Resource.hpp → ResourceType.hpp
 */

#pragma once

#include <map>
#include "Resource.hpp"

/**
 * @class ResourceManager
 * @brief Gestionnaire central de toutes les ressources du jeu.
 *
 * Règles de comportement importantes :
 * - addResource    : plafonne au maximum (std::min)
 * - consumeResource: vérifie avant de déduire, retourne false si insuffisant
 * - setMaxCapacity : si newMax < current, current est clampé à newMax
 * - Valeurs négatives passées à add/consume sont ignorées silencieusement
 */
class ResourceManager
{
public:
    /**
     * @brief Constructeur par défaut.
     *        Appelle initResources() pour initialiser les ressources avec
     *        les valeurs par défaut du jeu.
     */
    ResourceManager();

    /**
     * @brief Initialise les trois ressources avec leurs valeurs par défaut.
     *
     * Valeurs initiales :
     *   - WOOD  : current=0, maxCapacity=200
     *   - STONE : current=0, maxCapacity=150
     *   - GOLD  : current=0, maxCapacity=100
     *
     * Cette méthode peut être rappelée pour réinitialiser l'état (reset de partie).
     */
    void initResources();

    /**
     * @brief Ajoute une quantité à une ressource sans dépasser sa capacité maximale.
     *
     * Si (current + amount) dépasse maxCapacity, current est plafonné à maxCapacity.
     * Les valeurs amount <= 0 sont ignorées silencieusement.
     *
     * @param type   Type de ressource cible
     * @param amount Quantité à ajouter (doit être > 0)
     */
    void addResource(ResourceType type, int amount);

    /**
     * @brief Consomme une quantité d'une ressource si le stock est suffisant.
     *
     * Vérifie préalablement que current >= amount avant de déduire.
     * Les valeurs amount <= 0 sont considérées comme toujours satisfaites (retourne true).
     *
     * @param type   Type de ressource cible
     * @param amount Quantité à consommer
     * @return true  si la consommation a réussi (ressource déduite)
     * @return false si le stock est insuffisant (aucune modification)
     */
    bool consumeResource(ResourceType type, int amount);

    /**
     * @brief Retourne la quantité actuelle d'une ressource.
     * @param type Type de ressource
     * @return Quantité courante (int)
     */
    int getAmount(ResourceType type) const;

    /**
     * @brief Retourne la capacité maximale de stockage d'une ressource.
     * @param type Type de ressource
     * @return Capacité maximale (int)
     */
    int getMaxCapacity(ResourceType type) const;

    /**
     * @brief Modifie la capacité maximale de stockage d'une ressource.
     *
     * Si newMax < current, current est clampé à newMax pour maintenir
     * la cohérence de l'invariant (current <= maxCapacity).
     *
     * @param type   Type de ressource
     * @param newMax Nouvelle capacité maximale
     */
    void setMaxCapacity(ResourceType type, int newMax);

    /**
     * @brief Vérifie si le stock d'une ressource est suffisant pour une quantité donnée.
     * @param type   Type de ressource
     * @param amount Quantité requise
     * @return true si current >= amount, false sinon
     */
    bool hasEnough(ResourceType type, int amount) const;

    /**
     * @brief Vérifie si le stockage d'une ressource est plein.
     * @param type Type de ressource
     * @return true si current == maxCapacity, false sinon
     */
    bool isFull(ResourceType type) const;

    /**
     * @brief Affiche l'état de toutes les ressources sur la sortie standard (debug).
     *
     * Format : [DEBUG] TYPE: current/maxCapacity
     */
    void printStatus() const;

private:
    /// Map stockant l'état de chaque ressource, indexée par ResourceType.
    std::map<ResourceType, Resource> resources_;
};
