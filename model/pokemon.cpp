#include "pokemon.h"

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

namespace {
const int MIN_ID_POKEMON_VALUE = 1;
const int MAX_ID_POKEMON_VALUE = 1025;

// JSON keys
constexpr const char *KEY_NAME = "name";
constexpr const char *KEY_ID = "id";
constexpr const char *KEY_HEIGHT = "height";
constexpr const char *KEY_WEIGHT = "weight";
constexpr const char *KEY_BASE_EXPERIENCE = "base_experience";

constexpr const char *KEY_TYPES = "types";
constexpr const char *KEY_TYPE = "type";

constexpr const char *KEY_ABILITIES = "abilities";
constexpr const char *KEY_ABILITY = "ability";

constexpr const char *KEY_SPRITES = "sprites";
constexpr const char *KEY_OTHER = "other";
constexpr const char *KEY_OFFICIAL_ARTWORK = "official-artwork";
constexpr const char *KEY_FRONT_DEFAULT = "front_default";
} // namespace

Pokemon::Pokemon(int id) {
  if (id < MIN_ID_POKEMON_VALUE || id > MAX_ID_POKEMON_VALUE) {
    throw std::runtime_error(
        QString("The id parameter should be between %1 and %2.")
            .arg(MIN_ID_POKEMON_VALUE)
            .arg(MAX_ID_POKEMON_VALUE)
            .toStdString());
  }
  QJsonObject jsonObj = fetchPokemonJson(QString::number(id));
  parseFromJson(jsonObj);
}

Pokemon::Pokemon(const QString &name) {
  QJsonObject jsonObj = fetchPokemonJson(name);
  parseFromJson(jsonObj);
}

QJsonObject Pokemon::fetchPokemonJson(const QString &identifier) {
  QNetworkAccessManager manager;
  const QUrl url =
      QString("https://pokeapi.co/api/v2/pokemon/%1").arg(identifier);
  const QNetworkRequest request(url);
  const std::shared_ptr<QNetworkReply> reply(manager.get(request));

  // Waiting reply finish in a 'synchronous' way
  QEventLoop loop;
  QObject::connect(reply.get(), &QNetworkReply::finished, &loop,
                   &QEventLoop::quit);
  loop.exec();

  if (reply->error() != QNetworkReply::NoError) {
    throw std::runtime_error(
        QString("Invalid parameter: %1").arg(identifier).toStdString());
  }

  const QByteArray rawContent = reply->readAll();
  const QJsonDocument jsonDoc = QJsonDocument::fromJson(rawContent);

  if (!jsonDoc.isObject()) {
    throw std::runtime_error("Response JSON is not an object.");
  }

  return jsonDoc.object();
}

void Pokemon::parseFromJson(const QJsonObject &jsonObj) {
  m_name = jsonObj[KEY_NAME].toString();
  m_id = jsonObj[KEY_ID].toInt();

  m_height = jsonObj[KEY_HEIGHT].toInt();
  m_weight = jsonObj[KEY_WEIGHT].toInt();
  m_baseExperience = jsonObj[KEY_BASE_EXPERIENCE].toInt();

  // Parse types
  m_types.clear();
  const QJsonArray typesArray = jsonObj[KEY_TYPES].toArray();
  for (const auto &typeValue : typesArray) {
    QJsonObject typeObj = typeValue.toObject();
    QJsonObject typeInfo = typeObj[KEY_TYPE].toObject();
    m_types.append(typeInfo[KEY_NAME].toString());
  }

  // Parse abilities
  m_abilities.clear();
  const QJsonArray abilitiesArray = jsonObj[KEY_ABILITIES].toArray();
  for (const auto &abilityValue : std::as_const(abilitiesArray)) {
    QJsonObject abilityObj = abilityValue.toObject();
    QJsonObject abilityInfo = abilityObj[KEY_ABILITY].toObject();
    m_abilities.append(abilityInfo[KEY_NAME].toString());
  }

  // Parse official-artwork sprite
  const QJsonObject spritesObj = jsonObj[KEY_SPRITES].toObject();
  const QJsonObject otherObj = spritesObj[KEY_OTHER].toObject();
  const QJsonObject officialArtworkObj =
      otherObj[KEY_OFFICIAL_ARTWORK].toObject();

  if (!officialArtworkObj.isEmpty()) {
    QNetworkAccessManager imgManager;
    const QUrl imgUrl = officialArtworkObj[KEY_FRONT_DEFAULT].toString();

    const QNetworkRequest imgRequest(imgUrl);
    const std::shared_ptr<QNetworkReply> imgReply(imgManager.get(imgRequest));

    QEventLoop loop;
    QObject::connect(imgReply.get(), &QNetworkReply::finished, &loop,
                     &QEventLoop::quit);
    loop.exec();

    if (imgReply->error() == QNetworkReply::NoError) {
      const QByteArray imgData = imgReply->readAll();
      m_sprite.loadFromData(imgData);
    } else {
      qWarning() << "Failed to load official artwork sprite:"
                 << imgReply->errorString();
    }

    imgReply->deleteLater();
  } else {
    qWarning() << "No official artwork found.";
    m_sprite = QImage(); // vazio
  }
}

// Getters implementation
int Pokemon::id() const { return m_id; }

QString Pokemon::name() const { return m_name; }

QVector<QString> Pokemon::types() const { return m_types; }

QVector<QString> Pokemon::abilities() const { return m_abilities; }

std::optional<int> Pokemon::height() const { return m_height; }

std::optional<int> Pokemon::weight() const { return m_weight; }

std::optional<int> Pokemon::baseExperience() const { return m_baseExperience; }

const QImage &Pokemon::sprite() const { return m_sprite; }
