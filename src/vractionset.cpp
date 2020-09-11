#include "vractionset.h"

#include <mutex>

#include <QSet>
#include <QJsonObject>
#include <QJsonArray>

#include "vraction.h"
#include "vrmanifest.h"

inline const char* toString(const VRActionSet::Usage& usage) noexcept
{
	switch (usage) {
	case VRActionSet::Usage::Both:
		return "leftright";
	case VRActionSet::Usage::Mirrored:
		return "single";
	case VRActionSet::Usage::Hidden:
		return "hidden";
	default:
		return "";
	}
}

inline VRActionSet::Usage toUsage(const QString& string)
{
	if (string == "leftright")
		return VRActionSet::Usage::Both;
	else if (string == "single")
		return VRActionSet::Usage::Mirrored;
	else if (string == "hidden")
		return VRActionSet::Usage::Hidden;
	else
		return VRActionSet::Usage::None;
}

VRActionSet::VRActionSet(QObject* parent)
	: QObject(parent)
	, m_name()
	, m_usage(Usage::None)
	, m_actions()
{
}

QString VRActionSet::path() const
{
	return QString("/actions/%1").arg(name());
}

QString VRActionSet::name() const
{
	return m_name;
}

VRActionSet::Usage VRActionSet::usage() const
{
	return m_usage;
}

void VRActionSet::setUsage(const VRActionSet::Usage& usage)
{
	if (m_usage != usage)
	{
		m_usage = usage;
		emit usageChanged(usage);
	}
}

VRAction* VRActionSet::getOrAddAction(const QString& name)
{
	for (VRAction* action : m_actions)
	{
		if (action->name() == name)
		{
			return action;
		}
	}

	VRAction* action = new VRAction(this);

	action->m_name = name;
	action->m_actionSet = this;

	m_actions.push_back(action);

	connect(action, &QObject::destroyed, [this](QObject* object)
	{
		int i = m_actions.indexOf(reinterpret_cast<VRAction*>(object));

		if (i != -1)
		{
			m_actions.removeAt(i);
		}
	});

	return action;
}

void VRActionSet::removeAction(const QString& name)
{
	for (auto it = m_actions.begin(); it < m_actions.end(); it++)
	{
		if ((*it)->name() == name)
		{
			(*it)->deleteLater();
			m_actions.erase(it);
			return;
		}
	}
}

QList<VRAction*> VRActionSet::actions()
{
	return m_actions;
}

QMap<QLocale::Language, QString>& VRActionSet::localeNames()
{
	return m_localeNames;
}

void VRActionSet::FromJsonValue(const QJsonValue& value, VRManifest* manifest)
{
	if (!value.isObject()) return;

	QJsonObject object = value.toObject();

	QString name = object["name"].toString();

	if (name.isEmpty()) return;

	QStringList list = name.split('/', QString::SkipEmptyParts);

	if (list.size() != 2) return;

	Usage usage = toUsage(object["usage"].toString());
	if (usage == Usage::None) return;

	VRActionSet* actionSet = manifest->getOrAddActionSet(list[1]);

	actionSet->setUsage(usage);
}

QJsonObject VRActionSet::toJsonObject() const
{
	QJsonObject object;

	object["name"]  = path();
	object["usage"] = toString(m_usage);

	return object;
}
