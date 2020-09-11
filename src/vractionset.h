#ifndef VRACTIONSET_H
#define VRACTIONSET_H

#include <QObject>
#include <QList>
#include <QLocale>
#include <QMap>
#include <QMutex>
#include <QJsonObject>

class VRAction;
class VRManifest;

class VRActionSet : public QObject
{
	Q_OBJECT
	VRActionSet(QObject* parent = nullptr);
public:
	enum class Usage : std::uint8_t
	{
		None,
		Both,
		Mirrored,
		Hidden
	};
	Q_ENUM(Usage)

	QString path() const;

	QString name() const;
	Usage usage() const;
	VRAction* getOrAddAction(const QString& name);
	void removeAction(const QString& name);
	QList<VRAction*> actions();
	QMap<QLocale::Language, QString>& localeNames();
signals:
	void usageChanged(const Usage& usage);
public slots:
	void setUsage(const Usage& usage);
private:
	friend VRManifest;
	static void FromJsonValue(const QJsonValue& value, VRManifest* manifest);
	QJsonObject toJsonObject() const;

	QString m_name;
	Usage m_usage;

	QList<VRAction*> m_actions;
	QMap<QLocale::Language, QString> m_localeNames;
};

#endif // VRACTIONSET_H
