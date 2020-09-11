#ifndef VRACTION_H
#define VRACTION_H

#include <cstdint>

#include <QObject>
#include <QString>
#include <QLocale>
#include <QJsonObject>

class VRManifest;
class VRActionSet;

class VRAction : public QObject
{
	Q_OBJECT
	VRAction(QObject* parent = nullptr);
public:
	enum class ActionType : std::uint8_t
	{
		None,
		Boolean,
		Vector1,
		Vector2,
		Vector3,
		Pose,
		Skeleton,
		Vibration
	};
	Q_ENUM(ActionType)

	enum class ActionRequirement : std::uint8_t
	{
		None,
		Mandatory,
		Suggested,
		Optional
	};
	Q_ENUM(ActionRequirement)

	enum class Skeleton : std::uint8_t
	{
		None,
		HandLeft,
		HandRight
	};
	Q_ENUM(Skeleton)

	VRActionSet* actionSet() const;

	QString path() const;

	QString name() const;
	ActionType type() const;
	ActionRequirement requirement() const;
	Skeleton skeleton() const;
	QMap<QLocale::Language, QString>& localeNames();
signals:
	void actionSetChanged(VRActionSet* actionSet);
	void typeChanged(const ActionType& name);
	void requirementChanged(const ActionRequirement& name);
	void skeletonChanged(const Skeleton& name);
public slots:
	void setType(const ActionType& type);
	void setRequirement(const ActionRequirement& requirement);
	void setSkeleton(const Skeleton& skeleton);
private:
	friend VRManifest;
	friend VRActionSet;
	static void FromJsonValue(const QJsonValue& value, VRManifest* manifest);
	QJsonObject toJsonObject() const;

	QString m_name;
	ActionType m_type;
	ActionRequirement m_requirement;
	Skeleton m_skeleton;

	VRActionSet* m_actionSet;
	QMap<QLocale::Language, QString> m_localeNames;
};

#endif // VRACTION_H
