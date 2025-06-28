#ifndef POKEMON_H
#define POKEMON_H

#include <QImage>
#include <QJsonObject>
#include <QString>
#include <QVector>
#include <optional>

class Pokemon {
public:
  Pokemon() = delete;

  explicit Pokemon(int id);

  explicit Pokemon(const QString &name);

  int id() const;

  QString name() const;

  QVector<QString> types() const;

  QVector<QString> abilities() const;

  std::optional<int> height() const;

  std::optional<int> weight() const;

  std::optional<int> baseExperience() const;

  const QImage &sprite() const;

private:
  int m_id;
  QString m_name;
  QVector<QString> m_types;
  QVector<QString> m_abilities;
  std::optional<int> m_height;
  std::optional<int> m_weight;
  std::optional<int> m_baseExperience;
  QImage m_sprite;

  QJsonObject fetchPokemonJson(const QString &identifier);
  void parseFromJson(const QJsonObject &jsonObj);
};

#endif // POKEMON_H
