/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * ***** END LICENSE BLOCK ***** */

/*
 * Copyright (c) 2014, Filip Brcic <brcha@gna.org>. All rights reserved.
 *
 * This file is part of lusim
 */

#ifndef QTYAML_H
#define QTYAML_H

#include <yaml-cpp/yaml.h>

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QVariant>

namespace YAML {

// QString
template<>
struct convert<QString>
{
    static Node encode(const QString& rhs)
    {
        return Node(rhs.toStdString());
    }

    static bool decode(const Node& node, QString& rhs)
    {
        if (!node.IsScalar())
            return false;
        rhs = QString::fromStdString(node.Scalar());
        return true;
    }
};

// QMap
template<typename Key, typename Value>
struct convert<QMap<Key, Value> >
{
    static Node encode(const QMap<Key,Value>& rhs)
    {
        Node node(NodeType::Map);
        auto it = rhs.constBegin();
        while (it != rhs.constEnd())
        {
            node.force_insert(it.key(), it.value());
            ++it;
        }
        return node;
    }

    static bool decode(const Node& node, QMap<Key, Value>& rhs)
    {
        if (!node.IsMap())
            return false;

        rhs.clear();
        const_iterator it = node.begin();
        while (it != node.end())
        {
            rhs[it->first.as<Key>()] = it->second.as<Value>();
            ++it;
        }
        return true;
    }
};

// QVector
template<typename T>
struct convert<QVector<T> >
{
    static Node encode(const QVector<T>& rhs)
    {
        Node node(NodeType::Sequence);
        foreach (T value, rhs) {
            node.push_back(value);
        }
        return node;
    }

    static bool decode(const Node& node, QVector<T>& rhs)
    {
        if (!node.IsSequence())
            return false;

        rhs.clear();
        const_iterator it = node.begin();
        while (it != node.end())
        {
            rhs.push_back(it->as<T>());
            ++it;
        }
        return true;
    }
};

// QList
template<typename T>
struct convert<QList<T> >
{
    static Node encode(const QList<T>& rhs)
    {
        Node node(NodeType::Sequence);
        foreach (T value, rhs) {
            node.push_back(value);
        }
        return node;
    }

    static bool decode(const Node& node, QList<T>& rhs)
    {
        if (!node.IsSequence())
            return false;

        rhs.clear();
        const_iterator it = node.begin();
        while (it != node.end())
        {
            rhs.push_back(it->as<T>());
            ++it;
        }
        return true;
    }
};

// QPair
template<typename T, typename U>
struct convert<QPair<T,U> >
{
    static Node encode(const QPair<T,U>& rhs)
    {
        Node node(NodeType::Sequence);
        node.push_back(rhs.first);
        node.push_back(rhs.second);
        return node;
    }

    static bool decode(const Node& node, QPair<T,U>& rhs)
    {
        if (!node.IsSequence())
            return false;
        if (node.size() != 2)
            return false;

        rhs.first = node[0].as<T>();
        rhs.second = node[1].as<U>();
        return true;
    }
};

// QVariant
template<>
struct convert<QVariant>
{
  static Node encode(const QVariant& rhs)
  {
    switch (rhs.type()) {
    case QMetaType::Bool:
      return Node(rhs.toBool());
    case QMetaType::Int:
      return Node(rhs.toInt());
    case QMetaType::QString:
      return Node(rhs.toString());
    case QMetaType::QVariantList:
      return Node(rhs.toList());
    case QMetaType::QVariantMap:
      return Node(rhs.toMap());
    }

    return Node();
  }

  static bool decode(const Node& node, QVariant& rhs)
  {
    if (node.IsScalar()) {
      rhs = node.Scalar().c_str();
    } else if (node.IsMap()) {
      rhs = node.as<QMap<QString, QVariant>>();
    } else if (node.IsSequence()) {
      rhs = node.as<QList<QVariant>>();
    } else if (node.IsNull()) {
      rhs.clear();
    } else if (node.IsDefined()) {
      rhs.clear();
    } else {
      rhs = node.as<QVariant>();
    }
    return true;
  }
};

template<typename T>
inline T variantTo(const QVariant& var)
{
  return var;
}

template<>
inline int variantTo(const QVariant& var)
{
  return var.toInt();
}

template<>
inline bool variantTo(const QVariant& var)
{
  return var.toBool();
}

template<>
inline QString variantTo(const QVariant& var)
{
  return var.toString();
}

// TODO: Add the rest of the container classes
// QLinkedList, QStack, QQueue, QSet, QMultiMap, QHash, QMultiHash, QStringList, ...

} // end namespace YAML

#endif // QTYAML_H
