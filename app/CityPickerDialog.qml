import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import edu.openweather 1.0;

Dialog {
    id: dlgCityLookup
    title: qsTr("Search for a place...")
    modal: true
    standardButtons: Dialog.Cancel
    width: appWindow.width - 30
    height: appWindow.height - 60
    anchors.centerIn: parent

    onOpened: {
        citySearch.forceActiveFocus();
    }

    ColumnLayout {
        anchors.fill: parent
        TextField {
            id: citySearch
            Layout.fillWidth: true
            clip: true
        }
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            ScrollIndicator.vertical: ScrollIndicator {}
            model: CitySearchModel {
                filter: citySearch.text
            }
            delegate: ItemDelegate {
                text: city + "/" + region + ", " + country
                onClicked: {
                    OpenWeather.addCity(city + "," + country);
                    dlgCityLookup.close();
                    citySearch.clear();
                }
            }
        }
    }
}

