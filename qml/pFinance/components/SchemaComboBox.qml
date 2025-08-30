import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ComboBox {
    id: combo
    property string columnName: ""                  // Name of column containing enumerated value
    property real preferredWidth: 300               // External control for width
    property var accessObject: null
    property int selectedValue: -1

    model: ListModel { id: internalModel }
    textRole: "label"
    valueRole: "value"

    Layout.preferredWidth: preferredWidth

    Component.onCompleted: {
        if (!accessObject || columnName === "")
            return;

        const map = accessObject.columnValues(columnName);
        internalModel.clear();

        for (const key in map) {
            internalModel.append({
                value: parseInt(key),
                label: map[key]
            });
        }

        for (let i = 0; i < internalModel.count; ++i) {
            if (internalModel.get(i).value === selectedValue) {
                combo.currentIndex = i;
                break;
            }
        }
    }

    onCurrentIndexChanged: {
        selectedValue = internalModel.get(currentIndex).value;
    }
}
