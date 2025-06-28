#include "pokemon.h"
#include "json_keys.h"

#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include <stdexcept>

Pokemon::Pokemon(int id)
{
    if (id < PokemonConstants::MIN_ID || id > PokemonConstants::MAX_ID) {
        throw std::runtime_error(QString("The id parameter should be between %1 and %2.")
                                     .arg(PokemonConstants::MIN_ID)
                                     .arg(PokemonConstants::MAX_ID)
                                     .toStdString());
    }
    QJsonObject jsonObj = fetchPokemonJson(QString::number(id));
    parseFromJson(jsonObj);
}

Pokemon::Pokemon(const QString &name)
{
    QJsonObject jsonObj = fetchPokemonJson(name);
    parseFromJson(jsonObj);
}

QJsonObject Pokemon::fetchPokemonJson(const QString &identifier)
{
    QNetworkAccessManager manager;
    const QUrl url = QString("https://pokeapi.co/api/v2/pokemon/%1").arg(identifier);
    const QNetworkRequest request(url);
    const std::shared_ptr<QNetworkReply> reply(manager.get(request));

    // Waiting reply finish in a 'synchronous' way
    QEventLoop loop;
    QObject::connect(reply.get(), &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        throw std::runtime_error(QString("Invalid parameter: %1").arg(identifier).toStdString());
    }

    const QByteArray rawContent = reply->readAll();
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(rawContent);

    if (!jsonDoc.isObject()) {
        throw std::runtime_error("Response JSON is not an object.");
    }

    return jsonDoc.object();
}

void Pokemon::parseFromJson(const QJsonObject &jsonObj)
{
    m_name = jsonObj[JsonKeys::Pokemon::NAME].toString();
    m_id = jsonObj[JsonKeys::Pokemon::ID].toInt();

    m_height = jsonObj[JsonKeys::Pokemon::HEIGHT].toInt();
    m_weight = jsonObj[JsonKeys::Pokemon::WEIGHT].toInt();
    m_baseExperience = jsonObj[JsonKeys::Pokemon::BASE_EXPERIENCE].toInt();

    // Parse types
    m_types.clear();
    const QJsonArray typesArray = jsonObj[JsonKeys::Pokemon::TYPES].toArray();
    for (const auto &typeValue : typesArray) {
        QJsonObject typeObj = typeValue.toObject();
        QJsonObject typeInfo = typeObj[JsonKeys::Pokemon::TYPE].toObject();
        m_types.append(typeInfo[JsonKeys::Pokemon::NAME].toString());
    }

    // Parse abilities
    m_abilities.clear();
    const QJsonArray abilitiesArray = jsonObj[JsonKeys::Pokemon::ABILITIES].toArray();
    for (const auto &abilityValue : std::as_const(abilitiesArray)) {
        QJsonObject abilityObj = abilityValue.toObject();
        QJsonObject abilityInfo = abilityObj[JsonKeys::Pokemon::ABILITY].toObject();
        m_abilities.append(abilityInfo[JsonKeys::Pokemon::NAME].toString());
    }

    // Parse official-artwork sprite
    const QJsonObject spritesObj = jsonObj[JsonKeys::Pokemon::SPRITES].toObject();
    const QJsonObject otherObj = spritesObj[JsonKeys::Pokemon::OTHER].toObject();
    const QJsonObject officialArtworkObj = otherObj[JsonKeys::Pokemon::OFFICIAL_ARTWORK].toObject();

    if (!officialArtworkObj.isEmpty()) {
        QNetworkAccessManager imgManager;
        const QUrl imgUrl = officialArtworkObj[JsonKeys::Pokemon::FRONT_DEFAULT].toString();

        const QNetworkRequest imgRequest(imgUrl);
        const std::shared_ptr<QNetworkReply> imgReply(imgManager.get(imgRequest));

        QEventLoop loop;
        QObject::connect(imgReply.get(), &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (imgReply->error() == QNetworkReply::NoError) {
            const QByteArray imgData = imgReply->readAll();
            m_sprite.loadFromData(imgData);
        } else {
            qWarning() << "Failed to load official artwork sprite:" << imgReply->errorString();
        }

        imgReply->deleteLater();
    } else {
        qWarning() << "No official artwork found.";
        m_sprite = QImage(); // vazio
    }
}

// Getters implementation
int Pokemon::id() const
{
    return m_id;
}

QString Pokemon::name() const
{
    return m_name;
}

QVector<QString> Pokemon::types() const
{
    return m_types;
}

QVector<QString> Pokemon::abilities() const
{
    return m_abilities;
}

std::optional<int> Pokemon::height() const
{
    return m_height;
}

std::optional<int> Pokemon::weight() const
{
    return m_weight;
}

std::optional<int> Pokemon::baseExperience() const
{
    return m_baseExperience;
}

const QImage &Pokemon::sprite() const
{
    return m_sprite;
}
