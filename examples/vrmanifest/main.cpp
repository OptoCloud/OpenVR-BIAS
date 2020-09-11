#include <QMatrix4x4>
#include <QVector4D>
#include <QVector3D>
#include <QVector2D>
#include <QtSql>

#include <QDebug>

#include <QJsonObject>

#include <QStringList>
#include <QString>

#include "vraction.h"
#include "vractionset.h"
#include "vrmanifest.h"
#include "vrbinding.h"

int main()
{
	VRManifest manifest;

	manifest.Load("actions.json");

	VRActionSet* driving = manifest.getOrAddActionSet("driving");
	driving->setUsage(VRActionSet::Usage::Both);
	driving->localeNames().insert(QLocale::English, "driving bindings");
	driving->localeNames().insert(QLocale::NorwegianBokmal, "kjøring bindinger");


	VRAction* drivingHorn = driving->getOrAddAction("horn");
	drivingHorn->setType(VRAction::ActionType::Boolean);
	drivingHorn->setRequirement(VRAction::ActionRequirement::Optional);
	drivingHorn->localeNames().insert(QLocale::English, "activate horn");
	drivingHorn->localeNames().insert(QLocale::NorwegianBokmal, "aktiver tute");

	VRBinding* oculusBindings = manifest.getOrAddBinding("oculus bindings");
	oculusBindings->setBindingUrl("bindings_oculus.json");
	oculusBindings->setDescription("bindings for oculus controllers");
	oculusBindings->setControllerType("oculus_touch");
	oculusBindings->setName("uwu");

	manifest.Save("uwu.json");
}
