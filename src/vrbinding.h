#ifndef VRBINDING_H
#define VRBINDING_H

#include <QObject>

class VRManifest;

class VRBinding : public QObject
{
	Q_OBJECT
	VRBinding(QObject* parent = nullptr);
public:
	QString name() const;
	QString description() const;
	QString controllerType() const;
	QString bindingUrl() const;
signals:
	void nameChanged(const QString& name);
	void descriptionChanged(const QString& description);
	void controllerTypeChanged(const QString& name);
	void bindingUrlChanged(const QString& description);
public slots:
	void setName(const QString& name);
	void setDescription(const QString& description);
	void setControllerType(const QString& name);
	void setBindingUrl(const QString& description);
private:
	friend VRManifest;
	static void FromJsonValue(const QJsonValue& value, VRManifest* manifest);
	QJsonObject toJsonObject() const;

	QString m_name;
	QString m_description;
	QString m_controllerType;
	QString m_bindingUrl;
};

#endif // VRBINDING_H
