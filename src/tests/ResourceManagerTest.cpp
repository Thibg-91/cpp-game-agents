/**
 * @file ResourceManagerTest.cpp
 * @brief Suite de tests unitaires pour le ResourceManager.
 *
 * Utilise Google Test (GTest) pour valider :
 *   - L'initialisation par défaut des ressources
 *   - L'ajout normal et le plafonnement au maximum
 *   - La consommation réussie, insuffisante et exacte
 *   - Les vérifications hasEnough et isFull
 *   - La modification de capacité (upgrade et downgrade)
 *   - Les cas limites (valeurs nulles, négatives)
 *
 * Chaque TEST instancie un ResourceManager local pour garantir l'isolation complète.
 *
 * Compilation exemple :
 *   g++ -std=c++17 -I../src/resources ResourceManagerTest.cpp
 *       ../src/resources/ResourceManager.cpp
 *       -lgtest -lgtest_main -pthread -o run_tests
 */

#include <gtest/gtest.h>
#include "ResourceManager.hpp"

// ===========================================================================
// Fixture utilitaire
// ===========================================================================

/**
 * @brief Fixture Google Test pour le ResourceManager.
 *
 * Chaque test obtient un ResourceManager fraîchement initialisé via SetUp().
 * Cela garantit l'isolation totale entre les cas de test.
 */
class ResourceManagerTest : public ::testing::Test
{
protected:
    ResourceManager manager; ///< Instance fraîche pour chaque test

    void SetUp() override
    {
        // initResources() est déjà appelé dans le constructeur,
        // mais on le rappelle explicitement pour la clarté.
        manager.initResources();
    }
};

// ===========================================================================
// Tests d'initialisation
// ===========================================================================

/**
 * @brief Vérifie que les ressources sont initialisées avec les bonnes valeurs par défaut.
 *
 * Valeurs attendues :
 *   WOOD  : current=0, maxCapacity=200
 *   STONE : current=0, maxCapacity=150
 *   GOLD  : current=0, maxCapacity=100
 */
TEST_F(ResourceManagerTest, InitResourcesDefaultValues)
{
    // --- WOOD ---
    EXPECT_EQ(manager.getAmount(ResourceType::WOOD), 0);
    EXPECT_EQ(manager.getMaxCapacity(ResourceType::WOOD), 200);

    // --- STONE ---
    EXPECT_EQ(manager.getAmount(ResourceType::STONE), 0);
    EXPECT_EQ(manager.getMaxCapacity(ResourceType::STONE), 150);

    // --- GOLD ---
    EXPECT_EQ(manager.getAmount(ResourceType::GOLD), 0);
    EXPECT_EQ(manager.getMaxCapacity(ResourceType::GOLD), 100);
}

// ===========================================================================
// Tests d'ajout de ressource
// ===========================================================================

/**
 * @brief Vérifie qu'un ajout normal (sans dépasser la capacité) fonctionne correctement.
 */
TEST_F(ResourceManagerTest, AddResourceNormalCase)
{
    manager.addResource(ResourceType::WOOD, 50);
    EXPECT_EQ(manager.getAmount(ResourceType::WOOD), 50);

    // Ajout supplémentaire
    manager.addResource(ResourceType::WOOD, 30);
    EXPECT_EQ(manager.getAmount(ResourceType::WOOD), 80);
}

/**
 * @brief Vérifie que l'ajout est plafonné à la capacité maximale.
 *
 * Cas : ajout de 300 bois avec maxCapacity=200 → doit rester à 200.
 */
TEST_F(ResourceManagerTest, AddResourceCapsAtMaxCapacity)
{
    manager.addResource(ResourceType::WOOD, 300); // Dépasse le max de 200
    EXPECT_EQ(manager.getAmount(ResourceType::WOOD), 200);
    EXPECT_EQ(manager.getMaxCapacity(ResourceType::WOOD), 200);
}

/**
 * @brief Vérifie que l'ajout sur un stockage déjà plein ne dépasse pas le maximum.
 */
TEST_F(ResourceManagerTest, AddResourceAlreadyFull)
{
    // Remplir à fond
    manager.addResource(ResourceType::GOLD, 100);
    ASSERT_TRUE(manager.isFull(ResourceType::GOLD));

    // Tenter d'ajouter davantage
    manager.addResource(ResourceType::GOLD, 50);
    EXPECT_EQ(manager.getAmount(ResourceType::GOLD), 100); // Toujours au max
}

// ===========================================================================
// Tests de consommation de ressource
// ===========================================================================

