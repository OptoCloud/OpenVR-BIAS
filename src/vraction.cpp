#include "vraction.h"

#include "vrmanifest.h"
#include "vractionset.h"

inline const char* toString(const VRAction::ActionType& type) noexcept
{
	switch (type) {
	case VRAction::ActionType::Boolean:
		return "boolean";
	case VRAction::ActionType::Vector1:
		return "vector1";
	case VRAction::ActionType::Vector2:
		return "vector2";
	case VRAction::ActionType::Vector3:
		return "vector3";
	case VRAction::ActionType::Pose:
		return "pose";
	case VRAction::ActionType::Skeleton:
		return "skeleton";
	case VRAction::ActionType::Vibration:
		return "vibration";
	default:
		return "";
	}
}
inline const char* toString(const VRAction::ActionRequirement& requirement) noexcept
{
	switch (requirement) {
	case VRAction::ActionRequirement::Mandatory:
		return "mandatory";
	case VRAction::ActionRequirement::Suggested:
		return "suggested";
	case VRAction::ActionRequirement::Optional:
		return "optional";
	default:
		return "";
	}
}
inline const char* toString(const VRAction::Skeleton& skeleton) noexcept
{
	switch (skeleton) {
	case VRAction::Skeleton::HandLeft:
		return "/skeleton/hand/left";
	case VRAction::Skeleton::HandRight:
		return "/skeleton/hand/right";
	default:
		return "";
	}
}

inline VRAction::ActionType toActionType(const QString& string)
{
	if (string == "boolean")
		return VRAction::ActionType::Boolean;
	else if (string == "vector1")
		return VRAction::ActionType::Vector1;
	else if (string == "vector2")
		return VRAction::ActionType::Vector2;
	else if (string == "vector3")
		return VRAction::ActionType::Vector3;
	else if (string == "pose")
		return VRAction::ActionType::Pose;
	else if (string == "skeleton")
		return VRAction::ActionType::Skeleton;
	else if (string == "vibration")
		return VRAction::ActionType::Vibration;
	else
		return VRAction::ActionType::None;
}
inline VRAction::ActionRequirement toActionRequirement(const QString& string)
{
	if (string == "mandatory")
		return VRAction::ActionRequirement::Mandatory;
	else if (string == "suggested")
		return VRAction::ActionRequirement::Suggested;
	else if (string == "optional")
		return VRAction::ActionRequirement::Optional;
	else
		return VRAction::ActionRequirement::None;
}
inline VRAction::Skeleton toSkeleton(const QString& string)
{
	if (string == "/skeleton/hand/left")
		return VRAction::Skeleton::HandLeft;
	else if (string == "/skeleton/hand/right")
		return VRAction::Skeleton::HandRight;
	else
		return VRAction::Skeleton::None;
}


VRAction::VRAction(QObject* parent)
	: QObject(parent)
	, m_name()
	, m_type(ActionType::None)
	, m_requirement(ActionRequirement::Suggested)
	, m_skeleton(Skeleton::None)
	, m_actionSet(nullptr)
{
}

VRActionSet* VRAction::actionSet() const
{
	return m_actionSet;
}

QString VRAction::path() const
{
	return QString("%1/%2/%3")
			.arg(m_actionSet->path())
			.arg((m_type == ActionType::Vibration) ? "out" : "in")
			.arg(m_name);actionSet()->path();
}

QString VRAction::name() const
{
	return m_name;
}

VRAction::ActionType VRAction::type() const
{
	return m_type;
}

void VRAction::setType(const VRAction::ActionType& type)
{
	if (m_type != type)
	{
		m_type = type;
		emit typeChanged(type);
	}
}

VRAction::ActionRequirement VRAction::requirement() const
{
	return m_requirement;
}

void VRAction::setRequirement(const VRAction::ActionRequirement& requirement)
{
	if (m_requirement != requirement)
	{
		m_requirement = requirement;
		emit requirementChanged(requirement);
	}
}

VRAction::Skeleton VRAction::skeleton() const
{
	return m_skeleton;
}

void VRAction::setSkeleton(const VRAction::Skeleton& skeleton)
{
	if (m_skeleton != skeleton)
	{
		m_skeleton = skeleton;
		emit skeletonChanged(skeleton);
	}
}

QMap<QLocale::Language, QString>& VRAction::localeNames()
{
	return m_localeNames;
}

void VRAction::FromJsonValue(const QJsonValue& value, VRManifest* manifest)
{
	if (!value.isObject()) return;

	QJsonObject object = value.toObject();

	QString name = object["name"].toString();

	if (name.isEmpty()) return;

	QStringList list = name.split('/', QString::SkipEmptyParts);

	if (list.size() != 4) return;

	VRAction* action = manifest->getOrAddActionSet(list[1])->getOrAddAction(list[3]);

	action->setType(toActionType(object["type"].toString()));

	if (object.contains("requirement"))
	{
		action->setRequirement(toActionRequirement(object["requirement"].toString()));
	}

	action->setSkeleton(toSkeleton(object["skeleton"].toString()));
}

QJsonObject VRAction::toJsonObject() const
{
	QJsonObject object;

	object["name"] = path();

	object["requirement"] = toString(m_requirement);
	object["type"] = toString(m_type);

	if (m_type == ActionType::Skeleton)
	{
		object["skeleton"] = toString(m_skeleton);
	}

	return object;
}
