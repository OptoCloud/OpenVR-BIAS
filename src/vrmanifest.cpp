#include "vrmanifest.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

#include "vraction.h"
#include "vractionset.h"
#include "vrbinding.h"

VRManifest::VRManifest(QObject* parent)
	: QObject(parent)
{

}

bool VRManifest::Save(const QString& path)
{
	QFile file(path);
	if (!file.open(QFile::ReadWrite)) return false;
	file.write(QJsonDocument(toJsonObject()).toJson());
	return true;
}

bool VRManifest::Load(const QString& path)
{
	QByteArray data;

	{
		QFile file(path);
		if (!file.open(QFile::ReadOnly)) return false;
		data = file.readAll();
	}

	QJsonParseError err;
	QJsonDocument document = QJsonDocument::fromJson(data, &err);

	if (err.error != QJsonParseError::NoError)
	{
		qWarning() << "Error parsing vrmanifest:" << err.errorString();
		return false;
	}

	if (!document.isObject())
	{
		qWarning() << "Manifest is incorrect format!";
		return false;
	}

	return fromJsonValue(document.object());
}

VRAction* VRManifest::getOrAddAction(const QString& actionSetName, const QString& actionName)
{
	return getOrAddActionSet(actionSetName)->getOrAddAction(actionName);
}
void VRManifest::removeAction(const QString& actionSetName, const QString& actionName)
{
	return getOrAddActionSet(actionSetName)->removeAction(actionName);
}

VRActionSet* VRManifest::getOrAddActionSet(const QString& actionSetName)
{
	for (VRActionSet* actionSet : m_actionSets)
	{
		if (actionSet->name() == actionSetName)
		{
			return actionSet;
		}
	}

	VRActionSet* actionSet = new VRActionSet(this);

	actionSet->m_name = actionSetName;

	m_actionSets.push_back(actionSet);

	connect(actionSet, &QObject::destroyed, [this](QObject* object)
	{
		int i = m_actionSets.indexOf(reinterpret_cast<VRActionSet*>(object));

		if (i != -1)
		{
			m_actionSets.removeAt(i);
		}
	});

	return actionSet;
}

void VRManifest::removeActionSet(const QString& actionSetName)
{
	for (auto it = m_actionSets.begin(); it < m_actionSets.end(); it++)
	{
		if ((*it)->name() == actionSetName)
		{
			(*it)->deleteLater();
			m_actionSets.erase(it);
			return;
		}
	}
}

VRBinding* VRManifest::getOrAddBinding(const QString& controllerType)
{
	for (VRBinding* binding : m_bindings)
	{
		if (binding->controllerType() == controllerType)
		{
			return binding;
		}
	}

	VRBinding* binding = new VRBinding(this);

	binding->m_controllerType = controllerType;

	m_bindings.push_back(binding);

	connect(binding, &QObject::destroyed, [this](QObject* object)
	{
		int i = m_bindings.indexOf(reinterpret_cast<VRBinding*>(object));

		if (i != -1)
		{
			m_bindings.removeAt(i);
		}
	});

	return binding;
}

void VRManifest::removeBinding(const QString& controllerType)
{
	for (auto it = m_bindings.begin(); it < m_bindings.end(); it++)
	{
		if ((*it)->controllerType() == controllerType)
		{
			(*it)->deleteLater();
			m_bindings.erase(it);
			return;
		}
	}
}

bool VRManifest::fromJsonValue(const QJsonValue& value)
{
	if (!value.isObject()) return false;

	QJsonObject object = value.toObject();

	QJsonArray actionsJson = object["actions"].toArray();
	for (QJsonValue value : actionsJson)
	{
		VRAction::FromJsonValue(value, this);
	}

	QJsonArray actionSetsJson = object["action_sets"].toArray();
	for (QJsonValue value : actionSetsJson)
	{
		VRActionSet::FromJsonValue(value, this);
	}

	QJsonArray bindingsJson = object["default_bindings"].toArray();
	for (QJsonValue value : bindingsJson)
	{
		VRBinding::FromJsonValue(value, this);
	}

	return true;
}

inline void insertLocalizationMap(QMap<QLocale::Language, QJsonObject>& bigMap, const QString& pathName, const QMap<QLocale::Language, QString>& smolMap)
{
	for (auto it = smolMap.begin(); it != smolMap.end(); it++)
	{
		QJsonObject obj;
		obj["language_tag"] = QLocale(it.key()).name();
		auto lmit = bigMap.insert(it.key(), obj);
		lmit.value()[pathName] = it.value();
	}
}

QJsonObject VRManifest::toJsonObject() const
{
	QJsonObject object;

	QMap<QLocale::Language, QJsonObject> localizationMap;

	QJsonArray actionsJson;
	QJsonArray actionSetsJson;
	for (VRActionSet* actionSet : m_actionSets)
	{
		QList<VRAction*> actions = actionSet->actions();
		for (VRAction* action : actions)
		{
			actionsJson.push_back(action->toJsonObject());
			insertLocalizationMap(localizationMap, action->path(), action->localeNames());
		}

		actionSetsJson.push_back(actionSet->toJsonObject());
		insertLocalizationMap(localizationMap, actionSet->path(), actionSet->localeNames());
	}

	QJsonArray localizationJson;
	for (auto it = localizationMap.begin(); it != localizationMap.end(); it++)
	{
		localizationJson.append(it.value());
	}

	QJsonArray bindingsJson;
	for (VRBinding* binding : m_bindings)
	{
		bindingsJson.push_back(binding->toJsonObject());
	}

	object["actions"] = actionsJson;
	object["action_sets"] = actionSetsJson;
	object["default_bindings"] = bindingsJson;
	object["localization"] = localizationJson;

	return object;
}