/**
 * @brief Vérifie qu'une consommation réussie déduit correctement la quantité
 *        et retourne true.
 */
TEST_F(ResourceManagerTest, ConsumeResourceSuccess)
{
    manager.addResource(ResourceType::STONE, 100);

    const bool result = manager.consumeResource(ResourceType::STONE, 40);

    EXPECT_TRUE(result);
    EXPECT_EQ(manager.getAmount(ResourceType::STONE), 60);
}

/**
 * @brief Vérifie qu'une consommation avec stock insuffisant retourne false
 *        et ne modifie pas le stock.
 */
TEST_F(ResourceManagerTest, ConsumeResourceInsufficientAmount)
{
    manager.addResource(ResourceType::GOLD, 30);

    const bool result = manager.consumeResource(ResourceType::GOLD, 50); // Plus que disponible

    EXPECT_FALSE(result);
    EXPECT_EQ(manager.getAmount(ResourceType::GOLD), 30); // Stock inchangé
}

/**
 * @brief Vérifie qu'une consommation exactement égale au stock réussit
 *        et laisse le stock à 0.
 */
TEST_F(ResourceManagerTest, ConsumeResourceExactAmount)
{
    manager.addResource(ResourceType::WOOD, 150);

    const bool result = manager.consumeResource(ResourceType::WOOD, 150);

    EXPECT_TRUE(result);
    EXPECT_EQ(manager.getAmount(ResourceType::WOOD), 0);
}

// ===========================================================================
// Tests de hasEnough
// ===========================================================================

/**
 * @brief Vérifie que hasEnough retourne true quand le stock est suffisant.
 */
TEST_F(ResourceManagerTest, HasEnoughTrue)
{
    manager.addResource(ResourceType::STONE, 80);

    EXPECT_TRUE(manager.hasEnough(ResourceType::STONE, 80)); // Exact
    EXPECT_TRUE(manager.hasEnough(ResourceType::STONE, 50)); // Inférieur
    EXPECT_TRUE(manager.hasEnough(ResourceType::STONE, 0));  // Zéro
}

/**
 * @brief Vérifie que hasEnough retourne false quand le stock est insuffisant.
 */
TEST_F(ResourceManagerTest, HasEnoughFalse)
{
    manager.addResource(ResourceType::STONE, 30);

    EXPECT_FALSE(manager.hasEnough(ResourceType::STONE, 31)); // Juste au-dessus
    EXPECT_FALSE(manager.hasEnough(ResourceType::STONE, 100));
}

// ===========================================================================
// Tests de isFull
// ===========================================================================

/**
 * @brief Vérifie que isFull retourne true quand le stockage est exactement plein.
 */
TEST_F(ResourceManagerTest, IsFullTrue)
{
    // Remplir GOLD (maxCapacity=100)
    manager.addResource(ResourceType::GOLD, 100);
    EXPECT_TRUE(manager.isFull(ResourceType::GOLD));
}

/**
 * @brief Vérifie que isFull retourne false quand le stockage n'est pas plein.
 */
TEST_F(ResourceManagerTest, IsFullFalse)
{
    // WOOD est vide par défaut
    EXPECT_FALSE(manager.isFull(ResourceType::WOOD));

    // Partiellement rempli
    manager.addResource(ResourceType::WOOD, 100);
    EXPECT_FALSE(manager.isFull(ResourceType::WOOD)); // max est 200
}

// ===========================================================================
// Tests de setMaxCapacity
// ===========================================================================

/**
 * @brief Vérifie qu'un upgrade de capacité augmente correctement le maximum
 *        sans modifier le stock actuel.
 */
TEST_F(ResourceManagerTest, SetMaxCapacityUpgrade)
{
    manager.addResource(ResourceType::STONE, 100);
    manager.setMaxCapacity(ResourceType::STONE, 300); // Augmenter la capacité

    EXPECT_EQ(manager.getMaxCapacity(ResourceType::STONE), 300);
    EXPECT_EQ(manager.getAmount(ResourceType::STONE), 100); // Stock inchangé

    // On peut maintenant ajouter jusqu'à 300
    manager.addResource(ResourceType::STONE, 200);
    EXPECT_EQ(manager.getAmount(ResourceType::STONE), 300);
}

/**
 * @brief Vérifie que réduire la capacité en dessous du stock actuel
 *        clamp le stock à la nouvelle capacité.
 *
 * Politique de cohérence : current = min(current, newMax)
 */
