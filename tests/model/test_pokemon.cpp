#include <QtTest/QtTest>

#include "../model/pokemon.h"

class TestPokemon : public QObject
{
    Q_OBJECT

private slots:
    void testCreateById_valid();
    void testCreateByName_valid();
    void testCreateById_invalid();
    void testCreateByName_invalid();
};

static void assertIsValidPikachu(const Pokemon &p)
{
    QCOMPARE(p.id(), 25);
    QCOMPARE(p.name(), QString("pikachu"));

    QVERIFY(p.height().has_value());
    QCOMPARE(p.height().value(), 4);

    QVERIFY(p.weight().has_value());
    QCOMPARE(p.weight().value(), 60);

    QVERIFY(p.baseExperience().has_value());
    QCOMPARE(p.baseExperience().value(), 112);

    QVERIFY(!p.types().isEmpty());
    QVERIFY(p.types().contains("electric"));

    QVERIFY(!p.abilities().isEmpty());
    QVERIFY(p.abilities().contains("static"));
    QVERIFY(p.abilities().contains("lightning-rod"));

    QVERIFY(!p.sprite().isNull());
}

void TestPokemon::testCreateById_valid()
{
    Pokemon p(25);
    assertIsValidPikachu(p);
}

void TestPokemon::testCreateByName_valid()
{
    Pokemon p("pikachu");
    assertIsValidPikachu(p);
}

void TestPokemon::testCreateById_invalid()
{
    QVERIFY_THROWS_EXCEPTION(std::runtime_error, Pokemon(PokemonConstants::MIN_ID - 1));
    QVERIFY_THROWS_EXCEPTION(std::runtime_error, Pokemon(PokemonConstants::MAX_ID + 1));
}

void TestPokemon::testCreateByName_invalid()
{
    QVERIFY_THROWS_EXCEPTION(std::runtime_error, Pokemon("invalidname"));
}

QTEST_MAIN(TestPokemon)
#include "test_pokemon.moc"
