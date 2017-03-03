
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 */

#include <QObject>
#include <QMetaObject>

#include <QDebug>

#include "reactmoduleinterface.h"
#include "reactmoduledata.h"
#include "reactmodulemethod.h"


namespace
{
static int nextModuleId = 1;
// TODO: sort out all the issues around methodsToExport

QList<ReactModuleMethod*> buildMethodList(QObject* moduleImpl)
{
  const QMetaObject* metaObject = moduleImpl->metaObject();
  const int methodCount = metaObject->methodCount();

  QList<ReactModuleMethod*> methods;

  // from methodsToExport
  ReactModuleInterface* rmi = qobject_cast<ReactModuleInterface*>(moduleImpl);
  methods = rmi->methodsToExport();

  for (int i = metaObject->methodOffset(); i < methodCount; ++i) {
    QMetaMethod m = metaObject->method(i);
    if (m.methodType() == QMetaMethod::Method)
      methods << new ReactModuleMethod(metaObject->method(i),
                                       [moduleImpl](QVariantList&) {
                                         return moduleImpl;
                                       });
  }

  return methods;
}

QVariantMap buildConstantMap(QObject* moduleImpl)
{
  const QMetaObject* metaObject = moduleImpl->metaObject();
  const int propertyCount = metaObject->propertyCount();

  QVariantMap constants;

  // from constantsToExport
  ReactModuleInterface* rmi = qobject_cast<ReactModuleInterface*>(moduleImpl);
  constants = rmi->constantsToExport();

  // CONSTANT properties (limited usage?) - overrides values from constantsToExport
  for (int i = metaObject->propertyOffset(); i < propertyCount; ++i) {
    QMetaProperty p = metaObject->property(i);
    if (p.isConstant())
      constants.insert(p.name(), p.read(moduleImpl));
  }

  return constants;
}
}

class ReactModuleDataPrivate
{
public:
  int id;
  QObject* moduleImpl;
  QVariantMap constants;
  QList<ReactModuleMethod*> methods;
};

ReactModuleData::ReactModuleData(QObject* moduleImpl)
  : d_ptr(new ReactModuleDataPrivate)
{
  Q_D(ReactModuleData);
  d->id = nextModuleId++;
  d->moduleImpl = moduleImpl;
  d->constants = buildConstantMap(moduleImpl);
  d->methods = buildMethodList(moduleImpl);
}

ReactModuleData::~ReactModuleData()
{
  Q_D(ReactModuleData);
  d->moduleImpl->deleteLater();
}

int ReactModuleData::id() const
{
  return d_func()->id;
}

QString ReactModuleData::name() const
{
  return normalizeName(qobject_cast<ReactModuleInterface*>(d_func()->moduleImpl)->moduleName());
}

// Format:
// moduleName, constants, methods, promiseMethods, syncMethods
// constants is an object
// methods is the list of methods' names
// promisMethods and syncMethods are lists contain the ids of the methods
QVariant ReactModuleData::info() const
{
  Q_D(const ReactModuleData);

  QVariantList config;
  config.append(name());  // name first

  if (!d->constants.isEmpty())
    config.append(d->constants); // constants second
  else
    config.append(QVariantMap{});

  QVariantList methodConfig;
  QVariantList promiseMethods;
  QVariantList syncMethods;
  for (int i = 0; i < d->methods.size(); ++i) {
    auto method = d->methods.at(i);
    methodConfig.append(method->name());
    if (method->type() == "remote") syncMethods.append(i);
    if (method->type() == "remoteAsync") promiseMethods.append(i);
  }
  if (!methodConfig.isEmpty()) {
    config.append((QVariant)methodConfig);  // others
    config.append((QVariant)promiseMethods);
//    config.append((QVariant)syncMethods); // sync need nativeCallSyncHook support
  }

  return config;
}

ReactModuleMethod* ReactModuleData::method(int id) const
{
  // assert bounds
  return d_func()->methods.value(id);
}

ReactViewManager* ReactModuleData::viewManager() const
{
  return qobject_cast<ReactModuleInterface*>(d_func()->moduleImpl)->viewManager();
}

QString ReactModuleData::normalizeName(QString name) {
  if (name.startsWith("RCT")) return name.mid(3);
  if (name.startsWith("RK")) return name.mid(2);
  return name;
}

