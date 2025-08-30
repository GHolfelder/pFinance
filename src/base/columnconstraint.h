#ifndef COLUMNCONSTRAINT_H
#define COLUMNCONSTRAINT_H

#include <QObject>
#include <QMap>
#include <QVariant>

enum class ConstraintType {                         // Column constraint types
    EnumSet,
    Range,
    Regex,
    Custom
};

struct ColumnConstraint {                           // Column constraint structure
    ConstraintType type;                            // Type of constraint
    virtual ~ColumnConstraint() = default;          // Constraint properties object
};

/**
 * @brief The enumeration constraint class
 */
struct EnumConstraint : public ColumnConstraint {
    QMap<int, QString> valueMap;                    // e.g. { 0: "Open", 1: "Marked", 2: "Reconciled" }

    /**
     * @brief Enumeration constraint class constructor
     */
    EnumConstraint() {
        type = ConstraintType::EnumSet;
    }

    /**
     * @brief Retrieve QVariantMap of values and their labels
     * @returns QVariantMap of values and labels
     */
    QVariantMap values() const {
        QVariantMap returnMap;

        for (auto it = valueMap.constBegin(); it != valueMap.constEnd(); ++it) {
            returnMap.insert(QString::number(it.key()), QVariant(it.value()));
        }
        return returnMap;
    }

    /**
     * @brief Retrieve label for enumerated value
     *
     * @param value Value of enumeration
     * @returns QString label associated with value or "Unknown"
     */
    QString labelFor(int value) const {
        return valueMap.value(value, QString("Unknown"));
    }

    /**
     * @brief Retrieve list of enumeration values
     *
     * @returns Integer list of values
     */
    QList<int> allowedValues() const {
        return valueMap.keys();
    }
};

#endif // COLUMNCONSTRAINT_H
