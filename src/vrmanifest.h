#ifndef VRMANIFEST_H
#define VRMANIFEST_H

#include <QObject>
#include <QList>
#include <QLocale>
#include <QJsonObject>

class VRAction;
class VRActionSet;
class VRBinding;

class VRManifest : public QObject
{
	Q_OBJECT
public:
	VRManifest(QObject* parent = nullptr);

	bool Save(const QString& path);
	bool Load(const QString& path);

	VRAction* getOrAddAction(const QString& actionSetName, const QString& actionName);
	void removeAction(const QString& actionSetName, const QString& actionName);

	VRActionSet* getOrAddActionSet(const QString& actionSetName);
	void removeActionSet(const QString& actionSetName);

	VRBinding* getOrAddBinding(const QString& controllerType);
	void removeBinding(const QString& controllerType);
private:
	bool fromJsonValue(const QJsonValue& json);
	QJsonObject toJsonObject() const;

	QList<VRActionSet*> m_actionSets;
	QList<VRBinding*> m_bindings;
	QMap<QLocale::Language, QMap<QString, QString>> m_localeMap;
};

#endif // VRMANIFEST_H