TEST_F(ResourceManagerTest, SetMaxCapacityBelowCurrentClampsAmount)
{
    // Remplir GOLD à 80 (max=100)
    manager.addResource(ResourceType::GOLD, 80);
    ASSERT_EQ(manager.getAmount(ResourceType::GOLD), 80);

    // Réduire la capacité à 50 (inférieur au stock actuel de 80)
    manager.setMaxCapacity(ResourceType::GOLD, 50);

    EXPECT_EQ(manager.getMaxCapacity(ResourceType::GOLD), 50);
    // Le stock doit être clampé à la nouvelle capacité
    EXPECT_EQ(manager.getAmount(ResourceType::GOLD), 50);
    // L'invariant doit être respecté
    EXPECT_TRUE(manager.isFull(ResourceType::GOLD));
}

// ===========================================================================
// Tests des cas limites
// ===========================================================================

/**
 * @brief Vérifie qu'ajouter 0 unité ne modifie pas le stock.
 */
TEST_F(ResourceManagerTest, AddZeroAmountNoChange)
{
    manager.addResource(ResourceType::WOOD, 50);
    manager.addResource(ResourceType::WOOD, 0); // Doit être ignoré

    EXPECT_EQ(manager.getAmount(ResourceType::WOOD), 50);
}

/**
 * @brief Vérifie que consommer 0 unité retourne toujours true sans modifier le stock.
 */
TEST_F(ResourceManagerTest, ConsumeZeroAmountAlwaysTrue)
{
    // Stock vide
    EXPECT_TRUE(manager.consumeResource(ResourceType::STONE, 0));
    EXPECT_EQ(manager.getAmount(ResourceType::STONE), 0); // Inchangé

    // Stock non vide
    manager.addResource(ResourceType::STONE, 50);
    EXPECT_TRUE(manager.consumeResource(ResourceType::STONE, 0));
    EXPECT_EQ(manager.getAmount(ResourceType::STONE), 50); // Inchangé
}

/**
 * @brief Vérifie qu'ajouter une valeur négative est ignoré silencieusement.
 */
TEST_F(ResourceManagerTest, AddNegativeAmountIgnored)
{
    manager.addResource(ResourceType::GOLD, 30);
    manager.addResource(ResourceType::GOLD, -10); // Doit être ignoré

    EXPECT_EQ(manager.getAmount(ResourceType::GOLD), 30);
}

/**
 * @brief Vérifie que consommer une valeur négative retourne true sans modifier le stock.
 */
TEST_F(ResourceManagerTest, ConsumeNegativeAmountAlwaysTrue)
{
    manager.addResource(ResourceType::WOOD, 60);

    const bool result = manager.consumeResource(ResourceType::WOOD, -20);

    EXPECT_TRUE(result);
    EXPECT_EQ(manager.getAmount(ResourceType::WOOD), 60); // Inchangé
}

/**
 * @brief Vérifie que le plafonnement fonctionne avec une accumulation progressive.
 *
 * Cas : plusieurs ajouts successifs qui dépassent collectivement le maximum.
 */
TEST_F(ResourceManagerTest, AddResourceMultipleAddsCappedCorrectly)
{
    // GOLD maxCapacity = 100
    manager.addResource(ResourceType::GOLD, 70);
    EXPECT_EQ(manager.getAmount(ResourceType::GOLD), 70);

    manager.addResource(ResourceType::GOLD, 70); // 70+70=140 > 100 → plafonné à 100
    EXPECT_EQ(manager.getAmount(ResourceType::GOLD), 100);
    EXPECT_TRUE(manager.isFull(ResourceType::GOLD));
}

/**
 * @brief Vérifie que les ressources sont indépendantes les unes des autres.
 *
 * Modifier une ressource ne doit pas affecter les autres.
 */
TEST_F(ResourceManagerTest, ResourcesAreIndependent)
{
    manager.addResource(ResourceType::WOOD, 100);
    manager.addResource(ResourceType::GOLD, 50);

    // STONE ne doit pas être affectée
    EXPECT_EQ(manager.getAmount(ResourceType::STONE), 0);

    // Consommer du bois ne doit pas toucher l'or
    manager.consumeResource(ResourceType::WOOD, 40);
    EXPECT_EQ(manager.getAmount(ResourceType::WOOD), 60);
    EXPECT_EQ(manager.getAmount(ResourceType::GOLD), 50); // Inchangé
}

// ===========================================================================
// Point d'entrée des tests (si pas d'utilisation de gtest_main)
// ===========================================================================
// Note : Si vous liez avec -lgtest_main, ce main() est fourni automatiquement.
// Décommentez si vous liez uniquement avec -lgtest.
//
// int main(int argc, char** argv)
// {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
