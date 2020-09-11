#include "vrbinding.h"

#include <QJsonObject>
#include <QJsonArray>

#include "vrmanifest.h"

VRBinding::VRBinding(QObject* parent)
	: QObject(parent)
{

}

QString VRBinding::name() const
{
	return m_name;
}

void VRBinding::setName(const QString& name)
{
	if (m_name != name)
	{
		m_name = name;
		emit nameChanged(name);
	}
}

QString VRBinding::description() const
{
	return m_description;
}

void VRBinding::setDescription(const QString& description)
{
	if (m_description != description)
	{
		m_description = description;
		emit descriptionChanged(description);
	}
}

QString VRBinding::controllerType() const
{
	return m_controllerType;
}

void VRBinding::setControllerType(const QString& controllerType)
{
	if (m_controllerType != controllerType)
	{
		m_controllerType = controllerType;
		emit controllerTypeChanged(controllerType);
	}
}

QString VRBinding::bindingUrl() const
{
	return m_bindingUrl;
}

void VRBinding::setBindingUrl(const QString& bindingUrl)
{
	if (m_bindingUrl != bindingUrl)
	{
		m_bindingUrl = bindingUrl;
		emit bindingUrlChanged(bindingUrl);
	}
}

void VRBinding::FromJsonValue(const QJsonValue& value, VRManifest* manifest)
{
	if (!value.isObject()) return;

	QJsonObject object = value.toObject();

	QString controllerType = object["controller_type"].toString();
	QString bindingUrl = object["binding_url"].toString();

	if (controllerType.isNull() || bindingUrl.isNull()) return;

	VRBinding* binding = manifest->getOrAddBinding(controllerType);

	binding->setDescription(bindingUrl);
}

QJsonObject VRBinding::toJsonObject() const
{
	QJsonObject object;

	object["controller_type"] = m_controllerType;
	object["binding_url"] = m_bindingUrl;

	return object;
}
